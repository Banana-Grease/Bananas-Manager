#pragma once
#include "Includes.h"

using namespace std;

class KeyboardInputClass {
public:
	enum Keyboard {
		CTRL = VK_LCONTROL,
		One = 49,
		Two = 50,
		Three = 51,
		Four = 52,
		Five = 53
	};
	
	// checks if the user is pressing CTRL and a number
	bool PressingCTRLAnd(int KeyIndentifier) {
		if (GetKeyState(VK_LCONTROL) & ActiveKey && GetKeyState(KeyIndentifier) & ActiveKey)
			return true;
		else
			return false;
	}
};