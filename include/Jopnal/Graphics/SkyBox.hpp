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

#ifndef JOP_SKYBOX_HPP
#define JOP_SKYBOX_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/Mesh/BoxMesh.hpp>
#include <Jopnal/Graphics/Material.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Cubemap;

    class JOP_API SkyBox : public Drawable
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(SkyBox);

    public:

        /// \brief Constructor
        ///
        /// You should only ever need to change the default size if you're using
        /// a non-default near clipping plane with your cameras.
        ///
        /// \param obj The object component is bound to
        /// \param renderer The renderer
        /// \param size The box size
        ///
        SkyBox(Object& obj, Renderer& renderer, const float size = 2.f);

        /// \copydoc Drawable::draw()
        ///
        void draw(const ProjectionInfo& proj, const LightContainer& lights) const override;

        /// \brief Set the map
        ///
        /// \param map Reference to the map
        ///
        void setMap(const Cubemap& map);

        /// \brief Get the bound map
        ///
        /// \return Pointer to the map. nullptr if none is bound
        ///
        const Texture* getMap() const;

    private:

        BoxMesh m_mesh;         ///< The mesh
        Material m_material;    ///< The material
    };
}

/// \class jop::SkyBox
/// \ingroup graphics

#endif