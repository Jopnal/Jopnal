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

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/FileLoader.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/ShaderAssembler.hpp>
    #include <queue>

#endif

#ifndef GL_GEOMETRY_SHADER
    #define GL_GEOMETRY_SHADER GL_GEOMETRY_SHADER_EXT
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
        destroy();
    }

    //////////////////////////////////////////////

    void Shader::destroy()
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

    bool Shader::compile(const Type type, const bool preprocess)
    {
        #ifdef JOP_OPENGL_ES

        if (type == Type::Geometry && (gl::getVersionMajor() < 3 || gl::getVersionMinor() < 2) && !JOP_CHECK_GL_EXTENSION(GL_EXT_geometry_shader))
        {
            JOP_DEBUG_ERROR("Geometry shaders are not supported on this platform");
            return false;
        }

        #endif

        if (m_handle)
        {
            glCheck(glDeleteShader(m_handle));
        }

        // Create shader & set handle
        m_handle = glCheck(glCreateShader(ns_shaderTypes[static_cast<int>(type)]));
        m_shaderType = type;

        if (preprocess)
        {
            std::string finalSource;

            // Add version & extension string to the start of the output code
            finalSource += Shader::getVersionString();
            finalSource += Shader::getExtensionString();

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
            std::queue<int32> lines;

            auto& deb = DebugHandler::getInstance();
            std::lock_guard<std::recursive_mutex> lock(deb.getMutex());

            {
                GLint logLen;
                glCheck(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &logLen));

                std::string log(logLen, '0');
                glCheck(glGetShaderInfoLog(m_handle, logLen, NULL, &log[0]));
                deb << "Failed to compile shader:\n\n" << log << "\n\n";

                // Find lines with errors
                const char* current = log.c_str();

                while (current)
                {
                    const char* next = std::strpbrk(current, "123456789");
                    const char* newLine = std::strchr(current, '\n');

                    if (!next || !newLine)
                        break;

                    if (next < newLine)
                    {
                        auto checkNum = [](const char num) -> bool
                        {
                            return num == '0' || num == '1' || num == '2' || num == '3' || num == '4' ||
                                   num == '5' || num == '6' || num == '7' || num == '8' || num == '9';
                        };

                        std::size_t len = 1;

                        while (checkNum(next[len]))
                            ++len;

                        lines.push(std::stoi(std::string(next, len)));
                    }

                    current = newLine + 1;
                }
            }

            if (!lines.empty())
            {
                GLint sourceLength;
                glCheck(glGetShaderiv(m_handle, GL_SHADER_SOURCE_LENGTH, &sourceLength));

                std::string source(sourceLength, '0');
                glCheck(glGetShaderSource(m_handle, sourceLength, NULL, &source[0]));

                auto inRange = [](const int32 offset, const int32 ref, const int32 value) -> bool
                {
                    return (value >= std::max(0, ref - offset)) && (value <= ref + offset);
                };

                const int32 offset = 3;

                const char* current = source.c_str();
                int currentLine = 0;

                int32 currentRef = lines.front();
                while (current && !lines.empty())
                {
                    ++currentLine;

                    if (current[0] == '\0')
                        break;

                    if (inRange(offset, currentRef, currentLine))
                    {
                        deb << " ";

                        if (currentLine == currentRef)
                            deb << "> ";
                        else
                            deb << "  ";

                        deb << currentLine << " " << std::string(current, std::strpbrk(current, "\n\0") - current) << 
                            
                        #ifdef JOP_OS_ANDROID
                            std::endl
                        #else
                            "\n"
                        #endif
                        ;

                        if (currentLine == currentRef + offset)
                        {
                            deb << "\n";
                            lines.pop();

                            if (!lines.empty())
                                currentRef = lines.front();
                        }
                    }

                    current = std::strchr(current, '\n');

                    if (current)
                        current += 1;
                }
            }

            JOP_DEBUG_ERROR("\n");

            destroy();

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

        return compile(type, preprocess);
    }

    //////////////////////////////////////////////
 
    Shader::Type Shader::getType() const
    {
        return m_shaderType;
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
            versionString += std::to_string(gl::getGLSLVersion());

        #ifdef JOP_OPENGL_ES
            versionString += (gl::getGLSLVersion() > 100 ? " es\n" : "\nprecision mediump float;\n");
        #else
            versionString += " core\n";
        #endif
        }

        return versionString;
    }

    //////////////////////////////////////////////

    const std::string& Shader::getExtensionString()
    {
        static std::string extString;

        if (extString.empty())
        {
        #ifdef JOP_OPENGL_ES
            if (gl::getGLSLVersion() < 300 && JOP_CHECK_GL_EXTENSION(NV_explicit_attrib_location))
                extString += "#extension GL_NV_explicit_attrib_location : enable\n";
        #endif
        }

        return extString;
    }
}