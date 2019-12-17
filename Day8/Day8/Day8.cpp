// aoc8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <map>
#include <set>
#include <algorithm>
#include <assert.h>
#include <unordered_map>
using namespace std;

struct Layer
{
	static const int kLines = 25;
	static const int kCols = 6;

	typedef int Pixels[kCols][kLines];

	Pixels mPixels;
};

int main()
{
	ifstream  myfile;

	myfile.open("..//input.txt");
	//myfile.open("..//input1.txt");

	string line;

  vector<Layer> layers;

	getline(myfile, line);
	
  int count = 0;
  int min = Layer::kLines * Layer::kCols;
	for(size_t iPx = 0; iPx < line.size(); iPx += Layer::kLines * Layer::kCols)
	{
		Layer layer = { 0 };
		
		int crPx = iPx;
    int zeroPixels = 0;
    int count1 = 0, count2 = 0;
		for (int i = 0; i < Layer::kCols; i++)
		{
			for (int j = 0; j < Layer::kLines; j++)
			{
				int pixel = line[crPx++] - '0';
				layer.mPixels[i][j] = pixel;

				zeroPixels += pixel == 0 ? 1 : 0;

        count1 += pixel == 1 ? 1 : 0;
        count2 += pixel == 2 ? 1 : 0;
			}
		}
		
    if (zeroPixels < min)
    {
      min = zeroPixels;
      count = count1 * count2;
    }

    layers.push_back(layer);
	}

	cout << count << '\n';

	Layer::Pixels & frontLayer = (*layers.begin()).mPixels;

	for (int i = 0; i < Layer::kCols; i++)
	{
		for (int j = 0; j < Layer::kLines; j++)
		{
			auto it = layers.begin();
			for (++it; it != layers.end(); it++) //skip front layer
			{
				Layer::Pixels& backLayer = (*it).mPixels;

				if (frontLayer[i][j] == 2) //transparent
					frontLayer[i][j] = backLayer[i][j];
				else
			      break; // black or white
			}
		}
	}

	for (int i = 0; i < Layer::kCols; i++)
	{
		for (int j = 0; j < Layer::kLines; j++)
			cout << (frontLayer[i][j] == 1 ? '#' : '.');
		cout << '\n';
	}
}