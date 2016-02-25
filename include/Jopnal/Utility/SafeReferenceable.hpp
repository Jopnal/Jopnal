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
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    // Forward declarations
    template<typename T>
    class SafeReferenceable;
    template<typename T>
    class WeakReference;

    //////////////////////////////////////////////

    template<typename To, typename From>
    WeakReference<To> static_ref_cast(const WeakReference<From>& other);

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

        SafeReferenceable(T* ref);

        SafeReferenceable(SafeReferenceable<T>&& other);

        SafeReferenceable& operator =(SafeReferenceable<T>&& other);

        ~SafeReferenceable();

    public:

        WeakReference<T> getReference() const;

    private:

        std::shared_ptr<T*> m_ref;

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

        WeakReference() = default;

        WeakReference(const SafeReferenceable<T>& ref);


        bool expired() const;

        T* get();

        const T* get() const;

        T& operator *();

        const T& operator *() const;

        T* operator ->();

        const T* operator ->() const;

        operator bool() const;

        WeakReference& operator =(const SafeReferenceable<T>& other);


    private:

        std::weak_ptr<T*> m_ref;

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/SafeReferenceable.inl>
}

#endif