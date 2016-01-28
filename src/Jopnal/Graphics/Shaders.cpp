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

namespace jop
{
    Shader::Shader()
    {}

    Shader::~Shader()
    {}

    //////////////////////////////////////////////

    bool Shader::load(const std::string& shaders)
    {
       //"v;pathToVertexShader|g;pathToGeomShader|f;pathToFragShader";
       std::string placeHolder = shaders;
       std::size_t pos = shaders.find_first_of("|");
       std::string subStringVert = shaders.substr(0,pos);
       placeHolder.erase(0, pos);
       std::string subStringGeom = shaders.substr(0, pos);
       placeHolder.erase(0, pos);
       std::string subStringFrag = shaders.substr(0, pos);
       //ResourceManager::getResource<TextResource>(shaders);
       return true;
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::mat4& matrix)
    {
        int loc = getUniformLocation(name);
        gl::Uniform4fv(loc, 16, &matrix[0][0]);
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::mat3& matrix)
    {
        int loc = getUniformLocation(name);
        gl::Uniform3fv(loc, 9, &matrix[0][0]);
    }

    //////////////////////////////////////////////

    void Shader::setUniform(const std::string& name, const glm::vec4& vector)
    {
        int loc = getUniformLocation(name);
        gl::Uniform4f(loc, vector.x, vector.y, vector.z, vector.w);
    }

    //////////////////////////////////////////////
    
    /*
    void Shader::setUniform(const std::string& name, const jop::Texture&)
    {
        int loc = getUniformLocation(name);
        gl::Uniform1ui(loc, );
    }
    */

    //////////////////////////////////////////////

    int Shader::getUniformLocation(const std::string& name)
    {
        int location = gl::GetUniformLocation(m_shader, name.c_str);
        if (location == -1)
            JOP_DEBUG_ERROR("Location not found in shader");
        return location;
    }
}