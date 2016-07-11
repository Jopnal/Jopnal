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
          m_shaderType      (),
          m_sources         (0),
          m_handle          (0)

    {}

    Shader::~Shader()
    {
        glCheck(glDeleteShader(m_handle));
        m_sources.clear();
        m_handle = 0;
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
        m_shaderType = type;

        if (preprocess)
        {
            std::string finalSource;
            ShaderAssembler::preprocess(m_sources, finalSource);

            const char* strs[] = {finalSource.c_str()};

            glCheck(glShaderSource(m_handle, 1, strs, NULL));
        }
        else
        {
            // Set source without preprocessing
            glCheck(glShaderSource(m_handle, m_sources.size(), m_sources.data(), NULL));
        }

        // Compile
        glCheck(glCompileShader(m_handle));

        // Check status
        GLint status;
        glCheck(glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status));

        if (status != GL_TRUE)
        {
            GLchar message[1024];
            glCheck(glGetShaderInfoLog(m_handle, sizeof(message), NULL, message));
            JOP_DEBUG_ERROR("Failed to compile shader:\n\n" << message);

            m_sources.clear();

            return false;
        }
       
       
    #ifdef JOP_DEBUG_MODE

        else
        {
            GLint len;
            glCheck(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &len));

            if (len > 0)
            {
                char log[1024];
                glCheck(glGetShaderInfoLog(m_handle, sizeof(log), NULL, log));

                if (std::strpbrk(log, "0123456789"))
                    JOP_DEBUG_WARNING((type == Type::Vertex ? "Vertex" : (type == Type::Geometry ? "Geometry" : "Fragment")) << " shader compilation produced warnings:\n" << log);
            }
        }

    #endif

        // Clear sources after shader is compiled
        m_sources.clear();

        return true;
    }

    //////////////////////////////////////////////

    bool Shader::load(const std::string& path, Type type, bool preprocess)
    {
        // File loader error checks need to be disabled here to avoid console spam
        const bool previouslyEnabled = FileLoader::errorChecksEnabled();
        FileLoader::enableErrorChecks(false);

        std::string fileReadBuffer;

        // Source from file or straight from string
        const char* sources[] = { FileLoader::readTextfile(path, fileReadBuffer) ? reinterpret_cast<const char*>(fileReadBuffer.data()) : path.c_str() };

        FileLoader::enableErrorChecks(previouslyEnabled);
        
        // Add sources
        addSource(*sources);
        // For each type compile
        bool success = compile(type, preprocess);

        return success;
    }

    //////////////////////////////////////////////
 
    unsigned int Shader::getType() const
    {
        return shaderTypes[static_cast<int>(m_shaderType)];
    }

    //////////////////////////////////////////////

    unsigned int Shader::getHandle() const
    {
        return m_handle;
    }

    //////////////////////////////////////////////

    const std::string& Shader::getVersionString()
    {
        static std::string versionString;

        if (versionString.empty())
        {
            versionString += "#version ";

            versionString += std::to_string(getOGLVersionMajor());
            versionString += std::to_string(getOGLVersionMinor());

        #ifdef JOP_OPENGL_ES
            versionString += "0 es\n#define JOP_OPENGL_ES\n";
        #else
            versionString += "0 core\n";
        #endif
        }

        return versionString;
    }
}