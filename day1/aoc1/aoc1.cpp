// aoc1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
using namespace std;

int getFuel(int aMass)
{
	return (int)round(aMass / 3) - 2;
}

int getFuelOfFuel(int aFuel)
{
	int sumFuel = 0;
	int fuel = aFuel;
	while (true)
	{
		fuel = getFuel(fuel);
		
		if (fuel < 0)
			break;

		sumFuel += fuel;
	}
	return sumFuel;
}

int main()
{
	ifstream  myfile;

	myfile.open("..//input.txt");

	string line;
	
	int sumFuel = 0;
	int sumFuelOfFuel = 0;
	while (getline(myfile, line))
	{
		int mass = atoi(&line[0]);
		sumFuel += getFuel(mass);
		sumFuelOfFuel += getFuelOfFuel(mass);
	}

	cout << "Part1: " << sumFuel << "\n";
	cout <<	"Part2: " << sumFuelOfFuel;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
