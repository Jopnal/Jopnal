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

#ifndef JOP_RESPURCE_H
#define JOP_RESPURCE_H

// Headers
#include <jopnal/Header.hpp>
#include <string>
//////////////////////////////////////////////

namespace jop
{
	//this is the base class for all resources that are loaded from files:
	//sprites, sounds, scene data...
	//processed for use
	class Resource
	{
	public:
		virtual bool load(std::string path) = 0;
	private:
	};

	//////////////////////////////////////////////

	class TextResource : public jop::Resource
	{
	public:
		bool load(std::string path)override
		{
			const char * charPath = path.c_str();
			long long size = FileLoader::getSize(charPath);
			char* resBuf = new char[size+1];
			FileLoader::read(resBuf, charPath, size);
			resBuf[size] = '\0';
			text = std::string(resBuf);

			return true;
		}
		void write(){ JOP_DEBUG_INFO(text); }
	private:
		std::string text;
	};
}

#endif