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

#ifndef JOP_RESPURCEMANAGER_H
#define JOP_RESPURCEMANAGER_H

// Headers
#include <Jopnal/Header.hpp>
#include <unordered_map>
#include <Jopnal/core/Resource.h>
//#include <Jopnal/Core/FileLoader.h>

//////////////////////////////////////////////

namespace jop
{
	class ResourceManager
	{
	public:
		ResourceManager()
		{

		}
		~ResourceManager()
		{
			m_resources.clear();
		}

		Resource* getResource(std::string name);
		Resource* loadResource(std::string path, std::string name);
		bool unloadResource(std::string name);
	private:

		std::unordered_map < std::string, Resource* > m_resources;
	};
}

#endif