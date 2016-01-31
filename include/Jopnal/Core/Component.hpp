// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_COMPONENT_HPP
#define JOP_COMPONENT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <memory>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class Object;

    class JOP_API Component : public std::enable_shared_from_this<Component>
    {
    private:

        JOP_DISALLOW_MOVE(Component);

        void operator =(const Component&) = delete;

    protected:

        /// \brief Copy constructor
        ///
        Component(const Component& other);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Unique component identifier
        ///
        Component(Object& object, const std::string& ID);

        /// \brief Virtual destructor
        ///
        virtual ~Component() = 0;


        /// \brief Copy this component
        ///
        virtual Component* clone() const = 0;


        /// \brief Function to handle messages
        ///
        /// \param message String holding the message
        /// \param ptr Pointer to hold extra data
        ///
        virtual MessageResult sendMessage(const std::string& message, void* ptr);

        /// \brief Update function for component
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void update(const double deltaTime);

        /// \brief Fixed update function for component
        ///
        /// \param timeStep Double holding time step
        ///
        virtual void fixedUpdate(const double timeStep);

        /// \brief Function to get components unique identifier m_ID
        ///
        const std::string& getID() const;

        /// \brief Function to set components unique identifier m_ID
        ///
        void setID(const std::string& ID);

        /// \brief Get the object this component is bound to
        ///
        /// \return Reference to the object
        ///
        Object& getObject();

        /// \copydoc jop::Component::getObject()
        ///
        const Object& getObject() const;
        
    private:

        std::string m_ID;       ///< Unique component identifier
        Object& m_objectRef;    ///< Reference to the object this component is bound to
    };
}

#endif

/// \class Component
/// \ingroup core