// aoc3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
using namespace std;

struct Point
{
	Point(int ax, int ay)
	{
		x = ax;
		y = ay;
		steps = 0;
	}

	Point(int ax, int ay, int aSteps)
	{
		x = ax;
		y = ay;
		steps = aSteps;
	}

	bool operator== (const Point& aP)
	{
		return x == aP.x && y == aP.y;
	}

	int x;
	int y;
	int steps;
};

int main()
{
	ifstream  myfile;

	myfile.open("..//input.txt");

	string line;
	
	vector<Point> line1;
	vector<Point> intersections;

	int steps1 = 0;
	int steps2 = 0;

	int lineId = 0;
	int originX = 0, originY = 0;
	while (getline(myfile, line))
	{
		lineId++;
		// stringstream class check1 
		stringstream check1(line);

		string intermediate;

		int x = originX, y = originY;
		while (getline(check1, intermediate, ','))
		{
			char code = intermediate[0];
			int dist = atoi(&intermediate[1]);

			for (int i = 0; i < dist; i++)
			{
				if (lineId == 1)
					steps1 ++;
				else
					steps2 ++;

				if (code == 'U')
				{
					y++;
					if (lineId == 2)
					{
						auto found = ::find(line1.begin(), line1.end(), Point(x, y));
						if (found != line1.end())
							intersections.push_back(Point(x, y, steps2));
					}
					else
						line1.push_back(Point(x, y, steps1));
				}
				else if (code == 'D')
				{
					y--;
					if (lineId == 2)
					{
						auto found = ::find(line1.begin(), line1.end(), Point(x, y));
						if (found != line1.end())
							intersections.push_back(Point(x, y, steps2));
					}
					else
						line1.push_back(Point(x, y, steps1));
				}
				else if (code == 'R')
				{
					x++;
					if (lineId == 2)
					{
						auto found = ::find(line1.begin(), line1.end(), Point(x, y));
						if (found != line1.end())
							intersections.push_back(Point(x, y, steps2));
					}
					else
						line1.push_back(Point(x, y, steps1));
				}
				else if (code == 'L')
				{
					x--;
					if (lineId == 2)
					{
						auto found = ::find(line1.begin(), line1.end(), Point(x, y));
						if (found != line1.end())
							intersections.push_back(Point(x, y, steps2));
					}
					else
						line1.push_back(Point(x, y, steps1));
				}
			}
		}
		
	}
	
	/*int min = 10000;
	auto it = intersections.begin();
	for(;it!= intersections.end();it++)
	{
		Point& p = *it;
		int dist = abs(p.x - originX) + abs(p.y - originY);
		if (min > dist)
			min = dist;
			
	}
	cout << min << '\n';*/

    int min = 1000000;
	auto it = intersections.begin();
	for (; it != intersections.end(); it++)
	{
		Point& p = *it;
		int sum = 0;
		auto found = ::find(line1.begin(), line1.end(), Point(p.x, p.y));
		if (found != line1.end())
		{
			sum = found->steps + p.steps;
		}
		if (min > sum)
			min = sum;
	}
	cout << min << '\n';

}