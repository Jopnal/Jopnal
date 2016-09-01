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
        /// If you want your component to be copyable, you should override
        /// this function, either manually or by using the 
        /// JOP_GENERIC_COMPONENT_CLONE macro. If you don't do this,
        /// attempting to copy the component during object clone will produce errors.
        ///
        /// \param newObj The object for the cloned component
        ///
        /// \return Pointer to the cloned component
        /// 
        virtual Component* clone(Object& newObj) const;

    protected:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Identifier. If you don't plan on using this for anything, pass 0
        ///
        Component(Object& object, const uint32 ID);

        /// \brief Copy constructor
        ///
        /// This copy constructor differs in signature from the standard. Cloned components
        /// often need to know their objects during construction, and thus needs to be
        /// passed in here. Your own components' copy constructors should be defined
        /// the same way. See clone(Object&).
        ///
        /// \param other The other component to be copied
        /// \param newObj The new object
        ///
        Component(const Component& other, Object& newObj);

    public:

        /// \brief Virtual destructor
        ///
        virtual ~Component() = 0;


        /// \brief Send a message to this component
        ///
        /// \param message The message
        ///
        /// \return Message result
        ///
        Message::Result sendMessage(const Message& message);

        /// \brief Update function
        ///
        /// \param deltaTime The delta time
        ///
        virtual void update(const float deltaTime);

        /// \brief Get the identifier
        ///
        /// \return The identifier
        ///
        uint32 getID() const;

        /// \brief Set the identifier
        ///
        /// \param ID The new identifier
        ///
        /// \comm setID
        ///
        void setID(const uint32 ID);

        /// \brief Get the object this component is bound to
        ///
        /// \return Reference to the object
        ///
        WeakReference<Object> getObject();

        /// \copydoc getObject()
        ///
        WeakReference<const Object> getObject() const;

        /// \brief Check if this component is active
        ///
        /// This is the same as calling %getObject()->%isActive().
        ///
        /// \return True if active
        ///
        bool isActive() const;

        /// \brief Remove this component from its object
        ///
        /// The component will be removed immediately.
        ///
        void removeSelf();

    protected:

        /// \brief Receive a message
        ///
        /// Override this to handle messages sent to this.
        /// **Don't forget to call the base class' method as well
        /// to ensure that the message gets forwarded correctly.**
        ///
        /// \param message The message
        ///
        /// \return The message result
        ///
        virtual Message::Result receiveMessage(const Message& message);

    private:

        uint32 m_ID;                        ///< Identifier
        WeakReference<Object> m_objectRef;  ///< Reference to the object this component is bound to
    };
}

/// \class jop::Component
/// \ingroup core

#endif