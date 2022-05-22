
#include "CInput.h"

#define KEYDOWN(arr, key) (arr[key] & 0x80 ? true : false)
#define BUTTONDOWN(button) (button & 0x80 ? true : false)

struct STRUC_KEY
{
UCHAR keyByte;
char  keyName[30];
};

STRUC_KEY MyKey[] = 
{
	DIK_0, 				"DIK_0",
	DIK_1, 				"DIK_1",
	DIK_2, 				"DIK_2",
	DIK_3, 				"DIK_3",
	DIK_4, 				"DIK_4",
	DIK_5, 				"DIK_5",
	DIK_6, 				"DIK_6",
	DIK_7, 				"DIK_7",
	DIK_8, 				"DIK_8",
	DIK_9, 				"DIK_9",
	DIK_A, 				"DIK_A",
	DIK_ABNT_C1, 		"DIK_ABNT_C1",
	DIK_ABNT_C2, 		"DIK_ABNT_C2",
	DIK_ADD, 			"DIK_ADD",
	DIK_APOSTROPHE, 	"DIK_APOSTROPHE",
	DIK_APPS, 			"DIK_APPS",
	DIK_AT, 			"DIK_AT",
	DIK_AX, 			"DIK_AX",
	DIK_B, 				"DIK_B",
	DIK_BACK, 			"DIK_BACK",
	DIK_BACKSLASH, 		"DIK_BACKSLASH",
	DIK_C, 				"DIK_C",
	DIK_CALCULATOR, 	"DIK_CALCULATOR",
	DIK_CAPITAL, 		"DIK_CAPITAL",
	DIK_COLON, 			"DIK_COLON",
	DIK_COMMA, 			"DIK_COMMA",
	DIK_CONVERT, 		"DIK_CONVERT",
	DIK_D, 				"DIK_D",
	DIK_DECIMAL, 		"DIK_DECIMAL",
	DIK_DELETE, 		"DIK_DELETE",
	DIK_DIVIDE, 		"DIK_DIVIDE",
	DIK_DOWN, 			"DIK_DOWN",
	DIK_E, 				"DIK_E",
	DIK_END, 			"DIK_END",
	DIK_EQUALS, 		"DIK_EQUALS",
	DIK_ESCAPE, 		"DIK_ESCAPE",
	DIK_F, 				"DIK_F",
	DIK_F1, 			"DIK_F1",
	DIK_F2, 			"DIK_F2",
	DIK_F3, 			"DIK_F3",
	DIK_F4, 			"DIK_F4",
	DIK_F5, 			"DIK_F5",
	DIK_F6, 			"DIK_F6",
	DIK_F7, 			"DIK_F7",
	DIK_F8, 			"DIK_F8",
	DIK_F9, 			"DIK_F9",
	DIK_F10, 			"DIK_F10",
	DIK_F11, 			"DIK_F11",
	DIK_F12, 			"DIK_F12",
	DIK_F13, 			"DIK_F13",
	DIK_F14, 			"DIK_F14",
	DIK_F15, 			"DIK_F15",
	DIK_G, 				"DIK_G",
	DIK_GRAVE, 			"DIK_GRAVE",
	DIK_H, 				"DIK_H",
	DIK_HOME, 			"DIK_HOME",
	DIK_I, 				"DIK_I",
	DIK_INSERT, 		"DIK_INSERT",
	DIK_J, 				"DIK_J",
	DIK_K, 				"DIK_K",
	DIK_KANA, 			"DIK_KANA",
	DIK_KANJI, 			"DIK_KANJI",
	DIK_L, 				"DIK_L",
	DIK_LBRACKET, 		"DIK_LBRACKET",
	DIK_LCONTROL, 		"DIK_LCONTROL",
	DIK_LEFT, 			"DIK_LEFT",
	DIK_LMENU, 			"DIK_LMENU",
	DIK_LSHIFT, 		"DIK_LSHIFT",
	DIK_LWIN, 			"DIK_LWIN",
	DIK_M, 				"DIK_M",
	DIK_MAIL, 			"DIK_MAIL",
	DIK_MEDIASELECT, 	"DIK_MEDIASELECT",
	DIK_MEDIASTOP, 		"DIK_MEDIASTOP",
	DIK_MINUS, 			"DIK_MINUS",
	DIK_MULTIPLY, 		"DIK_MULTIPLY",
	DIK_MUTE, 			"DIK_MUTE",
	DIK_MYCOMPUTER, 	"DIK_MYCOMPUTER",
	DIK_N, 				"DIK_N",
	DIK_NEXT, 			"DIK_NEXT",
	DIK_NEXTTRACK, 		"DIK_NEXTTRACK",
	DIK_NOCONVERT, 		"DIK_NOCONVERT",
	DIK_NUMLOCK, 		"DIK_NUMLOCK",
	DIK_NUMPAD0, 		"DIK_NUMPAD0",
	DIK_NUMPAD1, 		"DIK_NUMPAD1",
	DIK_NUMPAD2, 		"DIK_NUMPAD2",
	DIK_NUMPAD3, 		"DIK_NUMPAD3",
	DIK_NUMPAD4, 		"DIK_NUMPAD4",
	DIK_NUMPAD5, 		"DIK_NUMPAD5",
	DIK_NUMPAD6, 		"DIK_NUMPAD6",
	DIK_NUMPAD7, 		"DIK_NUMPAD7",
	DIK_NUMPAD8, 		"DIK_NUMPAD8",
	DIK_NUMPAD9, 		"DIK_NUMPAD9",
	DIK_NUMPADCOMMA, 	"DIK_NUMPADCOMMA",
	DIK_NUMPADENTER, 	"DIK_NUMPADENTER",
	DIK_NUMPADEQUALS, 	"DIK_NUMPADEQUALS",
	DIK_O, 				"DIK_O",
	DIK_OEM_102, 		"DIK_OEM_102",
	DIK_P, 				"DIK_P",
	DIK_PAUSE, 			"DIK_PAUSE",
	DIK_PERIOD, 		"DIK_PERIOD",
	DIK_PLAYPAUSE, 		"DIK_PLAYPAUSE",
	DIK_POWER, 			"DIK_POWER",
	DIK_PREVTRACK, 		"DIK_PREVTRACK",
	DIK_PRIOR, 			"DIK_PRIOR",
	DIK_Q, 				"DIK_Q",
	DIK_R, 				"DIK_R",
	DIK_RBRACKET, 		"DIK_RBRACKET",
	DIK_RCONTROL, 		"DIK_RCONTROL",
	DIK_RETURN, 		"DIK_RETURN",
	DIK_RIGHT, 			"DIK_RIGHT",
	DIK_RMENU, 			"DIK_RMENU",
	DIK_RSHIFT, 		"DIK_RSHIFT",
	DIK_RWIN, 			"DIK_RWIN",
	DIK_S, 				"DIK_S",
	DIK_SCROLL, 		"DIK_SCROLL",
	DIK_SEMICOLON, 		"DIK_SEMICOLON",
	DIK_SLASH, 			"DIK_SLASH",
	DIK_SLEEP, 			"DIK_SLEEP",
	DIK_SPACE, 			"DIK_SPACE",
	DIK_STOP, 			"DIK_STOP",
	DIK_SUBTRACT, 		"DIK_SUBTRACT",
	DIK_SYSRQ, 			"DIK_SYSRQ",
	DIK_T, 				"DIK_T",
	DIK_TAB, 			"DIK_TAB",
	DIK_U, 				"DIK_U",
	DIK_UNDERLINE, 		"DIK_UNDERLINE",
	DIK_UNLABELED, 		"DIK_UNLABELED",
	DIK_UP, 			"DIK_UP",
	DIK_V, 				"DIK_V",
	DIK_VOLUMEDOWN, 	"DIK_VOLUMEDOWN",
	DIK_VOLUMEUP, 		"DIK_VOLUMEUP",
	DIK_W, 				"DIK_W",
	DIK_WAKE, 			"DIK_WAKE",
	DIK_WEBBACK, 		"DIK_WEBBACK",
	DIK_WEBFAVORITES, 	"DIK_WEBFAVORITES",
	DIK_WEBFORWARD, 	"DIK_WEBFORWARD",
	DIK_WEBHOME, 		"DIK_WEBHOME",
	DIK_WEBREFRESH, 	"DIK_WEBREFRESH",
	DIK_WEBSEARCH, 		"DIK_WEBSEARCH",
	DIK_WEBSTOP, 		"DIK_WEBSTOP",
	DIK_X, 				"DIK_X",
	DIK_Y, 				"DIK_Y",
	DIK_YEN, 			"DIK_YEN",
	DIK_Z, 				"DIK_Z",
	DIK_LALT,			"DIK_LALT",
	DIK_RALT,			"DIK_RALT",
};




CInput * CInput::GetInstance()
{
	static CInput instance;

	return &instance;
}

CInput::CInput(void)
{
	this->directInput = 0;
}
CInput::~CInput(void)
{

}

void CInput::Init(HWND hWnd, HINSTANCE hInst)
{
	Vactions.resize(MAX_PLAYERS);
	Shutdown();
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
}

void CInput::Shutdown()
{
	
	//Delete everything!
	for(unsigned int i(0); i < MAX_PLAYERS;++i)
	{
		while(Vactions[i].empty() == false)
		{
			map<Action, tAction*>::iterator itt;
			itt = Vactions[i].begin();

			SAFE_DELETE((*itt).second);
			itt = Vactions[i].erase(itt);
		}
	}
		
	while(devices.empty() == false)
	{
		delete devices[devices.size() - 1];
		devices.pop_back();
	}

	ReleaseCOM(directInput);
}

void CInput::AddDevice(InputDevice * device)
{
	devices.push_back(device);
}

void CInput::RemoveDevice(InputDevice * device)
{
	for(vector<InputDevice *>::iterator i = devices.begin(); i != devices.end(); i++)
	{
		if(*i == device)
		{
			devices.erase(i);
			break;
		}
	}
}

InputDevice * CInput::GetDevice(InputDeviceType type)
{
	for(vector<InputDevice *>::iterator i = devices.begin(); i != devices.end(); i++)
	{
		if((*i)->GetType() == type)
		{
			return (*i);
		}
	}

	return NULL;
}
void CInput::ClearDevices()
{
	for(int i = 0; i < (int)devices.size(); i++)
	{
		devices[i]->ClearStates();
	}
}

void CInput::Update()
{

	for(vector<InputDevice *>::iterator i = devices.begin(); i != devices.end(); i++)
	{		
		if((*i)->Update() == false)
			(*i)->Acquire();
	}
}

bool CInput::GetAnyPressed(int nPlayer)
{
	KeyboardDevice * keyboard = NULL;
	//MouseDevice * mouse = NULL;
	XboxGamePadDevice  * gamepads = NULL;

	for(int i = 0; i < (int)devices.size();i++)
	{
		if(devices[i]->GetType() == DEV_KEYBOARD)
			keyboard = (KeyboardDevice*)devices[i];
/*
		if(devices[i]->GetType() == DEV_MOUSE)
			mouse = (MouseDevice*)devices[i];*/

		if(devices[i]->GetType() == DEV_GAMEPAD)
			gamepads = (XboxGamePadDevice*)devices[i];
	}

	for(int i = 0; i < 145; i++)
	{
		if(keyboard != NULL)
		{
			if(IT_ISPRESSED)
			{
				if(keyboard->KeyPressed(MyKey[i].keyByte))
					return true;
			}
			else if(IT_ISDOWN)
			{
				if(keyboard->KeyDown(MyKey[i].keyByte))
					return true;
			}
		}
	}
/*
	if(mouse != NULL)
	{
		for(int i = 0; i < 3; i++)
		{
			if(mouse->GetButtonPressed(i))
				return true;
		}
	}*/

	if(gamepads != NULL)
	{
		for(int i = 0; i < 16; i++)
		{
			if(gamepads->GetButtonPressed(1 << i, nPlayer))
				return true;
		}
	}

	return false;
}

//Gets whether that action as performed the last frame
bool  CInput::GetAction(Action action, int player)
{
	//Get the action
	vector<map<Action,tAction*>>::iterator itr	= Vactions.begin();
	tAction* act = itr[player][action];

	if(act == NULL)
		return false;

	KeyboardDevice * keyboard = NULL;
	//MouseDevice * mouse = NULL;
	XboxGamePadDevice  * gamepads = NULL;

	//Get Pointers the devices
	//HACK there is a smarter way of doing this
	//rather than every time.... think about it
	for(int i = 0; i < (int)devices.size();i++)
	{
		if(devices[i]->GetType() == DEV_KEYBOARD)
			keyboard = (KeyboardDevice*)devices[i];
/*
		else if(devices[i]->GetType() == DEV_MOUSE)
			mouse = (MouseDevice*)devices[i];*/

		else if(devices[i]->GetType() == DEV_GAMEPAD)
			gamepads = (XboxGamePadDevice*)devices[i];
	}


	//TODO: Handle special case like ALTENTER HERE

	//Loop through all the values
	//If the value is found for that device it will return
	//otherwise it will go through all values
	for(map<int, vector<int>>::iterator itt = act->deviceValueMap.begin(); itt != act->deviceValueMap.end(); itt++)
	{
		for(vector<int>::iterator value = (*itt).second.begin(); value != (*itt).second.end(); value++)
		{
			if(((*itt).first & DEV_KEYBOARD) && keyboard != NULL)
			{
				if(act->type & IT_ISPRESSED)
				{
					if(keyboard->KeyPressed((*value)))
						return true;
				}
				else if(act->type & IT_ISDOWN)
				{
					if(keyboard->KeyDown((*value)))
						return true;
				}
				else if(act->type & IT_ISUP)
				{
					if(keyboard->KeyUp((*value)))
						return true;
				}
				else if(act->type & IT_ISRELEASED)
				{
					if(keyboard->KeyRelease((*value)))
						return true;
				}
			}
/*
			if(((*itt).first & DEV_MOUSE) && mouse != NULL)
			{
				if(act->type & IT_ISPRESSED)
				{	
					if(mouse->GetButtonPressed((*value)))
						return true;
				}
				else if(act->type & IT_ISDOWN)
				{
					if(mouse->GetButtonDown((*value)))
						return true;
				}
				else if(act->type & IT_ISUP)
				{
					if(mouse->GetButtonUp((*value)))
						return true;
				}

			}*/

			if(((*itt).first & DEV_GAMEPAD) && gamepads != NULL)
			{
				if(act->type & IT_ISPRESSED)
				{
					if(gamepads->GetButtonPressed((*value), player))
						return true;
				}
				else if(act->type & IT_ISDOWN)
				{
					if(gamepads->GetButtonDown((*value), player))
						return true;
				}
				else if(act->type & IT_ISUP)
				{
					if(gamepads->GetButtonUp((*value), player))
						return true;
				}
				else if(act->type & IT_ISRELEASED)
				{
					if(gamepads->GetButtonReleased((*value), player))
						return true;
				}
			}
		}
	}
	return false;

}

//Gets how much that action was performed that last frame
float CInput::GetActionAmount(Action action, int player)
{
	//Get the action
	vector<map<Action,tAction*>>::iterator itr	= Vactions.begin();
	tAction* act = itr[player][action];
	
	if(act == NULL)
		return false;

	KeyboardDevice * keyboard		= NULL;
	//MouseDevice * mouse				= NULL;
	XboxGamePadDevice  * gamepads	= NULL;

	//Get Pointers the devices
	//HACK there is a smarter way of doing this
	//rather than every time.... think about it
	for(int i = 0; i < (int)devices.size();i++)
	{
		if(devices[i]->GetType() == DEV_KEYBOARD)
			keyboard = (KeyboardDevice*)devices[i];
/*
		else if(devices[i]->GetType() == DEV_MOUSE)
			mouse = (MouseDevice*)devices[i];*/

		else if(devices[i]->GetType() == DEV_GAMEPAD)
			gamepads = (XboxGamePadDevice*)devices[i];
	}

	for(map<int, vector<int>>::iterator itt = act->deviceValueMap.begin(); itt != act->deviceValueMap.end(); itt++)
	{
		for(vector<int>::iterator value = (*itt).second.begin(); value != (*itt).second.end(); value++)
		{
			//if(((*itt).first & DEV_KEYBOARD) && keyboard != NULL)
			//{
			//	//HACK: not a good way to do it, but works
			//	if(act->type & (IT_MOVEX | IT_MOVEY))
			//	{
			//		return keyboard->KeyDown((*value)) ? 1.0f : 0.0f;
			//	}				
			//}
/*
			if(((*itt).first & DEV_MOUSE) && mouse != NULL)
			{
				if(act->type & IT_MOVEX)
				{
					return mouse->XPositionRelative();
				}
				else if(act->type & IT_MOVEY)
				{
					return mouse->YPositionRelative();
				}
				else if(act->type & IT_MOVEAMOUNT)
				{
					return mouse->GetWheelPosition();
				}
			}*/

			if(((*itt).first & DEV_GAMEPAD) && gamepads != NULL)
			{
				if((*value) == XboxGamePadDevice::LEFT_THUMB)
				{
					if(act->type == IT_MOVEX)
					{
						return gamepads->GetLeftThumbX(player);
					}
					else if(act->type == IT_MOVEY)
					{
						return gamepads->GetLeftThumbY(player);
					}
				}
				else if((*value) == XboxGamePadDevice::RIGHT_THUMB)
				{
					if(act->type == IT_MOVEX)
					{
						return gamepads->GetRightThumbX(player);
					}
					else if(act->type == IT_MOVEY)
					{
						return gamepads->GetRightThumbY(player);
					}
				}
				if((*value) == XboxGamePadDevice::LEFT_TRIGGER)
				{
					if(act->type == IT_MOVEAMOUNT)
					{
						return gamepads->GetLeftTrigger(player);
					}
				}
				else if((*value) == XboxGamePadDevice::RIGHT_TRIGGER)
				{
					if(act->type == IT_MOVEAMOUNT)
					{
						return gamepads->GetRightTrigger(player);
					}			
				}			
			}
		}
	}
	return 0.0f;
}

//Map an action to devices, types and a given value
void CInput::MapAction(Action action, InputDeviceType devices, InputType type, int value, int player)
{

	vector<map<Action,tAction*>>::iterator itr	= Vactions.begin();
	tAction * act = itr[player][action];

	if(act == NULL)
	{
		tAction *a	= new tAction;
		a->action	= action;
		a->devices	= devices;
		a->type		= type;
		a->deviceValueMap[devices].push_back(value);

		itr[player][action] = a;

	}
	else
	{
		act->devices |= (int)devices;
		act->type |= (int)type;
		act->deviceValueMap[devices].push_back(value);
	}
}

//TODO
//Remove an actions mapping, passing 0 will remove all mappings, passing
//in the device type will remove just that devices mapping
void CInput::UnmapAction(Action action, InputDeviceType devices, int player)
{
	vector<map<Action,tAction*>>::iterator itr	= Vactions.begin();
	tAction * act = itr[player][action];

	if(act == NULL)
	{

	}
	else
	{
		itr[player].erase(action);
	}
}

IDirectInput8 * CInput::GetDirectInputCOM() const
{
	return directInput;
}

InputDevice::~InputDevice()
{
	DestroyDevice();
}

void InputDevice::Acquire()
{
	if(diDevice)
		diDevice->Acquire();
			
}

void InputDevice::ClearStates()
{

}

InputDevice::InputDevice(CInput * CInput, bool windowMode)
{
	this->input = CInput;
	this->windowMode = windowMode;
	diDevice = 0;
}

void InputDevice::SetWindowMode(bool windowMode)
{
	diDevice->SetCooperativeLevel(GetForegroundWindow(), 
		(windowMode ? DISCL_FOREGROUND | DISCL_NONEXCLUSIVE : DISCL_FOREGROUND | DISCL_EXCLUSIVE));

}

KeyboardDevice::KeyboardDevice(CInput * input, bool windowMode) : InputDevice(input, windowMode)
{
	this->type = DEV_KEYBOARD;
	ZeroMemory(&keys, sizeof(keys));
	ZeroMemory(&lastKeys, sizeof(keys));
}

KeyboardDevice::~KeyboardDevice()
{
	DestroyDevice();
}

void KeyboardDevice::ClearStates()
{
	ZeroMemory(&keys, sizeof(keys));
	ZeroMemory(&lastKeys, sizeof(keys));
}
bool KeyboardDevice::Update()
{
	if(diDevice == NULL)
	{
		CreateDevice();
		if(diDevice == NULL)
		{
			DestroyDevice();
			return true;
		}
	}

	memcpy(lastKeys, keys, sizeof(keys));
	HRESULT hr = diDevice->GetDeviceState(sizeof(Key) * 256, keys);

	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			diDevice->Acquire();
		}
		return false;
	}
	else
		return true;
}

bool KeyboardDevice::KeyDown(Key key) const
{
	return KEYDOWN(keys, key);
}
bool KeyboardDevice::KeyUp(Key key) const
{
	return !KEYDOWN(keys, key);
}
bool KeyboardDevice::KeyPressed(Key key) const
{
	return KEYDOWN(keys, key) && !KEYDOWN(lastKeys, key);
}
bool KeyboardDevice::KeyRelease(Key key) const
{
	return !KEYDOWN(keys, key) && KEYDOWN(lastKeys, key);
}

void KeyboardDevice::CreateDevice()
{
	IDirectInput8 * di = input->GetDirectInputCOM();

	di->CreateDevice(GUID_SysKeyboard, &diDevice, 0);

	diDevice->SetDataFormat(&c_dfDIKeyboard);

	diDevice->SetCooperativeLevel(GetForegroundWindow(), 
		(windowMode ? DISCL_FOREGROUND | DISCL_NONEXCLUSIVE : DISCL_FOREGROUND | DISCL_EXCLUSIVE));

	diDevice->Acquire();
}

void KeyboardDevice::DestroyDevice()
{
	if(diDevice != NULL)
	{
		diDevice->Unacquire();
		diDevice->Release();
		diDevice = NULL;
	}
}


/*
MouseDevice::MouseDevice(CInput * CInput, bool windowMode) : InputDevice(CInput, windowMode)
{
	this->type = DEV_MOUSE;
	this->windowMode = windowMode;
	this->input = CInput;
	xDelta = yDelta = wheelDelta = 0.0f;

	ZeroMemory(&mouseState, sizeof(DIMOUSESTATE2));
	ZeroMemory(&lastMouseState, sizeof(DIMOUSESTATE2));
}

MouseDevice::~MouseDevice()
{
	DestroyDevice();
}

void MouseDevice::ClearStates()
{
	ZeroMemory(&mouseState, sizeof(DIMOUSESTATE2));
	ZeroMemory(&lastMouseState, sizeof(DIMOUSESTATE2));
}

void MouseDevice::CreateDevice()
{
	IDirectInput8 * di = input->GetDirectInputCOM();

	di->CreateDevice(GUID_SysMouse, &diDevice, 0);

	diDevice->SetDataFormat(&c_dfDIMouse2);

	SetWindowMode(windowMode);

	Acquire();
}

void MouseDevice::DestroyDevice()
{
	if(diDevice != NULL)
	{
		diDevice->Unacquire();
		diDevice->Release();
		diDevice = 0;
	}
}

//Gets latest mouse states and 
bool MouseDevice::Update()
{
	if(diDevice == NULL)
	{
		CreateDevice();
		if(diDevice == NULL)
		{
			DestroyDevice();
			return true;
		}
	}

	memcpy(&lastMouseState, &mouseState, sizeof(DIMOUSESTATE2));
	HRESULT hr = diDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mouseState);

	if(FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			if(FAILED(diDevice->Acquire()))
				return false;
		}
		else
			return true;
	}
	
	xDelta = static_cast<float>(mouseState.lX);
	yDelta = static_cast<float>(mouseState.lY);

	if(mouseState.lZ > 0)
		wheelDelta = 1.0f;
	else if(mouseState.lZ < 0)
		wheelDelta = -1.0f;
	else
		wheelDelta = 0.0f;

	return true;
	
}

bool MouseDevice::GetButtonDown(int button) const
{

	return BUTTONDOWN(mouseState.rgbButtons[button]);
}

bool MouseDevice::GetButtonPressed(int button) const
{
	return BUTTONDOWN(mouseState.rgbButtons[button]) && !BUTTONDOWN(lastMouseState.rgbButtons[button]);
}
bool MouseDevice::GetButtonUp(int button) const
{
	return !BUTTONDOWN(mouseState.rgbButtons[button]);
}

//Gets mouse wheel rotation
float MouseDevice::GetWheelPosition() const
{
	return wheelDelta;
}
//Determines how much the mouse moved left or right
float MouseDevice::XPositionRelative() const
{
	return xDelta;
}
//Determines how much the mouse moved up or down
float MouseDevice::YPositionRelative() const
{
	return yDelta;
}*/


//Constructor allowing numbers of controllers to monitor
XboxGamePadDevice::XboxGamePadDevice(CInput * CInput, bool windowMode, BYTE numPlayers) : InputDevice(CInput, windowMode)
{
	this->type = DEV_GAMEPAD;
	this->numPlayers = numPlayers;
	ZeroMemory(state, sizeof(XINPUT_STATE) * 4);
	ZeroMemory(lastState, sizeof(XINPUT_STATE) * 4);

}
XboxGamePadDevice::~XboxGamePadDevice()
{

}

void XboxGamePadDevice::ClearStates()
{
	ZeroMemory(state, sizeof(XINPUT_STATE) * 4);
	ZeroMemory(lastState, sizeof(XINPUT_STATE) * 4);
}


//Gets latest controller states
bool XboxGamePadDevice::Update()
{
	memcpy(lastControllerConnected, controllerConnected, sizeof(bool) * 4);
	memcpy(lastState, state, sizeof(XINPUT_STATE) * 4);

	DWORD result;
	for(DWORD player = 0; player < numPlayers; player++)
	{
		result = XInputGetState(player, &state[player]);

		if(result == ERROR_SUCCESS)
		{
			controllerConnected[player] = true;

			if((state[player].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state[player].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(state[player].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state[player].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
			{    
				state[player].Gamepad.sThumbLX = 0;
				state[player].Gamepad.sThumbLY = 0;
			}

			if( (state[player].Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state[player].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(state[player].Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state[player].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
			{
				state[player].Gamepad.sThumbRX = 0;
				state[player].Gamepad.sThumbRY = 0;
			}
		}
		else
		{
			controllerConnected[player] = false;
		}
	}


	return true;
}

//Not used
void XboxGamePadDevice::CreateDevice(){}
//Not used
void XboxGamePadDevice::DestroyDevice(){}

//Getters
bool XboxGamePadDevice::GetButtonDown(int button, BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return (state[playerIndex].Gamepad.wButtons & button ? true : false);
}
bool XboxGamePadDevice::GetButtonPressed(int button, BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return (state[playerIndex].Gamepad.wButtons & button) && !(lastState[playerIndex].Gamepad.wButtons & button);
}
bool XboxGamePadDevice::GetButtonReleased(int button, BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return !(state[playerIndex].Gamepad.wButtons & button) && (lastState[playerIndex].Gamepad.wButtons & button);
}
bool XboxGamePadDevice::GetButtonUp(int button, BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return !(state[playerIndex].Gamepad.wButtons & button);
}
BYTE XboxGamePadDevice::GetLeftTrigger(BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return (state[playerIndex].Gamepad.bLeftTrigger);// && (state[playerIndex].Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

BYTE XboxGamePadDevice::GetRightTrigger(BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return (state[playerIndex].Gamepad.bRightTrigger);// && (state[playerIndex].Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

SHORT XboxGamePadDevice::GetLeftThumbX(BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return state[playerIndex].Gamepad.sThumbLX;
}

SHORT XboxGamePadDevice::GetLeftThumbY(BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return state[playerIndex].Gamepad.sThumbLY;
}
SHORT XboxGamePadDevice::GetRightThumbX(BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return state[playerIndex].Gamepad.sThumbRX;
}
SHORT XboxGamePadDevice::GetRightThumbY(BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return state[playerIndex].Gamepad.sThumbRY;
}

//Controls force feedback
void XboxGamePadDevice::Vibrate(WORD leftMotorSpeed, WORD rightMotorSpeed, BYTE playerIndex)
{
	if(playerIndex >= numPlayers)
		return;

	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftMotorSpeed; 
	vibration.wRightMotorSpeed = rightMotorSpeed;
	XInputSetState(playerIndex, &vibration);
}

void XboxGamePadDevice::StopVibrate(BYTE playerIndex)
{
	if(playerIndex >= numPlayers)
		return;

	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0; 
	vibration.wRightMotorSpeed = 0;
	XInputSetState(playerIndex, &vibration);
}


//Sets the number of players to monitor
bool XboxGamePadDevice::ControllerConnected(BYTE playerIndex) const
{
	if(playerIndex >= numPlayers)
		return false;

	return controllerConnected[playerIndex];
}

void XboxGamePadDevice::SetNumPlayers(BYTE numPlayers)
{
	if(numPlayers > 4)
		numPlayers = 4;

	Update();
}


bool XboxGamePadDevice::ControllerWasConnected(BYTE playerIndex) const
{
	if(numPlayers > 4)
		return false;

	return controllerConnected[playerIndex] && !lastControllerConnected[playerIndex];
}