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

//////////////////////////////////////////////


namespace jop
{
    class JOP_API ModelLoader : public Component
    {
    private:

        /// \brief Copy constructor
        ///
        /// \param other The other loader to copy
        /// \param newObj The new object
        ///
        ModelLoader(const ModelLoader& other, Object& obj);

        JOP_DISALLOW_COPY_MOVE(ModelLoader);
        JOP_GENERIC_COMPONENT_CLONE(ModelLoader);

    public:

        /// Optional options structure
        ///
        struct JOP_API Options
        {
            /// \brief Constructor
            ///
            /// Initializes the default values.
            ///
            Options();

            /// Force the use of a diffuse alpha channel if one is available.
            ///
            /// False by default.
            ///
            bool forceDiffuseAlpha;

            /// Collapse the object tree.
            ///
            /// This will reduce the number of draw calls, but destroys the
            /// original object tree hierarchy and naming.
            ///
            /// True by default.
            ///
            bool collapseTree;

            /// Attempt to fix in-facing normals.
            ///
            /// The results are correct most of the time. If you're having problems
            /// with lighting, try disabling this.
            ///
            /// True by default.
            ///
            bool fixInfacingNormals;
        };

    public:

        /// \brief Constructor
        ///
        /// \param obj The object this loader is bound to
        ///
        ModelLoader(Object& obj);


        /// \brief Load a model from file
        ///
        /// This will create the object tree and load the correct meshes and materials.
        ///
        /// \warning If the model file has a name for the root node, the root object's
        ///          identifier will be changed accordingly. Be aware of this when
        ///          using this function.
        ///
        /// \param path Path to the model file
        /// \param options Optional options structure
        ///
        /// \return True if successful
        ///
        bool load(const std::string& path, const Options& options = Options());

    private:

        std::string m_path; ///< Path to model file
    };
}

#endif