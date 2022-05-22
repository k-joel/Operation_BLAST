#include "DebugSystem.h"
#include <windows.h>
#include "../Input/CInput.h"

CDebugSystem::CDebugSystem()
{
	m_pD3D = NULL;
	windowUp = false;
}
CDebugSystem& CDebugSystem::operator=(const CDebugSystem& sys)
{
	return *this;
}
CDebugSystem::CDebugSystem(const CDebugSystem& sys)
{
}

CDebugSystem* CDebugSystem::GetInstance(void)
{
	static CDebugSystem instance;
	return &instance;
}

CDebugSystem::~CDebugSystem()
{
	FreeConsole();
}

void CDebugSystem::RegisterData(char* name, int data)
{
	intMap[name] = data;
}

void CDebugSystem::RegisterData(char* name, float data)
{
	floatMap[name] = data;
}

void CDebugSystem::RegisterData(char* name, D3DXVECTOR3 data)
{
	vecMap[name] = data;
}

void CDebugSystem::Init()
{
	m_pD3D = CDirect3D::GetInstance();
}

void CDebugSystem::DisplayInfo(void)
{
	//Buffer to write everything to.
	char display[128];

	//Setting the iterators to the beginning of their respecting thangs.
	intIter = intMap.begin();
	floatIter = floatMap.begin();
	vecIter = vecMap.begin();

	// Position to draw to
	int displayOffsetY = 0;
	int displayOffsetX = 0;

	//Counter used to adjust the text in the rect
	int displayCounter = 0;

	//font->DrawText(0, textStuff, -1, &fontRec, 0, D3DCOLOR_XRGB(0,0,0));
	//sprintf(textStuff, "X: %.4g Y: %.4g Z: %.4g", xPos, yPos, zPos);

	//Looping through the int map
	for(; intIter != intMap.end(); ++intIter)
	{
		sprintf_s(display, "%s: %i", intIter->first, intIter->second);
		displayOffsetY = displayCounter * 13;

		m_pD3D->DrawText(display, displayOffsetX, displayOffsetY, 255, 0, 0);
		++displayCounter;
	}

	//Looping through the float map
	for(; floatIter != floatMap.end(); ++floatIter)
	{
		sprintf_s(display, "%s: %.3g", floatIter->first, floatIter->second);
		displayOffsetY = displayCounter * 13;

		m_pD3D->DrawText(display, displayOffsetX, displayOffsetY, 255, 0, 0);
		++displayCounter;
	}

	//Looping through the vec3f map
	for(; vecIter != vecMap.end(); ++vecIter)
	{
		sprintf_s(display, "%s: X:%.3g Y:%.3g Z:%.3g", vecIter->first, vecIter->second.x, vecIter->second.y, vecIter->second.z);
		displayOffsetY = displayCounter * 13;

		m_pD3D->DrawText(display, displayOffsetX, displayOffsetY, 255, 0, 0);
		++displayCounter;
	}
}

void CDebugSystem::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	if(keyboard->KeyPressed(DIK_GRAVE))
	{
		windowUp = !windowUp;

		if(windowUp)
		{
			AllocConsole();
			outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		}
		else
		{
			FreeConsole();
		}
	}

	//if(windowUp)
	//{
	//	char buffer[64];
	//	DWORD read;
	//	ReadConsole(inputHandle, buffer, 64, &read, NULL);
	//}
}

void CDebugSystem::WriteToConsole(void)
{
	//
	//if(windowUp)
	//{
	//	char display[64];
	//	floatIter = floatMap.begin();

	//	for(; floatIter != floatMap.end(); ++floatIter)
	//	{
	//		sprintf_s(display, "%s: %.3g\n", floatIter->first, floatIter->second);
	//		WriteConsole(outputHandle, display, strlen(display), NULL, NULL);
	//	}
	//}
	//
}