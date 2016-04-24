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
#include <Jopnal/Utility/Json.hpp>
#include <Jopnal/Utility/DirectoryWatcher.hpp>
#include <unordered_map>
#include <string>
#include <mutex>
#include <atomic>
#include <tuple>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        template<typename T>
        T getSetting(const std::string&, const T&);

        template<typename T>
        void setSetting(const std::string&, const T&);

        json::Document& findRoot(const std::string&);
    }

    class JOP_API SettingManager final : public Subsystem
    {
    public:

        class ChangeCallbackBase
        {
            friend class SettingManager;

            std::string m_path;


            virtual ~ChangeCallbackBase() = 0;

            virtual void valueChangedBase(const json::Value& val) = 0;
        };

        template<typename T>
        class ChangeCallback : ChangeCallbackBase
        {
            void valueChangedBase(const json::Value& val) final override;

        protected:

            virtual void valueChanged(const T& value) = 0;
        };

    private:

        friend class Engine;
        friend class ChangeCallbackBase;

        template<typename T>
        friend T detail::getSetting(const std::string&, const T&);

        template<typename T>
        friend void detail::setSetting(const std::string&, const T&);

        friend json::Document& detail::findRoot(const std::string&);


        template<typename T = void>
        static T get(const std::string& path, const T& defaultValue);

        template<typename T = void>
        static void set(const std::string& path, const T& value);

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

        void preUpdate(const float deltaTime) override;


        template<typename T>
        static bool settingExists(const std::string& path);


        template<>
        static bool get<bool>(const std::string& path, const bool& defaultValue);

        template<>
        static int get<int>(const std::string& path, const int& defaultValue);

        template<>
        static unsigned int get<unsigned int>(const std::string& path, const unsigned int& defaultValue);

        template<>
        static float get<float>(const std::string& path, const float& defaultValue);

        template<>
        static double get<double>(const std::string& path, const double& defaultValue);

        template<>
        static std::string get<std::string>(const std::string& path, const std::string& defaultValue);


        template<>
        static void set<bool>(const std::string& path, const bool& value);

        template<>
        static void set<int>(const std::string& path, const int& value);

        template<>
        static void set<unsigned int>(const std::string& path, const unsigned int& value);

        template<>
        static void set<float>(const std::string& path, const float& value);

        template<>
        static void set<double>(const std::string& path, const double& value);

        template<>
        static void set<std::string>(const std::string& path, const std::string& value);


        template<typename T>
        static unsigned int registerCallback(const std::string& path, ChangeCallback<T>& callback);


        /// \brief Reload the settings from file
        ///
        /// Avoid calling this function. It will replace all current settings with those found
        /// in the files, discarding all unsaved changes. It will also invoke all registered change
        /// callbacks, even when the value hadn't changed.
        ///
        /// \see set()
        ///
        static void reload();

        /// \brief Save the settings to file
        ///
        static void save();

    private:

        static void unregisterCallback(ChangeCallbackBase& callback);

        static SettingManager* m_instance;

        std::unordered_map<std::string, std::pair<json::Document, bool>> m_settings;
        std::recursive_mutex m_mutex;
        DirectoryWatcher m_watcher;
        std::atomic<bool> m_filesUpdated;
        std::string m_defaultRoot;
        std::unordered_multimap<std::string, ChangeCallbackBase*> m_updaters;
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/SettingManager.inl>
}

#endif

/// \class SettingManager
/// \ingroup core
///
/// #TODO Detailed description