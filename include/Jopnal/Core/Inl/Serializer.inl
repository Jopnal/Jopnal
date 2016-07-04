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
void Serializer::registerSerializeable(const std::string& id,
                                       const typename detail::FuncChooser<T>::FactoryFunc& factFunc,
                                       const typename detail::FuncChooser<T>::LoadFunc& loadFunc,
                                       const typename detail::FuncChooser<T>::SaveFunc& saveFunc)
{
    std::get<detail::FuncChooser<T>::ContainerID>(m_loaderSavers)[id] = std::make_tuple(factFunc, loadFunc, saveFunc);
    std::get<std::tuple_size<decltype(m_loaderSavers)>::value - 1>(m_loaderSavers)[std::type_index(typeid(T))] = id;
}

//////////////////////////////////////////////

template<typename T, uint32 FuncID, typename ... Args>
bool Serializer::callSingleFunc(const std::string& id, Args&&... args)
{
    auto& cont = std::get<detail::FuncChooser<T>::ContainerID>(getInstance().m_loaderSavers);
    auto tupleItr = cont.find(id);

    if (tupleItr != cont.end())
        return std::get<FuncID>(tupleItr->second)(std::forward<Args>(args)...);

    return false;
}

//////////////////////////////////////////////

template<typename T>
const std::string& Serializer::getSerializeID()
{
    auto& cont = std::get<std::tuple_size<decltype(getInstance().m_loaderSavers)>::value - 1>(getInstance().m_loaderSavers);
    auto itr = cont.find(std::type_index(typeid(T)));

    if (itr != cont.end())
        return itr->second;

    static const std::string dummy;
    return dummy;
}