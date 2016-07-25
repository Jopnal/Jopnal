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

#ifndef JOP_RENDERPASS_HPP
#define JOP_RENDERPASS_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <set>

//////////////////////////////////////////////


namespace jop
{
    class Renderer;
    class LightSource;
    class Camera;
    class EnvironmentRecorder;
    class Drawable;
    class LightContainer;
    class RenderTarget;

    class JOP_API RenderPass final
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderPass);

        friend class Renderer;
        friend class EnvironmentRecorder;

    public:

        typedef std::set<const LightSource*>    Lights;
        typedef std::set<const Camera*>         Cameras;
        typedef std::set<EnvironmentRecorder*>  EnvironmentRecorders;
        typedef std::set<const Drawable*>       Drawables;

        enum class Pass
        {
            Pre,
            Forward,
            Post
        };

    public:

        RenderPass(Renderer& renderer, const RenderTarget& target);


        void draw(const unsigned int group);

        void bind(const Drawable& drawable);

        void unbind(const Drawable& drawable);

    private:

        /// \brief Select the lights that affect the drawable
        ///
        /// \param drawable The drawable
        /// \param lights Reference to a light container to fill
        ///
        void chooseLights(const Drawable& drawable, LightContainer& lights) const;

        Drawables m_drawables;
        Renderer& m_rendererRef;
        const RenderTarget& m_target;
    };

    namespace detail
    {
        class RenderPassProxy final : public Subsystem
        {
        public:

            RenderPassProxy(const RenderPass::Pass pass);

            void draw() override;

        private:

            const RenderPass::Pass m_pass;
        };
    }
}

#endif