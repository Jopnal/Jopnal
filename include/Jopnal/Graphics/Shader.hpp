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

#ifndef JOP_SHADER_HPP
#define JOP_SHADER_HPP

// Headers
#include <Jopnal/Jopnal.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <unordered_map>
#include <array>

//////////////////////////////////////////////


namespace jop
{
    class Texture;

    class JOP_API Shader : public Resource
    {
    private:

        typedef std::unordered_map<std::string, int> LocationMap;

        JOP_DISALLOW_COPY_MOVE(Shader);

    public:

        /// The shader type
        ///
        enum class Type
        {
            Vertex,
            Geometry,
            Fragment,
            Preprocessor
        };

    public:

        /// \brief default constructor
        ///
        Shader(const std::string& name);

        /// \brief default destructor
        ///
        ~Shader() override;


        /// \brief Load a shader
        ///
        /// The arguments don't have to be paths. They may also contain the shader code.
        ///
        /// \param vert Vertex shader path
        /// \param geom Geometry shader path
        /// \param frag Fragment shader path
        /// \param pp Preprocessor definitions
        ///
        /// \return True if the shader was loaded, compiled and linked successfully
        ///
        bool load(const std::string& vert, const std::string& geom, const std::string& frag, const std::string& pp = "#version 330 core");

        /// \brief Destroy this shader
        ///
        void destroy();


        /// \brief Bind this shader for use
        ///
        /// \return True if successful
        ///
        bool bind() const;

        /// \brief Unbind the currently bound shader
        ///
        static void unbind();

          
        /// \brief method setting matrix 4x4
        ///
        /// \param name unique name
        /// \param matrix 4x4 matrix
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

        /// \brief method setting 3x3 matrix with unique name
        ///
        /// \param name unique name
        /// \param matrix 3x3 matrix
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

        /// \brief method setting vector with 3 dimensions and unique name
        ///
        /// \param name unique name
        /// \param vector 3 dimensional vector
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const glm::vec3& vector);
        
        /// \brief method setting vector with 4 dimensions and unique name
        ///
        /// \param name unique name
        /// \param vector 4 dimensional vector
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const glm::vec4& vector);

        /// \brief method setting texture with unique name and Uint
        ///
        /// \param name unique name
        /// \param texture The texture
        /// \param unit The texture unit
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const jop::Texture& texture, const unsigned int unit);

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

        /// \brief Set an unsigned integer uniform
        ///
        /// \param name Name of the uniform
        /// \param value The unsigned integer to set
        ///
        /// \return True if set successfully
        ///
        bool setUniform(const std::string& name, const unsigned int value);

        /// \brief Set an attribute using the location
        ///
        /// \param loc Attribute location
        /// \param type Data type, Use the appropriate OpenGL enum
        /// \param amount Amount of variables per element
        /// \param stride The stride. Refer to OpenGL documentation on what this is
        /// \param normalize Normalize the attributes?
        /// \param pointer Pointer to the data or the offset in the buffer
        ///
        void setAttribute(const unsigned int loc, unsigned int type, int amount, unsigned int stride, const bool normalize, const void* pointer);


        /// \brief Get the shader source
        ///
        /// This is either the file path from which the shader was loaded or the shader source code itself.
        ///
        /// \param type The shader type
        ///
        /// \return Reference to the source
        ///
        const std::string& getSource(const Type type) const;


        /// \brief Get the OpenGL handle
        ///
        /// \return The OpenGL handle
        ///
        unsigned int getHandle() const;

        /// \brief Validate this shader
        ///
        /// This is useful for catching shader errors. You should never call this
        /// unless you have a need to debug your shaders.
        ///
        /// \return True if validation passed
        ///
        bool validate() const;

        
        /// \brief Get the default shader
        ///
        /// \return Reference to the default shader
        ///
        static Shader& getDefault();

        /// \brief Get the error shader
        ///
        /// This shader will paint any drawn object bright red.
        ///
        /// \return Reference to the error shader
        ///
        static Shader& getError();

    private:

        /// \brief Get location of uniform by name
        ///
        int getUniformLocation(const std::string& name);

        
        std::array<std::string, 4> m_strings;   ///< The shader sources
        LocationMap m_unifMap;                  ///< Map with the uniform locations
        unsigned int m_shaderProgram;           ///< The OpenGL shader handle
    };
}

#endif

/// \class Shader
/// \ingroup Graphics
///
/// 