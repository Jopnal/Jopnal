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
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Physics/RayInfo.hpp>
#include <memory>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        struct WorldImpl;
        struct BroadPhaseCallback;
        struct GhostCallback;
        struct ContactListenerImpl;
    }
    class Camera;
    class Joint;

    class JOP_API World : public Drawable
    {
    private:

        JOP_DISALLOW_COPY_MOVE(World);

        friend class Collider;
        friend class Joint;
        friend class Renderer;
        friend class RigidBody;
        friend class PhantomBody;

        World* clone(Object&) const override;

    public:

        /// \brief Constructor
        ///
        /// \param obj The object to bind this world into
        /// \param renderer Reference to the renderer, needed for debug drawing
        ///
        World(Object& obj, Renderer& renderer);

        /// \brief Destructor
        ///
        ~World() override;


        /// \brief Update the world
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        /// \brief Debug draw the world
        ///
        /// \param camera Camera to use
        ///
        void draw(const ProjectionInfo& proj, const LightContainer& lights) const override;


        /// \brief Check if a ray hits a collider and return the closest one
        ///
        /// \param start The start position of the ray
        /// \param ray Ray to be shot from start
        /// \param group The collision group
        /// \param mask The collision mask
        ///
        /// \return Ray hit info
        ///
        RayInfo checkRayClosest(const glm::vec3& start, const glm::vec3& ray, const short group = 1, const short mask = 32767) const;

        /// \brief Check if a ray hits a collider and return all hits
        ///
        /// \param start The start position of the ray
        /// \param ray Ray to be shot from start
        /// \param group The collision group
        /// \param mask The collision mask
        ///
        /// \return Vector of ray infos, empty if none were hit
        ///
        std::vector<RayInfo> checkRayAllHits(const glm::vec3& start, const glm::vec3& ray, const short group = 1, const short mask = 32767) const;

        /// \brief Get all the colliders that overlap with the bounding box
        ///
        /// \param aabbStart Starting point of the bounding box
        /// \param aabbEnd Ending point of the bounding box
        /// \param group The collision group
        /// \param mask The collision mask
        ///
        /// \return Vector of colliders, empty if none were overlapping
        ///
        std::vector<Collider*> checkOverlapAll(const glm::vec3& aabbStart, const glm::vec3& aabbEnd, const short group = 1, const short mask = 32767) const;

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

    protected:

        Message::Result receiveMessage(const Message& message) override;


        std::unique_ptr<detail::WorldImpl> m_worldData;                 ///< The world data
        std::unique_ptr<detail::GhostCallback> m_ghostCallback;       ///< Internal ghost callback
        std::unique_ptr<detail::ContactListenerImpl> m_contactListener; ///< Contact listener implementation
        std::unique_ptr<detail::BroadPhaseCallback> m_bpCallback;
    };
}

#endif