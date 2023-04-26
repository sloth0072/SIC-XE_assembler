#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>

int hexadecimalToDecimal(std::string hexVal)
{
	int len = hexVal.size();

	// Initializing base value to 1, i.e 16^0
	int base = 1;

	int dec_val = 0;

	// Extracting characters as digits from last
	// character
	for (int i = len - 1; i >= 0; i--) {
		// if character lies in '0'-'9', converting
		// it to integral 0-9 by subtracting 48 from
		// ASCII value
		if (hexVal[i] >= '0' && hexVal[i] <= '9') {
			dec_val += (int(hexVal[i]) - 48) * base;

			// incrementing base by power
			base = base * 16;
		}

		// if character lies in 'A'-'F' , converting
		// it to integral 10 - 15 by subtracting 55
		// from ASCII value
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
			dec_val += (int(hexVal[i]) - 55) * base;

			// incrementing base by power
			base = base * 16;
		}
	}
	return dec_val;
}
//To seperate words from line
void ReadWordsFromLine(std::string& Obj1, std::vector<std::string>& Obj2) {
	Obj2.clear();
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
void CreateFileasStringVector(std::string File_Path, std::vector<std::string> &File_String) {
	std::string line;
	std::ifstream File_Ptr;
	File_Ptr.open(File_Path);
	while (std::getline(File_Ptr, line)) {
		File_String.push_back(line);
	}

}
int DecStringtoDec(std::string Dec_String) {
	int len = Dec_String.size();

	// Initializing base value to 1, i.e 16^0
	int base = 1;

	int dec_val = 0;

	// Extracting characters as digits from last
	// character
	for (int i = len - 1; i >= 0; i--) {
		// if character lies in '0'-'9', converting
		// it to integral 0-9 by subtracting 48 from
		// ASCII value
		if (Dec_String[i] >= '0' && Dec_String[i] <= '9') {
			dec_val += (int(Dec_String[i]) - 48) * base;

			// incrementing base by power
			base = base * 10;
		}
	}
	return dec_val;
}

std::string decToHexa(int n)
{
	// ans string to store hexadecimal number
	std::string ans = "";

	while (n != 0) {
		// remainder variable to store remainder
		int rem = 0;

		// ch variable to store each character
		char ch;
		// storing remainder in rem variable.
		rem = n % 16;

		// check if temp < 10
		if (rem < 10) {
			ch = rem + 48;
		}
		else {
			ch = rem + 55;
		}

		// updating the ans string with the character variable
		ans += ch;
		n = n / 16;
	}

	// reversing the ans string to get the final result
	int i = 0, j = ans.size() - 1;
	while (i <= j)
	{
		std::swap(ans[i], ans[j]);
		i++;
		j--;
	}
	return ans;
}


//Using this function instead of all the remove plus and comma elimintaor;
std::string RemoveAllSpecicalChar(std::string Word) {
	std::string return_word = "";
	for (int i = 0; i < Word.size(); i++) {
		if (Word[i]=='#' || Word[i]=='+' || Word[i] == ',' || Word[i]=='@')
			continue;
		return_word += Word[i];
	}
	return return_word;
}
bool PlusInWord(std::string Word) {
	for (int i = 0; i < Word.size(); i++) {
		if (Word[i] == '+')
			return true;
	}
	return false;
}