#pragma once
#include "Includes.h"

struct Password {
	std::string EmailText;
	std::string PasswordText;
	const char* FilePath;

	//constructor
	Password() {
		#include "Functions.h"
		
	}
};