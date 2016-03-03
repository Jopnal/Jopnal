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
WeakReference<T> Scene::getLayer() const
{
    static_assert(std::is_base_of<Layer, T>::value, "Scene::getLayer(): Attempted to get a layer which is not derived from jop::Layer");

    const std::type_info& ti = typeid(T);

    for (auto& i : m_layers)
    {
        if (typeid(*i) == ti)
            return static_ref_cast<T>(i);
    }

    return WeakReference<T>();
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
WeakReference<T> Scene::createLayer(Args&&... args)
{
    static_assert(std::is_base_of<Layer, T>::value, "Scene::createLayer(): Attempted to create a layer which is not derived from jop::Layer");

    // Make sure the default layer is created
    getDefaultLayer();

    m_layers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    return static_ref_cast<T>(m_layers.back()->getReference());
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
WeakReference<T> Scene::setDefaultLayer(Args&&... args)
{
    static_assert(std::is_base_of<Layer, T>::value, "Scene::createDefaultLayer(): Attempted to create a layer which is not derived from jop::Layer");

    // Make sure the default layer is created
    getDefaultLayer();

    m_layers.front() = std::make_unique<T>(std::forward<Args>(args)...);
    return static_ref_cast<T>(m_layers.front()->getReference());
}