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
    static const int shaderTypes[] =
    {
        GL_VERTEX_SHADER,
        GL_GEOMETRY_SHADER,
        GL_FRAGMENT_SHADER
    };

}

namespace jop
{
    Shader::Shader(const std::string& name)
        : Resource          (name),
        Type                m_shaderType,
        m_sources           ()

    {}

    Shader::~Shader()
    {
        //destroy();
        // glCheck(glDeleteShader(shader));
    }

    //////////////////////////////////////////////

    void Shader::addSource(const char* source)
    {
        // add source to m_sources
        // make sure that order of sources is correct

        // vert - geom - frag  

        // if sources change - compile again?
    }

    //////////////////////////////////////////////

    bool Shader::compile(const Type type)
    {      
        // Compile shader of given type
        
        GLuint shader = glCheck(glCreateShader(shaderTypes[type]))
        glCheck(glShaderSource(shader, 1, m_source[type], NULL));
        glCheck(glCompileShader(shader));

         GLint compiled;
         glCheck(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

         if (compiled != GL_TRUE)
         {
             GLsizei log_length = 0;
             GLchar message(1024);
             glCheck(glGetShaderInfoLog(shader, 1024, &log_length, message));
             // Write error to log
             JOP_DEBUG_ERROR("Failed to compile " << (shaderTypes == 0 ? "vertex" : (shaderTypes == 1 ? "geometry" : "fragment")) << " shader:\n" << message);

             return false;
         }
        
         return true;
    }

    //////////////////////////////////////////////

    bool Shader::load(const std::string& path)
    {
        // Check type


        // File loader error checks need to be disabled here to avoid console spam
        const bool previouslyEnabled = FileLoader::errorChecksEnabled();
        FileLoader::enableErrorChecks(false);

        std::string fileReadBuffer;
        const char* sources[] = { FileLoader::readTextfile(path, fileReadBuffer) ? reinterpret_cast<const char*>(fileReadBuffer.data()) : path.c_str() };

        FileLoader::enableErrorChecks(previouslyEnabled);

        
        //addsource(sources)

        // for each type compile

        // clear m_sources after done
    }


 
}