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

#ifndef JOP_DRAWABLE_HPP
#define JOP_DRAWABLE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <Jopnal/Graphics/RenderPass.hpp>
#include <Jopnal/Utility/Json.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <utility>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        class CullerComponent;
    }
    class ShaderProgram;
    class LightSource;
    class LightContainer;
    class Renderer;
    class Material;
    class Mesh;

    class JOP_API Drawable : public Component
    {
    protected:

        JOP_GENERIC_COMPONENT_CLONE(Drawable);

        friend class ShaderAssembler;

    public:
    
        /// Drawable flags
        ///
        enum Flag : uint32
        {
            ReceiveLights   = 1,        ///< Receive lights?
            ReceiveShadows  = 1 << 1,   ///< Receive shadows?
            CastShadows     = 1 << 2,   ///< Cast shadows?
            Reflected       = 1 << 3    ///< Reflect on dynamic environment maps?
        };

        /// Attribute flags
        ///
        struct Attribute
        {
            enum : uint64
            {
                __SkySphere = 1 << 10,
                __SkyBox    = __SkySphere << 1
            };
        };

        /// Projection info
        ///
        struct JOP_API ProjectionInfo
        {
            JOP_DISALLOW_COPY_MOVE(ProjectionInfo);

        public:

            /// \brief Constructor
            ///
            /// \param view The view matrix
            /// \param proj The projection matrix
            /// \param camPos The camera position
            ///
            ProjectionInfo(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camPos);

            const glm::mat4& viewMatrix;        ///< View matrix
            const glm::mat4& projectionMatrix;  ///< Projection matrix
            const glm::vec3& cameraPosition;    ///< Camera position
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param renderer Reference to the renderer
        /// \param cull Should this drawable be culled?
        ///
        Drawable(Object& object, Renderer& renderer, const bool cull = true);

        /// \copydoc Drawable::Drawable(Object&,Renderer&,const bool)
        ///
        /// \param pass The render pass type
        /// \param weight The render pass weight
        ///
        Drawable(Object& object, Renderer& renderer, const RenderPass::Pass pass, const uint32 weight, const bool cull = true);

        /// \brief Virtual destructor
        ///
        virtual ~Drawable() override;


        void update(const float deltaTime) override;

        /// \brief Draw function
        /// 
        /// \param proj The projection info
        /// \param lights The light container
        ///
        virtual void draw(const ProjectionInfo& proj, const LightContainer& lights) const;

        /// \brief Get the renderer this drawable is bound to
        ///
        /// \return Reference to the renderer
        ///
        Renderer& getRendrer();

        /// \brief Get the renderer this drawable is bound to
        ///
        /// \return Reference to the renderer
        ///
        const Renderer& getRenderer() const;

        /// \brief Set the render group
        ///
        /// The value will be clamped between 0 and 31.
        ///
        /// \param group The new group to set
        ///
        /// \comm setRenderGroup
        ///
        Drawable& setRenderGroup(const uint8 group);

        /// \brief Get the render group
        ///
        /// \return The render group 
        ///
        uint8 getRenderGroup() const;

        /// \brief Set the model (mesh & material)
        ///
        /// \param mesh Reference to the mesh
        /// \param material Reference to the material
        ///
        /// \return Reference to self
        ///
        /// \comm setModel
        ///
        Drawable& setModel(const Mesh& mesh, const Material& material);

        /// \brief Set the mesh
        ///
        /// \param mesh Reference to the mesh
        ///
        /// \return Reference to self
        ///
        Drawable& setMesh(const Mesh& mesh);

        /// \brief Set the material
        ///
        /// \param material Reference to the material
        ///
        /// \return Reference to self
        ///
        Drawable& setMaterial(const Material& material);

        /// \brief Get the mesh
        ///
        /// \return Reference to the mesh. nullptr if none bound
        ///
        const Mesh* getMesh() const;

        /// \brief Get the material
        ///
        /// \return Reference to the material. nullptr if none bound
        ///
        const Material* getMaterial() const;

        /// \brief Set the color
        ///
        /// This sets the drawable-specific color to be used as a base.
        /// If the bound material has a texture, the color will be used
        /// as a multiplier. The default color is white with an alpha of 1.
        ///
        /// \param color The color to set
        ///
        /// \return Reference to self
        ///
        Drawable& setColor(const Color& color);

        /// \brief Get the color
        ///
        /// \return Reference to the color
        ///
        /// \see setColor()
        ///
        const Color& getColor() const;

        /// \brief Get the local bounds
        ///
        /// This is the same as calling %getModel().%getMesh()->%getBounds().
        ///
        /// \return The local bounds
        ///
        const std::pair<glm::vec3, glm::vec3>& getLocalBounds() const;

        /// \brief Get the global bounds
        ///
        /// This will apply the current transformation to the local bounds and
        /// then return them.
        ///
        /// \return The global bounds
        ///
        std::pair<glm::vec3, glm::vec3> getGlobalBounds() const;

        /// \brief Set flags
        ///
        /// \param flags The flags to set
        ///
        /// \return Reference to self
        ///
        /// \see Flag
        ///
        Drawable& setFlags(const uint32 flags);

        /// \brief Check if this drawable has a flag
        ///
        /// \param flag The flag to check
        ///
        /// \return True if this drawable has the flag
        ///
        /// \see Flag
        ///
        bool hasFlag(const uint32 flag) const;

        /// \brief Get the current shader
        ///
        /// \return Reference to the shader
        ///
        ShaderProgram& getShader() const;

        /// \brief Set an override shader
        ///
        /// \param shader The override shader
        ///
        /// \see removeOverrideShader()
        ///
        void setOverrideShader(ShaderProgram& shader);

        /// \brief Remove the override shader if one is bound
        ///
        /// \see setOverrideShader()
        /// 
        void removeOverrideShader();

        /// \brief Check if using an override shader
        ///
        /// \return True if using an override shader
        ///
        bool hasOverrideShader() const;

        /// \copydoc Material::getAttributes()
        ///
        uint64 getAttributes() const;

        /// \brief Check if this drawable is translucent
        ///
        /// \return True if translucent
        ///
        bool hasAlpha() const;

        /// \brief Check is culling has been turned on for this drawable
        ///
        /// \return True if culling is enabled
        ///
        bool isCulled() const;

    protected:

        /// \copydoc Component::receiveMessage()
        ///
        virtual Message::Result receiveMessage(const Message& message) override;

    protected:

        uint64 m_attributes;                            ///< Attribute flags

    private:

        static std::string getShaderPreprocessorDef(const uint64 attributes);


        Color m_color;                                      ///< Color specific to this drawable
        WeakReference<const Mesh> m_mesh;                   ///< The bound mesh
        WeakReference<const Material> m_material;           ///< The bound material
        mutable WeakReference<ShaderProgram> m_shader;      ///< The bound shader (override)
        std::unique_ptr<detail::CullerComponent> m_culler;  ///< Culler
        Renderer& m_rendererRef;                            ///< Reference to the renderer
        const RenderPass::Pass m_pass;                      ///< The render pass type
        const uint32 m_weight;                              ///< Render pass weight
        uint32 m_flags;                                     ///< Property flags
        uint8 m_renderGroup;                                ///< The render group
    };
}

/// \class jop::Drawable
/// \ingroup graphics

#endif