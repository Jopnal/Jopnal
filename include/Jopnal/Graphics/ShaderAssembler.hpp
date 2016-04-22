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

#ifndef JOP_SHADERMANAGER_HPP
#define JOP_SHADERMANAGER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <Jopnal/Graphics/Material.hpp>
#include <unordered_map>
#include <mutex>

//////////////////////////////////////////////


namespace jop
{
    class Shader;

    class JOP_API ShaderAssembler final : public Subsystem
    {
    private:

        typedef std::unordered_map<Material::AttribType, WeakReference<Shader>> ShaderMap;

    public:

        /// \brief Default constructor
        ///
        /// Reads the uber shader from the resource dll
        ///
        ShaderAssembler();

        /// \brief Destructor
        ///
        ~ShaderAssembler() override;


        /// \brief Get a shader with the given attribute combination
        ///
        /// \param attributes The material attributes
        ///
        /// \return Reference to the shader
        ///
        static Shader& getShader(const Material::AttribType attributes);

        /// \brief Get a pre-processor shader string
        ///
        /// \param attrib Material attributes
        /// \param str The string to put the definitions into
        ///
        static void getPreprocessDef(const Material::AttribType attrib, std::string& str);

        /// \brief Get the shader map
        ///
        /// \return Reference to the internal shader map
        ///
        static const ShaderMap& getShaderMap();

    private:

        static ShaderAssembler* m_instance;   ///< The single instance

        ShaderMap m_shaders;                ///< Map with the shaders
        std::array<std::string, 3> m_uber;  ///< The uber shader sources
        std::recursive_mutex m_mutex;       ///< Mutex                                        
    };
}

#endif