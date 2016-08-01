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

#ifndef JOP_RESOURCEMANAGER_HPP
#define JOP_RESOURCEMANAGER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Core/Subsystem.hpp>
#include <Jopnal/Core/DebugHandler.hpp>
#include <Jopnal/Utility/Clock.hpp>
#include <Jopnal/STL.hpp>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <typeindex>
#include <mutex>

//////////////////////////////////////////////


namespace std
{
    template<>
    struct JOP_API hash<std::pair<std::string, std::type_index>>
    {
        std::size_t operator()(const std::pair<std::string, std::type_index>& pair) const;
    };
}

namespace jop
{
    class Resource;

    class JOP_API ResourceManager : public Subsystem
    {
    public:

        /// \brief Default constructor
        ///
        ResourceManager();

        /// \brief Destructor
        ///
        ~ResourceManager() override;


        /// \brief Get a resource
        ///
        /// If resource is not found this creates a new one. The first argument must be convertible
        /// into std::string, as it's used as a hash map key.
        ///
        /// \param args Arguments passed to resource's constructor
        ///
        /// \return Reference to the resource
        ///
        template<typename T, typename ... Args>
        static T& get(Args&&... args);

        /// \brief Get a named resource
        ///
        /// This is primarily used when the resource is not loaded from a file. After the resource
        /// has been loaded once, it can be retrieved by using getResource().
        ///
        /// \param name Name for the resource
        /// \param args Arguments passed to resource's constructor
        ///
        /// \return Reference to the resource
        ///
        template<typename T, typename ... Args>
        static T& getNamed(const std::string& name, Args&&... args);

        /// \brief Get an empty resource
        ///
        /// This function will replace the resource if one already exists with the same name.
        ///
        /// \param args Arguments to pass to the resource's constructor
        ///
        /// \return Reference to the resource
        ///
        template<typename T, typename ... Args>
        static T& getEmpty(Args&&... args);

        /// \brief Get an existing resource
        ///
        /// This function will not attempt to create the resource if it's not found
        /// or is not of the matching type. Instead a default is returned.
        ///
        /// \param name Name of the resource
        ///
        /// \return Reference to the resource
        ///
        template<typename T>
        static T& getExisting(const std::string& name);

        /// \brief Check is a resource exists
        ///
        /// The name and type must both match.
        ///
        /// \param name Name of the resource
        ///
        /// \return True if the resource exists
        /// 
        template<typename T>
        static bool exists(const std::string& name);


        /// \brief Copy a resource
        ///
        /// This function requires that the resource has a valid copy constructor.
        /// If the resource is not found or is of the wrong type, the default/error
        /// resource will be returned instead.
        ///
        /// \param name Name of the resource to copy
        /// \param newName Name of the copied resource
        ///
        /// \return Reference to the resource
        ///
        template<typename T>
        static T& copy(const std::string& name, const std::string& newName);


        /// \brief Deletes resources from memory
        ///
        /// This will delete all the resources with the given name, regardless of type.
        ///
        /// The resources, if found, will be deleted regardless of the persistence flag.
        /// Resources with the persistence level of 0 will not be removed, however.
        ///
        /// \param name Name of the resources to unload
        ///
        static void unload(const std::string& name);

        /// \brief Delete a resource from memory
        ///
        /// When possible, you should prefer this overload.
        /// It's possibly magnitudes faster.
        ///
        /// \param name Name of the resource
        ///
        template<typename T>
        static void unload(const std::string& name);

        /// \brief Deletes all resources from memory
        ///
        /// \param persistence The persistence of the resources to unload
        /// \param descending Set true to unload all resources with the given and below persistence levels
        ///
        static void unload(const unsigned short persistence = 0xFFFF, const bool descending = true);


        /// \brief Mark the beginning of a resource loading phase
        ///
        /// This function is provided to make it easier to manage big amounts of resources. When called, every resource
        /// that is referenced after, will become flagged. When the load phase is ended, all resources <b>not</b> flagged
        /// will be removed, provided they pass the persistence test.
        ///
        /// \warning It's very important to call endLoadPhase() after this, right after all the needed resources are loaded
        ///
        /// \see endLoadPhase()
        ///
        static void beginLoadPhase();

        /// \brief Mark the ending of a resource loading phase
        ///
        /// If a load phase was previously initiated, calling this function will end it and clear
        /// all resources as described on beginLoadPhase().
        ///
        /// \param persistence The maximum persistence level to take into account. Resources with equal or greater
        ///                    persistence level will be removed
        ///
        /// \see beginLoadPhase()
        ///
        static void endLoadPhase(const uint16 persistence);

        template<typename T>
        static bool isDefault(const T& resource);

        template<typename T>
        static bool isError(const T& resource);

    private:

        static ResourceManager* m_instance;         ///< Pointer to the single instance

        std::unordered_map
        <
            std::pair<std::string, std::type_index>,
            std::unique_ptr<Resource>
        > m_resources;                              ///< Container holding resources
        std::unordered_set
        <
            std::pair<std::string, std::type_index>
        > m_loadPhaseResources;                     ///< Resource keys loaded during a load phase
        std::atomic<bool> m_loadPhase;              ///< Is it load phase currently?
        std::recursive_mutex m_mutex;               ///< Mutex
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/ResourceManager.inl>
}

#endif

/// \class ResourceManager
/// \ingroup core