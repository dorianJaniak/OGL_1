#pragma once
#include "Perspective.h"
#include <glm.hpp>
#include <array>

namespace dj
{

/*! \class CameraCube
	\brief Keeps all Cube Camera's data: Transformations and Perspective.

	CameraCube is the camera that is used for rendering into CubeMap. In fact
	it consists of 6 transformation matrices (each oriented into one of the Cube walls).
	We may say that CameraCube keeps 6 standard 3D Cameras, but their position, perspective,
	field of view, aspect ratio (which is only 1:1) are common. Only rotation differs.

	\todo 
	- Cube Map planes indexing (enum)
*/
class CameraCube : private Perspective
{
	bool dirtyTransformations;
	glm::vec3 position;
	mutable std::array<glm::mat4, 6> transformations;

	const glm::mat4 defaultMat;

public:
	CameraCube() noexcept;
	CameraCube(const CameraCube& camera) noexcept = default;
	CameraCube(CameraCube&&) noexcept = default;

	/*! \brief Set Camera position in World Space.

		When pos.z < 0.0, then camera moves forward (world moves backward)
		When pos.z > 0, then camera moves backward (world moves forward)

		\param[in] pos new camera position in World Space
	*/
	void setPosition(const glm::vec3& pos);
	void setPlanes(float near, float far);

	/*! \brief Returns Perspective matrix (i.e. Matrix converting Camera / View Space to Perspective / Screen / Clip Space)
		\return Perspective matrix
	*/
	const glm::mat4& getPerspectiveMatrix();
	/*! \brief Returns View * Perspective Matrix for a single side of the CubeMap 
			(i.e. Matrix converting World to Perspective / Screen / Clip Space)
		\param[in] side numbers from 0 to 5 for CubeMap walls 
		\return View-Perspective Matrix for argument from 0 to 5; otherwise default matrix is returned
	*/
	const glm::mat4& getVPMatrix(unsigned int side);
	const std::array<glm::mat4, 6>& getVPMatrices();

	float getNear() const;
	float getFar() const;
	/*! \brief Returns Camera's position in World Space
		\return Camera's position in World Space
	*/
	const glm::vec3& getPosition() const;

private:
	void update();
	void updateTransformations();
};

} // namespace dj