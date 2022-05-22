#include "CLuaEngine.h"


CLuaEngine::CLuaEngine()
{
	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Config/Config.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Assaulter.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/AssaulterEffect.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Baron.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Barrel.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Boss.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/BossParts.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/ChargedLazer.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Claire.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Crate.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/DirtExplosion.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Flame_Burst.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Flame_ult.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Helios.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/HeliosEffect.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Laser_Flash.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Muzzle_Flash.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Pilot_Light.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Pop.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Shotgun_Blast.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Shroom.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Striker.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/StrikerEffect.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Words.lua");
}

CLuaEngine::~CLuaEngine()
{
	lua_close(luaState);
}

CLuaEngine& CLuaEngine::operator=(const CLuaEngine& eng)
{
	return *this;
}

CLuaEngine::CLuaEngine(const CLuaEngine& eng)
{
}

CLuaEngine* CLuaEngine::GetInstance(void)
{
	static CLuaEngine instance;
	return &instance;
}

void CLuaEngine::GetData(float& data, char* name)
{
	char buffer[64];

	sprintf_s(buffer, "%s", name);
	lua_getglobal(luaState, "GetVar");
	lua_pushstring(luaState, buffer);
	lua_pcall(luaState, 1, 1, 0);

	data = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, lua_gettop(luaState));
}

void CLuaEngine::GetData(std::string& data, char* name)
{
	char buffer[64];

	sprintf_s(buffer, "%s", name);
	lua_getglobal(luaState, "GetVar");
	lua_pushstring(luaState, buffer);
	lua_pcall(luaState, 1, 1, 0);

	data = lua_tostring(luaState, -1);

	lua_pop(luaState, lua_gettop(luaState));

}

void CLuaEngine::GetData(D3DXVECTOR3& data, char* name)
{
	char buffer[64];

	sprintf_s(buffer, "%sX", name);
	CLuaEngine::GetInstance()->GetData(data.x, buffer);
	sprintf_s(buffer, "%sY", name);
	CLuaEngine::GetInstance()->GetData(data.y, buffer);
	sprintf_s(buffer, "%sZ", name);
	CLuaEngine::GetInstance()->GetData(data.z, buffer);

	lua_pop(luaState, lua_gettop(luaState));

}

void CLuaEngine::GetData(bool& data, char* name)
{
	char buffer[64];

	sprintf_s(buffer, "%s", name);
	lua_getglobal(luaState, "GetVar");
	lua_pushstring(luaState, buffer);
	lua_pcall(luaState, 1, 1, 0);

	data = ((bool)lua_toboolean(luaState, -1));

	lua_pop(luaState, lua_gettop(luaState));

}

void CLuaEngine::GetData(char* data, char* name)
{
	char buffer[64];

	sprintf_s(buffer, "%s", name);
	lua_getglobal(luaState, "GetVar");
	lua_pushstring(luaState, buffer);
	lua_pcall(luaState, 1, 1, 0);
	const char* newData = lua_tostring(luaState, -1);

	strcpy_s(data, 64, newData); 

	lua_pop(luaState, lua_gettop(luaState));

}

void CLuaEngine::RefreshLua()
{
	luaL_dofile(luaState, "Resources/Lua/Config/Config.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Assaulter.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/AssaulterEffect.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Baron.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Barrel.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Boss.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/BossParts.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/ChargedLazer.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Claire.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Crate.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/DirtExplosion.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Flame_Burst.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Flame_ult.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Helios.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/HeliosEffect.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Laser_Flash.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Muzzle_Flash.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Pilot_Light.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Pop.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Shotgun_Blast.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Shroom.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Striker.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/StrikerEffect.lua");
	luaL_dofile(luaState, "Resources/Lua/Animations/Words.lua");
}

void CLuaEngine::GetAnimationData(float& data, char* characterName, char* animationName, char* dataName)
{

	if(strcmp(characterName, "Assaulter_geoShape") == 0)
	{
		lua_getglobal(luaState, "GetAssaulterVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "AssaulterExplosionShape") == 0)
	{
		lua_getglobal(luaState, "GetAssaulterFXVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "barron_geoShape") == 0)
	{
		lua_getglobal(luaState, "GetBaronVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "barrelexplodeShape") == 0)
	{
		lua_getglobal(luaState, "GetBarrelVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "boss_geoShape") == 0)
	{
		lua_getglobal(luaState, "GetBossVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "boss_bodyShape") == 0 ||
		strcmp(characterName, "boss_conveyorShape") == 0 ||
		strcmp(characterName, "boss_gun_bigShape") == 0 ||
		strcmp(characterName, "boss_gun_bottomShape") == 0 ||
		strcmp(characterName, "boss_gun_topShape") == 0)
	{
		lua_getglobal(luaState, "GetBossPartsVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "ChargedLaserShape") == 0)
	{
		lua_getglobal(luaState, "GetLazerVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "Claire_geoShape") == 0)
	{
		lua_getglobal(luaState, "GetClaireVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "CrateShape") == 0)
	{
		lua_getglobal(luaState, "GetCrateVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	//this here should be the fix
	if(strcmp(characterName, "Dirt_explosionAShape") == 0 ||
		strcmp(characterName, "Dirt_explosionBShape") == 0)
	{
		lua_getglobal(luaState, "GetDirtVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "Flame_Burst") == 0)
	{
		lua_getglobal(luaState, "GetFlameBurstVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "Flame_ultShape") == 0)
	{
		lua_getglobal(luaState, "GetFlameUltVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "Helios_geoShape") == 0)
	{
		lua_getglobal(luaState, "GetHeliosVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "HeliosExplosionShape") == 0)
	{
		lua_getglobal(luaState, "GetHeliosFXVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

		if(strcmp(characterName, "Laser_flashShape") == 0)
	{
		lua_getglobal(luaState, "GetLaserFlashVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "Muzzle_fireShape") == 0)
	{
		lua_getglobal(luaState, "GetMuzzleBurstVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "Flame_pilotShape") == 0)
	{
		lua_getglobal(luaState, "GetPilotLightVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "explosionShape") == 0)
	{
		lua_getglobal(luaState, "GetPopVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "shotgun_blastShape") == 0)
	{
		lua_getglobal(luaState, "GetShotgunFlashVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "largeexplosionShape") == 0)
	{
		lua_getglobal(luaState, "GetShroomVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "robot_geoShape") == 0 )
	{
		lua_getglobal(luaState, "GetStrikerVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "StrikerExplosionShape") == 0 )
	{
		lua_getglobal(luaState, "GetStrikerFXVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}

	if(strcmp(characterName, "GameOverShape") == 0 ||
		strcmp(characterName, "VictoryShape") == 0 ||
		strcmp(characterName, "Player_1Shape") == 0 ||
		strcmp(characterName, "Player_2Shape") == 0
		)
	{
		lua_getglobal(luaState, "GetWordsVar");
		lua_pushstring(luaState, animationName);
		lua_pushstring(luaState, dataName);
		lua_pcall(luaState, 2, 1, 0);
		data = ((float)lua_tonumber(luaState, -1));
		lua_pop(luaState, lua_gettop(luaState));
		return;
	}
}

