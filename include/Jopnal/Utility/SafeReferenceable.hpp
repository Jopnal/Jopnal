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

#ifndef JOP_SAFEREFERENCEABLE_HPP
#define JOP_SAFEREFERENCEABLE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Assert.hpp>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    template<typename T>
    class SafeReferenceable;
    template<typename T>
    class WeakReference;

    //////////////////////////////////////////////

    /// \brief Statically cast a weak reference
    ///
    /// \param other The reference to cast
    ///
    /// \return Weak reference with the cast type
    ///
    template<typename To, typename From>
    WeakReference<To> static_ref_cast(const WeakReference<From>& other);

    /// \brief Dynamically cast a weak reference
    ///
    /// \param other The reference to cast
    ///
    /// \return Weak reference with the cast type. Empty if unsuccessful
    ///
    template<typename To, typename From>
    WeakReference<To> dynamic_ref_cast(const WeakReference<From>& other);

    //////////////////////////////////////////////

    template<typename T>
    class SafeReferenceable
    {
    private:

        JOP_DISALLOW_COPY(SafeReferenceable);

        friend class WeakReference<T>;

    protected:

        /// \brief Constructor
        ///
        /// \param ref Pointer to the object to bind
        ///
        SafeReferenceable(T* ref);

        /// \brief Move constructor
        ///
        /// \param other The object to move
        ///
        SafeReferenceable(SafeReferenceable<T>&& other);

        /// \brief Move assignment operator
        ///
        /// \param other The object to move
        ///
        SafeReferenceable& operator =(SafeReferenceable<T>&& other);

        /// \brief Protected destructor
        ///
        ~SafeReferenceable();

    public:

        /// \brief Get a weak reference to the bound object
        ///
        /// \return New weak reference to the bound object
        ///
        WeakReference<T> getReference() const;

    private:

        std::shared_ptr<T*> m_ref;  ///< The bound reference
    };

    //////////////////////////////////////////////

    template<typename T>
    class WeakReference
    {
    private:

        template<typename To, typename From>
        friend WeakReference<To> static_ref_cast(const WeakReference<From>&);

        template<typename To, typename From>
        friend WeakReference<To> dynamic_ref_cast(const WeakReference<From>&);

    public:

        /// \brief Default constructor
        ///
        /// Initializes the internal reference to be empty
        ///
        WeakReference() = default;

        /// \brief Constructor for initializing from a SafeReferenceable object
        ///
        /// \param ref The object to bind
        ///
        WeakReference(const SafeReferenceable<T>& ref);


        /// \brief Check if this reference is expired
        ///
        /// \return True if expired
        ///
        bool expired() const;

        /// \brief Reset this reference
        ///
        /// After this call, this reference won't point to anything.
        ///
        void reset();

        /// \brief Get a pointer to the bound object
        ///
        /// \return Pointer to the object. nullptr if expired
        ///
        T* get();

        /// \copydoc get()
        ///
        const T* get() const;

        /// \brief Dereference operator
        ///
        /// Calling this operator when this reference is expired will
        /// cause a segmentation fault.
        ///
        /// \return Reference to the bound object
        ///
        T& operator *();

        /// \copydoc operator*()
        ///
        const T& operator *() const;

        /// \copydoc operator*()
        ///
        operator T&();

        /// \copydoc operator*()
        ///
        operator const T&() const;

        /// \copydoc get()
        ///
        T* operator ->();

        /// \copydoc get()
        ///
        const T* operator ->() const;

        /// \brief Check if this reference is not expired
        ///
        /// This is the same as !expired()
        ///
        /// \return True if not expired
        ///
        operator bool() const;

        /// \brief Assignment operator for assigning a SafeReferenceable object
        ///
        /// \param other The object to bind
        ///
        /// \return Reference to self
        ///
        WeakReference& operator =(const SafeReferenceable<T>& other);

    private:

        std::weak_ptr<T*> m_ref;    ///< The bound reference
    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/SafeReferenceable.inl>
}

/// \class jop::SafeReferenceable
/// \ingroup utility

#endif