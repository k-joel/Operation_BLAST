#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define SAFE_DELETE(p)			if (p) { delete p; p = NULL; }

#include <dinput.h>
#include <vector>
#include <XInput.h>
#pragma comment(lib, "DInput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "XInput.lib")
#include <map>

using namespace std;

#define ReleaseCOM(x)	{ if(x) { x->Release(); x=0;}}
#define MAX_PLAYERS 2
//Haven't used these yet

enum InputDeviceType 
{
	DEV_NONE     = 0x00000000,
	DEV_KEYBOARD = 0x00010000, 
	//DEV_MOUSE	 = 0x00020000, 
	DEV_GAMEPAD	 = 0x00040000
};

enum InputType
{
	IT_ISPRESSED		= 0x00100000, // This is only go for one frame
	IT_ISDOWN			= 0x00200000, // This goes till button is up
	IT_ISUP				= 0x00400000,
	IT_ISRELEASED		= 0x00800000,
	//This is mainly for gamepads
	IT_MOVEX			= 0x01000000, 
	IT_MOVEY			= 0x02000000,
	IT_MOVEAMOUNT		= 0x04000000,
	IT_RIGHTMOVEX		= 0x08000000,
	IT_RIGHTMOVEY		= 0x10000000,
	IT_RIGHTMOVEAMOUNT	= 0x20000000,

};

//These are actions that will be used to map in game actions to game responses
enum Action
{ 
	ACT_ACCEPT			, 
	ACT_BACK			, 
	ACT_SHOOT			,
	ACT_RELEASESHOOT	,
	ACT_JUMP			, 
	//Movement
	ACT_PRONE			,
	ACT_RELEASERIGHT	, 
	ACT_RELEASELEFT		,
	ACT_MOVERIGHT		, 
	ACT_MOVELEFT		,
	ACT_MOVEUP			, 
	ACT_MOVEDOWN		,
	ACT_HOLDUP			, // Use with IT_ISDOWN
	ACT_HOLDRIGHT		, // Use with IT_ISDOWN
	ACT_HOLDDOWN		, // Use with IT_ISDOWN
	ACT_HOLDLEFT		, // Use with IT_ISDOWN
	ACT_MOVEX			,
	ACT_MOVEY			,
	ACT_MENUBACK		,
	ACT_PAUSE			,
	ACT_ULTFIRE			,
	ACT_ALTFIRE			,
	ACT_LDASH			,
	ACT_RDASH			,
};

struct tAction
{
	int	devices;
	map<int,  vector<int>> deviceValueMap;
	int   type;
	Action		action;
};

typedef byte Key;
typedef byte Button;

class InputDevice;

class CInput
{
	IDirectInput8	      * directInput;
	vector<InputDevice *>   devices;
	vector<map<Action, tAction*>>	Vactions;

	CInput(void);
	~CInput(void);
	
	//CInput(const CInput & ref){}
	CInput & operator=(const CInput & ref) { return *this; }

public:

	static CInput * GetInstance();
	//Creates the DirectInput8 Device
	void Init(HWND hWnd, HINSTANCE hInst);

	//Release device
	void Shutdown();

	//Add an InputDevice to Devices
	void AddDevice(InputDevice * device);
	//Remove an InputDevice from Devices
	void RemoveDevice(InputDevice * device);
	//Clears all devices
	void ClearDevices();

	//Calls Update on all devices
	void Update();

	//
	bool GetAnyPressed(int nPlayer = 0);

	//Gets whether that action as performed the last frame
	bool  GetAction(Action action, int player = 0);

	//void  SetAction();

	//Gets how much that action was performed that last frame
	float GetActionAmount(Action action, int player = 0);

	//Map an action to devices, types and a given value
	void MapAction(Action action, InputDeviceType devices, InputType type, int value, int player = 0);

	//Remove an actions mapping, passing 0 will remove all mappings, passing
	//in the device type will remove just that devices mapping
	void UnmapAction(Action action, InputDeviceType devices = DEV_NONE, int player = 0);

	InputDevice * GetDevice(InputDeviceType type);

	//Getter
	IDirectInput8 * GetDirectInputCOM() const;
};

//Abstract class for InputDeviceType
class InputDevice
{
protected:
	CInput				* input;
	InputDeviceType	      type;
	IDirectInputDevice8 * diDevice;

	bool windowMode;

public:
	~InputDevice();

	InputDevice(CInput * input, bool windowMode = true);


	InputDeviceType	GetType() const { return type; }
	virtual void CreateDevice() = 0;
	virtual void DestroyDevice() {};

	virtual bool Update() { return true; }

	//Calls Acquire on the device
	virtual void Acquire();

	virtual void ClearStates();

	//Sets the cooperative mode flags to either exclusive or inclusive
	void SetWindowMode(bool windowMode); 

};

class KeyboardDevice : public InputDevice
{
	//buffers for key states
	Key keys[256];
	Key lastKeys[256];

public:
	KeyboardDevice(CInput * CInput, bool windowMode = true);
	~KeyboardDevice();
	
	//Gets latest key states and fills the lastKeys buffer
	bool Update();
	
	//Create DirectInput8 standard keyboard
	void CreateDevice();
	//Releases device
	void DestroyDevice();
	//Clear the keys
	void ClearStates();

	//use the DIK_ defines from dxinput8.h for testing keys
	bool KeyDown(Key key) const;
	bool KeyUp(Key key) const;
	bool KeyPressed(Key key) const;	
	bool KeyRelease(Key key) const;	
};

/*
class MouseDevice : public InputDevice
{
public:
	enum MouseButton  { LEFT = 0, RIGHT, MIDDLE, MOUSEWHEEL };

private:

	DIMOUSESTATE2	  mouseState;
	DIMOUSESTATE2	  lastMouseState;
	float			  xDelta, yDelta;
	float			  wheelDelta;



public:
	MouseDevice(CInput * CInput, bool windowMode = true);
	~MouseDevice();

	//Gets latest mouse states and 
	bool Update();

	void CreateDevice();
	void DestroyDevice();

	//clear states
	void ClearStates();

	bool GetButtonDown(int button) const;
	bool GetButtonPressed(int button) const;
	bool GetButtonUp(int button) const;

	//Gets mouse wheel rotation
	float GetWheelPosition() const;
	//Determines how much the mouse moved left or right
	float XPositionRelative() const;
	//Determines how much the mouse moved up or down
	float YPositionRelative() const;
};*/


class XboxGamePadDevice : public InputDevice
{
public:
	//These correspond the defines in XInput.h for easy mapping
	enum ControllerButton 
	{ 
		DPAD_UP = 0x01,
		DPAD_DOWN = 0x02,
		DPAD_LEFT = 0x04,
		DPAD_RIGHT = 0x08,
		START = 0x10,
		BACK = 0x20,
		LEFT_THUMB = 0x40,
		RIGHT_THUMB = 0x80,
		LEFT_BUTTON = 0x100,
		RIGHT_BUTTON = 0x200,
		A_BUTTON = 0x1000,
		B_BUTTON = 0x2000,
		X_BUTTON = 0x4000,
		Y_BUTTON = 0x8000,
		LEFT_TRIGGER = 0x10000,
		RIGHT_TRIGGER = 0x20000
	};

private:

	//CInput states for all four players
	XINPUT_STATE state[4];
	XINPUT_STATE lastState[4];
	
	BYTE numPlayers;

	bool controllerConnected[4];
	bool lastControllerConnected[4];

public:
	//Constructor allowing numbers of controllers to monitor
	XboxGamePadDevice(CInput * CInput, bool windowMode = true, BYTE numPlayers = 1);
	~XboxGamePadDevice();

	//Gets latest controller states and 
	bool Update();

	//Not used
	void CreateDevice();
	//Not used
	void DestroyDevice();

	//clear states
	void ClearStates();
	//Getters
	bool GetButtonDown(int button, BYTE playerIndex) const;
	bool GetButtonPressed(int button, BYTE playerIndex) const;
	bool GetButtonReleased(int button, BYTE playerIndex) const;
	bool GetButtonUp(int button, BYTE playerIndex) const;

	BYTE GetLeftTrigger(BYTE playerIndex) const;
	BYTE GetRightTrigger(BYTE playerIndex) const;
	SHORT GetLeftThumbX(BYTE playerIndex) const;
	SHORT GetLeftThumbY(BYTE playerIndex) const;
	SHORT GetRightThumbX(BYTE playerIndex) const;
	SHORT GetRightThumbY(BYTE playerIndex) const;

	//Controls force feedback
	void Vibrate(WORD leftMotorSpeed, WORD rightMotorSpeed, BYTE playerIndex);

	//Stops vibration for a controller
	void StopVibrate(BYTE playerIndex);

	//Sets the number of players to monitor
	bool ControllerConnected(BYTE playerIndex) const;

	//Was the controller just now connected this last frame?
	bool ControllerWasConnected(BYTE playerIndex) const;

	void SetNumPlayers(BYTE numPlayers);
};