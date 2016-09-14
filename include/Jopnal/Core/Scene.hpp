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

#ifndef JOP_SCENE_HPP
#define JOP_SCENE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Object.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <Jopnal/Physics/World.hpp>
#include <vector>
#include <tuple>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        class CullingBroadphaseCallback;
    }
    class Renderer;
    class World;
    class World2D;
    class Window;
    class RenderTarget;

    namespace detail
    {
        template<int D>
        struct WorldType
        {
            static_assert(D == 2 || D == 3, "Invalid world dimensions");
        };

        template<>
        struct WorldType<2>
        {
            typedef World2D type;
        };

        template<>
        struct WorldType<3>
        {
            typedef World type;
        };
    }

    class JOP_API Scene : private Object
    {
    public:

        using Object::getReference;
        using Object::reserveChildren;

        using Object::createComponent;
        using Object::getComponent;
        using Object::getComponents;
        using Object::removeComponents;
        using Object::clearComponents;
        using Object::componentCount;
        using Object::removeComponent;

        using Object::createChild;
        using Object::getChildren;
        using Object::cloneChild;
        using Object::removeChildren;
        using Object::clearChildren;
        using Object::childCount;
        using Object::childCountRecursive;
        using Object::findChild;
        using Object::findChildWithPath;
        using Object::findChildren;
        using Object::findChildrenWithTag;

        using Object::adoptChild;
        using Object::setParent;

        using Object::isActive;
        using Object::setActive;

        using Object::getID;
        using Object::setID;

        using Object::printDebugTree;

        using TransformRestriction = Object::TransformRestriction;

    public:

        JOP_DISALLOW_COPY_MOVE(Scene);

        friend class SceneLoader;
        friend class Object;
        friend class Renderer;

    public:

        /// \brief Constructor
        ///
        /// \param ID Scene identifier
        ///
        Scene(const std::string& ID);

        /// \brief Virtual destructor
        ///
        virtual ~Scene();


        /// \brief Set a new renderer
        ///
        /// \param args Arguments to pass into the renderer's constructor
        ///
        /// \return Reference to the newly constructed renderer
        ///
        template<typename T, typename ... Args>
        T& setRenderer(Args&&... args);

        /// \brief Get the physics world
        ///
        /// The template argument D must be either 2 or 3.
        /// If a world hasn't been enabled yet, it will be created by this function.
        ///
        /// \return Reference to the world
        ///
        template<int D>
        typename detail::WorldType<D>::type& getWorld();

        /// \brief Check if a world is enabled
        ///
        /// The template argument D must be either 2 or 3.
        ///
        /// \return True if world has been enabled
        ///
        template <int D>
        bool worldEnabled() const;

        /// \brief Disable a world
        ///
        /// The template argument D must be either 2 or 3.
        /// The world will be deleted immediately. You shouldn't
        /// need to ever call this in typical situations, but
        /// should you, you'll need to ensure that all colliders
        /// that exist within the world are destroyed beforehand.
        ///
        /// After this call returns, getWorld() will create the
        /// world again once called.
        ///
        /// \see getWorld()
        ///
        template<int D>
        void disableWorld();
        
        /// \brief Get the renderer
        ///
        /// \return Reference to the renderer
        ///
        Renderer& getRenderer() const;
        
        /// \brief Set the delta time scalar
        ///
        /// The delta time value will be multiplied by this value every frame.
        /// This makes it possible to create slow-down or fast-forward effects.
        ///
        /// \warning The delta scale is allowed to be zero. You should ensure
        ///          that divisions by zero won't occur because of this.
        /// 
        /// \param scale The scale to set
        ///
        /// \see Engine::getDeltaTimeUnscaled()
        ///
        void setDeltaScale(const float scale);

        /// \brief Get the delta time scalar
        ///
        /// \return The delta time scalar
        ///
        float getDeltaScale() const;

        /// \brief Send a message to this scene
        ///
        /// The message will be forwarded to the objects, should it
        /// pass the filter.
        ///
        /// \param message The message
        ///
        /// \return The message result
        ///
        Message::Result sendMessage(const Message& message);

        /// \brief Base update
        ///
        /// This will call preUpdate() and postUpdate().
        ///
        /// \param deltaTime The delta time
        ///
        void updateBase(const float deltaTime);
        
        /// \brief Pre-update
        ///
        /// This will be called before objects are updated.
        ///
        /// \param deltaTime The delta time
        ///
        virtual void preUpdate(const float deltaTime);

        /// \brief Post-update
        ///
        /// This will be called after objects are updated.
        ///
        /// \param deltaTime The delta time
        ///
        virtual void postUpdate(const float deltaTime);

        /// \brief Get this scene as object
        ///
        /// This can be used to work around the privately inherited jop::Object.
        /// **Do not** ever call this unless you know what you're doing.
        ///
        /// \return Reference to this as Object
        ///
        Object& getAsObject();

        /// \copydoc getAsObject()
        ///
        const Object& getAsObject() const;

    private:

        /// \copydoc Component::receiveMessage()
        ///
        virtual Message::Result receiveMessage(const Message& message);


        std::unique_ptr<Renderer> m_renderer;   ///< The renderer
        float m_deltaScale;                     ///< Delta time scalar
        std::tuple<World2D*, World*> m_worlds;  ///< 2D and 3D worlds
        World m_cullingWorld;
        std::unique_ptr<detail::CullingBroadphaseCallback> m_broadphaseCallback;
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Scene.inl>
}

/// \class jop::Scene
/// \ingroup core

#endif