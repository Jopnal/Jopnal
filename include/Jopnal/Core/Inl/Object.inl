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


namespace detail
{
    template<typename T = ::jop::Component>
    T* componentGetter(const std::vector<std::unique_ptr<::jop::Component>>& comps, const uint32* ID)
    {
        static_assert(std::is_base_of<::jop::Component, T>::value, "Object::getComponent(): Tried to get a component that doesn't inherit from jop::Component");

        for (auto& i : comps)
        {
            if ((ID == nullptr || i->getID() == *ID) && dynamic_cast<T*>(i.get()) != nullptr)
                return static_cast<T*>(i.get());
        }

        return nullptr;
    }
}

template<typename T>
T* Object::getComponent()
{
    return detail::componentGetter<T>(m_components, nullptr);
}

template<typename T>
const T* Object::getComponent() const
{
    return detail::componentGetter<const T>(m_components, nullptr);
}

//////////////////////////////////////////////

template<typename T>
T* Object::getComponent(const uint32 ID)
{
    return detail::componentGetter<T>(m_components, &ID);
}

template<typename T>
const T* Object::getComponent(const uint32 ID) const
{
    return detail::componentGetter<const T>(m_components, &ID);
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Object::createComponent(Args&&... args)
{
    static_assert(std::is_base_of<Component, T>::value, "Object::createComponent(): Tried to create a component that doesn't inherit from jop::Component");
    
    m_components.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
    return static_cast<T&>(*m_components.back());
}

//////////////////////////////////////////////

template<typename T>
T* Object::cloneComponent(Object& object, const uint32 ID) const
{
    auto i = getComponent<T>(ID);

    if (i)
    {
        auto ptr = std::make_unique<T>(i->clone(object));

        if (ptr)
        {
            object.m_components.emplace_back(std::move(ptr));
            return object.m_components.back().get();
        }
    }

    return nullptr;
}

//////////////////////////////////////////////

template<typename T>
Object& Object::removeComponent(const uint32 ID)
{
    for (auto itr = m_components.begin(); itr != m_components.end(); ++itr)
    {
        if (typeid(*(*itr)) == typeid(T) && itr->get()->getID() == ID)
        {
            m_components.erase(itr);
            break;
        }
    }

    return *this;
}