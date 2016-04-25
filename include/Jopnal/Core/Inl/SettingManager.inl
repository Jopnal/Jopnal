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
    template<typename T>
    bool queryVariable(const json::Value&)
    {
        static_assert(false, "Setting type not specialized");
        return false;
    }

    template<typename T>
    T fetchVariable(const json::Value&)
    {
        static_assert(false, "Setting type not specialized");
        return T();
    }
}

//////////////////////////////////////////////

template<typename T>
void SettingManager::ChangeCallback<T>::valueChangedBase(const json::Value& val)
{
    if (detail::queryVariable<T>(val))
        valueChanged(detail::fetchVariable<T>(val));
}

//////////////////////////////////////////////

template<typename T>
bool SettingManager::settingExists(const std::string& path)
{
    std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

    auto& map = m_instance->m_settings;
    auto itr = map.find(std::make_pair(path, std::type_index(typeid(T))));

    return itr != map.end();
}

//////////////////////////////////////////////

template<typename T>
T SettingManager::get(const std::string& path, const T& defaultValue)
{
    static_assert(false, "Setting type not registered");
    return T();
}

//////////////////////////////////////////////

template<typename T>
void SettingManager::set(const std::string& path, const T& value)
{
    static_assert(false, "Setting type not registered");
}