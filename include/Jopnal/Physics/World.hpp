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
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

#ifndef JOP_PHYSICSWORLD_HPP
#define JOP_PHYSICSWORLD_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <memory>

//////////////////////////////////////////////


namespace detail
{
    struct GhostCallback;
}

namespace jop
{
    namespace detail
    {
        struct WorldImpl;
    }
    class Camera;

    class JOP_API World final : public Component
    {
    private:

        JOP_DISALLOW_COPY_MOVE(World);

        friend class Scene;
        friend class Renderer;
        friend class RigidBody;
        friend class Collider;

        World* clone(Object&) const override;

    public:

        /// \brief Constructor
        ///
        /// \param renderer Reference to the renderer, needed for debug drawing
        ///
        World(Object& obj, Renderer& renderer);

        /// \brief Destructor
        ///
        ~World();


        /// \brief Update the world
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        /// \brief Debug draw the world
        ///
        /// \param camera Camera to use
        ///
        void draw(const Camera& camera);

    public:

        /// \brief Enable/disable debug drawing
        ///
        /// \comm setWorldDebugMode
        ///
        /// \param enable True to enable
        ///
        void setDebugMode(const bool enable);

        /// \brief Check if debug drawing is enabled
        ///
        /// \return True if enabled
        ///
        bool debugMode() const;

    private:

        std::unique_ptr<detail::WorldImpl> m_worldData;             ///< The world data
        std::unique_ptr<::detail::GhostCallback> m_ghostCallback;   ///< Internal ghost callback
    };
}

#endif