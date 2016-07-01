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


namespace jop
{
    ShaderProgram::ShaderProgram(const std::string& name)
        : Resource      (name),
          m_programID   (0),
          m_shaders     ()

    {
        m_programID = glCheck(glCreateProgram()); 

        if (m_programID == 0)
            JOP_DEBUG_ERROR("Failed to create shader program!");
    }

    ShaderProgram::~ShaderProgram()
    {
        unlink();
    }

    //////////////////////////////////////////////

    bool ShaderProgram::attachShader(const Shader& shader)
    {
        // Is shader valid?
        if (glIsShader(shader.getHandle()) != GL_TRUE)
            return false;
        
        m_shaders.emplace(shader.getType(), shader);

        // Check if a shader of this type already exists & remove it if so
        for (auto i : m_shaders)
        {
            if (i.first == shader.getType())
            {
                glCheck(glDetachShader(m_programID, i.second->getHandle()));
                //glDeleteShader(i.second->getHandle());
            }
        }
     
        // Attach new shader to program
        glCheck(glAttachShader(m_programID, shader.getHandle()));
        return true;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::link()
    {
        // Link program
        glCheck(glUseProgram(m_programID));

        // Check status
        if (checkStatus(GL_LINK_STATUS) != 0)
        {
            JOP_DEBUG_ERROR("Failed to link shader program:\n" << checkStatus(GL_LINK_STATUS));

            //We don't need the program anymore.
            glCheck(glDeleteProgram(m_programID));
            
            //Don't leak shaders either.
            for (auto& i : m_shaders)
                glCheck(glDeleteShader(i.second->getHandle()));

            return false;
        }

#ifdef JOP_DEBUG_MODE

        else if (checkStatus(GL_INFO_LOG_LENGTH) != 0)
        {
            if (std::strcmp(reinterpret_cast<const char*>(checkStatus(GL_INFO_LOG_LENGTH)), "No errors.") != 0)
                JOP_DEBUG_WARNING("Shader program linking produced warnings:\n" << checkStatus(GL_INFO_LOG_LENGTH));
        }

#endif

        // Detach shaders from program after linking
        for(auto& i : m_shaders)
            glCheck(glDetachShader(m_programID, i.second->getHandle()));

        return true;
    }

    //////////////////////////////////////////////

    void ShaderProgram::unlink()
    {
        glCheck(glDeleteProgram(m_programID));

        // Check status
        if(checkStatus(GL_DELETE_STATUS) != 0)
            JOP_DEBUG_ERROR("Failed to delete shader program:" << "\n" << checkStatus(GL_DELETE_STATUS));
    }

    //////////////////////////////////////////////

    GLchar ShaderProgram::checkStatus(GLenum pname)
    {
        GLint status;
        glCheck(glGetProgramiv(m_programID, pname, &status));

        if (status != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message(1024);
            glCheck(glGetProgramInfoLog(m_programID, 1024, &log_length, &message));
            return message;
        }

        return 0;
    }

}