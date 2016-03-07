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
#include <Jopnal/Core/SubSystem.hpp>
#include <Jopnal/Utility/Json.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <typeindex>

//////////////////////////////////////////////


namespace jop
{
    class Object;

    typedef std::function<bool(Object&, const Scene&, const json::Value&)>                      ComponentLoadFunc;
    typedef std::function<bool(const Component&, json::Value&, json::Value::AllocatorType&)>    ComponentSaveFunc;

    typedef std::function<bool(std::unique_ptr<Scene>&, const json::Value&)>                    SceneLoadFunc;
    typedef std::function<bool(const Scene&, json::Value&, json::Value::AllocatorType&)>        SceneSaveFunc;

    typedef std::function<bool(const json::Value&)>                                             SubsystemLoadFunc;
    typedef std::function<bool(const Subsystem&, json::Value&, json::Value::AllocatorType&)>    SubsystemSaveFunc;

    typedef std::function<bool(const void*, const json::Value&)>                                CustomLoadFunc;
    typedef std::function<bool(const void*, json::Value&, json::Value::AllocatorType&)>         CustomSaveFunc;

    namespace detail
    {
        template
        <
            typename T,
            bool IsComp      = std::is_base_of<Component, T>::value,
            bool IsScene     = std::is_base_of<Scene, T>::value,
            bool IsSubsystem = std::is_base_of<Subsystem, T>::value

        > struct FuncChooser
        {
            typedef CustomLoadFunc LoadFunc;
            typedef CustomSaveFunc SaveFunc;
            typedef std::unordered_map<std::string, std::tuple<LoadFunc, SaveFunc>> FuncContainer;
            enum{ContainerID = 3};
        };
        template<typename T>
        struct FuncChooser<T, true, false, false>
        {
            typedef ComponentLoadFunc LoadFunc;
            typedef ComponentSaveFunc SaveFunc;
            typedef std::unordered_map<std::string, std::tuple<LoadFunc, SaveFunc>> FuncContainer;
            enum{ContainerID = 0};
        };
        template<typename T>
        struct FuncChooser<T, false, true, false>
        {
            typedef SceneLoadFunc LoadFunc;
            typedef SceneSaveFunc SaveFunc;
            typedef std::unordered_map<std::string, std::tuple<LoadFunc, SaveFunc>> FuncContainer;
            enum{ContainerID = 1};
        };
        template<typename T>
        struct FuncChooser<T, false, false, true>
        {
            typedef SubsystemLoadFunc LoadFunc;
            typedef SubsystemSaveFunc SaveFunc;
            typedef std::unordered_map<std::string, std::tuple<LoadFunc, SaveFunc>> FuncContainer;
            enum{ContainerID = 2};
        };
    }

    class JOP_API StateLoader
    {
    private:

        /// \brief Default constructor
        ///
        StateLoader() = default;

    public:

        /// \brief Get the single StateLoader instance
        ///
        /// \return Reference to the instance
        ///
        static StateLoader& getInstance();
    

        /// \brief Register a loadable object
        ///
        /// Only use this function directly if you have a reason for not wanting
        /// to use the registration macros macros.
        ///
        /// \param id The type name
        /// \param func The registration function
        ///
        template<typename T>
        void registerLoadable(const char* id, const typename detail::FuncChooser<T>::LoadFunc& func);

        /// \brief Register a saveable object
        ///
        /// The same remarks as with registerLoadable() apply.
        ///
        /// \param id The type name
        /// \param func The registration function
        ///
        /// \see registerLoadable()
        ///
        template<typename T>
        void registerSaveable(const char* id, const typename detail::FuncChooser<T>::SaveFunc& func);
        

        /// \brief Save the current state
        ///
        /// \param path The file path + name to save into. The .jop extension will be appended automatically
        /// \param scene Save the current scene?
        /// \param sharedScene Save the shared scene?
        /// \param subsystems Save the sub systems?
        ///
        /// \return True if successful
        ///
        static bool saveState(const std::string& path, const bool scene = true, const bool sharedScene = false);

        /// \brief Load the current state
        ///
        /// \param path The file path + name to load from. The .jop extension will be appended automatically
        /// \param scene Load the current scene?
        /// \param sharedScene Load the shared scene?
        /// \param subsystems Load the sub systems?
        ///
        /// \return True if successful
        ///
        static bool loadState(const std::string& path, const bool scene = true, const bool sharedScene = false);


        /// \brief Get the container with the registered load/save functions for the type T
        ///
        /// \return Reference to the container
        ///
        template<typename T>
        static const typename detail::FuncChooser<T>::FuncContainer& getFunctionContainer();

        /// \brief Get the container with the type names used while saving
        ///
        /// \return Reference to the container
        ///
        static const std::unordered_map<std::type_index, std::string>& getSavenameContainer();

        /// \brief Check is the state is currently loading
        ///
        /// \return True if currently loading
        ///
        static bool currentlyLoading();

    private:

        bool loadScene(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path);

        bool loadObjects(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path);

        bool loadObject(Object& obj, const Scene& scene, const json::Value& data, const std::string& path);


        bool saveScene(const Scene& scene, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path);

        bool saveObjects(const Scene& scene, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path);

        bool saveObject(const Object& obj, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path);


        /// Tuple with the function containers
        ///
        std::tuple
        <
            detail::FuncChooser<Component>::FuncContainer,  // Component
            detail::FuncChooser<Scene>::FuncContainer,      // Scene
            detail::FuncChooser<Subsystem>::FuncContainer,  // Subsystem
            detail::FuncChooser<void>::FuncContainer,       // Custom (other)

            // Maps the type info to identifiers, to be used when saving. Keep this last
            std::unordered_map<std::type_index, std::string>

        > m_loaderSavers;
        bool m_loading; ///< Currently loading flag

    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/StateLoader.inl>
}

#endif

/// Macro for statically registering a loadable object
///
#define JOP_REGISTER_LOADABLE(nameSpace, className) \
struct ns_##className##_LoadRegistrar{ns_##className##_LoadRegistrar(){jop::StateLoader::getInstance().registerLoadable<className>(#nameSpace "::" #className,

/// Macro for statically registering a saveable object
///
#define JOP_REGISTER_SAVEABLE(nameSpace, className) \
struct ns_##className##_SaveRegistrar{ns_##className##_SaveRegistrar(){jop::StateLoader::getInstance().registerSaveable<className>(#nameSpace "::" #className,

#define JOP_END_LOADABLE_REGISTRATION(className) );}} ns_##className##_LoadRegistrarInstance;   ///< End loadable registration
#define JOP_END_SAVEABLE_REGISTRATION(className) );}} ns_##className##_SaveRegistrarInstance;   ///< End saveable registration

/// \class SceneLoader
/// \ingroup core
///
/// #TODO Detailed description