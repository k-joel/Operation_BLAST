//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CShaderManager.h"																//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Manages all the shaders loaded in the game			 						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// The include files for Direct3D9
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
using std::vector;

class CShaderManager
{
private:
	// All the data we need to contain a shader.
	typedef struct _EFFECT
	{
		char				filename[_MAX_PATH];	// The filename of this shader.
		int					ref;					// The number of times this shader has be loaded.
		LPD3DXEFFECT		effect;					// A pointer to the shader.
		int					pass;

		// Default constructor.
		_EFFECT()
		{
			filename[0] = '\0';
			ref = 0;
			effect = NULL;
		}
	} EFFECT, *LPEFFECT;

private:
	///////////////////////////////////////////////////////////////////
	//	Function:	CSGD_TextureManager(Constructors)
	///////////////////////////////////////////////////////////////////
	CShaderManager(void);
	CShaderManager(CShaderManager &ref);
	CShaderManager &operator=(CShaderManager &ref);

private:
	vector<EFFECT>				m_Shaders;			// A list of all the loaded textures.
	LPDIRECT3DDEVICE9			m_lpDevice;			// A pointer to the Direct3D device.
	static CShaderManager		m_Instance;			// An instance to this class.

	DWORD						m_dwShaderFlags;

public:
	///////////////////////////////////////////////////////////////////
	//	Function:	CSGD_TextureManager(Destructor)
	///////////////////////////////////////////////////////////////////
	~CShaderManager(void);

	static CShaderManager *GetInstance(void);

	void InitShaderManager(LPDIRECT3DDEVICE9 lpDevice);

	void ShutdownShaderManager(void);

	int LoadShader(const char* szFilename);

	void UnloadShader(int nID);

	void OnLostDevice();
	void OnResetDevice();

	LPD3DXEFFECT GetEffect(int nID);
};