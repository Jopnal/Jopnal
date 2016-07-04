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
        m_sources.emplace_back(source);
    }

    //////////////////////////////////////////////

    bool Shader::compile(const Type type, bool preprocess)
    {      
        // Create shader & set handle
        m_handle = glCheck(glCreateShader(shaderTypes[static_cast<int>(type)]));

        if (preprocess)
        {
            std::string finalSource;
            ShaderAssembler::preprocess(m_sources, finalSource);
            glCheck(glShaderSource(m_handle, 1, reinterpret_cast<const char* const*>(finalSource.c_str()), NULL));
        }
        else
        {
            // Set source
            glCheck(glShaderSource(m_handle, m_sources.size(), m_sources.data(), NULL));
        }
        // Compile
        glCheck(glCompileShader(m_handle));

        // Check status
        if(checkStatus(GL_COMPILE_STATUS) != 0)
        {
            JOP_DEBUG_ERROR("Failed to compile " << (type == Type::Vertex ? "vertex" : (type == Type::Geometry ? "geometry" : "fragment")) << " shader:\n" << checkStatus(GL_COMPILE_STATUS));
            
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
                JOP_DEBUG_WARNING((type == Type::Vertex ? "Vertex" : (type == Type::Geometry ? "Geometry" : "Fragment")) << " shader compilation produced warnings:\n" << checkStatus(GL_INFO_LOG_LENGTH));
        }

    #endif

        return true;
    }

    //////////////////////////////////////////////

    bool Shader::load(const std::string& path, Type type, bool preprocess)
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
        bool success = compile(type, preprocess);
        // clear sources after compiled
        m_sources.clear();

        return success;
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

    const std::string& Shader::getVersionString()
    {
        static std::string versionString;

        if (versionString.empty())
        {
            versionString += "#version ";

        #ifndef JOP_OPENGL_ES

            versionString += std::to_string(ogl_GetMajorVersion());
            versionString += std::to_string(ogl_GetMinorVersion());
            versionString += "0 core\n";

        #else

            const std::string esVersion(reinterpret_cast<const char*>(glGetString(GL_VERSION)));

            const std::size_t numPos = esVersion.find_first_of("0123456789");

            versionString += esVersion[numPos];
            versionString += esVersion[numPos + 2];
            versionString += "0 es\n#define JOP_OPENGL_ES\n";

        #endif
        }

        return versionString;
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