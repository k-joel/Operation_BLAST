////////////////////////////////////////////////////
//File Name	:	"CGame.h"
//
//
//
//Purpose	:	To encapsulate all game related code
/////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "CGame.h"
#include "../Rendering/CDirect3D.h"
#include "../Rendering/CTextureManager.h"
#include "../Input/CInput.h"
#include "../States/CStateMachine.h"
#include "../States/CGameState.h"
#include "../States/CWinState.h"
#include "../States/CMainMenuState.h"
#include "../States/CLoadState.h"
#include "../States/CIntroState.h"
#include "../Event System/CEventSystem.h"
#include "../Rendering/CShaderManager.h"
#include "../Mesh/CModelManager.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Game Objects/CObjectFactory.h"
#include "../Game Objects/CObjectManager.h"
#include "CConfigLoader.h"
#include "../Debug System/DebugSystem.h"
#include "../fx/CFXManager.h"
#include "../Spacial Tree/CBVH.h"
#include "../fx/CParticleManager.h"

#include <ctime>

CGame::CGame(void)
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pInput = NULL;
	m_pState = NULL;
	m_pWw = NULL;
	m_pSM = NULL;
	m_pRCM = NULL;
	m_pMM = NULL;
	m_pOF = NULL;
	m_pFX = NULL;
	m_pPM = NULL;

	m_nMasterVolume = 50;
	m_nMusicVolume = 100;
	m_nSFXVolume = 70;
	m_nDialogVolume = 70;
}

//Singleton Accessor
CGame *CGame::GetInstance()
{
	//Lazy initialization
	static CGame instance;
	return &instance;
}

void CGame::Initialization(HWND hWnd, HINSTANCE hInstance, const char* szSettingsFile)
{
	m_pCL	= CConfigLoader::GetInstance();
	m_pCL->LoadConfig(szSettingsFile);
	//m_pCL->WriteConfig();

	m_dwTimeStamp = GetTickCount();
	m_dwPreviousTimeStamp = GetTickCount();

	//Get pointers to singletons:
	m_pD3D	= CDirect3D::GetInstance();
	m_pTM	= CTextureManager::GetInstance();
	m_pWw	= CWwiseManager::GetInstance();
	m_pInput	= CInput::GetInstance();
	m_pSM	= CShaderManager::GetInstance();
	m_pRCM = CRenderContextManager::GetInstance();
	m_pMM = CModelManager::GetInstance();
	m_pOF = CObjectFactory::GetInstance();
	m_pOM = CObjectManager::GetInstance();
	m_pFX = CFXManager::GetInstance();
	m_pBVH = CBVH::GetInstance();
	m_pPM = CParticleManager::GetInstance();

	m_bMultiplayer = false;

	//Initialize singletons:
	m_pD3D->InitD3D(hWnd, hInstance, m_pCL->GetResWidth(), m_pCL->GetResHeight(), m_pCL->GetAA(),
		m_pCL->GetWindowed(), m_pCL->GetVSync());
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	m_pInput->Init(hWnd,hInstance);
	m_pSM->InitShaderManager(m_pD3D->GetDirect3DDevice());
	m_pRCM->Initialize();
	m_pFX->InitializeFXManager();

	// Wwise Init
	m_pWw->Init(hWnd);
	m_pInput->AddDevice(new KeyboardDevice(m_pInput, true));
	m_pInput->AddDevice(new XboxGamePadDevice(m_pInput, true, MAX_PLAYERS));

	//Player 1
	//Keyboard
	m_pInput->MapAction(ACT_PRONE,			DEV_KEYBOARD,	IT_ISPRESSED,	DIK_S,0);
	m_pInput->MapAction(ACT_MOVELEFT,		DEV_KEYBOARD,	IT_ISPRESSED,	DIK_A,0);
	m_pInput->MapAction(ACT_MOVERIGHT,		DEV_KEYBOARD,	IT_ISPRESSED,	DIK_D,0);
	m_pInput->MapAction(ACT_RELEASELEFT,	DEV_KEYBOARD,	IT_ISRELEASED,	DIK_A,0);
	m_pInput->MapAction(ACT_RELEASERIGHT,	DEV_KEYBOARD,	IT_ISRELEASED,	DIK_D,0);
	m_pInput->MapAction(ACT_MOVEUP,			DEV_KEYBOARD,	IT_ISPRESSED,	DIK_W,0);
	m_pInput->MapAction(ACT_MOVEDOWN,		DEV_KEYBOARD,	IT_ISPRESSED,	DIK_S,0);
	m_pInput->MapAction(ACT_HOLDLEFT,		DEV_KEYBOARD,	IT_ISDOWN,		DIK_A,0);
	m_pInput->MapAction(ACT_HOLDRIGHT,		DEV_KEYBOARD,	IT_ISDOWN,		DIK_D,0);
	m_pInput->MapAction(ACT_HOLDUP,			DEV_KEYBOARD,	IT_ISDOWN,		DIK_W,0);
	m_pInput->MapAction(ACT_HOLDDOWN,		DEV_KEYBOARD,	IT_ISDOWN,		DIK_S,0);
	m_pInput->MapAction(ACT_JUMP,			DEV_KEYBOARD,	IT_ISDOWN,		DIK_SPACE,0);
	m_pInput->MapAction(ACT_SHOOT,			DEV_KEYBOARD,	IT_ISDOWN,		DIK_J,0);
	m_pInput->MapAction(ACT_RELEASESHOOT,	DEV_KEYBOARD,	IT_ISRELEASED,	DIK_J,0);
	m_pInput->MapAction(ACT_ALTFIRE,		DEV_KEYBOARD,	IT_ISDOWN,		DIK_K,0);
	m_pInput->MapAction(ACT_ULTFIRE,		DEV_KEYBOARD,	IT_ISDOWN,		DIK_L,0);
	m_pInput->MapAction(ACT_BACK,			DEV_KEYBOARD,	IT_ISPRESSED,	DIK_ESCAPE,0);
	m_pInput->MapAction(ACT_ACCEPT,			DEV_KEYBOARD,	IT_ISPRESSED,	DIK_RETURN,0);
	m_pInput->MapAction(ACT_PAUSE,			DEV_KEYBOARD,   IT_ISPRESSED,	DIK_P,0);
	m_pInput->MapAction(ACT_LDASH,			DEV_KEYBOARD,   IT_ISPRESSED,	DIK_Q,0);
	m_pInput->MapAction(ACT_RDASH,			DEV_KEYBOARD,   IT_ISPRESSED,	DIK_E,0);
	//Gamepad
	m_pInput->MapAction(ACT_ACCEPT,			DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::A_BUTTON,0);
	m_pInput->MapAction(ACT_MOVEUP,			DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_UP,0);
	m_pInput->MapAction(ACT_MOVEDOWN,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_DOWN,0);
	m_pInput->MapAction(ACT_RELEASELEFT,	DEV_GAMEPAD,	IT_ISRELEASED,	XboxGamePadDevice::DPAD_LEFT,0);
	m_pInput->MapAction(ACT_RELEASERIGHT,	DEV_GAMEPAD,	IT_ISRELEASED,	XboxGamePadDevice::DPAD_RIGHT,0);
	m_pInput->MapAction(ACT_MOVELEFT,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_LEFT,0);
	m_pInput->MapAction(ACT_MOVERIGHT,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_RIGHT,0);
	m_pInput->MapAction(ACT_MOVEX,			DEV_GAMEPAD,	IT_MOVEX,		XboxGamePadDevice::LEFT_THUMB,0);
	m_pInput->MapAction(ACT_MOVEY,			DEV_GAMEPAD,	IT_MOVEY,		XboxGamePadDevice::LEFT_THUMB,0);
	m_pInput->MapAction(ACT_JUMP,			DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::A_BUTTON,0);
	m_pInput->MapAction(ACT_SHOOT,			DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::X_BUTTON,0);
	m_pInput->MapAction(ACT_RELEASESHOOT,	DEV_GAMEPAD,	IT_ISRELEASED,	XboxGamePadDevice::X_BUTTON,0);
	m_pInput->MapAction(ACT_ALTFIRE,		DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::B_BUTTON,0);
	m_pInput->MapAction(ACT_ULTFIRE,		DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::Y_BUTTON,0);
	m_pInput->MapAction(ACT_PAUSE,			DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::START,0);
	m_pInput->MapAction(ACT_MENUBACK,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::B_BUTTON,0);
	m_pInput->MapAction(ACT_LDASH,			DEV_GAMEPAD,   IT_ISPRESSED,	XboxGamePadDevice::LEFT_BUTTON,0);
	m_pInput->MapAction(ACT_RDASH,			DEV_GAMEPAD,   IT_ISPRESSED,	XboxGamePadDevice::RIGHT_BUTTON,0);

	//Player 2
	m_pInput->MapAction(ACT_ACCEPT,			DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::A_BUTTON,1);
	m_pInput->MapAction(ACT_MOVEUP,			DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_UP,1);
	m_pInput->MapAction(ACT_MOVEDOWN,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_DOWN,1);
	m_pInput->MapAction(ACT_RELEASELEFT,	DEV_GAMEPAD,	IT_ISRELEASED,	XboxGamePadDevice::DPAD_LEFT,1);
	m_pInput->MapAction(ACT_RELEASERIGHT,	DEV_GAMEPAD,	IT_ISRELEASED,	XboxGamePadDevice::DPAD_RIGHT,1);
	m_pInput->MapAction(ACT_MOVELEFT,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_LEFT,1);
	m_pInput->MapAction(ACT_MOVERIGHT,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::DPAD_RIGHT,1);
	m_pInput->MapAction(ACT_MOVEX,			DEV_GAMEPAD,	IT_MOVEX,		XboxGamePadDevice::LEFT_THUMB,1);
	m_pInput->MapAction(ACT_MOVEY,			DEV_GAMEPAD,	IT_MOVEY,		XboxGamePadDevice::LEFT_THUMB,1);
	m_pInput->MapAction(ACT_JUMP,			DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::A_BUTTON,1);
	m_pInput->MapAction(ACT_SHOOT,			DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::X_BUTTON,1);
	m_pInput->MapAction(ACT_RELEASESHOOT,	DEV_GAMEPAD,	IT_ISRELEASED,	XboxGamePadDevice::X_BUTTON,1);
	m_pInput->MapAction(ACT_ALTFIRE,		DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::B_BUTTON,1);
	m_pInput->MapAction(ACT_ULTFIRE,		DEV_GAMEPAD,	IT_ISDOWN,		XboxGamePadDevice::Y_BUTTON,1);
	m_pInput->MapAction(ACT_PAUSE,			DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::START,1);
	m_pInput->MapAction(ACT_MENUBACK,		DEV_GAMEPAD,	IT_ISPRESSED,	XboxGamePadDevice::B_BUTTON,1);
	m_pInput->MapAction(ACT_LDASH,			DEV_GAMEPAD,   IT_ISPRESSED,	XboxGamePadDevice::LEFT_BUTTON,1);
	m_pInput->MapAction(ACT_RDASH,			DEV_GAMEPAD,   IT_ISPRESSED,	XboxGamePadDevice::RIGHT_BUTTON,1);

	srand(unsigned int(time(0)));

	//// Sound Stuff
	//CWwiseManager::GetInstance()->SetMXVolume((float)m_nMusicVolume);
	//CWwiseManager::GetInstance()->SetFXVolume((float)m_nSFXVolume);
	//CWwiseManager::GetInstance()->SetDXVolume((float)m_nDialogVolume);

	//Debug system init
	CDebugSystem::GetInstance()->Init();

	//Loading Volumes
	m_nMasterVolume = CConfigLoader::GetInstance()->GetMasterVolume();
	m_nDialogVolume = CConfigLoader::GetInstance()->GetDialogueVolume();
	m_nSFXVolume = CConfigLoader::GetInstance()->GetSFXVolume();
	m_nMusicVolume = CConfigLoader::GetInstance()->GetMusicVolume();

	CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
	CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
	CWwiseManager::GetInstance()->SetDXVolume((float)CGame::GetInstance()->GetDialogVolume());
	CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());
	CWwiseManager::GetInstance()->SetListenerScale(0, 3);

	//Loading Difficulty
	m_nDifficulty = CConfigLoader::GetInstance()->GetDifficulty();

	// Keep this code at the bottom
	//Set the starting State to Main Menu
	m_pState = CStateSystem::GetInstance();
	m_pState->AddState(CIntroState::GetInstance());
	//ChangeState(CGamePlayState::GetInstance());
}

void CGame::Shutdown()
{
	//Unacquire in the opposite order
	if(m_pSM) { m_pSM->ShutdownShaderManager(); m_pSM = NULL; }
	if(m_pInput) { 	m_pInput->Shutdown(); m_pInput = NULL; }
	if(m_pTM)	{ m_pTM->ShutdownTextureManager(); m_pTM = NULL; }
	if(m_pD3D)	{ m_pD3D->ShutdownD3D(); m_pD3D = NULL; }
	if(m_pState) { m_pState->Shutdown(); m_pState = NULL; }
	if(m_pWw)	{	m_pWw->Shutdown();	m_pWw = NULL; }
	if(m_pMM)	{ m_pMM->ShutdownModelManager(); m_pMM = NULL; }
	if(m_pFX)	{ m_pFX->ShutdownFXManager(); m_pFX = NULL; }
	if(m_pBVH) { m_pBVH->ShutdownBVH(); m_pBVH = NULL; }
	if(m_pPM)	{m_pPM->ShutdownParticleManager(); m_pPM = NULL; }

	// shutdown last
	if(m_pOM)	{ m_pOM->RemoveEverything(); m_pOM->DeleteInstance(); m_pOM = NULL; }
	if(m_pOF)	{ m_pOF->ShutdownObjectFactory(); m_pOF = NULL; }
}

bool CGame::Main()
{
	//1.Input
	if(Input() == false)
		return false;

	//2.Update
	DWORD dwStartTimeStamp = GetTickCount();
	m_fElapsedTime = (float)(dwStartTimeStamp - m_dwPreviousTimeStamp) / 1000.0f;
	m_dwPreviousTimeStamp = dwStartTimeStamp;

	if(m_fElapsedTime >= 0.25f)
		m_fElapsedTime = 0.25f;

	if(!m_bIsPaused)
		Update(m_fElapsedTime);

	//3.Draw
	Render();

	return true;
}

//1.Input
bool CGame::Input(void)
{
	KeyboardDevice * keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	//full screen toggle alt + enter
	if(keyboard->KeyPressed(DIK_RETURN) && (keyboard->KeyDown(DIK_LALT) || keyboard->KeyDown(DIK_RALT)))
	{
		m_pCL->SetWindowed(!m_pCL->GetWindowed());
		m_pD3D->ChangeDisplayParam(m_pCL->GetResWidth(), m_pCL->GetResHeight(), m_pCL->GetAA(),
			m_pCL->GetWindowed(), m_pCL->GetVSync());
		return true;
	}

	if(keyboard->KeyPressed(DIK_GRAVE))
	{
		AllocConsole();
		m_bConsole = true;

		// locals
		CONSOLE_SCREEN_BUFFER_INFO  coninfo;
		FILE                       *pFile;
		int                         conHandle;
		HANDLE                      stdHandle;

		// reset console properties
		GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &coninfo );
		coninfo.dwSize.Y = MAX_CONSOLE_BUFFER_LINES;
		coninfo.dwSize.X = MAX_CONSOLE_BUFFER_COLUMNS;
		SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coninfo.dwSize );
		SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), true, &m_rConsoleWindow );

		// redirect STDOUT to console
		stdHandle = GetStdHandle( STD_OUTPUT_HANDLE );
		conHandle = _open_osfhandle( (intptr_t)stdHandle, _O_TEXT );
		pFile = _fdopen( conHandle, "w" );
		*stdout = *pFile;
		setvbuf( stdout, NULL, _IONBF, 0 ); // unbuffered

		// redirect STDIN to console
		stdHandle = GetStdHandle( STD_INPUT_HANDLE );
		conHandle = _open_osfhandle( (intptr_t)stdHandle, _O_TEXT );
		pFile = _fdopen( conHandle, "r" );
		*stdin = *pFile;
		setvbuf( stdin, NULL, _IONBF, 0 ); // unbuffered

		// redirect STDERR to console
		stdHandle = GetStdHandle( STD_ERROR_HANDLE );
		conHandle = _open_osfhandle( (intptr_t)stdHandle, _O_TEXT );
		pFile = _fdopen( conHandle, "w" );
		*stderr = *pFile;
		setvbuf( stderr, NULL, _IONBF, 0 ); // unbuffered

		// route cout, wcout, cin, wcin, wcerr, cerr, wclog & clog as well
		ios::sync_with_stdio();

		char buffer[64];

		for(; ; )
		{
			cin >> buffer;

			if(strcmp(buffer, "TJFarnie") == 0)
			{
				ShellExecute(NULL, "open", "http://facebook.com/", NULL, NULL, SW_SHOWNORMAL);
			}

			if(strcmp(buffer, "TJIsTheBest") == 0)
			{
				m_bGodLike = !m_bGodLike;

				if(m_bGodLike)
					cout << "God Mode: On\n";
				else
					cout << "God Mode: Off\n";
			}

			if(strcmp(buffer, "IHaveThePower") == 0)
			{
				m_bUnlimitedUlt = !m_bUnlimitedUlt;

				if(m_bUnlimitedUlt)
					cout << "Unlimited Ultimate Cheat: On\n";
				else
					cout << "Unlimited Ultimate Cheat: Off\n";
			}

			if(strcmp(buffer, "BeltFed") == 0)
			{
				m_bUnlimitedAmmo = !m_bUnlimitedAmmo;

				if(m_bUnlimitedAmmo)
					cout << "Unlimited Ammo: On\n";
				else
					cout << "Unlimited Ammo: Off\n";
			}

			if(strcmp(buffer, "Kakow") == 0)
			{
				m_bShotgun = true;

				cout << "Shotgun Cheat: On\n";
			}

			if(strcmp(buffer, "Lawnchair") == 0)
			{
				m_bRocketLauncher = true;

				cout << "Rocket Launcher Cheat: On\n";
			}

			if(strcmp(buffer, "Toasty") == 0)
			{
				m_bFlamethrower = true;

				cout << "Flamethrower Cheat: On\n";
			}

			if(strcmp(buffer, "Overload") == 0)
			{
				m_bLightningGun = true;

				cout << "Lightning Gun Cheat: On\n";
			}

			if(strcmp(buffer, "EasyStreet") == 0)
			{
				CEventSystem::GetInstance()->DispatchEvent(*CEventSystem::GetInstance()->FindEvent("WinEvent"), NULL, 2.0f);
			}

			if(strcmp(buffer, "Help") == 0)
			{
				cout << "List of available cheats:\n\n";
				cout << "TJIsTheBest\tGod Mode\n";
				cout << "BeltFed\t\tUnlimited Ammo\n";
				cout << "IHaveThePower\tUnlimited Ultimate\n";
				cout << "Kakow\t\tEquip the Shotgun\n";
				cout << "Overload\tEquip the Lightning Gun\n";
				cout << "Toasty\t\tEquip the Flamethrower\n";
				cout << "Lawnchair\tEquip the Rocket Launcher\n";
				cout << "EasyStreet\tWin a Game Session\n";

				cout << "\nCaps matters.  Because it makes you look proper.\n";
				cout << "Type Exit to leave the console.\n";
			}

			if(strcmp(buffer, "Exit") == 0)
			{
				FreeConsole();
				break;
			}
		}
	}

	return m_pState->Input();
}

//2.Update
void CGame::Update(float fDt)
{
	// dont use additional if checks for pointers we know exist

	// sound
	m_pWw->ProcessSound();

	// input
	m_pInput->Update();

	// gameplay updates
	m_pState->Update(fDt);

	//Processing all the events
	CEventSystem::GetInstance()->ProcessEvents();
}

//3.Draw
void CGame::Render(void)
{
	m_bIsPaused = m_pD3D->Clear(0, 0,0); // returns true if device lost
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();
	//drawing now started

	//rendering here
	// render the last state
	m_pState->Render();

	//end drawing area
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();
}

//void CGame::ChangeState(IGameState *newState)
//{
//	if(this->m_pState)
//		m_pState->Exit();
//
//	m_pState = newState;
//
//	m_pState->Enter();
//}