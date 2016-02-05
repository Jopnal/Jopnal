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
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

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