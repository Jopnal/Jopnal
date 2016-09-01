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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/SettingCallback.hpp>

    #include <Jopnal/Core/SettingManager.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        #pragma region VariableQueries
        template<>
        bool queryVariable<bool>(const json::Value& val)
        {
            return val.IsBool();
        }

        template<>
        bool queryVariable<double>(const json::Value& val)
        {
            return val.IsDouble();
        }

        template<>
        bool queryVariable<float>(const json::Value& val)
        {
            return val.IsDouble();
        }

        template<>
        bool queryVariable<int>(const json::Value& val)
        {
            return val.IsInt();
        }

        template<>
        bool queryVariable<unsigned int>(const json::Value& val)
        {
            return val.IsUint();
        }

        template<>
        bool queryVariable<std::string>(const json::Value& val)
        {
            return val.IsString();
        }
        #pragma endregion VariableQueries

        #pragma region VariableFetchers
        template<>
        bool fetchVariable<bool>(const json::Value& val)
        {
            return val.GetBool();
        }

        template<>
        double fetchVariable<double>(const json::Value& val)
        {
            return val.GetDouble();
        }

        template<>
        float fetchVariable<float>(const json::Value& val)
        {
            return static_cast<float>(val.GetDouble());
        }

        template<>
        int fetchVariable<int>(const json::Value& val)
        {
            return val.GetInt();
        }

        template<>
        unsigned int fetchVariable<unsigned int>(const json::Value& val)
        {
            return val.GetUint();
        }

        template<>
        std::string fetchVariable<std::string>(const json::Value& val)
        {
            return std::string(val.GetString());
        }
        #pragma endregion VariableFetchers
    }

    //////////////////////////////////////////////

    namespace detail
    {
        SettingCallbackBase::~SettingCallbackBase()
        {
            SettingManager::unregisterCallback(*this);
        }
    }
}