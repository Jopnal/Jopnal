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


template<typename T, typename ... Args>
T& Scene::setRenderer(Args&&... args)
{
    m_renderer = std::make_unique<T>(std::forward<Args>(args)...);
    return static_cast<T&>(*m_renderer);
}

//////////////////////////////////////////////

template<int D>
typename detail::WorldType<D>::type& Scene::getWorld()
{
    if (!worldEnabled<D>())
        std::get<D - 2>(m_worlds) = &createComponent<typename detail::WorldType<D>::type>(getRenderer());
    
    return *std::get<D - 2>(m_worlds);
}

//////////////////////////////////////////////

template<int D>
bool Scene::worldEnabled() const
{
    return std::get<D - 2>(m_worlds) != nullptr;
}

//////////////////////////////////////////////

template<int D>
void Scene::disableWorld()
{
    removeComponent<typename detail::WorldType<D>::type>();
}