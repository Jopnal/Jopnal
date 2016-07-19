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
#include <Jopnal/Graphics/RenderPass.hpp>
#include <set>

//////////////////////////////////////////////


namespace jop
{
    class LightContainer;
    class RenderTarget;

    class JOP_API Renderer
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Renderer);

        friend class Camera;
        friend class LightSource;
        friend class Scene;
        friend class World;
        friend class EnvironmentRecorder;
        friend class RenderPass;
        friend class Drawable;

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

        void draw(const RenderPass::Pass pass);

    private:

        void bind(const LightSource& light);

        void bind(const Camera& camera);

        void bind(const Drawable& drawable, const RenderPass::Pass pass);

        void bind(EnvironmentRecorder& envRecorder);

        void unbind(const LightSource& light);

        void unbind(const Camera& camera);

        void unbind(const Drawable& drawable, const RenderPass::Pass pass);

        void unbind(EnvironmentRecorder& envRecorder);

    protected:

        /// \brief Select the lights that affect the drawable
        ///
        /// \param drawable The drawable
        /// \param lights Reference to a light container to fill
        ///
        void chooseLights(const Drawable& drawable, LightContainer& lights) const;

    private:

        RenderPass::Lights m_lights;                        ///< The bound lights
        RenderPass::Cameras m_cameras;                      ///< The bound cameras
        std::array<RenderPass, 3> m_passes;        
        RenderPass::EnvironmentRecorders m_envRecorders;    ///< The bound environment recorders
        uint32 m_mask;                                      ///< The rendering mask
        const RenderTarget& m_mainTarget;
    };
}

#endif