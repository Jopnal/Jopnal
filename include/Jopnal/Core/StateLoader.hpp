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
#include <Jopnal/Core/Object.hpp>
#include <Jopnal/Graphics/Layer.hpp>
#include <unordered_map>
#include <functional>
#include <tuple>

//////////////////////////////////////////////


namespace jop
{
    class Object;

    typedef std::function<bool(Object&, const char*)> ComponentLoadFunc;
    typedef std::function<bool(const Component&, std::string& json)> ComponentSaveFunc;

    typedef std::function<bool(Scene&, const char*)> LayerLoadFunc;
    typedef std::function<bool(const Layer&, std::string& json)> LayerSaveFunc;

    typedef std::function<bool(const char*)> SceneLoadFunc;
    typedef std::function<bool(const Scene&, std::string& json)> SceneSaveFunc;

    namespace detail
    {
        template
        <
            typename T,
            bool IsComp = std::is_base_of<Component, T>::value,
            bool IsLayer = std::is_base_of<Layer, T>::value,
            bool IsScene = std::is_base_of<Scene, T>::value

        > struct FuncChooser{};

        template<typename T>
        struct FuncChooser<T, true, false, false>
        {
            typedef ComponentLoadFunc LoadFunc;
            typedef ComponentSaveFunc SaveFunc;
            enum{ ContainerID = 0 };
        };
        template<typename T>
        struct FuncChooser<T, false, true, false>
        {
            typedef LayerLoadFunc LoadFunc;
            typedef LayerSaveFunc SaveFunc;
            enum{ ContainerID = 1 };
        };
        template<typename T>
        struct FuncChooser<T, false, false, true>
        {
            typedef SceneLoadFunc LoadFunc;
            typedef SceneSaveFunc SaveFunc;
            enum{ ContainerID = 2 };
        };
    }

    class JOP_API StateLoader
    {
    private:

        typedef std::unordered_map<std::string, std::tuple<ComponentLoadFunc, ComponentSaveFunc>> CompFuncContainer;
        typedef std::unordered_map<std::string, std::tuple<LayerLoadFunc, LayerSaveFunc>> LayerFuncContainer;
        typedef std::unordered_map<std::string, std::tuple<SceneLoadFunc, SceneSaveFunc>> SceneFuncContainer;

        StateLoader() = default;

    public:

        static StateLoader& getInstance();
    
        template<typename T>
        void registerLoadable(const std::string& id, const typename detail::FuncChooser<T>::LoadFunc& func);

        template<typename T>
        void registerSaveable(const std::string& id, const typename detail::FuncChooser<T>::SaveFunc& func);
        

    private:

        std::tuple
        <
            CompFuncContainer,
            LayerFuncContainer,
            SceneFuncContainer

        > m_loaderSavers;

    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/StateLoader.inl>
}

#endif

#define JOP_REGISTER_LOADABLE(className) \
struct ns_##className##_LoadRegistrar{ns_##className##_LoadRegistrar(){jop::StateLoader::getInstance().registerLoadable<className>(#className,

#define JOP_REGISTER_SAVEABLE(className) \
struct ns_##className##_SaveRegistrar{ns_##className##_SaveRegistrar(){jop::StateLoader::getInstance().registerSaveable<className>(#className,

#define JOP_END_LOADABLE_REGISTRATION(className) );}} ns_##className##_LoadRegistrarInstance;
#define JOP_END_SAVEABLE_REGISTRATION(className) );}} ns_##className##_SaveRegistrarInstance;

/// \class SceneLoader
/// \ingroup core
///
/// #TODO Detailed description