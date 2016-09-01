// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

#ifndef JOP_RANDOMIZER_HPP
#define JOP_RANDOMIZER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <random>
#include <type_traits>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Randomizer
    {
    public:

        /// \brief Default constructor
        ///
        /// Initializes the seed using std::random_device.
        ///
        Randomizer();

        /// \brief Constructor for initialization with a custom seed
        ///
        /// \param seed The seed to use
        ///
        Randomizer(const std::default_random_engine::result_type seed);


        /// \brief Returns a random value from given distribution
        ///
        /// \param start Starting point of distribution
        /// \param end Ending point of distribution
        ///
        template<typename T>
        T range(const T start, const T end) const;

        /// \copydoc range()
        ///
        template<typename T>
        T operator ()(const T start, const T end) const;

        /// \brief Get a random point within a circle
        ///
        /// \param radius Radius of the circle
        ///
        /// \return Random point inside a circle
        ///
        template<typename T>
        glm::vec2 insideCircle(const T radius) const;

        /// \brief Get a random point within a sphere
        ///
        /// \param radius Radius of the circle
        ///
        /// \return Random point inside a sphere
        ///
        template<typename T>
        glm::vec3 insideSphere(const T radius) const;

    private:

        mutable std::default_random_engine m_randomGenerator;   ///< The random engine
    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/Randomizer.inl>
}

/// \class jop::Randomizer
/// \ingroup utility
///
/// Class for picking random values from given distribution

#endif