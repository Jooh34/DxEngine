#include "stdafx.h"
#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = nullptr;
	m_Graphics = nullptr;
}

SystemClass::SystemClass(const SystemClass&)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// �Լ��� ���̿� �ʺ� �����ϱ� ���� ������ 0���� �ʱ�ȭ�Ѵ�.
	screenWidth = 0;
	screenHeight = 0;

	// �������� api�� ����Ͽ� �ʱ�ȭ�Ѵ�.
	InitializeWindows(screenWidth, screenHeight);

	// input ��ü�� �����Ѵ�.
	// �� ��ü�� �����κ��� ������ Ű���� �Է��� ó���ϱ����� ���ȴ�.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Input ��ü�� �ʱ�ȭ�Ѵ�.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// graphics ��ü�� �����Ѵ�.
	// �� ��ü�� �� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ���� �Ѵ�.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// graphics ��ü�� �ʱ�ȭ �Ѵ�.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();
	//Createand initialize the CpuClass.

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Graphics ��ü�� ��ȯ�մϴ�.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Input ��ü�� ��ȯ�մϴ�.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = nullptr;
	}

	// â�� �����ŵ�ϴ�.
	ShutdownWindows();
	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// �޼��� ����ü�� �ʱ�ȭ�մϴ�.
	ZeroMemory(&msg, sizeof(MSG));

	// �����κ��� ���� �޽����� ���� ������ ������ ����.
	done = false;
	while (!done)
	{
		// ������ �޽����� ó���մϴ�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �����쿡�� ���ø����̼��� ���Ḧ ��û�ϴ� ��� ���������ϴ�.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// �� �ܿ��� Frame �Լ��� ó���մϴ�.
			result = Frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Ű����Ű�� ���ȴ��� Ȯ��
		//case WM_KEYDOWN:
		//{
		//	m_Input->IsKeyDown(static_cast<unsigned int>(wparam));
		//	return 0;
		//}

		//// Ű������ ���� Ű�� ���������� Ȯ��
		//case WM_KEYUP:
		//{
		//	// Ű�� �������ٸ� input ��ü�� �� ����� �����Ͽ� �� Ű�� ������� �Ѵ�.
		//	m_Input->KeyUp(static_cast<unsigned int>(wparam));
		//	return 0;
		//}

		// �ٸ� �޼������� ������� �����Ƿ� �⺻ �޼��� ó���⿡ ����
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

bool SystemClass::Frame()
{
	bool result;

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// graphics ��ü�� �۾��� ó���մϴ�.
	result = m_Graphics->Render(0);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺ� �����͸� �� ��ü�� ����
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ� ��������.
	m_hinstance = GetModuleHandle(nullptr);

	// ���ø����̼��� �̸� ����
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻ �������� ����.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ���� ���
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ� ��������
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ������ �մϴ�.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� ��ġ�� ȭ���� ���� ���� ����ϴ�.
		posX = posY = 0;

	}
	else
	{
		// ������ ����� 800X600���� ũ�� ����
		screenWidth = 800;
		screenHeight = 600;

		// â�� ������� �߾ӿ� ������ ����
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ���� ������ â�� ����� �� �ڵ��� �����ɴϴ�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hinstance, nullptr);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �ݴϴ�.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ�� ǥ������ �ʱ�
	ShowCursor(false);

	return;

}

void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� ǥ���մϴ�.
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲߴϴ�.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	// â�� �����մϴ�.
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;

	// ���ø����̼� �ν��Ͻ��� �����Ѵ�.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = nullptr;

	// �� Ŭ������ ���� �ܺ������� ������ ����
	ApplicationHandle = nullptr;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// �����찡 ���ŵǾ����� Ȯ��
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// �����찡 �������� Ȯ���Ѵ�.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}

		// �ٸ� ��� �޼������� system Ŭ������ �޼��� ó���⿡ �����մϴ�.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}