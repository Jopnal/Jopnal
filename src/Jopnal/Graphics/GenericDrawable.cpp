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
    JOP_REGISTER_LOADABLE(jop, GenericDrawable)[](Object& obj, const Scene& scene, const json::Value& val) -> bool
    {
        return Drawable::loadStateBase(obj.createComponent<GenericDrawable>(scene.getRenderer()), scene, val);
    }
    JOP_END_LOADABLE_REGISTRATION(GenericDrawable)

    JOP_REGISTER_SAVEABLE(jop, GenericDrawable)[](const Component& comp, json::Value& val, json::Value::AllocatorType& alloc) -> bool
    {
        return Drawable::saveStateBase(static_cast<const GenericDrawable&>(comp), val, alloc);
    }
    JOP_END_SAVEABLE_REGISTRATION(GenericDrawable)
}

namespace jop
{
    GenericDrawable::GenericDrawable(Object& object, Renderer& renderer)
        : Drawable(object, renderer, "genericdrawable")
    {}

    GenericDrawable::GenericDrawable(const GenericDrawable& other, Object& newObj)
        : Drawable(other, newObj)
    {}

    //////////////////////////////////////////////

    void GenericDrawable::draw(const Camera* camera, const LightContainer& lights, Shader& shader) const
    {
        // Check that there's a mesh and a material
        if (!getModel().isValid())
            return;

        auto& s = shader;
        auto& mat = *getModel().getMaterial();
        auto& msh = *getModel().getMesh();

        auto& modelMat = getObject()->getMatrix();

        // Set common uniforms. If camera is null, it means that the perspective and view matrices
        // have already been set somewhere else
        if (camera)
        {
            s.setUniform("u_PMatrix", camera->getProjectionMatrix());
            s.setUniform("u_VMatrix", camera->getViewMatrix());
        }
        s.setUniform("u_MMatrix", modelMat);

        // Set vertex attributes
        msh.getVertexBuffer().bind();
        const auto stride = msh.getVertexSize();
        s.setAttribute(0, gl::FLOAT, 3, stride, false, (void*)Vertex::Position);
        s.setAttribute(1, gl::FLOAT, 2, stride, false, (void*)Vertex::TexCoords);

        if (mat.hasAttribute(Material::Attribute::__Lighting | Material::Attribute::EnvironmentMap))
        {
            s.setUniform("u_NMatrix", glm::transpose(glm::inverse(glm::mat3(modelMat))));
            s.setAttribute(2, gl::FLOAT, 3, stride, false, (void*)Vertex::Normal);

            // Set lights
            if (mat.hasAttribute(Material::Attribute::__Lighting))
                lights.sendToShader(s, *this);
        }

        // Set material
        mat.sendToShader(s, camera);

    #ifdef JOP_DEBUG_MODE

        static const bool validate = SettingManager::getBool("bValidateShaders", false);

        if (validate && !s.validate())
            return;

    #endif

        // Use indices if they exist
        if (msh.getElementAmount())
        {
            // Bind index buffer
            msh.getIndexBuffer().bind();

            // Finally draw
            glCheck(gl::DrawElements(gl::TRIANGLES, msh.getElementAmount(), msh.getElementEnum(), (void*)0));
        }
        else
        {
            glCheck(gl::DrawArrays(gl::TRIANGLES, 0, msh.getVertexAmount()));
        }
    }
}