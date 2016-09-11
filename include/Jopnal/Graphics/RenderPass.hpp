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
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class Renderer;
    class Drawable;
    class RenderTarget;

    class JOP_API RenderPass
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderPass);

        friend class Renderer;
        friend class Drawable;

    public:

        /// \brief The default render pass weight
        ///
        /// Defined as 0x88888888
        ///
        static const uint32 DefaultWeight;

        /// Render pass type
        ///
        enum class Pass
        {
            BeforePost, ///< Before post-processing
            AfterPost   ///< After post-processing
        };

    public:

        /// \brief Constructor
        ///
        /// \param renderer Reference to the renderer
        /// \param target Reference to the render target
        /// \param pass The render pass type
        /// \param weight The weight of this render pass
        ///
        RenderPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight);

        /// \brief Virtual destructor
        ///
        virtual ~RenderPass() = 0;


        /// \brief Set this render pass active/inactive
        ///
        /// \param active True to set active
        ///
        void setActive(const bool active);

        /// \brief Check if this render pass is active
        ///
        /// \return True if active
        ///
        bool isActive() const;

        /// \brief Get the renderer this pass is bound to
        ///
        /// \return Reference to the renderer
        ///
        Renderer& getRenderer();

        /// \brief Get the render pass type
        ///
        /// \return The render pass type
        ///
        Pass getPass() const;

        /// \brief Set the default render pass type
        ///
        /// \param pass The default pass type
        ///
        static void setDefaultType(const Pass pass);

        /// \brief Set the default render pass weight
        ///
        /// \param weight The default weight
        ///
        static void setDefaultWeight(const uint32 weight);

        /// \brief Get the default pass type
        ///
        /// \return Default pass type
        ///
        static Pass getDefaultType();

        /// \brief Get the default pass weight
        ///
        /// \return Default pass weight
        ///
        static uint32 getDefaultWeight();

    protected:

        /// \brief Virtual draw function
        ///
        virtual void draw() = 0;

        /// \brief Bind a drawable
        ///
        /// \param drawable The drawable to bind
        ///
        virtual void bind(const Drawable* drawable) = 0;

        /// \brief Unbind a drawable
        ///
        /// \param drawable The drawable to unbind
        ///
        virtual void unbind(const Drawable* drawable) = 0;


        Renderer& m_rendererRef;        ///< Reference to the renderer
        const RenderTarget& m_target;   ///< Reference to the render target
        const uint32 m_weight;          ///< Weight value
        const Pass m_pass;              ///< Render pass type
        bool m_active;                  ///< Is this render pass active?
    };

    /// \brief Distance-sorted render pass
    ///
    /// Sorts opaque drawables from nearest to farthest and translucent
    /// drawables the other way around.
    ///
    /// Draws its drawables in three different sets:
    /// 1. Opaque objects
    /// 2. Sky boxes/spheres
    /// 3. Translucent objects
    ///
    /// By default Renderer creates one of these as a BeforePost pass, using
    /// RenderPass::DefaultWeight.
    ///
    struct JOP_API SortedRenderPass : RenderPass
    {
        /// \copydoc RenderPass::RenderPass()
        ///
        SortedRenderPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight);

    protected:

        /// \copydoc RenderPass::draw()
        ///
        void draw() override;

        /// \copydoc RenderPass::bind()
        ///
        void bind(const Drawable* drawable) override;

        /// \copydoc RenderPass::unbind()
        ///
        void unbind(const Drawable* drawable) override;

    private:

        std::set<const Drawable*> m_drawables; ///< Drawable set
    };

    /// \brief Orderer render pass
    ///
    /// Maintains the order in which drawables are added.
    ///
    /// \note This pass by default doesn't do depth testing or writing
    /// \note Lighting is disabled for all drawables bound to this kind of pass
    ///
    /// By default Renderer creates one of these as a AfterPost pass, using
    /// RenderPass::DefaultWeight.
    ///
    struct JOP_API OrderedRenderPass : RenderPass
    {
        /// \copydoc RenderPass::RenderPass()
        ///
        OrderedRenderPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight);

    protected:

        /// \copydoc RenderPass::draw()
        ///
        void draw() override;

        /// \copydoc RenderPass::bind()
        ///
        void bind(const Drawable* drawable) override;

        /// \copydoc RenderPass::unbind()
        ///
        void unbind(const Drawable* drawable) override;

    private:

        std::vector<const Drawable*> m_drawables; ///< Drawable vector
    };

    namespace detail
    {
        struct RenderPassProxy final : Subsystem
        {
            RenderPassProxy(const RenderPass::Pass pass);

            void draw() override;

        private:

            const RenderPass::Pass m_pass;
        };
    }
}

/// \class jop::RenderPass
/// \ingroup graphics

#endif