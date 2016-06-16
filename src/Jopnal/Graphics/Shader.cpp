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

	#include <Jopnal/Graphics/Shader.hpp>

    #include <Jopnal/Core/FileLoader.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/OpenGL.hpp>
    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/Graphics/GlCheck.hpp>
    #include <../tools/Jopresource/Resources.hpp>
    #include <glm/gtc/type_ptr.hpp>

    #ifdef JOP_OPENGL_ES

    #ifdef GL_GEOMETRY_SHADER_EXT
        #define GL_GEOMETRY_SHADER GL_GEOMETRY_SHADER_EXT
    #else
        #define GL_GEOMETRY_SHADER 0
    #endif

    #endif

#endif

//////////////////////////////////////////////


namespace
{
    static const int ns_shaderTypes[] =
    {
        GL_VERTEX_SHADER,
        GL_GEOMETRY_SHADER,
        GL_FRAGMENT_SHADER
    };

    unsigned int ns_boundProgram = 0;
}

namespace jop
{
    Shader::Shader(const std::string& name)
        : Resource          (name),
          m_strings         (),
          m_unifMap         (),
          m_shaderProgram   (0)
    {}

    Shader::~Shader()
    {
        destroy();
    }

    //////////////////////////////////////////////

    bool Shader::load(const std::string& vert, const std::string& geom, const std::string& frag, const std::string& pp)
    {
        static const int shaderAmount = sizeof(ns_shaderTypes) / sizeof(ns_shaderTypes[0]);

        destroy();
        m_shaderProgram = glCheck(glCreateProgram());

        unsigned int shaderHandles[shaderAmount] = {0u, 0u, 0u};
        auto deleteHandles = [](const unsigned int* handles, const unsigned int program)
        {
            for (std::size_t i = 0; i < shaderAmount; ++i)
            {
                if (handles[i])
                {
                    if (program)
                        glCheck(glDetachShader(handles[i], program));

                    glCheck(glDeleteShader(handles[i]));
                }
            }
        };

        auto handleShaderInfo = [](const unsigned int handle, const unsigned int shaderType) -> bool
        {
            GLint success;
            glCheck(glGetShaderiv(handle, GL_COMPILE_STATUS, &success));

            if (success == GL_FALSE)
            {
                char log[1024];
                glCheck(glGetShaderInfoLog(handle, sizeof(log), NULL, log));

                JOP_DEBUG_ERROR("Failed to compile " << (shaderType == 0 ? "vertex" : (shaderType == 1 ? "geometry" : "fragment")) << " shader:\n" << log);

            #if JOP_CONSOLE_VERBOSITY < 0
                shaderType;
            #endif
            }

        #ifdef JOP_DEBUG_MODE

            else
            {
                GLint len;
                glCheck(glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len));

                if (len > 0)
                {
                    char log[1024];
                    glCheck(glGetShaderInfoLog(handle, sizeof(log), NULL, log));

                    if (std::strcmp(log, "No errors.") != 0 && std::strlen(log) > 0)
                        JOP_DEBUG_WARNING((shaderType == 0 ? "Vertex" : (shaderType == 1 ? "Geometry" : "Fragment")) << " shader compilation produced warnings:\n" << log);
                }
            }

        #endif

            return success == GL_TRUE;
        };
        auto handleProgramInfo = [](const unsigned int program) -> bool
        {
            GLint success;
            glCheck(glGetProgramiv(program, GL_LINK_STATUS, &success));

            if (success == GL_FALSE)
            {
                char log[1024];
                glCheck(glGetProgramInfoLog(program, sizeof(log), NULL, log));

                JOP_DEBUG_ERROR("Failed to link shader program:\n" << log);

            }

        #ifdef JOP_DEBUG_MODE

            else
            {
                GLint len;
                glCheck(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len));

                if (len > 0)
                {
                    char log[1024];
                    glCheck(glGetProgramInfoLog(program, sizeof(log), NULL, log));

                    if (std::strcmp(log, "No errors.") != 0 && std::strlen(log) > 0)
                        JOP_DEBUG_WARNING("Shader program linking produced warnings:\n" << log);
                }
            }

        #endif

            return success == GL_TRUE;
        };

        for (int i = 0; i < shaderAmount; i++)
        {
            const std::string& shaderStr = i == 0 ? vert : i == 1 ? geom : frag;

            if (shaderStr.empty())
                continue;

            // File loader error checks need to be disabled here to avoid console spam
            const bool previouslyEnabled = FileLoader::errorChecksEnabled();
            FileLoader::enableErrorChecks(false);

            std::string fileReadBuffer;
            const char* sources[] = {pp.c_str(), "\n", FileLoader::readTextfile(shaderStr, fileReadBuffer) ? reinterpret_cast<const char*>(fileReadBuffer.data()) : shaderStr.c_str()};

            FileLoader::enableErrorChecks(previouslyEnabled);

            shaderHandles[i] = glCheck(glCreateShader(ns_shaderTypes[i]));

            int sizes[] = {static_cast<int>(pp.length()), 1, static_cast<int>(fileReadBuffer.empty() ? shaderStr.length() : fileReadBuffer.size())};
            glCheck(glShaderSource(shaderHandles[i], 3, sources, sizes));
            glCheck(glCompileShader(shaderHandles[i]));

            if (!handleShaderInfo(shaderHandles[i], i))
            {
                deleteHandles(shaderHandles, 0);
                destroy();

                return false;
            }
            glCheck(glAttachShader(m_shaderProgram, shaderHandles[i]));
        }

        glCheck(glLinkProgram(m_shaderProgram));

        if (!handleProgramInfo(m_shaderProgram))
        {
            deleteHandles(shaderHandles, m_shaderProgram);
            destroy();

            return false;
        }

        deleteHandles(shaderHandles, 0);

        m_strings[0] = vert;
        m_strings[1] = geom;
        m_strings[2] = frag;
        m_strings[3] = pp;

        return true;
    }

    //////////////////////////////////////////////

    void Shader::destroy()
    {
        if (m_shaderProgram)
        {
            // Unbind the program if it's currently active
            if (ns_boundProgram == m_shaderProgram)
                unbind();

            glCheck(glDeleteProgram(m_shaderProgram));
        }

        m_unifMap.clear();
        m_shaderProgram = 0;
    }

    //////////////////////////////////////////////

    bool Shader::bind() const
    {
        if (m_shaderProgram)
        {
            if (ns_boundProgram != m_shaderProgram)
            {
                glCheck(glUseProgram(m_shaderProgram));
                ns_boundProgram = m_shaderProgram;
            }
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    void Shader::unbind()
    {
        if (ns_boundProgram != 0)
        {
            glCheck(glUseProgram(0));
            ns_boundProgram = 0;
        }
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::mat4& matrix)
    {
        return setUniform(name, glm::value_ptr(matrix), 1);
    }
    
    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const float* matrices, const unsigned int amount)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(glUniformMatrix4fv(loc, amount, GL_FALSE, matrices));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::mat3& matrix)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix)));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::vec2& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(glUniform2f(loc, vector.x, vector.y));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::vec3& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(glUniform3f(loc, vector.x, vector.y, vector.z));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::vec4& vector)
    {
        const int loc = getUniformLocation(name);
        
        if (loc != -1)
            glCheck(glUniform4f(loc, vector.x, vector.y, vector.z, vector.w));

        return loc != -1;
    }

    //////////////////////////////////////////////
    
    bool Shader::setUniform(const std::string& name, const jop::Texture& texture, const unsigned int unit)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            texture.bind(unit);
            glCheck(glUniform1i(loc, unit));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const float value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(glUniform1f(loc, value));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const int value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(glUniform1i(loc, value));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const unsigned int value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(glUniform1ui(loc, value));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const bool value)
    {
        return setUniform(name, value ? 1 : 0);
    }

    //////////////////////////////////////////////

    void Shader::setAttribute(const unsigned int loc, unsigned int type, int amount, unsigned int stride, const bool normalize, const void* pointer)
    {
        GlState::setVertexAttribute(true, loc);
        glCheck(glVertexAttribPointer(loc, amount, type, normalize, stride, pointer));
    }

    //////////////////////////////////////////////

    const std::string& Shader::getSource(const Type type) const
    {
        return m_strings[static_cast<int>(type)];
    }

    //////////////////////////////////////////////

    unsigned int Shader::getHandle() const
    {
        return m_shaderProgram;
    }
    
    //////////////////////////////////////////////

    bool Shader::validate() const
    {
        glCheck(glValidateProgram(m_shaderProgram));

        GLint valid;
        glCheck(glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &valid));

        if (valid == GL_FALSE)
        {
            GLint size;
            glCheck(glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &size));

            std::string log(size, '0');
            glCheck(glGetProgramInfoLog(m_shaderProgram, size, &size, &log[0]));

            JOP_DEBUG_ERROR("Shader validation failed: " << log);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    const std::string& Shader::getVersionString()
    {
        static std::string versionString;

        if (versionString.empty())
        {
            versionString += "#version ";

        #ifndef JOP_OPENGL_ES

            versionString += std::to_string(ogl_GetMajorVersion());
            versionString += std::to_string(ogl_GetMinorVersion());
            versionString += " core";

        #else

            const std::string esVersion(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
            versionString += *(esVersion.end() - 3);
            versionString += *(esVersion.end() - 1);
            versionString += " es\n#define JOP_OPENGL_ES";

        #endif
        }

        return versionString;
    }

    //////////////////////////////////////////////

    Shader& Shader::getDefault()
    {
        static WeakReference<Shader> defShader;

        if (defShader.expired())
        {
            defShader = static_ref_cast<Shader>(ShaderAssembler::getShader(Material::Attribute::Default).getReference());

            JOP_ASSERT(defShader.get() != &getError(), "Failed to compile default shader!");
        }

        return *defShader;
    }

    //////////////////////////////////////////////

    Shader& Shader::getError()
    {
        static WeakReference<Shader> errShader;

        if (errShader.expired())
        {
            errShader = static_ref_cast<Shader>(ResourceManager::getEmptyResource<Shader>("jop_shader_error").getReference());

            JOP_ASSERT_EVAL(errShader->load(std::string(reinterpret_cast<const char*>(jopr::defaultShaderVert), sizeof(jopr::defaultShaderVert)),
                                            "",
                                            std::string(reinterpret_cast<const char*>(jopr::defaultShaderFrag), sizeof(jopr::defaultShaderFrag)),
                                            Shader::getVersionString()),
                                            "Couldn't compile the default shader!");

            errShader->setPersistence(0);
        }

        return *errShader;
    }

    //////////////////////////////////////////////

    int Shader::getUniformLocation(const std::string& name)
    {
        static const struct Callback : SettingCallback<bool>
        {
            const char* const str;
            bool err;
            Callback()
                : str("engine@Debug|bPrintShaderUniformLocationErrors"),
                  err(SettingManager::get<bool>(str, true))
            {
                SettingManager::registerCallback(str, *this);
            }
            void valueChanged(const bool& value) override
            {
                err = value;
            }
        } cb;

        if (bind())
        {
            auto itr = m_unifMap.find(name);

            if (itr != m_unifMap.end())
                return itr->second;

            const int location = glCheck(glGetUniformLocation(m_shaderProgram, name.c_str()));

            if (location == -1)
            {
                if (cb.err)
                    JOP_DEBUG_WARNING("Uniform/attribute named \"" << name << "\" not found in shader \"" << getName() << "\"");
            }
            else
                m_unifMap[name] = location;

            return location;
        }

        return -1;
    }
}