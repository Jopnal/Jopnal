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


template<typename T>
T* Object::getComponent()
{
    static_assert(std::is_base_of<Component, T>::value, "Object::getComponent(): Tried to get a component that doesn't inherit from jop::Component");

    const std::type_info& ti = typeid(T);

    for (auto& i : m_components)
    {
        if (typeid(*i) == ti)
            return static_cast<T*>(i.get());
    }

    return nullptr;
}

template<typename T>
const T* Object::getComponent() const
{
    static_assert(std::is_base_of<Component, T>::value, "Object::getComponent(): Tried to get a component that doesn't inherit from jop::Component");

    const std::type_info& ti = typeid(T);

    for (auto& i : m_components)
    {
        if (typeid(*i) == ti)
            return static_cast<const T*>(i.get());
    }

    return nullptr;
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Object::createComponent(Args&&... args)
{
    static_assert(std::is_base_of<Component, T>::value, "Object::createComponent(): Tried to create a component that doesn't inherit from jop::Component");
    
    m_components.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
    return static_cast<T&>(*m_components.back());
}