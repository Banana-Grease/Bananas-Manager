#pragma once
#include "Includes.h"
#include "PasswordClass.h"

using namespace std;

struct CSVDataBase {
	// note, the AppDirectoryVector should be set to point to the file where the data will be stored
	// The AppDirectory should point to the same place, just without being 'splitup'
	// The Indexdirectory SHOULD also be set to point to the index.txt file
	vector<const char*> AppDirectoryVector = { "C:", "\\ProgramFiles\\", "x86\\", "BananasManager\\", "Data\\", "Passwords\\" };
	const char* IndexDirectory = "C:\\ProgramFiles\\x86\\BananasManager\\Data\\index.txt";
	const char* AppDirectory = "C:\\ProgramFiles\\x86\\BananasManager\\Data\\Passwords\\";

	enum DataPositions{
		PasswordName = 0,
		PasswordEmail = 1,
		PasswordContent = 2
	};

	const char* GetDirectory(int AmountOfArgs, string Directories[]) {
		string BDirect;
		for (int i = 0; i < AmountOfArgs; i++) {
			BDirect += Directories[i];
		}
		const char* FDirect = BDirect.c_str();
	}

	void BootDirectoryCheck() {
		// make each directory to make sure it exists using mk_dir
		string MkDirect = "";
		for (int i = 0; i < AppDirectoryVector.size(); i++) {
			MkDirect += AppDirectoryVector[i];
			_mkdir(MkDirect.c_str());
		}
		//---- index.txt
		// check to see if index exists
		ifstream ifs;
		ifs.open(IndexDirectory);
		if (ifs.is_open()) {
			ifs.close(); // close ifs to prevent memory leak
			return; // dont do anything its fine
		}
		else { // make the file with ofs and populate both at once with WriteDataToFile
			ofstream ofs;
			ofs.open(IndexDirectory);
			ofs.close(); // prevent memory leak
		}
		
		// write a warning password and correspoding segment in index.txt
		vector<string> PasswordData;
		PasswordData.push_back("NoPasswordsDetected");
		PasswordData.push_back("No_Email");
		PasswordData.push_back("No_Password");
		MkDirect.clear(); // empty that bish
		MkDirect = AppDirectory;
		MkDirect.append(PasswordData[PasswordName] + ".txt"); // final directory
		
		// write the data finally the password file seperetly to prevent crash
		WriteDataToFile(MkDirect.c_str(), PasswordData);
		// write the data to index
		PasswordData.clear();
		PasswordData.push_back("NoPasswordsDetected");
		WriteDataToFile(IndexDirectory, PasswordData);
	}

	// (Indexing for the second parameter starts at 1)
	// get the data from inside of a .txt file, the system is similar to a CSV file, ',,,' indicates the start of the file, and each comma after that seperates a piece of data from another
	// if 'IndexOfDataInFile' is parsed as -1, it will return all the data from the file, otherwise it will return a vector with a size of 1 with 1 piece of data
	vector<string> GetDataFromFile(const char* FileDirectory, size_t IndexOfDataInFile) {
		string RawFileData = ""; // this is where the entire content of the .txt target will be dumbed into and then sufted through
		vector<string> FileData; // this is where all the actual data in the file will be stored, nothing more, nothing less
		vector<size_t> CommaPositions;
		ifstream ifs;
		size_t Found = 0;

		ifs.open(FileDirectory);
		while (ifs >> RawFileData) {} // gathering the entire content of index.txt into 'RawFileData'
		ifs.close();

		// finding comma positions, a comma is the terminating character so the stored passwords can be split up properly
		// Comma positions are stored in 'CommaPositions'
		for (; Found != string::npos;) {
			Found = RawFileData.find(",", Found + 1);
			if (Found != string::npos) {
				CommaPositions.push_back(Found);
			}
		}

		// Splits up all of the names in index.txt into seperate positions in PasswordList. note that index 0 is the entire content of the file index.txt, so actual password names start at index 1
		for (int i = 0; i < CommaPositions.size() - 1; i++) {
			FileData.push_back(RawFileData.substr(CommaPositions[i] + 1, (CommaPositions[i + 1] - CommaPositions[i]) - 1));
		}

		// return requested data
		vector<string> RequestedData;
		switch (IndexOfDataInFile) {
		case -1:
			for (int i = 1; i < FileData.size(); i++)
				RequestedData.push_back(FileData[i]);
			return RequestedData;
			break;
		default:
			RequestedData.push_back(FileData[IndexOfDataInFile]);
			return RequestedData;
			break;
		}
	}

	// write data to a file in the same file format as GetDataFromFile
	// this function will nuke all data already in the file, so for data preservation sake, even if just editing one secion of data,
	// get the data out before writing and pass all of it through with the changes, this isn't needed if the file has no data or doesnt exist yet
	// If the file does not already exist, it will add it to the index automatically. Note WILL ONLY WORK IF THE THIRD PARAMETER IS SET CORRECTLY
	bool WriteDataToFile(const char* FileDirectory, vector<string> Data, string FileName = "null") {
		// is the file included in the index folder? DO NOT CHECK INDEX
		if (FileDirectory != IndexDirectory && FileName != "null") { // do not check index.txt otherwise there will be an infinate loop becuase function calls itself to write to index.txt. also dont do anything with an index if the filename is null
			vector<string> IndexData = GetDataFromFile(IndexDirectory, -1);
			bool FoundInIndex = false;
			for (int i = 0; i < IndexData.size(); i++) {
				if (IndexData[i] == FileName) {
					FoundInIndex = true;
					break;
				}
			}
			if (!FoundInIndex) {
				IndexData.push_back(FileName);
				WriteDataToFile(IndexDirectory, IndexData);
			}
		}
		ofstream ofs;
		ofs.open(FileDirectory, ofstream::out | ofstream::trunc);
		ofs << ",,,";
		for (int i = 0; i < Data.size(); i++) {
			ofs << Data[i] << ",";
		}
		ofs.close();
		return true;
	}

	bool RemoveFile(const char* FileDirectory, bool RemoveFromIndex = false, string IndexAndFileName = "null") {
		if (RemoveFromIndex && IndexAndFileName == "null") // error check
			return false;

		// removing the file from the index first, if the 'RemoveFromIndex' was true
		if (RemoveFromIndex) {
			vector<string> NewIndexData = GetDataFromFile(IndexDirectory, -1); // get all the data from the index
			for (int i = 0; i < NewIndexData.size(); i++) {
				if (IndexAndFileName == NewIndexData[i]) {
					NewIndexData.erase(std::next(NewIndexData.begin(), i));
					WriteDataToFile(IndexDirectory, NewIndexData);
				}
			}
		}

		// delete that bitch
		DeleteFileA(FileDirectory);
		return true;
	}

	// get all passwords stored inside of the directory files. Of course this is only to be used when the data is stored in the right format, E.G. if this wasn't coded by you, this block can be removed
	vector<PasswordStruct> GetPasswords() {
		vector <PasswordStruct> Passwords{}; // this is what will be returned
		// --------- Get Passwords in index.txt
		vector<string> PasswordNames = GetDataFromFile(IndexDirectory, -1);

		// --------- Get the data of each password using a for statement
		PasswordStruct BufferPasswordStruct{};
		string PasswordDirectory{};
		for (int i = 0; i < PasswordNames.size(); i++) {
			PasswordDirectory = (AppDirectory + PasswordNames[i] + ".txt");
			BufferPasswordStruct.FilePath = PasswordDirectory;
			BufferPasswordStruct.PasswordName = GetDataFromFile(PasswordDirectory.c_str(), 1)[0];
			BufferPasswordStruct.EmailContent = GetDataFromFile(PasswordDirectory.c_str(), 2)[0];
			BufferPasswordStruct.PasswordContent = GetDataFromFile(PasswordDirectory.c_str(), 3)[0];

			Passwords.push_back(BufferPasswordStruct);
		}

		// return data
		return Passwords;
	}

	// project specific, taking all of the stored data and sending it to a single text file
	void GeneratePasswordSummary() {
		string OutDirectory;
		for (int i = 0; i < AppDirectoryVector.size() - 1; i++) {
			OutDirectory += AppDirectoryVector[i];
		}
		OutDirectory += "DataSummary.txt";

		//get password data
		vector<PasswordStruct> Passwords = GetPasswords();
		
		// if file already exists, delete
		ifstream ifs;
		ifs.open(OutDirectory);
		if (ifs.is_open()) {
			//delete file
			DeleteFileA(OutDirectory.c_str());
		}
		ifs.close();

		// create file and write to it
		ofstream ofs;
		ofs.open(OutDirectory.c_str(), ofstream::out | ofstream::trunc);
		for (int i = 0; i < Passwords.size(); i++) {
			ofs << "Password[" << i << "] - '" << Passwords[i].PasswordName << "'\n"; // write which password and endl
			ofs << "   Email: '" << Passwords[i].EmailContent << "'\n"; // write email and endl
			ofs << "   Password: '" << Passwords[i].PasswordContent << "'"; // write password
			if (i < Passwords.size()-1) {
				ofs << "\n\n\n"; // if it's not the last, endl 4x to make room for next password
			}
		}

		//done writting
		ofs.close();

		//open in file explorer using system call (stinky i know)
		system(OutDirectory.c_str());
	}
};