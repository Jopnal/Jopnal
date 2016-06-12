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
    #pragma region VariableSetters
    template<typename T>
    void setVariable(json::Value& val, const T& value, json::Value::AllocatorType&);

    template<>
    void setVariable<bool>(json::Value& val, const bool& value, json::Value::AllocatorType&)
    {
        val.SetBool(value);
    }

    template<>
    void setVariable<double>(json::Value& val, const double& value, json::Value::AllocatorType&)
    {
        val.SetDouble(value);
    }

    template<>
    void setVariable<float>(json::Value& val, const float& value, json::Value::AllocatorType&)
    {
        val.SetDouble(static_cast<float>(value));
    }

    template<>
    void setVariable<int>(json::Value& val, const int& value, json::Value::AllocatorType&)
    {
        val.SetInt(value);
    }

    template<>
    void setVariable<unsigned int>(json::Value& val, const unsigned int& value, json::Value::AllocatorType&)
    {
        val.SetUint(value);
    }

    template<>
    void setVariable<std::string>(json::Value& val, const std::string& value, json::Value::AllocatorType& alloc)
    {
        // We can assume the map has at least one setting structure
        val.SetString(value.c_str(), alloc);
    }
    #pragma endregion VariableSetters

    #pragma region VariableCompare
    template<typename T>
    bool compareVariable(const jop::json::Value& left, const T& right)
    {
        return left == right;
    }
    template<>
    bool compareVariable<std::string>(const jop::json::Value& left, const std::string& right)
    {
        return left == right.c_str();
    }
    #pragma endregion VariableCompare

    //////////////////////////////////////////////

    template<typename T>
    T getSetting(const std::string& path, const T& defaultValue, std::recursive_mutex& mutex, json::Document& root)
    {
        if (path.empty())
            return defaultValue;

        std::lock_guard<std::recursive_mutex> lock(mutex);

        auto val = detail::getJsonValue(path, root, false);

        if (!val)
        {
            auto newVal = detail::getJsonValue(path, root, true);

            if (newVal)
            {
                JOP_DEBUG_DIAG("Setting \"" << path << "\" doesn't exist. Creating an entry using value \"" << defaultValue << "\"");
                detail::setVariable<T>(*newVal, defaultValue, root.GetAllocator());
                return detail::fetchVariable<T>(*newVal);
            }
            else
            {
                JOP_DEBUG_ERROR("Setting \"" << path << "\" couldn't be written. Using default value \"" << defaultValue << "\"");
                return defaultValue;
            }
        }

        if (!detail::queryVariable<T>(*val))
        {
            JOP_DEBUG_ERROR("Setting \"" << path << "\" is not convertible into \"" << typeid(T).name() << "\". Using default value \"" << defaultValue << "\"");
            return defaultValue;
        }

        return detail::fetchVariable<T>(*val);
    }

    //////////////////////////////////////////////

    template<typename T>
    void setSetting(const std::string& path, const T& value, std::recursive_mutex& mutex, SettingManager::UpdaterMap& updaters, json::Document& root)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

        json::Value* val = detail::getJsonValue(path, root, true);

        if (!val)
        {
            JOP_DEBUG_ERROR("Setting \"" << path << "\" couldn't be written");
            return;
        }

        if (detail::queryVariable<T>(*val) || val->IsNull())
        {
            if (val->IsNull() || !detail::compareVariable(*val, value))
                detail::setVariable<T>(*val, value, root.GetAllocator());
            else
                return;
        }
        else
        {
            JOP_DEBUG_ERROR("Setting \"" << path << "\" was not set, unmatched type");
            return;
        }

        auto range = updaters.equal_range(path);
        for (auto itr = range.first; itr != range.second; ++itr)
            itr->second->valueChangedBase(*val);
    }
}

//////////////////////////////////////////////

#define GET_SETTING detail::getSetting(path, defaultValue, m_instance->m_mutex, detail::findRoot(path, m_instance->m_settings, m_instance->m_defaultRoot))

template<>
bool SettingManager::get<bool>(const std::string& path, const bool& defaultValue)
{
    return GET_SETTING;
}

template<>
int SettingManager::get<int>(const std::string& path, const int& defaultValue)
{
    return GET_SETTING;
}

template<>
unsigned int SettingManager::get<unsigned int>(const std::string& path, const unsigned int& defaultValue)
{
    return GET_SETTING;
}

template<>
float SettingManager::get<float>(const std::string& path, const float& defaultValue)
{
    return GET_SETTING;
}

template<>
double SettingManager::get<double>(const std::string& path, const double& defaultValue)
{
    return GET_SETTING;
}

template<>
std::string SettingManager::get<std::string>(const std::string& path, const std::string& defaultValue)
{
    return GET_SETTING;
}

//////////////////////////////////////////////

#define SET_SETTING detail::setSetting(path, value, m_instance->m_mutex, m_instance->m_updaters, detail::findRoot(path, m_instance->m_settings, m_instance->m_defaultRoot))

template<>
void SettingManager::set<bool>(const std::string& path, const bool& value)
{
    SET_SETTING;
}

template<>
void SettingManager::set<int>(const std::string& path, const int& value)
{
    SET_SETTING;
}

template<>
void SettingManager::set<unsigned int>(const std::string& path, const unsigned int& value)
{
    SET_SETTING;
}

template<>
void SettingManager::set<float>(const std::string& path, const float& value)
{
    SET_SETTING;
}

template<>
void SettingManager::set<double>(const std::string& path, const double& value)
{
    SET_SETTING;
}

template<>
void SettingManager::set<std::string>(const std::string& path, const std::string& value)
{
    SET_SETTING;
}