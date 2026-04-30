#include "CameraNodeProperty.h"
using namespace dj;

CameraNodeProperty::CameraNodeProperty(CameraPtr camera)
	: camera(camera)
{
}

void CameraNodeProperty::setCamera(CameraPtr camera)
{
	this->camera = camera;
}

CameraPtr CameraNodeProperty::getCamera() const
{
	return camera;
}