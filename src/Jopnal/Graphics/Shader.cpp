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
    JOP_REGISTER_LOADABLE(jop, Shader)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load Shader, no name found");
            return false;
        }

        if (!val.HasMember("shaders") || !val["shaders"].IsArray() || val["shaders"].Size() < 4)
        {
            JOP_DEBUG_ERROR("Couldn't load Shader, no shader paths or sources found");
            return false;
        }

        auto& arr = val["shaders"];

        ResourceManager::getNamedResource<Shader>(val["name"].GetString(),
                                                  arr[0u].IsString() ? arr[0u].GetString() : "",
                                                  arr[1u].IsString() ? arr[1u].GetString() : "", 
                                                  arr[2u].IsString() ? arr[2u].GetString() : "",
                                                  arr[3u].IsString() ? arr[3u].GetString() : "")
            
;//            .setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(Shader)

    JOP_REGISTER_SAVEABLE(jop, Shader)[](const void* shader, json::Value& val, json::Value::AllocatorType& alloc)
    {
        const Shader& ref = *static_cast<const Shader*>(shader);

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
        //val.AddMember(json::StringRef("persistent"), ref.isPersistent(), alloc);

        val.AddMember(json::StringRef("shaders"), json::kArrayType, alloc)["shaders"]
           .PushBack(json::StringRef(ref.getSource(Shader::Type::Vertex).c_str()), alloc)
           .PushBack(json::StringRef(ref.getSource(Shader::Type::Geometry).c_str()), alloc)
           .PushBack(json::StringRef(ref.getSource(Shader::Type::Fragment).c_str()), alloc)
           .PushBack(json::StringRef(ref.getSource(Shader::Type::Preprocessor).c_str()), alloc);


        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(Shader)
}

namespace
{
    static const int ns_shaderTypes[] =
    {
        gl::VERTEX_SHADER,
        gl::GEOMETRY_SHADER,
        gl::FRAGMENT_SHADER
    };

    unsigned int ns_boundProgram = 0;
}

namespace jop
{
    Shader::Shader(const std::string& name)
        : Resource          (name),
          m_strings         (),
          m_unifMap         (),
          m_attribMap       (),
          m_shaderProgram   (0)
    {}

    Shader::~Shader()
    {
        destroy();
    }

    //////////////////////////////////////////////

    bool Shader::load(const std::string& vert, const std::string& geom, const std::string& frag, const std::string& pp)
    {
        static const int shaderAmount = sizeof(ns_shaderTypes) / sizeof(ns_shaderTypes[0]);

        destroy();
        m_shaderProgram = glCheck(gl::CreateProgram());

        unsigned int shaderHandles[shaderAmount] = {0u, 0u, 0u};
        auto deleteHandles = [](const unsigned int* handles, const unsigned int program)
        {
            for (std::size_t i = 0; i < shaderAmount; ++i)
            {
                if (handles[i])
                {
                    if (program)
                        glCheck(gl::DetachShader(handles[i], program));

                    glCheck(gl::DeleteShader(handles[i]));
                }
            }
        };

        auto handleShaderInfo = [](const unsigned int handle, const unsigned int shaderType) -> bool
        {
            GLint success;
            glCheck(gl::GetShaderiv(handle, gl::COMPILE_STATUS, &success));

            if (success == gl::FALSE_)
            {
                char log[1024];
                glCheck(gl::GetShaderInfoLog(handle, sizeof(log), NULL, log));

                JOP_DEBUG_ERROR("Failed to compile " << (shaderType == 0 ? "vertex" : (shaderType == 1 ? "geometry" : "fragment")) << " shader:\n" << log);

            }

        #ifdef JOP_DEBUG_MODE

            else
            {
                GLint len;
                glCheck(gl::GetShaderiv(handle, gl::INFO_LOG_LENGTH, &len));

                if (len > 0)
                {
                    char log[1024];
                    glCheck(gl::GetShaderInfoLog(handle, sizeof(log), NULL, log));

                    if (std::strcmp(log, "No errors.") != 0 && std::strlen(log) > 0)
                        JOP_DEBUG_WARNING((shaderType == 0 ? "Vertex" : (shaderType == 1 ? "Geometry" : "Fragment")) << " shader compilation produced warnings:\n" << log);
                }
            }

        #endif

            return success == gl::TRUE_;
        };
        auto handleProgramInfo = [](const unsigned int program) -> bool
        {
            GLint success;
            glCheck(gl::GetProgramiv(program, gl::LINK_STATUS, &success));

            if (success == gl::FALSE_)
            {
                char log[1024];
                glCheck(gl::GetProgramInfoLog(program, sizeof(log), NULL, log));

                JOP_DEBUG_ERROR("Failed to link shader program:\n" << log);

            }

        #ifdef JOP_DEBUG_MODE

            else
            {
                GLint len;
                glCheck(gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &len));

                if (len > 0)
                {
                    char log[1024];
                    glCheck(gl::GetProgramInfoLog(program, sizeof(log), NULL, log));

                    if (std::strcmp(log, "No errors.") != 0 && std::strlen(log) > 0)
                        JOP_DEBUG_WARNING("Shader program linking produced warnings:\n" << log);
                }
            }

        #endif

            return success == gl::TRUE_;
        };

        for (int i = 0; i < shaderAmount; i++)
        {
            const std::string& shaderStr = i == 0 ? vert : i == 1 ? geom : frag;

            if (shaderStr.empty())
                continue;

            // File loader error checks need to be disabled here to avoid console spam
            const bool previouslyEnabled = FileLoader::errorChecksEnabled();
            FileLoader::enableErrorChecks(false);

            std::string fileReadBuffer;
            const char* sources[] = {pp.c_str(), "\n", FileLoader::readTextfile(shaderStr, fileReadBuffer) ? reinterpret_cast<const char*>(fileReadBuffer.data()) : shaderStr.c_str()};

            FileLoader::enableErrorChecks(previouslyEnabled);

            shaderHandles[i] = glCheck(gl::CreateShader(ns_shaderTypes[i]));

            int sizes[] = {pp.length(), 1, fileReadBuffer.empty() ? shaderStr.length() : fileReadBuffer.size()};
            glCheck(gl::ShaderSource(shaderHandles[i], 3, sources, sizes));
            glCheck(gl::CompileShader(shaderHandles[i]));

            if (!handleShaderInfo(shaderHandles[i], i))
            {
                deleteHandles(shaderHandles, 0);
                destroy();

                return false;
            }
            glCheck(gl::AttachShader(m_shaderProgram, shaderHandles[i]));
        }

        glCheck(gl::LinkProgram(m_shaderProgram));

        if (!handleProgramInfo(m_shaderProgram))
        {
            deleteHandles(shaderHandles, m_shaderProgram);
            destroy();

            return false;
        }

        deleteHandles(shaderHandles, 0);

        m_strings[0] = vert;
        m_strings[1] = geom;
        m_strings[2] = frag;
        m_strings[3] = pp;

        return true;
    }

    //////////////////////////////////////////////

    void Shader::destroy()
    {
        if (m_shaderProgram)
        {
            // Unbind the program if it's currently active
            if (ns_boundProgram == m_shaderProgram)
                unbind();

            glCheck(gl::DeleteProgram(m_shaderProgram));
        }

        m_unifMap.clear();
        m_attribMap.clear();
        
        m_shaderProgram = 0;
    }

    //////////////////////////////////////////////

    bool Shader::bind() const
    {
        if (m_shaderProgram)
        {
            if (ns_boundProgram != m_shaderProgram)
            {
                glCheck(gl::UseProgram(m_shaderProgram));
                ns_boundProgram = m_shaderProgram;
            }
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    void Shader::unbind()
    {
        if (ns_boundProgram != 0)
        {
            glCheck(gl::UseProgram(0));
            ns_boundProgram = 0;
        }
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::mat4& matrix)
    {
        return setUniform(name, glm::value_ptr(matrix), 1);
    }
    
    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const float* matrices, const unsigned int amount)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::UniformMatrix4fv(loc, amount, gl::FALSE_, matrices));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::mat3& matrix)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::UniformMatrix3fv(loc, 1, gl::FALSE_, glm::value_ptr(matrix)));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::vec2& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform2f(loc, vector.x, vector.y));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::vec3& vector)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform3f(loc, vector.x, vector.y, vector.z));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const glm::vec4& vector)
    {
        const int loc = getUniformLocation(name);
        
        if (loc != -1)
            glCheck(gl::Uniform4f(loc, vector.x, vector.y, vector.z, vector.w));

        return loc != -1;
    }

    //////////////////////////////////////////////
    
    bool Shader::setUniform(const std::string& name, const jop::Texture& texture, const unsigned int unit)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
        {
            texture.bind(unit);
            glCheck(gl::Uniform1i(loc, unit));
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const float value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform1f(loc, value));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const int value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform1i(loc, value));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const unsigned int value)
    {
        const int loc = getUniformLocation(name);

        if (loc != -1)
            glCheck(gl::Uniform1ui(loc, value));

        return loc != -1;
    }

    //////////////////////////////////////////////

    bool Shader::setUniform(const std::string& name, const bool value)
    {
        return setUniform(name, value ? 1 : 0);
    }

    //////////////////////////////////////////////

    bool Shader::setAttribute(const std::string& name, unsigned int type, int amount, unsigned int stride, const bool normalize, const void* pointer)
    {
        const int loc = getAttributeLocation(name);

        if (loc != -1)
        {
            glCheck(gl::VertexAttribPointer(loc, amount, type, normalize, stride, pointer));
            GlState::setVertexAttribute(true, loc);
        }

        return loc != -1;
    }

    //////////////////////////////////////////////

    void Shader::setAttribute(const unsigned int loc, unsigned int type, int amount, unsigned int stride, const bool normalize, const void* pointer)
    {
        GlState::setVertexAttribute(true, loc);
        glCheck(gl::VertexAttribPointer(loc, amount, type, normalize, stride, pointer));
    }

    //////////////////////////////////////////////

    const std::string& Shader::getSource(const Type type) const
    {
        return m_strings[static_cast<int>(type)];
    }

    //////////////////////////////////////////////

    unsigned int Shader::getHandle() const
    {
        return m_shaderProgram;
    }
    
    //////////////////////////////////////////////

    bool Shader::validate() const
    {
        glCheck(gl::ValidateProgram(m_shaderProgram));

        GLint valid;
        glCheck(gl::GetProgramiv(m_shaderProgram, gl::VALIDATE_STATUS, &valid));

        if (valid == gl::FALSE_)
        {
            GLint size;
            glCheck(gl::GetProgramiv(m_shaderProgram, gl::INFO_LOG_LENGTH, &size));

            std::string log(size, '0');
            glCheck(gl::GetProgramInfoLog(m_shaderProgram, size, &size, &log[0]));

            JOP_DEBUG_ERROR("Shader validation failed: " << log);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    Shader& Shader::getDefault()
    {
        static WeakReference<Shader> defShader;

        if (defShader.expired())
        {
            defShader = static_ref_cast<Shader>(ShaderManager::getShader(Material::Attribute::Default).getReference());

            JOP_ASSERT(defShader.get() != &getError(), "Failed to compile default shader!");
        }

        return *defShader;
    }

    //////////////////////////////////////////////

    Shader& Shader::getError()
    {
        static WeakReference<Shader> errShader;

        if (errShader.expired())
        {
            std::vector<unsigned char> vert;
            std::vector<unsigned char> frag;
            JOP_ASSERT_EVAL(FileLoader::readResource(IDR_SHADER1, vert) && FileLoader::readResource(IDR_SHADER2, frag), "Failed to load error shader!");

            errShader = static_ref_cast<Shader>(ResourceManager::getEmptyResource<Shader>("jop_shader_error").getReference());

            JOP_ASSERT_EVAL(errShader->load(std::string(reinterpret_cast<const char*>(vert.data()), vert.size()),
                "",
                std::string(reinterpret_cast<const char*>(frag.data()), frag.size())),
                "Couldn't compile the default shader!");

            errShader->setPersistence(0);
            errShader->setManaged(true);
        }

        return *errShader;
    }

    //////////////////////////////////////////////

    int Shader::getUniformLocation(const std::string& name)
    {
        return getLocation(name, m_unifMap, &getLocUnif);
    }

    //////////////////////////////////////////////

    int Shader::getAttributeLocation(const std::string& name)
    {
        return getLocation(name, m_attribMap, &getLocAttr);
    }

    //////////////////////////////////////////////

    int Shader::getLocation(const std::string& name, LocationMap& map, int (*func)(unsigned int, const std::string&))
    {
        static const bool printErr = SettingManager::getBool("bPrintShaderLocationErrors", true);

        if (bind())
        {
            auto itr = map.find(name);

            if (itr != map.end())
                return itr->second;

            const int location = func(m_shaderProgram, name);

            if (location == -1)
            {
                if (printErr)
                    JOP_DEBUG_WARNING("Uniform/attribute named \"" << name << "\" not found in shader \"" << getName() << "\"");
            }
            else
                map[name] = location;

            return location;
        }

        return -1;
    }

    //////////////////////////////////////////////

    int Shader::getLocUnif(unsigned int prog, const std::string& name)
    {
        const int loc = glCheck(gl::GetUniformLocation(prog, name.c_str()));
        return loc;
    }

    //////////////////////////////////////////////

    int Shader::getLocAttr(unsigned int prog, const std::string& name)
    {
        const int loc = glCheck(gl::GetAttribLocation(prog, name.c_str()));
        return loc;
    }
}