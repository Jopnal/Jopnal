//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_OBJECT_HPP
#define JOP_OBJECT_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Activateable.hpp>
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <Jopnal/Graphics/Camera.hpp>
#include <Jopnal/Graphics/LightSource.hpp>
#include <Jopnal/Graphics/Transform.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Layer;

    class JOP_API Object : public Transform, public Activateable, public SafeReferenceable<Object>
    {
    private:

        void operator =(const Object&) = delete;

        friend class StateLoader;

    public:

        /// \brief Default constructor
        ///
        Object();

        /// \brief Constructor
        ///
        /// \param ID Unique object identifier m_ID
        ///
        Object(const std::string& ID);

        /// \brief Copy constructor
        ///
        Object(const Object& other);

        /// \brief Overloaded copy constructor
        ///
        Object(const Object& other, const std::string& newName);

        /// \brief Move constructor
        ///
        Object(Object&& other);

        /// \brief Move assignment operator
        ///
        Object& operator =(Object&& other);


        /// \brief Get a component with the given id
        ///
        /// \param ID Component identifier to search with
        ///
        /// \return Weak reference with the component, empty if the component wasn't found
        ///
        WeakReference<Component> getComponent(const std::string& ID);

        const std::vector<std::unique_ptr<Component>>& getCmponents() const;

        /// \brief Get a component using type info
        ///
        /// \return Pointer to the component. Empty if not found
        ///
        template<typename T>
        WeakReference<T> getComponent();

        /// \brief Template function to create components
        ///
        /// \param args User determined arguments
        ///
        template<typename T, typename ... Args>
        WeakReference<T> createComponent(Args&... args);

        /// \brief Method to remove components with 'ID'
        /// 
        /// \param ID Unique object identifier m_ID
        ///
        void removeComponents(const std::string& ID);

        /// \brief Get amount of components
        ///
        unsigned int componentCount() const;

        /// \brief Create a new child
        ///
        /// \param ID Id for the new object
        ///
        /// \return Reference to the newly created child
        ///
        WeakReference<Object> createChild(const std::string& ID);

        /// \brief Get a child with the given id
        ///
        /// \param ID The id to search with
        ///
        /// \return Pointer to the child if found, nullptr otherwise
        ///
        WeakReference<Object> getChild(const std::string& ID);

        const std::vector<Object>& getChildren() const;

        /// \brief Clone a child with the given id
        ///
        /// The child object, if successfully cloned, will be added to the internal array
        /// and then returned.
        ///
        /// \param ID The id to search with
        /// \param clonedID The id of the cloned object
        ///
        /// \return Pointer to the newly cloned child object if the object was found, nullptr otherwise
        ///
        WeakReference<Object> cloneChild(const std::string& ID, const std::string& clonedID);

        /// \brief Remove children with the given id
        ///
        /// \param ID The id to search with
        ///
        void removeChildren(const std::string& ID);

        /// \brief Remove all children
        ///
        void clearChildren();

        /// \brief Get amount of children
        ///
        unsigned int childCount() const;

        /// \brief Get amount of children recursively
        ///
        /// Goes through the children and their children all the way down the tree
        /// and return the total amount of children
        ///
        unsigned int childCountRecursive() const;


        /// \brief Method to send messages
        ///
        /// Forwards messages to this object's components
        ///
        /// \param message String holding the message
        ///
        Message::Result sendMessage(const std::string& message);

        /// \brief Method to send messages
        ///
        /// Forwards messages to this object's components
        ///
        /// \param message String holding the message
        /// \param returnWrap Pointer to hold extra data
        ///
        Message::Result sendMessage(const std::string& message, Any& returnWrap);

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        Message::Result sendMessage(const Message& message);


        /// \brief Method for getting m_ID
        ///
        const std::string& getID() const;

        /// \brief method for setting the ID
        ///
        /// \param ID unique object identifier
        ///
        void setID(const std::string& ID);

        /// \brief Update method for object - forwarded for its components
        ///
        /// \param deltaTime Double holding delta time
        ///
        void update(const float deltaTime); 

        /// \brief Fixed Update method for object - forwarded for its components
        ///
        /// \param timeStep Double holding time step
        ///
        void fixedUpdate(const float timeStep);

        /// \brief Update the transformation tree
        ///
        /// This is automatically called by Scene
        ///
        void updateTransformTree(const Object* parent, bool parentUpdated);

    private:

        std::vector<Object> m_children;                       ///< Container holding this object's children
        std::vector<std::unique_ptr<Component>> m_components; ///< Container holding components
        std::string m_ID;                                     ///< Unique object identifier
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Object.inl>
}

#endif

/// \class Object 
/// \ingroup graphics