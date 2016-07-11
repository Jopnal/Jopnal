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

    #include <Jopnal/Graphics/ShaderAssembler.hpp>

    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <cctype>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace jop
{
    ShaderAssembler::ShaderAssembler()
        : Subsystem (0),
          m_plugins (),
          m_shaders (),
          m_uber    (),
          m_mutex   ()
    {
        JOP_ASSERT(m_instance == nullptr, "There must only be one ShaderAssembler instance!");
        m_instance = this;

        m_uber[0].assign(reinterpret_cast<const char*>(jopr::defaultUberShaderVert), sizeof(jopr::defaultUberShaderVert));
        m_uber[1].assign(reinterpret_cast<const char*>(jopr::depthRecordShaderPointGeom), sizeof(jopr::depthRecordShaderPointGeom));
        m_uber[2].assign(reinterpret_cast<const char*>(jopr::defaultUberShaderFrag), sizeof(jopr::defaultUberShaderFrag));

        // Load plugins
        addPlugins(std::string(reinterpret_cast<const char*>(jopr::defaultPlugins), sizeof(jopr::defaultPlugins)));

    }

    ShaderAssembler::~ShaderAssembler()
    {
        m_instance = nullptr;
    }

    //////////////////////////////////////////////

    ShaderProgram& ShaderAssembler::getShader(const Material::AttribType attributes)
    {
        JOP_ASSERT(m_instance != nullptr, "Couldn't load shader, no ShaderAssembler instance!");

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        auto& cont = m_instance->m_shaders;

        auto itr = cont.find(attributes);
        if (itr != cont.end() && !itr->second.expired())
            return *itr->second;

        const auto& uber = m_instance->m_uber;
        const std::string shaderName = "jop_shader_" + std::to_string(attributes);

        if (ResourceManager::resourceExists<ShaderProgram>(shaderName))
            return ResourceManager::getExistingResource<ShaderProgram>(shaderName);

        
        std::string pp;
        getPreprocessDef(attributes, pp);
        ShaderProgram* s = nullptr;

        if ((attributes & Material::Attribute::__RecordEnv))
        {
            s = &ResourceManager::getNamedResource<ShaderProgram>(shaderName, pp, Shader::Type::Vertex, uber[0], Shader::Type::Geometry, uber[1], Shader::Type::Fragment, uber[2]);
        }
        else
        {
            s = &ResourceManager::getNamedResource<ShaderProgram>(shaderName, pp, Shader::Type::Vertex, uber[0], Shader::Type::Fragment, uber[2]);
        }

        if (s != &ShaderProgram::getError())
        {
            s->setShouldSerialize(false);

            cont[attributes] = static_ref_cast<ShaderProgram>(s->getReference());

            // Needed so that different samplers don't all point to zero
            if ((attributes & Material::Attribute::__Lighting) != 0)
            {
                static const int maxUnits = Texture::getMaxTextureUnits();

                for (std::size_t i = 0; i < LightSource::getMaximumLights(LightSource::Type::Point); ++i)
                    s->setUniform("u_PointLightShadowMaps[" + std::to_string(i) + "]", maxUnits - 1);
            }
        }
        else
            JOP_DEBUG_ERROR("Failed to load shader with attributes: " << attributes);

        return *s;
    }

    //////////////////////////////////////////////

    void ShaderAssembler::getPreprocessDef(const Material::AttribType attrib, std::string& str)
    {
        using m = Material::Attribute;

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        // Material
        if ((attrib & m::__Lighting) != 0)
            str += "#define JMAT_MATERIAL\n";
        
        // Diffuse map
        if ((attrib & m::DiffuseMap) != 0)
        {
            str += "#define JMAT_DIFFUSEMAP\n";

            if ((attrib & m::DiffuseAlpha) != 0)
                str += "#define JMAT_DIFFUSEALPHA\n";
        }

        // Specular map
        if ((attrib & m::SpecularMap) != 0)
            str += "#define JMAT_SPECULARMAP\n";

        // Emission map
        if ((attrib & m::EmissionMap) != 0)
            str += "#define JMAT_EMISSIONMAP\n";

        // Environment map
        if ((attrib & m::EnvironmentMap) != 0)
            str += "#define JMAT_ENVIRONMENTMAP\n";

        // Reflection map
        if ((attrib & m::ReflectionMap) != 0)
            str += "#define JMAT_REFLECTIONMAP\n";

        // Reflection map
        if ((attrib & m::OpacityMap) != 0)
            str += "#define JMAT_OPACITYMAP\n";

        // Gloss map
        if ((attrib & m::GlossMap) != 0)
            str += "#define JMAT_GLOSSMAP\n";

        // Lighting
        {
            static const std::string maxLights =
                "#define JMAT_MAX_POINT_LIGHTS "            + std::to_string(LightSource::getMaximumLights(LightSource::Type::Point)) +
                "\n#define JMAT_MAX_DIRECTIONAL_LIGHTS "    + std::to_string(LightSource::getMaximumLights(LightSource::Type::Directional)) +
                "\n#define JMAT_MAX_SPOT_LIGHTS "           + std::to_string(LightSource::getMaximumLights(LightSource::Type::Spot)) + "\n";

            // Phong model
            if ((attrib & m::Phong) != 0)
                str += "#define JMAT_PHONG\n" + maxLights;
        }

        // Environment map record
        if ((attrib & m::__RecordEnv) != 0)
            str += "#define JMAT_ENVIRONMENT_RECORD\n";

        // Skybox/sphere
        if ((attrib & m::__SkyBox) != 0)
            str += "#define JMAT_SKYBOX\n";
        else if ((attrib & m::__SkySphere) != 0)
            str += "#define JMAT_SKYSPHERE\n";
    }

    //////////////////////////////////////////////

    const ShaderAssembler::ShaderMap& ShaderAssembler::getShaderMap()
    {
        return m_instance->m_shaders;
    }

    //////////////////////////////////////////////

    void ShaderAssembler::addPlugin(const std::string& name, const std::string& source)
    {
        if (!m_instance)
            return;

    #ifdef JOP_DEBUG_MODE

        if (m_instance->m_plugins.find(name) != m_instance->m_plugins.end())
            JOP_DEBUG_WARNING("Shader plugin \"" << name << "\" overwritten")

    #endif

        m_instance->m_plugins.emplace(name, source);
    }

    //////////////////////////////////////////////

    void ShaderAssembler::addPlugins(const std::string& source)
    {
        if (!m_instance)
            return;

        // Parse string
        const char* const src = source.c_str();
        const char* first = strstr(src, "#plugin");
        const char* next = src;

        while (first)
        {
            // Move to the end of first #plugin
            first += 7;

            // Handle whitespace
            std::size_t whitespace = 0;
            while (std::isspace(first[0]))
            {
                ++whitespace;
                first += 1;
            }

            // Handle missing opener case
            if (first[0] != '<')
                next = first - whitespace;
            else
            {
                const char* opener = first;
                const char* closer = strchr(opener, '>');
                std::string temp(opener + 1, closer);
                const char* pluginEnd = strstr(first, "#pluginend");
                if (!pluginEnd)
                {
                    JOP_DEBUG_WARNING("Shader plugins not found or invalid formatting!");
                    return;
                }

                addPlugin(temp, std::string(closer + 1, pluginEnd));

                next = closer + 1;
            }
            first = strstr(next, "#plugin");
        }
    }

    //////////////////////////////////////////////

    void ShaderAssembler::removePlugin(const std::string& name)
    {
        if (!m_instance)
            return;
        
        m_instance->m_plugins.erase(name);
    }

    //////////////////////////////////////////////

    void ShaderAssembler::clearPlugins()
    {
        if (!m_instance)
            return;

        m_instance->m_plugins.clear();
    }

    //////////////////////////////////////////////

    void ShaderAssembler::preprocess(const std::vector<const char*>& input, std::string& output)
    {
        if (!m_instance)
            return;
        
        // Add version string to the start of the output code
        output += Shader::getVersionString();

        for (auto i : input)
        {
            // Search for #include in shader source
            const char* current = strstr(i, "#include");
            const char* next = i;
            
            while (current != NULL)
            {
                // Add everything before first #include
                output.append(next, current);

                // Move to the end of first #include
                current += 8;
                std::size_t whitespace = 0;
                while (std::isspace(current[0]))
                {
                    ++whitespace;
                    current += 1;
                }

                // Handle missing opener case
                if (current[0] != '<')
                    next = current - whitespace;
                else
                {
                    const char* opener = current;
                    const char* closer = strchr(opener, '>');
                    std::string temp(opener + 1, closer);
                    auto itr = m_instance->m_plugins.find(temp);
                    if (itr != m_instance->m_plugins.end())
                        output.append(itr->second);
                    else
                        JOP_DEBUG_WARNING("Shader plugin: \"" << temp << "\" not found, or invalid formatting!");

                    next = closer + 1;
                }
                current = strstr(next, "#include");

            }
            // Add rest of the code
            output.append(next);
        }
    }

    //////////////////////////////////////////////

    ShaderAssembler* ShaderAssembler::m_instance = nullptr;
}