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
        return Drawable::loadStateBase(*obj.createComponent<GenericDrawable>(""), scene, val);
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
    GenericDrawable::GenericDrawable(Object& object, const std::string& ID)
        : Drawable(object, ID)
    {}

    //////////////////////////////////////////////

    GenericDrawable* GenericDrawable::clone() const
    {
        return new GenericDrawable(*this);
    }

    //////////////////////////////////////////////

    void GenericDrawable::draw(const Camera& camera, const LightContainer& lights)
    {
        if (getShader().expired() || getModel().getMesh().expired())
            return;

        auto& s = *getShader();
        auto& mod = getModel();
        auto& msh = *mod.getMesh();

        auto& modelMat = getObject()->getMatrix();

        // Set common uniforms
        s.setUniform("u_PMatrix", camera.getProjectionMatrix());
        s.setUniform("u_VMatrix", camera.getViewMatrix());
        s.setUniform("u_MMatrix", modelMat);
        s.setUniform("u_NMatrix", glm::mat3(modelMat));

        // Set material
        mod.getMaterial().sendToShader(s);

        // Set lights
        lights.sendToShader(s);

        // Set vertex attributes
        msh.getVertexBuffer().bind();
        s.setAttribute(0, gl::FLOAT, 3, sizeof(Vertex), false, (void*)Vertex::Position);
        s.setAttribute(1, gl::FLOAT, 2, sizeof(Vertex), false, (void*)Vertex::TexCoords);
        s.setAttribute(2, gl::FLOAT, 3, sizeof(Vertex), false, (void*)Vertex::Normal);

        // Use indices if they exist
        if (mod.getElementAmount())
        {
            // Bind index buffer
            msh.getIndexBuffer().bind();

            // Finally draw
            glCheck(gl::DrawElements(gl::TRIANGLES, mod.getElementAmount(), gl::UNSIGNED_INT, (void*)0));
        }
        else
        {
            // Unbind the index buffer in case there is one bound
            Buffer::unbind(Buffer::BufferType::ElementArrayBuffer);

            glCheck(gl::DrawArrays(gl::TRIANGLES, 0, mod.getVertexAmount()));
        }
    }
}