//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

//////////////////////////////////////////////

//Headers
#include <Jopnal\Precompiled.hpp>

//////////////////////////////////////////////

namespace jop
{
//////////////////////////////////////////////
    bool Object::checkComponent(int ID) const
    {
		for (int i = 0; i < components.size; i++)
		{
			if (components.at(i)->getID() == ID)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
    }
/////////////////////////////////////////////
    void Object::update(double timeStep)
    {
		for (int i = 0; i < components.size; i++)
		{
			components.at(i)->update(timeStep);
		}
    } 
/////////////////////////////////////////////
	Component Object::createComponent(int ID)
	{
		Component *c;
		c = new Component();
		c->setID(ID);
		components.push_back(c);
	}
/////////////////////////////////////////////
	void Object::sendMessage(std::string* message, void* ptr)
	{
		for (int i = 0; i < components.size; i++)
		{
			components.at(i)->listenMessage(message, ptr);
		}
	}

	Component Object::checkComponentType()
	{
		for (int i = 0; i < components.size; i++)
		{
			

			if (typeid(components.at(i)) == /*haluttu tyyppi*/)
			{
				//return component(s)
			}
			else
			{
				//component of given type not found
			}
		}
	}
}
/////////////////////////////////////////////
