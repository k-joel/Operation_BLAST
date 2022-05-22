//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CDirect3D.h"																	//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Creates the direct3D device							 						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <sstream>
#include <vector>
#include <string>
#include <crtdbg.h>
#include <list>
#include "Vertex.h"
#include "../Core/Util.h"

struct TDirLight
{
	D3DXCOLOR m_Ambient;
	D3DXCOLOR m_Diffuse;
	D3DXVECTOR3 m_vDirection;
};

struct TSpotLight
{
	D3DXCOLOR m_Ambient;
	D3DXCOLOR m_Diffuse;
	D3DXVECTOR4 m_vPosition;
	D3DXVECTOR4 m_vDirection;
	float m_fCosTheta;
};

struct TMaterial
{
	D3DXCOLOR m_Ambient;
	D3DXCOLOR m_Diffuse;
};

//////////////////////////////////////////////////////////////////////////
// D3D
//////////////////////////////////////////////////////////////////////////
const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);

//===============================================================
// Debug

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x) { HRESULT hr = x;  if(FAILED(hr))	{ DXTrace(__FILE__, __LINE__, hr, #x, TRUE);} }
#endif

#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif

#else

#ifndef HR
#define HR(x) x;
#endif

#endif

using namespace std;
class CDirect3D
{
private:

	CDirect3D();
	~CDirect3D();
	CDirect3D(CDirect3D &ref);
	CDirect3D &operator=(CDirect3D &ref);

	LPDIRECT3DDEVICE9		m_pD3DDevice;
	LPDIRECT3D9				m_pD3DObject;
	LPD3DXSPRITE			m_pD3DSprite;
	LPD3DXLINE				m_pD3DLine;
	LPD3DXFONT				m_pD3DFont;
	D3DPRESENT_PARAMETERS	m_D3DPresentParams;

	// Application, Windows, and Direct3D data members.
	HINSTANCE             m_hAppInst;
	HWND                  m_hWnd;

	// caps
	DWORD					m_dwMSAAQualityLevels;
	D3DDISPLAYMODE*			m_pModes;
	unsigned int			m_nNumModes;
	D3DDISPLAYMODE			m_Mode;

	static CDirect3D		m_Instance;

public:

	void OnLostDevice();
	void OnResetDevice();

	void InitD3D(HWND hWnd, HINSTANCE hInstance, int nWindowWidth, int nWindowHeight, int nMultisample, bool bIsWindowed, bool bVSync);

	void ChangeDisplayParam(int nWindowWidth, int nWindowHeight,  int nMultisample,
								bool bIsWindowed, bool bVSync);
	void ShutdownD3D(void);

	bool Clear(unsigned char ucRed = 0, unsigned char ucGreen = 0, unsigned char ucBlue = 0);
	void DeviceBegin(void);
	void SpriteBegin(void);
	void LineBegin(void);
	void DeviceEnd(void);
	void SpriteEnd(void);
	void LineEnd(void);
	void Present(void);

	void DrawText(char *lpzText, int nX, int nY, unsigned char ucRed = 255, unsigned char ucGreen = 255, unsigned char ucBlue = 255);
	void DrawLine(int nX1, int nY1, int nX2, int nY2, unsigned char ucRed = 255, unsigned char ucGreen = 255, unsigned char ucBlue = 255);
	void DrawRect(RECT rRt, unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue);

	static CDirect3D* GetInstance(void);

	LPDIRECT3D9						GetDirect3DObject(void);
	LPDIRECT3DDEVICE9				GetDirect3DDevice(void);
	LPD3DXSPRITE					GetSprite(void);
	LPD3DXLINE						GetLine(void);
	LPD3DXFONT						GetFont(void);
	const D3DPRESENT_PARAMETERS*	GetPresentParams(void);

	D3DDISPLAYMODE &GetDisplayMode(unsigned int idx) { return m_pModes[idx]; }
	unsigned int GetNumModes() { return m_nNumModes; }
};