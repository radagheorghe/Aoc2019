// aoc5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
using namespace std;

void getOpcode(int aInput, int & aOpcode, int & aInput1, int & aInput2, int & aOutput)
{
	string opcode = std::to_string(aInput);
	while (opcode.size() < 5)
		opcode.insert(opcode.begin(), ' ');

	aOpcode = std::atoi(&opcode[3]);
	aInput1 = opcode[2] == ' ' ? 0 : opcode[2] - '0';
	aInput2 = opcode[1] == ' ' ? 0 : opcode[1] - '0';
	aOutput = opcode[0] == ' ' ? 0 : opcode[0] - '0';
}

int main()
{
	ifstream  myfile;

	myfile.open("..//input.txt");

	string line;

	vector<int> program;

	while (getline(myfile, line, ','))
	{
		int code = atoi(&line[0]);
		program.push_back(code);
	}
	
	int id = 1;

	int i = 0;
	while(true)
	{
		int opcode, input1, input2, output;
		getOpcode(program[i], opcode, input1, input2, output);

		if (opcode == 3)
		{
			program[program[i + 1]] = id;
			i += 2;
		}
		else if (opcode == 4)
		{
			if(input1 == 1)
				cout << program[i + 1] << '\n';
			else
				cout << program[program[i + 1]] << '\n';
			i += 2;
		}
		else if (opcode == 99)
		{
			break;
		}
		else
		{
			int pInput1 = program[i + 1];
			if (input1 == 0)
				pInput1 = program[program[i + 1]];

			int pInput2 = program[i + 2];
			if (input2 == 0)
				pInput2 = program[program[i + 2]];

			if (opcode == 5)
			{
				if (pInput1 != 0)
					i = pInput2;
				else
					i += 3;
			}
			else if (opcode == 6)
			{
				if (pInput1 == 0)
					i = pInput2;
				else
					i += 3;
			}
			else if (opcode == 7)
			{
				if (pInput1 < pInput2)
					program[program[i + 3]] = 1;
				else
					program[program[i + 3]] = 0;
				i += 4;
			}
			else if (opcode == 8)
			{
				if (pInput1 == pInput2)
					program[program[i + 3]] = 1;
				else
					program[program[i + 3]] = 0;
				i += 4;
			}
			else if (opcode == 1)
			{
				int sum = pInput1 + pInput2;
				program[program[i + 3]] = sum;
				i += 4;
			}
			else if (opcode == 2)
			{
				int multi = pInput1 * pInput2;
				program[program[i + 3]] = multi;
				i += 4;
			}
		}
	}

	//cout << program[0];
}