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

#ifndef JOP_FONT_HPP
#define JOP_FONT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <memory>

#include <Jopnal/Graphics/stb/stb_truetype.h>
#include <Jopnal/MathInclude.hpp>

#include <unordered_map>

//////////////////////////////////////////////

struct stbtt_fontinfo;

namespace jop
{
    struct Bitmap
    {
        int width, height;
        unsigned char* data;
    };

	class Font
        :public Resource
	{
	public:
        bool load(const std::string& path)override;
        std::pair<glm::ivec2, glm::ivec2> getBounds(const int codepoint);
        int getKerning(const int codepoint1, const int codepoint2);
        unsigned char* getCodepointBitmap(const float scaleX, const float scaleY, const int codepoint, int* width, int* height);
        ~Font();
	private:
        std::unique_ptr<stbtt_fontinfo> m_info; //font info
        //std::unordered_map <int, std::pair<unsigned char*, glm::uvec2> > m_bitmaps; //codepoint, data, size
        std::unordered_map <int, Bitmap> m_bitmaps;
	};
}

#endif

