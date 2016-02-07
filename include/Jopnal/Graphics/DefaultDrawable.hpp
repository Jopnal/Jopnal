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

#ifndef JOP_DEFAULTDRAWABLE_HPP
#define JOP_DEFAULTDRAWABLE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API DefaultDrawable : public Drawable
    {
    public:

        /// \brief Constructor
        ///
        /// This doesn't take the layer as an argument. Instead it fetches the default layer
        /// of the currently active scene.
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param ID Unique component identifier
        ///
        DefaultDrawable(Object& object, const std::string& ID);


        virtual DefaultDrawable* clone() const override;

        virtual void draw(const Camera& camera) override;

    };
}

#endif