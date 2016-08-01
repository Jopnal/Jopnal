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

///////////////////////////////////////////

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/LightSource.hpp>

    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/Drawable.hpp>
    #include <Jopnal/Graphics/Texture/Texture.hpp>
    #include <Jopnal/Graphics/Mesh/Mesh.hpp>
    #include <Jopnal/Graphics/Renderer.hpp>
    #include <Jopnal/Graphics/ShaderAssembler.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <glm/mat4x4.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>

#endif

#include <Jopnal/Resources/Resources.hpp>

///////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(LightSource)

        JOP_BIND_MEMBER_COMMAND((LightSource& (LightSource::*)(const LightSource::Intensity, const Color&))&LightSource::setIntensity, "setIntensity");
        JOP_BIND_MEMBER_COMMAND((LightSource& (LightSource::*)(const float, const float, const float))&LightSource::setAttenuation, "setAttenuation");
        JOP_BIND_MEMBER_COMMAND(&LightSource::setCutoff, "setCutoff");

    JOP_END_COMMAND_HANDLER(LightSource)
}

namespace
{
    const std::array<jop::Color, 3>& getDefaultIntensities()
    {
        static const std::array<jop::Color, 3> intensities =
        {
            jop::Color(jop::SettingManager::get<std::string>("engine@Graphics|Shading|Light|sDefaultAmbient", "000000FF")),
            jop::Color(jop::SettingManager::get<std::string>("engine@Graphics|Shading|Light|sDefaultDiffuse", "FFFFFFFF")),
            jop::Color(jop::SettingManager::get<std::string>("engine@Graphics|Shading|Light|sDefaultSpecular", "FFFFFFFF"))
        };

        return intensities;
    }
}

namespace jop
{
    LightSource::LightSource(Object& object, Renderer& renderer, const Type type)
        : Component             (object, 0),
          m_lightSpaceMatrices  (),
          m_type                (type),
          m_intensities         (getDefaultIntensities()),
          m_attenuation         (1.f, 1.f, 2.f),
          m_cutoff              (0.17f, 0.17f), // ~10 degrees
          m_rendererRef         (renderer),
          m_renderMask          (1),
          m_shadowMap           ()
    {
        renderer.bind(this);
    }

    LightSource::LightSource(const LightSource& other, Object& newObj)
        : Component             (other, newObj),
          m_lightSpaceMatrices  (),
          m_type                (other.m_type),
          m_intensities         (other.m_intensities),
          m_attenuation         (other.m_attenuation),
          m_cutoff              (other.m_cutoff),
          m_rendererRef         (other.m_rendererRef),
          m_renderMask          (other.m_renderMask),
          m_shadowMap           ()
    {
        setCastShadows(other.castsShadows());
        m_rendererRef.bind(this);
    }

    LightSource::~LightSource()
    {
        m_rendererRef.unbind(this);
    }

    //////////////////////////////////////////////

    void LightSource::setRenderMask(const uint32 mask)
    {
        m_renderMask = mask;
    }

    //////////////////////////////////////////////

    uint32 LightSource::getRenderMask() const
    {
        return m_renderMask;
    }

    ///////////////////////////////////////////

    LightSource::Type LightSource::getType() const 
    {
        return m_type;
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setCastShadows(const bool castShadows)
    {
        if (castsShadows() != castShadows)
        {
            if (castShadows)
            {
            #ifndef JOP_GEOMETRY_SHADERS

                if (m_type == Type::Point)
                    return *this;

            #endif

                static const unsigned int mapSize = SettingManager::get<unsigned int>("engine@Graphics|Shading|uShadowMapResolution", 512);
                
                using CA = RenderTexture::ColorAttachment;

                m_shadowMap.addColorAttachment(RenderTexture::ColorAttachmentSlot::_1, m_type == Type::Point ? CA::DepthCube16 : CA::Depth2D16, glm::uvec2(mapSize));

                m_lightSpaceMatrices.resize(m_type == Type::Point ? 6 : 1);
            }
            else
            {
                m_shadowMap.destroy(true, true);
                m_lightSpaceMatrices.clear();
            }
        }

        return *this;
    }

    //////////////////////////////////////////////

    bool LightSource::castsShadows() const
    {
        return m_shadowMap.isValid();
    }

    //////////////////////////////////////////////

    const glm::mat4& LightSource::getLightspaceMatrix(const DepthFace face) const
    {
        if (!castsShadows())
        {
            static const glm::mat4 dummy;
            return dummy;
        }

        return m_lightSpaceMatrices[static_cast<int>(face)];
    }

    //////////////////////////////////////////////

    bool LightSource::drawShadowMap() const
    {
        if (!castsShadows())
            return false;

        static WeakReference<ShaderProgram> dirSpotShader, pointShader;

        // Directional/spot light recorder
        if (dirSpotShader.expired())
        {
            dirSpotShader = static_ref_cast<ShaderProgram>(ResourceManager::getEmpty<ShaderProgram>("jop_depth_record_shader").getReference());
            dirSpotShader->setPersistence(0);

            JOP_ASSERT_EVAL(dirSpotShader->load("", Shader::Type::Vertex, std::string(reinterpret_cast<const char*>(jopr::depthRecordShaderVert), sizeof(jopr::depthRecordShaderVert)),
                Shader::Type::Fragment, std::string(reinterpret_cast<const char*>(jopr::depthRecordShaderFrag), sizeof(jopr::depthRecordShaderFrag))),
                "Failed to compile depth record shader!");
        }

        // Point light recorder
        if (pointShader.expired())
        {
            pointShader = static_ref_cast<ShaderProgram>(ResourceManager::getEmpty<ShaderProgram>("jop_depth_record_shader_point").getReference());
            pointShader->setPersistence(0);

            JOP_ASSERT_EVAL(pointShader->load("", Shader::Type::Vertex, std::string(reinterpret_cast<const char*>(jopr::depthRecordShaderPointVert), sizeof(jopr::depthRecordShaderPointVert)),
                Shader::Type::Geometry, std::string(reinterpret_cast<const char*>(jopr::depthRecordShaderPointGeom), sizeof(jopr::depthRecordShaderPointGeom)),
                Shader::Type::Fragment, std::string(reinterpret_cast<const char*>(jopr::depthRecordShaderPointFrag), sizeof(jopr::depthRecordShaderPointFrag))),
                "Failed to compile point depth record shader!");
        }

        auto& shdr = m_type == Type::Point ? *pointShader : *dirSpotShader;

        if (!m_shadowMap.bind() || !shdr.bind())
            return false;

        m_shadowMap.clear(RenderTarget::DepthBit);

        if (getType() == Type::Point)
        {
            auto pos = getObject()->getGlobalPosition();

            const float range = getRange() * 10.f;

            const glm::mat4 proj = glm::perspective(glm::half_pi<float>(), 1.f, 0.1f, range);

            makeCubemapMatrices(proj, pos, m_lightSpaceMatrices);

            shdr.setUniform("u_PVMatrices", glm::value_ptr(m_lightSpaceMatrices[0]), 6);
            shdr.setUniform("u_FarClippingPlane", range);
            shdr.setUniform("u_LightPosition", pos);
        }
        else
        {
            // Use the object's scale to construct the frustum
            auto scl = getObject()->getLocalScale();

            // We need scale at 1 to avoid messing up the view transform
            glm::mat4 trans(getObject()->getInverseTransform().getMatrix());
            trans[0] = glm::normalize(trans[0]);
            trans[1] = glm::normalize(trans[1]);
            trans[2] = glm::normalize(trans[2]);

            if (getType() == Type::Directional)
            {
                m_lightSpaceMatrices[0] = glm::ortho(scl.x * -0.5f, scl.x * 0.5f, scl.y * -0.5f, scl.y * 0.5f, 0.f, scl.z) * trans;
                shdr.setUniform("u_PVMatrix", m_lightSpaceMatrices[0]);
            }
            else
            {
                auto s = glm::vec2(m_shadowMap.getSize());
                m_lightSpaceMatrices[0] = glm::perspective(getCutoff().y * 2.f, s.x / s.y, 0.5f, getRange()) * trans;

                shdr.setUniform("u_PVMatrix", m_lightSpaceMatrices[0]);
            }
        }

        bool drawn = false;

        for (auto d : drawables)
        {
            if (!d->isActive() || !d->castShadows() || (getRenderMask() & (1 << d->getRenderGroup())) == 0 || !d->lightTouches(*this))
                continue;

            drawn = true;

            auto& mesh = *d->getModel().getMesh();
            mesh.getVertexBuffer().bind();

            shdr.setAttribute("a_Position", 0, GL_FLOAT, 3, mesh.getVertexSize(), mesh.getVertexOffset(Mesh::Position));
            shdr.setUniform("u_MMatrix", d->getObject()->getTransform().getMatrix());

            if (mesh.getElementAmount())
            {
                mesh.getIndexBuffer().bind();
                glCheck(glDrawElements(GL_TRIANGLES, mesh.getElementAmount(), mesh.getElementEnum(), (void*)0));
            }
            else
            {
                glCheck(glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexAmount()));
            }
        }

        return drawn;
    }

    //////////////////////////////////////////////

    const Texture* LightSource::getShadowMap() const
    {
        return m_shadowMap.getDepthTexture();
    }

    ///////////////////////////////////////////

    LightSource& LightSource::setIntensity(const Intensity intensity, const Color& color)
    {
        m_intensities[static_cast<int>(intensity)] = color;
        return *this;
    }

    ///////////////////////////////////////////

    LightSource& LightSource::setIntensity(const Color& ambient, const Color& diffuse, const Color& specular)
    {
        return setIntensity(Intensity::Ambient, ambient)
              .setIntensity(Intensity::Diffuse, diffuse)
              .setIntensity(Intensity::Specular, specular);
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setIntensity(const Color& intensity)
    {
        return setIntensity(Intensity::Ambient, intensity)
              .setIntensity(Intensity::Diffuse, intensity)
              .setIntensity(Intensity::Specular, intensity);
    }

    ///////////////////////////////////////////

    const Color& LightSource::getIntensity(const Intensity intensity) const
    {
        return m_intensities[static_cast<int>(intensity)];
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setAttenuation(const Attenuation attenuation, const float value)
    {
        m_attenuation[static_cast<int>(attenuation)] = value;
        return *this;
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setAttenuation(const float constant, const float linear, const float quadratic)
    {
        m_attenuation = glm::vec3(constant, linear, quadratic);
        return *this;
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setAttenuation(const float range)
    {
        return setAttenuation(1.0f, 4.5f / std::max(FLT_MIN, range), 75.0f / std::max(FLT_MIN, range * range));
    }

    //////////////////////////////////////////////

    float LightSource::getAttenuation(const Attenuation attenuation) const
    {
        return m_attenuation[static_cast<int>(attenuation)];
    }

    //////////////////////////////////////////////

    glm::vec3 LightSource::getAttenuationVec() const
    {
        return glm::vec3(m_attenuation);
    }

    //////////////////////////////////////////////

    float LightSource::getRange() const
    {
        if (m_type == Type::Directional)
            return FLT_MAX;

        const float max = std::max(
        {
            m_intensities[0].colors.r, m_intensities[0].colors.g, m_intensities[0].colors.b,
            m_intensities[1].colors.r, m_intensities[1].colors.g, m_intensities[1].colors.b,
            m_intensities[2].colors.r, m_intensities[2].colors.g, m_intensities[2].colors.b
        });

        const auto& att = m_attenuation;

        return (att.y + std::sqrtf(att.y * att.y - 4 * att.z * (att.x - (255.f / 4.5f) * max))) / (2 * att.z);
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setCutoff(const float inner, const float outer)
    {
        m_cutoff.x = inner; m_cutoff.y = outer;
        return *this;
    }

    //////////////////////////////////////////////

    const glm::vec2& LightSource::getCutoff() const
    {
        return m_cutoff;
    }

    //////////////////////////////////////////////

    unsigned int LightSource::getMaximumLights(const Type type)
    {
        static const unsigned int maxLights[] =
        {
            SettingManager::get<unsigned int>("engine@Graphics|Shading|uMaxPointLights", 8),
            SettingManager::get<unsigned int>("engine@Graphics|Shading|uMaxDirectionalLights", 2),
            SettingManager::get<unsigned int>("engine@Graphics|Shading|uMaxSpotLights", 2)
        };

        return maxLights[static_cast<int>(type)];
    }

    //////////////////////////////////////////////

    void LightSource::makeCubemapMatrices(const glm::mat4& projection, const glm::vec3& position, std::vector<glm::mat4>& viewMats)
    {
        viewMats[0] = projection * glm::lookAt(position, position + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)); // Right
        viewMats[1] = projection * glm::lookAt(position, position + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)); // Left
        viewMats[2] = projection * glm::lookAt(position, position + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)); // Top
        viewMats[3] = projection * glm::lookAt(position, position + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)); // Bottom
        viewMats[4] = projection * glm::lookAt(position, position + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)); // Back
        viewMats[5] = projection * glm::lookAt(position, position + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)); // Front
    }

    //////////////////////////////////////////////

    Message::Result LightSource::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(LightSource, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }

    //////////////////////////////////////////////


    LightContainer::LightContainer()
        : m_container()
    {
        clear();
    }

    //////////////////////////////////////////////

    bool LightContainer::empty() const
    {
        for (auto& i : m_container)
        {
            if (!i.empty())
                return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    void LightContainer::clear()
    {
        for (auto& i : m_container)
            i.clear();

        (*this)[LightSource::Type::Point].reserve(LightSource::getMaximumLights(LightSource::Type::Point));
        (*this)[LightSource::Type::Directional].reserve(LightSource::getMaximumLights(LightSource::Type::Directional));
        (*this)[LightSource::Type::Spot].reserve(LightSource::getMaximumLights(LightSource::Type::Spot));
    }

    //////////////////////////////////////////////

    void LightContainer::sendToShader(ShaderProgram& shader, const Drawable& drawable) const
    {
        const bool receiveLights = drawable.hasFlag(Drawable::ReceiveLights);
        const bool receiveShadows = drawable.hasFlag(Drawable::ReceiveShadows);

        shader.setUniform("u_ReceiveLights", receiveLights);

        if (!receiveLights || empty())
            return;

        shader.setUniform("u_ReceiveShadows", receiveShadows);

        typedef LightSource LS;

        static const unsigned int pointShadowStartUnit = static_cast<unsigned int>(Material::Map::Last);
        unsigned int currentPointShadowUnit = pointShadowStartUnit;

        static const unsigned int dirSpotShadowStartUnit = pointShadowStartUnit + LS::getMaximumLights(LS::Type::Point);
        unsigned int currentDirSpotShadowUnit = dirSpotShadowStartUnit;
        
        static std::array<std::vector<std::vector<std::string>>, 3> strCache;
        static bool cacheInit = false;

        if (!cacheInit)
        {
            strCache[0].resize(LS::getMaximumLights(LS::Type::Point));
            strCache[1].resize(LS::getMaximumLights(LS::Type::Spot));
            strCache[2].resize(LS::getMaximumLights(LS::Type::Directional));

            for (std::size_t i = 0; i < LS::getMaximumLights(LS::Type::Point); ++i)
            {
                auto& cache = strCache[static_cast<int>(LS::Type::Point)][i];
                cache.reserve(8);
                const std::string indexed = "u_PointLights[" + std::to_string(i) + "].";

                /* 0 */ cache.emplace_back(indexed + "position");

                /* 1 */ cache.emplace_back(indexed + "ambient");
                /* 2 */ cache.emplace_back(indexed + "diffuse");
                /* 3 */ cache.emplace_back(indexed + "specular");

                /* 4 */ cache.emplace_back(indexed + "attenuation");

                /* 5 */ cache.emplace_back(indexed + "castShadow");
                /* 6 */ cache.emplace_back("u_PointLightShadowMaps[" + std::to_string(i) + "]");
                /* 7 */ cache.emplace_back(indexed + "farPlane");
            }

            for (std::size_t i = 0; i < LS::getMaximumLights(LS::Type::Spot); ++i)
            {
                auto& cache = strCache[static_cast<int>(LS::Type::Spot)][i];
                cache.reserve(10);
                const std::string indexed = "u_SpotLights[" + std::to_string(i) + "].";

                /* 0 */ cache.emplace_back(indexed + "position");

                /* 1 */ cache.emplace_back(indexed + "direction");

                /* 2 */ cache.emplace_back(indexed + "ambient");
                /* 3 */ cache.emplace_back(indexed + "diffuse");
                /* 4 */ cache.emplace_back(indexed + "specular");

                /* 5 */ cache.emplace_back(indexed + "attenuation");

                /* 6 */ cache.emplace_back(indexed + "cutoff");

                /* 7 */ cache.emplace_back(indexed + "castShadow");
                /* 8 */ cache.emplace_back(indexed + "lsMatrix");
                /* 9 */ cache.emplace_back("u_SpotLightShadowMaps[" + std::to_string(i) + "]");
            }

            for (std::size_t i = 0; i < LS::getMaximumLights(LS::Type::Directional); ++i)
            {
                auto& cache = strCache[static_cast<int>(LS::Type::Directional)][i];
                cache.reserve(7);
                const std::string indexed = "u_DirectionalLights[" + std::to_string(i) + "].";

                /* 0 */ cache.emplace_back(indexed + "direction");

                /* 1 */ cache.emplace_back(indexed + "ambient");
                /* 2 */ cache.emplace_back(indexed + "diffuse");
                /* 3 */ cache.emplace_back(indexed + "specular");

                /* 4 */ cache.emplace_back(indexed + "castShadow");
                /* 5 */ cache.emplace_back(indexed + "lsMatrix");
                /* 6 */ cache.emplace_back("u_DirectionalLightShadowMaps[" + std::to_string(i) + "]");
            }

            cacheInit = true;
        }

        // Point lights
        {
            auto& points = (*this)[LS::Type::Point];
            shader.setUniform("u_NumPointLights", static_cast<int>(points.size()));

            for (std::size_t i = 0; i < points.size(); ++i)
            {
                auto& li = *points[i];
                auto& cache = strCache[static_cast<int>(LS::Type::Point)][i];

                // Position
                shader.setUniform(cache[0], li.getObject()->getGlobalPosition());

                // Intensity
                shader.setUniform(cache[1], li.getIntensity(LS::Intensity::Ambient).colors);
                shader.setUniform(cache[2], li.getIntensity(LS::Intensity::Diffuse).colors);
                shader.setUniform(cache[3], li.getIntensity(LS::Intensity::Specular).colors);

                // Attenuation
                shader.setUniform(cache[4], li.getAttenuationVec());

                // Shadow map
                if (receiveShadows)
                {
                    shader.setUniform(cache[5], li.castsShadows());

                    if (li.castsShadows())
                    {
                        shader.setUniform(cache[6], *li.getShadowMap(), currentPointShadowUnit++);
                        shader.setUniform(cache[7], li.getRange() * 10.f);
                    }
                }
            }
        }

        // Spot lights
        {
            auto& spots = (*this)[LS::Type::Spot];
            shader.setUniform("u_NumSpotLights", static_cast<int>(spots.size()));

            for (std::size_t i = 0; i < spots.size(); ++i)
            {
                auto& li = *spots[i];
                auto& cache = strCache[static_cast<int>(LS::Type::Spot)][i];

                // Position
                shader.setUniform(cache[0], li.getObject()->getGlobalPosition());

                // Direction
                shader.setUniform(cache[1], li.getObject()->getGlobalFront());

                // Intensity
                shader.setUniform(cache[2], li.getIntensity(LS::Intensity::Ambient).colors);
                shader.setUniform(cache[3], li.getIntensity(LS::Intensity::Diffuse).colors);
                shader.setUniform(cache[4], li.getIntensity(LS::Intensity::Specular).colors);

                // Attenuation
                shader.setUniform(cache[5], li.getAttenuationVec());

                // Cutoff
                shader.setUniform(cache[6], glm::vec2(std::cos(li.getCutoff().x), std::cos(li.getCutoff().y)));

                // Shadow map
                if (receiveShadows)
                {
                    shader.setUniform(cache[7], li.castsShadows());

                    if (li.castsShadows())
                    {
                        shader.setUniform(cache[8], li.getLightspaceMatrix());
                        shader.setUniform(cache[9], *li.getShadowMap(), currentDirSpotShadowUnit++);
                    }
                }
            }
        }

        // Directional lights
        {
            auto& dirs = (*this)[LS::Type::Directional];
            shader.setUniform("u_NumDirectionalLights", static_cast<int>(dirs.size()));

            for (std::size_t i = 0; i < dirs.size(); ++i)
            {
                auto& li = *dirs[i];
                auto& cache = strCache[static_cast<int>(LS::Type::Directional)][i];

                // Direction
                shader.setUniform(cache[0], li.getObject()->getGlobalFront());
                
                // Intensity
                shader.setUniform(cache[1], li.getIntensity(LS::Intensity::Ambient).colors);
                shader.setUniform(cache[2], li.getIntensity(LS::Intensity::Diffuse).colors);
                shader.setUniform(cache[3], li.getIntensity(LS::Intensity::Specular).colors);

                // Shadow map
                if (receiveShadows)
                {
                    shader.setUniform(cache[4], li.castsShadows());

                    if (li.castsShadows())
                    {
                        shader.setUniform(cache[5], li.getLightspaceMatrix());
                        shader.setUniform(cache[6], *li.getShadowMap(), currentDirSpotShadowUnit++);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////

    LightContainer::ContainerType& LightContainer::operator[](const LightSource::Type type)
    {
        return m_container[static_cast<int>(type)];
    }

    //////////////////////////////////////////////

    const LightContainer::ContainerType& LightContainer::operator[](const LightSource::Type type) const
    {
        return m_container[static_cast<int>(type)];
    }
}