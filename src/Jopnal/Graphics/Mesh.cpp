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

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_LOADABLE(jop, Mesh)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load Mesh, no name found");
            return false;
        }

        //ResourceManager::getResource<Mesh>(val["name"].GetString())
;//            .setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(Mesh)

    JOP_REGISTER_SAVEABLE(jop, Mesh)[](const void* mesh, json::Value& val, json::Value::AllocatorType& alloc)
    {
        const Mesh& ref = *static_cast<const Mesh*>(mesh);

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
//        val.AddMember(json::StringRef("persistent"), ref.isPersistent(), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(Mesh)
}

namespace jop
{
    Mesh::Mesh(const std::string& name)
        : Resource          (name),
          m_vertexbuffer    (Buffer::Type::ArrayBuffer),
          m_indexbuffer     (Buffer::Type::ElementArrayBuffer)
    {}

    //////////////////////////////////////////////

    bool Mesh::load(const std::vector<unsigned char>& vertexData, const std::vector<unsigned char>& indices)
    {
        m_vertexbuffer.setData(vertexData.data(), vertexData.size());

        if (!indices.empty())
            m_indexbuffer.setData(indices.data(), indices.size());

        return true;
    }

    //////////////////////////////////////////////

    bool Mesh::load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray)
    {
        m_vertexbuffer.setData(vertexArray.data(), sizeof(Vertex) * vertexArray.size());

        if (!indexArray.empty())
            m_indexbuffer.setData(indexArray.data(), sizeof(unsigned int) * indexArray.size());

        return true;
    }

    //////////////////////////////////////////////

    unsigned int Mesh::getVertexAmount() const
    {
        return m_vertexbuffer.getAllocatedSize() / sizeof(Vertex);
    }

    //////////////////////////////////////////////

    unsigned int Mesh::getElementAmount() const
    {
        return m_indexbuffer.getAllocatedSize() / sizeof(unsigned int);
    }

    //////////////////////////////////////////////

    const VertexBuffer& Mesh::getIndexBuffer() const
    {
        return m_indexbuffer;
    }

    //////////////////////////////////////////////

    const VertexBuffer& Mesh::getVertexBuffer() const
    {
        return m_vertexbuffer;
    }

    //////////////////////////////////////////////

    Mesh& Mesh::getDefault()
    {
        static WeakReference<BoxMesh> defMesh;

        if (defMesh.expired())
        {
            defMesh = static_ref_cast<BoxMesh>(ResourceManager::getEmptyResource<BoxMesh>("jop_default_mesh").getReference());

            JOP_ASSERT_EVAL(defMesh->load(1.f), "Couldn't load default model!");

            defMesh->setPersistence(0);
            defMesh->setManaged(true);
        }

        return *defMesh;
    }
}