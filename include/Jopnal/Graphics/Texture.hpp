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

#ifndef JOP_TEXTURE_HPP
#define JOP_TEXTURE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////

namespace jop

{
	class Texture : public Resource
	{
	public:

        /// \brief Constructor
        ///
        Texture();

		/// \brief Destructor
		///
		~Texture();


		/// \brief Method for using Fileloader to load new resource from file
		///
		/// \param Name or path for wanted resource
		///
		bool load(const std::string& path);

        /// \brief Creates flat/empty texture
        ///
        /// \param Takes width height and bits and sorts them with '!' mark
        ///
        /// Example: !800!600!4
        ///
        bool loadEmpty(const std::string& xyBpp);

        ///
        ///
        bool loadFromPixels(const int x, const int y, const int bytesPerPixel, const unsigned char* pixels);


        void destroy();


        bool bind(const unsigned int texUnit = 0) const;

        static void unbind(const unsigned int texUnit = 0);


        /// \brief Returns image's width
        ///
        int getWidth() const;

        /// \brief Returns image's height
        ///
        int getHeight() const;

        /// \brief Returns image's bytes per pixel value
        ///
        int getDepth() const;

        unsigned int getHandle() const;

        static int getMaximumSize();

    private:

        bool checkDepthValid(const int depth) const;

        int m_width;
        int m_height;
        int m_bytesPerPixel;
        unsigned int m_texture;

    };
}

#endif

/// \class Texture
/// \ingroup graphics

/// stores and creates image data