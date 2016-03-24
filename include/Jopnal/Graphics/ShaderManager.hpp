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

//////////////////////////////////////////////


namespace jop
{
    class Shader;

    class JOP_API ShaderManager final : public Subsystem
    {
    public:

        /// \brief Default constructor
        ///
        /// Reads the uber shader from the resource dll
        ///
        ShaderManager();

        /// \brief Destructor
        ///
        ~ShaderManager() override;


        /// \brief Get a shader with the given attribute combination
        ///
        /// \param attributes The material attributes
        ///
        /// \return Reference to the shader
        ///
        static Shader& getShader(const Material::AttribType attributes);

        static void getPreprocessDef(const Material::AttribType attrib, std::string& str);

    private:

        static ShaderManager* m_instance;   ///< The single instance

        std::unordered_map<Material::AttribType, WeakReference<Shader>> m_shaders;  ///< Map with the shaders
        std::array<std::string, 3> m_uber;                                          ///< The uber shader sources

    };
}

#endif