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

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Core/SerializeInfo.hpp>
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <Jopnal/Graphics/Transform.hpp>
#include <Jopnal/STL.hpp>
#include <unordered_set>
#include <memory>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class Scene;

    class JOP_API Object : public SafeReferenceable<Object>, public SerializeInfo
    {
    private:

        JOP_DISALLOW_COPY(Object);

        friend class SceneLoader;
        friend class Component;

        enum Flag : uint32
        {
            ActiveFlag          = 1,
            RemoveFlag          = 1 << 2,
            ChildrenRemovedFlag = 1 << 3,

            // Transformations
            MatrixDirty         = 1 << 4,
            InverseMatrixDirty  = 1 << 5,

            GlobalRotationDirty = 1 << 6,
            GlobalScaleDirty    = 1 << 7,
            GlobalPositionDirty = 1 << 8,

            TransformDirty      = MatrixDirty | InverseMatrixDirty | GlobalRotationDirty | GlobalScaleDirty | GlobalPositionDirty
        };

    public:

        /// Transformation restrictions
        ///
        enum TransformRestriction : uint32
        {
            TranslationX = 1 << 10, ///< Translation on the X axis
            TranslationY = 1 << 11, ///< Translation on the Y axis
            TranslationZ = 1 << 12, ///< Translation on the Z axis

            /// Translation on all axes
            Translation = TranslationX | TranslationY | TranslationZ,

            ScaleX = 1 << 13,   ///< Scale on the X axis
            ScaleY = 1 << 14,   ///< Scale on the Y axis
            ScaleZ = 1 << 15,   ///< Scale on the Z axis

            /// Scale on all axes
            Scale = ScaleX | ScaleY | ScaleZ,

            /// Rotation on all axes
            Rotation = 1 << 16,

            /// Ignore parent transform completely
            IgnoreParent = Translation | Scale | Rotation
        };

    public:

        /// \brief Constructor
        ///
        /// \param ID Object identifier
        ///
        Object(const std::string& ID);

        /// \brief Copy constructor
        /// 
        /// \param other The other object to copy
        /// \param newID The ID of the new object
        /// \param newTransform The transform values of the new object
        ///
        Object(const Object& other, const std::string& newID, const Transform::Variables& newTransform);

        /// \brief Move constructor
        ///
        Object(Object&& other);

        /// \brief Move assignment operator
        ///
        /// \return Reference to self
        ///
        Object& operator =(Object&& other);

        /// \brief Destructor
        ///
        ~Object();

        
        /// \brief Get all components
        ///
        /// \return Reference to the internal vector with the components
        ///
        std::vector<std::unique_ptr<Component>>& getComponents();

        /// \copydoc getComponents()
        ///
        const std::vector<std::unique_ptr<Component>>& getComponents() const;

        /// \brief Get a component using type info
        ///
        /// \return Pointer to the component. nullptr if not found
        ///
        template<typename T>
        T* getComponent();

        /// \copydoc getComponent()
        ///
        template<typename T>
        const T* getComponent() const;

        /// \brief Get a component using component ID and type info
        ///
        /// \param ID The identifier to search with
        ///
        /// \return Pointer to the component. nullptr if not found
        ///
        template<typename T>
        T* getComponent(const uint32 ID);

        /// \copydoc getComponent(const uint32)
        ///
        template<typename T>
        const T* getComponent(const uint32 ID) const;

        /// \brief Create a component
        ///
        /// \param args Arguments to use with construction
        ///
        /// \return Reference to the newly created component
        ///
        template<typename T, typename ... Args>
        T& createComponent(Args&&... args);

        /// \brief Clone a component
        ///
        /// \param object New object for the cloned component
        /// \param ID Identifier of the component to clone
        ///
        /// \return Pointer to the cloned component. nullptr if not found
        ///
        template<typename T>
        T* cloneComponent(Object& object, const uint32 ID = 0) const;

        /// \brief Remove components
        ///
        /// All components matching the identifier will be removed
        /// 
        /// \param ID The ID to search with
        ///
        /// \return Reference to self
        ///
        /// \comm removeComponents
        /// 
        Object& removeComponents(const uint32 ID);

        /// \brief Remove a component
        ///
        /// \param ID The ID to search with
        ///
        /// \return Reference to self 
        ///
        template<typename T>
        Object& removeComponent(const uint32 ID = 0);

        /// \brief Remove all components
        ///
        /// \return Reference to self
        ///
        /// \comm clearComponents
        /// 
        Object& clearComponents();

        /// \brief Get amount of components
        ///
        /// \return Amount of components
        ///
        unsigned int componentCount() const;

        /// \brief Create a new child
        ///
        /// \param ID Id for the new child object
        ///
        /// \return Reference to the newly created child
        ///
        /// \comm createChild
        ///
        WeakReference<Object> createChild(const std::string& ID);

        /// \brief Adopt a child
        ///
        /// This will move the child onto this object and remove it from its old parent.
        /// If the child's parent is equal to this, this function does nothing.
        ///
        /// \param child The child to adopt
        ///
        /// \return Reference to the adopted child. The old reference will become invalid
        ///
        /// \comm adoptChild
        ///
        WeakReference<Object> adoptChild(Object& child);

        /// \brief Get all children
        ///
        /// \return Reference to the internal vector with the children
        ///
        std::vector<Object>& getChildren();

        /// \copydoc getChildren()
        ///
        const std::vector<Object>& getChildren() const;

        /// \brief Clone a child with the given id
        ///
        /// The child object, if successfully cloned, will be added to the internal array
        /// and then returned.
        ///
        /// \param ID The ID to search with
        /// \param clonedID The ID of the cloned object
        ///
        /// \return Reference to the newly cloned child object if the object was found, empty otherwise
        ///
        /// \comm cloneChild
        /// 
        WeakReference<Object> cloneChild(const std::string& ID, const std::string& clonedID);

        /// \copydoc cloneChild(const std::string&, const std::string&)
        ///
        /// \param newTransform New transform for the cloned child
        /// 
        WeakReference<Object> cloneChild(const std::string& ID, const std::string& clonedID, const Transform::Variables& newTransform);

        /// \brief Remove children with the given identifier
        ///
        /// The children will actually be removed only at the beginning of the next update call.
        ///
        /// \param ID The id to search with
        ///
        /// \return Reference to self
        ///
        /// \comm removeChildren
        /// 
        Object& removeChildren(const std::string& ID);

        /// \brief Remove all children with the given tag
        ///
        /// The children will actually be removed only at the beginning of the next update call.
        ///
        /// \param tag The tag to search with
        /// \param recursive Search recursively?
        ///
        /// \return Reference to self
        ///
        Object& removeChildrenWithTag(const std::string& tag, const bool recursive);

        /// \brief Reserve memory for children
        ///
        /// Reallocates children if param size is greater than currently reserved size.
        /// It's recommended to call tis before adding a large amount of children.
        ///
        /// \param size Size to reserve
        ///
        /// \return Reference to self
        ///
        Object& reserveChildren(const uint32 size);

        /// \brief Remove all children
        /// 
        /// The children will be removed immediately.
        ///
        /// \return Reference to self
        ///
        /// \comm clearChildren
        ///
        Object& clearChildren();

        /// \brief Mark this to be removed
        ///
        /// The object will be actually removed at the beginning of the next update call.
        ///
        /// \comm removeSelf
        ///
        void removeSelf();

        /// \brief Clone/instantiate this object
        ///
        /// The same ID and transform will be used.
        ///
        /// \return Reference to the cloned object
        ///
        WeakReference<Object> cloneSelf();

        /// \copybrief cloneSelf()
        ///
        /// The same transform will be used.
        ///
        /// \param newID The ID of the cloned object
        ///
        /// \return Reference to the cloned object
        /// 
        WeakReference<Object> cloneSelf(const std::string& newID);

        /// \copybrief cloneSelf()
        ///
        /// \param newID The ID of the cloned object
        /// \param newTransform The transform of the cloned object
        ///
        /// \return Reference to the cloned object
        ///
        WeakReference<Object> cloneSelf(const std::string& newID, const Transform::Variables& newTransform);

        /// \brief Check if this object has been marked to be removed
        ///
        /// \return True if this is to be removed
        ///
        bool isRemoved() const;

        /// \brief Get amount of children
        ///
        /// \return Amount of children
        ///
        unsigned int childCount() const;

        /// \brief Get amount of children recursively
        ///
        /// \return Amount of children, summed recursively
        ///
        unsigned int childCountRecursive() const;

        /// \brief Get this object's parent
        ///
        /// If the result is empty, it means that this object is a scene.
        ///
        /// \return Reference to the parent
        ///
        WeakReference<Object> getParent() const;

        /// \brief Set a new parent
        ///
        /// This is equivalent to calling newParent.adoptChild(this)
        ///
        /// \param newParent The new parent
        ///
        /// \return Reference to this. The old reference will become invalid
        ///
        /// \comm setParent
        ///
        WeakReference<Object> setParent(Object& newParent);

        /// \brief Get the scene this objects is bound to
        ///
        /// You should avoid calling this when you can use Engine::getCurrentScene()
        /// or Engine::getSharedScene().
        ///
        /// \return Reference to the scene
        ///
        Scene& getScene();

        /// \copydoc getScene()
        ///
        const Scene& getScene() const;

        /// \brief Send a message
        ///
        /// The message will be forwarded to children and components.
        ///
        /// \param message The message
        ///
        /// \return The message result
        ///
        Message::Result sendMessage(const Message& message);

        /// \brief Find a child
        ///
        /// \param ID Object identifier
        /// \param recursive Search recursively?
        /// \param strict Does the ID have to match exactly?
        ///
        /// \return Reference to the child, empty if not found
        ///
        WeakReference<Object> findChild(const std::string& ID, const bool recursive = false, const bool strict = true) const;

        /// \brief Find all children matching the criteria
        ///
        /// When ID is empty and strict is false, all children will be returned.
        /// 
        /// \param ID Object identifier
        /// \param recursive Search recursively?
        /// \param strict Does the ID have to match exactly?
        ///
        /// \return Vector with all the found children, empty if none were found
        ///
        std::vector<WeakReference<Object>> findChildren(const std::string& ID, const bool recursive, const bool strict) const;

        /// \brief Find children with a tag
        ///
        /// \param tag Object identifier
        /// \param recursive Search recursively?
        ///
        /// \return Vector with all the found children, empty if none were found
        ///
        std::vector<WeakReference<Object>> findChildrenWithTag(const std::string& tag, const bool recursive) const;

        /// \brief Find child with a search path
        /// 
        /// \param path Search path
        ///
        /// \return Reference to the child, empty if not found
        ///
        /// \see makeSearchPath()
        ///
        WeakReference<Object> findChildWithPath(const std::string& path) const;

        /// \brief Make a string path through this object's parents
        ///
        /// The path will be in the following format:
        /// `GrandParent>Parent>ThisObject`.
        /// Due to how the format works, the character '>'
        /// in an object ID is forbidden.
        ///
        /// \return String with the path
        ///
        /// \see findChildWithPath()
        ///
        std::string makeSearchPath() const;

        /// \brief Set this object active/inactive
        ///
        /// \param active True to set active
        ///
        /// \return Reference to self
        ///
        /// \comm setActive
        /// 
        Object& setActive(const bool active);

        /// \brief Check if this object is active
        ///
        /// \return True if active
        ///
        bool isActive() const;

        /// \brief Get this object's identifier
        ///
        /// \return Reference to the internal id string
        ///
        /// \see setID()
        ///
        const std::string& getID() const;

        /// \brief Set the identifier
        ///
        /// The character '>' in an object identifier is
        /// forbidden.
        ///
        /// \param ID Object identifier
        ///
        /// \return Reference to self
        ///
        /// \comm setID
        /// 
        Object& setID(const std::string& ID);

        /// \brief Add a tag
        ///
        /// \param tag The tag to add
        ///
        /// \return Reference to self
        ///
        /// \comm addTag
        ///
        Object& addTag(const std::string& tag);

        /// \brief Remove a tag
        ///
        /// \param tag The tag to be removed
        ///
        /// \return Reference to self
        ///
        /// \comm removeTag
        ///
        Object& removeTag(const std::string& tag);

        /// \brief Clear tags
        ///
        /// \return Reference to self
        ///
        /// \comm clearTags
        ///
        Object& clearTags();

        /// \brief Check if this object has a tag
        ///
        /// \param tag The tag to check
        ///
        /// \return True if found
        ///
        bool hasTag(const std::string& tag) const;

        /// \brief Update
        ///
        /// This is for internal use.
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime);

        /// \brief Print the tree of objects into the console, this object as root
        ///
        void printDebugTree() const;

    public: // Transformations

        /// \brief Get the transform
        ///
        /// \return Reference to the internal transform
        ///
        const Transform& getTransform() const;

        /// \brief Get the inverse transform
        ///
        /// \return Reference to the internal inverse transform
        ///
        const Transform& getInverseTransform() const;

        /// \brief Get the local transformation variables
        ///
        /// \return Reference to the internal variables
        ///
        const Transform::Variables& getLocalTransformVars() const;

        /// \brief Get the global transformation variables
        ///
        /// \return Reference to the internal variables
        ///
        const Transform::Variables& getGlobalTransformVars() const;

        /// \brief Set the rotation
        ///
        /// \param x The X angle
        /// \param y The Y angle
        /// \param z The Z angle
        ///
        /// \return Reference to self
        ///
        Object& setRotation(const float x, const float y, const float z);

        /// \brief Set the rotation
        ///
        /// \param rotation Rotation vector in euler angles
        ///
        /// \return Reference to self
        ///
        Object& setRotation(const glm::vec3& rotation);

        /// \brief Set the rotation using angle-axis
        ///
        /// \param angle The angle
        /// \param axis The axis
        ///
        /// \return Reference to self
        ///
        Object& setRotation(const float angle, const glm::vec3& axis);

        /// \brief Set the rotation
        ///
        /// \param rotation Quaternion with the rotation to set
        ///
        /// \return Reference to self
        ///
        Object& setRotation(const glm::quat& rotation);

        /// \brief Get the local rotation
        ///
        /// \return The local rotation
        ///
        const glm::quat& getLocalRotation() const;

        /// \brief Get the global rotation
        ///
        /// \return The global rotation
        ///
        const glm::quat& getGlobalRotation() const;

        /// \brief Get the global front vector
        ///
        /// \return The global front vector
        ///
        glm::vec3 getGlobalFront() const;

        /// \brief Get the global right vector
        ///
        /// \return The global right vector
        ///
        glm::vec3 getGlobalRight() const;

        /// \brief Get the global up vector
        ///
        /// \return The global up vector
        ///
        glm::vec3 getGlobalUp() const;

        /// \brief Get the local front vector
        ///
        /// \return The local front vector
        ///
        glm::vec3 getLocalFront() const;

        /// \brief Get the local right vector
        ///
        /// \return The local right vector
        ///
        glm::vec3 getLocalRight() const;

        /// \brief Get the local up vector
        ///
        /// \return The local up vector
        ///
        glm::vec3 getLocalUp() const;

        /// \brief Set the scale
        ///
        /// 1.f means the original scale.
        ///
        /// \param x The X component
        /// \param y The Y component
        /// \param z The Z component
        ///
        /// \return Reference to self
        ///
        Object& setScale(const float x, const float y, const float z);

        /// \brief Set the scale
        ///
        /// \param scale Vector with the scale to set
        ///
        /// \return Reference to self
        ///
        Object& setScale(const glm::vec3& scale);

        /// \brief Set the scale
        ///
        /// This call is equal to setScale(delta, delta, delta)
        ///
        /// \param delta The new scale
        ///
        /// \return Reference to self
        ///
        Object& setScale(const float delta);

        /// \brief Get the local scale
        ///
        /// \return Vector with the scale
        ///
        const glm::vec3& getLocalScale() const;

        /// \brief Get the global scale
        ///
        /// \return The global scale
        ///
        const glm::vec3& getGlobalScale() const;

        /// \brief Set the position
        ///
        /// \param x The X component
        /// \param y The Y component
        /// \param z The Z component
        ///
        /// \return Reference to self
        ///
        Object& setPosition(const float x, const float y, const float z);

        /// \brief Set the position
        ///
        /// \param position Vector with the position to set
        ///
        /// \return Reference to self
        ///
        Object& setPosition(const glm::vec3& position);

        /// \brief Get the local position
        ///
        /// \return The local position
        ///
        const glm::vec3& getLocalPosition() const;

        /// \brief Get the global position
        ///
        /// \return The global position
        ///
        const glm::vec3& getGlobalPosition() const;

        /// \brief Set this transform to look at a certain point
        ///
        /// The rotation is applied locally. If you want to set the rotation
        /// to point at an absolute world position, use setIgnoreTransform() to
        /// restrict the rotation.
        ///
        /// This overload will use Transform::Up as the up vector.
        ///
        /// \param point The point to look at
        ///
        /// \return Reference to self
        ///
        Object& lookAt(const glm::vec3& point);

        /// \copydoc lookAt()
        ///
        /// \param up Custom up vector
        ///
        Object& lookAt(const glm::vec3& point, const glm::vec3& up);

        /// \brief Set this transform to look at a certain point
        ///
        /// \param x The X point
        /// \param y The Y point
        /// \param z The Z point
        ///
        /// \return Reference to self
        ///
        Object& lookAt(const float x, const float y, const float z);

        /// \brief Move this object
        ///
        /// \param x The X offset
        /// \param y The Y offset
        /// \param z The Z offset
        ///
        /// \returns Reference to self
        ///
        Object& move(const float x, const float y, const float z);

        /// \brief Move this object
        ///
        /// \param offset The movement offset
        ///
        /// \return Reference to self
        ///
        Object& move(const glm::vec3& offset);

        /// \brief Rotate this object
        ///
        /// \param x The X offset
        /// \param y The Y offset
        /// \param z The Z offset
        ///
        /// \returns Reference to self
        ///
        Object& rotate(const float x, const float y, const float z);

        /// \brief Rotate this object
        ///
        /// \param rotation The rotation offset
        ///
        /// \return Reference to self
        ///
        Object& rotate(const glm::quat& rotation);

        /// \copydoc rotate(const glm::quat&)
        ///
        Object& rotate(const glm::vec3& rotation);

        /// \brief Rotate this object using an axis-angle
        ///
        /// \param angle The angle
        /// \param axis The axis
        /// 
        /// \return Reference to self
        ///
        Object& rotate(const float angle, const glm::vec3& axis);

        /// \brief Scale this object
        ///
        /// \param x The X multiplier
        /// \param y The Y multiplier
        /// \param z The Z multiplier
        ///
        /// \returns Reference to self
        ///
        Object& scale(const float x, const float y, const float z);

        /// \brief Scale this object
        ///
        /// \param scale The scale multiplier
        ///
        /// \return Reference to self
        ///
        Object& scale(const glm::vec3& scale);

        /// \brief Scale this object
        ///
        /// This call is equal to scale(delta, delta, delta).
        ///
        /// \param delta The scale multiplier
        ///
        /// \return Reference to self
        ///
        Object& scale(const float delta);

        /// \brief Set this node to ignore its parent
        ///
        /// Objects that ignore their parent will not take
        /// into account the parent's transformation.
        ///
        /// \param ignore The flag to set
        ///
        /// \return Reference to self
        ///
        Object& setIgnoreParent(const bool ignore);

        /// \brief Check if this node ignores its parent
        ///
        /// \return True if ignores parent
        ///
        bool ignoresParent() const;

        /// \brief Set transform restriction flags
        ///
        /// Use this function to ignore certain global transformations. The
        /// local transformation values remain unaffected.
        ///
        /// \param flags The flags to set
        ///
        /// \return Reference to self
        ///
        /// \see TransformRestriction
        ///
        Object& setIgnoreTransform(const uint32 flags);

        /// \brief Check if a transform restriction has been set
        ///
        /// You should only check a single flag at a time.
        ///
        /// \param flag The flag to check
        ///
        /// \return True if the flag is set
        ///
        bool ignoresTransform(const uint32 flag) const;

    private:

        void printDebugTreeImpl(std::vector<uint32> spacing, const bool isLast) const;

        void sweepRemoved();

        bool flagSet(const uint32 flag) const;

        void setFlags(const uint32 flags) const;

        void clearFlags(const uint32 flags) const;

        void setFlagsIf(const uint32 flags, const bool cond) const;

        void propagateFlags(const uint32 flags);


        // Transformation
        mutable Transform m_transform;
        mutable Transform m_inverseTransform;
        mutable Transform::Variables m_locals;
        mutable Transform::Variables m_globals;

        // Object
        std::vector<Object> m_children;                         ///< Container holding this object's children
        std::vector<std::unique_ptr<Component>> m_components;   ///< Container holding components
        std::unordered_set<std::string> m_tags;                 ///< Container holding tags
        std::string m_ID;                                       ///< Unique object identifier
        WeakReference<Object> m_parent;                         ///< The parent
        mutable uint32 m_flags;                                 ///< Flags
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Object.inl>
}

/// \class jop::Object 
/// \ingroup core

#endif