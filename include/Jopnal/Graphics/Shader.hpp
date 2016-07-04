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

#ifndef JOP_SHADER_HPP
#define JOP_SHADER_HPP

// Headers
#include <Jopnal/Jopnal.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <unordered_map>
#include <array>

//////////////////////////////////////////////


namespace jop
{

    class JOP_API Shader : public Resource
    {

    public:

        /// The shader type
        ///
        enum class Type
        {
            Vertex,
            Geometry,
            Fragment
        };

    public:

        /// \brief default constructor
        ///
        Shader(const std::string& name);

        /// \brief default destructor
        ///
        ~Shader() override;

        /// \brief Add source
        ///
        void addSource(const char* source);

        /// \brief Compile shader
        ///
        /// \param type Shader Type:: (Vertex / Geometry / Fragment)
        ///
        bool compile(const Type type, bool preprocess = true);

        /// \brief Load shader
        ///
        bool load(const std::string& path, Type type, bool preprocess = true);

        /// \brief Get shader type (Vertex / Geometry / Fragment)
        ///
        GLuint getType() const;

        /// \brief Get shader handle
        ///
        GLuint getHandle() const;

        /// \brief Get OpenGL version number
        ///
        static const std::string& getVersionString();

    private:

        /// \brief Check shader status
        ///
        /// \param pname Object parameter: GL_SHADER_TYPE, GL_DELETE_STATUS, GL_COMPILE_STATUS
        /// \return Returns message used for error messages
        ///
        GLchar checkStatus(GLenum pname);

        Type                        m_shaderType;
        std::vector<const char*>    m_sources;
        GLuint                      m_handle;

    };
}

#endif

/// \class Shader
/// \ingroup Graphics
///
/// 