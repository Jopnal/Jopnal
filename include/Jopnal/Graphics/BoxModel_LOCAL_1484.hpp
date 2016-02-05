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

#ifndef JOP_BOXMODEL_HPP
#define JOP_BOXMODEL_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Model.hpp>

//////////////////////////////////////////////

namespace jop
{
    class JOP_API BoxModel : public Model
    {
    public:
        BoxModel();
        ~BoxModel();

        const VertexBuffer& getVertexBuffer() const;
        const std::vector<Vertex>& getVertexArray() const;

    private:
        std::vector<Vertex> m_vertexArray;
        VertexBuffer m_vertexbuffer;

        const glm::vec3 m_positions[8] =
        {
            glm::vec3(0.0, 0.0, 0.0),
            glm::vec3(0.0, 0.0, 1.0),
            glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, 1.0, 1.0),
            glm::vec3(1.0, 0.0, 0.0),
            glm::vec3(1.0, 0.0, 1.0),
            glm::vec3(1.0, 1.0, 0.0),
            glm::vec3(1.0, 1.0, 1.0),
        };

        const glm::vec2 m_texcoords[4] =
        {
            glm::vec2(0.0, 0.0),
            glm::vec2(1.0, 0.0),
            glm::vec2(1.0, 1.0),
            glm::vec2(0.0, 1.0),
        };

        const glm::vec3 m_normals[6] =
        {
            glm::vec3(0.0, 0.0, 1.0),
            glm::vec3(0.0, 0.0, -1.0),
            glm::vec3(0.0, 1.0, 0.0),
            glm::vec3(0.0, -1.0, 0.0),
            glm::vec3(1.0, 0.0, 0.0),
            glm::vec3(-1.0, 0.0, 0.0),
        };
    };
}

#endif