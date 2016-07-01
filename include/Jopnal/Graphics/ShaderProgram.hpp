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

#ifndef JOP_SHADERPROGRAM_HPP
#define JOP_SHADERPROGRAM_HPP

// Headers
#include <Jopnal/Jopnal.hpp>
#include <Jopnal/Core/Resource.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API ShaderProgram : public Resource
    {
    public:
        /// \brief Constructor
        ///
        ShaderProgram(const std::string& name);

        /// \brief Destructor
        ///
        ~ShaderProgram();

        /// \brief Load shaders
        ///
        /// attaches a variable number of shaders to shader program
        ///
        template<typename ... Args>
        bool load(const Args&&... args);

        /// \brief Attach shader to program
        ///
        bool attachShader(const Shader& shader);

        /// \brief Link shader program
        ///
        bool link();

        /// \brief Unlink shader program
        ///
        void unlink();

    private:

        /// \brief Check program status
        ///
        /// \param pname Object parameter: GL_LINK_STATUS, GL_DELETE_STATUS, GL_VALIDATE_STATUS, GL_ATTACHED_SHADERS
        /// \return Returns message used for error messages
        ///
        GLchar checkStatus(GLenum pname);

        unsigned int m_programID;
        std::unordered_map<GLuint, WeakReference<const Shader>> m_shaders;

    };
#include <Jopnal/Graphics/Inl/ShaderProgram.inl>
}

#endif