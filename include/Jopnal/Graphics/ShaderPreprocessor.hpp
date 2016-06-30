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

#ifndef JOP_SHADERPREPROCESSOR_HPP
#define JOP_SHADERPREPROCESSOR_HPP

// Headers
#include <Jopnal/Jopnal.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API ShaderPreprocessor : public Subsystem
    {
        /// \brief Constructor
        ///
        /// Load all plugin files from resource.hpp 
        ///
        ShaderPreprocessor(const std::string& id);

        /// \brief Add plugin to shader
        ///
        void addPlugin(const std::string&, const std::string&);

        /// \brief Remove plugin from shader
        ///
        void removePlugin(const std::string&);

        /// \brief Preprocess shader
        ///
        void preprocess(const std::string&,const std::string&);

    };

}

#endif