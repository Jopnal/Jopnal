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

//headers

#include "Jopnal\Header.hpp"
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();
	typedef std::vector<Object> ObjectList;

	virtual bool checkobject();
	virtual void addObject(Object* ExistingObject); //create object from existing object class
	virtual void deleteObject(Object* ExistingObject); //sets created object to remove pile
	virtual void deleteUnNeededObjects(); //deletes objects from pile
	virtual void deleteAllobjects(Object* ExistingObject);

	virtual void update(float deltaTime) = 0;

private:

	ObjectList m_Objects;
	ObjectList m_ObjectsToDelete;

};

Scene::Scene()
{
}

Scene::~Scene()
{
}