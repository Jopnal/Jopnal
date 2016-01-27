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

#ifndef JOP_LAYER_HPP
#define JOP_LAYER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <memory>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class Drawable;
    class Camera;
    class RenderTexture;

    class JOP_API Layer : public Subsystem, public std::enable_shared_from_this<Layer>
    {
    public:
    
        /// \brief Constructor
        ///
        /// \param ID Identifier of this layer
        ///
        Layer(const std::string& ID);

        /// \brief Virtual destructor
        ///
        virtual ~Layer() override;


        /// \brief Draw function
        ///
        virtual void draw() override;

        /// \brief Add a new drawable component to the draw list
        ///
        /// \param drawable Reference to the drawable to be added
        ///
        void addDrawable(const Drawable& drawable);

        /// \brief Bind a layer's draw list into this layer
        ///
        /// \brief layer Reference to the layer to be bound
        ///
        void bindOtherLayer(const Layer& layer);


    protected:

        std::vector<std::weak_ptr<const Drawable>> m_drawList;
        std::vector<std::weak_ptr<const Layer>> m_boundLayers;
        std::weak_ptr<Camera> m_camera;
        std::weak_ptr<RenderTexture> m_renderTexture;

    };
}

#endif

/// \class Layer
/// \ingroup graphics
///
/// #TODO Detailed description