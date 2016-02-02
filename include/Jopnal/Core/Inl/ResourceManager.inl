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
std::weak_ptr<T> ResourceManager::getResource(const std::string& path)
{
    static_assert(std::is_base_of<Resource, T>::value, "Tried to load a resource that doesn't inherit from jop::Resource");

    auto it = m_resources.find(path);

    if (it == m_resources.end())
    {
        auto res = std::make_shared<T>();

        if (res->load(path))
        {
            m_resources[path] = std::move(res);
            return std::weak_ptr<T>(std::static_pointer_cast<T>(m_resources[path]));
        }

        JOP_DEBUG_ERROR("Couldn't load resource: " << path);
    }
    else
    {
        if (typeid(T) == typeid(*it->second.get()))
            return std::weak_ptr<T>(std::static_pointer_cast<T>(it->second));
        else
            JOP_DEBUG_ERROR("Resource is not of type " << typeid(T).name() << ": " << path);
    }

    return std::weak_ptr<T>();
}

#endif