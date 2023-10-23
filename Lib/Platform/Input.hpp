#pragma once

#include "Defines.hpp"
#include "Containers\Vector.hpp"

enum ButtonCode
{
	/*Mouse Buttons*/
	BUTTON_CODE_LEFT_MOUSE = 0x01,
	BUTTON_CODE_RIGHT_MOUSE = 0x02,
	BUTTON_CODE_MIDDLE_MOUSE = 0x04,
	BUTTON_CODE_X_MOUSE_ONE = 0x05,
	BUTTON_CODE_X_MOUSE_TWO = 0x06,

	/*Keyboard Buttons*/
	BUTTON_CODE_CANCEL = 0x03,
	BUTTON_CODE_BACK = 0x08,
	BUTTON_CODE_TAB = 0x09,
	BUTTON_CODE_CLEAR = 0x0C,
	BUTTON_CODE_RETURN = 0x0D,
	BUTTON_CODE_SHIFT = 0x10,
	BUTTON_CODE_CTRL = 0x11,
	BUTTON_CODE_ALT = 0x12,
	BUTTON_CODE_PAUSE = 0x13,
	BUTTON_CODE_CAPITAL = 0x14,
	BUTTON_CODE_KANA = 0x15,
	BUTTON_CODE_HANGEUL = 0x15,
	BUTTON_CODE_HANGUL = 0x15,
	BUTTON_CODE_JUNJA = 0x17,
	BUTTON_CODE_FINAL = 0x18,
	BUTTON_CODE_HANJA = 0x19,
	BUTTON_CODE_KANJI = 0x19,
	BUTTON_CODE_ESCAPE = 0x1B,
	BUTTON_CODE_CONVERT = 0x1C,
	BUTTON_CODE_NONCONVERT = 0x1D,
	BUTTON_CODE_ACCEPT = 0x1E,
	BUTTON_CODE_MODE_CHANGE = 0x1F,
	BUTTON_CODE_SPACE = 0x20,
	BUTTON_CODE_PRIOR = 0x21,
	BUTTON_CODE_NEXT = 0x22,
	BUTTON_CODE_END = 0x23,
	BUTTON_CODE_HOME = 0x24,
	BUTTON_CODE_LEFT = 0x25,
	BUTTON_CODE_UP = 0x26,
	BUTTON_CODE_RIGHT = 0x27,
	BUTTON_CODE_DOWN = 0x28,
	BUTTON_CODE_SELECT = 0x29,
	BUTTON_CODE_PRINT = 0x2A,
	BUTTON_CODE_EXECUTE = 0x2B,
	BUTTON_CODE_SNAPSHOT = 0x2C,
	BUTTON_CODE_INSERT = 0x2D,
	BUTTON_CODE_DELETE = 0x2E,
	BUTTON_CODE_HELP = 0x2F,
	BUTTON_CODE_ZERO = 0x30, //TODO: Maybe 0..9
	BUTTON_CODE_ONE = 0x31,
	BUTTON_CODE_TWO = 0x32,
	BUTTON_CODE_THREE = 0x33,
	BUTTON_CODE_FOUR = 0x34,
	BUTTON_CODE_FIVE = 0x35,
	BUTTON_CODE_SIX = 0x36,
	BUTTON_CODE_SEVEN = 0x37,
	BUTTON_CODE_EIGHT = 0x38,
	BUTTON_CODE_NINE = 0x39,
	BUTTON_CODE_A = 0x41,
	BUTTON_CODE_B = 0x42,
	BUTTON_CODE_C = 0x43,
	BUTTON_CODE_D = 0x44,
	BUTTON_CODE_E = 0x45,
	BUTTON_CODE_F = 0x46,
	BUTTON_CODE_G = 0x47,
	BUTTON_CODE_H = 0x48,
	BUTTON_CODE_I = 0x49,
	BUTTON_CODE_J = 0x4A,
	BUTTON_CODE_K = 0x4B,
	BUTTON_CODE_L = 0x4C,
	BUTTON_CODE_M = 0x4D,
	BUTTON_CODE_N = 0x4E,
	BUTTON_CODE_O = 0x4F,
	BUTTON_CODE_P = 0x50,
	BUTTON_CODE_Q = 0x51,
	BUTTON_CODE_R = 0x52,
	BUTTON_CODE_S = 0x53,
	BUTTON_CODE_T = 0x54,
	BUTTON_CODE_U = 0x55,
	BUTTON_CODE_V = 0x56,
	BUTTON_CODE_W = 0x57,
	BUTTON_CODE_X = 0x58,
	BUTTON_CODE_Y = 0x59,
	BUTTON_CODE_Z = 0x5A,
	BUTTON_CODE_LWIN = 0x5B,
	BUTTON_CODE_RWIN = 0x5C,
	BUTTON_CODE_APPS = 0x5D,
	BUTTON_CODE_SLEEP = 0x5F,
	BUTTON_CODE_NUMPAD0 = 0x60,
	BUTTON_CODE_NUMPAD1 = 0x61,
	BUTTON_CODE_NUMPAD2 = 0x62,
	BUTTON_CODE_NUMPAD3 = 0x63,
	BUTTON_CODE_NUMPAD4 = 0x64,
	BUTTON_CODE_NUMPAD5 = 0x65,
	BUTTON_CODE_NUMPAD6 = 0x66,
	BUTTON_CODE_NUMPAD7 = 0x67,
	BUTTON_CODE_NUMPAD8 = 0x68,
	BUTTON_CODE_NUMPAD9 = 0x69,
	BUTTON_CODE_NPAD_MULTIPLY = 0x6A,
	BUTTON_CODE_NPAD_ADD = 0x6B,
	BUTTON_CODE_NPAD_SEPARATOR = 0x6C,
	BUTTON_CODE_NPAD_SUBTRACT = 0x6D,
	BUTTON_CODE_NPAD_DECIMAL = 0x6E,
	BUTTON_CODE_NPAD_DIVIDE = 0x6F,
	BUTTON_CODE_F1 = 0x70,
	BUTTON_CODE_F2 = 0x71,
	BUTTON_CODE_F3 = 0x72,
	BUTTON_CODE_F4 = 0x73,
	BUTTON_CODE_F5 = 0x74,
	BUTTON_CODE_F6 = 0x75,
	BUTTON_CODE_F7 = 0x76,
	BUTTON_CODE_F8 = 0x77,
	BUTTON_CODE_F9 = 0x78,
	BUTTON_CODE_F10 = 0x79,
	BUTTON_CODE_F11 = 0x7A,
	BUTTON_CODE_F12 = 0x7B,
	BUTTON_CODE_F13 = 0x7C,
	BUTTON_CODE_F14 = 0x7D,
	BUTTON_CODE_F15 = 0x7E,
	BUTTON_CODE_F16 = 0x7F,
	BUTTON_CODE_F17 = 0x80,
	BUTTON_CODE_F18 = 0x81,
	BUTTON_CODE_F19 = 0x82,
	BUTTON_CODE_F20 = 0x83,
	BUTTON_CODE_F21 = 0x84,
	BUTTON_CODE_F22 = 0x85,
	BUTTON_CODE_F23 = 0x86,
	BUTTON_CODE_F24 = 0x87,
	BUTTON_CODE_NAVIGATION_VIEW = 0x88,
	BUTTON_CODE_NAVIGATION_MENU = 0x89,
	BUTTON_CODE_NAVIGATION_UP = 0x8A,
	BUTTON_CODE_NAVIGATION_DOWN = 0x8B,
	BUTTON_CODE_NAVIGATION_LEFT = 0x8C,
	BUTTON_CODE_NAVIGATION_RIGHT = 0x8D,
	BUTTON_CODE_NAVIGATION_ACCEPT = 0x8E,
	BUTTON_CODE_NAVIGATION_CANCEL = 0x8F,
	BUTTON_CODE_NUMLOCK = 0x90,
	BUTTON_CODE_SCROLL = 0x91,
	BUTTON_CODE_OEM_NEC_EQUAL = 0x92,
	BUTTON_CODE_OEM_FJ_JISHO = 0x92,
	BUTTON_CODE_OEM_FJ_MASSHOU = 0x93,
	BUTTON_CODE_OEM_FJ_TOUROKU = 0x94,
	BUTTON_CODE_OEM_FJ_LOYA = 0x95,
	BUTTON_CODE_OEM_FJ_ROYA = 0x96,
	BUTTON_CODE_LSHIFT = 0xA0,
	BUTTON_CODE_RSHIFT = 0xA1,
	BUTTON_CODE_LCTRL = 0xA2,
	BUTTON_CODE_RCTRL = 0xA3,
	BUTTON_CODE_LALT = 0xA4,
	BUTTON_CODE_RALT = 0xA5,
	BUTTON_CODE_BROWSER_BACK = 0xA6,
	BUTTON_CODE_BROWSER_FORWARD = 0xA7,
	BUTTON_CODE_BROWSER_REFRESH = 0xA8,
	BUTTON_CODE_BROWSER_STOP = 0xA9,
	BUTTON_CODE_BROWSER_SEARCH = 0xAA,
	BUTTON_CODE_BROWSER_FAVORITES = 0xAB,
	BUTTON_CODE_BROWSER_HOME = 0xAC,
	BUTTON_CODE_VOLUME_MUTE = 0xAD,
	BUTTON_CODE_VOLUME_DOWN = 0xAE,
	BUTTON_CODE_VOLUME_UP = 0xAF,
	BUTTON_CODE_MEDIA_NEXT_TRACK = 0xB0,
	BUTTON_CODE_MEDIA_PREV_TRACK = 0xB1,
	BUTTON_CODE_MEDIA_STOP = 0xB2,
	BUTTON_CODE_MEDIA_PLAY_PAUSE = 0xB3,
	BUTTON_CODE_LAUNCH_MAIL = 0xB4,
	BUTTON_CODE_LAUNCH_MEDIA_SELECT = 0xB5,
	BUTTON_CODE_LAUNCH_APP1 = 0xB6,
	BUTTON_CODE_LAUNCH_APP2 = 0xB7,
	BUTTON_CODE_OEM_SEMICOLON = 0xBA,
	BUTTON_CODE_OEM_PLUS = 0xBB,
	BUTTON_CODE_OEM_COMMA = 0xBC,
	BUTTON_CODE_OEM_MINUS = 0xBD,
	BUTTON_CODE_OEM_PERIOD = 0xBE,
	BUTTON_CODE_OEM_FORWARD_SLASH = 0xBF,
	BUTTON_CODE_OEM_TILDE = 0xC0,
	BUTTON_CODE_GAMEPAD_A = 0xC3,
	BUTTON_CODE_GAMEPAD_B = 0xC4,
	BUTTON_CODE_GAMEPAD_X = 0xC5,
	BUTTON_CODE_GAMEPAD_Y = 0xC6,
	BUTTON_CODE_GAMEPAD_RIGHT_SHOULDER = 0xC7,
	BUTTON_CODE_GAMEPAD_LEFT_SHOULDER = 0xC8,
	BUTTON_CODE_GAMEPAD_LEFT_TRIGGER = 0xC9,
	BUTTON_CODE_GAMEPAD_RIGHT_TRIGGER = 0xCA,
	BUTTON_CODE_GAMEPAD_DPAD_UP = 0xCB,
	BUTTON_CODE_GAMEPAD_DPAD_DOWN = 0xCC,
	BUTTON_CODE_GAMEPAD_DPAD_LEFT = 0xCD,
	BUTTON_CODE_GAMEPAD_DPAD_RIGHT = 0xCE,
	BUTTON_CODE_GAMEPAD_MENU = 0xCF,
	BUTTON_CODE_GAMEPAD_VIEW = 0xD0,
	BUTTON_CODE_GAMEPAD_LEFT_THUMBSTICK_BUTTON = 0xD1,
	BUTTON_CODE_GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2,
	BUTTON_CODE_GAMEPAD_LEFT_THUMBSTICK_UP = 0xD3,
	BUTTON_CODE_GAMEPAD_LEFT_THUMBSTICK_DOWN = 0xD4,
	BUTTON_CODE_GAMEPAD_LEFT_THUMBSTICK_RIGHT = 0xD5,
	BUTTON_CODE_GAMEPAD_LEFT_THUMBSTICK_LEFT = 0xD6,
	BUTTON_CODE_GAMEPAD_RIGHT_THUMBSTICK_UP = 0xD7,
	BUTTON_CODE_GAMEPAD_RIGHT_THUMBSTICK_DOWN = 0xD8,
	BUTTON_CODE_GAMEPAD_RIGHT_THUMBSTICK_RIGHT = 0xD9,
	BUTTON_CODE_GAMEPAD_RIGHT_THUMBSTICK_LEFT = 0xDA,
	BUTTON_CODE_OEM_OBRACKET = 0xDB,
	BUTTON_CODE_OEM_BACK_SLASH = 0xDC,
	BUTTON_CODE_OEM_CBRACKET = 0xDD,
	BUTTON_CODE_OEM_QUOTES = 0xDE,
	BUTTON_CODE_OEM_8 = 0xDF,
	BUTTON_CODE_OEM_AX = 0xE1,  //  'AX' key on Japanese AX kbd
	BUTTON_CODE_OEM_102 = 0xE2,  //  "<>" or "\|" on RT 102-key kbd.
	BUTTON_CODE_ICO_HELP = 0xE3,  //  Help key on ICO
	BUTTON_CODE_ICO_00 = 0xE4,  //  00 key on ICO
	BUTTON_CODE_PROCESS_KEY = 0xE5,
	BUTTON_CODE_ICO_CLEAR = 0xE6,
	BUTTON_CODE_PACKET = 0xE7,
	BUTTON_CODE_OEM_RESET = 0xE9,
	BUTTON_CODE_OEM_JUMP = 0xEA,
	BUTTON_CODE_OEM_PA1 = 0xEB,
	BUTTON_CODE_OEM_PA2 = 0xEC,
	BUTTON_CODE_OEM_PA3 = 0xED,
	BUTTON_CODE_OEM_WSCTRL = 0xEE,
	BUTTON_CODE_OEM_CUSEL = 0xEF,
	BUTTON_CODE_OEM_ATTN = 0xF0,
	BUTTON_CODE_OEM_FINISH = 0xF1,
	BUTTON_CODE_OEM_COPY = 0xF2,
	BUTTON_CODE_OEM_AUTO = 0xF3,
	BUTTON_CODE_OEM_ENLW = 0xF4,
	BUTTON_CODE_OEM_BACKTAB = 0xF5,
	BUTTON_CODE_ATTN = 0xF6,
	BUTTON_CODE_CRSEL = 0xF7,
	BUTTON_CODE_EXSEL = 0xF8,
	BUTTON_CODE_EREOF = 0xF9,
	BUTTON_CODE_PLAY = 0xFA,
	BUTTON_CODE_ZOOM = 0xFB,
	BUTTON_CODE_NONAME = 0xFC,
	BUTTON_CODE_PA1 = 0xFD,
	BUTTON_CODE_OEM_CLEAR = 0xFE,

	BUTTON_CODE_ANY,
	BUTTON_CODE_SCROLL_WHEEL,
	BUTTON_CODE_COUNT
};

enum AxisCode
{
	AXIS_CODE_LEFT_JOYSTICK_X,
	AXIS_CODE_LEFT_JOYSTICK_Y,
	AXIS_CODE_RIGHT_JOYSTICK_X,
	AXIS_CODE_RIGHT_JOYSTICK_Y,
	AXIS_CODE_LEFT_TRIGGER,
	AXIS_CODE_RIGHT_TRIGGER,

	AXIS_CODE_COUNT
};

struct Device;
struct HRAWINPUT__;

class NH_API Input
{
	struct ButtonState
	{
		bool pressed;
		bool changed;
		bool doubleClicked;
		bool held;
		bool heldChanged;
		//TODO: Use Time::AbsoluteTime() to check if being held
	};

public:
	static bool OnAnyButtonDown();
	static bool OnAnyButtonChanged();
	static bool ButtonUp(ButtonCode code);
	static bool ButtonDown(ButtonCode code);
	static bool ButtonHeld(ButtonCode code);
	static bool ButtonDragging(ButtonCode code);
	static bool OnButtonUp(ButtonCode code);
	static bool OnButtonDown(ButtonCode code);
	static bool OnButtonChange(ButtonCode code);
	static bool OnButtonDoubleClick(ButtonCode code);
	static bool OnButtonHold(ButtonCode code);
	static bool OnButtonRelease(ButtonCode code);
	static void MousePos(I32& x, I32& y);
	static void MouseDelta(I32& x, I32& y);
	static void PreviousMousePos(I32& x, I32& y);
	static void ConsumeInput();
	static I16 MouseWheelDelta();
	static I16 MouseHWheelDelta();

	static F32 GetAxis(AxisCode code);

private:
	static bool Initialize();
	static void Shutdown();

	static void Update();
	static void ReceiveInput(HRAWINPUT__* handle);
	static void InputSink(HRAWINPUT__* handle);
	static void AddDevice(void* handle);
	static void RemoveDevice(void* handle);

	static Vector<Device> devices;

	//MOUSE
	static F32 mouseSensitivity;
	static I16 mouseWheelDelta;
	static I16 mouseHWheelDelta;
	static F32 mousePosX;
	static F32 mousePosY;
	static F32 deltaMousePosX;
	static F32 deltaMousePosY;

	//CONTROLLER
	static F32 axisStates[];

	//ALL
	static ButtonState buttonStates[];
	static bool receiveInput;
	static bool anyButtonDown;
	static bool anyButtonChanged;

	STATIC_CLASS(Input);
	friend class Engine;
	friend class Platform;
};