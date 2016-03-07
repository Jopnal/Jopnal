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

#ifndef JOP_RENDERER_HPP
#define JOP_RENDERER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <set>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Renderer
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Renderer);

        friend class Drawable;
        friend class Camera;
        friend class LightSource;
        friend class Scene;

    public:

        Renderer();

        virtual ~Renderer();


        void setMask(const uint32 mask);

        uint32 getMask() const;


    private:

        void bind(const LightSource& light);

        void bind(const Camera& camera);

        void bind(const Drawable& drawable);

        void unbind(const LightSource& light);

        void unbind(const Camera& camera);

        void unbind(const Drawable& drawable);

        virtual void draw();

        void chooseLights(const Drawable& drawable, LightContainer& lights) const;

    private:

        std::set<const LightSource*> m_lights;
        std::set<const Camera*> m_cameras;
        std::set<const Drawable*> m_drawables;
        uint32 m_mask;

    };
}

#endif