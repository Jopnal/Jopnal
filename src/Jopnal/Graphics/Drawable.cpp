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
    Drawable::Drawable(Object& object, const std::string& ID)
        : Component (object, ID),
          m_model   (std::static_pointer_cast<Model>(Model::getDefault().shared_from_this())),
          m_shader  (std::static_pointer_cast<Shader>(Shader::getDefault().shared_from_this()))
    {}

    Drawable::~Drawable()
    {}

    //////////////////////////////////////////////

    void Drawable::setModel(const Model& model)
    {
        m_model = std::weak_ptr<const Model>(std::static_pointer_cast<const Model>(model.shared_from_this()));
    }

    //////////////////////////////////////////////

    void Drawable::setShader(Shader& shader)
    {
        m_shader = std::weak_ptr<Shader>(std::static_pointer_cast<Shader>(shader.shared_from_this()));
    }
}