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
	OUT_File.open("./output/output.txt", std::fstream::out);
	BASE_Value = "0";
	PC_Value = "0";
	Reg = { Registers(
	"A", 0),Registers(
	"X", 1),Registers(
	"L", 2),Registers(
	"B", 3),Registers(
	"S", 4),Registers(
	"T", 5),Registers(
	"F", 6) };

	line_by_line();
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
			EXTREF.clear();
			OP_Codes temp;

			temp.Location = "";
			string word;
			word = Line_Reader[1];
			if (Line_Reader.size() > 2) {
				for (int i = 2; i < Line_Reader.size(); i++)
				{
					word = word + " " + RemoveAllSpecicalChar(Line_Reader[i]);

					if (Line_Reader[1] == "EXTREF") {
						OP_Codes temp1;
						temp1.OBJ_Code = RemoveAllSpecicalChar(Line_Reader[i]);
						temp1.Location = "00000";
						EXTREF.push_back(temp1);
					}
				}
			}
			temp.OBJ_Code = word;
			OBJ_File.push_back(temp);


		}


		else if (check == 2) {
			OP_Codes temp;
			temp.OBJ_Code = "END";
			temp.Location = "000000";
			OBJ_File.push_back(temp);
			OBJECT_PROGRAM();
		}
		else if (check == 3) {
			OP_Codes temp;
			temp.Location = LOCATIONS[line];
			temp.OBJ_Code = "START";
			temp.OBJ_Code += " " + Line_Reader[1];
			OBJ_File.push_back(temp);

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
				else if (Line_Reader[2] == "BYTE")
					temp.OBJ_Code = ValueOfByte();
			}
			OBJ_File.push_back(temp);

		}
		else if (check == 5) {



			OP_Codes temp;
			temp.Location = LOCATIONS[line];
			temp.OBJ_Code = "CSECT";
			temp.OBJ_Code += " " + Line_Reader[1];
			OBJ_File.push_back(temp);

		}



		else if (check == 6) {
			OP_Codes temp;
			temp.Location = LOCATIONS[line];
			temp.OBJ_Code = "";
			string word1 = Line_Reader[2];


			//tackling format 1 statements
			if (Line_Reader.size() == 3) {
				if (Line_Reader[2] == "RSUB") {
					temp.OBJ_Code = "4F0000";
				}
				else if (Line_Reader[2] == "+RSUB") {
					temp.OBJ_Code = "4F000000";
				}
				else
					temp.OBJ_Code = ValueOfOPCode(word1);
			}
			else {
				string word2 = Line_Reader[3];


				if (PlusInWord(word1)) {
					temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2);
					if (temp.OBJ_Code == "") {
						cout << "PROBLEM IN GENERATION OF 1 OBJECT CODE \n\n\n\n\n";
					}
					if (CheckIfinEXTREF(word2)) {
						temp.OBJ_Code += "00000";
					}
					else
					{
						temp.OBJ_Code += Address_Value(word2);
					}
				}
				else {
					temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2);
					if (temp.OBJ_Code == "") {
						cout << "PROBLEM IN GENERATION OF 2 OBJECT CODE \n\n\n\n\n";
					}
					if (CheckIfinEXTREF(word2)) {
						temp.OBJ_Code += "00000";
					}
					else
					{
						temp.OBJ_Code += Displacement_Value(word2);
					}
				}
			}

			OBJ_File.push_back(temp);
		}


		else if (check == 7) {
			OP_Codes temp;
			temp.OBJ_Code = "";
			temp.Location = LOCATIONS[line];
			string word1 = Line_Reader[1];

			//tackling format 1 statements
			if (Line_Reader.size() == 2) {
				if (Line_Reader[1] == "RSUB") {
					temp.OBJ_Code = "4F0000";
				}
				else if (Line_Reader[1] == "+RSUB") {
					temp.OBJ_Code = "4F000000";
				}
				else
					temp.OBJ_Code = ValueOfOPCode(word1);
			}

			//tackling other format statements
			else {
				string word2 = Line_Reader[2];


				if (PlusInWord(word1)) {
					temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2);
					if (temp.OBJ_Code == "") {
						cout << "PROBLEM IN GENERATION OF 1 OBJECT CODE \n\n\n\n\n";
					}
					if (CheckIfinEXTREF(word2)) {
						temp.OBJ_Code += "00000";
					}
					else
					{
						temp.OBJ_Code += Address_Value(word2);
					}
				}
				else {
					temp.OBJ_Code = OP_Code_Value(word1, word2) + Flags_Value(word1, word2);
					if (temp.OBJ_Code == "") {
						cout << "PROBLEM IN GENERATION OF 2 OBJECT CODE \n\n\n\n\n";
					}
					if (CheckIfinEXTREF(word2)) {
						temp.OBJ_Code += "00000";
					}
					else
					{
						temp.OBJ_Code += Displacement_Value(word2);
					}
				}
			}
			OBJ_File.push_back(temp);
		}


		else if (check == 8)
		{
			BASE_Value = decToHexaString(hexadecimalToDecimal(SearchInSymTab_ReturnAddress(Line_Reader[2])));
		}
		else if (check == 10) {
			OP_Codes temp;
			temp.Location = LOCATIONS[line];
			temp.OBJ_Code = OP_Code_Value(Line_Reader[2]);
			for (int i = 3; i < Line_Reader.size(); i++) {
				temp.OBJ_Code += RegisterValue(RemoveAllSpecicalChar(Line_Reader[i]));
			}
			if (temp.OBJ_Code.size() == 3)
			{
				temp.OBJ_Code += "0";
			}
			if (temp.OBJ_Code.size() == 2)
			{
				temp.OBJ_Code += "00";
			}
			OBJ_File.push_back(temp);
		}
		else if (check == 11) {
			OP_Codes temp;
			temp.Location = LOCATIONS[line];
			temp.OBJ_Code = OP_Code_Value(Line_Reader[1]);
			for (int i = 2; i < Line_Reader.size(); i++) {
				temp.OBJ_Code += RegisterValue(RemoveAllSpecicalChar(Line_Reader[i]));
			}
			if (temp.OBJ_Code.size() == 3)
			{
				temp.OBJ_Code += "0";
			}
			if (temp.OBJ_Code.size() == 2)
			{
				temp.OBJ_Code += "00";
			}
			OBJ_File.push_back(temp);
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

	if (CheckIfOPCode(RemoveAllSpecicalChar(Word1)))
	{
		if (SizeOfOPCode(Word1) == "2")
		{
			return 11;
		}
		else
		{
			return 7;
		}

	}
	std::string Word2 = Line_Reader[2];

	if (Word2 == "START")
		return 3;

	if (Word2 == "WORD" || Word2 == "BYTE" || Word2 == "RESW" || Word2 == "RESB")
		return 4;

	if (Word2 == "CSECT")
		return 5;
	if (CheckIfOPCode(RemoveAllSpecicalChar(Word2))) {
		if (SizeOfOPCode(Word2) == "2")
		{
			return 10;
		}
		else
		{
			return 6;
		}
	}

	return -1;


}

//WORD AND BYTE READER IS DONE
string Pass2Reader::ValueOfWord()
{
	string Word = Line_Reader[3];
	int converter = DecStringtoDec(Word);
	Word = decToHexa(converter);
	return Word;
}
string Pass2Reader::ValueOfByte()
{
	string Word = Line_Reader[3];
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
	string Variable;
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
	if (return_word.size() == 1) {
		return_word = "0" + return_word;
	}
	return return_word;

}
string Pass2Reader::OP_Code_Value(string Word) {
	return ValueOfOPCode(Word);
}

string Pass2Reader::Flags_Value(string OPERATION, string VARIABLE) {
	string return_word;
	string Variable = "";

	if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 1] == ',') {
		Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
	}
	else {
		Variable = VARIABLE;
	}
	int converter = 0;
	if (OPERATION[0] != '+' && SearchInSymTab_ifPresent(RemoveAllSpecicalChar(Variable))) {
		int location_of_var = hexadecimalToDecimal(SearchInSymTab_ReturnAddress(Variable));
		int value_of_PC = hexadecimalToDecimal(PC_Value);
		int value_of_BASE = hexadecimalToDecimal(BASE_Value);
		int value;
		value = location_of_var - value_of_PC;
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

	return_word = decToHexaString(converter);
	if (return_word.size() == 0) {
		return_word = "0";
	}
	return return_word;
}

string Pass2Reader::Displacement_Value(string VARIABLE) {
	string word = "";
	string Variable = "";

	if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 1] == ',') {
		Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
	}
	else {
		Variable = VARIABLE;
	}
	if (SearchInSymTab_ifPresent(RemoveAllSpecicalChar(Variable)))
	{
		int location_of_var = hexadecimalToDecimal(SearchInSymTab_ReturnAddress(RemoveAllSpecicalChar(Variable)));
		int value_of_PC = hexadecimalToDecimal(PC_Value);
		int value_of_BASE = hexadecimalToDecimal(BASE_Value);
		int value;
		value = location_of_var - value_of_PC;
		if (value <= 2047 && value >= (-2048)) {
			cout << "DOING PC RELATIVE \n\n\n\n\n\n";
		}
		else {
			cout << "DOING BASE RELATIVE \n\n\n\n\n\n";
			value = location_of_var - value_of_BASE;
		}
		word = decToHexaString(value);

	}
	else if (VARIABLE[0] = '#') {
		word = RemoveAllSpecicalChar(Variable);
	}

	if (word.size() == 0) {
		word = "000" + word;
	}
	else if (word.size() == 1) {
		word = "00" + word;
	}
	else if (word.size() == 2) {
		word = "0" + word;
	}
	return word;

}

string Pass2Reader::Address_Value(string VARIABLE) {
	string word = "";
	string Variable = "";

	if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 2] == ',') {
		Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
	}
	else {
		Variable = VARIABLE;
	}

	if (SearchInSymTab_ifPresent(RemoveAllSpecicalChar(Variable))) {
		word = decToHexaString(hexadecimalToDecimal(SearchInSymTab_ReturnAddress(RemoveAllSpecicalChar(Variable))));
	}
	else if (VARIABLE[0] == '#') {
		word = decToHexa(DecStringtoDec(RemoveAllSpecicalChar(Variable)));
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
	return "";
}


std::string Pass2Reader::SizeOfOPCode(string OP_Code) {
	for (auto i = OP_File.begin(); i < OP_File.end(); i++) {
		std::vector<std::string> OP_line;
		for (auto i = OP_File.begin(); i < OP_File.end(); i++) {
			ReadWordsFromLine((*i), OP_line);
			if (OP_Code == OP_line[0]) {
				return OP_line[1];
			}
		}
	}
	return "";
}
string Pass2Reader::RegisterValue(string word) {
	for (auto i = Reg.begin(); i < Reg.end(); i++) {
		if ((*i).Reg_Name == word) {
			if ((*i).Reg_No = 0)
				return "0";
			return decToHexaString((*i).Reg_No);
		}
	}
	return "";
}

bool Pass2Reader::CheckIfinEXTREF(string VARIABLE) {
	string Variable = "";
	if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 2] == ',') {
		Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
	}
	else {
		Variable = VARIABLE;
	}
	for (auto i = EXTREF.begin(); i < EXTREF.end(); i++) {
		if ((*i).OBJ_Code == Variable) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// Object program writer code
/// 
/// </summary>
/// <returns> writes the output file that has all the OBJECT PGM</returns>

void Pass2Reader::OBJECT_PROGRAM() {
	vector<string> EXT_REF;
	int counter;
	for (int line = 0; line < OBJ_File.size(); line++) {
		vector<string> value_of_objcode;
		ReadWordsFromLine(OBJ_File[line].OBJ_Code, value_of_objcode);
		int size_OP = value_of_objcode.size();
		if (size_OP == 0)
		{
			OUT_File << "";
		}
		else if (size_OP == 1) {
			if (value_of_objcode[0] == "EXTDEF")
				OUT_File << "";
			else if (value_of_objcode[0] == "EXTREF")
				OUT_File << "";
			else if (value_of_objcode[0] == "END") {
				//Modification Record

				for (int line_new = counter; line_new < line; line_new++) {
					ReadWordsFromLine(Inter_File[line_new], Line_Reader);
					int check = TypeChecker();
					if (check == 7 && Line_Reader.size()>2) {
						for (auto i = EXT_REF.begin(); i < EXT_REF.end(); i++) {
							string VARIABLE = Line_Reader[2];
							string Variable;
							if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 2] == ',') {
								Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
							}
							else {
								Variable = VARIABLE;
							}
							if ((*i) == RemoveAllSpecicalChar(Variable)) {
								string add = decToHexaString(hexadecimalToDecimal(LOCATIONS[line_new]) + 1);
								if (add.size() == 0) {
									add = "000000" + add;
								}
								else if (add.size() == 1) {
									add = "00000" + add;
								}
								else if (add.size() == 2) {
									add = "0000" + add;
								}
								else if (add.size() == 3) {
									add = "000" + add;
								}
								else if (add.size() == 4) {
									add = "00" + add;
								}
								else if (add.size() == 5) {
									add = "0" + add;
								}
								OUT_File << endl<<"M" << add << "05" << "+" << RemoveAllSpecicalChar(Variable);
							}
						}
					}

					if (check == 6 && Line_Reader.size()>3) {
						for (auto i = EXT_REF.begin(); i < EXT_REF.end(); i++) {
							string VARIABLE = Line_Reader[3];
							string Variable;
							if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 2] == ',') {
								Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
							}
							else {
								Variable = VARIABLE;
							}
							if ((*i) == RemoveAllSpecicalChar(Variable)) {
								string add = decToHexaString(hexadecimalToDecimal(LOCATIONS[line_new]) + 1);
								if (add.size() == 0) {
									add = "000000" + add;
								}
								else if (add.size() == 1) {
									add = "00000" + add;
								}
								else if (add.size() == 2) {
									add = "0000" + add;
								}
								else if (add.size() == 3) {
									add = "000" + add;
								}
								else if (add.size() == 4) {
									add = "00" + add;
								}
								else if (add.size() == 5) {
									add = "0" + add;
								}
								OUT_File<<endl << "M" << add << "05" << "+" << RemoveAllSpecicalChar(Variable);
							}
						}
					}
				}
				EXT_REF.clear();
				
				OUT_File<<endl <<"E";
				vector<string> End_line;
				ReadWordsFromLine(OBJ_File[counter].OBJ_Code, value_of_objcode);
				if (value_of_objcode[0] == "START")
					OUT_File << "000000";



			}
			else
				//Print TEXT RECORD
			{


				OUT_File << endl << "T" << OBJ_File[line].Location;
				string word = "";
				for (int i = 0; i + line < OBJ_File.size(); i++) {
					if (OBJ_File[line + i].OBJ_Code == "") {
						line += i;
						break;
					}

					word += OBJ_File[line + i].OBJ_Code;
					if (line + i + 1 >= OBJ_File.size()) {
						line += i;
						break;
					}
					else if (OBJ_File[line + i + 1].OBJ_Code == "END") {
						line += i;
						break;
					}
					else {
						vector<string> temp;
						ReadWordsFromLine(OBJ_File[line + i + 1].OBJ_Code, temp);
						if (temp.size() >= 2) {
							line += i;
							break;
						}
						else {
							if (word.size() + OBJ_File[line + i + 1].OBJ_Code.size() > 60) {
								line += i;
								break;
							}
						}
					}

				}


				string size = decToHexaString(word.size() / 2);
				if (size.size() == 1)
				{
					size = "0" + size;
				}
				OUT_File << size << word;
			}

		}
		else if (size_OP == 2)
		{
			if (value_of_objcode[0] == "START") {
				OUT_File << "H";
				counter = line;
				string name;
				for (int i = 0; i < value_of_objcode[1].size(); i++)
				{
					if (i == 7) {
						break;
					}
					name += value_of_objcode[1][i];
				}
				OUT_File << name << OBJ_File[line].Location << SizeOfCSECT(line);
			}
			else if (value_of_objcode[0] == "CSECT") {
				//Modification Record

				for (int line_new = counter; line_new < line; line_new++) {
					ReadWordsFromLine(Inter_File[line_new], Line_Reader);
					int check = TypeChecker();
					if (check == 7 && Line_Reader.size()>2) {
						for (auto i = EXT_REF.begin(); i < EXT_REF.end(); i++)
						{
							string VARIABLE = Line_Reader[2];
							string Variable;
							if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 2] == ',') {
								Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
							}
							else {
								Variable = VARIABLE;
							}
							if ((*i) == RemoveAllSpecicalChar(Variable)) {
								string add = decToHexaString(hexadecimalToDecimal(LOCATIONS[line_new]) + 1);
								if (add.size() == 0) {
									add = "000000" + add;
								}
								else if (add.size() == 1) {
									add = "00000" + add;
								}
								else if (add.size() == 2) {
									add = "0000" + add;
								}
								else if (add.size() == 3) {
									add = "000" + add;
								}
								else if (add.size() == 4) {
									add = "00" + add;
								}
								else if (add.size() == 5) {
									add = "0" + add;
								}
								OUT_File<<endl << "M" << add << "05" << "+" << RemoveAllSpecicalChar(Variable);
							}
						}
					}

					if (check == 6 && Line_Reader.size()>3) {
						for (auto i = EXT_REF.begin(); i < EXT_REF.end(); i++) {
							string VARIABLE = Line_Reader[3];
							string Variable;
							if (VARIABLE.back() == 'X' && VARIABLE[VARIABLE.size() - 2] == ',') {
								Variable = VARIABLE.substr(0, VARIABLE.size() - 2);
							}
							else {
								Variable = VARIABLE;
							}
							if ((*i) == RemoveAllSpecicalChar(Variable)) {
								string add = decToHexaString(hexadecimalToDecimal(LOCATIONS[line_new]) + 1);
								if (add.size() == 0) {
									add = "000000" + add;
								}
								else if (add.size() == 1) {
									add = "00000" + add;
								}
								else if (add.size() == 2) {
									add = "0000" + add;
								}
								else if (add.size() == 3) {
									add = "000" + add;
								}
								else if (add.size() == 4) {
									add = "00" + add;
								}
								else if (add.size() == 5) {
									add = "0" + add;
								}
								OUT_File <<endl <<"M" << add << "05" << "+" << RemoveAllSpecicalChar(Variable) ;
							}
						}
					}
				}
				EXT_REF.clear();







				
				OUT_File << endl << "E";
				vector<string> End_line;
				ReadWordsFromLine(OBJ_File[counter].OBJ_Code, value_of_objcode);
				if (value_of_objcode[0] == "START")
					OUT_File << "000000";
				
				counter = line;
				OUT_File << endl << "H";
				string name;
				for (int i = 0; i < value_of_objcode[1].size(); i++)
				{
					if (i == 7) {
						break;
					}
					name += value_of_objcode[1][i];
				}
				OUT_File << name << OBJ_File[line].Location << SizeOfCSECT(line);
			}


			else if (value_of_objcode[0] == "EXTDEF") {
				OUT_File << endl << "D";
				string word;
				for (int i = 1; i < value_of_objcode.size(); i++) {
					word += RemoveAllSpecicalChar(value_of_objcode[i]) + SearchInSymTab_ReturnAddress(RemoveAllSpecicalChar(value_of_objcode[i]));
				}
				OUT_File << word;
			}




			else if (value_of_objcode[0] == "EXTREF")
			{
				OUT_File << endl << "R";
				string word;

				for (int i = 1; i < value_of_objcode.size(); i++) {
					word += RemoveAllSpecicalChar(value_of_objcode[i]);
					EXT_REF.push_back(RemoveAllSpecicalChar(value_of_objcode[i]));
				}
				OUT_File << word;
			}
		}
		else if (size_OP >= 3) {
			if (value_of_objcode[0] == "EXTDEF") {
				OUT_File << endl << "D";
				string word;
				for (int i = 1; i < value_of_objcode.size(); i++) {
					string name;
					for (int j = 0; j < value_of_objcode[i].size(); j++)
					{
						if (i == 7) {
							break;
						}
						name += value_of_objcode[i][j];
					}
					word += name + SearchInSymTab_ReturnAddress(RemoveAllSpecicalChar(value_of_objcode[i]));
				}
				OUT_File << word;
			}
			else if (value_of_objcode[0] == "EXTREF")
			{
				OUT_File << endl << "R";
				string word;
				for (int i = 1; i < value_of_objcode.size(); i++) {
					string name;
					for (int j = 0; j < value_of_objcode[i].size(); j++)
					{
						if (i == 7) {
							break;
						}
						name += value_of_objcode[i][j];
					}
					word += name;
					EXT_REF.push_back(RemoveAllSpecicalChar(value_of_objcode[i]));
				}
				OUT_File << word;
			}
		}
	}
}
string Pass2Reader::SizeOfCSECT(int line_no) {
	string size_of_csect;
	for (int line1 = line_no + 1; line1 < Inter_File.size(); line1++) {
		ReadWordsFromLine(Inter_File[line1], Line_Reader);
		vector <string> Previous_Line;
		// 5 - CSECT
		int check = TypeChecker();


		if (check == 5 || check == 2) {

			line1--;
			ReadWordsFromLine(Inter_File[line1], Previous_Line);
			line1++;
			size_of_csect = Previous_Line[0];
			std::string type = Previous_Line[2];
			std::string size = Previous_Line[3];
			if (type == "WORD")
				size_of_csect = decToHexaString((hexadecimalToDecimal(size_of_csect) + hexadecimalToDecimal("3")));


			else if (type == "BYTE") {
				if (size[0] == 'C') {
					size_of_csect = decToHexaString((hexadecimalToDecimal(size_of_csect) + hexadecimalToDecimal(decToHexaString(size.size() - 3))));
				}
				if (size[0] == 'X') {
					size_of_csect = decToHexaString((hexadecimalToDecimal(size_of_csect) + hexadecimalToDecimal("1")));;
				}
			}

			else if (type == "RESW")
				size_of_csect = decToHexaString((hexadecimalToDecimal(size_of_csect) + hexadecimalToDecimal(decToHexaString(3 * DecStringtoDec(size)))));


			else if (type == "RESB")
				size_of_csect = decToHexaString((hexadecimalToDecimal(size_of_csect) + hexadecimalToDecimal(decToHexaString(DecStringtoDec(size)))));


			if (size_of_csect.size() == 0)
				size_of_csect = "000000" + size_of_csect;

			else if (size_of_csect.size() == 1)
				size_of_csect = "00000" + size_of_csect;

			else if (size_of_csect.size() == 2)
				size_of_csect = "0000" + size_of_csect;

			else if (size_of_csect.size() == 3)
				size_of_csect = "000" + size_of_csect;

			else if (size_of_csect.size() == 4)
				size_of_csect = "00" + size_of_csect;

			else if (size_of_csect.size() == 5)
				size_of_csect = "0" + size_of_csect;
			return size_of_csect;

		}
	}
	return"";

}

bool Pass2Reader::Confirm_if_Start() {
	//used while writing records
	return false;
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

