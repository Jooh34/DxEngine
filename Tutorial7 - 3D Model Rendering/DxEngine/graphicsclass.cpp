#include "stdafx.h"
#include "graphicsclass.h"
#include "D3dclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;

	m_LightShader = 0;
	m_Light = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), (char*) "../DxEngine/data/cube.txt", (WCHAR*) L"../DxEngine/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	//The new light object is created here.

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	//	The color of the light is set to purpleand the light direction is set to point down the positive Z axis.

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// D3D 객체를 반환합니다.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);

	//그래픽 렌더링을 수행합니다.
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = worldMatrix * XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// Render the model using the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_Model->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	// Render the model using the color shader.
	//result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	//if (!result)
	//{
	//	return false;
	//}

	// 버퍼에 그려진 씬을 화면에 표시합니다.
	m_D3D->EndScene();

	return true;
}
