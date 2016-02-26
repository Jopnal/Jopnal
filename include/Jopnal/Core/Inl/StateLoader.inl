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
void StateLoader::registerLoadable(const char* id, const typename detail::FuncChooser<T>::LoadFunc& func)
{
    std::get<0>(std::get<detail::FuncChooser<T>::ContainerID>(m_loaderSavers)[std::string(id)]) = func;
}

//////////////////////////////////////////////

template<typename T>
void StateLoader::registerSaveable(const char* id, const typename detail::FuncChooser<T>::SaveFunc& func)
{
    std::get<1>(std::get<detail::FuncChooser<T>::ContainerID>(m_loaderSavers)[std::string(id)]) = func;
    std::get<std::tuple_size<decltype(m_loaderSavers)>::value - 1>(m_loaderSavers)[std::type_index(typeid(T))] = id;
}

//////////////////////////////////////////////

template<typename T>
const typename detail::FuncChooser<T>::FuncContainer& StateLoader::getFunctionContainer()
{
    return std::get<detail::FuncChooser<T>::ContainerID>(getInstance().m_loaderSavers);
}