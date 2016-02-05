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

// Headers
#include <Jopnal/Precompiled.hpp>
// Tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

//////////////////////////////////////////////


namespace jop
{
    Model::Model() :
        Resource()
        , m_vertexbuffer(Buffer::BufferType::ArrayBuffer)
        , m_indexbuffer(Buffer::BufferType::ElementArrayBuffer)
    {}

    Model::~Model()
    {}

    //////////////////////////////////////////////

    bool Model::load(const std::string& filepath)
    {
        std::string buffer;
        FileLoader::read(filepath, buffer);
        std::istringstream sstream(buffer);

        class MatRead : public tinyobj::MaterialReader
        {
            bool operator()(const std::string&,
                std::vector<tinyobj::material_t>&,
                std::map<std::string, int>&,
                std::string&)
            {
                return true;
            }
        };

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texcoords;
        std::vector<unsigned int> indices;
        //std::vector<int> material_ids; // per-mesh material ID

        std::string err;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        
        bool ret = tinyobj::LoadObj(shapes, materials, err, sstream, MatRead());

      if (!err.empty())
      {
          std::cerr << err << std::endl;
      }
      if (!ret)
          return false;

          positions = shapes.front().mesh.positions;
          normals = shapes.front().mesh.normals;
          texcoords = shapes.front().mesh.texcoords;
          indices = shapes.front().mesh.indices;
          //material_ids = shapes.front().mesh.material_id
          
          std::vector<Vertex> vertexArray;
          vertexArray.reserve(positions.size() / 3);

          for (std::size_t i = 0; i < vertexArray.capacity(); ++i)
          {
              vertexArray.emplace_back
                  (
                  glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]),
                  texcoords.size() > i * 2 ? glm::vec2(texcoords[i * 2], texcoords[i * 2 + 1]) : glm::vec2(),
                  normals.size() > i * 3 ? glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]) : glm::vec3()
                  );
          }
          
          m_vertexbuffer.setData(&vertexArray[0], sizeof(Vertex)*vertexArray.size());

          if (!indices.empty())
          m_indexbuffer.setData(&indices[0], sizeof(unsigned int)*indices.size());
          
      return true;
    }

    //////////////////////////////////////////////

    bool Model::load(const std::vector<Vertex>& vertexArray,const std::vector<unsigned int>& indices)
    {
        m_vertexbuffer.setData(&vertexArray[0], sizeof(Vertex)*vertexArray.size());

        if (!indices.empty())
            m_indexbuffer.setData(&indices[0], sizeof(unsigned int)*indices.size());

        return true;
    }

    //////////////////////////////////////////////

    const VertexBuffer& Model::getIndexBuffer() const
    {
        return m_indexbuffer;
    }

    //////////////////////////////////////////////

    const VertexBuffer& Model::getVertexBuffer() const
    {
        return m_vertexbuffer;
    }

}