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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/MainRenderTarget.hpp>

    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Window/Window.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    MainRenderTarget::MainRenderTarget(const Window& window)
        : RenderTexture (),
          Subsystem     (0),
          m_maxResScale (1.f),
          m_resScale    (1.f),
          m_windowRef   (window)
    {
        const glm::uvec2 scaledRes(SettingManager::get<float>("engine@Graphics|MainRenderTarget|fResolutionScale", 1.f) * glm::vec2(window.getSize()));

        const bool hdr = SettingManager::get<bool>("engine@Graphics|MainRenderTarget|bHDR", !gl::isES() || JOP_CHECK_EGL_EXTENSION(GL_EXT_color_buffer_half_float));

        using CA = Texture::Format;

        setSize(scaledRes);
        addTextureAttachment(Slot::Color0, hdr ? CA::RGBA_F_16 : CA::RGBA_UB_8);
        addRenderbufferAttachment(Slot::DepthStencil, CA::DepthStencil_UI_24_B_8);

        getTextureAttachment(Slot::Color0)->setFilterMode(TextureSampler::Filter::Bilinear).setRepeatMode(TextureSampler::Repeat::ClampEdge);

        setClearColor(Color(SettingManager::get<std::string>("engine@Graphics|MainRenderTarget|sClearColor", "000000FF")));
    }

    //////////////////////////////////////////////

    void MainRenderTarget::preUpdate(const float)
    {
        clear(AllBit);
    }
}