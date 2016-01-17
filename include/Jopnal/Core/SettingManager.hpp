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

#ifndef JOP_SETTINGMANAGER_HPP
#define JOP_SETTINGMANAGER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API SettingManager
    {
    private:

        SettingManager() = delete;

        friend class Engine;

    public:

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

    private:

        static void initialize();

    };
}

#endif

/// \class SettingManager
/// \brief SettingManager handles loading and saving the engine settings
/// \addtogroup Core
///
/// #TODO Detailed description