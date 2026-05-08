#pragma once
#include "Perspective.h"
#include <glm.hpp>

namespace dj {

/*! \class Camera
	\brief Keeps all 3D Camera's data: Transformation, field of view, aspect ratio.
	*/
class Camera : public Perspective {
	bool dirtyView;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::mat4 rotationMatrix;
	glm::mat4 matrix;

public:
	Camera();
	Camera(const Camera& camera);

	/*! \brief Forces all camera transformation matrices update.
	*/
	void updateView();

	/*! \brief Set Camera position in World Space.
		
		When pos.z < 0.0, then camera moves forward (world moves backward)
		When pos.z > 0, then camera moves backward (world moves forward)

		\param[in] pos new camera position in World Space
	*/
	void setPosition(const glm::vec3& pos);
	/*! \brief Set Camera rotation
	
		\param[in] rot rotations vector expressed in degrees (pitch, yaw, roll)
	*/
	void setRotation(const glm::vec3& rot);

	/*! \brief Moves camera along Z-axis in its coordinate system.
		\param[in] move interval that needs to be added to Camera's position
	*/
	void fly(const glm::vec3& move);
	/*! \brief Rotates Camera around its Y-axis (yaw in Camera Space)
		\param[in] degree interval (in degrees) that needs to be added to Camera's around Y-axis rotation
	*/
	void lookSide(float degree);
	/*! \brief Rotate Camera around its X-axis (pitch in Camera Space)
		\param[in] degree interval (in degrees) that needs to be added to Camera's around X-axis rotation
	*/
	void lookVerticaly(float degree);

	/*! \brief Returns View matrix (i.e. Matrix needed for World Transformation)
		\return View matrix
	*/
	glm::mat4 getViewMatrix();

	/*! \brief Returns View * Perspective Matrix (i.e. Matrix converting World to Perspective / Screen / Clip Space) 
		\return View-Perspective Matrix 
	*/
	glm::mat4 getVPMatrix();
	/*! \brief Returns View-Perspective matrix needed for Skybox transformation (i.e. Matrix converting Object Space to Perspective Space for Skybox)
		It generates View-Perspective matrix that omits influence of Camera's Translation and Scale   
		\return Skybox matrix
	*/
	glm::mat4 getSkyboxMatrix();
	/*! \brief Returns Camera's position in World Space
		\return Camera's position in World Space
	*/
	glm::vec3 getPosition() const;

private:
	// Updates rotation matrix (but for world rotation)
	void updateRotationMatrix();
};

} // namespace dj