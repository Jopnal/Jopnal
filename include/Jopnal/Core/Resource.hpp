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

#ifndef JOP_RESOURCE_HPP
#define JOP_RESOURCE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SerializeInfo.hpp>
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <string>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Resource : public SafeReferenceable<Resource>, public SerializeInfo
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Resource);

        friend class ResourceManager;

    protected:

        /// \brief Copy constructor
        ///
        /// This differs from the standard copy constructor signature.
        /// Should you wish your custom resources to be copyable via
        /// the resource manager, you should define a copy constructor
        /// with an similar signature.
        ///
        /// \param other The other resource to copy
        /// \param newName New name for this resource
        ///
        Resource(const Resource& other, const std::string& newName);

    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource. This must be the file path if this resource is loaded from a file.
        ///
        Resource(const std::string& name);

        /// \brief Virtual destructor
        ///
        virtual ~Resource() = 0;


        /// \brief Get the name of this resource
        ///
        /// \return Reference to the name
        ///
        const std::string& getName() const;

        /// \brief Set the persistence level
        ///
        /// The persistence level controls how resources are unloaded
        /// by the \ref ResourceManager "resource manager".
        ///
        /// \param level The persistence level
        ///
        /// \see ResourceManager
        ///
        void setPersistence(const unsigned short level);

        /// \brief Get the persistence level
        ///
        /// \return The persistence level
        ///
        /// \see setPersistence()
        ///
        unsigned short getPersistence() const;

    private:

        const std::string m_name;       ///< Name of this resource
        unsigned short m_persistence;   ///< Persistence level
    };
}

/// \class jop::Resource
/// \ingroup core
///
/// \brief Base class for resources.
///
/// ## Default & error resources
///
/// It's possible to define special resources in case of load failure, to be
/// returned instead by jop::ResourceManager. To do so, define one or both of
/// the following functions for your derived resource class:
///
/// \code{.cpp}
/// static ResourceType& getDefault();
/// static ResourceType& getError();
/// \endcode
/// 
/// If at least one of these exists, the resource manager will use them to fetch
/// a fallback resource if the load() method fails. Make sure that these functions
/// always succeed, so that they don't cause an infinite recursive loop.

#endif