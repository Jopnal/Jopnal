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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/Mesh/Mesh.hpp>
    #include <Jopnal/Graphics/Mesh/BoxMesh.hpp>

#include <Jopnal/Core/ResourceManager.hpp>
#include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
#include <Jopnal/Utility/Assert.hpp>


#endif

//////////////////////////////////////////////


namespace jop
{
    Mesh::Mesh(const std::string& name)
        : Resource              (name),
          m_vertexbuffer        (Buffer::Type::ArrayBuffer),
          m_indexbuffer         (Buffer::Type::ElementArrayBuffer),
          m_vertexComponents    (0),
          m_elementSize         (0)
    {}

    //////////////////////////////////////////////

    bool Mesh::load(const void* vertexData, const unsigned int vertexBytes, const uint32 vertexComponents, const void* indexData, const unsigned short indexSize, const unsigned int indexAmount)
    {
        m_vertexbuffer.destroy();
        m_indexbuffer.destroy();

        m_elementSize = std::min((unsigned short)4, indexSize);
        m_vertexComponents = vertexComponents;
        m_vertexSize = getVertexSize(m_vertexComponents);

        m_vertexbuffer.setData(vertexData, vertexBytes);

        if (indexData && m_elementSize && indexAmount)
            m_indexbuffer.setData(indexData, m_elementSize * indexAmount);

        return true;
    }

    //////////////////////////////////////////////

    bool Mesh::load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray)
    {
        return load(vertexArray.data(), vertexArray.size() * sizeof(Vertex), Position | TexCoords | Normal, indexArray.data(), sizeof(unsigned int), indexArray.size());
    }

    //////////////////////////////////////////////

    unsigned int Mesh::getVertexAmount() const
    {
        return m_vertexbuffer.getAllocatedSize() / getVertexSize();
    }

    //////////////////////////////////////////////

    uint16 Mesh::getVertexSize() const
    {
        return m_vertexSize;
    }

    //////////////////////////////////////////////

    void* Mesh::getVertexOffset(const VertexComponent component) const
    {
        using VC = VertexComponent;

        return reinterpret_cast<void*>
        (
            ((m_vertexComponents & VC::Position)   != 0 && Position  < component) * sizeof(glm::vec3) +     // Position
            ((m_vertexComponents & VC::TexCoords)  != 0 && TexCoords < component) * sizeof(glm::vec2) +     // Texture coordinates
            ((m_vertexComponents & VC::Normal)     != 0 && Normal    < component) * sizeof(glm::vec3) +     // Normal
            ((m_vertexComponents & VC::Tangents)   != 0 && Tangents  < component) * sizeof(glm::vec3) * 2   // Tangents
        );
    }

    //////////////////////////////////////////////

    bool Mesh::hasVertexComponent(const uint32 component) const
    {
        return (m_vertexComponents & component) != 0;
    }

    //////////////////////////////////////////////

    unsigned int Mesh::getElementAmount() const
    {
        return m_indexbuffer.getAllocatedSize() / getElementSize();
    }

    //////////////////////////////////////////////

    uint16 Mesh::getElementSize() const
    {
        return m_elementSize;
    }

    //////////////////////////////////////////////

    unsigned int Mesh::getElementEnum() const
    {
        static const GLenum enums[] =
        {
            GL_UNSIGNED_INT,
            GL_UNSIGNED_BYTE,
            GL_UNSIGNED_SHORT,
            GL_UNSIGNED_INT,
            GL_UNSIGNED_INT
        };

        return enums[getElementSize()];
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

    uint16 Mesh::getVertexSize(const uint32 components)
    {
        return sizeof(glm::vec3) //< Positions always present

            + ((VertexComponent::TexCoords & components) != 0) *  sizeof(glm::vec2)
            + ((VertexComponent::Normal & components)    != 0) *  sizeof(glm::vec3)
            + ((VertexComponent::Tangents & components)  != 0) * (sizeof(glm::vec3) * 2) //< Tangent + bitangent
            + ((VertexComponent::Color & components)     != 0) *  sizeof(Color)
            ;
    }

    //////////////////////////////////////////////

    uint16 Mesh::getElementSize(const uint32 amount)
    {
        return 4
          - (amount <= USHRT_MAX) * 2
          - (amount <= UCHAR_MAX);
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
        }

        return *defMesh;
    }
}