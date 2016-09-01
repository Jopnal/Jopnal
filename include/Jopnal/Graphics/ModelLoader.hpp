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

#ifndef JOP_MODELLOADER_HPP
#define JOP_MODELLOADER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <glm/vec3.hpp>
#include <utility>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API ModelLoader : public Component
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(ModelLoader);

    public:

        /// \brief Constructor
        ///
        /// \param obj The object this loader is bound to
        ///
        explicit ModelLoader(Object& obj);


        /// \brief Load a model from file
        ///
        /// This will create the object tree and load the correct meshes and materials.
        /// Only models converted with [Jopmodel](https://github.com/Jopnal/Jopmodel) are supported.
        ///
        /// \param path Path to the model file
        ///
        /// \return True if successful
        ///
        bool load(const std::string& path);

        /// \brief Get the local bounds
        ///
        /// \return The local bounds
        ///
        const std::pair<glm::vec3, glm::vec3>& getLocalBounds() const;

        /// \brief Get the global bounds
        ///
        /// \return The global bounds
        ///
        std::pair<glm::vec3, glm::vec3> getGlobalBounds() const;

    private:

        std::string m_path;                             ///< Path to model file
        std::pair<glm::vec3, glm::vec3> m_localBounds;  ///< Local bounds
    };
}

/// \class jop::ModelLoader
/// \ingroup graphics

#endif