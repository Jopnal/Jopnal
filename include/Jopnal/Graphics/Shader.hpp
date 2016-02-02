// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_SHADER_HPP
#define JOP_SHADER_HPP

// Headers
#include <Jopnal/Jopnal.hpp>
#include <Jopnal/Core/Resource.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Texture;

    class Shader : public Resource
	{
	public:

        /// \brief default constructor
        ///
		Shader();

        /// \brief default destructor
        ///
		~Shader();

        /// \brief Load method for loading shaders
        ///
        /// The format goes like this:
        /// "v;pathToVertexShader|g;pathToGeometryShader|f;pathToFragmentShader"
        ///
        /// The paths must appear in this order (vertex, geometry, fragment).
        /// If you have less than three shader types to compile, simply leave out
        /// the ones you don't need. Example: "v;pathToVertexShader|f;pathToFragmentShader".
        ///
        /// \param shaders A correctly formatted string with the shader file paths
        ///
        bool load(const std::string& shaders) override;

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
        void setUniform(const std::string& name, const glm::mat4& matrix);

        /// \brief method setting 3x3 matrix with unique name
        ///
        /// \param name unique name
        /// \param matrix 3x3 matrix
        ///
        void setUniform(const std::string& name, const glm::mat3& matrix);
        
        /// \brief method setting vector with 4 dimensions and unique name
        ///
        /// \param name unique name
        /// \param vector 4 dimensional vector
        ///
        void setUniform(const std::string& name, const glm::vec4& vector);

        /// \brief method setting texture with unique name and Uint
        ///
        /// \param name unique name
        /// \param texture Uint 
        ///
        void setUniform(const std::string& name, const jop::Texture& );

        /// \brief method setting attributes
        ///
        /// \param name unique name
        /// \param type GLenum type
        /// \param amount attributes size Glint size
        /// \param stride 
        /// \param pointer void pointer
        ///
        void setAttribute(const std::string& name, unsigned int type, int amount, unsigned int stride, const void* pointer);

    private:

        ///< get location of uniform by name
        int getUniformLocation(const std::string& name);

        ///< get location of attribute by name
        int getAttributeLocation(const std::string& name);
        
        unsigned int m_shaderProgram; ///< The OpenGL shader handle
                
	};
}

#endif

/// \class Shader
/// \ingroup Graphics
///
/// Do explanation about the class