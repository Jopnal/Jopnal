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

// Headers
#include <Jopnal/Precompiled.hpp>

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/ModelLoader.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    ModelLoader::Options::Options()
        : forceDiffuseAlpha     (false),
          collapseTree          (true),
          fixInfacingNormals    (true)
    {}

    //////////////////////////////////////////////

    ModelLoader::ModelLoader(Object& obj)
        : Component (obj, 0),
          m_path    ()
    {}

    ModelLoader::ModelLoader(Object& obj, const std::string& path, const Options& options)
        : Component (obj, 0),
          m_path    ()
    {
        load(path, options);
    }

    ModelLoader::ModelLoader(const ModelLoader& other, Object& obj)
        : Component (other, obj),
          m_path    (other.m_path)
    {}

    //////////////////////////////////////////////

    bool ModelLoader::load(const std::string& path, const Options& options)
    {
        

        return false;
    }
}