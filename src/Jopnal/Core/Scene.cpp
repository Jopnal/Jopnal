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
#include <Jopnal/Precompiled.hpp>


/////////////////////////////////////////////

namespace jop
{

	Scene::Scene()
	{}

	Scene::~Scene()
	{}
	//////////////////////////////////////////////

	Object& Scene::createObject(const std::string& ID)
	{

		m_objects.emplace_back(std::make_unique<Object>());
		return *m_objects.back();
	}

	//////////////////////////////////////////////

	bool Scene::hasObject(const std::string& ID) const
	{
		for (auto i = m_objects.begin(); i != m_objects.end(); i++)
		{
			if ((*i)->getID() == ID)
			{
				return true;
			}
			else if ((*i) - getID() != ID)
			{
				return false;
			}
		}
	}

	//////////////////////////////////////////////

	void Scene::deleteObject(const std::string& ID)
	{
		for (auto itr = m_objects.begin(); itr != m_objects.end(); ++itr)
		{
			if ((*itr)->getID() == ID)
				m_objects.erase(itr);
		}
	}


	//////////////////////////////////////////////

	void Scene::clearObjects()
	{
		m_objects.clear();
	}
	//////////////////////////////////////////////

	void Scene::updateBase(const double deltaTime)
	{
		preUpdate(deltaTime);

		for (auto& i : m_objects)
			i->update(deltaTime);

		postUpdate(deltaTime);
	}

	//////////////////////////////////////////////

	void preUpdate(const double deltaTime)
	{
	}

	//////////////////////////////////////////////

	void postUpdate(const double deltaTime)
	{
	}

	//////////////////////////////////////////////

	void preFixedUpdate(const double)
	{
	}

	//////////////////////////////////////////////

	void postFixedUpdate(const double)
	{
	}

	//////////////////////////////////////////////

	void preDraw()
	{
	}

	//////////////////////////////////////////////

	void postDraw()
	{
	}

	//////////////////////////////////////////////
}