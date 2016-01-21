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

// Headers
#include <Jopnal/Header.hpp>
#include <vector>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
	class Object;

	/// \brief scene class
	///
	class Scene 
	{
	public:

		/// \brief Constructor for scene class
		///
		/// \param name String holding unique name for scene
		///
		Scene(const std::string& name);

		/// \brief Destructor for scene class
		///
		virtual ~Scene();

		/// \brief Container holding Objects
		///
		typedef std::vector<std::unique_ptr<Object>> ObjectList;

		/// \brief method for checking if object with 'ID' exists
		///
		/// \param ID unique Object identifier  
		///
		bool hasObject(const std::string& ID) const;

		/// \brief method that creates object
		///
		/// \param ID unique Object identifier 
		///
		Object& createObject(const std::string& ID);

		/// \brief method for deleting object 
		///
		/// \param ID unique Object identifier 
		///
		void deleteObject(const std::string& ID); 

		/// \brief method for clearing m_objects 
		///
		void clearObjects(); 

		/// \brief update method for scene
		///
		/// \param deltaTime double holding delta time
		///
		void updateBase(const double deltaTime);

		/// \brief update method for dixed scene
		///
		/// \param deltaTime double holding delta time
		///
		void fixedUpdateBase(const double deltaTime);

		/// \brief method for drawing
		///
		void drawBase();
		
		/// \brief method for pre updating
		///
		/// \param deltaTime double hodling delta time
		///
		virtual void preUpdate(const double deltaTime);

		/// \brief method for doing post update stuff
		///
		/// \param deltaTime double hodling delta time
		///
		virtual void postUpdate(const double deltaTime);

		/// \brief method for pre fixed updating
		///
		/// \param deltaTime double hodling delta time
		///
		virtual void preFixedUpdate(const double timeStep);

		/// \brief method for doing post fixed updating stuff
		///
		/// \param deltaTime double hodling delta time
		///
		virtual void postFixedUpdate(const double timeStep);
		
		/// \brief method for pre drawign 
		///
		virtual void preDraw();

		/// \brief method for post drawing
		///
		virtual void postDraw();

	private:

		ObjectList m_objects; ///< container holding objects

		std::string m_name; ///< string holdign unique scene name
	};
}