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

    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/Material.hpp>
    #include <Jopnal/Graphics/Mesh/BoxMesh.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlState.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Utility/Assert.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Mesh::Mesh(const std::string& name)
        : Resource              (name),
          m_vertexbuffer        (Buffer::Type::ArrayBuffer),
          m_indexbuffer         (Buffer::Type::ElementArrayBuffer),
          m_shape               (""),
          m_bounds              (),
          m_vertexComponents    (0),
          m_elementSize         (0),
          m_vertexSize          (0)
    {
        m_shape.load(glm::vec3(1.f));
    }

    Mesh::Mesh(const Mesh& other, const std::string& newName)
        : Resource              (other, newName),
          m_vertexbuffer        (other.m_vertexbuffer),
          m_indexbuffer         (other.m_indexbuffer),
          m_shape               (other.m_shape, ""),
          m_bounds              (other.m_bounds),
          m_vertexComponents    (other.m_vertexComponents),
          m_elementSize         (other.m_elementSize),
          m_vertexSize          (other.m_vertexSize)
    {}

    //////////////////////////////////////////////

    bool Mesh::load(const void* vertexData, const unsigned int vertexBytes, const uint32 vertexComponents, const void* indexData, const unsigned short indexSize, const unsigned int indexAmount, const bool calculateBounds)
    {
        m_vertexbuffer.destroy();
        m_indexbuffer.destroy();

        m_elementSize = std::min((unsigned short)4, indexSize);
        m_vertexComponents = vertexComponents;
        m_vertexSize = getVertexSize(m_vertexComponents);

        m_vertexbuffer.setData(vertexData, vertexBytes);

        if (indexData && m_elementSize && indexAmount)
            m_indexbuffer.setData(indexData, m_elementSize * indexAmount);

        if (calculateBounds)
        {
            const auto vs = getVertexSize();
            const uint8* binData = reinterpret_cast<const uint8*>(vertexData);

            for (std::size_t i = 0; i < vertexBytes; i += vs)
            {
                m_bounds.first = glm::min(m_bounds.first, *reinterpret_cast<const glm::vec3*>(binData[i]));
                m_bounds.second = glm::max(m_bounds.second, *reinterpret_cast<const glm::vec3*>(binData[i]));
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool Mesh::load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray, const bool calculateBounds)
    {
        return load(vertexArray.data(), vertexArray.size() * sizeof(Vertex), Position | TexCoords | Normal, indexArray.data(), sizeof(unsigned int), indexArray.size(), calculateBounds);
    }

    //////////////////////////////////////////////

    void Mesh::draw() const
    {
        if (updateVertexAttributes())
        {
            if (getElementAmount())
            {
                getIndexBuffer().bind();
                glCheck(glDrawElements(GL_TRIANGLES, getElementAmount(), getElementEnum(), 0));
            }
            else
            {
                glCheck(glDrawArrays(GL_TRIANGLES, 0, getVertexAmount()));
            }
        }
    }

    //////////////////////////////////////////////

    void Mesh::destroy()
    {
        m_vertexbuffer.destroy();
        m_indexbuffer.destroy();

        m_vertexComponents = 0;
        m_elementSize = 0;
        m_vertexSize = 0;
    }

    //////////////////////////////////////////////

    const std::pair<glm::vec3, glm::vec3>& Mesh::getBounds() const
    {
        return m_bounds;
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
            ((m_vertexComponents & VC::Tangent)    != 0 && Tangent   < component) * sizeof(glm::vec3) +     // Tangent
            ((m_vertexComponents & VC::BiTangent)  != 0 && BiTangent < component) * sizeof(glm::vec3)       // BiTangent
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

            + ((VertexComponent::TexCoords  & components)   != 0) * sizeof(glm::vec2)
            + ((VertexComponent::Normal     & components)   != 0) * sizeof(glm::vec3)
            + ((VertexComponent::Tangent    & components)   != 0) * sizeof(glm::vec3)
            + ((VertexComponent::BiTangent  & components)   != 0) * sizeof(glm::vec3)
            + ((VertexComponent::Color      & components)   != 0) * sizeof(::jop::Color);
    }

    //////////////////////////////////////////////

    void Mesh::updateBounds(const glm::vec3& min, const glm::vec3& max)
    {
        m_bounds.first = min;
        m_bounds.second = max;

        m_shape.load(max - min);
    }

    //////////////////////////////////////////////

    BoxShape& Mesh::getCullingShape() const
    {
        return m_shape;
    }

    //////////////////////////////////////////////

    Mesh& Mesh::getDefault()
    {
        static WeakReference<BoxMesh> defMesh;

        if (defMesh.expired())
        {
            defMesh = static_ref_cast<BoxMesh>(ResourceManager::getEmpty<BoxMesh>("jop_default_mesh").getReference());

            JOP_ASSERT_EVAL(defMesh->load(glm::vec3(1.f)), "Couldn't load default model!");

            defMesh->setPersistence(0);
        }

        return *defMesh;
    }

    //////////////////////////////////////////////

    bool Mesh::updateVertexAttributes() const
    {
        if (!getVertexAmount())
            return false;

        m_vertexbuffer.bind();

        const auto vertSize = getVertexSize();

        // Positions (should always be present)
        GlState::setVertexAttribute(true, VertexIndex::Position);
        glCheck(glVertexAttribPointer(VertexIndex::Position, 3, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(Position)));

        // Texture coordinates
        if ((m_vertexComponents & TexCoords))
        {
            GlState::setVertexAttribute(true, VertexIndex::TexCoords);
            glCheck(glVertexAttribPointer(VertexIndex::TexCoords, 2, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(TexCoords)));
        }
        else
            GlState::setVertexAttribute(false, VertexIndex::TexCoords);

        // Normals
        if (m_vertexComponents & Normal)
        {
            GlState::setVertexAttribute(true, VertexIndex::Normal);
            glCheck(glVertexAttribPointer(VertexIndex::Normal, 3, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(Normal)));
        }
        else
            GlState::setVertexAttribute(false, VertexIndex::Normal);

        // Colors
        if (m_vertexComponents & Color)
        {
            GlState::setVertexAttribute(true, VertexIndex::Color);
            glCheck(glVertexAttribPointer(VertexIndex::Color, 4, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(Color)));
        }
        else
            GlState::setVertexAttribute(false, VertexIndex::Color);

        return true;
    }
}