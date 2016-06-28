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

#ifndef JOP_SERIALIZER_HPP
#define JOP_SERIALIZER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Core/Object.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Core/Scene.hpp>
#include <Jopnal/Utility/Json.hpp>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <typeindex>
#include <atomic>

//////////////////////////////////////////////


#define JOP_COMPONENT_FACTORY_ARGS Object& obj, const Scene& scene, const json::Value& val
#define JOP_COMPONENT_LOAD_ARGS Component& comp, const json::Value& val
#define JOP_COMPONENT_SAVE_ARGS const Component& comp, json::Value& val, json::Value::AllocatorType& alloc

#define JOP_SCENE_FACTORY_ARGS const json::Value& val
#define JOP_SCENE_LOAD_ARGS Scene& scene, const json::Value& val
#define JOP_SCENE_SAVE_ARGS const Scene& scene, json::Value& val, json::Value::AllocatorType& alloc

#define JOP_RESOURCE_FACTORY_ARGS const json::Value& val
#define JOP_RESOURCE_LOAD_ARGS Resource& res, const json::Value& val
#define JOP_RESOURCE_SAVE_ARGS const Resource& res, json::Value& val, json::Value::AllocatorType& alloc

namespace jop
{
    class Object;

    typedef std::function<Component&(JOP_COMPONENT_FACTORY_ARGS)>   ComponentFactoryFunc;
    typedef std::function<bool(JOP_COMPONENT_LOAD_ARGS)>            ComponentLoadFunc;
    typedef std::function<bool(JOP_COMPONENT_SAVE_ARGS)>            ComponentSaveFunc;

    typedef std::function<Scene&(JOP_SCENE_FACTORY_ARGS)>           SceneFactoryFunc;
    typedef std::function<bool(JOP_SCENE_LOAD_ARGS)>                SceneLoadFunc;
    typedef std::function<bool(JOP_SCENE_SAVE_ARGS)>                SceneSaveFunc;

    typedef std::function<Resource&(JOP_RESOURCE_FACTORY_ARGS)>     ResourceFactoryFunc;
    typedef std::function<bool(JOP_RESOURCE_LOAD_ARGS)>             ResourceLoadFunc;
    typedef std::function<bool(JOP_RESOURCE_SAVE_ARGS)>             ResourceSaveFunc;

    namespace detail
    {
        template
        <
            typename T,
            bool IsComp     = std::is_base_of<Component,    T>::value,
            bool IsScene    = std::is_base_of<Scene,        T>::value,
            bool IsResource = std::is_base_of<Resource,     T>::value

        > struct FuncChooser{};

        template<typename T>
        struct FuncChooser<T, true, false, false>
        {
            typedef ComponentFactoryFunc FactoryFunc;
            typedef ComponentLoadFunc LoadFunc;
            typedef ComponentSaveFunc SaveFunc;
            typedef std::unordered_map<std::string, std::tuple<FactoryFunc, LoadFunc, SaveFunc>> FuncContainer;
            enum{ContainerID = 0};
        };
        template<typename T>
        struct FuncChooser<T, false, true, false>
        {
            typedef SceneFactoryFunc FactoryFunc;
            typedef SceneLoadFunc LoadFunc;
            typedef SceneSaveFunc SaveFunc;
            typedef std::unordered_map<std::string, std::tuple<FactoryFunc, LoadFunc, SaveFunc>> FuncContainer;
            enum{ContainerID = 1};
        };
        template<typename T>
        struct FuncChooser<T, false, false, true>
        {
            typedef ResourceFactoryFunc FactoryFunc;
            typedef ResourceLoadFunc LoadFunc;
            typedef ResourceSaveFunc SaveFunc;
            typedef std::unordered_map<std::string, std::tuple<FactoryFunc, LoadFunc, SaveFunc>> FuncContainer;
            enum{ContainerID = 2};
        };
    }

    class JOP_API Serializer final
    {
    private:

        /// \brief Default constructor
        ///
        Serializer() = default;

    public:

        struct Modes
        {
            enum : uint32
            {
                SceneReplace        = 1,
                SceneReload         = 1 << 1,
                ResourceReplace     = 1 << 2,
                ResourceReload      = 1 << 3
            };
        };

        struct FunctionID
        {
            enum : uint32
            {
                Load = 1,
                Save
            };
        };

    public:

        /// \brief Get the single SceneLoader instance
        ///
        /// \return Reference to the instance
        ///
        static Serializer& getInstance();
    

        /// \brief 
        ///
        template<typename T>
        void registerSerializeable(const std::string& id,
                                   const typename detail::FuncChooser<T>::FactoryFunc& factFunc,
                                   const typename detail::FuncChooser<T>::LoadFunc& loadFunc,
                                   const typename detail::FuncChooser<T>::SaveFunc& saveFunc);
        
        /// \brief 
        ///
        /// \return True if successful
        ///
        static bool save(const std::string& descPath, const uint32 modes);

        /// \brief 
        ///
        /// \return True if successful
        ///
        static bool load(const std::string& descPath, const uint32 modes);


        template<typename T, uint32 FuncID, typename ... Args>
        static bool callSingleFunc(const std::string& id, Args&&... args);

        template<typename T>
        static const std::string& getSerializeID();

    private:

        /// Tuple with the function containers
        ///
        std::tuple
        <
            detail::FuncChooser<Component>::FuncContainer,  // Component
            detail::FuncChooser<Scene>::FuncContainer,      // Scene
            detail::FuncChooser<Resource>::FuncContainer,   // Resource

            // Maps the type info to identifiers, to be used when saving. Keep this last
            std::unordered_map<std::type_index, std::string>

        > m_loaderSavers;
        std::atomic<bool> m_loading; ///< Currently loading flag
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Serializer.inl>
}

#define JOP_REGISTER_SERIALIZER(nameSpace, className, factoryFunc, loadFunc, saveFunc)  \
    static const struct __ns_##className##_SerializeRegistrar                           \
    {                                                                                   \
        __ns_##className##_SerializeRegistrar()                                         \
        {                                                                               \
            jop::Serializer::getInstance()                                              \
            .registerSerializeable<className>                                           \
            (                                                                           \
                #nameSpace "::" #className,                                             \
                factoryFunc, loadFunc, saveFunc                                         \
            );                                                                          \
        }                                                                               \
    } __ns_##className##_SerializeRegistrar

#define JOP_REGISTER_SERIALIZER_NO_FACTORY(nameSpace, className, loadFunc, saveFunc)    \
        JOP_REGISTER_SERIALIZER(nameSpace, className,                                   \
        jop::detail::FuncChooser<className>::FactoryFunc(), loadFunc, saveFunc)

#endif

/// \class SceneLoader
/// \ingroup core
///
/// #TODO Detailed description