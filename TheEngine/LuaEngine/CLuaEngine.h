#ifndef _CLUA_ENGINE_H
#define _CLUA_ENGINE_H

//Lua includes
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

//Direct X includes
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <string>

class CLuaEngine
{
private:
	lua_State* luaState;

	CLuaEngine();
	~CLuaEngine();
	CLuaEngine& operator=(const CLuaEngine& eng);
	CLuaEngine(const CLuaEngine& eng);
public:
	//Config functions
	static CLuaEngine* GetInstance(void);
	void GetData(float& data, char* name);
	void GetData(std::string& data, char* name);
	void GetData(D3DXVECTOR3& data, char* name);
	void GetData(bool& data, char* name);
	void GetData(char* data, char* name);
	void GetAnimationData(float& data, char* characterName, char* animationName, char* dataName);

	void RefreshLua();

	//Accessor
	lua_State* GetLuaState(void) { return luaState; }
};

#endif
