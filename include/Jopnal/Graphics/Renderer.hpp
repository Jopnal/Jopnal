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
#include <Jopnal/Graphics/RenderTexture.hpp>
#include <set>

//////////////////////////////////////////////


namespace jop
{
    class World;
    class Window;
    class RenderTarget;

    class JOP_API Renderer
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Renderer);

        friend class Drawable;
        friend class Camera;
        friend class LightSource;
        friend class Scene;
        friend class World;
        friend class EnvironmentRecorder;

    public:

        /// \brief Constructor
        ///
        Renderer(const RenderTarget& mainTarget);

        /// \brief Virtual destructor
        ///
        virtual ~Renderer();


        /// \brief Set the render mask
        ///
        /// \param mask The new mask to set
        ///
        void setMask(const uint32 mask);

        /// \brief Get the render mask
        ///
        /// \return The render mask
        ///
        uint32 getMask() const;

    private:

        void bind(const LightSource& light);

        void bind(const Camera& camera);

        void bind(const Drawable& drawable);

        void bind(EnvironmentRecorder& envRecorder);

        void unbind(const LightSource& light);

        void unbind(const Camera& camera);

        void unbind(const Drawable& drawable);

        void unbind(EnvironmentRecorder& envRecorder);

        virtual void draw();

    protected:

        /// \brief Select the lights that affect the drawable
        ///
        /// \param drawable The drawable
        /// \param lights Reference to a light container to fill
        ///
        void chooseLights(const Drawable& drawable, LightContainer& lights) const;

    private:

        std::set<const LightSource*> m_lights;          ///< The bound lights
        std::set<const Camera*> m_cameras;              ///< The bound cameras
        std::set<const Drawable*> m_drawables;          ///< The bound drawables
        std::set<EnvironmentRecorder*> m_envRecorders;  ///< The bound environment recorders
        uint32 m_mask;                                  ///< The rendering mask
        const RenderTarget& m_mainTarget;

    #ifdef JOP_DEBUG_MODE
        World* m_physicsWorld;                          ///< Pointer to the physics world, used for debug drawing
    #endif
    };
}

#endif