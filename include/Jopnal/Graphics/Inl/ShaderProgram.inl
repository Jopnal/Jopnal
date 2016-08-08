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

namespace detail
{
    JOP_API bool attach(ShaderProgram& program, const Shader& currentShader);

    template<typename First, typename ... Rest>
    bool attach(ShaderProgram& program, const First& first, const Rest&... rest)
    {      
        return attach(program, first) && attach(program, rest...);
    }

    JOP_API bool attachSingle(ShaderProgram& program, const std::string& pp, std::vector<std::unique_ptr<Shader>>& shaders, const Shader::Type& type, const std::string& source);

    template<typename ... Rest>
    bool attachSingle(ShaderProgram& program, const std::string& pp, std::vector<std::unique_ptr<Shader>>& shaders, const Shader::Type& type, const std::string& source, const Rest&... rest)
    {
        return attachSingle(program, pp, shaders, type, source) && attachSingle(program, pp, shaders, rest...);
    }

    template<typename ... Rest>
    bool attacher(ShaderProgram& program, const std::string&, const Shader& shader, const Rest&... rest)
    {
        return attach(program, shader, rest...) && program.link();
    }

    template<typename ... Rest>
    bool attacher(ShaderProgram& program, const std::string& pp, const Shader::Type& type, const Rest&... rest)
    {
        std::vector<std::unique_ptr<Shader>> shaders;
        return attachSingle(program, pp, shaders, type, rest...) && program.link();
    }
}

template<typename ... Args>
bool ShaderProgram::load(const std::string& pp, const Args&... shaders)
{
    return detail::attacher(*this, pp, shaders...);
}