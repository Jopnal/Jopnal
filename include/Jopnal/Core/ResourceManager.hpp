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

#ifndef JOP_RESOURCEMANAGER_HPP
#define JOP_RESOURCEMANAGER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <unordered_map>


//////////////////////////////////////////////

namespace jop
{
	/// \forward declaration for Resource class
	///
	class Resource;

	class ResourceManager
	{
	public:

		/// \brief Default constructor
		///
		ResourceManager();

		/// \brief Default destructor
		///
		~ResourceManager();

		/// \brief Template function finds resource
		///
		/// if resource is not found it makes new one
		///
		/// \param Name or path for wanted resource
		///
		template<typename T>
        T* getResource(const std::string& path);

		/// \brief Deletes resource from memory
		///
		/// \param Name or path for wanted resource
		///
		bool unloadResource(const std::string& path);

		/// \brief Deletes all resources from memory
		///
		void unloadAll();
	private:
		std::unordered_map < std::string, std::unique_ptr<Resource>> m_resources;///< Container holds resources
	};
}

#endif

/// \class ResourceManager
/// \ingroup core