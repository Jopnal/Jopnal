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
        return val.GetInt();
    }

    template<>
    unsigned int getCastValue<unsigned int>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsNumber(), "Setting type not convertible! (asked for an unsigned int): " + name);
        return val.GetUint();
    }

    template<>
    float getCastValue<float>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsNumber(), "Setting type not convertible! (asked for a float): " + name);
        return static_cast<float>(val.GetDouble());
    }

    template<>
    double getCastValue<double>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsNumber(), "Setting type not convertible! (asked for a double): " + name);
        return val.GetDouble();
    }

    template<>
    bool getCastValue<bool>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsBool(), "Setting type not convertible! (asked for a bool): " + name);
        return val.GetBool();
    }

    template<>
    std::string getCastValue<std::string>(const rj::Value& val, const std::string& name)
    {
        JOP_ASSERT(val.IsString(), "Setting type not convertible! (asked for an string): " + name);
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
        // #TODO Move the file reading/writing to the dedicated file class

        PHYSFS_File* file = PHYSFS_openRead("config.json");

        if (!file)
        {
            // Not using DebugHandler here since it relies on the initialization of this class
            std::cout << "Couldn't read the config file. Default settings will be used\n" << std::endl;
            return;
        }

        int size = static_cast<int>(PHYSFS_fileLength(file));

        if (size != -1)
        {
            std::string buf(size, 0);
            PHYSFS_read(file, &buf[0], 1, size);
            ns_document.Parse<0>(buf.c_str());

            if (ns_document.HasParseError())
                std::cout << "Config file has a parse error at " << ns_document.GetErrorOffset() << std::endl;
        }

        PHYSFS_close(file);
    }

    //////////////////////////////////////////////

    std::string fullPath;

    void SettingManager::save()
    {
        rj::StringBuffer buffer;
        rj::PrettyWriter<rj::StringBuffer> writer(buffer);
        ns_document.Accept(writer);

        PHYSFS_setWriteDir(fullPath.c_str());
        PHYSFS_File* file = PHYSFS_openWrite("config.json");

        if (file)
            PHYSFS_write(file, buffer.GetString(), 1, buffer.GetSize());

        PHYSFS_close(file);
    }

    //////////////////////////////////////////////

    SettingManager::SettingManager()
        : Subsystem("Setting Manager")
    {
        JOP_ASSERT(!ns_init, "There must not be more than one jop::SettingManager!");

        if (!PHYSFS_isInit())
            PHYSFS_init(0);

        static const std::string docDir("Documents");

        fullPath = PHYSFS_getUserDir();
        fullPath += docDir + PHYSFS_getDirSeparator() + getProjectName();

        if (!PHYSFS_mount(fullPath.c_str(), NULL, 0))
        {
            // Create the directory
            JOP_ASSERT_EVAL(PHYSFS_setWriteDir((PHYSFS_getUserDir() + docDir).c_str()) != 0, "Failed to set write directory!");
            JOP_ASSERT_EVAL(PHYSFS_mkdir(getProjectName().c_str()) != 0, "Failed to create config directory!");

            // Try to mount again
            PHYSFS_mount(fullPath.c_str(), NULL, 0);
        }
        else if (PHYSFS_exists("config.json"))
        {
            reload();
            goto Mark;
        }

        ns_document.SetObject();
        save();

    Mark:

        ns_init = true;
    }

    //////////////////////////////////////////////

    SettingManager::~SettingManager()
    {
        save();

        if (PHYSFS_isInit())
        {
            if (!PHYSFS_deinit())
                JOP_DEBUG_ERROR("Could not deinitialize PhysicsFS. Some file handles might be left open.");
        }

        ns_init = false;
    }
}