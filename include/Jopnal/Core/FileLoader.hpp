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
#include <Jopnal/Core/SubSystem.hpp>
#include <string>
#include <vector>

/////////////////////////////////////////////


namespace jop
{
	class FileLoader : public Subsystem
	{
	public:

        /// \brief Constructor
        ///
		/// Initializes the PhysicsFs file system
		///
		/// \param argv The first element of the char* array passed from main()
		///
		FileLoader(const char* argv);

		/// \brief Destructor
        ///
        /// Deinitializes the PhysicsFs file system
		///
		~FileLoader() override;


		/// \brief Reads data from file into a buffer
		///
		/// \param pointer to premade data array
		/// 
		static bool read(const std::string& filePath, std::vector<unsigned char>& buffer);

	};
}
#endif

/// \class FileLoader
/// \ingroup core