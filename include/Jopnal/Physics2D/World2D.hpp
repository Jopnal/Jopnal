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

#ifndef JOP_PHYSICSWORLD2D_HPP
#define JOP_PHYSICSWORLD2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Physics2D/RayInfo2D.hpp>
#include <memory>
#include <vector>

//////////////////////////////////////////////


class b2World;

namespace jop
{
    class Camera;
    class Joint2D;

    namespace detail
    {
        struct ContactListener2DImpl;
        struct DebugDraw;
    }

    class JOP_API World2D : public Drawable
    {
    private:

        JOP_DISALLOW_COPY_MOVE(World2D);

        friend class Scene;
        friend class Renderer;
        friend class RigidBody2D;
        friend class Collider2D;
        friend class Joint2D;

        World2D* clone(Object&) const override;

    public:

        /// \brief Constructor
        ///
        /// \param obj The object to bind this world into
        /// \param renderer Reference to the renderer, needed for debug drawing
        ///
        World2D(Object& obj, Renderer& renderer);

        /// \brief Destructor
        ///
        ~World2D() override;


        /// \brief Update the world
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        /// \copydoc World::draw()
        ///
        void draw(const ProjectionInfo& proj, const LightContainer& lights) const override;

        /// \brief Set gravity for world
        ///
        /// \param gravity Vector holding amplitude of gravity in each dimension
        ///
        void setGravity(const glm::vec2& gravity);

        /// \brief Get gravity for world
        ///
        glm::vec2 getGravity() const;

        /// \brief Check if a ray hits a collider and return the closest one
        ///
        /// \param start The start position of the ray
        /// \param ray Ray to be shot from start
        /// \param group The collision group
        /// \param mask The collision mask
        ///
        /// \return Ray hit info
        ///
        RayInfo2D checkRayClosest(const glm::vec2 start, const glm::vec2 ray, const short group = 1, const short mask = 32767) const;

        /// \brief Check if a ray hits a collider and return all hits
        ///
        /// \param start The start position of the ray
        /// \param ray Ray to be shot from start
        /// \param group The collision group
        /// \param mask The collision mask
        ///
        /// \return Vector of ray infos, empty if none were hit
        ///
        std::vector<RayInfo2D> checkRayAllHits(const glm::vec2& start, const glm::vec2& ray, const short group = 1, const short mask = 32767) const;

        /// \brief Get all the colliders that overlap with the bounding box
        ///
        /// \param aabbStart Starting point of the bounding box
        /// \param aabbEnd Ending point of the bounding box
        /// \param group The collision group
        /// \param mask The collision mask
        ///
        /// \return Vector of colliders, empty if none were overlapping
        ///
        std::vector<Collider2D*> checkOverlapAll(const glm::vec2& aabbStart, const glm::vec2& aabbEnd, const short group = 1, const short mask = 32767) const;

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

        Message::Result receiveMessage(const Message& message) override;

        std::unique_ptr<detail::ContactListener2DImpl> m_contactListener;   ///< Contact listener implementation
        std::unique_ptr<b2World> m_worldData2D;                             ///< The world data
        std::unique_ptr<detail::DebugDraw> m_dd;                            ///< Debug drawer
        float m_step;                                                       ///< Current step timer
    };
}

/// \class jop::World2D
/// \ingroup physics2d

#endif