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
	Resource* ResourceManager::getResource(std::string name)
	{
		auto it = m_resources.find(name);
		if (it == m_resources.end())
		{
			std::cout << "Error 404: Resource not found." << std::endl;
			return 0;
		}
		else
		{
			return it->second;
		}
	}
	Resource* ResourceManager::loadResource(std::string path, std::string name)
	{
		Resource* res;
		/*
		Resource* res = fileLManager.read(path);
		if(res != nullptr)
		{
			*/
			m_resources.insert(std::make_pair(name, res));
			return res;
			/*
		}
		*/
		std::cout << "Can't load resource \"" << name << "\"" << std::endl;
		return nullptr;
	}
	bool ResourceManager::unloadResource(std::string name)
	{
		auto it = m_resources.find(name);
		if (it == m_resources.end())
		{
			std::cout << "Error 404: Resource not found." << std::endl;
			return false;
		}
		else
		{
			//it->second->getSelfDestructButton().activate(time::seconds(10));
			m_resources.erase(it);
			return true;
		}
	}
}