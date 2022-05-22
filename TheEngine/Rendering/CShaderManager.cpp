//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CShaderManager.cpp"																//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Manages all the shaders loaded in the game			 						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

//	For macros
#include "../Core/Util.h" // for SAFE_RELEASE and DXERROR

#include "CShaderManager.h"
#include <assert.h>		// code in assertions gets compiled out in Release mode

#define MATRIX_PALETTE_SIZE "50"

CShaderManager CShaderManager::m_Instance;

CShaderManager::CShaderManager(void)
{
	m_lpDevice	= NULL;
}

CShaderManager::~CShaderManager(void)
{
}

CShaderManager *CShaderManager::GetInstance(void)
{
	return &m_Instance;
}

void CShaderManager::InitShaderManager(LPDIRECT3DDEVICE9 lpDevice)
{
	m_lpDevice = lpDevice;
	m_lpDevice->AddRef();

	m_dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	m_dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	m_dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	m_dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
}

void CShaderManager::ShutdownShaderManager(void)
{
	for (unsigned int i = 0; i < m_Shaders.size(); i++)
	{
		// Remove ref.
		m_Shaders[i].ref = 0;

		// Release the shader if it's not being used.
		SAFE_RELEASE(m_Shaders[i].effect);
		m_Shaders[i].filename[0] = '\0';
	}

	// Clear the list of all loaded textures.
	m_Shaders.clear();

	//	Release our references to the sprite interface and d3d device
	SAFE_RELEASE(m_lpDevice);
}

int CShaderManager::LoadShader(const char* szFilename)
{
	//	Make sure the filename is valid.
	if (!szFilename)	return -1;

	// Make sure the shader isn't already loaded.
	for (unsigned int i = 0; i < m_Shaders.size(); i++)
	{
		// compare strings without caring about upper or lowercase.
		if (_stricmp(szFilename, m_Shaders[i].filename) == 0)	// 0 means they are equal.
		{
			m_Shaders[i].ref++; // add a reference to this shader.
			return i;	// return the index.
		}
	}

	// Look for an open spot.
	int nID = -1;
	for (unsigned int i = 0; i < m_Shaders.size(); i++)
	{
		if (m_Shaders[i].ref == 0)
		{
			nID = i;
			break;
		}
	}

	// if we didn't find an open spot, load it in a new one
	if (nID == -1)
	{
		EFFECT loaded;

		// Copy the filename of the loaded effect.
		strcpy_s(loaded.filename, MAX_PATH, szFilename);

		// Load the shader from the given file.
		HRESULT hr = 0;

		// Create the FX from a .fx file.
		// research id3dxeffectpool
		ID3DXBuffer* errors = 0;
		if(FAILED(hr = D3DXCreateEffectFromFile(m_lpDevice, szFilename,
												NULL, 0, m_dwShaderFlags, 0,
												&loaded.effect, &errors)))
		{
			// Failed.
			char szBuffer[256] = {0};
			sprintf_s(szBuffer, sizeof(szBuffer), "Failed to Create Shader - %s", szFilename);
			MessageBox(0, szBuffer, "ShaderManager Error", MB_OK);

			if( errors )
				MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

			return -1;
		}

		// release the error buffer.
		// this will get filled out for all warnings/errors
		SAFE_RELEASE(errors);

		// AddRef.
		loaded.ref = 1;

		// Put the shader into the list.
		m_Shaders.push_back(loaded);

		// Return the nID of the shader.
		return (int)m_Shaders.size() - 1;
	}
	// we found an open spot
	else
	{
		// Make sure the shader has been released.
		SAFE_RELEASE(m_Shaders[nID].effect);

		// Copy the filename of the loaded shader.
		strcpy_s(m_Shaders[nID].filename, MAX_PATH, szFilename);

		// Load the shader from the given file.
		HRESULT hr = 0;
		ID3DXBuffer* errors = 0;

		if(FAILED(hr = D3DXCreateEffectFromFile(m_lpDevice, szFilename,
												NULL, 0, m_dwShaderFlags, 0,
												&m_Shaders[nID].effect, &errors)))
		{
			// Failed.
			char szBuffer[256] = {0};
			sprintf_s(szBuffer, sizeof(szBuffer), "Failed to Create Shader - %s", szFilename);
			MessageBox(0, szBuffer, "ShaderManager Error", MB_OK);

			if( errors )
				MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

			return -1;
		}

		// release the error buffer.
		// this will get filled out for all warnings/errors
		SAFE_RELEASE(errors);

		// AddRef
		m_Shaders[nID].ref = 1;

		// Return the nID of the shader.
		return nID;
	}
}

void CShaderManager::UnloadShader(int nID)
{
	// Make sure the nID is in range.
	assert(nID > -1 && nID < (int)m_Shaders.size() && "nID is out of range");

	// Remove ref.
	m_Shaders[nID].ref--;

	// Release the shader if it's not being used.
	if (m_Shaders[nID].ref <= 0)
	{
		// Do a lazy delete and leave this spot empty
		SAFE_RELEASE(m_Shaders[nID].effect);
		m_Shaders[nID].filename[0] = '\0';
		m_Shaders[nID].ref = 0;
	}
}

void CShaderManager::OnLostDevice()
{
	for (unsigned int i = 0; i < m_Shaders.size(); i++)
	{
		if (m_Shaders[i].ref != 0)
			m_Shaders[i].effect->OnLostDevice();
	}
}

void CShaderManager::OnResetDevice()
{
	for (unsigned int i = 0; i < m_Shaders.size(); i++)
	{
		if (m_Shaders[i].ref != 0)
			m_Shaders[i].effect->OnResetDevice();
	}
}

LPD3DXEFFECT CShaderManager::GetEffect(int nID)
{
	assert(nID > -1 && nID < (int)m_Shaders.size() && "nID is out of range");

	return m_Shaders[nID].effect;
}