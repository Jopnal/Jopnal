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

#include <Jopnal\Precompiled.hpp>

//////////////////////////////////////////////

void FileManager::init(const char * argv)
{
	if (!PHYSFS_isInit())
	{
		PHYSFS_init(argv);
		if (!PHYSFS_addToSearchPath("Jopnal", 0));
		else
		{
			std::cout << "\nError in PHYSFS_addToSearchPath()\n";
		}
		if (!PHYSFS_exists("PhysicsFs License.txt"));
		else
		{
			std::cout << "\nError in file reading check(PHYSFS_exists())\n";
		}
	}
	else
	{
		std::cout << "\nPhysicsFs is already initialized\n";
	}
	
}

//////////////////////////////////////////////

void FileManager::deinit()
{
	PHYSFS_deinit();
}

//////////////////////////////////////////////
//////////////////////////////////////////////



long FileManager::getSize(const char* fileName)
{
	if (!PHYSFS_exists(fileName))
	{
		if (!PHYSFS_openRead(fileName))
		{
			PHYSFS_file* inputFile = PHYSFS_openRead(fileName);
			PHYSFS_close(inputFile);
			return PHYSFS_fileLength(inputFile);
		}
		else
		{
			std::cout << "\nError in reading " << fileName << "file\n";
		}


	}
	else
	{
		std::cout << "\nError can't find " << fileName << "file\n";
	}

	return -1;
}

//////////////////////////////////////////////

void FileManager::read(void* data, const char* fileName,int size)
{
	PHYSFS_exists(fileName);
	if (!PHYSFS_openRead(fileName))
	{
	PHYSFS_file* inputFile = PHYSFS_openRead(fileName);	
	PHYSFS_read(inputFile, data, 1, size);
	PHYSFS_close(inputFile);
	}
	else
	{
		std::cout << "\nError in reading " << fileName << "file\n";
	}
	
}

//////////////////////////////////////////////

