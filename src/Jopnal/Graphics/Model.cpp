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
    Model::Model(/*IndexBuffer* ib, VertexBuffer* vb*/) : 
        Resource()
        /*,m_ib(ib)
        ,m_vb(vb)*/
    {}

    Model::~Model()
    {}

    //////////////////////////////////////////////

    bool Model::load(const std::string& filepath)
    {
        std::string buffer;
        FileLoader::read("cube.obj", buffer);
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

          //material_ids = shapes.front().mesh.material_ids);
          /*

          IndexBuffer* ib = new IndexBuffer(indices);


          std::vector<VertexArray*> vertArr;

          vertArr.push_back(<vec3>(ATTRIBUTE_POSITION, positions));
          
          vertArr.push_back(<vec3>(ATTRIRBUTE_TEXCOORD, texcoords));

          vertArr.push_back(<vec3>(ATTRIBUTE_NORMAL, normals));

          m_vertexbuffer = new VertexBuffer(&vertArr[0], vertArr.size());
          VertexBuffer* vb = new VertexBuffer(&vertArr[0], vertArr.size());
          
          */
       

      return true;
    }
}