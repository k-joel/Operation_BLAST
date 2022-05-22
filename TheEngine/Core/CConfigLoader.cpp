#include "CConfigLoader.h"

#include <stdio.h>

CConfigLoader CConfigLoader::m_Instance;

CConfigLoader::CConfigLoader()
{
	//strcpy_s(m_szSettingsPath, MAX_PATH, "./settings.ini");

	m_nResolutionWidth = 1024;
	m_nResolutionHeight = 768;
	m_nAntiAliasLevel = 2;
	m_bVSync = true;
	m_nGammaLevel = 50;
	m_bWindowed = true;

	m_nSFXVolume = 50;
	m_nMusicVolume = 50;

	m_nDifficulty = 1;
	m_nMaxLives = 3;
}

CConfigLoader::~CConfigLoader()
{
}

CConfigLoader *CConfigLoader::GetInstance(void)
{
	return &m_Instance;
}

void CConfigLoader::LoadConfig(const char* pFile)
{
	strcpy_s(m_szSettingsPath, MAX_PATH, pFile);

	// video options
	ReadData(m_bWindowed, "video", "Windowed");
	ReadData(m_nResolutionWidth, "video", "ResolutionWidth");
	ReadData(m_nResolutionHeight, "video", "ResolutionHeight");
	ReadData(m_nAntiAliasLevel, "video", "AntiAlias");
	ReadData(m_bVSync, "video", "VSync");
	ReadData(m_nGammaLevel, "video", "Gamma");

	// sound options
	ReadData(m_nMasterVolume, "sound", "Master");
	ReadData(m_nSFXVolume, "sound", "SFX");
	ReadData(m_nMusicVolume, "sound", "Music");
	ReadData(m_nDialogueVolume,"sound","Dialogue");

	// gameplay options
	ReadData(m_nDifficulty, "gameplay", "Difficulty");
	ReadData(m_nMaxLives, "gameplay", "MaxLives");

	// engine options
	ReadData(m_szLuaPath, MAX_PATH, "engine", "LuaPath" );
	ReadData(m_szShaderPath, MAX_PATH, "engine", "ShaderPath" );
	ReadData(m_szTexturePath, MAX_PATH, "engine", "TexturePath" );
	ReadData(m_szLevelLayoutPath, MAX_PATH, "engine", "LevelLayoutPath" );
	ReadData(m_szModelPath, MAX_PATH, "engine", "ModelPath" );
	ReadData(m_szFontPath, MAX_PATH, "engine", "FontPath" );
}

void CConfigLoader::WriteConfig()
{
	// video options
	WriteData(m_bWindowed, "video", "Windowed");
	WriteData(m_nResolutionWidth, "video", "ResolutionWidth");
	WriteData(m_nResolutionHeight, "video", "ResolutionHeight");
	WriteData(m_nAntiAliasLevel, "video", "AntiAlias");
	WriteData(m_bVSync, "video", "VSync");
	WriteData(m_nGammaLevel, "video", "Gamma");

	// sound options
	WriteData(m_nMasterVolume, "sound", "Master");
	WriteData(m_nSFXVolume, "sound", "SFX");
	WriteData(m_nMusicVolume, "sound", "Music");
	WriteData(m_nDialogueVolume,"sound","Dialogue");

	// gameplay options
	WriteData(m_nDifficulty, "gameplay", "Difficulty");
	WriteData(m_nMaxLives, "gameplay", "MaxLives");

	// engine options
	WriteData(m_szLuaPath, "engine", "LuaPath" );
	WriteData(m_szShaderPath, "engine", "ShaderPath" );
	WriteData(m_szTexturePath, "engine", "TexturePath" );
	WriteData(m_szLevelLayoutPath, "engine", "LevelLayoutPath" );
	WriteData(m_szModelPath, "engine", "ModelPath" );
	WriteData(m_szFontPath, "engine", "FontPath" );
}

void CConfigLoader::ReadData(char* pData, int nLength, char* pAppName, char* pKeyName)
{
	GetPrivateProfileString(pAppName, pKeyName, '\0', pData, nLength, m_szSettingsPath);
}

void CConfigLoader::ReadData(int &nVal, char* pAppName, char* pKeyName)
{
	nVal = GetPrivateProfileInt(pAppName, pKeyName, 0, m_szSettingsPath);
}

void CConfigLoader::ReadData(float &fVal, char* pAppName, char* pKeyName)
{
	char buffer[MAX_PATH];
	GetPrivateProfileString(pAppName, pKeyName, '\0', buffer, MAX_PATH, m_szSettingsPath);
	fVal = (float)atof(buffer);
}

void CConfigLoader::ReadData(bool &bVal, char* pAppName, char* pKeyName)
{
	int nVal = GetPrivateProfileInt(pAppName, pKeyName, 0, m_szSettingsPath);
	bVal = nVal ? true : false;
}

void CConfigLoader::WriteData(char* pData, char* pAppName, char* pKeyName)
{
	WritePrivateProfileString(pAppName, pKeyName, pData, m_szSettingsPath);
}

void CConfigLoader::WriteData(int &nVal, char* pAppName, char* pKeyName)
{
	char buffer[MAX_PATH];
	_itoa_s( nVal, buffer, MAX_PATH, 10);
	WritePrivateProfileString(pAppName, pKeyName, buffer, m_szSettingsPath);
}

void CConfigLoader::WriteData(float &fVal, char* pAppName, char* pKeyName)
{
	char buffer[MAX_PATH];
	sprintf_s(buffer, MAX_PATH, "%f", fVal);
	WritePrivateProfileString(pAppName, pKeyName, buffer, m_szSettingsPath);
}

void CConfigLoader::WriteData(bool &bVal, char* pAppName, char* pKeyName)
{
	char buffer[MAX_PATH];
	_itoa_s( bVal ? 1 : 0, buffer, MAX_PATH, 10);
	WritePrivateProfileString(pAppName, pKeyName, buffer, m_szSettingsPath);
}