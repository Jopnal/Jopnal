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

        JOP_BIND_MEMBER_COMMAND(&Layer::bindOtherLayer, "bindOtherLayer");
        JOP_BIND_MEMBER_COMMAND(&Layer::unbindOtherLayer, "unbindOtherLayer");
        JOP_BIND_MEMBER_COMMAND(&Layer::addDrawable, "addDrawable");
        JOP_BIND_MEMBER_COMMAND(&Layer::removeDrawable, "removeDrawable");
        JOP_BIND_MEMBER_COMMAND(&Layer::setRenderTexture, "setRenderTexture");
        JOP_BIND_MEMBER_COMMAND(&Layer::setCamera, "setCamera");
        JOP_BIND_MEMBER_COMMAND(&Layer::setActive, "setActive");
        JOP_BIND_MEMBER_COMMAND(&Layer::setID, "setID");

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

        // Drawables will be responsible for binding themselves

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
        : Subsystem             (ID),
          m_drawList            (),
          m_lights              (),
          m_boundLayers         (),
          m_camera              (),
          m_renderTexture       (),
          m_drawablesRemoved    (false)
    {}

    Layer::~Layer()
    {
        if (!m_camera.expired())
            handleDrawableRemoval(*m_camera);

        for (auto& i : m_lights)
        {
            if (!i.expired())
                handleDrawableRemoval(*i);
        }

        for (auto& i : m_drawList)
        {
            if (!i.expired())
                handleDrawableRemoval(*i);
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

            draw(*m_camera);

            sweepRemoved();
        }
    }

    //////////////////////////////////////////////

    void Layer::draw(const Camera& camera)
    {
        GlState::setDepthTest(true);
        GlState::setFaceCull(true);
        GlState::setPolygonMode(GlState::PolygonMode::Fill);

        LightContainer lights;

        for (auto& i : m_boundLayers)
        {
            if (!i.expired())
            {
                for (auto& j : i->m_drawList)
                {
                    selectLights(lights, *j);

                    if (!j.expired())
                    {
                        selectLights(lights, *j);
                        j->draw(camera, lights);
                    }
                    else
                        m_drawablesRemoved = true;
                }
            }
        }

        for (auto& i : m_drawList)
        {
            if (!i.expired())
            {
                selectLights(lights, *i);
                i->draw(camera, lights);
            }
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

        m_drawList.emplace_back(static_ref_cast<Drawable>(drawable.getReference()));
        handleDrawableAddition(*m_drawList.back());
    }

    //////////////////////////////////////////////

    void Layer::removeDrawable(const std::string& id)
    {
        for (auto itr = m_drawList.begin(); itr != m_drawList.end(); ++itr)
        {
            if (!itr->expired() && (*itr)->getID() == id)
            {
                handleDrawableRemoval(*(*itr));
                m_drawList.erase(itr);
                return;
            }
        }
    }

    //////////////////////////////////////////////

    void Layer::bindOtherLayer(Layer& layer)
    {
        m_boundLayers.emplace_back(static_ref_cast<Layer>(layer.getReference()));
    }

    //////////////////////////////////////////////

    void Layer::unbindOtherLayer(const std::string& ID)
    {
        for (auto itr = m_boundLayers.begin(); itr != m_boundLayers.end(); ++itr)
        {
            if (!itr->expired() && (*itr)->getID() == ID)
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
            handleDrawableRemoval(*m_camera);

        m_camera = static_ref_cast<const Camera>(camera.getReference());
        handleDrawableAddition(*m_camera);
    }

    //////////////////////////////////////////////

    void Layer::addLight(const LightSource& light)
    {
        m_lights.push_back(static_ref_cast<const LightSource>(light.getReference()));
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
            m_drawList.erase(std::remove_if(m_drawList.begin(), m_drawList.end(), [](const WeakReference<Drawable>& drawable)
            {
                return drawable.expired();

            }), m_drawList.end());

            m_boundLayers.erase(std::remove_if(m_boundLayers.begin(), m_boundLayers.end(), [](const WeakReference<Layer>& layer)
            {
                return layer.expired();

            }), m_boundLayers.end());

            m_drawablesRemoved = false;
        }
    }

    //////////////////////////////////////////////

    void Layer::selectLights(LightContainer& lights, const Drawable& drawable) const
    {
        auto selectContainer = [](const LightSource& l, LightContainer& cont) -> LightContainer::ContainerType*
        {
            auto& c = cont[l.getType()];

            if (c.size() < c.capacity())
                return &c;

            return nullptr;
        };

        if (drawable.receiveLights())
        {
            lights.clear();

            for (auto& i : m_lights)
            {
                if (i.expired())
                    continue;

                auto cont = selectContainer(*i, lights);

                if (!cont)
                    continue;

                if (i->getType() == LightSource::Type::Directional)
                    cont->push_back(i.get());
                else
                {
                    cont->push_back(i.get());
                }
            }
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