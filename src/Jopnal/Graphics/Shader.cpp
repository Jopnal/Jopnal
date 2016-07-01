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
#include <Jopnal/Precompiled/Precompiled.hpp>

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
        m_shaderType        (),
        m_sources           (0),
        m_handle            (0)

    {}

    Shader::~Shader()
    {
        glCheck(glDeleteShader(m_handle));

        if(checkStatus(GL_DELETE_STATUS) != 0)
            JOP_DEBUG_ERROR("Failed to delete shader " << checkStatus(GL_DELETE_STATUS));
    }

    //////////////////////////////////////////////

    void Shader::addSource(const char* source)
    {
        // add source to m_sources
        // make sure that order of sources is correct
        m_sources.emplace_back(source);
    }

    //////////////////////////////////////////////

    bool Shader::compile(const Type type)
    {      
        if (type > Type::Fragment)
            return false;

        // Create shader & set handle
        m_handle = glCheck(glCreateShader(shaderTypes[static_cast<int>(type)]));
        // Set source
        glCheck(glShaderSource(m_handle, 1, m_sources.data(), NULL));
        // Compile
        glCheck(glCompileShader(m_handle));

        // Check status
        if(checkStatus(GL_COMPILE_STATUS) != 0)
        {
            JOP_DEBUG_ERROR("Failed to compile " << (type == 0 ? "vertex" : (type == 1 ? "geometry" : "fragment")) << " shader:\n" << checkStatus(GL_COMPILE_STATUS));
            
            // Compile failed - delete shader
            glCheck(glDeleteShader(m_handle));

            if (checkStatus(GL_DELETE_STATUS) != 0)
                JOP_DEBUG_ERROR("Failed to delete shader " << checkStatus(GL_DELETE_STATUS));

            return false;
        }
       
#ifdef JOP_DEBUG_MODE

        else if (checkStatus(GL_INFO_LOG_LENGTH) != 0)
        {
            if (std::strcmp(reinterpret_cast<const char*>(checkStatus(GL_INFO_LOG_LENGTH)), "No errors.") != 0)
                JOP_DEBUG_WARNING((type == 0 ? "Vertex" : (type == 1 ? "Geometry" : "Fragment")) << " shader compilation produced warnings:\n" << checkStatus(GL_INFO_LOG_LENGTH));
        }

#endif

        return true;
    }

    //////////////////////////////////////////////

    bool Shader::load(const std::string& path, Type type)
    {

        // File loader error checks need to be disabled here to avoid console spam
        const bool previouslyEnabled = FileLoader::errorChecksEnabled();
        FileLoader::enableErrorChecks(false);

        std::string fileReadBuffer;
        const char* sources[] = { FileLoader::readTextfile(path, fileReadBuffer) ? reinterpret_cast<const char*>(fileReadBuffer.data()) : path.c_str() };

        FileLoader::enableErrorChecks(previouslyEnabled);
        
        // Add sources
        addSource(*sources);
        // for each type compile
        compile(type);
        // clear sources after compiled
        m_sources.clear();
    }

    //////////////////////////////////////////////
 
    GLuint Shader::getType() const
    {
        return shaderTypes[static_cast<int>(m_shaderType)];
    }

    //////////////////////////////////////////////

    GLuint Shader::getHandle() const
    {
        return m_handle;
    }

    //////////////////////////////////////////////

    GLchar Shader::checkStatus(GLenum pname)
    {
        GLint status;
        glCheck(glGetShaderiv(m_handle, pname, &status));

        if (status != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message(1024);
            glCheck(glGetShaderInfoLog(m_handle, 1024, &log_length, &message));
            return message;
        }

        return 0;
    }

}