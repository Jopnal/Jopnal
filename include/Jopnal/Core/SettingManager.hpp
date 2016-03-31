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
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API SettingManager : public Subsystem
    {
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


        /// \brief Get an integer setting
        ///
        /// \param name Name of the setting
        /// \param defaultValue The default value
        ///
        /// \return The integer setting
        ///
        static int getInt(const std::string& name, const int defaultValue);

        /// \brief Get an unsigned integer setting
        ///
        /// \param name Name of the setting
        /// \param defaultValue The default value
        ///
        /// \return The unsigned integer setting
        ///
        static unsigned int getUint(const std::string& name, const unsigned int defaultValue);

        /// \brief Get a float setting
        ///
        /// \param name Name of the setting
        /// \param defaultValue The default value
        ///
        /// \return The float setting
        ///
        static float getFloat(const std::string& name, const float defaultValue);

        /// \brief Get a double setting
        ///
        /// \param name Name of the setting
        /// \param defaultValue The default value
        ///
        /// \return The double setting
        ///
        static double getDouble(const std::string& name, const double defaultValue);

        /// \brief Get a boolean setting
        ///
        /// \param name Name of the setting
        /// \param defaultValue The default value
        ///
        /// \return The boolean setting
        ///
        static bool getBool(const std::string& name, const bool defaultValue);

        /// \brief Get a string setting
        ///
        /// \param name Name of the setting
        /// \param defaultValue The default value
        ///
        /// \return The string setting
        ///
        static std::string getString(const std::string& name, const std::string& defaultValue);


        /// \brief Set an integer setting
        ///
        /// \param name Name of the setting
        /// \param value The new value
        ///
        static void setInt(const std::string& name, const int value);

        /// \brief Set an unsigned integer setting
        ///
        /// \param name Name of the setting
        /// \param value The new value
        ///
        static void setUint(const std::string& name, const unsigned int value);

        /// \brief Set a float setting
        ///
        /// \param name Name of the setting
        /// \param value The new value
        ///
        static void setFloat(const std::string& name, const float value);

        /// \brief Set a double setting
        ///
        /// \param name Name of the setting
        /// \param value The new value
        ///
        static void setDouble(const std::string& name, const double value);

        /// \brief Set a bool setting
        ///
        /// \param name Name of the setting
        /// \param value The new value
        ///
        static void setBool(const std::string& name, const bool value);

        /// \brief Set a string setting
        ///
        /// \param name Name of the setting
        /// \param value The new value
        ///
        static void setString(const std::string& name, const std::string& value);


        /// \brief Reload the settings from file
        ///
        /// All individual settings need to be refreshed manually for changes to
        /// take effect.
        ///
        static void reload();

        /// \brief Save the settings to file
        ///
        static void save();
    };
}

#endif

/// \class SettingManager
/// \ingroup core
///
/// #TODO Detailed description