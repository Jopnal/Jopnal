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
#include "Jopnal\Precompiled.hpp"
#include "Jopnal\Core\Scene.hpp"


/////////////////////////////////////////////


void Scene::addObject(Object* ExistingObject)
{
	
	m_Objects.push_back(Object);
	
}

bool Scene::checkObject()
{
	
}

void Scene::deleteObject(Object* ExistingObject)
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i].ptr() == ExistingObject)
		{
			m_ObjectsToDelete.push_back(ExistingObject);
			return;
		}
	}
}

void Scene::deleteUnNeededObjects()
{
	for (int i = 0; i < m_ObjectsToDelete.size(); i++)
	{
		for (int j = 0; j < m_Objects.size(); j++)
		{
			if (m_ObjectsToDelete[i] == m_Objects[j])
			{
				m_Objects.erase(m_Objects.begin() + j);
			}
		}

		m_ObjectsToDelete[i] = 0;
	}
	m_ObjectsToDelete.clear();
		
}

void Scene::deleteAllobjects(Object* ExistingObject)
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects.size > 0)
		{
			m_Objects.erase(m_Objects.size);
		}
		else
		{
			return;
		}
	}
}
