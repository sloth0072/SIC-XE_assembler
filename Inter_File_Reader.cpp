#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>
#include"Resources\Header.h"
using namespace std;



Pass2Reader::Pass2Reader() {
	CreateFileasStringVector("./Resources/Intermediate.txt", Inter_File);
	CreateFileasStringVector("./Resources/Data/SYMTAB.txt", SYM_File);
	CreateFileasStringVector("./Resources/Data/OPTAB.txt", OP_File);
	BASE_Value = "0";
	PC_Value = "0";
}

void Pass2Reader::line_by_line() {
	for (int line = 0; line < Inter_File.size(); line++) {
		ReadWordsFromLine(Inter_File[line], Line_Reader);
		LOCATIONS.push_back(Line_Reader[0]);
		if (line < Inter_File.size() - 1) {
			line += 1;
			ReadWordsFromLine(Inter_File[line], Next_Line);
			line -= 1;
		}
		PC_Value = Next_Line[0];

		// 0 - RSUB
		// 1 - EXTREF EXTDEF
		// 2 - END
		// 3 - START
		// 4 - WORD BYTE RESW RESB
		// 5 - CSECT
		// 6 - LOOP	OP_Codes
		// 7 - NORMAL STATEMENT
		// 8 - BASE


		int check = TypeChecker();


		if (check == 1) {

			/*
			*			//EXTREF AND EXTDEF HAS SEPARATE TABLES
						LOCCTR_LINES.push_back(LOCCTR);
						std::ofstream EXTREF_File, EXTDEF_File;

						if (Column_Reader[0] == "EXTDEF")
						{
							for (int symbols = 1; symbols < Column_Reader.size(); symbols++) {
								OP_Codes temp;
								temp.SYM_NAME = RemoveAllSpecicalChar(Column_Reader[symbols]);
								temp.SYM_ADDRESS = "0";
								EXTDEF.push_back(temp);
							}
							OP_Codes temp = SYM_TABLE.back();
							EXTDEF_File.open(Directory + temp.SYM_NAME + "_EXTDEF.txt");
							for (auto i = EXTDEF.begin(); i < EXTDEF.end(); i++) {
								EXTDEF_File << (*i).SYM_NAME << " " << (*i).SYM_ADDRESS << std::endl;
							}
							EXTDEF_File.close();

						}
						else if (Column_Reader[0] == "EXTREF") {
							for (int symbols = 1; symbols < Column_Reader.size(); symbols++) {
								OP_Codes temp;
								temp.SYM_NAME = RemoveAllSpecicalChar(Column_Reader[symbols]);
								temp.SYM_ADDRESS = "0";
								EXTREF.push_back(temp);
							}
							OP_Codes temp = SYM_TABLE.back();
							EXTREF_File.open(Directory + temp.SYM_NAME + "_EXTREF.txt");
							for (auto i = EXTREF.begin(); i < EXTREF.end(); i++) {
								EXTREF_File << (*i).SYM_NAME << " " << (*i).SYM_ADDRESS << std::endl;
							}
							EXTREF.clear();
							EXTREF_File.close();
							}
			*/


		}


		else if (check == 2) {
			LOCCTR_LINES.push_back(NULL);
			std::cout << "PROGRAM PROCESSING IS COMPLETE" << std::endl << std::endl;
			for (auto i = SYM_TABLE.begin(); i < SYM_TABLE.end(); i++) {
				if ((i) == SYM_TABLE.end() - 1) {
					SYM_File << (*i).SYM_NAME << " " << (*i).SYM_ADDRESS;
				}
				else {
					SYM_File << (*i).SYM_NAME << " " << (*i).SYM_ADDRESS << std::endl;
				}
			}
			Intermediate_File_Creator();

		}
		else if (check == 3) {
			OP_Codes temp;
			temp.SYM_NAME = Column_Reader[0];
			temp.SYM_ADDRESS = Column_Reader[2];
			SYM_TABLE.push_back(temp);
			LOCCTR = hexadecimalToDecimal(temp.SYM_ADDRESS);
			LOCCTR_LINES.push_back(LOCCTR);
		}
		else if (check == 4) {
			OP_Codes temp;
			temp.Location = LOCATIONS[line];

			if (Line_Reader[2] == "RESW" || Line_Reader[2] == "RESB") {

				temp.OBJ_Code = "";

			}
			else {
				if (Line_Reader[2] == "WORD")
					temp.OBJ_Code = ValueOfWord();
				else
					temp.OBJ_Code = ValueOfByte();
			}
			OBJ_File.push_back(temp);

		}
		else if (check == 5) {
			//Updated to have EXTREF AND EXTDEF
			// We have to clear symbol table also ;
			// we need to also have symbol to be used again in the new CSECT;
			// write a function to store the data in new symbol file;
			OP_Codes temp;
			temp.SYM_NAME = Column_Reader[0];
			temp.SYM_ADDRESS = "0000";
			SYM_TABLE.push_back(temp);


			LOCCTR = 0;
			LOCCTR_LINES.push_back(LOCCTR);

		}



		else if (check == 6) {
			OP_Codes temp;
			temp.Location = LOCATIONS[line];
			temp.OBJ_Code = "";
			string word1 = Line_Reader[2];
			string word2 = Line_Reader[3];
			if (PlusInWord(word1)) {
				temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2) + Address_Value(word2);
				if (temp.OBJ_Code == "") {
					cout << "PROBLEM IN GENERATION OF 1 OBJECT CODE \n\n\n\n\n";
				}
			}
			else {
				temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2) + Displacement_Value(word2);
				if (temp.OBJ_Code == "") {
					cout << "PROBLEM IN GENERATION OF 2 OBJECT CODE \n\n\n\n\n";
				}
			}
			OBJ_File.push_back(temp);
		}


		else if (check == 7) {
			OP_Codes temp;
			temp.OBJ_Code = "";
			temp.Location = LOCATIONS[line];
			string word1 = Line_Reader[1];
			string word2 = Line_Reader[2];
			if (PlusInWord(word1)) {
				temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2) + Address_Value(word2);
				if (temp.OBJ_Code == "") {
					cout << "PROBLEM IN GENERATION OF 3 OBJECT CODE \n\n\n\n\n";
				}
			}
			else {
				temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2) + Displacement_Value(word2);
				if (temp.OBJ_Code == "")
					cout << "PROBLEM IN GENERATION OF 4 OBJECT CODE \n\n\n\n\n";
			}
			OBJ_File.push_back(temp);
		}


		else if (check == 8)
		{
			OP_Codes temp;
			temp.SYM_NAME = Column_Reader[0];
			temp.SYM_ADDRESS = "";
			LOCCTR_LINES.push_back(LOCCTR);
			temp.SYM_NAME = Column_Reader[1];
			temp.SYM_ADDRESS = "";

			std::cout << "BASE IS DECLARED AT: " << decToHexa(LOCCTR) << " ADDRESS STATEMENT\n\n\n\n\n\n";


		}
		else if (check == -1) {
			std::cout << "ERROR HAS OCCURED WHILE UPDATING LOCCTR PER LINE \n\n\n\n\n\n\n";
		}

		std::cout << "The line being processed is : " << line + 1 << "\n\n\n\n\n\n";

	}
}

int Pass2Reader::TypeChecker() {

	std::string Word1 = Line_Reader[1];

	if (Word1 == "BASE") {
		return 8;
	}
	if (Word1 == "EXTREF" || Word1 == "EXTDEF")
		return 1;
	if (Word1 == "END")
		return 2;
	std::string Word2 = Line_Reader[2];

	if (Word2 == "START")
		return 3;
	else
		if (Word2 == "WORD" || Word2 == "BYTE" || Word2 == "RESW" || Word2 == "RESB")
			return 4;
		else
			if (Word2 == "CSECT")
				return 5;
			else
				if (CheckIfOPCode(RemoveAllSpecicalChar(Word1)))
					return 7;
				else
				{
				}

	return -1;


}

//WORD AND BYTE READER IS DONE
string Pass2Reader::ValueOfWord()
{
	string Word = Line_Reader[2];
	int converter = DecStringtoDec(Word);
	Word = decToHexa(converter);
	return Word;
}
string Pass2Reader::ValueOfByte()
{
	string Word = Line_Reader[2];
	string return_word = "";
	if (Word[0] == 'C') {
		for (int i = 2; i < Word.size() - 1; i++) {
			int temp = Word[i];
			return_word += decToHexaString(temp);
		}
	}
	else if (Word[0] == 'X') {
		for (int i = 2; i < Word.size() - 1; i++) {
			return_word += Word[i];
		}
	}
	return return_word;
}

//OBJECT CODE FOR NORMAL STATEMENTS
string Pass2Reader::OP_Code_Value(string OPERATION, string VARIABLE) {
	string return_word = "";
	int converter = hexadecimalToDecimal(ValueOfOPCode(RemoveAllSpecicalChar(OPERATION)));
	if (VARIABLE[0] == '#') {
		converter += 1;
	}
	else if (VARIABLE[0] == '@') {
		converter += 2;
	}
	else {
		converter += 3;
	}
	return_word = decToHexaString(converter);
	return return_word;

}

string Pass2Reader::Flags_Value(string OPERATION, string VARIABLE) {
	string return_word;
	int converter = 0;
	if (OPERATION[0] != '+' && SearchInSymTab_ifPresent(RemoveAllSpecicalChar(VARIABLE))) {
		int value = hexadecimalToDecimal(Displacement_Value(VARIABLE));
		if (value <= 2047 && value >= (-2048)) {
			if (PC_Value == "")
				cout << "PC IS EMPTY DO PROPER CODING\n\n\n\n\n\n\n\n";
			else {
				converter += 2;
			}
		}
		else if (value <= 4097 && value >= 0) {
			if (BASE_Value == "")
				cout << "PC IS EMPTY DO PROPER CODING\n\n\n\n\n\n\n\n";
			else {
				converter += 4;
			}
		}
	}
	else if (OPERATION[0] == '+') {
		converter += 1;
	}
	if (VARIABLE.back() == 'X') {
		converter += 8;
	}
	return_word = decToHexa(converter);
	return return_word;
}

string Pass2Reader::Displacement_Value(string VARIABLE) {
	string word = "";
	if (SearchInSymTab_ifPresent(RemoveAllSpecicalChar(VARIABLE)))
	{
		int location_of_var = hexadecimalToDecimal(SearchInSymTab_ReturnAddress(VARIABLE));
		int value_of_PC = hexadecimalToDecimal(PC_Value);
		int value_of_BASE = hexadecimalToDecimal(BASE_Value);
		int value = location_of_var - value_of_PC;

		if (value <= 2047 && value >= (-2048)) {
			cout << "DOING PC RELATIVE \n\n\n\n\n\n";
		}
		else {
			cout << "DOING BASE RELATIVE \n\n\n\n\n\n";
			value = location_of_var - value_of_BASE;
		}
		word = decToHexa(value);

	}
	else if (VARIABLE[0] = '#') {
		word = RemoveAllSpecicalChar(VARIABLE);
	}

	if (word.size() == 0) {
		word = "0000" + word;
	}
	else if (word.size() == 1) {
		word = "000" + word;
	}
	else if (word.size() == 2) {
		word = "00" + word;
	}
	else if (word.size() == 3) {
		word = "0" + word;
	}
	return word;

}

string Pass2Reader::Address_Value(string VARIABLE) {
	string word = "";
	if (SearchInSymTab_ifPresent(RemoveAllSpecicalChar(VARIABLE))) {
		word = decToHexaString(hexadecimalToDecimal(SearchInSymTab_ReturnAddress(RemoveAllSpecicalChar(VARIABLE))));
	}
	else if (VARIABLE[0] == '#') {
		word = decToHexa(DecStringtoDec(RemoveAllSpecicalChar(VARIABLE)));
	}
	if (word.size() == 0) {
		word = "00000" + word;
	}
	else if (word.size() == 1) {
		word = "0000" + word;
	}
	else if (word.size() == 2) {
		word = "000" + word;
	}
	else if (word.size() == 3) {
		word = "00" + word;
	}
	else if (word.size() == 4) {
		word = "0" + word;
	}
	return word;

}



string Pass2Reader::SearchInSymTab_ReturnAddress(string VARIABLE) {
	vector<string> SYM_line;
	for (auto i = SYM_File.begin(); i < SYM_File.end(); i++) {
		ReadWordsFromLine((*i), SYM_line);
		if (VARIABLE == SYM_line[0]) {
			return SYM_line[1];
		}
	}
	return "ERROR";

}

bool Pass2Reader::SearchInSymTab_ifPresent(string VARIABLE) {
	vector<string> SYM_line;
	for (auto i = SYM_File.begin(); i < SYM_File.end(); i++) {
		ReadWordsFromLine((*i), SYM_line);
		if (VARIABLE == SYM_line[0]) {
			return true;
		}
	}
	return false;

}




string Pass2Reader::ValueOfOPCode(string OP_Code) {
	for (auto i = OP_File.begin(); i < OP_File.end(); i++) {
		std::vector<std::string> OP_line;
		for (auto i = OP_File.begin(); i < OP_File.end(); i++) {
			ReadWordsFromLine((*i), OP_line);
			if (OP_Code == OP_line[0]) {
				return OP_line[2];
			}
		}
	}
}

bool Pass2Reader::CheckIfOPCode(std::string word) {
	std::vector<std::string> OP_line;
	for (auto i = OP_File.begin(); i < OP_File.end(); i++) {
		ReadWordsFromLine((*i), OP_line);
		if (word == OP_line[0]) {
			return true;
		}
	}
	return false;
}

