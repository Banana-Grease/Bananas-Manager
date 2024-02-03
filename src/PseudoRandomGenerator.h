#pragma once
#include "Includes.h"
#include "FileManagement.h"

using namespace std;

// PRSG for short :)
// the idea is to generate as much of a random string as i can, it wont be that important to be truely random as there isnt a high likely hood of similarities being detected in 32 long string.
class PseudoRandomGenerator {
	string LegalCharsFileName = "PseudoLegalChars.txt";

public:
	// returns each character that is LEGAL for the randomstring generator. each character is represented by a string object.
	vector<string> GetLegalCharsAsStrings() {
		CSVDataBase DB;
		vector<string> LegalChars = { "a", "A", "b", "B", "c", "C", "d", "D", "e", "E", "f", "F", "g", "G", "h", "H", "i", "I", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "o", "O", "p", "P", "q", "Q", "r", "R", "s", "S", "t", "T", "u", "U", "v", "V", "w", "W", "x", "X", "y", "Y", "z", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "!", ".", "#", "$", "(", ")" };
		string LegalCharFileDirectory = "";
		// loop through the vector of folders to the second last one
		for (int i = 0; i < DB.AppDirectoryVector.size() - 1; i++) {
			LegalCharFileDirectory += DB.AppDirectoryVector[i];
		}
		//finalise the directory
		LegalCharFileDirectory.append(LegalCharsFileName);
		// check to see if the file exists
		ifstream ifs;
		ifs.open(LegalCharFileDirectory);
		if (!ifs.is_open()) { // if it doesnt exist write deafult data to a new file called that
			DB.WriteDataToFile(LegalCharFileDirectory.c_str(), LegalChars); // dont include file name so nothing gets added to any index
			return LegalChars; // stop here because rest of code is flushing legalchars and reading from the text file
		}

		// if the file did exist
		LegalChars.clear(); // remove all indexes
		LegalChars = DB.GetDataFromFile(LegalCharFileDirectory.c_str(), -1);
		return LegalChars;
	}

	int PseudoRandomNumber(int IncludedMin, int IncludedMax) { // mostly stolen code lol
		// initialize some better random number shit that doesn't repeat like rand()
		std::random_device device;
		std::mt19937 generator(device());
		std::uniform_int_distribution<int> distribution(IncludedMin, IncludedMax - 1);

		return distribution(generator);
	}

	// uses a fancy new random generator ive never heard of before and at a glance it looks pretty random
	string GetPseudoRandomString(size_t ReturnStringLength) {
		string PseudoRandomString = "";

		vector<string> LegalChars = GetLegalCharsAsStrings();

		for (int i = 0; i < ReturnStringLength; i++) {
			PseudoRandomString.append(LegalChars[PseudoRandomNumber(0, LegalChars.size())]); // appends a random index of legalchars
		}

		return PseudoRandomString;
	}
};