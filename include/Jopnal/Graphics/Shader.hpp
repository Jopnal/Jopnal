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
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Shader : public Resource
    {
    public:

        /// Shader type
        ///
        enum class Type
        {
            Vertex,
            Geometry,
            Fragment
        };

    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        explicit Shader(const std::string& name);

        /// \brief Destructor
        ///
        ~Shader() override;


        /// \brief Destroy this shader
        ///
        void destroy();

        /// \brief Add shader source
        ///
        /// The string must stay in existence until after compile() is called.
        ///
        /// \param source The shader source
        ///
        void addSource(const char* source);

        /// \brief Compile shader
        ///
        /// \param type Shader type
        /// \param preprocess Should shader be preprocessed?
        ///
        /// \return True if successful
        ///
        bool compile(const Type type, const bool preprocess = true);

        /// \brief Load shader
        ///
        /// \param path Path to file containing shader or shaders source code
        /// \param type Shader type
        /// \param preprocess Should shader be preprocessed?
        ///
        /// \return True if successful
        ///
        bool load(const std::string& path, Type type, const bool preprocess = true);

        /// \brief Get the shader type
        ///
        /// \return The shader type
        ///
        Type getType() const;

        /// \brief Get the OpenGL handle
        ///
        /// \return The shader handle
        ///
        unsigned int getHandle() const;

        /// \brief Get the version definition string
        ///
        /// \return The version string
        ///
        static const std::string& getVersionString();

        /// \brief Get the extension definition string
        ///
        /// \return The extension string
        ///
        static const std::string& getExtensionString();

    private:

        Type m_shaderType;                  ///< Shaders type (Vertex / Geometry / Fragment)
        std::vector<const char*> m_sources; ///< Container holding shaders sources
        unsigned int m_handle;              ///< OpenGL handle for the shader
    };
}

/// \class jop::Shader
/// \ingroup graphics

#endif