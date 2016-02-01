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
    private:

        JOP_DISALLOW_COPY_MOVE(Layer);

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
        /// The default version of this function simply iterates through the
        /// local & bound draw lists and calls draw() on the drawables.
        ///
        virtual void draw() override;


        MessageResult sendMessage(const std::string& message, PtrWrapper returnWrap);

        MessageResult sendMessage(const Message& message);


        /// \brief Add a new drawable component to the draw list
        ///
        /// \param drawable Reference to the drawable to be added
        ///
        void addDrawable(Drawable& drawable);

        /// \brief Bind a layer's draw list into this layer
        ///
        /// \brief layer Reference to the layer to be bound
        ///
        void bindOtherLayer(Layer& layer);

        /// \brief Set the camera
        ///
        /// \param camera Reference to the camera to be set
        ///
        void setCamera(const Camera& camera);

        /// \brief Set a RenderTexture
        ///
        /// \param renderTexture Pointer to the render texture to be set. Pass nullptr to unbind the current one
        ///
        //void setRenderTexture(RenderTexture* renderTexture);


    protected:

        std::vector<std::weak_ptr<Drawable>> m_drawList;  ///< The local draw list
        std::vector<std::weak_ptr<Layer>> m_boundLayers;  ///< Bound layers
        std::weak_ptr<const Camera> m_camera;             ///< Bound camera
        std::weak_ptr<RenderTexture> m_renderTexture;     ///< Bound RenderTexture

    };
}

#endif

/// \class Layer
/// \ingroup graphics
///
/// #TODO Detailed description