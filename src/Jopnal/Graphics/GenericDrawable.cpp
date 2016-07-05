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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/GenericDrawable.hpp>

    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/Shader.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    GenericDrawable::GenericDrawable(Object& object, Renderer& renderer)
        : Drawable(object, renderer, 0)
    {}

    GenericDrawable::GenericDrawable(const GenericDrawable& other, Object& newObj)
        : Drawable(other, newObj)
    {}

    //////////////////////////////////////////////

    void GenericDrawable::draw(const Camera* camera, const LightContainer& lights, ShaderProgram& shader) const
    {
        // Check that there's a mesh and a material
        if (!getModel().isValid())
            return;

        auto& s = shader;
        auto& mat = *getModel().getMaterial();
        auto& msh = *getModel().getMesh();

        auto& modelMat = getObject()->getTransform().getMatrix();

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
        s.setAttribute(0, GL_FLOAT, 3, stride, msh.getVertexOffset(Mesh::Position));
        s.setAttribute(1, GL_FLOAT, 2, stride, msh.getVertexOffset(Mesh::TexCoords));

        if (mat.hasAttribute(Material::Attribute::VertexColor) && msh.hasVertexComponent(Mesh::VertexComponent::Color))
            s.setAttribute(5, GL_FLOAT, 4, stride, msh.getVertexOffset(Mesh::Color));

        if (mat.hasAttribute(Material::Attribute::__Lighting | Material::Attribute::EnvironmentMap))
        {
            s.setUniform("u_NMatrix", glm::transpose(glm::inverse(glm::mat3(modelMat))));
            s.setAttribute(2, GL_FLOAT, 3, stride, msh.getVertexOffset(Mesh::Normal));

            // Set lights
            if (mat.hasAttribute(Material::Attribute::__Lighting))
                lights.sendToShader(s, *this);
        }

        // Set material
        mat.sendToShader(s, camera, getAlphaMultiplier());

    #ifdef JOP_DEBUG_MODE

        static const struct Callback : SettingCallback<bool>
        {
            const char* const str;
            bool validate;
            Callback()
                : str("engine@Debug|bValidateShaders"),
                  validate(SettingManager::get<bool>(str, false))
            {
                SettingManager::registerCallback(str, *this);
            }
            void valueChanged(const bool& value) override
            {
                validate = value;
            }
        } cb;

        if (cb.validate && !s.validate())
            return;

    #endif

        // Use indices if they exist
        if (msh.getElementAmount())
        {
            // Bind index buffer
            msh.getIndexBuffer().bind();

            // Finally draw
            glCheck(glDrawElements(GL_TRIANGLES, msh.getElementAmount(), msh.getElementEnum(), (void*)0));
        }
        else
        {
            glCheck(glDrawArrays(GL_TRIANGLES, 0, msh.getVertexAmount()));
        }
    }
}