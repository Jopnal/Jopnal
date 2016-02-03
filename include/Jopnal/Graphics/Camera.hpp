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
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

#ifndef JOP_CAMERA_HPP
#define JOP_CAMERA_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/MathInclude.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Camera final : public Component
    {
    private:

        JOP_DISALLOW_MOVE(Camera);

        void operator =(const Camera&) = delete;

    public:

        typedef std::pair<float, float> ClippingPlanes;

        struct ProjectionData
        {
            struct
            {
                float sizeX;
                float sizeY;
            } orthographic;

            struct
            {
                float fov;
                float aspectRatio;
            } perspective;
        };

        enum class Projection
        {
            Orthographic,
            Perspective
        };

    public:
    
        /// \brief Constructor
        ///
        /// \param object The object this camera will be bound to
        /// \param mode The initial projection mode
        ///
        Camera(Object& object, const Projection mode);

        /// \brief Copy constructor
        ///
        Camera(const Camera& other);

        /// \brief Destructor
        ///
        ~Camera() override;


        /// \brief Clone this camera
        ///
        /// \return Pointer to the newly cloned Camera
        ///
        Camera* clone() const override;


        /// \brief Get the projection matrix
        ///
        /// \return The projection matrix
        ///
        const glm::mat4& getProjectionMatrix() const;

        /// \brief Get the view matrix
        ///
        /// The matrix will be fetched straight from the object.
        ///
        /// \return The view matrix
        ///
        const glm::mat4& getViewMatrix() const;


        /// \brief Set the projection mode
        ///
        /// You need to accompany this call with calls to set the camera
        /// properties, as they won't be set to defaults automatically.
        /// Failing to set the properties will cause the projection to
        /// malfunction.
        ///
        /// \param mode The mode to be set
        ///
        void setProjectionMode(const Projection mode);

        /// \brief Get the projection mode
        ///
        /// \return The projection mode
        ///
        Projection getProjectionMode() const;


        /// \brief Set the near and far clipping planes
        ///
        /// In perspective projection the clipping planes need to be positive.
        /// Otherwise, the projection result is undefined.
        ///
        /// \param clipNear The near clipping plane
        /// \param clipFar The far clipping plane
        ///
        void setClippingPlanes(const float clipNear, const float clipFar);

        /// \brief Get the values of the clipping planes
        ///
        /// The first value is the near clipping plane. The second value
        /// is the far clipping plane.
        ///
        /// \return Reference to the internal pair of clipping planes
        ///
        const ClippingPlanes& getClippingPlanes() const;


        /// \brief Brief set the size of the projection
        ///
        /// In perspective mode this call is equal to calling setAspectRatio(size.x / size.y)
        ///
        /// \param size The new size of the projection
        ///
        void setSize(const glm::vec2& size);

        /// \copydoc jop::Camera::setSize(const glm::vec2&)
        ///
        void setSize(const float x, const float y);

        /// \brief Get the size of the projection
        ///
        /// In perspective mode the return value is undefined.
        ///
        /// \return Size of the projection
        ///
        glm::vec2 getSize() const;

        /// \brief Set the aspect ratio
        ///
        /// This call is only valid in perspective mode.
        ///
        /// \param ratio The new aspect ratio to be set
        ///
        void setAspectRatio(const float ratio);

        /// \brief Get the aspect ratio
        ///
        /// In orthographic mode the return value is undefined
        ///
        /// \return The aspect ratio
        ///
        float getAspectRatio() const;


        /// \brief Set the vertical field of view
        ///
        /// The minimum value is 1 and maximum 179.
        /// The value will clamped inside this range.
        ///
        /// \param fovY The new field of view value
        ///
        void setFieldOfView(const float fovY);

        /// \brief Get the field of view value
        ///
        /// \return The field of view value
        ///
        float getFieldOfView() const;

        /// \brief Get the default camera
        ///
        /// This camera is a const object and it's not meant to be used actively.
        ///
        /// \return Const reference to the camera
        ///
        static const Camera& getDefault();

    private:

        mutable glm::mat4 m_projectionMatrix;   ///< The projection matrix
        ProjectionData m_projData;              ///< Union with data for orthographic and perspective projections
        ClippingPlanes m_clippingPlanes;        ///< The clipping planes
        Projection m_mode;                      ///< Projection mode
        mutable bool m_projectionNeedUpdate;    ///< Flag to mark if the projection needs to be updated
        
    };
}

#endif

/// \class Camera
/// \ingroup graphics
///
/// #TODO Detailed description