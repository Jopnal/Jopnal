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


namespace jop
{
    ShaderProgram::ShaderProgram(const std::string& name)
        : Resource(name)
    {
        //m_programID = glCheck(glCreateProgram()); 
    }

    ShaderProgram::~ShaderProgram()
    {
        unlink();
    }

    //////////////////////////////////////////////

    bool ShaderProgram::attachShader(const Shader&)
    {
        // Check if a shader of this type already exists
        // if so - replace it



        //glCheck(glAttachShader(programID, shader));


    }

    //////////////////////////////////////////////

    bool ShaderProgram::link()
    {
        glCheck(glUseProgram(programID));

        Glint linked;
        glCheck(glGetProgramiv(programID, GL_LINK_STATUS, &linked));
        if (linked != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message(1024);
            glCheck(glGetProgramInfoLog(programID, 1024, &log_length, message));
            // Write error to log
            JOP_DEBUG_ERROR("Failed to link shader program:"<< "\n" << message);

            return false;
        }
    }

    //////////////////////////////////////////////

    void ShaderProgram::unlink()
    {
        //glCheck(glDeleteProgram(programID));

        GLint deleted;
        glCheck(glGetProgramInfoLog(programID, GL_DELETE_STATUS, &deleted));
        if (deleted != GL_TRUE)
        {

        }
    }

}