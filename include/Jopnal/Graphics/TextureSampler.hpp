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

#ifndef JOP_TEXTURESAMPLER_HPP
#define JOP_TEXTURESAMPLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API TextureSampler
    {
    private:

        JOP_DISALLOW_COPY(TextureSampler);

    public:

        /// The filtering mode
        ///
        enum Filter
        {
            None,
            Bilinear,
            Trilinear,
            Anisotropic
        };

        /// The repeat mode
        ///
        enum Repeat
        {
            Basic,
            Mirrored,
            ClampEdge,
            ClampBorder
        };

    public:

        /// \brief Constructor
        /// 
        /// This will create a sampler object but not modify its values
        ///
        TextureSampler();

        /// \brief Move constructor
        ///
        TextureSampler(TextureSampler&& other);

        /// \brief Move assignment operator
        ///
        TextureSampler& operator =(TextureSampler&& other);

        /// \brief Destructor
        ///
        ~TextureSampler();


        /// \brief Bind this sampler
        ///
        /// \param textureUnit The texture unit to bind this sample into
        ///
        void bind(const unsigned int textureUnit) const;

        /// \brief Delete and recreate this sampler, clearing all the settings
        ///
        void reset();

        /// \brief Set the filtering mode
        ///
        /// \param mode The filtering mode
        /// \param param Possible anisotropic filtering level
        ///
        void setFilterMode(const Filter mode, const float param = 1.f);

        /// \brief Set the repeating mode
        ///
        /// \param repeat The repeating mode
        ///
        void setRepeatMode(const Repeat repeat);

        /// \brief Set the border color
        ///
        /// \param color The border color
        ///
        void setBorderColor(const Color& color);

        /// \brief Get the OpenGL handle
        ///
        /// \return The OpenGL handle
        ///
        unsigned int getHandle() const;

        /// \brief Get the maximum anisotropy level supported by the system
        ///
        /// \return The maximum anisotropy level. Zero if not supported
        ///
        static float getMaxAnisotropy();

        /// \brief Get the default sampler
        ///
        static std::shared_ptr<const TextureSampler> getDefaultSampler();

    private:

        unsigned int m_sampler; ///< The OpenGL sampler handle

    };
}

#endif

/// \class TextureSampler
/// \ingroup graphics