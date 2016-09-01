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

#ifndef JOP_SKYSPHERE_HPP
#define JOP_SKYSPHERE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/Mesh/SphereMesh.hpp>
#include <Jopnal/Graphics/Material.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Texture2D;

    class JOP_API SkySphere : public Drawable
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(SkySphere);

    public:

        /// \brief Constructor
        ///
        /// You should only ever need to change the default radius if you're using
        /// a non-default near clipping plane with your cameras.
        ///
        /// \param obj The object component is bound to
        /// \param renderer The renderer
        /// \param radius The sphere radius
        ///
        SkySphere(Object& obj, Renderer& renderer, const float radius = 2.f);


        /// \copydoc Drawable::draw()
        ///
        void draw(const ProjectionInfo& proj, const LightContainer& lights) const override;

        /// \brief Set the map
        ///
        /// The map should use spherical texture coordinates.
        ///
        /// \param map Reference to the map
        ///
        SkySphere& setMap(const Texture2D& map);

        /// \brief Get the bound map
        ///
        /// \return Pointer to the map. nullptr if none is bound
        ///
        const Texture* getMap() const;

    private:

        SphereMesh m_mesh;      ///< The mesh
        Material m_material;    ///< The material
    };
}

/// \class jop::SkySphere
/// \ingroup graphics

#endif