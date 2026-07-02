#pragma once
#include "Definitions.h"

namespace dj 
{

/*! \class CameraNodeProperty
	\brief Base class for adding Camera property / attribute to other classes.

	It is mostly designed for RenderNodes in order to give simple interface for 
	setting and getting Camera. CameraNodeProperty only holds pointer to the Camera,
	it does not create it, nor delete it.
*/
class CameraNodeProperty
{
protected:
	CameraPtr camera;

public:
	CameraNodeProperty(CameraPtr camera = nullptr);

	/*! \brief Assigns active Camera
		\param[in] camera Pointer to the Camera
	*/
	void setCamera(CameraPtr camera);

	/*! \brief Returns active Camera pointer
		\return Pointer to active Camera
	*/
	CameraPtr getCamera() const;
};
} // namespace dj