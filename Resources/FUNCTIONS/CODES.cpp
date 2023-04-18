#include<iostream>
#include <string>
class linestring{
private:
	string line[3];
public:
	linestring() {
		line[0] = line[1] = line[2] = "EMPTY";
	}
	void readcols();
};

class LOCCTR {
private:
	unsigned int LOC;
public:
	LOCCTR()
	{
		LOC = 0x0;
	}
	int getOPCODELen(string OPCODE) {
		//Search OPCODE in the OPTAB file
		/*CODE FOR THE SEARCH*/
		return value;
	}
	int getRESsize()

	int getLOCadd(string Command) {
		switch (Command)
		{
		case "START":
		case "END":
			return 0;
		case "BYTE":
			return 1;
		case "WORD":
			return 3;
		case "RESB":
			return getRESsize(/*Next string*/)

		default:
			break;
		}

	}
};