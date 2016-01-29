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
#include <Jopnal\Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
    static const int shaderTypes[] =
    {
        gl::VERTEX_SHADER,
        gl::GEOMETRY_SHADER,
        gl::FRAGMENT_SHADER
    };
}

namespace jop
{
    Shader::Shader()
        :m_shaderProgram(0)
    {}

    Shader::~Shader()
    {}

    //////////////////////////////////////////////

    bool Shader::load(const std::string& shaders)
    {

        "v;pathToVertexShader|f;pathToFragShader|g:opafjopdjksop";
        std::string shaderPaths = shaders;
       
        std::string vertPath, geomPath, fragPath;

        auto firstPos = shaderPaths.find("v;");

        if (firstPos != std::string::npos)
        {
            auto lastPos = shaderPaths.find_first_of('|');
            vertPath = shaderPaths.substr(firstPos + 2, lastPos == std::string::npos ? std::string::npos : lastPos - firstPos - 2);
            shaderPaths.erase(firstPos, lastPos + 1);
        }

        firstPos = shaderPaths.find("g;");

        if (firstPos != std::string::npos)
        {
            auto lastPos = shaderPaths.find_first_of('|');
            geomPath = shaderPaths.substr(firstPos + 2, lastPos == std::string::npos ? std::string::npos : lastPos - firstPos - 2);
            shaderPaths.erase(firstPos, lastPos);
        }

        firstPos = shaderPaths.find("f;");

        if (firstPos != std::string::npos)
        {
            fragPath = shaderPaths.substr(firstPos + 2);
        }

        std::string paths[3];
        paths[0] = vertPath;
        paths[1] = geomPath;
        paths[2] = fragPath;

        std::string shaderStrs[3];

       //shader loading from path file

       for (int i = 0; i < sizeof(shaderTypes)/sizeof(shaderTypes[0]); i++)
       {
           if (paths[i].empty())
               continue;

           unsigned int shaderHandle = glCheck(gl::CreateShader(shaderTypes[i]));

           std::vector<unsigned char> buffer;

           if (!paths[i].empty())
               FileLoader::read(paths[i], buffer);
           
           if (!buffer.empty())
           {
               const char* code = reinterpret_cast<const char*>(buffer.data());
               int size = buffer.size();
               gl::ShaderSource(shaderHandle, 1, &code, &size);

               gl::CompileShader(shaderHandle);
               GLint isCompiled;
               glCheck(gl::GetShaderiv(shaderHandle, gl::COMPILE_STATUS, &isCompiled));

               if (isCompiled == gl::FALSE_)
               {
                   char log[1024];
                   glCheck(gl::GetShaderInfoLog(shaderHandle, sizeof(log), 0, log));
                   JOP_DEBUG_ERROR("shader compiler failed while compiling" << (i == 0 ? "Vertex" : i == 1 ? "Geometry" : "Fragment") << " shader");

                   glCheck(gl::DetachShader(m_shaderProgram, shaderHandle));
                   glCheck(gl::DeleteShader(shaderHandle));

                   return false;
               }
               glCheck(gl::AttachShader(m_shaderProgram, shaderHandle));
           }
           glCheck(gl::LinkProgram(m_shaderProgram));

           glCheck(gl::DetachShader(m_shaderProgram, shaderHandle));
           glCheck(gl::DeleteShader(shaderHandle));

           GLint linked;
           glCheck(gl::GetProgramiv(m_shaderProgram, gl::LINK_STATUS, &linked));
           if (linked == gl::FALSE_)
           {
               char log[1024];
               glCheck(gl::GetProgramInfoLog(m_shaderProgram, sizeof(log), 0, log));

               JOP_DEBUG_ERROR("Shader link failed: " << log);

               destroy();

               return false;
           }
       }

       return true;
    }

    //////////////////////////////////////////////

    void Shader::destroy()
    {
        unbind();

        if (m_shaderProgram)
            glCheck(gl::DeleteProgram(m_shaderProgram));
        
        m_shaderProgram = 0;
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::mat4& matrix)
    {
        bind();
        int loc = getUniformLocation(name);
        gl::Uniform4fv(loc, 16, &matrix[0][0]);
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::mat3& matrix)
    {
        bind();
        int loc = getUniformLocation(name);
        gl::Uniform3fv(loc, 9, &matrix[0][0]);
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::vec4& vector)
    {
        bind();
        int loc = getUniformLocation(name);
        gl::Uniform4f(loc, vector.x, vector.y, vector.z, vector.w);
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
        bind();
        int loc = getAttributeLocation(name);
        gl::VertexAttribPointer(loc, amount, type, gl::FALSE_, stride, pointer);
        gl::EnableVertexAttribArray(loc);
    }
    //////////////////////////////////////////////

    void Shader::bind()const
    {
        if (m_shaderProgram)
            glCheck(gl::UseProgram(m_shaderProgram));
    }

    //////////////////////////////////////////////

    void Shader::unbind()const
    {
        glCheck(gl::UseProgram(0));
    }

    //////////////////////////////////////////////
    int Shader::getAttributeLocation(const std::string& name)
    {
        bind();
        int location = gl::GetUniformLocation(m_shaderProgram, name.c_str());
        if (location == -1)
            JOP_DEBUG_ERROR("Location not found in shader");
        return location;
    }

    //////////////////////////////////////////////

    int Shader::getUniformLocation(const std::string& name)
    {
        int location = gl::GetUniformLocation(m_shaderProgram, name.c_str());
        if (location == -1)
            JOP_DEBUG_ERROR("Location not found in shader");
        return location;
    }
}