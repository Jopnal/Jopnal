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
#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace jop
{
    ShaderProgram::ShaderProgram(const std::string& name)
        : Resource      (name),
          m_shaders     (),
          m_unifMap     (),
          m_programID   (0)
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
        if (!shader.getHandle())
            return false;
        
        m_shaders.emplace(shader.getType(), shader);

        return true;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::link()
    {
        unlink();
        // Link program
        glCheck(glUseProgram(m_programID));

        for (auto& i : m_shaders)
        {
            if (!i.second.expired())
            {
                glCheck(glAttachShader(m_programID, i.second->getHandle()));
            }
        }
        

        glLinkProgram(m_programID);

        // Check status
        if (checkStatus(GL_LINK_STATUS) != 0)
        {
            JOP_DEBUG_ERROR("Failed to link shader program:\n" << checkStatus(GL_LINK_STATUS));

            //We don't need the program anymore.
            unlink();
            
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
        for (auto& i : m_shaders)
        {
            glCheck(glDetachShader(m_programID, i.second->getHandle()));
        }

        return true;
    }

    //////////////////////////////////////////////

    void ShaderProgram::unlink()
    {
        glCheck(glDeleteProgram(m_programID));

        // Check status
        if(checkStatus(GL_DELETE_STATUS) != 0)
            JOP_DEBUG_ERROR("Failed to delete shader program:\n" << checkStatus(GL_DELETE_STATUS));
    }

    //////////////////////////////////////////////

    bool ShaderProgram::bind() const
    {
        if (m_programID)
        {
            glCheck(glUseProgram(m_programID));
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    void ShaderProgram::unbind() const
    {
        glCheck(glUseProgram(0));
    }

    bool ShaderProgram::validate()
    {
        if (checkStatus(GL_VALIDATE_STATUS) != 0)
        {
            JOP_DEBUG_ERROR("Failed to validate shader program:\n" << checkStatus(GL_VALIDATE_STATUS));
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix)
    {
        return setUniform(name, glm::value_ptr(matrix), 1);
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const float* matrices, const unsigned int amount)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniformMatrix4fv(loc, amount, GL_FALSE, matrices));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const glm::mat3& matrix)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix)));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const glm::vec2& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniform2f(loc, vector.x, vector.y));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const glm::vec3& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniform3f(loc, vector.x, vector.y, vector.z));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const glm::vec4& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniform4f(loc, vector.x, vector.y, vector.z, vector.w));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const jop::Texture& texture, const unsigned int unit)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            texture.bind(unit);
            glCheck(glUniform1i(loc, unit));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const float value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniform1f(loc, value));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const int value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniform1i(loc, value));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const unsigned int value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            glCheck(glUniform1ui(loc, value));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool ShaderProgram::setUniform(const std::string& name, const bool value)
    {
        return setUniform(name, value ? 1 : 0);
    }

    //////////////////////////////////////////////

    void ShaderProgram::setAttribute(const unsigned int loc, unsigned int type, int amount, unsigned int stride, const void* pointer)
    {
        GlState::setVertexAttribute(true, loc);
        glCheck(glVertexAttribPointer(loc, amount, type, GL_FALSE, stride, pointer));
    }

    //////////////////////////////////////////////

    ShaderProgram& ShaderProgram::getDefault()
    {

        static WeakReference<ShaderProgram> defShader;

        if (defShader.expired())
        {
            defShader = static_ref_cast<ShaderProgram>(ShaderAssembler::getShader(Material::Attribute::Default).getReference());

            JOP_ASSERT(defShader.get() != &getError(), "Failed to compile default shader!");
        }

        return *defShader;
    }

    //////////////////////////////////////////////

    ShaderProgram& ShaderProgram::getError()
    {
        static WeakReference<ShaderProgram> errProgram;

        if (errProgram.expired())
        {
            Shader vertex("");
            Shader fragment("");

            std::string vertSource(reinterpret_cast<const char*>(jopr::defaultShaderVert, sizeof(jopr::defaultShaderVert)));
            vertex.addSource(vertSource.c_str());

            std::string fragSource(reinterpret_cast<const char*>(jopr::defaultShaderFrag, sizeof(jopr::defaultShaderFrag)));
            fragment.addSource(fragSource.c_str());

            errProgram = static_ref_cast<ShaderProgram>(ResourceManager::getEmptyResource<ShaderProgram>("jop_default_shader_program").getReference());

            JOP_ASSERT_EVAL(errProgram->load(vertex, fragment), "Couldn't compile the default shader program!");

            errProgram->setPersistence(0);
        }

        return *errProgram;
    }

    //////////////////////////////////////////////

    int ShaderProgram::getUniformLocation(const std::string& name)
    {
        static const struct Callback : SettingCallback<bool>
        {
            const char* const str;
            bool err;
            Callback()
                : str("engine/Debug|bPrintShaderUniformLocationErrors"),
                err(SettingManager::get<bool>(str, true))
            {
                SettingManager::registerCallback(str, *this);
            }
            void valueChanged(const bool& value) override
            {
                err = value;
            }
        } cb;

        if (bind())
        {
            auto itr = m_unifMap.find(name);

            if (itr != m_unifMap.end())
                return itr->second;

            const int location = glCheck(glGetUniformLocation(m_programID, name.c_str()));

            if (location == -1)
            {
                if (cb.err)
                    JOP_DEBUG_WARNING("Uniform/attribute named \"" << name << "\" not found in shader \"" << getName() << "\"");
            }
            else
                m_unifMap[name] = location;

            return location;
        }

        return -1;
    }

    //////////////////////////////////////////////

    GLchar ShaderProgram::checkStatus(GLenum glStatus)
    {
        GLint status;
        glCheck(glGetProgramiv(m_programID, glStatus, &status));

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