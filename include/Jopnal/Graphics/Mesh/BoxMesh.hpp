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

#ifndef JOP_BOXMESH_HPP
#define JOP_BOXMESH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Mesh/Mesh.hpp>
#include <Jopnal/Graphics/Texture/TextureAtlas.hpp>
#include <glm/vec3.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API BoxMesh : public Mesh
    {
    public:

        /// \brief Default constructor
        ///
        /// Does not initialize the vertices.
        ///
        /// \param name Name of the box
        ///
        BoxMesh(const std::string& name);

        /// \brief Copy constructor
        ///
        /// \param other The other mesh to be copied
        /// \param newName Name of the new mesh
        ///
        BoxMesh(const BoxMesh& other, const std::string& newName);


        /// \brief Load this box
        ///
        /// This will set up the vertices and create the buffers
        ///
        /// \param size Size of the box as vector
        ///
        /// \return True if successful
        ///
        bool load(const glm::vec3& size);

        /// \brief Load this box with certain part of texture
        ///
        /// \param size Size of the box
        /// \param min Minimum bound of texture
        /// \param max Maximum bound of texture
        /// \param invert Is cube inside-out?
        ///
        bool load(const glm::vec3& size, const glm::vec2& min, const glm::vec2& max);

        /// \brief Load from atlas with different texture for each face
        ///
        /// \param size Size of the box
        /// \param atlas Reference to the atlas
        /// \param front Index to the texture to be placed in to front face
        /// \param left Index to the texture to be placed in to left face
        /// \param back Index to the texture to be placed in to back face
        /// \param right Index to the texture to be placed in to right face
        /// \param top Index to the texture to be placed in to top face
        /// \param bottom Index to the texture to be placed in to bottom face
        /// \param invert Is cube inside-out?
        ///
        bool load(const glm::vec3& size,
                  const TextureAtlas& atlas,
                  const unsigned int front,
                  const unsigned int left,
                  const unsigned int back,
                  const unsigned int right,
                  const unsigned int top,
                  const unsigned int bottom);
        
        /// \brief Load from atlas with different texture for each face
        ///
        /// \param size Size of the box
        /// \param front Coordinates of the texture to be placed in front face
        /// \param left Coordinates of the texture to be placed in left face
        /// \param back Coordinates of the texture to be placed in back face
        /// \param right Coordinates of the texture to be placed in right face
        /// \param top Coordinates of the texture to be placed in top face
        /// \param bottom Coordinates of the texture to be placed in bottom face
        /// \param invert Is cube inside-out?
        ///
        bool load(const glm::vec3& size,
                  const std::pair<glm::vec2, glm::vec2>& front,
                  const std::pair<glm::vec2, glm::vec2>& left,
                  const std::pair<glm::vec2, glm::vec2>& back,
                  const std::pair<glm::vec2, glm::vec2>& right,
                  const std::pair<glm::vec2, glm::vec2>& top,
                  const std::pair<glm::vec2, glm::vec2>& bottom);
        
        /// \brief Get the size
        ///
        /// \return The size
        ///
        const glm::vec3& getSize() const;

    private:

        glm::vec3 m_size;       ///< This box's size
    };
}

#endif

/// \class BoxMesh
/// \ingroup Graphics
///
/// 