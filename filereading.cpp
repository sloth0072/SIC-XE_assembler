#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>
#include"Resources\Header.h"
SYMBOL::SYMBOL() {
	SYM_ADDRESS = SYM_NAME = "";
}

//Class for pass 1


Pass1Reader::Pass1Reader() {


	file_reader = "./Input/input1.txt";
	OP_tab = "./Resources/Data/OPTAB.txt";
	Intermediate = "./Resources/Intermediate.txt";
	SYM_Tab = "./Resources/Data/SYMTAB.txt";
	Directory = "./Resources/Data/";


	CreateFileasStringVector(file_reader, Data_File);
	CreateFileasStringVector(OP_tab, OP_File);

	SYM_File.open(SYM_Tab, std::fstream::out);

	Inter_File.open(Intermediate, std::fstream::out);

	lineReader();
}

bool Pass1Reader::CheckIfOPCode(std::string word) {
	std::vector<std::string> OP_line;
	for (auto i = OP_File.begin(); i < OP_File.end(); i++) {
		ReadWordsFromLine((*i), OP_line);
		if (word == OP_line[0]) {
			return true;
		}
	}
	return false;
}

std::string Pass1Reader::SizeOfOPCode(std::string OP_Code) {
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
bool Pass1Reader::CheckIfSYMTABLE(std::string word) {
	for (auto i = SYM_TABLE.begin(); i < SYM_TABLE.end(); i++) {
		if ((*i).SYM_NAME == word) {
			return true;
		}
	}
	return false;
}
int Pass1Reader::VariableSize() {
	std::string type = Column_Reader[1];
	std::string size = Column_Reader[2];
	if (type == "WORD")
		return 3;
	else if (type == "BYTE") {
		if (size[0] == 'C') {
			return size.size() - 3;
		}
		if (size[0] == 'X') {
			return 1;
		}
	}
	else if (type == "RESW")
		return 3 * DecStringtoDec(size);
	else if (type == "RESB") {
		return DecStringtoDec(size);
	}
}
int Pass1Reader::DirectiveChecker() {
	std::string Word1 = Column_Reader[0];

	if (Word1 == "BASE") {
		return 8;
	}
	if (Word1 == "EXTREF" || Word1 == "EXTDEF")
		return 1;
	if (Word1 == "END")
		return 2;
	if (CheckIfOPCode(RemoveAllSpecicalChar(Word1)))
		return 7;
	std::string Word2 = Column_Reader[1];

	if (Word2 == "START")
		return 3;

	if (Word2 == "WORD" || Word2 == "BYTE" || Word2 == "RESW" || Word2 == "RESB")
		return 4;
	if (Word2 == "CSECT")
		return 5;
	if (CheckIfOPCode(RemoveAllSpecicalChar(Word2)))
		return 6;

	return -1;


}


void Pass1Reader::lineReader()
{
	for (int line = 0; line < Data_File.size(); line++)
	{
		std::string word = "";

		int size = Data_File[line].length();

		//extracting words from the line
		ReadWordsFromLine(Data_File[line], Column_Reader);


		// 0 - RSUB
		// 1 - EXTREF EXTDEF
		// 2 - END
		// 3 - START
		// 4 - WORD BYTE RESW RESB
		// 5 - CSECT
		// 6 - LOOP SYMBOL
		// 7 - NORMAL STATEMENT
		// 8 - BASE



		int check = DirectiveChecker();
		if (check == 0) {
			LOCCTR_LINES.push_back(LOCCTR);
			LOCCTR += 3;
		}
		else if (check == 1) {
			//EXTREF AND EXTDEF HAS SEPARATE TABLES
			LOCCTR_LINES.push_back(LOCCTR);
			std::ofstream EXTREF_File, EXTDEF_File;

			if (Column_Reader[0] == "EXTDEF")
			{
				for (int symbols = 1; symbols < Column_Reader.size(); symbols++) {
					SYMBOL temp;
					temp.SYM_NAME = RemoveAllSpecicalChar(Column_Reader[symbols]);
					temp.SYM_ADDRESS = "0";
					EXTDEF.push_back(temp);
				}
				SYMBOL temp = SYM_TABLE.back();
				EXTDEF_File.open(Directory + temp.SYM_NAME + "_EXTDEF.txt");
				for (auto i = EXTDEF.begin(); i < EXTDEF.end(); i++) {
					EXTDEF_File << (*i).SYM_NAME << " " << (*i).SYM_ADDRESS << std::endl;
				}
				EXTDEF_File.close();

			}
			else if (Column_Reader[0] == "EXTREF") {
				for (int symbols = 1; symbols < Column_Reader.size(); symbols++) {
					SYMBOL temp;
					temp.SYM_NAME = RemoveAllSpecicalChar(Column_Reader[symbols]);
					temp.SYM_ADDRESS = "0";
					EXTREF.push_back(temp);
				}
				SYMBOL temp = SYM_TABLE.back();
				EXTREF_File.open(Directory + temp.SYM_NAME + "_EXTREF.txt");
				for (auto i = EXTREF.begin(); i < EXTREF.end(); i++) {
					EXTREF_File << (*i).SYM_NAME << " " << (*i).SYM_ADDRESS << std::endl;
				}
				EXTREF.clear();
				EXTREF_File.close();
			}

		}
		else if (check == 2) {
			LOCCTR_LINES.push_back(NULL);
			std::cout << "PROGRAM PROCESSING IS COMPLETE" << std::endl << std::endl;
			for (auto i = SYM_TABLE.begin(); i < SYM_TABLE.end(); i++) {

				SYM_File << (*i).SYM_NAME << " " << (*i).SYM_ADDRESS << std::endl;

			}
			Intermediate_File_Creator();

		}
		else if (check == 3) {
			SYMBOL temp;
			temp.SYM_NAME = Column_Reader[0];
			temp.SYM_ADDRESS = Column_Reader[2];
			SYM_TABLE.push_back(temp);
			LOCCTR = hexadecimalToDecimal(temp.SYM_ADDRESS);
			LOCCTR_LINES.push_back(LOCCTR);
		}
		else if (check == 4) {
			SYMBOL temp;
			temp.SYM_NAME = Column_Reader[0];
			temp.SYM_ADDRESS = decToHexa(LOCCTR);
			LOCCTR_LINES.push_back(LOCCTR);
			LOCCTR += VariableSize();
			if (CheckIfSYMTABLE(temp.SYM_NAME))
			{
				for (auto i = SYM_TABLE.begin(); i < SYM_TABLE.end(); i++) {
					if ((*i).SYM_NAME == temp.SYM_NAME) {
						(*i).SYM_ADDRESS = temp.SYM_ADDRESS;
					}
				}
			}
			else {
				SYM_TABLE.push_back(temp);
			}

		}
		else if (check == 5) {
			//Updated to have EXTREF AND EXTDEF
			// We have to clear symbol table also ;
			// we need to also have symbol to be used again in the new CSECT;
			// write a function to store the data in new symbol file;
			SYMBOL temp;
			temp.SYM_NAME = Column_Reader[0];
			temp.SYM_ADDRESS = "0";
			SYM_TABLE.push_back(temp);


			LOCCTR = 0;
			LOCCTR_LINES.push_back(LOCCTR);

		}
		else if (check == 6) {
			SYMBOL temp;
			temp.SYM_NAME = Column_Reader[0];
			temp.SYM_ADDRESS = decToHexa(LOCCTR);
			SYM_TABLE.push_back(temp);
			LOCCTR_LINES.push_back(LOCCTR);
			if (PlusInWord(Column_Reader[1])) {
				LOCCTR += 4;
			}
			else {
				LOCCTR += hexadecimalToDecimal(SizeOfOPCode(Column_Reader[1]));
			}
		}
		else if (check == 7) {
			LOCCTR_LINES.push_back(LOCCTR);
			if (PlusInWord(Column_Reader[0])) {
				LOCCTR += 4;
			}
			else {
				LOCCTR += hexadecimalToDecimal(SizeOfOPCode(Column_Reader[0]));
			}

		}
		else if (check == 8)
		{

		}
		else if (check == -1) {
			std::cout << "ERROR HAS OCCURED WHILE UPDATING LOCCTR PER LINE \n\n\n\n\n\n\n";
		}

		std::cout << "The line being processed is : " << line + 1 << "\n\n\n\n\n\n";

	}

}


void Pass1Reader::Intermediate_File_Creator() {
	std::vector<std::string> Inter_Lines;
	for (int i = 0; i < Data_File.size(); i++) {
		Inter_Lines.push_back(decToHexa(LOCCTR_LINES[i]) + " " + Data_File[i]);
	}
	for (auto j = Inter_Lines.begin(); j < Inter_Lines.end(); j++) {

		Inter_File << (*j) << std::endl;
	}
}


Pass1Reader::~Pass1Reader() {
	SYM_File.close(); Inter_File.close();
}
