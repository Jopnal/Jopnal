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


namespace jop
{
    namespace detail
    {
        json::Value* getJsonValue(const std::string& path, json::Document& root, const bool create)
        {
            if (root.IsObject())
            {
                auto rootPos = path.find_first_of('@');

                auto pos = (rootPos == std::string::npos ? 0 : rootPos + 1);
                json::Value* currentVal = &root;

                do
                {
                    auto next = path.find_first_of('|', pos + 1);

                    if (next == std::string::npos)
                    {
                        const std::string obj(path.substr(pos));

                        if (currentVal->HasMember(obj.c_str()))
                            return &(*currentVal)[obj.c_str()];
                        else
                            return create ? &currentVal->AddMember(json::Value(obj.c_str(), root.GetAllocator()), json::Value(jop::json::kNullType), root.GetAllocator())[obj.c_str()] : nullptr;
                    }

                    const std::string obj(path.substr(pos, next - pos));

                    if (currentVal->HasMember(obj.c_str()))
                        currentVal = &(*currentVal)[obj.c_str()];
                    else
                    {
                        if (create)
                            currentVal = &currentVal->AddMember(json::Value(obj.c_str(), root.GetAllocator()), json::Value(jop::json::kObjectType), root.GetAllocator())[obj.c_str()];
                        else
                            return nullptr;
                    }

                    pos = next + 1;

                } while (pos != std::string::npos);
            }

            return nullptr;
        }

        //////////////////////////////////////////////

        void checkChanges(json::Value& oldVal, json::Value& newVal, const std::string& path, json::Value::AllocatorType& alloc, SettingManager::UpdaterMap& changers)
        {
            if (newVal.IsObject())
            {
                for (auto itr = newVal.MemberBegin(); itr != newVal.MemberEnd(); ++itr)
                {
                    if (oldVal.HasMember(itr->name))
                        checkChanges(oldVal[itr->name], itr->value, path + itr->name.GetString() + (itr->value.IsObject() ? "|" : ""), alloc, changers);
                }
            }
            else
            {
                if (oldVal != newVal)
                {
                    oldVal.CopyFrom(newVal, alloc);

                #if JOP_CONSOLE_VERBOSITY >= 2
                    const auto count = changers.count(path);
                #endif

                    JOP_DEBUG_INFO("Setting \"" << path << "\" changed, " << (count > 0 ? ("invoking " + std::to_string(count) + " callback" + (count > 1 ? "s" : "")) : ("no callbacks registered")));

                    auto range = changers.equal_range(path);

                    for (auto itr = range.first; itr != range.second; ++itr)
                        itr->second->valueChangedBase(oldVal);
                }
            }
        }

        //////////////////////////////////////////////

        json::Document& findRoot(const std::string& name, SettingManager::SettingMap& settings, const std::string& defRoot)
        {
            auto pos = name.find_first_of('@');

            json::Document& doc = std::get<0>(settings[pos == std::string::npos || pos == 0 ? defRoot : name.substr(0, pos)]);

            if (!doc.IsObject())
                doc.SetObject();

            return doc;
        }

        //////////////////////////////////////////////

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

    //////////////////////////////////////////////

    bool SettingManager::settingExists(const std::string& path)
    {
        return detail::getJsonValue(path, detail::findRoot(path, m_instance->m_settings, m_instance->m_defaultRoot), false) != nullptr;
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
        FileLoader::listFilesRecursive("Config", files);

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
                const auto cutPos = i.find_first_of('/') + 1;
                auto& doc = m_instance->m_settings[i.substr(cutPos, i.find_last_of('.') - cutPos)];
                doc.second = false;

                doc.first.Parse<0>(text.c_str());

                if (!json::checkParseError(doc.first))
                    JOP_DEBUG_ERROR("Setting file \"" << i << "\" has a parse error. Default values will be used")
                else
                    JOP_DEBUG_INFO("Setting file \"" << i << "\" found and loaded");
            }
        }

        m_instance->m_filesUpdated.store(false);
    }

    //////////////////////////////////////////////

    void SettingManager::save()
    {
        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        if (!m_instance)
        {
            JOP_DEBUG_ERROR("Couldn't save settings, no SettingManager instance");
            return;
        }

        for (auto& i : m_instance->m_settings)
        {
            json::StringBuffer buffer;
            json::PrettyWriter<json::StringBuffer> writer(buffer);
            i.second.first.Accept(writer);

            if (i.first.find_first_of('/') != std::string::npos)
            {
                if (!FileLoader::makeDirectory(FileLoader::Directory::User, "Config/" + i.first.substr(0, i.first.find_last_of('/'))))
                    JOP_DEBUG_ERROR("Failed to save setting file \"" << i.first << "\", couldn't create directory");
            }

            if (FileLoader::writeTextfile(FileLoader::Directory::User, "Config/" + i.first + ".json", buffer.GetString()))
                JOP_DEBUG_INFO("Saved setting file \"" << i.first << ".json\"")

            else
                JOP_DEBUG_ERROR("Failed to save setting file \"" << i.first << ".json\"");
        }

        m_instance->m_wasSaved.store(true);
    }

    //////////////////////////////////////////////

    std::vector<std::unique_ptr<SettingCallbackBase>> ns_callbacks;

    SettingManager::SettingManager()
        : Subsystem         (0),
          m_settings        (),
          m_mutex           (),
          m_watcher         (),
          m_filesUpdated    (),
          m_defaultRoot     ("root"),
          m_updaters        (),
          m_wasSaved        (false)
    {
        {
            std::lock_guard<std::recursive_mutex> lock(m_mutex);

            JOP_ASSERT(m_instance == nullptr, "There must not be more than one jop::SettingManager!");
            m_instance = this;
        }

        reload();

        m_defaultRoot = get<std::string>("sDefaultSettingRoot", "root");

        if (get<bool>("engine@Settings|bAutoUpdate", true))
        {
            if (!m_watcher.start(FileLoader::getDirectory(FileLoader::Directory::User) + FileLoader::getDirectorySeparator() + "Config", [this](DirectoryWatcher::Info info)
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
            const bool debugMode =
            #ifdef JOP_DEBUG_MODE
                true;
            #else
                false;
            #endif

            {
                const char* const str = "engine@Debug|Console|bEnabled";

                DebugHandler::getInstance().setEnabled(get<bool>(str, debugMode));

                struct Callback : SettingCallback<bool>
                {
                    void valueChanged(const bool& value) override
                    {DebugHandler::getInstance().setEnabled(value);}
                };

                ns_callbacks.emplace_back(std::make_unique<Callback>());
                registerCallback(str,  *ns_callbacks.back());
            }{
            #ifndef JOP_DISABLE_CONSOLE

                const char* const str = "engine@Debug|Console|uVerbosity";
                using S = DebugHandler::Severity;

                DebugHandler::getInstance().setVerbosity(static_cast<S>(std::min(static_cast<unsigned int>(S::Diagnostic), get<uint32>(str, JOP_CONSOLE_VERBOSITY))));

                struct Callback : SettingCallback<uint32>
                {
                    void valueChanged(const uint32& value) override
                    {
                        using S = DebugHandler::Severity;
                        DebugHandler::getInstance().setVerbosity(static_cast<S>(std::min(static_cast<unsigned int>(S::Diagnostic), value)));
                    }
                };

                ns_callbacks.emplace_back(std::make_unique<Callback>());
                registerCallback(str, *ns_callbacks.back());

            #endif
            }{
                const char* const str = "engine@Debug|Console|bReduceSpam";

                DebugHandler::getInstance().setReduceSpam(get<bool>(str, true));

                struct Callback : SettingCallback<bool>
                {
                    void valueChanged(const bool& value) override
                    {DebugHandler::getInstance().setReduceSpam(value);}
                };

                ns_callbacks.emplace_back(std::make_unique<Callback>());
                registerCallback(str, *ns_callbacks.back());
            }{
                const char* const str = "engine@Debug|Console|bLogToFile";

                DebugHandler::getInstance().setFileLogging(get<bool>(str, debugMode));

                struct Callback : SettingCallback<bool>
                {
                    void valueChanged(const bool& value) override
                    {DebugHandler::getInstance().setFileLogging(value);}
                };

                ns_callbacks.emplace_back(std::make_unique<Callback>());
                registerCallback(str, *ns_callbacks.back());
            }
        #ifdef JOP_OS_WINDOWS
            {
                const char* const str = "engine@Debug|Console|bDebuggerOutput";

                DebugHandler::getInstance().setDebuggerOutput(IsDebuggerPresent() && get<bool>(str, true));

                struct Callback : SettingCallback<bool>
                {
                    void valueChanged(const bool& value) override
                    {DebugHandler::getInstance().setDebuggerOutput(IsDebuggerPresent() && value);}
                };

                ns_callbacks.emplace_back(std::make_unique<Callback>());
                registerCallback(str, *ns_callbacks.back());
            }
        #endif

            {
                const char* const str = "engine@Filesystem|bErrorChecks";

                FileLoader::enableErrorChecks(get<bool>(str, true));

                struct Callback : SettingCallback<bool>
                {
                    void valueChanged(const bool& value) override
                    {FileLoader::enableErrorChecks(value);}
                };

                ns_callbacks.emplace_back(std::make_unique<Callback>());
                registerCallback(str, *ns_callbacks.back());
            }
        }
    }

    SettingManager::~SettingManager()
    {
        m_watcher.stop();
        ns_callbacks.clear();

        save();
        m_instance = nullptr;
    }

    //////////////////////////////////////////////

    void SettingManager::preUpdate(const float)
    {
        if (m_filesUpdated.load() && !m_wasSaved.load())
        {
            std::lock_guard<std::recursive_mutex> lock(m_mutex);

            for (auto& i : m_settings)
            {
                if (!i.second.second)
                    continue;

                JOP_DEBUG_INFO("Setting file modification detected (\"" << i.first << ".json\"), reloading...");

                auto& val = i.second.first;

                std::string newStr;
                if (!FileLoader::readTextfile("Config/" + i.first + ".json", newStr))
                {
                    JOP_DEBUG_ERROR("Couldn't read updated setting file \"" << i.first << ".json\"");
                    continue;
                }

                json::Document newVals;
                newVals.ParseInsitu<0>(&newStr[0]);

                if (!json::checkParseError(newVals))
                {
                    JOP_DEBUG_ERROR("Couldn't parse updated setting file \"" << i.first << ".json\"");
                    continue;
                }

                for (auto itr = newVals.MemberBegin(); itr != newVals.MemberEnd(); ++itr)
                {
                    if (val.HasMember(itr->name))
                        detail::checkChanges(val, newVals, i.first + "@", val.GetAllocator(), m_updaters);
                }
            }
        }

        m_wasSaved.store(false);
        m_filesUpdated.store(false);
    }

    //////////////////////////////////////////////

    unsigned int SettingManager::registerCallback(const std::string& path, SettingCallbackBase& callback)
    {
        if (!m_instance)
            return 0;

        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        auto& inst = *m_instance;
        auto& map = inst.m_updaters;

        callback.m_path = path;

        map.emplace(path, &callback);

        return map.count(path);
    }

    //////////////////////////////////////////////

    void SettingManager::unregisterCallback(SettingCallbackBase& callback)
    {
        if (!m_instance)
            return;

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