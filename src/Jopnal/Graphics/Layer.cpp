// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Layer)

        JOP_BIND_MEMBER_COMMAND(&Layer::removeDrawable, "removeDrawable");

    JOP_END_COMMAND_HANDLER(Layer)

    JOP_REGISTER_LOADABLE(jop, Layer)[](std::unique_ptr<Layer>& ptr, const json::Value& val) -> bool
    {
        const char* id = val.HasMember("id") && val["id"].IsString() ? val["id"].GetString() : "";

        ptr = std::make_unique<Layer>(id);

        const char* const rtexField = "rendertexture";
        if (val.HasMember(rtexField) && val[rtexField].IsArray() && val[rtexField].Size() >= 4)
        {
            auto& rt = val[rtexField];

            if (rt[0u].IsUint() && rt[1u].IsUint() && rt[2u].IsUint() && rt[3u].IsUint())
                ptr->setRenderTexture(glm::ivec2(rt[0u].GetInt(), rt[1u].GetInt()), rt[2u].GetUint(), rt[3u].GetUint());
            else
                JOP_DEBUG_WARNING("Encountered unexpected values while loading Layer's render texture with id \"" << ptr->getID() << "\"");
        }

        // Drawables will be resposible for binding themselves

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(Layer)

    JOP_REGISTER_SAVEABLE(jop, Layer)[](const Layer& layer, json::Value& val, json::Value::AllocatorType& alloc) -> bool
    {
        val.AddMember(json::StringRef("id"), json::StringRef(layer.getID().c_str()), alloc);
        
        if (layer.getRenderTexture())
        {
            auto& rt = *layer.getRenderTexture();
            auto& rtarr = val.AddMember(json::StringRef("rendertexture"), json::kArrayType, alloc)["rendertexture"];

            rtarr.PushBack(rt.getTexture().getWidth(), alloc)
                 .PushBack(rt.getTexture().getHeight(), alloc)
                 .PushBack(rt.getDepthBits(), alloc)
                 .PushBack(rt.getStencilBits(), alloc);
        }

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(Layer)
}

namespace jop
{
    Layer::Layer(const std::string& ID)
        : Subsystem                             (ID),
          m_drawList                            (),
          m_boundLayers                         (),
          m_camera                              (),
          m_renderTexture                       (),
          m_drawablesRemoved                    (false)
    {}

    Layer::~Layer()
    {
        if (!m_camera.expired())
            handleDrawableRemoval(*m_camera.lock());

        for (auto& i : m_drawList)
        {
            if (!i.expired())
                handleDrawableRemoval(*i.lock());
        }
    }

    //////////////////////////////////////////////

    void Layer::drawBase()
    {
        if (isActive())
        {
            if (m_camera.expired())
                setCamera(Camera::getDefault());

            if (m_renderTexture)
                m_renderTexture->bind();
            else
                RenderTexture::unbind();

            draw(*m_camera.lock());

            sweepRemoved();
        }
    }

    //////////////////////////////////////////////

    void Layer::draw(const Camera& camera)
    {
        GlState::setDepthTest(true);
        GlState::setFaceCull(true);

        for (auto& i : m_boundLayers)
        {
            if (!i.expired())
            {
                for (auto& j : i.lock()->m_drawList)
                {
                    if (!j.expired())
                        j.lock()->draw(camera);
                    else
                        m_drawablesRemoved = true;
                }
            }
        }

        for (auto& i : m_drawList)
        {
            if (!i.expired())
                i.lock()->draw(camera);
            else
                m_drawablesRemoved = true;
        }
    }

    //////////////////////////////////////////////

    Message::Result Layer::sendMessage(const std::string& message)
    {
        Any wrap;
        return sendMessage(message, wrap);
    }

    //////////////////////////////////////////////

    Message::Result Layer::sendMessage(const std::string& message, Any& returnWrap)
    {
        const Message msg(message, returnWrap);
        return sendMessage(msg);
    }

    //////////////////////////////////////////////

    Message::Result Layer::sendMessage(const Message& message)
    {
        if (message.passFilter(Message::Layer, getID()))
        {
            if (message.passFilter(Message::Command))
            {
                Any instance(this);
                if (JOP_EXECUTE_COMMAND(Layer, message.getString(), instance, message.getReturnWrapper()) == Message::Result::Escape)
                    return Message::Result::Escape;
            }

            if (message.passFilter(Message::Custom))
                return sendMessageImpl(message);
        }

        return Message::Result::Continue;
    }

    //////////////////////////////////////////////

    void Layer::addDrawable(Drawable& drawable)
    {
        if (typeid(drawable) == typeid(Camera) || typeid(drawable) == typeid(LightSource))
        {
            JOP_DEBUG_WARNING("You must not bind cameras or light sources with addDrawable. Camera/LightSource was not bound");
            return;
        }

        m_drawList.emplace_back(std::static_pointer_cast<Drawable>(drawable.shared_from_this()));
        handleDrawableAddition(*m_drawList.back().lock());
    }

    //////////////////////////////////////////////

    void Layer::removeDrawable(const std::string& id)
    {
        for (auto itr = m_drawList.begin(); itr != m_drawList.end(); ++itr)
        {
            if (!itr->expired() && itr->lock()->getID() == id)
            {
                handleDrawableRemoval(*itr->lock());
                m_drawList.erase(itr);
                return;
            }
        }
    }

    //////////////////////////////////////////////

    void Layer::bindOtherLayer(Layer& layer)
    {
        m_boundLayers.emplace_back(std::static_pointer_cast<Layer>(layer.shared_from_this()));
    }

    //////////////////////////////////////////////

    void Layer::unbindOtherLayer(const std::string& ID)
    {
        for (auto itr = m_boundLayers.begin(); itr != m_boundLayers.end(); ++itr)
        {
            if (!itr->expired() && itr->lock()->getID() == ID)
            {
                m_boundLayers.erase(itr);
                return;
            }
        }
    }

    //////////////////////////////////////////////

    void Layer::setCamera(const Camera& camera)
    {
        if (!m_camera.expired())
            handleDrawableRemoval(*m_camera.lock());

        m_camera = std::static_pointer_cast<const Camera>(camera.shared_from_this());
        handleDrawableAddition(*m_camera.lock());
    }

    //////////////////////////////////////////////

    void Layer::setRenderTexture(const glm::ivec2& size, const unsigned int depth, const unsigned int stencil)
    {
        m_renderTexture = std::make_unique<RenderTexture>(size, depth, stencil);
    }

    //////////////////////////////////////////////

    const RenderTexture* Layer::getRenderTexture() const
    {
        return m_renderTexture.get();
    }

    //////////////////////////////////////////////

    void Layer::sweepRemoved()
    {
        if (m_drawablesRemoved)
        {
            m_drawList.erase(std::remove_if(m_drawList.begin(), m_drawList.end(), [](const std::weak_ptr<Drawable>& drawable)
            {
                return drawable.expired();

            }), m_drawList.end());

            m_boundLayers.erase(std::remove_if(m_boundLayers.begin(), m_boundLayers.end(), [](const std::weak_ptr<Layer>& layer)
            {
                return layer.expired();

            }), m_boundLayers.end());

            m_drawablesRemoved = false;
        }
    }

    //////////////////////////////////////////////

    void Layer::handleDrawableAddition(const Drawable& drawable)
    {
        drawable.m_boundToLayers.emplace(this);
    }

    //////////////////////////////////////////////

    void Layer::handleDrawableRemoval(const Drawable& drawable)
    {
        drawable.m_boundToLayers.erase(this);
    }
}