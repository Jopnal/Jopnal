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

#ifndef JOP_COMPONENT_HPP // Comment - Include guard format: JOP_FILENAME_FILEEXTENSION
#define JOP_COMPONENT_HPP

// Headers
#include <Jopnal\Header.hpp>
//////////////////////////////////////////////



namespace jop
{
	class Component
	{
	public:

		virtual ~Component();// Virtual destructor

		virtual void sendMessage(std::string* message, void* ptr);// Send message
		virtual void listenMessage(std::string* message, void* ptr);//Listen to message

		virtual void update(double timeStep);
		virtual int getID() { return m_ID; };
		virtual int setID(int ID) { m_ID = ID; }
		bool isActive() const; // Check if active - false means it will be deleted

	private:
        int m_ID; // Unique component identifier - to seperate components of the same type (typeid)
	};

	class TestComponent : public Component
	{
	public:
        TestComponent(int x) { m_ID = x; };
		// Base functions for component
		bool IsWorking();
		int GetValues(){ return x; };
	private:
		int m_ID; // Unique component identifier - to seperate components of the same type (typeid)
		int x = 2;	
	};
}

#endif