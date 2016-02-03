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


namespace
{
    static const int ns_shaderTypes[] =
    {
        gl::VERTEX_SHADER,
        gl::GEOMETRY_SHADER,
        gl::FRAGMENT_SHADER
    };

    unsigned int ns_boundProgram = 0;
}

namespace jop
{
    Shader::Shader()
        : m_shaderProgram(0)
    {}

    Shader::~Shader()
    {
        destroy();
    }

    //////////////////////////////////////////////

    bool Shader::load(const std::string& shaders)
    {
        static const int shaderAmount = sizeof(ns_shaderTypes) / sizeof(ns_shaderTypes[0]);

        destroy();
        m_shaderProgram = glCheck(gl::CreateProgram());

        std::string shaderStr[shaderAmount];
        std::size_t firstPos = shaders.find("v;");

        if (firstPos != std::string::npos)
        {
            auto lastPos = shaders.find_first_of('|');
            shaderStr[0] = shaders.substr(firstPos + 2, lastPos == std::string::npos ? std::string::npos : lastPos - firstPos - 2);
        }

        if ((firstPos = shaders.find("g;")) != std::string::npos)
        {
            auto lastPos = shaders.find_first_of('|', firstPos);
            shaderStr[1] = shaders.substr(firstPos + 2, lastPos == std::string::npos ? std::string::npos : lastPos - firstPos - 2);
        }

        if ((firstPos = shaders.find("f;")) != std::string::npos)
            shaderStr[2] = shaders.substr(firstPos + 2);

        unsigned int shaderHandles[shaderAmount] = {0u, 0u, 0u};
        auto deleteHandles = [](const unsigned int* handles, const unsigned int program)
        {
            for (std::size_t i = 0; i < shaderAmount; ++i)
            {
                if (handles[i])
                {
                    if (program)
                        glCheck(gl::DetachShader(handles[i], program));

                    glCheck(gl::DeleteShader(handles[i]));
                }
            }
        };

        auto handleShaderInfo = [](const unsigned int handle, const unsigned int shaderType) -> bool
        {
            GLint success;
            glCheck(gl::GetShaderiv(handle, gl::COMPILE_STATUS, &success));

            if (success == gl::FALSE_)
            {
                char log[1024];
                glCheck(gl::GetShaderInfoLog(handle, sizeof(log), NULL, log));

                JOP_DEBUG_ERROR("Failed to compile " << (shaderType == 0 ? "vertex" : (shaderType == 1 ? "geometry" : "fragment")) << " shader:\n" << log);

            }

        #ifdef JOP_DEBUG_MODE

            else
            {
                GLint len;
                glCheck(gl::GetShaderiv(handle, gl::INFO_LOG_LENGTH, &len));

                if (len > 0)
                {
                    char log[1024];
                    glCheck(gl::GetShaderInfoLog(handle, sizeof(log), NULL, log));

                    if (std::strcmp(log, "No errors.") != 0)
                        JOP_DEBUG_WARNING((shaderType == 0 ? "Vertex" : (shaderType == 1 ? "Geometry" : "Fragment")) << " shader compilation produced warnings:\n" << log);
                }
            }

        #endif

            return success == gl::TRUE_;
        };
        auto handleProgramInfo = [](const unsigned int program) -> bool
        {
            GLint success;
            glCheck(gl::GetProgramiv(program, gl::LINK_STATUS, &success));

            if (success == gl::FALSE_)
            {
                char log[1024];
                glCheck(gl::GetProgramInfoLog(program, sizeof(log), NULL, log));

                JOP_DEBUG_ERROR("Failed to link shader program:\n" << log);

            }

        #ifdef JOP_DEBUG_MODE

            else
            {
                GLint len;
                glCheck(gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &len));

                if (len > 0)
                {
                    char log[1024];
                    glCheck(gl::GetProgramInfoLog(program, sizeof(log), NULL, log));

                    if (std::strcmp(log, "No errors.") != 0)
                        JOP_DEBUG_WARNING("Shader program linking produced warnings:\n" << log);
                }
            }

        #endif

            return success == gl::TRUE_;
        };

        for (int i = 0; i < shaderAmount; i++)
        {
            if (shaderStr[i].empty())
                continue;

            std::vector<unsigned char> buffer;
            FileLoader::read(shaderStr[i], buffer);

            if (!buffer.empty())
            {
                shaderHandles[i] = glCheck(gl::CreateShader(ns_shaderTypes[i]));

                int size[] = {buffer.size()};
                const char* source = reinterpret_cast<const char*>(buffer.data());
                glCheck(gl::ShaderSource(shaderHandles[i], 1, &source, size));
                glCheck(gl::CompileShader(shaderHandles[i]));

                if (!handleShaderInfo(shaderHandles[i], i))
                {
                    deleteHandles(shaderHandles, 0);
                    destroy();

                    return false;
                }
                glCheck(gl::AttachShader(m_shaderProgram, shaderHandles[i]));
            }
        }

        glCheck(gl::LinkProgram(m_shaderProgram));

        if (!handleProgramInfo(m_shaderProgram))
        {
            deleteHandles(shaderHandles, m_shaderProgram);
            destroy();

            return false;
        }

        deleteHandles(shaderHandles, 0);

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

            glCheck(gl::DeleteProgram(m_shaderProgram));
        }
        
        m_shaderProgram = 0;
    }

    //////////////////////////////////////////////

    bool Shader::bind() const
    {
        if (m_shaderProgram && ns_boundProgram != m_shaderProgram)
        {
            glCheck(gl::UseProgram(m_shaderProgram));
            ns_boundProgram = m_shaderProgram;
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    void Shader::unbind()
    {
        if (ns_boundProgram != 0)
        {
            glCheck(gl::UseProgram(0));
            ns_boundProgram = 0;
        }
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::mat4& matrix)
    {
        const int loc = getUniformLocation(name);
        
        if (loc != -1)
            glCheck(gl::UniformMatrix4fv(loc, 1, gl::FALSE_, &matrix[0][0]));
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::mat3& matrix)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::UniformMatrix3fv(loc, 1, gl::FALSE_, &matrix[0][0]));
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::vec3& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform3f(loc, vector.x, vector.y, vector.z));
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::vec4& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform4f(loc, vector.x, vector.y, vector.z, vector.w));
    }

    //////////////////////////////////////////////
    
    void Shader::setUniform(const std::string& name, const jop::Texture& texture, const unsigned int unit)
    {
        const int loc = getUniformLocation(name);

        texture.bind(unit);

        if (loc != -1)
            glCheck(gl::Uniform1i(loc, texture.getHandle()));
    }

    //////////////////////////////////////////////

    void Shader::setAttribute(const std::string& name, unsigned int type, int amount, unsigned int stride, const bool normalize, const void* pointer)
    {
        const int loc = getAttributeLocation(name);

        if (loc != -1)
        {
            glCheck(gl::VertexAttribPointer(loc, amount, type, normalize, stride, pointer));
            glCheck(gl::EnableVertexAttribArray(loc));
        }
    }

    //////////////////////////////////////////////

    int Shader::getUniformLocation(const std::string& name)
    {
        if (bind())
        {
            const int location = glCheck(gl::GetUniformLocation(m_shaderProgram, name.c_str()));

            if (location == -1)
                JOP_DEBUG_ERROR("Uniform named \"" << name << "\"not found in shader");

            return location;
        }

        return -1;
    }

    //////////////////////////////////////////////

    int Shader::getAttributeLocation(const std::string& name)
    {
        if (bind())
        {
            const int location = glCheck(gl::GetUniformLocation(m_shaderProgram, name.c_str()));

            if (location == -1)
                JOP_DEBUG_WARNING("Attrubute named \"" << name << "\" not found in shader");

            return location;
        }

        return -1;
    }
}