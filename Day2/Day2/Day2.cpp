// aoc2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
using namespace std;

int main()
{
	ifstream  myfile;

	myfile.open("..//input.txt");

	string line;

	vector<int> program;
	vector<int> programInit;

	while (getline(myfile, line, ',')) 
	{
		int code = atoi(&line[0]);
		programInit.push_back(code);
	}

	for(int noun = 0; noun < 99; noun++)
		for (int verb = 0; verb < 99; verb++)
		{
			program.assign(programInit.begin(), programInit.end());

			program[1] = noun;
			program[2] = verb;

			for (size_t i = 0; i < program.size(); i += 4)
			{
				int opcode = i;

				if (program[opcode] == 99)
				{
					break;
				}

				int input1 = program[i + 1];
				int input2 = program[i + 2];
				int output = program[i + 3];

				if (program[opcode] == 1)
				{
					int sum = program[input1] + program[input2];
					program[output] = sum;
				}
				else if (program[opcode] == 2)
				{
					int multi = program[input1] * program[input2];
					program[output] = multi;
				}
				if (program[0] == 19690720)
				{
					cout << 100*noun+verb;
					break;
				}
			}
		}

	//cout << program[0];
}