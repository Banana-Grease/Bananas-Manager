#pragma once
#include "Includes.h"
#include "KeyboardInput.h"
#include "PseudoRandomGenerator.h"

using namespace std;

class DisplayClass {
    bool DoRefreshScreen = true;
    vector<string> Title = { // this is the title for the application (Always displayed at the top)
    "    ____                                _       __  __                                   ",
    "   |  _ \\                              ( )     |  \\/  |                                  ",
    "   | |_) | __ _ _ __   __ _ _ __   __ _|/ ___  | \\  / | __ _ _ __   __ _  __ _  ___ _ __ ",
    "   |  _ < / _` | '_ \\ / _` | '_ \\ / _` | / __| | |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '__|",
    "   | |_) | (_| | | | | (_| | | | | (_| | \\__ \\ | |  | | (_| | | | | (_| | (_| |  __/ |   ",
    "   |____/ \\__,_|_| |_|\\__,_|_| |_|\\__,_| |___/ |_|  |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_|   ",
    "                                                                          __/ |          ",
    "                                                                         |___/           ",
    };
    void PrintTitle(vector<string> Title) { // litteraly just prints the above vector onto the screen
        system("cls");
        for (int i = 0; i < Title.size(); i++)
            cout << Title[i] << "\n";
        cout << "\n\n";
    }

    // takes inputs as a hashmap and outputs a serries of 'ctrl + 1 -> Create Password' , etc
    // this hashmap contains the 'ctrl + x' as the key, and the string as the printed option text
    void PrintKeyboardOptions(unordered_map<int, string> HashMap) {
        for (int i = 1; i < HashMap.size() + 1; i++)
            cout << "   CTRL + " << i << " -> " << HashMap[i] << "\n\n";
    }

    // print the main screen
    void PrintMainScreen() {
        PrintTitle(Title);
        // generating hashmap to display user options
        unordered_map<int, string> ControlMap;
        ControlMap.insert({ 1, "View Passwords" });
        ControlMap.insert({ 2, "Create New Password" });
        ControlMap.insert({ 3, "Edit Existing Password" });
        ControlMap.insert({ 4, "Delete Existing Password" });
        PrintKeyboardOptions(ControlMap);
    }

    // print password select screen
    void PrintPasswordSelectScreen(vector<PasswordStruct> SelectablePasswords) {
        PrintTitle(Title);
        cout << "   Please Type The Number For a Password" << "\n\n";
        for (int i = 0; i < SelectablePasswords.size(); i++) {
            cout << "   " << i << " - " << SelectablePasswords[i].PasswordName << "\n\n";
        }
        cout << "\n   -- ";
    }

    // print selected password data
    void PrintPassword(PasswordStruct TargetPassword) {
        PrintTitle(Title);
        cout << "   Password Name   :  \"" << TargetPassword.PasswordName << "\"\n\n";
        cout << "   Password Email  :  \"" << TargetPassword.EmailContent<< "\"\n\n";
        cout << "   Password        :  \"" << TargetPassword.PasswordContent << "\"\n\n\n\n\n   ";
    }

public:
    // enums so identifying what screen should be displayed is easier
    enum CurrentScreen {
        PasswordSelect = 5,
        Main = 10,
        ViewPassword = 20,
        GeneratePasswords = 30,
        EditPasswords = 40,
        DeletePasswords = 50
    };

    int CurrentScreenIdentifier = Main;

    // refreshes the screen and handles almost all the logic behind taking input to select password and displaying and most things really. WHAT A HUNK
    // when this function is completed it will automatically refresh to main due to 'Banana's Manager.cpp' having another refresh straight away
    void RefreshScreen(int DesiredScreenIdentifier,int NextScreenIdentifier = -1, vector<PasswordStruct> LoadedPasswords = {}) {
        if (DesiredScreenIdentifier != CurrentScreenIdentifier) { DoRefreshScreen = true; } // if the current screen and target screen do not match, 'DoRefreshScreen = true'
        if (!DoRefreshScreen) { return; } // if the screen is not to be refreshed, return from the func call
        DoRefreshScreen = false;
        CurrentScreenIdentifier = DesiredScreenIdentifier;
        CSVDataBase PasswordDB; string PasswordDirectory; vector<string> NewPasswordData; KeyboardInputClass Keyboard; unordered_map<int, string> HashMap; PseudoRandomGenerator PRG; // used to generate stuff later in the case situations
    Start: // TODO: move all of the logic out of the switch cases into their own class / functions. all i really want in the case statements is drawing and calls to the previosly mentioned classes / functions
        switch (DesiredScreenIdentifier) {

            case PasswordSelect:
                PrintPasswordSelectScreen(LoadedPasswords);
                int PasswordChoice;
                cin >> PasswordChoice;
                DesiredScreenIdentifier = NextScreenIdentifier;
                goto Start;
                break;
            
            case Main:
                PrintMainScreen();
                break;
            
            case ViewPassword:
                PrintPassword(LoadedPasswords[PasswordChoice]);
                system("pause");
                break;

            case GeneratePasswords:
                for (int i = 0; i < 3; i++)
                    NewPasswordData.push_back("");

                PrintTitle(Title);
                cout << "   What is the Name of the service: ";
                cin >> NewPasswordData[PasswordDB.PasswordName];
                system("cls");
                PrintTitle(Title);
                cout << "   What is the Email Connected to the service: ";
                cin >> NewPasswordData[PasswordDB.PasswordEmail];
                PrintTitle(Title);
                HashMap.clear();
                HashMap.insert({ 1, "Input Custom Password" });
                HashMap.insert({ 2, "Generate Random Password" });
                PrintKeyboardOptions(HashMap);
                for (;; Sleep(5)) { // THIS FOR LOOP WAS STOLEN FROM EDIIT PASSWORDS AS THAT PASSWORD SELECTOR WAS WRITTEN FIRST. TODO: WRITE A FUNCTION THAT DOES THIS SO I DONT HAVE TO COPY PASTE CODE (rushed and copied to to first release being delayed)
                    if (Keyboard.PressingCTRLAnd(Keyboard.One)) { // edit password to custom password
                        PrintTitle(Title);
                        cout << "   Password: ";
                        cin >> NewPasswordData[PasswordDB.PasswordContent];
                        PasswordDB.WriteDataToFile(PasswordDirectory.c_str(), NewPasswordData, NewPasswordData[PasswordDB.PasswordName]);

                        break;
                    }
                    else if (Keyboard.PressingCTRLAnd(Keyboard.Two)) { // edit password to random generated password
                        PrintTitle(Title);
                        cout << "   Password Length: ";
                        string NewPasswordLength;
                        cin >> NewPasswordLength;
                        NewPasswordData[PasswordDB.PasswordContent] = PRG.GetPseudoRandomString(stoi(NewPasswordLength)); // get random string based on input
                        PasswordDB.WriteDataToFile(PasswordDirectory.c_str(), NewPasswordData, NewPasswordData[PasswordDB.PasswordName]);

                        break;
                    }
                }

                // after passwords are chosen wite data :)
                PasswordDirectory = PasswordDB.AppDirectory; PasswordDirectory.append(NewPasswordData[PasswordDB.PasswordName] + ".txt");
                PasswordDB.WriteDataToFile(PasswordDirectory.c_str(), NewPasswordData, NewPasswordData[PasswordDB.PasswordName]);
                break;

            case EditPasswords:
                PrintTitle(Title);
                HashMap.insert({1, "Edit Email"});
                HashMap.insert({ 2, "Edit Password" });
                HashMap.insert({ 3, "Cancel" });
                PrintKeyboardOptions(HashMap); // print the ctrl + (1-3) options
                PasswordDirectory = PasswordDB.AppDirectory;
                PasswordDirectory.append(LoadedPasswords[PasswordChoice].PasswordName + ".txt");
                NewPasswordData = PasswordDB.GetDataFromFile(PasswordDirectory.c_str(), -1); // load old password data so it can be replaced
                for (;; Sleep(5)) { // for loop used to detect the key press options
                    // new email
                    if (Keyboard.PressingCTRLAnd(Keyboard.One)) {
                        PrintTitle(Title);
                        cout << "   Old Email      : " << NewPasswordData[PasswordDB.PasswordEmail] << "\n\n";
                        cout << "   Enter New Email: ";
                        cin >> NewPasswordData[PasswordDB.PasswordEmail];
                        PasswordDB.WriteDataToFile(PasswordDirectory.c_str(), NewPasswordData, NewPasswordData[PasswordDB.PasswordName]); // confirmed vector crash here (maybe?? it just stopped and idk why. no more windows crashes ig)
                        break;
                    }
                    // new password
                    else if (Keyboard.PressingCTRLAnd(Keyboard.Two)) {
                        HashMap.clear();
                        HashMap.insert({ 1, "Input Custom Password" });
                        HashMap.insert({ 2, "Generate Random Password" });
                        PrintTitle(Title);
                        PrintKeyboardOptions(HashMap);
                        Sleep(150); // prevent the key press going through 2 check
                        for (;; Sleep(5)) { // used to detect keys
                            if (Keyboard.PressingCTRLAnd(Keyboard.One)) { // edit password to custom password
                                PrintTitle(Title);
                                cout << "   Old Password: " << NewPasswordData[PasswordDB.PasswordContent] << "\n\n";
                                cout << "   New Password: ";
                                cin >> NewPasswordData[PasswordDB.PasswordContent];
                                PasswordDB.WriteDataToFile(PasswordDirectory.c_str(), NewPasswordData, NewPasswordData[PasswordDB.PasswordName]);

                                // break doesnt work here so set DesiredScreenIndentifier to main
                                DesiredScreenIdentifier = Main;
                                goto Start; // gets user back to main screen and breaks out of this refresh screen func
                            }
                            else if (Keyboard.PressingCTRLAnd(Keyboard.Two)) { // edit password to random generated password
                                PrintTitle(Title);
                                cout << "   Password Length: ";
                                string NewPasswordLength;
                                cin >> NewPasswordLength;
                                NewPasswordData[PasswordDB.PasswordContent] = PRG.GetPseudoRandomString(stoi(NewPasswordLength)); // get random string based on input
                                PasswordDB.WriteDataToFile(PasswordDirectory.c_str(), NewPasswordData, NewPasswordData[PasswordDB.PasswordName]);

                                // break doesnt work here so set DesiredScreenIndentifier to main
                                DesiredScreenIdentifier = Main;
                                goto Start; // gets user back to main screen and breaks out of this refresh screen func
                            }
                        }
                    }
                    // cancel
                    if (Keyboard.PressingCTRLAnd(Keyboard.Three)) {
                        break;
                    }
                }
                break;

            case DeletePasswords:
                PrintTitle(Title);
                cout << "   Deleting \"" << LoadedPasswords[PasswordChoice].PasswordName << "\"                    (Type 'yes' in lowercase to confirm, anything else will cancel)\n\n\n";
                cout << "   Are You Sure???\n\n-- ";
                string Choice;
                cin >> Choice;
                if (Choice == "yes") {
                    // delete the password :shrug: making sure it is removed from the index too
                    PasswordDB.RemoveFile(LoadedPasswords[PasswordChoice].FilePath.c_str(), true, LoadedPasswords[PasswordChoice].PasswordName);
                }

                break;
        }
    }
};