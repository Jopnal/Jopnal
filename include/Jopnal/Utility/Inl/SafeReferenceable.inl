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

template<typename To, typename From>
inline WeakReference<To> static_ref_cast(const WeakReference<From>& from)
{
    return reinterpret_cast<const WeakReference<To>&>(from);
}

//////////////////////////////////////////////

template<typename To, typename From>
inline WeakReference<To> dynamic_ref_cast(const WeakReference<From>& from)
{
    if (!from.expired() && dynamic_cast<To*>(*from.m_ref.lock()))
        return static_ref_cast<To>(from);

    return WeakReference<To>();
}

//////////////////////////////////////////////

template<typename T>
SafeReferenceable<T>::SafeReferenceable(T* ref)
    : m_ref(std::make_shared<T*>(nullptr))
{
    *m_ref = ref;
}

template<typename T>
SafeReferenceable<T>::SafeReferenceable(SafeReferenceable<T>&& other)
    : m_ref(std::move(other.m_ref))
{
    if (m_ref)
        *m_ref = static_cast<T*>(this);
}

template<typename T>
SafeReferenceable<T>& SafeReferenceable<T>::operator =(SafeReferenceable<T>&& other)
{
    m_ref = std::move(other.m_ref);

    if (m_ref)
        *m_ref = static_cast<T*>(this);

    return *this;
}

template<typename T>
SafeReferenceable<T>::~SafeReferenceable()
{}

//////////////////////////////////////////////

template<typename T>
inline WeakReference<T> SafeReferenceable<T>::getReference() const
{
    return WeakReference<T>(*this);
}


//////////////////////////////////////////////


template<typename T>
inline WeakReference<T>::WeakReference(const SafeReferenceable<T>& ref)
    : m_ref(ref.m_ref)
{}

//////////////////////////////////////////////

template<typename T>
inline bool WeakReference<T>::expired() const
{
    return m_ref.expired();
}

//////////////////////////////////////////////

template<typename T>
inline void WeakReference<T>::reset()
{
    m_ref.reset();
}

//////////////////////////////////////////////

template<typename T>
inline T* WeakReference<T>::get()
{
    return m_ref.expired() ? nullptr : *m_ref.lock();
}

//////////////////////////////////////////////

template<typename T>
inline const T* WeakReference<T>::get() const
{
    return m_ref.expired() ? nullptr : *m_ref.lock();
}

//////////////////////////////////////////////

template<typename T>
inline T& WeakReference<T>::operator *()
{
    return **m_ref.lock();
}

//////////////////////////////////////////////

template<typename T>
inline const T& WeakReference<T>::operator *() const
{
    return **m_ref.lock();
}

//////////////////////////////////////////////

template<typename T>
inline WeakReference<T>::operator T&()
{
    return *(*this);
}

//////////////////////////////////////////////

template<typename T>
inline WeakReference<T>::operator const T&() const
{
    return *(*this);
}

//////////////////////////////////////////////

template<typename T>
inline T* WeakReference<T>::operator ->()
{
    return get();
}

//////////////////////////////////////////////

template<typename T>
inline const T* WeakReference<T>::operator ->() const
{
    return get();
}

//////////////////////////////////////////////

template<typename T>
inline WeakReference<T>::operator bool() const
{
    return !expired();
}

//////////////////////////////////////////////

template<typename T>
inline WeakReference<T>& WeakReference<T>::operator =(const SafeReferenceable<T>& other)
{
    m_ref = other.m_ref;
    return *this;
}    