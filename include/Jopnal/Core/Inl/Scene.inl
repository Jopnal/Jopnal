// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<typename T>
std::weak_ptr<T> Scene::getLayer()
{
    static_assert(std::is_base_of<Layer, T>::value, "Scene::getLayer(): Attempted to get a layer which is not derived from jop::Layer");

    const std::type_info& ti = typeid(T);

    for (auto& i : m_layers)
    {
        if (typeid(*i) == ti)
            return std::weak_ptr<T>(std::static_pointer_cast<T>(i));
    }

    return std::weak_ptr<T>();
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Scene::createLayer(Args&... args)
{
    static_assert(std::is_base_of<Layer, T>::value, "Scene::createLayer(): Attempted to create a layer which is not derived from jop::Layer");

    m_layers.emplace_back(std::make_shared<T>(args...));
    return static_cast<T&>(*m_layers.back());
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Scene::setDefaultLayer(Args&... args)
{
    static_assert(std::is_base_of<Layer, T>::value, "Scene::createDefaultLayer(): Attempted to create a layer which is not derived from jop::Layer");

    m_defaultLayer = std::make_shared<T>(args...);
    return static_cast<T&>(*m_defaultLayer);
}

#endif