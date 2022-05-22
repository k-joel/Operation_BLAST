#ifndef _DEBUG_SYSTEM_H
#define _DEBUG_SYSTEM_H
#include <map>
#include <vector>
//Make sure this is fixed
//#include "../EDCamera/Source/vec3.h"

//Direct X includes
#include "../Rendering/CDirect3D.h"

////////////////////////////////////////////////////////////////////////////
//Class Name:		CDebugSystem
//Author:			TJ "The Guy" Farnie
//
//Purpose:			To provide easily accessable debug info.  I probably
//					spelled that wrong.  Ah well.
////////////////////////////////////////////////////////////////////////////
class CDebugSystem
{
private:
	HANDLE outputHandle; //Handle for the console window output stream
	HANDLE inputHandle; //Handle for the console window input stream
	bool windowUp; //True if the window is displayed, false if it is not

	//DRX font object stuff
	CDirect3D *m_pD3D;

	//The maps that hold all the info.  Look in to a better way to combine all the maps
	std::map<char*, int> intMap;
	std::map<char*, float> floatMap;
	std::map<char*, D3DXVECTOR3> vecMap;

	//Associated map iterators
	std::map<char*, int>::iterator intIter;
	std::map<char*, float>::iterator floatIter;
	std::map<char*, D3DXVECTOR3>::iterator vecIter;

	//Singleton Functions
	CDebugSystem();
	CDebugSystem& operator=(const CDebugSystem& sys);
	CDebugSystem(const CDebugSystem& sys);
	~CDebugSystem();

public:
	static CDebugSystem* GetInstance(void);
	void Init();
	void RegisterData(char* name, int data);
	void RegisterData(char* name, float data);
	void RegisterData(char* name, D3DXVECTOR3 data);
	void DisplayInfo(void);

	void Input(void);
	void WriteToConsole(void);
};
#endif