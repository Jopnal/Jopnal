// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
    static const int shaderTypes[] =
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
        std::string shaderStr[3];
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

        unsigned int shaderHandles[3] = {0u, 0u, 0u};
        auto deleteHandles = [](const unsigned int* handles, const unsigned int program)
        {
            for (std::size_t i = 0; i < 3; ++i)
            {
                if (handles[i])
                {
                    if (program)
                        glCheck(gl::DetachShader(handles[i], program));

                    glCheck(gl::DeleteShader(handles[i]));
                }
            }
        };

        for (int i = 0; i < sizeof(shaderTypes) / sizeof(shaderTypes[0]); i++)
        {
            if (shaderStr[i].empty())
                continue;

            std::vector<unsigned char> buffer;
            FileLoader::read(shaderStr[i], buffer);

            if (!buffer.empty())
            {
                shaderHandles[i] = glCheck(gl::CreateShader(shaderTypes[i]));

                int size = buffer.size();
                glCheck(gl::ShaderSource(shaderHandles[i], 1, reinterpret_cast<const char**>(buffer.data()), &size));

                glCheck(gl::CompileShader(shaderHandles[i]));
                GLint compiled;
                glCheck(gl::GetShaderiv(shaderHandles[i], gl::COMPILE_STATUS, &compiled));

                if (compiled == gl::FALSE_)
                {
                    char log[1024];
                    glCheck(gl::GetShaderInfoLog(shaderHandles[i], sizeof(log), 0, log));

                    JOP_DEBUG_ERROR("Failed to compile " << (i == 0 ? "Vertex" : i == 1 ? "Geometry" : "Fragment") << " shader: "<< log);

                    deleteHandles(shaderHandles, 0);
                    destroy();

                    return false;
                }
                glCheck(gl::AttachShader(m_shaderProgram, shaderHandles[i]));
            }
        }

       glCheck(gl::LinkProgram(m_shaderProgram));

       GLint linked;
       glCheck(gl::GetProgramiv(m_shaderProgram, gl::LINK_STATUS, &linked));
       if (linked == gl::FALSE_)
       {
           char log[1024];
           glCheck(gl::GetProgramInfoLog(m_shaderProgram, sizeof(log), 0, log));

           JOP_DEBUG_ERROR("Failed to link shader: " << log);

           deleteHandles(shaderHandles, m_shaderProgram);
           destroy();

           return false;
       }

       return true;
    }

    //////////////////////////////////////////////

    void Shader::destroy()
    {
        if (m_shaderProgram)
        {
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
            glCheck(gl::Uniform4fv(loc, sizeof(glm::mat4) / sizeof(glm::mat4::value_type), &matrix[0][0]));
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::mat3& matrix)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform3fv(loc, sizeof(glm::mat3) / sizeof(glm::mat3::value_type), &matrix[0][0]));
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::vec4& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform4f(loc, vector.x, vector.y, vector.z, vector.w));
    }

    //////////////////////////////////////////////
    
    /*
    void Shader::setUniform(const std::string& name, const jop::Texture& )
    {
        int loc = getUniformLocation(name);
        gl::Uniform1ui(loc, );
    }
    */

    //////////////////////////////////////////////

    void Shader::setAttribute(const std::string& name, unsigned int type, int amount, unsigned int stride, const void* pointer)
    {
        const int loc = getAttributeLocation(name);

        if (loc != -1)
        {
            glCheck(gl::VertexAttribPointer(loc, amount, type, gl::FALSE_, stride, pointer));
            glCheck(gl::EnableVertexAttribArray(loc));
        }
    }

    //////////////////////////////////////////////

    int Shader::getUniformLocation(const std::string& name)
    {
        if (bind())
        {
            const int location = glCheck(gl::GetUniformLocation(m_shaderProgram, name.c_str()));

            // Avoid printing the error if there's no program
            if (location == -1 && m_shaderProgram)
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

            if (location == -1 && m_shaderProgram)
                JOP_DEBUG_WARNING("Attrubute named \"" << name << "\" not found in shader");

            return location;
        }

        return -1;
    }
}