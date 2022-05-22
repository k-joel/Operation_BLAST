#pragma once
#include "CGame.h"

class CConfigLoader
{
	char m_szSettingsPath[MAX_PATH];

	// video options
	int m_nResolutionWidth;
	int m_nResolutionHeight;
	int m_nAntiAliasLevel;
	bool m_bVSync;
	int m_nGammaLevel;
	bool m_bWindowed;

	// sound options
	int m_nMasterVolume;
	int m_nSFXVolume;
	int m_nMusicVolume;
	int m_nDialogueVolume;

	// gameplay options
	int m_nDifficulty;
	int m_nMaxLives;

	// engine options
	char m_szLuaPath[MAX_PATH];
	char m_szShaderPath[MAX_PATH];
	char m_szTexturePath[MAX_PATH];
	char m_szLevelLayoutPath[MAX_PATH];
	char m_szModelPath[MAX_PATH];
	char m_szFontPath[MAX_PATH];

	// singleton private
	CConfigLoader();
	CConfigLoader& operator=(const CConfigLoader& c);
	CConfigLoader(const CConfigLoader& c);
	~CConfigLoader();

	static CConfigLoader		m_Instance;			// An instance to this class.

public:
	static CConfigLoader *GetInstance(void);

	void LoadConfig(const char* pFile);
	void WriteConfig();

	void ReadData(char* pData, int nLength, char* pAppName, char* pKeyName);
	void ReadData(int &nVal, char* pAppName, char* pKeyName);
	void ReadData(float &fVal, char* pAppName, char* pKeyName);
	void ReadData(bool &bVal, char* pAppName, char* pKeyName);

	void WriteData(char* pData, char* pAppName, char* pKeyName);
	void WriteData(int &nVal, char* pAppName, char* pKeyName);
	void WriteData(float &fVal, char* pAppName, char* pKeyName);
	void WriteData(bool &bVal, char* pAppName, char* pKeyName);

	char* GetPath() { return m_szSettingsPath; }

	// getters
	// video options
	int GetResWidth() { return m_nResolutionWidth; }
	int GetResHeight() { return m_nResolutionHeight; }
	int GetAA() { return m_nAntiAliasLevel; }
	bool GetVSync() { return m_bVSync; }
	int GetGammaLevel() { return m_nGammaLevel; }
	bool GetWindowed() { return m_bWindowed; }

	// sound options
	int GetMasterVolume() { return m_nMasterVolume; }
	int GetSFXVolume() { return m_nSFXVolume; }
	int GetMusicVolume() { return m_nMusicVolume; }
	int GetDialogueVolume() {return m_nDialogueVolume;}

	// gameplay options
	int GetDifficulty() { return m_nDifficulty; }
	int GetMaxLives() { return m_nMaxLives; }

	// engine options
	char* GetLuaPath() { return m_szLuaPath; }
	char* GetShaderPath() { return m_szShaderPath; }
	char* GetTexturePath() { return m_szTexturePath; }
	char* GetLevelLayout() { return m_szLevelLayoutPath; }
	char* GetModelPath() { return m_szModelPath; }
	char* GetFontPath() { return m_szFontPath; }

	// setters
	// video options
	void SetResWidth(int nWidth) { m_nResolutionWidth = nWidth; }
	void SetResHeight(int nHeight) { m_nResolutionHeight = nHeight; }
	void SetAA(int nAA) { m_nAntiAliasLevel = nAA; }
	void SetVSync(bool bVS) { m_bVSync = bVS; }
	void SetGammaLevel(int nGamma) { m_nGammaLevel = nGamma; }
	void SetWindowed(bool bWindowed) { m_bWindowed = bWindowed; }

	// sound options
	void SetMasterVolume(int nVol) { m_nMasterVolume = nVol; }
	void SetSFXVolume(int nVol) { m_nSFXVolume = nVol; }
	void SetMusicVolume(int nVol) { m_nMusicVolume = nVol; }
	void SetDialogueVolume(int nVol) {m_nDialogueVolume = nVol; }

	// gameplay options
	void SetDifficulty(int nDiff) { m_nDifficulty = nDiff; }
	void SetMaxLives(int nLives) { m_nMaxLives = nLives; }

	// engine options
	void SetLuaPath(char* pPath) { strcpy_s(m_szLuaPath, MAX_PATH, pPath); }
	void SetShaderPath(char* pPath) { strcpy_s(m_szShaderPath, MAX_PATH, pPath); }
	void SetTexturePath(char* pPath) { strcpy_s(m_szTexturePath, MAX_PATH, pPath); }
	void SetLevelLayout(char* pPath) { strcpy_s(m_szLevelLayoutPath, MAX_PATH, pPath); }
	void SetModelPath(char* pPath) { strcpy_s(m_szModelPath, MAX_PATH, pPath); }
	void SetFontPath(char* pPath) { strcpy_s(m_szFontPath, MAX_PATH, pPath); }
};