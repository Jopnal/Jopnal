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

#ifndef JOP_COMPONENT_HPP
#define JOP_COMPONENT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class Object;

    class JOP_API Component
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Component);

        friend class Object;

        /// \brief Clone function
        ///
        /// \param newObj The object for the cloned component
        ///
        /// \return Pointer to the cloned component
        /// 
        virtual Component* clone(Object& newObj) const = 0;

    protected:

        /// \brief Copy constructor
        ///
        /// \param other The other component to be copied
        /// \param newObj The new object
        ///
        Component(const Component& other, Object& newObj);

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Component identifier
        ///
        Component(Object& object, const std::string& ID);

    public:

        /// \brief Virtual destructor
        ///
        virtual ~Component() = 0;


        /// \brief Function to handle messages
        ///
        /// \param message String holding the message
        ///
        /// \return Message result
        ///
        Message::Result sendMessage(const std::string& message);

        /// \brief Function to handle messages
        ///
        /// \param message String holding the message
        /// \param returnWrap Pointer to hold extra data
        ///
        /// \return Message result
        ///
        Message::Result sendMessage(const std::string& message, Any& returnWrap);

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        /// \return Message result
        ///
        Message::Result sendMessage(const Message& message);

        /// \brief Update function for component
        ///
        /// \param deltaTime The delta time
        ///
        virtual void update(const float deltaTime);

        /// \brief Get the identifier
        ///
        /// \return Reference to the identifier
        ///
        const std::string& getID() const;

        /// \brief Set the identifier
        ///
        /// \param ID The new identifier
        ///
        /// \comm setID
        ///
        void setID(const std::string& ID);

        /// \brief Get the object this component is bound to
        ///
        /// \return Reference to the object
        ///
        WeakReference<Object> getObject();

        /// \copydoc jop::Component::getObject()
        ///
        WeakReference<const Object> getObject() const;

        /// \brief Check if this component is active
        ///
        /// This is the same as calling getObject()->isActive().
        ///
        /// \return True if active
        ///
        bool isActive() const;

        /// \brief Remove this component from its object
        ///
        /// The component will be removed immediately.
        ///
        void removeSelf();
        
    private:

        /// \brief Set active
        ///
        /// Some components need to know if the object they're bound to
        /// goes active/inactive. This virtual method will be called by the object when
        /// that happens.
        ///
        virtual void setActive(const bool active);

        /// \brief Virtual sendMessage
        ///
        virtual Message::Result sendMessageImpl(const Message& message);


        std::string m_ID;                   ///< Identifier
        WeakReference<Object> m_objectRef;  ///< Reference to the object this component is bound to
    };
}

#endif

/// \class Component
/// \ingroup core