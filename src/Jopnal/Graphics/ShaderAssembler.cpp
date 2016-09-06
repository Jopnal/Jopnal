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
    #include <Jopnal/Graphics/Drawable.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <cctype>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace
{
    template<typename T>
    inline std::size_t combinedHash(const T& first, const T& second)
    {
        std::hash<T> hasher;
        const std::size_t seed = hasher(first);

        return seed ^ (hasher(second) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }
}

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
        m_uber[2].assign(reinterpret_cast<const char*>(jopr::defaultUberShaderFrag), sizeof(jopr::defaultUberShaderFrag));

        // Load plugins
        addPlugins(std::string(reinterpret_cast<const char*>(jopr::compatibilityPlugins), sizeof(jopr::compatibilityPlugins)));
        addPlugins(std::string(reinterpret_cast<const char*>(jopr::lightingPlugins), sizeof(jopr::lightingPlugins)));
        addPlugins(std::string(reinterpret_cast<const char*>(jopr::structurePlugins), sizeof(jopr::structurePlugins)));
        addPlugins(std::string(reinterpret_cast<const char*>(jopr::shadowPlugins), sizeof(jopr::shadowPlugins)));
    }

    ShaderAssembler::~ShaderAssembler()
    {
        m_instance = nullptr;
    }

    //////////////////////////////////////////////

    ShaderProgram& ShaderAssembler::getShader(const uint64 materialAttribs, const uint64 drawableAttribs)
    {
        JOP_ASSERT(m_instance != nullptr, "Couldn't load shader, no ShaderAssembler instance!");

        auto& cont = m_instance->m_shaders;

        const std::size_t combinedAttribs = combinedHash(materialAttribs, drawableAttribs);

        {
            std::unique_lock<std::recursive_mutex> lock(m_instance->m_mutex);

            auto itr = cont.find(combinedAttribs);
            if (itr != cont.end() && !itr->second.expired())
                return *itr->second;
        }

        const auto& uber = m_instance->m_uber;
        const std::string shaderName = "jop_shader_" + std::to_string(combinedAttribs);

        std::string pp;
        Material::getShaderPreprocessorDef(materialAttribs, pp);
        Drawable::getShaderPreprocessorDef(drawableAttribs, pp);
        
        ShaderProgram* s = &ResourceManager::getNamed<ShaderProgram>(shaderName, pp, Shader::Type::Vertex, uber[0], Shader::Type::Geometry, uber[1], Shader::Type::Fragment, uber[2]);

        if (!ResourceManager::isError(*s))
        {
            s->setShouldSerialize(false);
            s->setPersistence(5);

            {
                std::unique_lock<std::recursive_mutex> lock(m_instance->m_mutex);

                cont[combinedAttribs] = static_ref_cast<ShaderProgram>(s->getReference());
            }

            // Needed so that different samplers don't all point to zero
            if ((materialAttribs & Material::Attribute::__Lighting) != 0)
            {
                static const int maxUnits = Texture::getMaxTextureUnits();

                for (std::size_t i = 0; i < LightSource::getMaximumLights(LightSource::Type::Point); ++i)
                    s->setUniform("u_PointLightShadowMaps[" + std::to_string(i) + "]", maxUnits - 1);
            }
        }

        return *s;
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

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

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

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);
        
        m_instance->m_plugins.erase(name);
    }

    //////////////////////////////////////////////

    void ShaderAssembler::clearPlugins()
    {
        if (!m_instance)
            return;

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        m_instance->m_plugins.clear();
    }

    //////////////////////////////////////////////

    void ShaderAssembler::preprocess(const std::vector<const char*>& input, std::string& output)
    {
        if (!m_instance)
            return;

        std::unordered_set<const char*> dupeSet;

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        preprocess(input, output, false, dupeSet);
    }

    //////////////////////////////////////////////

    void ShaderAssembler::setShaderSource(const Shader::Type type, const std::string& source)
    {
        if (!m_instance)
            return;

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        m_instance->m_uber[static_cast<int>(type)] = source;
    }

    //////////////////////////////////////////////

    void ShaderAssembler::preprocess(const std::vector<const char*>& input, std::string& output, const bool nested, std::unordered_set<const char*>& duplicateSet)
    {
        for (auto i : input)
        {
            // Search for #include in shader source
            const char* current = strstr(i, "#include");
            const char* next = i;

            bool wasDuplicate = false;

            while (current != NULL)
            {
                // Add everything before first #include
                if (!wasDuplicate)
                {
                    output.append(next, current);
                    wasDuplicate = false;
                }

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
                    {
                        if (duplicateSet.find(itr->first.c_str()) != duplicateSet.end())
                        {
                            wasDuplicate = true;
                            continue;
                        }

                        duplicateSet.insert(itr->first.c_str());

                        if (strstr(itr->second.c_str(), "#include"))
                        {
                            std::vector<const char*> nestedVec(1, itr->second.c_str());
                            preprocess(nestedVec, output, true, duplicateSet);

                            std::string stripped = itr->second;

                            while (auto pos = stripped.find("#include") != std::string::npos)
                                stripped.erase(pos, stripped.find_first_of('>', pos));

                            output.append(stripped);
                        }
                        else
                            output.append(itr->second);
                    }
                    else
                        JOP_DEBUG_WARNING("Shader plugin \"" << temp << "\" not found, or has invalid formatting");

                    next = closer + 1;
                }

                current = strstr(next, "#include");
            }

            // Add rest of the code
            if (!nested)
                output.append(next);
        }
    }

    //////////////////////////////////////////////

    ShaderAssembler* ShaderAssembler::m_instance = nullptr;
}