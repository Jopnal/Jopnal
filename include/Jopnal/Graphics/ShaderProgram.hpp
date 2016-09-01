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

#ifndef JOP_SHADERPROGRAM_HPP
#define JOP_SHADERPROGRAM_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Graphics/Shader.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    class Texture;

    class JOP_API ShaderProgram : public Resource
    {
    private:

        typedef std::unordered_map<std::string, int> LocationMap; ///< Map containing uniform locations

    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        ShaderProgram(const std::string& name);

        /// \brief Destructor
        ///
        ~ShaderProgram();


        /// \brief Load shaders
        ///
        /// Attaches a variable number of shaders to shader program.
        /// \note Can load some shaders as objects and others as source codes simultaneously.
        ///
        /// Invalid shader objects or empty sources will be silently ignored and won't be used
        /// in the final link operation.
        /// 
        /// \param pp Preprocessor string
        /// \param args Variable amount of shaders objects or shader sources
        ///
        /// \return True if successful
        ///
        template<typename ... Args>
        bool load(const std::string& pp, const Args&... args);

        /// \brief Attach shader to the program
        ///
        /// \param shader Reference to shader object
        ///
        /// \return True if successful
        ///
        bool attachShader(const Shader& shader);

        /// \brief Link the shader program
        ///
        /// \return True if successful
        ///
        bool link();

        /// \brief Unlink the shader program
        ///
        /// Unbinds shader program and destroys it.
        ///
        void unlink();

        /// \brief Bind this shader program for use
        ///
        /// \return True if successful
        ///
        bool bind() const;

        /// \brief Unbind this shader program
        ///
        /// \return True if successful
        ///
        void unbind() const;

        /// \brief Validate program status
        ///
        /// \return True if successfully validated
        ///
        bool validate() const;

        /// \brief Check if this program is valid
        ///
        /// \return True if valid
        ///
        bool isValid() const;

        /// \brief Set a mat4 uniform
        ///
        /// \param name Uniform name
        /// \param matrix A 4x4 matrix
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const glm::mat4& matrix);

        /// \brief Set an array of matrices
        ///
        /// This is for 4x4 matrices (glm::mat4).
        ///
        /// \param name The uniform name
        /// \param matrices Pointer to the first element of the first matrix
        /// \param amount Amount of the matrices
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const float* matrices, const unsigned int amount);

        /// \brief Set a mat3 uniform
        ///
        /// \param name Uniform name
        /// \param matrix A 3x3 matrix
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const glm::mat3& matrix);

        /// \brief Set a two-dimensional float vector uniform
        ///
        /// \param name Name of the uniform
        /// \param vector The vector to set
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const glm::vec2& vector);

        /// \brief Set a vec3 uniform
        ///
        /// \param name Uniform name
        /// \param vector A 3-dimensional vector
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const glm::vec3& vector);

        /// \brief Set a vec4 uniform
        ///
        /// \param name Uniform name
        /// \param vector A 4-dimensional vector
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const glm::vec4& vector);

        /// \brief Set a texture uniform
        ///
        /// \param name Uniform name
        /// \param texture A texture
        /// \param unit The texture unit
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const Texture& texture, const unsigned int unit);

        /// \brief Set a float uniform
        ///
        /// \param name Name of the uniform
        /// \param value The float value
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const float value);

        /// \brief Set an integer uniform
        ///
        /// \param name Name of the uniform
        /// \param value The integer to set
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const int value);

        /// \brief Set a boolean uniform
        ///
        /// \param name Name of the uniform
        /// \param value The boolean to set
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const bool value);

        /// \brief Get the default shader program
        ///
        /// \return Reference to the default shader
        ///
        static ShaderProgram& getDefault();

        /// \brief Get the error shader program
        ///
        /// This shader will paint any drawn object bright red.
        ///
        /// \return Reference to the error shader
        ///
        static ShaderProgram& getError();

        /// \brief Get the maximum vertex attributes
        ///
        /// \return The maximum number of vertex attributes supported
        ///
        static unsigned int getMaxAttributes();

    private:

        /// \brief Get the location of a uniform by name
        ///
        int getUniformLocation(const std::string& name);


        std::unordered_map<unsigned int, WeakReference<const Shader>> m_shaders;    ///< Map with shaders
        LocationMap m_unifMap;                                                      ///< Map with uniform locations
        unsigned int m_programID;                                                   ///< OpenGL program handle
    };

    // Include the template implementation file
    #include <Jopnal/Graphics/Inl/ShaderProgram.inl>
}

/// \class jop::ShaderProgram
/// \ingroup graphics

#endif