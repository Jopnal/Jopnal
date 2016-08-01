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

    class JOP_API RenderPass
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderPass);

        friend class Renderer;
        friend class EnvironmentRecorder;

    public:

        typedef std::set<const LightSource*>            Lights;
        typedef std::set<const Camera*>                 Cameras;
        typedef std::set<const EnvironmentRecorder*>    EnvironmentRecorders;
        typedef std::set<const Drawable*>               Drawables;

        static const uint32 DefaultWeight;

        enum class Pass
        {
            BeforePost,
            AfterPost
        };

    public:

        RenderPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight);

        virtual ~RenderPass() = 0;


        virtual void draw() = 0;

        void bind(const Drawable* drawable);

        void unbind(const Drawable* drawable);

        void setActive(const bool active);

        bool isActive() const;

        Renderer& getRenderer();

        Pass whichPass() const;

    protected:

        Drawables m_drawables;
        Renderer& m_rendererRef;
        const RenderTarget& m_target;
        const uint32 m_weight;
        const Pass m_pass;
        bool m_active;
    };

    class JOP_API DefaultPrePass : public RenderPass
    {
    public:

        DefaultPrePass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight);

        void draw() override;
    };

    class JOP_API DefaultPostPass : public RenderPass
    {
        DefaultPostPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight);

        void draw() override;
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