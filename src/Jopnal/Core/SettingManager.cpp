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
    JOP_DERIVED_COMMAND_HANDLER(Subsystem, SettingManager)

        JOP_BIND_COMMAND(&SettingManager::reload, "reloadSettings");
        JOP_BIND_COMMAND(&SettingManager::save, "saveSettings");

    JOP_END_COMMAND_HANDLER(SettingManager)
}

namespace
{
    namespace rj = rapidjson;

    rj::Document ns_document;
    bool ns_init = false;
    std::mutex ns_mutex;

    rj::Value& getRJValue(const std::string& name)
    {
        if (!ns_document.HasMember(name.c_str()))
            ns_document.AddMember(rj::Value(name.c_str(), ns_document.GetAllocator()), rj::Value(rj::kNullType), ns_document.GetAllocator());
        
        return ns_document[name.c_str()];
    }

    #pragma region ValueCasters

    template<typename T = void>
    T getCastValue(const rj::Value&, const std::string&)
    {
        static_assert(!std::is_void<T>::value, "Setting type not specialized");
    }

    template<>
    int getCastValue<int>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsNumber(), "Setting type not convertible! (asked for an int): " + name);

    #ifndef JOP_ENABLE_ASSERTS
        name;
    #endif

        return val.GetInt();
    }

    template<>
    unsigned int getCastValue<unsigned int>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsNumber(), "Setting type not convertible! (asked for an unsigned int): " + name);

    #ifndef JOP_ENABLE_ASSERTS
        name;
    #endif

        return val.GetUint();
    }

    template<>
    float getCastValue<float>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsNumber(), "Setting type not convertible! (asked for a float): " + name);

    #ifndef JOP_ENABLE_ASSERTS
        name;
    #endif

        return static_cast<float>(val.GetDouble());
    }

    template<>
    double getCastValue<double>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsNumber(), "Setting type not convertible! (asked for a double): " + name);

    #ifndef JOP_ENABLE_ASSERTS
        name;
    #endif

        return val.GetDouble();
    }

    template<>
    bool getCastValue<bool>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsBool(), "Setting type not convertible! (asked for a bool): " + name);

    #ifndef JOP_ENABLE_ASSERTS
        name;
    #endif

        return val.GetBool();
    }

    template<>
    std::string getCastValue<std::string>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsString(), "Setting type not convertible! (asked for an string): " + name);

    #ifndef JOP_ENABLE_ASSERTS
        name;
    #endif

        return std::string(val.GetString());
    }

    #pragma endregion ValueCasters

    #pragma region ValueSetters

    template<typename T>
    void setRJValue(rj::Value& val, const T& newVal)
    {
        val = rj::Value(newVal);
    }

    template<>
    void setRJValue<std::string>(rj::Value& val, const std::string& newVal)
    {
        val = rj::Value(newVal.c_str(), ns_document.GetAllocator());
    }

    #pragma endregion ValueSetters

    template<typename T>
    T getSetting(const std::string& name, const T& defaultValue)
    {
        if (name.empty())
            return defaultValue;

        std::lock_guard<std::mutex> lock(ns_mutex);

        rj::Value& val = getRJValue(name);

        if (val.IsNull())
        {
            setRJValue(val, defaultValue);
            return defaultValue;
        }

        return getCastValue<T>(val, name);
    }

    template<typename T>
    void setSetting(const std::string& name, const T& newValue)
    {
        if (name.empty())
            return;

        std::lock_guard<std::mutex> lock(ns_mutex);

        rj::Value& val = getRJValue(name);
        setRJValue(val, newValue);
    }
}

namespace jop
{
    int SettingManager::getInt(const std::string& name, const int defaultValue)
    {
        return getSetting<int>(name, defaultValue);
    }

    //////////////////////////////////////////////

    unsigned int SettingManager::getUint(const std::string& name, const unsigned int defaultValue)
    {
        return getSetting<unsigned int>(name, defaultValue);
    }

    //////////////////////////////////////////////

    float SettingManager::getFloat(const std::string& name, const float defaultValue)
    {
        return getSetting<float>(name, defaultValue);
    }

    //////////////////////////////////////////////

    double SettingManager::getDouble(const std::string& name, const double defaultValue)
    {
        return getSetting<double>(name, defaultValue);
    }

    //////////////////////////////////////////////

    bool SettingManager::getBool(const std::string& name, const bool defaultValue)
    {
        return getSetting<bool>(name, defaultValue);
    }

    //////////////////////////////////////////////

    std::string SettingManager::getString(const std::string& name, const std::string& defaultValue)
    {
        return getSetting<std::string>(name, defaultValue);
    }

    //////////////////////////////////////////////

    void SettingManager::setInt(const std::string& name, const int value)
    {
        setSetting<int>(name, value);
    }

    //////////////////////////////////////////////

    void SettingManager::setUint(const std::string& name, const unsigned int value)
    {
        setSetting<unsigned int>(name, value);
    }

    //////////////////////////////////////////////

    void SettingManager::setFloat(const std::string& name, const float value)
    {
        setSetting<float>(name, value);
    }

    //////////////////////////////////////////////

    void SettingManager::setDouble(const std::string& name, const double value)
    {
        setSetting<double>(name, value);
    }

    //////////////////////////////////////////////

    void SettingManager::setBool(const std::string& name, const bool value)
    {
        setSetting<bool>(name, value);
    }

    //////////////////////////////////////////////

    void SettingManager::setString(const std::string& name, const std::string& value)
    {
        setSetting<std::string>(name, value);
    }

    //////////////////////////////////////////////

    void SettingManager::reload()
    {
        std::lock_guard<std::mutex> lock(ns_mutex);

        std::string buf;
        if (!FileLoader::readTextfile("config.json", buf))
        {
            // Not using DebugHandler here since it relies on the initialization of this class
            std::cout << "Couldn't read the config file. Default settings will be used\n" << std::endl;
            return;
        }

        ns_document.Parse<0>(buf.c_str());

        if (!json::checkParseError(ns_document))
            std::cout << "Couldn't parse the config file. Default settings will be used\n" << std::endl;
    }

    //////////////////////////////////////////////

    void SettingManager::save()
    {
        std::lock_guard<std::mutex> lock(ns_mutex);

        rj::StringBuffer buffer;
        rj::PrettyWriter<rj::StringBuffer> writer(buffer);
        ns_document.Accept(writer);

        if (FileLoader::writeTextfile(FileLoader::Directory::User, "config.json", buffer.GetString()))
            JOP_DEBUG_INFO("Successfully saved settings")
        else
            JOP_DEBUG_ERROR("Error while saving settings");
    }

    //////////////////////////////////////////////

    SettingManager::SettingManager()
        : Subsystem("Setting Manager")
    {
        JOP_ASSERT(!ns_init, "There must not be more than one jop::SettingManager!");

        if (FileLoader::fileExists("config.json"))
        {
            reload();
            goto Mark;
        }

        ns_document.SetObject();
        save();

    Mark:

        FileLoader::enableErrorChecks(getBool("bFilesystemErrorChecks", true));

        ns_init = true;
    }

    //////////////////////////////////////////////

    SettingManager::~SettingManager()
    {
        save();
    }
    
    //////////////////////////////////////////////

    bool SettingManager::checkInit()
    {
        std::lock_guard<std::mutex> lock(ns_mutex);

        return ns_init;
    }
}