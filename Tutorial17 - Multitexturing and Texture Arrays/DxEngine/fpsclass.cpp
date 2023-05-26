#include "fpsclass.h"

FpsClass::FpsClass()
{
}


FpsClass::FpsClass(const FpsClass& other)
{
}


FpsClass::~FpsClass()
{
}

void FpsClass::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void FpsClass::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}
//GetFps returns the frame per second speed for the last second that just passed.This function should be constantly queried so the latest fps can be displayed to the screen.

int FpsClass::GetFps()
{
	return m_fps;
}
