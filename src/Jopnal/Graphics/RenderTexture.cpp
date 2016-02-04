// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    RenderTexture::RenderTexture()
    {
        m_texture = NULL;
        m_frameBuffer = 0;
        m_depthBuffer = 0;
    }

    //////////////////////////////////////////////

    RenderTexture::~RenderTexture()
    {
        unBind();
        deInit();
    }

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void RenderTexture::get()
    {
        init(0, 0);
        bind();
    }
    
    //////////////////////////////////////////////

    void RenderTexture::deInit()
    {
  
        gl::DeleteRenderbuffers(1, &idBase);
        idBase = NULL;

        gl::DeleteRenderbuffers(1, &m_depthBuffer);
        m_depthBuffer = NULL;

        gl::DeleteFramebuffers(1, &m_frameBuffer);
        m_frameBuffer = NULL;

    }

    //////////////////////////////////////////////

    void RenderTexture::unBind()
    {
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
    }

    //////////////////////////////////////////////

    bool RenderTexture::init(int width,int height)
    {
        gl::GenFramebuffers(1, &m_frameBuffer);
        gl::BindFramebuffer(gl::FRAMEBUFFER, m_frameBuffer);

        gl::GenRenderbuffers(1, &idBase);
        gl::BindRenderbuffer(gl::RENDERBUFFER, idBase);
        gl::RenderbufferStorage(gl::RENDERBUFFER, gl::RGBA8,width,height);
        
        gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::RENDERBUFFER, idBase);

        gl::GenRenderbuffers(1, &m_depthBuffer);
        gl::BindRenderbuffer(gl::RENDERBUFFER, m_depthBuffer);
        gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT24, width, height);
    
        gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, m_depthBuffer);

        if (!gl::CheckFramebufferStatus(gl::FRAMEBUFFER))
        {
            JOP_DEBUG_ERROR("GPU does not support current FBO. Error in RenderTexture::render");
            return false;
        }
        else
            return true;
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind()
    {
        gl::BindFramebuffer(gl::FRAMEBUFFER, m_frameBuffer);
        return m_frameBuffer!=NULL;
    }

    //////////////////////////////////////////////

    std::weak_ptr<Texture> RenderTexture::get()
    {
        return m_texture;
    }
}