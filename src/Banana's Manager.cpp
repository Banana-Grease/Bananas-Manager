#include "Includes.h"
#include "FileManagement.h"
#include "Display.h"
#include "KeyboardInput.h"
#include "PseudoRandomGenerator.h"

using namespace std;

int main()
{
	PseudoRandomGenerator PRG;
	DisplayClass Screen;
	KeyboardInputClass Keyboard;
	CSVDataBase PasswordDB; // database fiilesystem used to store and retrieve passwords

	// make sure directory and files exist
	PasswordDB.BootDirectoryCheck();

	// after directory verification, get passwords
	vector<PasswordStruct> LoadedPasswords = PasswordDB.GetPasswords(); // this is where the passwords will be stored that are currently 'loaded'

	// set console title
	SetConsoleTitleA(AppTitle);

	// debug code section here ---------------


	//system("pause");

	// end debug code section here -----------

	for (; ; Sleep(5)) { // main for loop of the program (repeats every 5 ms)

		// View Passwords (CTRL + 1)
		if (Keyboard.PressingCTRLAnd(Keyboard.One)) {
			Screen.RefreshScreen(Screen.PasswordSelect, Screen.ViewPassword, LoadedPasswords);
		}

		// Create Password (CTRL + 2)
		if (Keyboard.PressingCTRLAnd(Keyboard.Two)) {
			Screen.RefreshScreen(Screen.GeneratePasswords);
			LoadedPasswords = PasswordDB.GetPasswords();
		}

		// Edit Password (CTRL + 3)
		if (Keyboard.PressingCTRLAnd(Keyboard.Three)) {
			Screen.RefreshScreen(Screen.PasswordSelect, Screen.EditPasswords, LoadedPasswords);
			LoadedPasswords = PasswordDB.GetPasswords();
		}

		// Delete Password (CTRL + 4)
		if (Keyboard.PressingCTRLAnd(Keyboard.Four)) {
			Screen.RefreshScreen(Screen.PasswordSelect, Screen.DeletePasswords, LoadedPasswords);
			LoadedPasswords = PasswordDB.GetPasswords();
		}

		// Export Passwords (CTRL + 5)
		if (Keyboard.PressingCTRLAnd(Keyboard.Five)) {
			PasswordDB.GeneratePasswordSummary();
		}

		Screen.RefreshScreen(Screen.Main); // print main screen if needed / just came out of a Screen.Refresh cycle!
	}
}