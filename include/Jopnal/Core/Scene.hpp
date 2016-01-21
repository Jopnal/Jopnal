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

#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Object.hpp>
#include <vector>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		typedef std::vector<std::unique_ptr<Object>> ObjectList;

		bool hasObject(const std::string& ID) const;

		Object& createObject(const std::string& ID); //create object from existing object class
		void deleteObject(const std::string& ID); //sets created object to remove pile
		void clearObjects(); //clears m_objects vector

		void updateBase(const double deltaTime);

		
		virtual void preUpdate(const double deltaTime);

		virtual void postUpdate(const double deltaTime);

		virtual void preFixedUpdate(const double);

		virtual void postFixedUpdate(const double);

		virtual void preDraw();

		virtual void postDraw();

	private:

		ObjectList m_objects;


	};

}