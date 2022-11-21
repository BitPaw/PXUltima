#include "PXCamera.h"

#include <Memory/PXMemory.h>
#include <Math/PXMath.h>

void PXCameraConstruct(PXCamera* const camera)
{
    MemoryClear(camera, sizeof(PXCamera));

	camera->WalkSpeed = 0.2;
	camera->ViewSpeed = 0.5;
	camera->FollowSpeed = 0.98f; 
	camera->FieldOfView = 60;
	camera->Height = 1;
	camera->Width = 1;
	camera->Near = 0.00001f;
	camera->Far = 100000;

	PXMatrix4x4FIdentity(&camera->MatrixModel);
	PXMatrix4x4FIdentity(&camera->MatrixView);
	PXMatrix4x4FIdentity(&camera->MatrixProjection);

	PXCameraViewChange(camera, PXCameraPerspective3D);
}

void PXCameraDestruct(PXCamera* const camera)
{

}

void PXCameraViewChangeToOrthographic(PXCamera* const camera, const float width, const float height, const float nearPlane, const float farPlane)
{
	const float scaling = 0.10;
	const float left = -(width / 2.0f) * scaling;
	const float right = (width / 2.0f) * scaling;
	const float bottom = -(height / 2.0f) * scaling;
	const float top = (height / 2.0f) * scaling;

	camera->Width = width;
	camera->Height = height;
	camera->Near = nearPlane;
	camera->Far = farPlane;

	PXMatrix4x4FOrthographic(&camera->MatrixProjection, left, right, bottom, top, nearPlane, farPlane);
}

void PXCameraViewChangeToPerspective(PXCamera* const camera, const float fieldOfView, const float aspectRatio, const float nearPlane, const float farPlane)
{
	camera->FieldOfView = fieldOfView;
	camera->Near = nearPlane;
	camera->Far = farPlane;

	PXMatrix4x4FPerspective(&camera->MatrixProjection, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PXCameraAspectRatioChange(PXCamera* const camera, const size_t width, const size_t height)
{
	camera->Width = width;
	camera->Height = height;

	PXCameraViewChange(camera, camera->Perspective);
}

float PXCameraAspectRatio(const PXCamera* const camera)
{
	return (float)camera->Width / (float)camera->Height;
}

void PXCameraViewChange(PXCamera* const camera, const PXCameraPerspective cameraPerspective)
{
	camera->Perspective = cameraPerspective;

	switch (cameraPerspective)
	{
		case PXCameraPerspective2D:
		{
			PXCameraViewChangeToOrthographic(camera, camera->Width, camera->Height, camera->Near, camera->Far);
			break;
		}

		case PXCameraPerspective3D:
		{
			const float aspectRatio = PXCameraAspectRatio(camera);

			PXCameraViewChangeToPerspective(camera, camera->FieldOfView, aspectRatio, camera->Near, camera->Far);

			break;
		}
	}
}

void PXCameraRotate(PXCamera* const camera, const PXVector3F* const vector3F)
{
	const float maxValue = 85.0f;
	const float minValue = -85.0f;

	camera->CurrentRotation.X -= vector3F->X * camera->ViewSpeed;
	camera->CurrentRotation.Y -= vector3F->Y * camera->ViewSpeed;

	camera->CurrentRotation.Y = MathLimit(camera->CurrentRotation.Y, minValue, maxValue);

	const float pitchRAD = MathDegreeToRadians(camera->CurrentRotation.Y);
	const float yawRAD = MathDegreeToRadians(camera->CurrentRotation.X);
	const float rx = MathCosinus(pitchRAD) * MathCosinus(yawRAD);
	const float ry = MathSinus(pitchRAD);
	const float rz = MathCosinus(pitchRAD) * MathSinus(yawRAD);

	PXVector3FSetXYZ(&camera->LookAtPosition, rx, ry, rz, &camera->LookAtPosition);
	PXVector3FNormalize(&camera->LookAtPosition, &camera->LookAtPosition);
}

void PXCameraRotateXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
	const PXVector3F vector = {x, y, z};

	PXCameraRotate(camera, &vector);
}

void PXCameraMoveXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
	const PXVector3F vector3F = { x, y, z };

	PXCameraMove(camera, &vector3F);
}

void PXCameraMove(PXCamera* const camera, const PXVector3F* const vector3F)
{
	PXVector3F xAxis = { 0,0,0 };
	const PXVector3F yAxis = { 0, vector3F->Y, 0 };
	PXVector3F zAxis = { 0,0,0 };	

	// ...
	{
		const PXVector3F up = { 0, 1, 0 };
		const PXVector3F lookAtPosition = { camera->LookAtPosition.X, camera->LookAtPosition.Y, camera->LookAtPosition.Z };

		PXVector3FCrossProduct(&lookAtPosition, &up, &xAxis);
		PXVector3FNormalize(&xAxis, &xAxis);
		PXVector3FMultiplyXYZ(&xAxis, vector3F->X, 0, vector3F->X, &xAxis);

		zAxis = lookAtPosition;

		PXVector3FNormalize(&zAxis, &zAxis);	
		PXVector3FMultiplyXYZ(&zAxis, vector3F->Z, 0, vector3F->Z, &zAxis);
	}

	{
		PXVector3F targetedMovement = { 0,0,0 };

		PXVector3FAdd(&targetedMovement, &xAxis, &targetedMovement);
		PXVector3FAdd(&targetedMovement, &yAxis, &targetedMovement);
		PXVector3FAdd(&targetedMovement, &zAxis, &targetedMovement);
		PXVector3FMultiplyS(&targetedMovement, camera->WalkSpeed, &targetedMovement);

		PXMatrix4x4FMove3F(&camera->MatrixModel, &targetedMovement, &camera->MatrixModel);
	}
}

void PXCameraFollow(PXCamera* const camera, const float deltaTime)
{
	PXVector3F cameraPositionCurrent;
	PXVector3F desiredPosition;

	if (!camera->Target)
	{
		return;
	}

	PXMatrix4x4FPosition(&camera->MatrixModel, &cameraPositionCurrent); // Get current camera pos
	PXMatrix4x4FPosition(&camera->Target, &desiredPosition); // get current target pos

	PXVector3FAdd(&desiredPosition, &camera->Offset, &desiredPosition); // add offset to target pos

	PXVector3FInterpolate(&cameraPositionCurrent, &desiredPosition, camera->FollowSpeed * deltaTime, &desiredPosition); // calculate delta movement

	PXMatrix4x4FMoveTo(&camera->MatrixModel, &desiredPosition, &camera->MatrixModel); // Set delte movement
}

void PXCameraUpdate(PXCamera* const camera, const float deltaTime)
{
	const float walkSpeedSmoothed = camera->WalkSpeed * deltaTime;
	const float viewSpeedSmoothed = camera->ViewSpeed * deltaTime;
	const PXVector3F up = { 0,1,0 };
	PXVector3F currentPosition;
	PXVector3F centerPosition;

	PXMatrix4x4FPosition(&camera->MatrixModel, &currentPosition);
	PXVector3FAdd(&currentPosition, &camera->LookAtPosition, &centerPosition);

	PXMatrix4x4FLookAt(&camera->MatrixView, &currentPosition, &centerPosition, &up, &camera->MatrixView);
}