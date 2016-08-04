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


        /// \brief Get the name
        ///
        /// \return Reference to the name
        ///
        const std::string& getName() const;

        /// \brief Set the persistence level
        ///
        /// \param level The persistence level
        ///
        void setPersistence(const unsigned short level);

        /// \brief Get the persistence level
        ///
        /// \return The persistence level
        ///
        unsigned short getPersistence() const;

    private:

        const std::string m_name;       ///< Name of this resource
        unsigned short m_persistence;   ///< Persistence level
    };
}

#endif

/// \class jop::Resource
/// \ingroup core
///
/// This is the base class for all resources.