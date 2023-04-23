#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>
//To seperate words from line
void ReadWordsFromLine(std::string& Obj1, std::vector<std::string>& Obj2) {
	unsigned int size = Obj1.length();
	std::string word;
	//extracting words from the line
	for (unsigned int i = 0; i < size; i++) {
		if (Obj1[i] == ' ') {
			(Obj2).push_back(word);
			word = "";
		}
		else {
			word += (Obj1)[i];
			if (i == size - 1)
			{
				Obj2.push_back(word);
				word = "";
			}
		}
	}
}
//To search in the file the desired word given as parameter and return the BYTE size
std::string SearchinFile(std::string File_path,std::string word ) {
	std::ifstream File_ptr;
	File_ptr.open(File_path);
	std::vector<std::string> words;
	std::string line;
	char result=0;
	while (std::getline(Filptr, line)) {
		ReadWordsFromLine(line, words);
		if (words[0] == word) {
			return words[1];
		}
	}
	return "Not_Found";
}
//Class for pass 1
class Pass1Reader {
protected:
	//Variables created to store words, and line input file
	std::vector<std::string> Column_Reader, intermediate_file;
	//Variables created to store the locations of the files 
	std::string file_reader, OP_tab, Directives;
	std::string SYM_Tab, Intermediate;
	// This is the location counter required to create the intermediate file and sym tab file
	unsigned int LOCCTR;


public:
	friend void ReadWordsFromLine(std::string& Obj1, std::vector<std::string>& Obj2);
	Pass1Reader() {
		file_reader="./Input/input1.txt";
		OP_tab="./Resources/OPTAB.txt";
		Directives="./Resources/DIRECTIVES.txt";
		SYM_Tab="./Resources/SYM_Tab.txt";

	}
	void lineReader() {
		while (std::getline(file_reader, line))
		{
			std::string line, word;
			std::getline(file_reader, line);
			intermediate_file.push_back(line);
			int size = std::strlen(line);
			std::string Not_found="Not_Found"
			//extracting words from the line
			ReadWordsFromLine(line, Column_Reader);
			unsigned int size_line = Column_Reader.size();
			std::string hex_value;
			if (size == 1) {

			}
			else if (size_line == 2) {
				hex_value = SearchinFile(Directives, Column_Reader[0]);
				if (hex_value == Not_found) {
					hex_value = SearchinFile(SYM_Tab, Column_Reader[0]);
					if (hex_value == Not_found) {
						std::cout << Column_Reader[0] << std::endl;
					}
				}
			}



		}
	}



};