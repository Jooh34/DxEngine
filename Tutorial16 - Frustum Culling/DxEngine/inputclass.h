#pragma once
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_
//You need to define the version of Direct Input you are using in the header or the compiler will generate annoying messages that it is defaulting to version 8.

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800
//The following two libraries need to be linked for Direct Input to work.

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
//This is the required header for Direct Input.

//////////////
// INCLUDES //
//////////////
#include <dinput.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	//The first three private member variables are the interfaces to Direct Input, the keyboard device, and the mouse device.

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	//The next two private member variables are used for recording the current state of the keyboardand mouse devices.

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif