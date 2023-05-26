#include "frustumclass.h"


FrustumClass::FrustumClass()
{
}


FrustumClass::FrustumClass(const FrustumClass& other)
{
}


FrustumClass::~FrustumClass()
{
}
//ConstructFrustum is called every frame by the GraphicsClass.It passes in the the depth of the screen, the projection matrix, and the view matrix.We then use these input variables to calculate the matrix of the view frustum at that frame.With the new frustum matrix we then calculate the six planes that form the view frustum.

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float zMinimum, r;
	float x, y, z, w;
	XMMATRIX matrix;
	XMFLOAT4X4A matrixFF;
	
	XMFLOAT4X4A projectionMatrixFF;
	XMStoreFloat4x4A(&projectionMatrixFF, projectionMatrix);

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrixFF._43 / projectionMatrixFF._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrixFF._33 = r;
	projectionMatrixFF._43 = -r * zMinimum;

	projectionMatrix = XMLoadFloat4x4A(&projectionMatrixFF);
	
	// Create the frustum matrix from the view matrix and updated projection matrix.
	matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	XMStoreFloat4x4A(&matrixFF, matrix);

	// Calculate near plane of frustum.
	x = matrixFF._14 + matrixFF._13;
	y = matrixFF._24 + matrixFF._23;
	z = matrixFF._34 + matrixFF._33;
	w = matrixFF._44 + matrixFF._43;
	m_planes[0] = XMPlaneNormalize(XMVectorSet(x, y, z, w));

	// Calculate far plane of frustum.
	x = matrixFF._14 - matrixFF._13;
	y = matrixFF._24 - matrixFF._23;
	z = matrixFF._34 - matrixFF._33;
	w = matrixFF._44 - matrixFF._43;
	m_planes[1] = XMPlaneNormalize(XMVectorSet(x, y, z, w));

	// Calculate left plane of frustum.
	x = matrixFF._14 + matrixFF._11;
	y = matrixFF._24 + matrixFF._21;
	z = matrixFF._34 + matrixFF._31;
	w = matrixFF._44 + matrixFF._41;
	m_planes[2] = XMPlaneNormalize(XMVectorSet(x, y, z, w));

	// Calculate right plane of frustum.
	x = matrixFF._14 - matrixFF._11;
	y = matrixFF._24 - matrixFF._21;
	z = matrixFF._34 - matrixFF._31;
	w = matrixFF._44 - matrixFF._41;
	m_planes[3] = XMPlaneNormalize(XMVectorSet(x, y, z, w));

	// Calculate top plane of frustum.
	x = matrixFF._14 - matrixFF._12;
	y = matrixFF._24 - matrixFF._22;
	z = matrixFF._34 - matrixFF._32;
	w = matrixFF._44 - matrixFF._42;
	m_planes[4] = XMPlaneNormalize(XMVectorSet(x, y, z, w));

	// Calculate bottom plane of frustum.
	x = matrixFF._14 + matrixFF._12;
	y = matrixFF._24 + matrixFF._22;
	z = matrixFF._34 + matrixFF._32;
	w = matrixFF._44 + matrixFF._42;
	m_planes[5] = XMPlaneNormalize(XMVectorSet(x, y, z, w));

	return;
}
//CheckPoint checks if a single point is inside the viewing frustum.This is the most general of the four checking algorithms but can be very efficient if used correctly in the right situation over the other checking methods.It takes the pointand checks to see if it is inside all six planes.If the point is inside all six then it returns true, otherwise it returns false if not.

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;


	// Check if the point is inside all six planes of the view frustum.
	for (i = 0; i < 6; i++)
	{
		if(DirectX::XMVectorGetX(XMPlaneDotCoord(m_planes[i], DirectX::XMVectorSet(x, y, z, 1))) < 0.0f)
		{
			return false;
		}
	}

	return true;
}
//CheckCube checks if any of the eight corner points of the cube are inside the viewing frustum.It only requires as input the center point of the cubeand the radius, it uses those to calculate the 8 corner points of the cube.It then checks if any one of the corner points are inside all 6 planes of the viewing frustum.If it does find a point inside all six planes of the viewing frustum it returns true, otherwise it returns false.

bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if any one point of the cube is in the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - radius, xCenter - radius, xCenter - radius,1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + radius, xCenter - radius, xCenter - radius,1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - radius, xCenter + radius, xCenter - radius,1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + radius, xCenter + radius, xCenter - radius,1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - radius, xCenter - radius, xCenter + radius,1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + radius, xCenter - radius, xCenter + radius, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - radius, xCenter + radius, xCenter + radius, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + radius, xCenter + radius, xCenter + radius, 1))) >= 0.0f) continue;
		return false;
	}

	return true;
}
//CheckSphere checks if the radius of the sphere from the center point is inside all six planes of the viewing frustum.If it is outside any of them then the sphere cannot be seenand the function will return false.If it is inside all six the function returns true that the sphere can be seen.

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if the radius of the sphere is inside the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter, yCenter, zCenter,1))) < - radius)
		{
			return false;
		}
	}

	return true;
}
//CheckRectangle works the same as CheckCube except that that it takes as input the x radius, y radius, and z radius of the rectangle instead of just a single radius of a cube.It can then calculate the 8 corner points of the rectangle and do the frustum checks similar to the CheckCube function.

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	int i;


	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter - zSize, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter - zSize, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter - zSize, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter - zSize, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter + zSize, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter + zSize, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter + zSize, 1))) >= 0.0f) continue;
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter + zSize, 1))) >= 0.0f) continue;

		return false;
	}

	return true;
}