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

#ifndef JOP_FILELOADER_HPP
#define JOP_FILELOADER_HPP

// Headers
#include <Jopnal/Jopnal.hpp>
#include <string>

/////////////////////////////////////////////


namespace jop
{
	class FileLoader
	{
	public:

		/// \brief Initializes PhysicsFs filesystem
		///
		/// \param Unix-like systems such as Linux needs to pass argv[0] from main()
		///
		/// otherwise can be set to null
		///
		static void init(const char * argv);

		/// \brief Deinitializes PhysicsFs filesystem
		///
		/// releases used memory
		///
		static void deinit();

		/// \brief getSize returns size needed for data arrays buffer
		///
		/// \param Name or path for wanted resource
		///
		static long long getSize(const char* fileName);

		/// \brief Reads data from file into buffer
		///
		/// \param pointer to premade data array
		///
		/// name or path for wanted resource
		///
		/// size needed for data arrays buffer
		/// 
		static void read(void* data, const char* fileName, int size);

	};
}
#endif

/// \class FileLoader
/// \ingroup core