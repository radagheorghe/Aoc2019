// aoc4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
using namespace std;

bool isPassword(int aNr)
{
	int countCresc = 0;
	bool hasTowDigit = false;
	int x = 100000;
	bool hasLargerGroup = false;

	int largerGroup = -1;
	int cifraTrecuta = -1;
	int cifraTrecuta1 = -1;
	while (aNr != 0)
	{
		int cifraCurenta = aNr / x;
		
		if (cifraCurenta == cifraTrecuta && cifraTrecuta == cifraTrecuta1)
		{
			largerGroup = cifraCurenta;
			hasLargerGroup = true;
		}
		else if (cifraCurenta == cifraTrecuta)
		{
			if (largerGroup != -1 && cifraCurenta > largerGroup)
				hasLargerGroup = false;

		    hasTowDigit = true;
		}

		if (cifraTrecuta != -1 && cifraCurenta >= cifraTrecuta)
			countCresc++;

		aNr = aNr % x;
		x = x / 10;

		cifraTrecuta1 = cifraTrecuta;
		cifraTrecuta = cifraCurenta;
	}
	return hasTowDigit && countCresc == 5 && !hasLargerGroup;
}

int main()
{
	int start = 387638;
	int end = 919123;

	bool is = isPassword(488999);
	is;

	int count = 0;
	for (int i = start; i < end; i++)
	{
		if (isPassword(i))
			count++;
	}
	cout << count;
}