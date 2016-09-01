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

#ifndef JOP_SETTINGCALLBACK_HPP
#define JOP_SETTINGCALLBACK_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Json.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class SettingManager;

    namespace detail
    {
        template<typename T>
        bool queryVariable(const json::Value&);
        template<>
        bool queryVariable<bool>(const json::Value& val);
        template<>
        bool queryVariable<double>(const json::Value& val);
        template<>
        bool queryVariable<float>(const json::Value& val);
        template<>
        bool queryVariable<int>(const json::Value& val);
        template<>
        bool queryVariable<unsigned int>(const json::Value& val);
        template<>
        bool queryVariable<std::string>(const json::Value& val);

        template<typename T>
        T fetchVariable(const json::Value& val);
        template<>
        bool fetchVariable<bool>(const json::Value& val);
        template<>
        double fetchVariable<double>(const json::Value& val);
        template<>
        float fetchVariable<float>(const json::Value& val);
        template<>
        int fetchVariable<int>(const json::Value& val);
        template<>
        unsigned int fetchVariable<unsigned int>(const json::Value& val);
        template<>
        std::string fetchVariable<std::string>(const json::Value& val);

        /// Base-base class for a setting change callback
        ///
        /// This is for internal use only.
        ///
        class JOP_API SettingCallbackBase
        {
            friend class ::jop::SettingManager;
            std::string m_path;

        public:

            virtual ~SettingCallbackBase() = 0;
            virtual void valueChangedBase(const json::Value& val) = 0;
        };
    }

    /// \brief Base class For a setting change callback
    ///
    /// Inherit from this class to create a setting change callback.
    /// 
    /// \see SettingManager::registerCallback()
    ///
    template<typename T>
    class SettingCallback : public detail::SettingCallbackBase
    {
    private:

        void valueChangedBase(const json::Value& val) final override;

    protected:

        /// \brief The setting change callback function
        ///
        /// This will be automatically called when the associated
        /// setting has been changed.
        ///
        /// \param value The new setting value
        /// 
        virtual void valueChanged(const T& value) = 0;
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/SettingCallback.inl>
}

/// \class jop::SettingCallback
/// \ingroup core

#endif