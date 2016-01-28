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

#ifndef RESOURCEMODEL_HPP
#define RESOURCEMODEL_HPP


// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>

//////////////////////////////////////////////


namespace jop
{
    /// \brief Class for 3D model loading and storing its data.
    ///
    /// NOTE: Currently only supports .obj format.
    ///
    class JOP_API Model : public Resource
    {
    public:
        Model();
        ~Model();

        /// \brief Method to load .obj model
        ///
        /// Loads .obj and copies data to their containers (positions, normals, texcoords, indices)
        ///
        /// \param filePath The path to the file you want to load
        ///
        bool load(const std::string& filePath);
        /// \brief Method to get index values from container
        ///
        const std::vector<unsigned int>& getIndices();
        /// \brief Method to get vertex position values from container
        ///
        const std::vector<float>& getVertexPositions();
        /// \brief Method to get normal values from container
        ///
        const std::vector<float>& getNormals();
        /// \brief Method to get texture coordinate (UV) values from container
        ///
        const std::vector<float>& getTexCoords();

    private:

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texcoords;
        std::vector<unsigned int> indices;
        //std::vector<int> material_ids; ///< per-mesh material ID
    };
}

#endif

/// \class Model
/// \ingroup graphics