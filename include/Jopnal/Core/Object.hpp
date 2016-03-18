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

#ifndef JOP_OBJECT_HPP
#define JOP_OBJECT_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <Jopnal/Graphics/Transform.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Object : public Transform, public SafeReferenceable<Object>
    {
    private:

        JOP_DISALLOW_COPY(Object);

        friend class StateLoader;

    public:

        /// \brief Constructor
        ///
        /// \param ID Object id
        ///
        Object(const std::string& ID);

        /// \brief Copy constructor
        /// 
        /// \param other The other object to copy
        /// \param newName The ID of the new object
        ///
        Object(const Object& other, const std::string& newID);

        /// \copydoc Object(const Object&,const std::string&)
        ///
        /// \param newTransform Transform for the new object
        ///
        Object(const Object& other, const std::string& newID, const Transform& newTransform);

        /// \brief Move constructor
        ///
        /// \param other The other object to move
        ///
        Object(Object&& other);

        /// \brief Move assignment operator
        ///
        /// \param other The other object to move
        ///
        Object& operator =(Object&& other);


        /// \brief Get a component with the given id
        ///
        /// \param ID Component identifier to search with
        ///
        /// \return Pointer to the component, nullptr if the component wasn't found
        ///
        Component* getComponent(const std::string& ID);

        /// \copycod getComponent()
        ///
        const Component* getComponent(const std::string& ID) const;

        
        /// \brief Get all components
        ///
        /// \return Reference to the internal vector with the components
        ///
        const std::vector<std::unique_ptr<Component>>& getComponents() const;

        /// \brief Get a component using type info
        ///
        /// \return Pointer to the component. Nullptr if not found
        ///
        template<typename T>
        T* getComponent();

        /// \copydoc getComponent()
        ///
        template<typename T>
        const T* getComponent() const;

        /// \brief Template function to create components
        ///
        /// \param args Arguments to use with construction
        ///
        template<typename T, typename ... Args>
        T& createComponent(Args&&... args);

        /// \brief Method to remove components with 'ID'
        /// 
        /// \param ID Unique object identifier m_ID
        ///
        void removeComponents(const std::string& ID);

        /// \brief Remove all components
        ///
        void clearComponents();

        /// \brief Get amount of components
        ///
        /// \return Amount of components
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

        /// \brief Get all children
        ///
        /// \return Reference to the internal vector with the components
        ///
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

        /// \copydoc cloneChild(const std::string&, const std::string&)
        ///
        /// \param newTransform 
        ///
        WeakReference<Object> cloneChild(const std::string& ID, const std::string& clonedID, const Transform& newTransform);



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
        /// \return Amount of children
        ///
        unsigned int childCount() const;

        /// \brief Get amount of children recursively
        ///
        /// Goes through the children and their children all the way down the tree
        /// and return the total amount of children
        ///
        /// \return Amount of children, summed recursively
        ///
        unsigned int childCountRecursive() const;


        /// \brief Set this object to ignore its parent
        ///
        /// This only affects transformations. Objects that ignore their parent will not take
        /// into account the parent's tranformation.
        ///
        /// \param ignore The flag to set
        ///
        void setIgnoreParent(const bool ignore);

        /// \brief Check if this object ignores its parent
        ///
        /// \return True if ignores parent
        ///
        bool ignoresParent() const;


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


        /// \brief Set this object active/inactive
        ///
        /// \param active Activity flag to set
        ///
        void setActive(const bool active);

        /// \brief Check if this object is active
        ///
        /// \return True if active
        ///
        bool isActive() const;


        /// \brief Get this object's id
        ///
        /// \return Reference to the internal id string
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

        /// \brief Update the transformation tree
        ///
        /// This is automatically called by Scene
        ///
        void updateTransformTree(const Object* parent, bool parentUpdated);

    private:

        std::vector<Object> m_children;                       ///< Container holding this object's children
        std::vector<std::unique_ptr<Component>> m_components; ///< Container holding components
        std::string m_ID;                                     ///< Unique object identifier
        WeakReference<Object> m_parent;
        bool m_ignoreParent;
        bool m_active;
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Object.inl>
}

#endif

/// \class Object 
/// \ingroup graphics