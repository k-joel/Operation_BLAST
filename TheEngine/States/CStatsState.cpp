#include "CStatsState.h"
#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "CGameState.h"
#include "CLoadState.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Core/CGame.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

CStatsState::CStatsState(void)
{
	m_nSelection = 0;
	m_fScale = 0.85f;
	m_nFontX = 150;
	m_nArrowPosition = 120;
	m_nFontOffset = 42;
	m_nFontStart = 150;
	m_nFinalScoreP1 = 0;
	m_nFinalScoreP2 = 0;
}

CStatsState::~CStatsState(void)
{
}

void CStatsState::Enter(void)
{
	m_CBF = CBitMapFont::GetInstance();
	m_CTM = CTextureManager::GetInstance();
	m_CIP = CInput::GetInstance();

	m_fBucket = 0.0;
	m_fTransTimer = 0.0;
	m_nScroll = 0;

	m_nSelection = 0;
	m_nInitialSelection = 0;

	m_nFinalScoreP1 = 0;
	m_nFinalScoreP2 = 0;

	m_nFinalScoreP1 = CGameState::GetInstance()->GetHUD()->GetScore();
	m_nFinalScoreP1 = m_nFinalScoreP1 + (CGameState::GetInstance()->GetHUD()->GetMult() * 500);
	m_nFinalScoreP1 = m_nFinalScoreP1 + (CGameState::GetInstance()->GetHUD()->GetHealth() * 200);
	m_nFinalScoreP1 = m_nFinalScoreP1 + (CGameState::GetInstance()->GetHUD()->GetLife() * 1000);

	m_nFinalScoreP2 = CGameState::GetInstance()->GetHUD2()->GetScore();
	m_nFinalScoreP2 = m_nFinalScoreP2 + (CGameState::GetInstance()->GetHUD2()->GetMult() * 500);
	m_nFinalScoreP2 = m_nFinalScoreP2 + (CGameState::GetInstance()->GetHUD2()->GetHealth() * 200);
	m_nFinalScoreP2 = m_nFinalScoreP2 + (CGameState::GetInstance()->GetHUD2()->GetLife() * 1000);

	m_nP1Tally = 0;
	m_nP2Tally = 0;
	m_nRecursion = 0;

	m_fScoreBucket = 0.0f;
	m_nRenderNext = 0;
	m_bDone = false;

	m_nRenderType = 0;
	m_nWhichPlayer = PLAYER1;
	Player1Initials.clear();
	Player2Initials.clear();
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_SCORETICKER_LOOP, 200);	// AudioID 200 is MENU

	m_Buttons.Initialize(true);

}

bool CStatsState::Input(void)
{
	//KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	switch(m_nRenderType)
	{
	case SCORE:
		{
			if(m_CIP->GetAction(ACT_ACCEPT)||m_CIP->GetAction(ACT_ACCEPT,1))
			{
				if(m_bDone)
				{
					m_nRenderType++;
					m_bDone = false;
				}

				if(!m_bDone && m_nRenderType == SCORE)
				{
					m_nP1Tally = m_nFinalScoreP1;
					m_nP2Tally = m_nFinalScoreP2;
					m_bDone = true;
				}
			}
		break;
		}
	case INITIALS:
		{
			if(!m_bDone && m_nWhichPlayer == PLAYER1)
			{
				static float m_fPrevX = 0.0f;
				static float m_fCurrX = 0.0f;
				m_fPrevX = m_fCurrX;
				m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
				if(m_fCurrX >= 32765 && m_fPrevX != m_fCurrX)
				{
					m_nInitialSelection++;
						CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
				}
				if(m_fCurrX <= -32765 && m_fPrevX != m_fCurrX)
				{
					m_nInitialSelection--;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
				}

				if(m_CIP->GetAction(ACT_MOVELEFT))
				{
					m_nInitialSelection--;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
				}

				if(m_CIP->GetAction(ACT_MOVERIGHT))
				{
					m_nInitialSelection++;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
				}
				
				if(m_nInitialSelection > 27)
				{
					m_nInitialSelection = 0;
				}

				if(m_nInitialSelection < 0)
				{
					m_nInitialSelection = 27;
				}

				if(m_CIP->GetAction(ACT_ACCEPT))
				{
					AddInitials(0);
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				}
			}
			else if(!m_bDone && m_nWhichPlayer == PLAYER2)
			{
				static float m_fPrevX = 0.0f;
				static float m_fCurrX = 0.0f;
				m_fPrevX = m_fCurrX;
				m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX,1);
				if(m_fCurrX >= 32765 && m_fPrevX != m_fCurrX)
				{
					m_nInitialSelection++;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
				}
				if(m_fCurrX <= -32765 && m_fPrevX != m_fCurrX)
				{
					m_nInitialSelection--;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
				}
				if(m_CIP->GetAction(ACT_MOVELEFT,1))
				{
					m_nInitialSelection++;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
				}

				if(m_CIP->GetAction(ACT_MOVERIGHT,1))
				{
					m_nInitialSelection--;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
				}

				if(m_nInitialSelection > 27)
				{
					m_nInitialSelection = 0;
				}

				if(m_nInitialSelection < 0)
				{
					m_nInitialSelection = 27;
				}

				if(m_CIP->GetAction(ACT_ACCEPT,1))
				{
					AddInitials(1);
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				}
			}

			if(m_bDone)
			{				
				static float m_fPrevX = 0.0f;
			static float m_fCurrX = 0.0f;
			m_fPrevX = m_fCurrX;
			m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
			if(m_fCurrX >= 32765 && m_fPrevX != m_fCurrX)
			{
				m_nSelection++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			}
			if(m_fCurrX <= -32765 && m_fPrevX != m_fCurrX)
			{
				m_nSelection--;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}
				
				if(m_CIP->GetAction(ACT_MOVELEFT) || m_CIP->GetAction(ACT_MOVELEFT,1))
				{
					--m_nSelection;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200); // AudioID 200 is MENU
				}
				if(m_CIP->GetAction(ACT_MOVERIGHT) ||  m_CIP->GetAction(ACT_MOVERIGHT,1))
				{
					++m_nSelection;
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200); // AudioID 200 is MENU
				}

			
					if(m_nSelection < 0)
						m_nSelection = 0;

					if(m_nSelection > 1)
						m_nSelection = 1;
				

				if((m_CIP->GetAction(ACT_ACCEPT)|| m_CIP->GetAction(ACT_ACCEPT,1)) && m_nRenderType == INITIALS)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);	// AudioID 200 is MENU
					switch(m_nSelection)
					{
					case 0:
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
					CStateSystem::GetInstance()->ChangeBaseState(CGameState::GetInstance());
						break;
					case 1:
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
					CStateSystem::GetInstance()->ChangeBaseState(CMainMenuState::GetInstance());
						break;
					}
				}
			}
				break;
		}
	}
		
	return true;
}

void CStatsState::Update(float fElapsedTime)
{

	m_fScoreBucket += fElapsedTime;

	if(m_nRenderType == SCORE)
	{
		if(m_fScoreBucket > 0.025f)
		{
			if(m_nP1Tally < m_nFinalScoreP1)
			{
				m_nP1Tally += 1000;
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_COINPICKUP, 200);
			}
			
			if(m_nP2Tally < m_nFinalScoreP2)
			{
				m_nP2Tally += 1000;
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_COINPICKUP, 200);
			}

			m_fScoreBucket = 0.0f;
		}

		if(m_nP1Tally > m_nFinalScoreP1)
		{
			m_nP1Tally = m_nFinalScoreP1;
		}

		if(m_nP2Tally > m_nFinalScoreP2)
		{
			m_nP2Tally = m_nFinalScoreP2;
		}

		if(m_nP1Tally == m_nFinalScoreP1 && m_nP2Tally == m_nFinalScoreP2)
		{
			m_bDone = true;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_SCORETICKER_LOOP, 200);
		}
	}

	if(m_nRenderType == INITIALS)
	{
		if(Player1Initials.size() >= 3 && CGame::GetInstance()->GetMutliplayer())
		{
			m_nWhichPlayer = PLAYER2;

			if(Player2Initials.size() >= 3 && !m_bDone)
			{
				SaveScore(PLAYER1);
				SaveScore(PLAYER2);
				m_bDone = true;
			}
		}
		else if(Player1Initials.size() >= 3 && !m_bDone)
		{
			SaveScore(PLAYER1);
			m_bDone = true;
		}
	}

}

void CStatsState::Render(void)
{
	
	switch(m_nRenderType)
	{
	case SCORE:
		RenderPlayerScore();
		break;
	case INITIALS:
		RenderPlayerInitials();
		break;
	}
}

bool CStatsState::Exit(void)
{
	CLoadState::GetInstance()->WriteHighScore("hello.bin");
	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_06, 200);	// AudioID 200 is MENU
	return false;
}

CStatsState* CStatsState::GetInstance(void)
{
	static CStatsState instance;
	return &instance;
}

void CStatsState::RenderPlayerScore()
{
	D3DXMATRIX m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

		m_CBF->Print("FINAL SCORE",300,110,false,false,1.0);

		//Player 1 Stats
		m_CBF->Print("Player 1",m_nFontX,m_nFontStart,false,false);

		m_CBF->Print("Score",m_nFontX,m_nFontStart+m_nFontOffset,false,false,m_fScale);
		sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD()->GetScore());
		m_CBF->Print(szStats,m_nFontX + 100,m_nFontStart+m_nFontOffset*2,true);

		m_CBF->Print("Multiplier",m_nFontX,m_nFontStart+m_nFontOffset*3,false,false,m_fScale);
		sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD()->GetMult()*500);
		m_CBF->Print(szStats,m_nFontX + 100,m_nFontStart+m_nFontOffset*4,true);

		m_CBF->Print("Health",m_nFontX,m_nFontStart+(m_nFontOffset*5),false,false,m_fScale);
		sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD()->GetHealth()*200);
		m_CBF->Print(szStats,m_nFontX + 100,m_nFontStart+(m_nFontOffset*6),true);

		m_CBF->Print("Lives",m_nFontX,m_nFontStart+(m_nFontOffset*7),false,false,m_fScale);
		sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD()->GetLife()*1000);
		m_CBF->Print(szStats,m_nFontX + 100,m_nFontStart+(m_nFontOffset*8),true);

		m_CBF->Print("Final Score",m_nFontX,m_nFontStart+(m_nFontOffset*9),false,false,m_fScale);
		sprintf_s(szStats, sizeof(szStats),"%i",m_nP1Tally);
		m_CBF->Print(szStats,m_nFontX + 100,m_nFontStart+(m_nFontOffset*10),true);

		//Player 2 Stats
		if(CGame::GetInstance()->GetMutliplayer())
		{
			m_CBF->Print("Player 2",m_nFontX+400,m_nFontStart,false,false);

			m_CBF->Print("Score",m_nFontX+400,m_nFontStart+m_nFontOffset,false,false,m_fScale);
			sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD2()->GetScore());
			m_CBF->Print(szStats,m_nFontX + 500,m_nFontStart+m_nFontOffset*2,true);

			m_CBF->Print("Multiplier",m_nFontX+400,m_nFontStart+m_nFontOffset*3,false,false,m_fScale);
			sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD2()->GetMult()*500);
			m_CBF->Print(szStats,m_nFontX + 500,m_nFontStart+m_nFontOffset*4,true);

			m_CBF->Print("Health",m_nFontX+400,m_nFontStart+(m_nFontOffset*5),false,false,m_fScale);
			sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD2()->GetHealth()*200);
			m_CBF->Print(szStats,m_nFontX + 500,m_nFontStart+(m_nFontOffset*6),true);

			m_CBF->Print("Lives",m_nFontX+400,m_nFontStart+(m_nFontOffset*7),false,false,m_fScale);
			sprintf_s(szStats, sizeof(szStats),"%i",CGameState::GetInstance()->GetHUD2()->GetLife()*1000);
			m_CBF->Print(szStats,m_nFontX + 500,m_nFontStart+(m_nFontOffset*8),true);

			m_CBF->Print("Final Score",m_nFontX+400,m_nFontStart+(m_nFontOffset*9),false,false,m_fScale);
			sprintf_s(szStats, sizeof(szStats),"%i",m_nP2Tally);
			m_CBF->Print(szStats,m_nFontX + 500,m_nFontStart+(m_nFontOffset*10),true);
		}

		m_Buttons.Render(m_mxViewProj,false,false,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
		
}

void CStatsState::RenderPlayerStats()
{
		m_CBF->Print("Press 'Accept' to Continue...",250,600,false,false,m_fScale);
}

void CStatsState::RenderPlayerInitials()
{
	D3DXMATRIX m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

		char x[4];
		sprintf_s(x,2,"%c",65+m_nInitialSelection);
		m_CBF->Print(x,400,300,false,false,2.0);

		if(m_nWhichPlayer == PLAYER1)
		m_CBF->Print("Enter your intials Player 1",200,200,false,false,m_fScale);
		else if(m_nWhichPlayer == PLAYER2)
		m_CBF->Print("Enter your intials Player 2",200,200,false,false,m_fScale);

		//m_CBF->Print("
		m_CBF->Print("Player 1",200,400,false,false,m_fScale);
		if(!Player1Initials.empty())
		{
			m_CBF->Print(Player1Initials.c_str(),200,450,false,false,m_fScale);
		}

		if(CGame::GetInstance()->GetMutliplayer())
		{
			m_CBF->Print("Player 2",400,400,false,false,m_fScale);
			if(!Player2Initials.empty())
			{
				m_CBF->Print(Player2Initials.c_str(),400,450,false,false,m_fScale);
			}
		}

		m_CBF->Print("New Game",200,560,false,false,m_fScale);
		m_CBF->Print("Quit",500,560,false,false,m_fScale);

		if(m_bDone)
		{
			if(m_nSelection == 0)
			{
				m_CBF->Print("=>",150,560,false,false,m_fScale);
			}
			else 
			{
				m_CBF->Print("=>",450,560,false,false,m_fScale);
			}
		}

		m_Buttons.Render(m_mxViewProj,false,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));

}

void CStatsState::AddInitials(int player)
{
	if(player == PLAYER1)
	{
		if(Player1Initials.size() < 3)
		Player1Initials.push_back(65+m_nInitialSelection);
	}
	else
	{
		if(Player2Initials.size() < 3)
		Player2Initials.push_back(65+m_nInitialSelection);
	}
}

void CStatsState::SaveScore(int player)
{
	for(unsigned int i = 0; i < 10; i++)
	{
		if(player == PLAYER1)
		{
			if(CLoadState::GetInstance()->GetHighScore(i)->GetScore() < m_nP1Tally)
			{
				int x = i;
					SavePlayer(2,x);
				SavePlayer(PLAYER1,i);
				break;
			}
		}
		else if(player == PLAYER2)
		{
			if(CLoadState::GetInstance()->GetHighScore(i)->GetScore() < m_nP2Tally)
			{
				int x = i;
				SavePlayer(2,x);
				SavePlayer(PLAYER2,i);
				break;
			}
		}

	}
}

void CStatsState::SavePlayer(int player, int ID)
{
	if(player == PLAYER1)
	{	
		CLoadState::GetInstance()->GetHighScore(ID)->SetName(Player1Initials);
		CLoadState::GetInstance()->GetHighScore(ID)->SetScore(m_nP1Tally);
	}
	else if(player == PLAYER2)
	{
		CLoadState::GetInstance()->GetHighScore(ID)->SetName(Player2Initials);
		CLoadState::GetInstance()->GetHighScore(ID)->SetScore(m_nP2Tally);
	}
	else
	{
		if(m_nRecursion < 8)
		{
			m_nRecursion++;
			SavePlayer(2,m_nRecursion);
		}
		CLoadState::GetInstance()->GetHighScore(ID+1)->SetName(CLoadState::GetInstance()->GetHighScore(ID)->GetName());
		CLoadState::GetInstance()->GetHighScore(ID+1)->SetScore(CLoadState::GetInstance()->GetHighScore(ID)->GetScore());
	}
}