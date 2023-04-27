#pragma once
#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>
using namespace std;


class SYMBOL {
public:
	std::string SYM_NAME, SYM_ADDRESS;
	SYMBOL();
};

std::string decToHexaString(int n);

void CreateFileasStringVector(std::string File_Path, std::vector<std::string>& File_String);


void ReadWordsFromLine(std::string& Obj1, std::vector<std::string>& Obj2);


int hexadecimalToDecimal(std::string hexVal);


std::string decToHexa(int n);


bool PlusInWord(std::string Word);


std::string RemoveAllSpecicalChar(std::string Word);


int DecStringtoDec(std::string Dec_String);

class Pass1Reader {
protected:
	//Variables created to store words, and line input file
	std::vector<std::string> Column_Reader;
	std::vector<std::string> Data_File, OP_File;
	//Variables created to store the locations of the files 
	std::string file_reader, OP_tab, Directory;
	std::string SYM_Tab, Intermediate;

	
	std::ofstream SYM_File, Inter_File;
	std::vector<SYMBOL> SYM_TABLE;
	// This is the location counter required to create the intermediate file and sym tab file
	unsigned int LOCCTR;
	std::vector<int> LOCCTR_LINES;


	// SYM_TABLE needs to be built for each csect
	std::vector<SYMBOL> EXTREF,EXTDEF;



public:
	Pass1Reader();
	bool CheckIfOPCode(std::string word);
	std::string SizeOfOPCode(std::string OP_Code);
	bool CheckIfSYMTABLE(std::string word);
	int VariableSize();
	int DirectiveChecker();


	void lineReader();
	void Intermediate_File_Creator();

	~Pass1Reader();
	void UpdateBaseValue();

};

class OP_Codes {
public:
	string OBJ_Code, Location;
};

class Pass2Reader {
private:
	vector<string> Inter_File, SYM_File, EXTREF, EXTDEF, LOCATIONS, Line_Reader,Next_Line, OP_File;
	vector<OP_Codes> OBJ_File;
	string PC_Value, BASE_Value;
public:
	Pass2Reader();
	void line_by_line();
	string ValueOfWord();
	int TypeChecker();
	bool CheckIfOPCode(std::string word);

	string ValueOfByte();

	string OP_Code_Value(string,string);
	
	string Flags_Value(string,string);
	
	string Displacement_Value(string);
	
	string ValueOfOPCode(std::string OP_Code);

	
	string Address_Value(string);


	string SearchInSymTab_ReturnAddress(string);

	bool SearchInSymTab_ifPresent(string VARIABLE);
	
	

};