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

#ifndef JOP_SCENELOADER_HPP
#define JOP_SCENELOADER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Core/Scene.hpp>
#include <Jopnal/Graphics/Layer.hpp>
#include <unordered_map>
#include <typeindex>
#include <functional>

//////////////////////////////////////////////


namespace jop
{
    class Object;

    typedef std::function<bool(Object&, const char*)> ComponentLoadFunc;
    typedef std::function<bool(Scene&, const char*)> LayerLoadFunc;
    typedef std::function<bool(const char*)> SceneLoadFunc;

    namespace detail
    {
        template
        <
            typename T,
            bool IsComp = std::is_base_of<Component, T>::value,
            bool IsLayer = std::is_base_of<Layer, T>::value,
            bool IsScene = std::is_base_of<Component, T>::value

        > struct LoadFuncChooser{};

        template<typename T>
        struct LoadFuncChooser < T, true, false, false >
        {
            typedef ComponentLoadFunc func;
        };

        template<typename T>
        struct LoadFuncChooser < T, false, true, false >
        {
            typedef LayerLoadFunc func;
        };

        template<typename T>
        struct LoadFuncChooser < T, false, false, true>
        {
            typedef SceneLoadFunc func;
        };
    }

    class SceneLoader
    {
    public:

    public:
    
        template<typename T>
        void registerLoadable(const detail::LoadFuncChooser<T>::func& func);
        

    private:

        std::unordered_map<

    };
}

#endif

#define JOP_LOADABLE_REGISTRAR(className) 

/// \class SceneLoader
/// \ingroup core
///
/// #TODO Detailed description