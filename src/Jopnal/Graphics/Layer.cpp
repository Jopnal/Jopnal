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
    Layer::Layer(const std::string& ID)
        : Subsystem                             (ID),
          std::enable_shared_from_this<Layer>   (),
          m_drawList                            (),
          m_boundLayers                         (),
          m_camera                              (),
          m_renderTexture                       ()
    {}

    Layer::~Layer()
    {

    }

    //////////////////////////////////////////////

    void Layer::draw()
    {}

    //////////////////////////////////////////////

    void Layer::addDrawable(const Drawable& drawable)
    {
        m_drawList.emplace_back(std::static_pointer_cast<Drawable const>(drawable.shared_from_this()));
    }

    //////////////////////////////////////////////

    void Layer::bindOtherLayer(const Layer& layer)
    {
        m_boundLayers.emplace_back(layer.shared_from_this());
    }
}