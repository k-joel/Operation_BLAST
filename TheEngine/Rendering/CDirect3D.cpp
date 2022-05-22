//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CDirect3D.cpp"																//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Creates the direct3D device							 						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#include "CDirect3D.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "../FX/CParticleManager.h"

CDirect3D CDirect3D::m_Instance;

CDirect3D::CDirect3D()
{
	m_hWnd   = 0;
	m_pD3DObject  = 0;
	m_pD3DDevice = 0;
	m_pD3DFont = 0;
	m_pD3DSprite = 0;
	m_pD3DLine = 0;

	m_pModes = 0;

	ZeroMemory(&m_D3DPresentParams, sizeof(m_D3DPresentParams));
}

CDirect3D::~CDirect3D()
{
}

void CDirect3D::ShutdownD3D(void)
{
	SAFE_DELETE_ARRAY(m_pModes);

	DestroyAllVertexDeclarations();

	SAFE_RELEASE(m_pD3DFont);
	SAFE_RELEASE(m_pD3DLine);
	SAFE_RELEASE(m_pD3DSprite);
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3DObject);
}

CDirect3D* CDirect3D::GetInstance(void)
{
	return &m_Instance;
}

LPDIRECT3D9						CDirect3D::GetDirect3DObject(void)	{ return m_pD3DObject; }
LPDIRECT3DDEVICE9				CDirect3D::GetDirect3DDevice(void)	{ return m_pD3DDevice; }
LPD3DXSPRITE					CDirect3D::GetSprite(void)			{ return m_pD3DSprite; }
LPD3DXLINE						CDirect3D::GetLine(void)			{ return m_pD3DLine; }
LPD3DXFONT						CDirect3D::GetFont(void)			{ return m_pD3DFont; }

const D3DPRESENT_PARAMETERS*	CDirect3D::GetPresentParams(void)	{ return &m_D3DPresentParams;	 }

void CDirect3D::InitD3D(HWND hWnd, HINSTANCE hInstance, int nWindowWidth, int nWindowHeight,  int nMultisample,
						bool bIsWindowed, bool bVSync)
{
	// Step 1: Create the IDirect3D9 object.
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);
	if( !m_pD3DObject )
	{
		OutputDebugString("Failed to Create D3D Object\n");
	}

	m_hWnd = hWnd;
	m_hAppInst = hInstance;

	// Step 2: Verify hardware support for specified formats in windowed and full screen modes.
	m_pD3DObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_Mode);
	HR(m_pD3DObject->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_Mode.Format, m_Mode.Format, true));
	HR(m_pD3DObject->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	m_nNumModes = m_pD3DObject->GetAdapterModeCount(D3DADAPTER_DEFAULT, m_Mode.Format);
	m_pModes = new D3DDISPLAYMODE[m_nNumModes];
	for(unsigned int i = 0; i < m_nNumModes; i++)
	{
		m_pD3DObject->EnumAdapterModes(D3DADAPTER_DEFAULT, m_Mode.Format, i, &m_pModes[i]);
	}

	// Step 3: Check for requested vertex processing and pure device.
	D3DCAPS9 caps;
	HR(m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));

	// Check for shader version 2.0 support.
	if( caps.VertexShaderVersion < D3DVS_VERSION(2, 0) || caps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
	{
		char szBuffer[256] = {0};
		sprintf_s(szBuffer, sizeof(szBuffer), "Shader Model Version 2.0 or higher required\n");
		MessageBox(0, szBuffer, "Direct3D Error", MB_OK);
	}

	DWORD devBehaviorFlags = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		devBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// If pure device and HW T&L supported
	if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
		devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;

	D3DMULTISAMPLE_TYPE multisampleType = D3DMULTISAMPLE_NONE;
	if( (nMultisample >= 0 && nMultisample <= 16) &&
		SUCCEEDED(m_pD3DObject->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		(bIsWindowed) ? m_Mode.Format : D3DFMT_X8R8G8B8, bIsWindowed, (D3DMULTISAMPLE_TYPE)nMultisample, &m_dwMSAAQualityLevels)) )
	{
		multisampleType = (D3DMULTISAMPLE_TYPE)nMultisample;
	}

	// Step 4: Fill out the D3DPRESENT_PARAMETERS structure.
	m_D3DPresentParams.BackBufferWidth            = nWindowWidth;
	m_D3DPresentParams.BackBufferHeight           = nWindowHeight;
	m_D3DPresentParams.BackBufferFormat           = (bIsWindowed) ? m_Mode.Format : D3DFMT_X8R8G8B8;
	m_D3DPresentParams.BackBufferCount            = 1;
	m_D3DPresentParams.MultiSampleType            = multisampleType;
	m_D3DPresentParams.MultiSampleQuality         = 0;
	m_D3DPresentParams.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	m_D3DPresentParams.hDeviceWindow              = m_hWnd;
	m_D3DPresentParams.Windowed                   = bIsWindowed;
	m_D3DPresentParams.EnableAutoDepthStencil     = true;
	m_D3DPresentParams.AutoDepthStencilFormat     = D3DFMT_D24S8;
	m_D3DPresentParams.Flags                      = 0;/*D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;*/
	m_D3DPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_D3DPresentParams.PresentationInterval       = (bVSync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 5: Create the device.
	HR(m_pD3DObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		devBehaviorFlags, 	&m_D3DPresentParams,  &m_pD3DDevice));

	// Create Sprite Object.
	HR(D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite));

	// Create the Line Object.
	HR(D3DXCreateLine(m_pD3DDevice, &m_pD3DLine));

	// Create the Font
	HR(D3DXCreateFontA(m_pD3DDevice, 0, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_pD3DFont));

	//////////////////////////////////////////////////////////////////////////
	InitAllVertexDeclarations();

	// Setup window style flags
	DWORD dwWindowStyleFlags = WS_VISIBLE;

	//	Set the window to the middle of the screen.
	if (bIsWindowed)
	{
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
		ShowCursor(TRUE); // show the mouse cursor
		SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);

		// Setup the desired client area size
		RECT rWindow;
		rWindow.left	= 0;
		rWindow.top		= 0;
		rWindow.right	= nWindowWidth;
		rWindow.bottom	= nWindowHeight;

		// Get the dimensions of a window that will have a client rect that
		// will really be the resolution we're looking for.
		AdjustWindowRectEx(&rWindow,
			dwWindowStyleFlags,
			FALSE,
			WS_EX_APPWINDOW);

		// Calculate the width/height of that window's dimensions
		int windowWidth		= rWindow.right - rWindow.left;
		int windowHeight	= rWindow.bottom - rWindow.top;

		SetWindowPos(m_hWnd, HWND_TOP,	(GetSystemMetrics(SM_CXSCREEN)>>1) - (windowWidth>>1),
			(GetSystemMetrics(SM_CYSCREEN)>>1) - (windowHeight>>1),
			windowWidth, windowHeight, SWP_SHOWWINDOW);
	}
	else
	{
		dwWindowStyleFlags |= WS_POPUP;
		ShowCursor(FALSE); // hide the mouse cursor
		SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);

		// Let windows know the window has changed.
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void CDirect3D::OnLostDevice()
{
	CTextureManager::GetInstance()->OnLostDevice();
	CParticleManager::GetInstance()->OnLostDevice();
	CShaderManager::GetInstance()->OnLostDevice();
	m_pD3DFont->OnLostDevice();
	m_pD3DLine->OnLostDevice();
	m_pD3DSprite->OnLostDevice();
}

void CDirect3D::OnResetDevice()
{
	m_pD3DSprite->OnResetDevice();
	m_pD3DLine->OnResetDevice();
	m_pD3DFont->OnResetDevice();
	CShaderManager::GetInstance()->OnResetDevice();
	CParticleManager::GetInstance()->OnResetDevice();
	CTextureManager::GetInstance()->OnResetDevice();
}

// returns true if failed/lost
bool CDirect3D::Clear(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue)
{
	// Clear the backbuffer and depth buffer.
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(ucRed, ucGreen, ucBlue), 1.0f, 0);

	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
	if(hr == D3DERR_DEVICENOTRESET)
	{
		OnLostDevice();
		m_pD3DDevice->Reset(&m_D3DPresentParams);
		OnResetDevice();
		return false;
	}
	else if(hr == D3DERR_DEVICELOST)
	{
		Sleep(200);
		return true;
	}

	return false;
}

void CDirect3D::DeviceBegin(void)
{
	m_pD3DDevice->BeginScene();
}

void CDirect3D::SpriteBegin(void)
{
	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDirect3D::LineBegin(void)
{
	m_pD3DLine->Begin();
}

void CDirect3D::DeviceEnd(void)
{
	m_pD3DDevice->EndScene();
}

void CDirect3D::SpriteEnd(void)
{
	m_pD3DSprite->End();
}

void CDirect3D::LineEnd(void)
{
	m_pD3DLine->End();
}

void CDirect3D::Present(void)
{
	// Present the backbuffer.
	m_pD3DDevice->Present(0, 0, 0, 0);
}

void CDirect3D::DrawText(char *lpzText, int nX, int nY, unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue)
{
	RECT rect;
	SetRect(&rect, nX, nY, 0, 0);

	m_pD3DFont->DrawTextA(m_pD3DSprite, lpzText, -1, &rect, DT_NOCLIP, D3DCOLOR_XRGB(ucRed, ucGreen, ucBlue));
}

void CDirect3D::DrawLine(int nX1, int nY1, int nX2, int nY2, unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue)
{
	D3DXVECTOR2 verts[2];

	verts[0].x = (float)nX1;
	verts[0].y = (float)nY1;
	verts[1].x = (float)nX2;
	verts[1].y = (float)nY2;

	m_pD3DLine->Draw(verts, 2, D3DCOLOR_XRGB(ucRed, ucGreen, ucBlue));
}

void CDirect3D::DrawRect(RECT rRt, unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue)
{
	D3DRECT d3dRect;
	d3dRect.x1 = rRt.left;
	d3dRect.y1 = rRt.top;
	d3dRect.x2 = rRt.right;
	d3dRect.y2 = rRt.bottom;

	m_pD3DDevice->Clear(1, &d3dRect, D3DCLEAR_TARGET, D3DCOLOR_XRGB(ucRed, ucGreen, ucBlue), 1.0f, 0);
}

void CDirect3D::ChangeDisplayParam(int nWindowWidth, int nWindowHeight,  int nMultisample,
								   bool bIsWindowed, bool bVSync)
{
	D3DMULTISAMPLE_TYPE multisampleType = D3DMULTISAMPLE_NONE;
	if( (nMultisample >= 0 && nMultisample <= 16) &&
		SUCCEEDED(m_pD3DObject->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		(bIsWindowed) ? m_Mode.Format : D3DFMT_X8R8G8B8, bIsWindowed, (D3DMULTISAMPLE_TYPE)nMultisample, &m_dwMSAAQualityLevels)) )
	{
		multisampleType = (D3DMULTISAMPLE_TYPE)nMultisample;
	}

	// Set the new Presentation Parameters.
	m_D3DPresentParams.BackBufferWidth            = nWindowWidth;
	m_D3DPresentParams.BackBufferHeight           = nWindowHeight;
	m_D3DPresentParams.BackBufferFormat           = (bIsWindowed) ? m_Mode.Format : D3DFMT_X8R8G8B8;
	m_D3DPresentParams.MultiSampleType            = multisampleType;
	m_D3DPresentParams.Windowed                   = bIsWindowed;
	m_D3DPresentParams.PresentationInterval       = (bVSync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	// Reset the device.
	OnLostDevice();
	m_pD3DDevice->Reset(&m_D3DPresentParams);
	OnResetDevice();

	// Setup window style flags
	DWORD dwWindowStyleFlags = WS_VISIBLE;

	//	Set the window to the middle of the screen.
	if (bIsWindowed)
	{
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
		ShowCursor(TRUE); // show the mouse cursor
		SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);

		// Setup the desired client area size
		RECT rWindow;
		rWindow.left	= 0;
		rWindow.top		= 0;
		rWindow.right	= nWindowWidth;
		rWindow.bottom	= nWindowHeight;

		// Get the dimensions of a window that will have a client rect that
		// will really be the resolution we're looking for.
		AdjustWindowRectEx(&rWindow,
			dwWindowStyleFlags,
			FALSE,
			WS_EX_APPWINDOW);

		// Calculate the width/height of that window's dimensions
		int windowWidth		= rWindow.right - rWindow.left;
		int windowHeight	= rWindow.bottom - rWindow.top;

		SetWindowPos(m_hWnd, HWND_TOP,	(GetSystemMetrics(SM_CXSCREEN)>>1) - (windowWidth>>1),
			(GetSystemMetrics(SM_CYSCREEN)>>1) - (windowHeight>>1),
			windowWidth, windowHeight, SWP_SHOWWINDOW);
	}
	else
	{
		dwWindowStyleFlags |= WS_POPUP;
		ShowCursor(FALSE); // hide the mouse cursor
		SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);

		// Let windows know the window has changed.
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
}