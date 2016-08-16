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
          m_bounds              (),
          m_vertexComponents    (0),
          m_elementSize         (0),
          m_vertexSize          (0)
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

    void Mesh::draw(const uint64 materialAttributes) const
    {
        if (updateVertexAttributes(materialAttributes))
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

    bool Mesh::updateVertexAttributes(const uint64 materialAttribs) const
    {
        if (!getVertexAmount())
            return false;

        m_vertexbuffer.bind();

        const auto vertSize = getVertexSize();

        // Positions (should always be present)
        GlState::setVertexAttribute(true, 0);
        glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(Position)));

        // Texture coordinates
        if ((m_vertexComponents & TexCoords))
        {
            GlState::setVertexAttribute(true, 1);
            glCheck(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(TexCoords)));
        }
        else
            GlState::setVertexAttribute(false, 1);

        // Lighting
        if (materialAttribs & Material::Attribute::__Lighting)
        {
            // Normals
            if (m_vertexComponents & Normal)
            {
                GlState::setVertexAttribute(true, 2);
                glCheck(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(Normal)));
            }
            else
            {
                GlState::setVertexAttribute(false, 2);
                JOP_DEBUG_WARNING("Mesh \"" << getName() << "\" doesn't have normals, although material has lighting enabled");
            }

            //if (materialAttribs & Material::Attribute::NormalMap)
            //{
            //    // Tangents
            //    if (m_vertexComponents & Tangent)
            //    {
            //        GlState::setVertexAttribute(m_vertexComponents & Tangent, 3);
            //        glCheck(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(Tangent)));
            //    }
            //    if (m_vertexComponents & BiTangent)
            //    {
            //        GlState::setVertexAttribute(m_vertexComponents & BiTangent, 4);
            //        glCheck(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(BiTangent)));
            //    }
            //}
        }
        else
        {
            GlState::setVertexAttribute(false, 2);
            GlState::setVertexAttribute(false, 3);
            GlState::setVertexAttribute(false, 4);
        }

        // Colors
        if (m_vertexComponents & Color)
        {
            GlState::setVertexAttribute(true, 5);
            glCheck(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vertSize, getVertexOffset(Color)));
        }
        else
            GlState::setVertexAttribute(false, 5);

        return true;
    }
}