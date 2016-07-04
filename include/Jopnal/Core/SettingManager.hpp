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

#ifndef JOP_SETTINGMANAGER_HPP
#define JOP_SETTINGMANAGER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <Jopnal/Core/SettingCallback.hpp>
#include <Jopnal/Utility/DirectoryWatcher.hpp>
#include <Jopnal/Utility/Json.hpp>
#include <unordered_map>
#include <string>
#include <mutex>
#include <atomic>
#include <tuple>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API SettingManager final : public Subsystem
    {
    public:

        typedef std::unordered_multimap<std::string, SettingCallbackBase*> UpdaterMap;
        typedef std::unordered_map<std::string, std::pair<json::Document, bool>> SettingMap;

    private:

        friend class SettingCallbackBase;

    public:

        /// \brief Default constructor
        ///
        /// Loads all the existing settings from file.
        ///
        SettingManager();

        /// \brief Destructor
        ///
        /// Saves the settings to file.
        ///
        ~SettingManager() override;

        /// \brief Update function
        ///
        /// Whenever any setting changes occur via file modification, the change
        /// callbacks will be called by this function in a synchronized manner.
        ///
        /// This never does anything if automatic setting updates have been
        /// turned off.
        ///
        /// \param deltaTime The delta time, not used
        ///
        void preUpdate(const float deltaTime) override;


        /// \brief Check if a certain setting exists
        ///
        /// \param path The setting path
        ///
        /// \return True if the setting exists
        ///
        static bool settingExists(const std::string& path);


        /// \brief Get a setting value
        ///
        /// The entry will be created if it doesn't exist.
        ///
        /// Example of a correctly formatted setting path:
        /// "engine@Path|To|Setting|actualSetting"
        ///
        /// The first section "engine", specifies the setting file. The .json extension will be added
        /// automatically. The file may be in a nested folder, but it must be specified using a
        /// <b>forward</b> slash (/). You may also leave the file part
        /// out, in which case the default file will be used ("root.json" by default).
        ///
        /// After the file specification, comes the actual setting path. Nodes are separated with
        /// "|" after which comes the actual setting name. This example would produce the following
        /// setting file:
        ///
        /// \code
        /// {
        ///     "Path": {
        ///         "To": {
        ///             "Setting": {
        ///                 "actualSetting": value
        ///             }
        ///         }
        ///     }
        /// }
        /// \endcode
        ///
        /// \param path The setting path
        /// \param defaultValue The default value, this will be used if the entry doesn't exist
        ///
        /// \return The setting value
        ///
        template<typename T>
        static T get(const std::string& path, const T& defaultValue);

        ///// \brief Set a setting value
        /////
        ///// The entry will be created if it doesn't exist.
        ///// 
        ///// The value will be updated only if it differs from the existing one,
        ///// as is the requirement for invoking any corresponding callbacks.
        ///// 
        ///// \param path The setting path
        ///// \param value The value to set
        /////
        template<typename T>
        static void set(const std::string& path, const T& value);


        /// \brief Register a setting change callback
        ///
        /// The callback will be invoked when you change a corresponding setting using set()
        /// or modify the setting file. The new value has to differ from the old one, or the
        /// invocation won't happen.
        ///
        /// You are responsible for ensuring that the callback stays alive as long as it's
        /// meant to be used. You won't need to explicitly unbind it. The callback will be unbound
        /// during its destruction.
        ///
        /// \param path The setting path to associate the callback with
        /// \param callback Reference to a callback object
        ///
        /// \return The number of callbacks that exist for the same setting, includes the one being registered
        ///
        static unsigned int registerCallback(const std::string& path, SettingCallbackBase& callback);


        /// \brief Reload the settings from file
        ///
        /// Avoid calling this function. It will replace all current settings with those found
        /// in the files, discarding all unsaved changes. It will also invoke all registered change
        /// callbacks, even when the value hadn't changed.
        ///
        /// \see set()
        /// \see registerCallback()
        ///
        static void reload();

        /// \brief Save the settings to file
        ///
        static void save();

        static void setDefaultDirectory(const std::string& directory);

        static void setOverrideWithDefaults();

    private:

        static void unregisterCallback(SettingCallbackBase& callback);

        static SettingManager* m_instance;

        SettingMap m_settings;              ///< Setting map
        std::recursive_mutex m_mutex;       ///< Mutex
        DirectoryWatcher m_watcher;         ///< File change watcher
        std::atomic<bool> m_filesUpdated;   ///< Has any file been externally modified?
        std::string m_defaultRoot;          ///< Default setting file
        UpdaterMap m_updaters;              ///< Change callback map
        std::atomic<bool> m_wasSaved;
    };

    namespace detail
    {
        json::Value* getJsonValue(const std::string& path, json::Document& root, const bool create);

        json::Document& findRoot(const std::string& name, SettingManager::SettingMap& settings, const std::string& defRoot);
    }

    template<typename T>
    struct DynamicSetting : SettingCallback<T>
    {
        DynamicSetting(const std::string& path, const T& defaultValue);

    private:

        void valueChanged(const T& val) override;

    public:

        T value;
    };

    #include <Jopnal/Core/Inl/SettingManager.inl>
}

#endif

/// \class SettingManager
/// \ingroup core
///
/// #TODO Detailed description