#include "CLoadState.h"
#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "CHighScoreState.h"
#include "../Rendering/CTextureManager.h"
#include "../Rendering/CShaderManager.h"
#include "../Font/CBitMapFont.h"
#include "../Mesh/CModelManager.h"
#include "../Game Objects/layoutImporter.h"
#include "../Game Objects/CObjectManager.h"
#include "../Spacial Tree/CBVH.h"
#include "../FX/CParticleManager.h"

#include <fstream>

CLoadState::CLoadState(void)
{
	m_pBF = NULL;
	m_pTM = NULL;
	m_pMM = NULL;

	m_nCurrentStage = 0;

	m_rLoadbarBGRect.left = 0;
	m_rLoadbarBGRect.top = 0;
	m_rLoadbarBGRect.right = 506;
	m_rLoadbarBGRect.bottom = 84;

	m_rLoadbarRect.left = 0;
	m_rLoadbarRect.top = 0;
	m_rLoadbarRect.right = 468;
	m_rLoadbarRect.bottom = 48;
}

CLoadState::~CLoadState(void)
{
}

void CLoadState::Enter(void)
{
	m_pBF = CBitMapFont::GetInstance();
	m_pTM = CTextureManager::GetInstance();
	m_pMM = CModelManager::GetInstance();
	m_pPM = CParticleManager::GetInstance();

	m_mImageIDMap["GPLogo"] = m_pTM->LoadTexture("Resources/Textures/GPGlogo.png");
	m_mImageIDMap["Threewaytie"] = m_pTM->LoadTexture("Resources/Textures/Threewaytie.png");
	m_mImageIDMap["IntroN2"] = m_pTM->LoadTexture("Resources/Textures/nslogo.png");
	m_mImageIDMap["FontA"] = m_pTM->LoadTexture("Resources/Fonts/TestFont8.png");
	m_mImageIDMap["FontASel"] = m_pTM->LoadTexture("Resources/Fonts/TestFont8.png");
	m_mImageIDMap["2"] = m_pTM->LoadTexture("Resources/Textures/2.png");

	m_pBF->LoadKerningInfo("Resources/Fonts/TestFont8.bmf",FONT_A);
	m_pBF->SetImageID(m_mImageIDMap["FontA"], FONT_A);
	m_pBF->SetSelectedID(m_mImageIDMap["FontASel"], FONT_A);
	m_pBF->SetCurFont(FONT_A);
}

bool CLoadState::Input(void)
{
	return true;
}

void CLoadState::Update(float fElapsedTime)
{
	m_rLoadbarRect.right = int((float)m_rLoadbarRect.left + 468.0f*((float)m_nCurrentStage/(float)NUM_ASSETS));

	//////////////////////////////////////////////////////////////////////////
	// REMEMBER TO INCREMENT THE NUM_ASSETS DEFINE WHEN YOU ADD MORE STAGES
	//////////////////////////////////////////////////////////////////////////
	switch(m_nCurrentStage)
	{
	case 0:
		//keep this case empty, it just needs to set the name of the next stage
		//////////////////////////////////////////////////////////////////////////
		m_mImageIDMap["2"] = m_pTM->LoadTexture("Resources/Textures/2.png");
		m_mImageIDMap["shootEverything"] = m_pTM->LoadTexture("Resources/Textures/shootem.png");
		m_mImageIDMap["moveForward"] = m_pTM->LoadTexture("Resources/Textures/go.png");
		m_mImageIDMap["splitUp"] = m_pTM->LoadTexture("Resources/Textures/split.png");
		m_mImageIDMap["regroup"] = m_pTM->LoadTexture("Resources/Textures/regroup.png");
		m_mImageIDMap["player1win"] = m_pTM->LoadTexture("Resources/Textures/Victory_Player1.png");
		m_mImageIDMap["player2win"] = m_pTM->LoadTexture("Resources/Textures/Victory_Player2.png");
		m_mImageIDMap["Wwise"] = m_pTM->LoadTexture("Resources/Textures/Wwise.png");
		m_mImageIDMap["Lua"] = m_pTM->LoadTexture("Resources/Textures/Lua.png");

		m_szCurAssetName = "Sound Effects";
		break;
	case 1:
		// LOAD SFX HERE
		m_mImageIDMap["3"] = m_pTM->LoadTexture("Resources/Textures/3.png");
		m_mImageIDMap["fade"] = m_pTM->LoadTexture("Resources/Textures/fade.tga");
		// Credit Texture
		m_mImageIDMap["credit"] = m_pTM->LoadTexture("Resources/Textures/CreditScreen.png");
			//How To Textures
		m_mImageIDMap["Cover"] = m_pTM->LoadTexture("Resources/Textures/Barron'sFieldJournal.png");
		m_mImageIDMap["Enemies"] = m_pTM->LoadTexture("Resources/Textures/EnemiesAndPointValues.png");
		m_mImageIDMap["SObjective"] = m_pTM->LoadTexture("Resources/Textures/SinglePlayerObjective.png");
		m_mImageIDMap["MObjective"] = m_pTM->LoadTexture("Resources/Textures/MultiplayerObjective.png");
		m_mImageIDMap["Weapons"] = m_pTM->LoadTexture("Resources/Textures/weapons.png");
		m_mImageIDMap["HUD"] = m_pTM->LoadTexture("Resources/Textures/HUD.png");
		m_mImageIDMap["GamePad"] = m_pTM->LoadTexture("Resources/Textures/GamePadControls.png");
		m_mImageIDMap["KeyBoard"] = m_pTM->LoadTexture("Resources/Textures/KeyBoardControls.png");
		m_mImageIDMap["FT"] = m_pTM->LoadTexture("Resources/Textures/FlameThrower.png");
		m_mImageIDMap["RL"] = m_pTM->LoadTexture("Resources/Textures/RocketLauncher.png");
		m_mImageIDMap["SG"] = m_pTM->LoadTexture("Resources/Textures/Shotgun.png");
		m_mImageIDMap["LG"] = m_pTM->LoadTexture("Resources/Textures/LightningGun.png");

		//////////////////////////////////////////////////////////////////////////
		m_szCurAssetName = "Music";
		break;
	case 2:
		// LOAD MUSIC HERE

		//////////////////////////////////////////////////////////////////////////
		//split alt textures for models
		m_mImageIDMap["4"] = m_pTM->LoadTexture("Resources/Textures/4.png");
		m_mImageIDMap["Assaulter_color2"] = m_pTM->LoadTexture("Resources/Textures/Assaulter_color2.png");
		m_mImageIDMap["Striker_color2"] = m_pTM->LoadTexture("Resources/Textures/Striker_color2.png");
		m_mImageIDMap["Helios_color2"] = m_pTM->LoadTexture("Resources/Textures/Helios_color2.png");
		m_mImageIDMap["Hit"] = m_pTM->LoadTexture("Resources/Textures/Hit.png");

		m_szCurAssetName = "Images";
		break;
	case 3:
		// LOAD IMAGES HERE
		//New HUD
		m_mImageIDMap["5"] = m_pTM->LoadTexture("Resources/Textures/5.png");
		m_mModelIDMap["Dial_MultiShape"] = m_pMM->LoadModel("Resources/Models/Dial_MultiShape");
		m_mModelIDMap["Dial_scoreShape"] = m_pMM->LoadModel("Resources/Models/Dial_scoreShape");
		m_mModelIDMap["Health_heartsShape"] = m_pMM->LoadModel("Resources/Models/Health_heartsShape");
		m_mModelIDMap["Health_heartP2Shape"] = m_pMM->LoadModel("Resources/Models/Health_heartP2Shape");
		m_mModelIDMap["HUD_specialShape"] = m_pMM->LoadModel("Resources/Models/HUD_specialShape");
		m_mModelIDMap["Lives_0xShape"] = m_pMM->LoadModel("Resources/Models/Lives_0xShape");
		m_mModelIDMap["Lives_1xShape"] = m_pMM->LoadModel("Resources/Models/Lives_1xShape");
		m_mModelIDMap["Lives_2xShape"] = m_pMM->LoadModel("Resources/Models/Lives_2xShape");
		m_mModelIDMap["Lives_3xShape"] = m_pMM->LoadModel("Resources/Models/Lives_3xShape");
		m_mModelIDMap["HUD_mainP1Shape"] = m_pMM->LoadModel("Resources/Models/HUD_mainP1Shape");
		m_mModelIDMap["Weapon_flamethrowerShape"] = m_pMM->LoadModel("Resources/Models/Weapon_flamethrowerShape");
		m_mModelIDMap["Weapon_lightningShape"] = m_pMM->LoadModel("Resources/Models/Weapon_lightningShape");
		m_mModelIDMap["Weapon_machinegunShape"] = m_pMM->LoadModel("Resources/Models/Weapon_machinegunShape");
		m_mModelIDMap["Weapon_RocketlauncherShape"] = m_pMM->LoadModel("Resources/Models/Weapon_RocketlauncherShape");
		m_mModelIDMap["Weapon_shotgunShape"] = m_pMM->LoadModel("Resources/Models/Weapon_shotgunShape");
		m_mModelIDMap["HUD_mainP2Shape"] = m_pMM->LoadModel("Resources/Models/HUD_mainP2Shape");
		m_mModelIDMap["HUD_specialP2Shape"] = m_pMM->LoadModel("Resources/Models/HUD_specialP2Shape");
		m_mModelIDMap["Weapon_flamethrowerP2Shape"] = m_pMM->LoadModel("Resources/Models/Weapon_flamethrowerP2Shape");
		m_mModelIDMap["Weapon_lightningP2Shape"] = m_pMM->LoadModel("Resources/Models/Weapon_lightninggunP2Shape");
		m_mModelIDMap["Weapon_machinegunP2Shape"] = m_pMM->LoadModel("Resources/Models/Weapon_machinegunP2Shape");
		m_mModelIDMap["Weapon_rocketlauncherP2Shape"] = m_pMM->LoadModel("Resources/Models/Weapon_rocketlauncherP2Shape");
		m_mModelIDMap["Weapon_shotgunP2Shape"] = m_pMM->LoadModel("Resources/Models/Weapon_shotgunP2Shape");
		m_mModelIDMap["Portrait_barronShape"] = m_pMM->LoadModel("Resources/Models/Portrait_barronShape");
		m_mModelIDMap["Portrait_claireShape"] = m_pMM->LoadModel("Resources/Models/Portrait_claireShape");
		m_mModelIDMap["Portrait_deathShape"] = m_pMM->LoadModel("Resources/Models/Portrait_deathShape");
		m_mModelIDMap["Weapon_deathShape"] = m_pMM->LoadModel("Resources/Models/Weapon_deathShape");
		m_mModelIDMap["P1Ultimate"] = m_pMM->LoadModel("Resources/Models/P1_Ultimate_BarRedShape");
		m_mModelIDMap["P2Ultimate"] = m_pMM->LoadModel("Resources/Models/P2_Ultimate_BarRedShape");
		m_mModelIDMap["REC"] = m_pMM->LoadModel("Resources/Models/RECShape");
		//m_mModelIDMap["Scan"] = m_pMM->LoadModel("Resources/Models/ScanlineShape");
		//////////////////////////////////////////////////////////////////////////
		m_szCurAssetName = "Models";
		break;
	case 4:
			//Menu Models
		m_mImageIDMap["6"] = m_pTM->LoadTexture("Resources/Textures/6.png");
		m_mModelIDMap["GamePadAccept"] = m_pMM->LoadModel("Resources/Models/A_AcceptShape"); //Good
		m_mModelIDMap["GamePadBack"] = m_pMM->LoadModel("Resources/Models/B_BackShape"); //Good
		m_mModelIDMap["GamePadLR"] = m_pMM->LoadModel("Resources/Models/DPad_LeftRightShape"); //Good
		m_mModelIDMap["GamePadUD"] = m_pMM->LoadModel("Resources/Models/DPad_UPDownShape"); //Good
		m_mModelIDMap["KeyAccept"] = m_pMM->LoadModel("Resources/Models/Enter_AcceptShape"); //Good
		m_mModelIDMap["KeyBack"] = m_pMM->LoadModel("Resources/Models/ESC_BackShape"); //Good
		m_mModelIDMap["KeyU"] = m_pMM->LoadModel("Resources/Models/W_UPShape");
		m_mModelIDMap["KeyD"] = m_pMM->LoadModel("Resources/Models/S_DownShape");
		m_mModelIDMap["KeyL"] = m_pMM->LoadModel("Resources/Models/A_LeftShape");
		m_mModelIDMap["KeyR"] = m_pMM->LoadModel("Resources/Models/D_RightShape");
		m_mModelIDMap["PressStart"] = m_pMM->LoadModel("Resources/Models/PressStart_ToJoinWhiteShape");
		m_mModelIDMap["PressStartOffset"] = m_pMM->LoadModel("Resources/Models/PressStart_ToJoinOffsetShape");
		break;
	case 5:
		m_mImageIDMap["7"] = m_pTM->LoadTexture("Resources/Textures/7.png");
		m_mModelIDMap["GamePadAcceptU"] = m_pMM->LoadModel("Resources/Models/A_Accept_UnselectedShape"); //Good
		m_mModelIDMap["GamePadBackU"] = m_pMM->LoadModel("Resources/Models/B_Back_UnselectedShape"); //Good
		m_mModelIDMap["GamePadLRU"] = m_pMM->LoadModel("Resources/Models/DPad_LeftRight_UnselectedShape"); //Good
		m_mModelIDMap["GamePadUDU"] = m_pMM->LoadModel("Resources/Models/DPad_UPDown_UnselectedShape"); //Good
		m_mModelIDMap["KeyAcceptU"] = m_pMM->LoadModel("Resources/Models/Enter_Accept_UnselectedShape"); //Good
		m_mModelIDMap["KeyBackU"] = m_pMM->LoadModel("Resources/Models/ESC_Back_UnselectedShape"); //Good
		m_mModelIDMap["KeyUU"] = m_pMM->LoadModel("Resources/Models/W_UP_UnselectedShape");
		m_mModelIDMap["KeyDU"] = m_pMM->LoadModel("Resources/Models/S_Down_UnselectedShape");
		m_mModelIDMap["KeyLU"] = m_pMM->LoadModel("Resources/Models/A_Left_UnselectedShape");
		m_mModelIDMap["KeyRU"] = m_pMM->LoadModel("Resources/Models/D_Right_UnselectedShape");
		break;
	case 6:
		m_mImageIDMap["8"] = m_pTM->LoadTexture("Resources/Textures/8.png");
		m_mModelIDMap["PressStartU"] = m_pMM->LoadModel("Resources/Models/PressStart_ToJoinWhiteShape");
		m_mModelIDMap["PressStartOffsetU"] = m_pMM->LoadModel("Resources/Models/PressStart_ToJoinOffsetShape");

		//HATS HATS TJ SUCKS HATS
		m_mModelIDMap["Barron_helmetShape"] = m_pMM->LoadModel("Resources/Models/Barron_helmetShape");
		m_mModelIDMap["Claire_hatShape"] = m_pMM->LoadModel("Resources/Models/Claire_hatShape");

		m_mModelIDMap["armyhatShape"] = m_pMM->LoadModel("Resources/Models/armyhatShape");
		m_mModelIDMap["bearhatShape"] = m_pMM->LoadModel("Resources/Models/bearhatShape");
		m_mModelIDMap["cathatShape"] = m_pMM->LoadModel("Resources/Models/cathatShape");
		m_mModelIDMap["cowboyhatShape"] = m_pMM->LoadModel("Resources/Models/cowboyhatShape");
		m_mModelIDMap["fancyhatShape"] = m_pMM->LoadModel("Resources/Models/fancyhatShape");
		m_mModelIDMap["fruithatShape"] = m_pMM->LoadModel("Resources/Models/fruithatShape");
		m_mModelIDMap["narwhalhatShape"] = m_pMM->LoadModel("Resources/Models/narwhalhatShape");
		m_mModelIDMap["marchinghatShape"] = m_pMM->LoadModel("Resources/Models/marchinghatShape");
		m_mModelIDMap["pokehatShape"] = m_pMM->LoadModel("Resources/Models/pokehatShape");
		m_mModelIDMap["pothatShape"] = m_pMM->LoadModel("Resources/Models/pothatShape");
		m_mModelIDMap["sharkhatShape"] = m_pMM->LoadModel("Resources/Models/sharkhatShape");
		m_mModelIDMap["sombrerohatShape"] = m_pMM->LoadModel("Resources/Models/sombrerohatShape");

		//organizing this shit

		//units
		m_mModelIDMap["barron_geoShape"] = m_pMM->LoadModel("Resources/Models/barron_geoShape");
		m_mModelIDMap["Claire_geoShape"] = m_pMM->LoadModel("Resources/Models/Claire_geoShape");
		m_mModelIDMap["robot_geoShape"] = m_pMM->LoadModel("Resources/Models/robot_geoShape");
		m_mModelIDMap["Assaulter_geoShape"] = m_pMM->LoadModel("Resources/Models/Assaulter_geoShape");
		m_mModelIDMap["Helios_geoShape"] = m_pMM->LoadModel("Resources/Models/Helios_geoShape");
		break;
	case 7:
		m_mImageIDMap["9"] = m_pTM->LoadTexture("Resources/Textures/9.png");
	//	m_mModelIDMap["bossTest"] = m_pMM->LoadModel("Resources/Models/bossTest");
		m_mModelIDMap["boss_geoShape"] = m_pMM->LoadModel("Resources/Models/boss_geoShape");

		m_mModelIDMap["boss_bodyShape"] = m_pMM->LoadModel("Resources/Models/boss_bodyShape");
		m_mModelIDMap["boss_conveyorShape"] = m_pMM->LoadModel("Resources/Models/boss_conveyorShape");
		m_mModelIDMap["boss_gun_bigShape"] = m_pMM->LoadModel("Resources/Models/boss_gun_bigShape");
		m_mModelIDMap["boss_gun_bottomShape"] = m_pMM->LoadModel("Resources/Models/boss_gun_bottomShape");
		m_mModelIDMap["boss_gun_topShape"] = m_pMM->LoadModel("Resources/Models/boss_gun_topShape");

		//weapons and bullets
		m_mModelIDMap["ClairesGunShape"] = m_pMM->LoadModel("Resources/Models/ClairesGunShape");
		m_mModelIDMap["BaronsGunShape"] = m_pMM->LoadModel("Resources/Models/BaronsGunShape");
		m_mModelIDMap["bullet_geoShape"] = m_pMM->LoadModel("Resources/Models/bullet_geoShape");

		m_mModelIDMap["FlameThrowerShape"] = m_pMM->LoadModel("Resources/Models/FlameThrowerShape");
		m_mModelIDMap["Flame_Burst"] = m_pMM->LoadModel("Resources/Models/Flame_Burst");
		m_mModelIDMap["Flame_altShape"] = m_pMM->LoadModel("Resources/Models/Flame_altShape");
		m_mModelIDMap["Flame_ultShape"] = m_pMM->LoadModel("Resources/Models/Flame_ultShape");
		break;
	case 8:
		m_mImageIDMap["10"] = m_pTM->LoadTexture("Resources/Textures/10.png");
		m_mModelIDMap["ShotgunShape"] = m_pMM->LoadModel("Resources/Models/ShotgunShape");
		m_mModelIDMap["Shotgun_PelletShape"] = m_pMM->LoadModel("Resources/Models/Shotgun_PelletShape");
		m_mModelIDMap["Shotgun_UltimateShape"] = m_pMM->LoadModel("Resources/Models/Shotgun_UltimateShape");

		m_mModelIDMap["RocketLauncherShape"] = m_pMM->LoadModel("Resources/Models/RocketLauncherShape");
		m_mModelIDMap["RocketShape"] = m_pMM->LoadModel("Resources/Models/RocketShape");

		m_mModelIDMap["LightningGunShape"] = m_pMM->LoadModel("Resources/Models/LightningGunShape");

		m_mModelIDMap["laser_orbShape"] = m_pMM->LoadModel("Resources/Models/laser_orbShape");
		m_mModelIDMap["SmallredlaserShape"] = m_pMM->LoadModel("Resources/Models/SmallredlaserShape");
		m_mModelIDMap["sphereofdoomshape"] = m_pMM->LoadModel("Resources/Models/sphereofdoomshape");
		m_mModelIDMap["MissileShape"] = m_pMM->LoadModel("Resources/Models/MissileShape");
		break;
	case 9:
		//set piece
		m_mImageIDMap["11"] = m_pTM->LoadTexture("Resources/Textures/11.png");
		m_mModelIDMap["victoryFlagShape"] = m_pMM->LoadModel("Resources/Models/victoryFlagShape");
		m_mModelIDMap["CrateShape"] = m_pMM->LoadModel("Resources/Models/CrateShape");
		m_mModelIDMap["barrelexplodeShape"] = m_pMM->LoadModel("Resources/Models/barrelexplodeShape");

		//new flag
		m_mModelIDMap["Helicopter_BodyShape"] = m_pMM->LoadModel("Resources/Models/Helicopter_BodyShape");
		m_mModelIDMap["Helicopter_RearPropellerShape"] = m_pMM->LoadModel("Resources/Models/Helicopter_RearPropellerShape");
		m_mModelIDMap["Helicopter_TopPropellerShape"] = m_pMM->LoadModel("Resources/Models/Helicopter_TopPropellerShape");
		m_mModelIDMap["TiresBackLeftShape"] = m_pMM->LoadModel("Resources/Models/TiresBackLeftShape");
		m_mModelIDMap["TiresBackRightShape"] = m_pMM->LoadModel("Resources/Models/TiresBackRightShape");
		m_mModelIDMap["TiresInFrontShape"] = m_pMM->LoadModel("Resources/Models/TiresInFrontShape");

		//Effect
		m_mModelIDMap["barrel_geoShape"] = m_pMM->LoadModel("Resources/Models/barrel_geoShape");
		m_mModelIDMap["Dirt_explosionAShape"] = m_pMM->LoadModel("Resources/Models/Dirt_explosionAShape");
		m_mModelIDMap["Dirt_explosionBShape"] = m_pMM->LoadModel("Resources/Models/Dirt_explosionBShape");
		m_mModelIDMap["largeexplosionShape"] = m_pMM->LoadModel("Resources/Models/largeexplosionShape");
		m_mModelIDMap["Muzzle_fireShape"] = m_pMM->LoadModel("Resources/Models/Muzzle_fireShape");
		m_mModelIDMap["shotgun_blastShape"] = m_pMM->LoadModel("Resources/Models/shotgun_blastShape");
		m_mModelIDMap["Laser_flashShape"] = m_pMM->LoadModel("Resources/Models/Laser_flashShape");
		m_mModelIDMap["Flame_pilotShape"] = m_pMM->LoadModel("Resources/Models/Flame_pilotShape");
		m_mModelIDMap["shadow_characterShape"] = m_pMM->LoadModel("Resources/Models/shadow_characterShape");
		m_mModelIDMap["P1_Indicator_BlueShape"] = m_pMM->LoadModel("Resources/Models/P1_Indicator_BlueShape");
		m_mModelIDMap["P2_Indicator_RedShape"] = m_pMM->LoadModel("Resources/Models/P2_Indicator_RedShape");
		m_mModelIDMap["XShape"] = m_pMM->LoadModel("Resources/Models/XShape");
		break;
	case 10:
		m_mImageIDMap["12"] = m_pTM->LoadTexture("Resources/Textures/12.png");
		m_mModelIDMap["AssaulterExplosionShape"] = m_pMM->LoadModel("Resources/Models/AssaulterExplosionShape");
		m_mModelIDMap["HeliosExplosionShape"] = m_pMM->LoadModel("Resources/Models/HeliosExplosionShape");
		m_mModelIDMap["StrikerExplosionShape"] = m_pMM->LoadModel("Resources/Models/StrikerExplosionShape");

		m_mModelIDMap["bossLaserFizzleShape"] = m_pMM->LoadModel("Resources/Models/bossLaserFizzleShape");
		m_mModelIDMap["bossLaserShape"] = m_pMM->LoadModel("Resources/Models/bossLaserShape");

		//other
		m_mModelIDMap["PauseMenuScreenShape"] = m_pMM->LoadModel("Resources/Models/PauseMenuScreenShape");
		m_mModelIDMap["VictoryShape"] = m_pMM->LoadModel("Resources/Models/VictoryShape");
		m_mModelIDMap["GameOverShape"] = m_pMM->LoadModel("Resources/Models/GameOverShape");
		m_mModelIDMap["Player_1Shape"] = m_pMM->LoadModel("Resources/Models/Player_1Shape");
		m_mModelIDMap["Player_2Shape"] = m_pMM->LoadModel("Resources/Models/Player_2Shape");

		//currently unused
		m_mModelIDMap["crate_stationary_geoShape"] = m_pMM->LoadModel("Resources/Models/crate_stationary_geoShape");
		m_mModelIDMap["potatocycleShape"] = m_pMM->LoadModel("Resources/Models/potatocycleShape");
		m_mModelIDMap["M60_Shape"] = m_pMM->LoadModel("Resources/Models/M60_Shape");
		m_mModelIDMap["ChargedLaserShape"] = m_pMM->LoadModel("Resources/Models/ChargedLaserShape");
		// LOAD MODELS HERE
		break;
	case 11:

		// Load particles
		m_pPM->LoadParticle("Assaulter_jetpack");
		m_pPM->LoadParticle("Assaulter_land");
		m_pPM->LoadParticle("Blood_splurt");
		m_pPM->LoadParticle("Bullet_dust");
		m_pPM->LoadParticle("Dash_steak");
		m_pPM->LoadParticle("Dust_kickup");
		m_pPM->LoadParticle("Explosion2");
		m_pPM->LoadParticle("Flamethrower_alt");
		m_pPM->LoadParticle("Flamethrower_idle");
		m_pPM->LoadParticle("Flamethrower_nrm2");
		m_pPM->LoadParticle("Flamethrower_ult");
		m_pPM->LoadParticle("HUD_fire");
		m_pPM->LoadParticle("Laser_charge");
		m_pPM->LoadParticle("Laser_charge2");
		m_pPM->LoadParticle("Laser_chargeburst");
		m_pPM->LoadParticle("Leaf_flutter2");
		m_pPM->LoadParticle("Robot_spark2");
		m_pPM->LoadParticle("Shotgun_altA");
		m_pPM->LoadParticle("Shotgun_altB");
		m_pPM->LoadParticle("Shotgun_ult");
		m_pPM->LoadParticle("Flamethrower_windup");
		m_pPM->LoadParticle("Shotgun_windup");
		m_pPM->LoadParticle("Menu_Textclash");
		m_pPM->LoadParticle("Plus_100");
		m_pPM->LoadParticle("Plus_100b");
		m_pPM->LoadParticle("Plus_50");
		m_pPM->LoadParticle("Plus_50b");
		m_pPM->LoadParticle("Plus_200");
		m_pPM->LoadParticle("Plus_200b");
		m_pPM->LoadParticle("BlackSmoke");
		m_pPM->LoadParticle("BigBlackSmoke");
		m_pPM->LoadParticle("BossSmoke");
		m_pPM->LoadParticle("BossSmallSmoke");
		m_pPM->LoadParticle("BossExplosion");
		//m_pPM->LoadParticle("Rain");
		m_pPM->LoadParticle("Plus_win");
		m_pPM->LoadParticle("Plus_winb");

		//Fact:  Kahran is a jerk.
		//Seconded: Thus it must be true.

		// Fact: TJ is always wrong.

		//Alpha Fact: TJ is so wrong, it makes him right.  All the time.
		//Beta Fact: TJ is so wrong, it makes him STILL WRONG.  All the time.
		//Philosoraptor: If TJ is always wrong, and if he says he's wrong, does that make him right?

		/************************************************************************/
		/* Loading the level here                                                */
		/************************************************************************/
		CLayoutImporter::GetInstance()->LoadLayout("Resources/Layout/test.NNlvl");

		// Steps to build a new BVH (Only do this if a new layout needs to be imported)
		// 1. Uncomment build and writebvh and comment out readbvh
		// 2. run the game and quit out
		// 3. Comment out build and writebvh and uncommment readbvh

		//CBVH::GetInstance()->Build(&CObjectManager::GetInstance()->m_vSceneRenderableList,
		//	&CObjectManager::GetInstance()->m_vSceneCollisionList);

		//CBVH::GetInstance()->WriteBVH();
		CBVH::GetInstance()->ReadBVH();

		/***********************************************************************/
		/* Loading HighScores
		/ TJ's name will never be on them because he sucks at our game.
		/ TOO BAD I'M SERIOUSLY LIKE THE BEST EVER AND I WIN AT EVERYTHING.
		/***********************************************************************/

		//LoadHighScore("File.bin");

		//////////////////////////////////////////////////////////////////////////
		m_szCurAssetName = "Done!";
		break;
	case NUM_ASSETS:
		m_nCurrentStage--;
		static float fTimer = 0.0f;
		fTimer += fElapsedTime;
		if(fTimer >= 1.0f)
		{
			CStateSystem::GetInstance()->ChangeBaseState(CMainMenuState::GetInstance());
		}
		break;
	}

	m_nCurrentStage++;
}

void CLoadState::Render(void)
{
	RECT bgRect;
	bgRect.top = 0;
	bgRect.left = 0;
	bgRect.right = 1024;
	bgRect.bottom = 768;
	switch(m_nCurrentStage)
	{
	case 0:
		m_pTM->Draw(m_mImageIDMap["2"],0,0,1,0.85f);
		break;
	case 1:
		m_pTM->Draw(m_mImageIDMap["2"],0,0,1,0.85f);
		break;
	case 2:
		m_pTM->Draw(m_mImageIDMap["3"],0,0,1,0.85f);
		break;
	case 3:
		m_pTM->Draw(m_mImageIDMap["4"],0,0,1,0.85f);
		break;
	case 4:
		m_pTM->Draw(m_mImageIDMap["5"],0,0,1,0.85f);
		break;
	case 5:
			m_pTM->Draw(m_mImageIDMap["6"],0,0,1,0.85f);
			break;
	case 6:
			m_pTM->Draw(m_mImageIDMap["7"],0,0,1,0.85f);
			break;
	case 7:
			m_pTM->Draw(m_mImageIDMap["8"],0,0,1,0.85f);
			break;
	case 8:
			m_pTM->Draw(m_mImageIDMap["9"],0,0,1,0.85f);
			break;
	case 9:
			m_pTM->Draw(m_mImageIDMap["10"],0,0,1,0.85f);
			break;
	case 10:
			m_pTM->Draw(m_mImageIDMap["11"],0,0,1,0.85f);
			break;
	case 11:
			m_pTM->Draw(m_mImageIDMap["11"],0,0,1,0.85f);
			break;
	case NUM_ASSETS:
		m_pTM->Draw(m_mImageIDMap["12"], 0,0,1,0.85f);
		break;
	}

	int nX = 512 - 253;
	int nY = 384 + 200;

	//m_pTM->Draw(m_mImageIDMap["LoadingBarBG"], nX, nY, 1.0f, 1.0f, &m_rLoadbarBGRect);
	//m_pTM->Draw(m_mImageIDMap["LoadingBar"], nX + 19, nY + 18, 1.0f, 1.0f, &m_rLoadbarRect);

	//string szText = "Loading... " + m_szCurAssetName;
	//m_pBF->Print(szText.c_str(), 512, nY + 90, true );
}

bool CLoadState::Exit(void)
{
	m_nCurrentStage = 0;

	return false;
}

CLoadState* CLoadState::GetInstance(void)
{
	static CLoadState instance;
	return &instance;
}

void CLoadState::LoadHighScore(string file)
{
	fstream in;
	in.open(file.c_str(),ios::in | ios::binary);

	if(in.is_open())
	{
		in.read((char*)&m_LoadScore,sizeof(m_LoadScore));
		in.close();
	}
}

void CLoadState::WriteHighScore(string file)
{
	fstream out;

	out.open(file.c_str(), ios::out | ios::binary);

	if(out.is_open())
	{
		out.write((char*)&m_LoadScore,sizeof(m_LoadScore));
		out.close();
	}
}