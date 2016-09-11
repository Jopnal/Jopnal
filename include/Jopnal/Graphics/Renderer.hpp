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
#include <Jopnal/Core/Object.hpp>
#include <Jopnal/Graphics/RenderPass.hpp>
#include <Jopnal/Physics/World.hpp>
#include <Jopnal/STL.hpp>
#include <map>
#include <array>

//////////////////////////////////////////////


namespace jop
{
    class LightSource;
    class Camera;
    class EnvironmentRecorder;
    class LightContainer;
    class RenderTarget;

    class JOP_API Renderer final
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Renderer);

        typedef std::array<std::map<uint32, std::unique_ptr<RenderPass>>, 2> PassContainer;
        typedef std::set<const Camera*> CameraSet;
        typedef std::set<const LightSource*> LightSet;

        friend class Camera;
        friend class LightSource;
        friend class EnvironmentRecorder;
        friend class Drawable;

    public:

        /// \brief Constructor
        ///
        /// \param mainTarget Reference to the main render target
        ///
        Renderer(const RenderTarget& mainTarget, Scene& sceneRef);


        /// \brief Get the bound render target
        ///
        /// \return Reference to the render target
        ///
        const RenderTarget& getRenderTarget() const;

        /// \brief Get the camera set
        ///
        /// \return Reference to the camera set
        ///
        const CameraSet& getCameras() const;

        /// \brief Get the light set
        ///
        /// \return Reference to the light set
        ///
        const LightSet& getLights() const;

        /// \brief Create a new render pass
        ///
        /// If a pass with the same type and weight already exists, it will be replaced.
        ///
        /// \param pass The render pas type
        /// \param weight The weight. Lesser weight means higher priority during rendering
        /// \param args The arguments to pass to the render pass' constructor
        ///
        /// \return Reference to the newly created render pass
        ///
        template<typename T, typename ... Args>
        T& createRenderPass(const RenderPass::Pass pass, const uint32 weight, Args&&... args);

        /// \brief Get a render pass
        ///
        /// \param pass The render pass type
        /// \param weight The weight
        ///
        /// \return Pointer to the render pass. nullptr if no pass exists with the given type and weight
        ///
        template<typename T>
        T* getRenderPass(const RenderPass::Pass pass, const uint32 weight);

        /// \brief Remove and delete a render pass
        ///
        /// You must only call this after all the bound drawables have been removed.
        ///
        /// \param pass Render pass type
        /// \param weight The weight of the pass to remove
        ///
        void removeRenderPass(const RenderPass::Pass pass, const uint32 weight);

        /// \brief Draw
        ///
        /// \param pass The render passes to draw
        ///
        void draw(const RenderPass::Pass pass);

        /// \brief Get the culling world
        ///
        World& getCullingWorld();

    private:

        void bind(const LightSource* light);

        void bind(const Camera* camera);

        void bind(const Drawable* drawable, const RenderPass::Pass pass, const uint32 weight);

        void bind(const EnvironmentRecorder* envRecorder);

        void unbind(const LightSource* light);

        void unbind(const Camera* camera);

        void unbind(const Drawable* drawable, const RenderPass::Pass pass, const uint32 weight);

        void unbind(const EnvironmentRecorder* envRecorder);

    private:

        LightSet m_lights;                                      ///< The bound lights
        CameraSet m_cameras;                                    ///< The bound cameras
        PassContainer m_passes;                                 ///< Render passes
        std::set<const EnvironmentRecorder*> m_envRecorders;    ///< The bound environment recorders
        const RenderTarget& m_target;                           ///< Main render target reference
        Scene& m_sceneRef;
    };

    // Include template implementation file
    #include <Jopnal/Graphics/Inl/Renderer.inl>
}

/// \class jop::Renderer
/// \ingroup graphics

#endif
