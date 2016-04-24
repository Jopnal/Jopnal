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

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace detail
{
    jop::json::Value* getJsonValue(const std::string& path, jop::json::Document& root, const bool create)
    {
        if (root.IsObject())
        {
            auto rootPos = path.find_first_of("/\\");

            auto pos = (rootPos == std::string::npos ? 0 : rootPos + 1);
            jop::json::Value* currentVal = &root;

            do
            {
                auto next = path.find_first_of('|', pos + 1);

                if (next == std::string::npos)
                {
                    const std::string obj(path.substr(pos));

                    if (currentVal->HasMember(obj.c_str()))
                        return &(*currentVal)[obj.c_str()];
                    else
                        return create ? &currentVal->AddMember(jop::json::Value(obj.c_str(), root.GetAllocator()), jop::json::Value(jop::json::kNullType), root.GetAllocator())[obj.c_str()] : nullptr;
                }

                const std::string obj(path.substr(pos, next - pos));

                if (currentVal->HasMember(obj.c_str()))
                    currentVal = &(*currentVal)[obj.c_str()];
                else
                {
                    if (create)
                        currentVal = &currentVal->AddMember(jop::json::Value(obj.c_str(), root.GetAllocator()), jop::json::Value(jop::json::kObjectType), root.GetAllocator())[obj.c_str()];
                    else
                        return nullptr;
                }

                pos = next + 1;

            } while (pos != std::string::npos);
        }

        return nullptr;
    }

    #pragma region VariableQueries
    template<typename T>
    bool queryVariable(const jop::json::Value&)
    {
        static_assert(false, "Setting type not specialized");
        return false;
    }

    template<>
    bool queryVariable<bool>(const jop::json::Value& val)
    {
        return val.IsBool();
    }

    template<>
    bool queryVariable<double>(const jop::json::Value& val)
    {
        return val.IsDouble();
    }

    template<>
    bool queryVariable<float>(const jop::json::Value& val)
    {
        return val.IsDouble();
    }

    template<>
    bool queryVariable<int>(const jop::json::Value& val)
    {
        return val.IsInt();
    }

    template<>
    bool queryVariable<unsigned int>(const jop::json::Value& val)
    {
        return val.IsUint();
    }

    template<>
    bool queryVariable<std::string>(const jop::json::Value& val)
    {
        return val.IsString();
    }
#pragma endregion VariableQueries

    #pragma region VariableFetchers
    template<typename T>
    T fetchVariable(const jop::json::Value&)
    {
        static_assert(false, "Setting type not specialized");
    }

    template<>
    bool fetchVariable<bool>(const jop::json::Value& val)
    {
        return val.GetBool();
    }

    template<>
    double fetchVariable<double>(const jop::json::Value& val)
    {
        return val.GetDouble();
    }

    template<>
    float fetchVariable<float>(const jop::json::Value& val)
    {
        return static_cast<float>(val.GetDouble());
    }

    template<>
    int fetchVariable<int>(const jop::json::Value& val)
    {
        return val.GetInt();
    }

    template<>
    unsigned int fetchVariable<unsigned int>(const jop::json::Value& val)
    {
        return val.GetUint();
    }

    template<>
    std::string fetchVariable<std::string>(const jop::json::Value& val)
    {
        return std::string(val.GetString());
    }
#pragma endregion VariableFetchers

    #pragma region VariableSetters
    template<typename T>
    void setVariable(jop::json::Value&, const T&, jop::json::Value::AllocatorType&)
    {
        static_assert(false, "Setting type not specialized");
    }

    template<>
    void setVariable<bool>(jop::json::Value& val, const bool& value, jop::json::Value::AllocatorType&)
    {
        val.SetBool(value);
    }

    template<>
    void setVariable<double>(jop::json::Value& val, const double& value, jop::json::Value::AllocatorType&)
    {
        val.SetDouble(value);
    }

    template<>
    void setVariable<float>(jop::json::Value& val, const float& value, jop::json::Value::AllocatorType&)
    {
        val.SetDouble(static_cast<float>(value));
    }

    template<>
    void setVariable<int>(jop::json::Value& val, const int& value, jop::json::Value::AllocatorType&)
    {
        val.SetInt(value);
    }

    template<>
    void setVariable<unsigned int>(jop::json::Value& val, const unsigned int& value, jop::json::Value::AllocatorType&)
    {
        val.SetUint(value);
    }

    template<>
    void setVariable<std::string>(jop::json::Value& val, const std::string& value, jop::json::Value::AllocatorType& alloc)
    {
        // We can assume the map has at least one setting structure
        val.SetString(value.c_str(), alloc);
    }
#pragma endregion VariableSetters
}

namespace jop
{
    SettingManager::ChangeCallbackBase::~ChangeCallbackBase()
    {
        SettingManager::unregisterCallback(*this);
    }

    //////////////////////////////////////////////

    namespace detail
    {
        json::Document& findRoot(const std::string& name)
        {
            auto pos = name.find_first_of("/\\");

            json::Document& doc = std::get<0>(SettingManager::m_instance->m_settings[pos == std::string::npos || pos == 0 ? SettingManager::m_instance->m_defaultRoot : name.substr(0, pos)]);

            if (!doc.IsObject())
                doc.SetObject();

            return doc;
        }

        //////////////////////////////////////////////

        template<typename T>
        T getSetting(const std::string& path, const T& defaultValue)
        {
            if (path.empty())
                return defaultValue;

            auto& inst = *SettingManager::m_instance;

            std::lock_guard<std::recursive_mutex> lock(inst.m_mutex);

            jop::json::Document& root = findRoot(path);
            auto val = ::detail::getJsonValue(path, root, false);

            if (!val)
            {
                auto newVal = ::detail::getJsonValue(path, root, true);

                if (newVal)
                {
                    JOP_DEBUG_DIAG("Setting \"" << path << "\" doesn't exist. Creating an entry using value " << defaultValue);
                    ::detail::setVariable<T>(*newVal, defaultValue, root.GetAllocator());
                    return ::detail::fetchVariable<T>(*newVal);
                }
                else
                {
                    JOP_DEBUG_ERROR("Setting \"" << path << "\" couldn't be written. Using default value " << defaultValue);
                    return defaultValue;
                }
            }

            if (!::detail::queryVariable<T>(*val))
            {
                JOP_DEBUG_ERROR("Setting \"" << path << "\" is not convertible into \"" << typeid(T).name() << "\". Using default value " << defaultValue);
                return defaultValue;
            }

            return ::detail::fetchVariable<T>(*val);
        }

        //////////////////////////////////////////////

        template<typename T>
        void setSetting(const std::string& path, const T& value)
        {
            auto& inst = *SettingManager::m_instance;

            std::lock_guard<std::recursive_mutex> lock(inst.m_mutex);

            json::Document& root = findRoot(path);
            json::Value* val = ::detail::getJsonValue(path, root, true);

            if (!val)
            {
                JOP_DEBUG_ERROR("Setting \"" << path << "\" couldn't be written");
                return;
            }

            if (::detail::queryVariable<T>(*val) || val->IsNull())
                ::detail::setVariable<T>(*val, value, root.GetAllocator());

            else
                JOP_DEBUG_ERROR("Setting \"" << path << "\" was not set, unmatched type");
        }
    }

    template<>
    bool SettingManager::get<bool>(const std::string& path, const bool& defaultValue)
    {
        return detail::getSetting(path, defaultValue);
    }

    template<>
    int SettingManager::get<int>(const std::string& path, const int& defaultValue)
    {
        return detail::getSetting(path, defaultValue);
    }

    template<>
    unsigned int SettingManager::get<unsigned int>(const std::string& path, const unsigned int& defaultValue)
    {
        return detail::getSetting(path, defaultValue);
    }

    template<>
    float SettingManager::get<float>(const std::string& path, const float& defaultValue)
    {
        return detail::getSetting(path, defaultValue);
    }

    template<>
    double SettingManager::get<double>(const std::string& path, const double& defaultValue)
    {
        return detail::getSetting(path, defaultValue);
    }

    template<>
    std::string SettingManager::get<std::string>(const std::string& path, const std::string& defaultValue)
    {
        return detail::getSetting(path, defaultValue);
    }

    //////////////////////////////////////////////

    template<>
    void SettingManager::set<bool>(const std::string& path, const bool& value)
    {
        detail::setSetting(path, value);
    }

    template<>
    void SettingManager::set<int>(const std::string& path, const int& value)
    {
        detail::setSetting(path, value);
    }

    template<>
    void SettingManager::set<unsigned int>(const std::string& path, const unsigned int& value)
    {
        detail::setSetting(path, value);
    }

    template<>
    void SettingManager::set<float>(const std::string& path, const float& value)
    {
        detail::setSetting(path, value);
    }

    template<>
    void SettingManager::set<double>(const std::string& path, const double& value)
    {
        detail::setSetting(path, value);
    }

    template<>
    void SettingManager::set<std::string>(const std::string& path, const std::string& value)
    {
        detail::setSetting(path, value);
    }

    //////////////////////////////////////////////

    void SettingManager::reload()
    {
        {
            std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

            if (!m_instance)
            {
                JOP_DEBUG_ERROR("Couldn't load settings, no SettingManager instance");
                return;
            }
        }

        std::vector<std::string> files;
        FileLoader::listFiles("", files);

        files.erase(std::remove_if(files.begin(), files.end(), [](const std::string& str)
        {
            return str.find(".json") == std::string::npos;

        }), files.end());

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        for (auto& i : files)
        {
            std::string text;
            if (!FileLoader::readTextfile(i, text))
                JOP_DEBUG_ERROR("Failed to read setting file \"" << i << "\". Default values will be used")

            else
            {
                auto& doc = m_instance->m_settings[i.substr(0, i.find_last_of('.'))];
                doc.second = false;
                doc.first.Parse<0>(text.c_str());

                if (!json::checkParseError(doc.first))
                    JOP_DEBUG_ERROR("Setting file \"" << i << "\" has a parse error. Default values will be used");
            }
        }

        m_instance->m_filesUpdated.store(false);
    }

    //////////////////////////////////////////////

    void SettingManager::save()
    {
        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        if (!m_instance)
            JOP_DEBUG_ERROR("Couldn't save settings, no SettingManager instance");

        for (auto& i : m_instance->m_settings)
        {
            json::StringBuffer buffer;
            json::PrettyWriter<json::StringBuffer> writer(buffer);
            i.second.first.Accept(writer);

            if (FileLoader::writeTextfile(FileLoader::Directory::User, i.first + ".json", buffer.GetString()))
                JOP_DEBUG_INFO("Saved setting file \"" << i.first << ".json\"")

            else
                JOP_DEBUG_ERROR("Failed to save setting file \"" << i.first << ".json\"");
        }
    }

    //////////////////////////////////////////////

    SettingManager::SettingManager()
        : Subsystem("Setting Manager"),
          m_defaultRoot("root")
    {
        {
            std::lock_guard<std::recursive_mutex> lock(m_mutex);

            JOP_ASSERT(m_instance == nullptr, "There must not be more than one jop::SettingManager!");
            m_instance = this;
        }

        reload();

        m_defaultRoot = get<std::string>("sDefaultSettingRoot", "root");

        if (get<bool>("engine/Settings|bAutoUpdate", true))
        {
            if (!m_watcher.start(FileLoader::getDirectory(FileLoader::Directory::User), [this](DirectoryWatcher::Info info)
            {
                if (info.filename.find(".json") != std::string::npos)
                {
                    m_settings[info.filename.substr(0, info.filename.find_last_of('.'))].second = true;
                    m_filesUpdated.store(true);
                }
            }))
                JOP_DEBUG_ERROR("Failed to start setting file watcher, settings won't be automatically refreshed");
        }

        // Load some settings for external systems. They cannot fetch these themselves as they might be
        // initialized before SettingManager.
        {
            {
                const char* const str = "engine/Debug|Console|bEnabled";

                const bool console =
                #ifdef JOP_DEBUG_MODE
                    true;
                #else
                    false;
                #endif

                DebugHandler::getInstance().setEnabled(get<bool>(str, console));
            }{
                const char* const str = "engine/Debug|Console|uVerbosity";

                DebugHandler::getInstance()
                .setVerbosity(static_cast<DebugHandler::Severity>(std::min(static_cast<unsigned int>(DebugHandler::Severity::Diagnostic), get<uint32>(str, JOP_CONSOLE_VERBOSITY))));
            }{
                const char* const str = "engine/Debug|Console|bReduceSpam";

                DebugHandler::getInstance().setReduceSpam(get<bool>(str, true));
            }
        #ifdef JOP_OS_WINDOWS
            {
                const char* const str = "engine/Debug|Console|bDebuggerOutput";

                DebugHandler::getInstance().setDebuggerOutput(IsDebuggerPresent() && get<bool>(str, true));
            }
        #endif

            {
                const char* const str = "engine/Filesystem|bErrorChecks";

                FileLoader::enableErrorChecks(get<bool>(str, true));
            }
        }
    }

    SettingManager::~SettingManager()
    {
        m_watcher.stop();

        save();
        m_instance = nullptr;
    }

    //////////////////////////////////////////////

    void SettingManager::preUpdate(const float)
    {
        if (m_filesUpdated.load())
        {


            m_filesUpdated = false;
        }
    }

    //////////////////////////////////////////////

    void SettingManager::unregisterCallback(ChangeCallbackBase& callback)
    {
        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        auto& inst = *m_instance;
        auto& map = inst.m_updaters;

        auto range = map.equal_range(callback.m_path);

        for (auto itr = range.first; itr != range.second; ++itr)
        {
            if (itr->second == &callback)
            {
                map.erase(itr);
                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    SettingManager* SettingManager::m_instance = nullptr;
}