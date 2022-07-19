// ------------------------------------------------------------------------------
// 
// Rebirth
//    Copyright 2022 Matthew Rogers
// 
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// 
// File Name: InputCodes.h
// Date File Created: 7/13/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once


namespace rebirth
{

	enum class KeyCode : uint16
	{

		SPACE			= 32,
		APOSTROPHE		= 39,  /* ' */
		COMMA			= 44,  /* , */
		MINUS			= 45,  /* - */
		PERIOD			= 46,  /* . */
		SLASH			= 47,  /* / */
		D0				= 48,
		D1				= 49,
		D2				= 50,
		D3				= 51,
		D4				= 52,
		D5				= 53,
		D6				= 54,
		D7				= 55,
		D8				= 56,
		D9				= 57,
		SEMICOLON		= 59,  /* ; */
		EQUAL			= 61,  /* = */
		A				= 65,
		B				= 66,
		C				= 67,
		D				= 68,
		E				= 69,
		F				= 70,
		G				= 71,
		H				= 72,
		I				= 73,
		J				= 74,
		K				= 75,
		L				= 76,
		M				= 77,
		N				= 78,
		O				= 79,
		P				= 80,
		Q				= 81,
		R				= 82,
		S				= 83,
		T				= 84,
		U				= 85,
		V				= 86,
		W				= 87,
		X				= 88,
		Y				= 89,
		Z				= 90,
		LEFT_BRACKET	= 91,  /* [ */
		BACKSLASH		= 92,  /* \ */
		RIGHT_BRACKET	= 93,  /* ] */
		GRAVE_ACCENT	= 96,  /* ` */
		WORLD_1			= 161, /* non-US #1 */
		WORLD_2			= 162, /* non-US #2 */

		ESCAPE          = 256,
		ENTER           = 257,
		TAB             = 258,
		BACKSPACE       = 259,
		INSERT          = 260,
		DELETE_KEY      = 261,
		RIGHT           = 262,
		LEFT            = 263,
		DOWN            = 264,
		UP              = 265,
		PAGE_UP         = 266,
		PAGE_DOWN       = 267,
		HOME            = 268,
		END             = 269,
		CAPS_LOCK       = 280,
		SCROLL_LOCK     = 281,
		NUM_LOCK        = 282,
		PRINT_SCREEN    = 283,
		PAUSE           = 284,
		F1              = 290,
		F2              = 291,
		F3              = 292,
		F4              = 293,
		F5              = 294,
		F6              = 295,
		F7              = 296,
		F8              = 297,
		F9              = 298,
		F10             = 299,
		F11             = 300,
		F12             = 301,
		F13             = 302,
		F14             = 303,
		F15             = 304,
		F16             = 305,
		F17             = 306,
		F18             = 307,
		F19             = 308,
		F20             = 309,
		F21             = 310,
		F22             = 311,
		F23             = 312,
		F24             = 313,
		F25             = 314,
		NUMPAD_0        = 320,
		NUMPAD_1        = 321,
		NUMPAD_2        = 322,
		NUMPAD_3        = 323,
		NUMPAD_4        = 324,
		NUMPAD_5        = 325,
		NUMPAD_6        = 326,
		NUMPAD_7        = 327,
		NUMPAD_8        = 328,
		NUMPAD_9        = 329,
		NUMPAD_DECIMAL  = 330,
		NUMPAD_DIVIDE   = 331,
		NUMPAD_MULTIPLY = 332,
		NUMPAD_SUBTRACT = 333,
		NUMPAD_ADD      = 334,
		NUMPAD_ENTER    = 335,
		NUMPAD_EQUAL    = 336,
		LEFT_SHIFT      = 340,
		LEFT_CONTROL    = 341,
		LEFT_ALT        = 342,
		LEFT_SUPER      = 343,
		RIGHT_SHIFT     = 344,
		RIGHT_CONTROL   = 345,
		RIGHT_ALT       = 346,
		RIGHT_SUPER     = 347,
		MENU            = 348

	};


	enum class MouseButton : uint16
	{
		// Rebirth buttons start at "Button 1" instead of 0. Many games refer to the left mouse button as button 1, not 0
		B1     = 0,
		B2     = 1,
		B3     = 2,
		B4     = 3,
		B5     = 4,
		B6     = 5,
		B7     = 6,
		B8     = 7,
		LAST   = B8,
		LEFT   = B1,
		RIGHT  = B2,
		MIDDLE = B3
	};


	inline std::ostream& operator<<(std::ostream& os, KeyCode keycode)
	{
		os << (uint32)keycode;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, MouseButton button)
	{
		os << (uint32)button;
		return os;
	}
}

// modified from glfw3.h

/* The unknown key */
#define RB_KEY_UNKNOWN            -1

/* Printable keys */
#define RB_KEY_SPACE              32
#define RB_KEY_APOSTROPHE         39  /* ' */
#define RB_KEY_COMMA              44  /* , */
#define RB_KEY_MINUS              45  /* - */
#define RB_KEY_PERIOD             46  /* . */
#define RB_KEY_SLASH              47  /* / */
#define RB_KEY_0                  48
#define RB_KEY_1                  49
#define RB_KEY_2                  50
#define RB_KEY_3                  51
#define RB_KEY_4                  52
#define RB_KEY_5                  53
#define RB_KEY_6                  54
#define RB_KEY_7                  55
#define RB_KEY_8                  56
#define RB_KEY_9                  57
#define RB_KEY_SEMICOLON          59  /* ; */
#define RB_KEY_EQUAL              61  /* = */
#define RB_KEY_A                  65
#define RB_KEY_B                  66
#define RB_KEY_C                  67
#define RB_KEY_D                  68
#define RB_KEY_E                  69
#define RB_KEY_F                  70
#define RB_KEY_G                  71
#define RB_KEY_H                  72
#define RB_KEY_I                  73
#define RB_KEY_J                  74
#define RB_KEY_K                  75
#define RB_KEY_L                  76
#define RB_KEY_M                  77
#define RB_KEY_N                  78
#define RB_KEY_O                  79
#define RB_KEY_P                  80
#define RB_KEY_Q                  81
#define RB_KEY_R                  82
#define RB_KEY_S                  83
#define RB_KEY_T                  84
#define RB_KEY_U                  85
#define RB_KEY_V                  86
#define RB_KEY_W                  87
#define RB_KEY_X                  88
#define RB_KEY_Y                  89
#define RB_KEY_Z                  90
#define RB_KEY_LEFT_BRACKET       91  /* [ */
#define RB_KEY_BACKSLASH          92  /* \ */
#define RB_KEY_RIGHT_BRACKET      93  /* ] */
#define RB_KEY_GRAVE_ACCENT       96  /* ` */
#define RB_KEY_WORLD_1            161 /* non-US #1 */
#define RB_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define RB_KEY_ESCAPE             256
#define RB_KEY_ENTER              257
#define RB_KEY_TAB                258
#define RB_KEY_BACKSPACE          259
#define RB_KEY_INSERT             260
#define RB_KEY_DELETE             261
#define RB_KEY_RIGHT              262
#define RB_KEY_LEFT               263
#define RB_KEY_DOWN               264
#define RB_KEY_UP                 265
#define RB_KEY_PAGE_UP            266
#define RB_KEY_PAGE_DOWN          267
#define RB_KEY_HOME               268
#define RB_KEY_END                269
#define RB_KEY_CAPS_LOCK          280
#define RB_KEY_SCROLL_LOCK        281
#define RB_KEY_NUM_LOCK           282
#define RB_KEY_PRINT_SCREEN       283
#define RB_KEY_PAUSE              284
#define RB_KEY_F1                 290
#define RB_KEY_F2                 291
#define RB_KEY_F3                 292
#define RB_KEY_F4                 293
#define RB_KEY_F5                 294
#define RB_KEY_F6                 295
#define RB_KEY_F7                 296
#define RB_KEY_F8                 297
#define RB_KEY_F9                 298
#define RB_KEY_F10                299
#define RB_KEY_F11                300
#define RB_KEY_F12                301
#define RB_KEY_F13                302
#define RB_KEY_F14                303
#define RB_KEY_F15                304
#define RB_KEY_F16                305
#define RB_KEY_F17                306
#define RB_KEY_F18                307
#define RB_KEY_F19                308
#define RB_KEY_F20                309
#define RB_KEY_F21                310
#define RB_KEY_F22                311
#define RB_KEY_F23                312
#define RB_KEY_F24                313
#define RB_KEY_F25                314
#define RB_KEY_NUMPAD_0           320
#define RB_KEY_NUMPAD_1           321
#define RB_KEY_NUMPAD_2           322
#define RB_KEY_NUMPAD_3           323
#define RB_KEY_NUMPAD_4           324
#define RB_KEY_NUMPAD_5           325
#define RB_KEY_NUMPAD_6           326
#define RB_KEY_NUMPAD_7           327
#define RB_KEY_NUMPAD_8           328
#define RB_KEY_NUMPAD_9           329
#define RB_KEY_NUMPAD_DECIMAL     330
#define RB_KEY_NUMPAD_DIVIDE      331
#define RB_KEY_NUMPAD_MULTIPLY    332
#define RB_KEY_NUMPAD_SUBTRACT    333
#define RB_KEY_NUMPAD_ADD         334
#define RB_KEY_NUMPAD_ENTER       335
#define RB_KEY_NUMPAD_EQUAL       336
#define RB_KEY_LEFT_SHIFT         340
#define RB_KEY_LEFT_CONTROL       341
#define RB_KEY_LEFT_ALT           342
#define RB_KEY_LEFT_SUPER         343
#define RB_KEY_RIGHT_SHIFT        344
#define RB_KEY_RIGHT_CONTROL      345
#define RB_KEY_RIGHT_ALT          346
#define RB_KEY_RIGHT_SUPER        347
#define RB_KEY_MENU               348





// Mouse
#define RB_MOUSE_BUTTON_1         0
#define RB_MOUSE_BUTTON_2         1
#define RB_MOUSE_BUTTON_3         2
#define RB_MOUSE_BUTTON_4         3
#define RB_MOUSE_BUTTON_5         4
#define RB_MOUSE_BUTTON_6         5
#define RB_MOUSE_BUTTON_7         6
#define RB_MOUSE_BUTTON_8         7
#define RB_MOUSE_BUTTON_LAST      RB_MOUSE_BUTTON_8
#define RB_MOUSE_BUTTON_LEFT      RB_MOUSE_BUTTON_1
#define RB_MOUSE_BUTTON_RIGHT     RB_MOUSE_BUTTON_2
#define RB_MOUSE_BUTTON_MIDDLE    RB_MOUSE_BUTTON_3