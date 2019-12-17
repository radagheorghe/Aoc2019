// aoc6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <string.h>
#include <algorithm>

using namespace std;

struct node
{
	string mStr;
	int mOrbits;
	vector<node*> mChild;
};

node* findnode(node* aNode, string aStr)
{
	
		if (aNode->mStr == aStr)
			return aNode;
				
	    for(size_t i = 0; i < aNode->mChild.size();i++)
			aNode = findnode(aNode->mChild[i], aStr);
	
		return aNode;
}

void getOrbits(vector<node*>& aNodes, int & aOrbits)
{
	if (aNodes.empty())
		return;

	auto it = aNodes.begin();
	for (; it != aNodes.end(); it++)
	{
		aOrbits += (*it)->mOrbits;
		getOrbits((*it)->mChild, aOrbits);
	}
}

int main()
{
	ifstream  myfile;

	myfile.open("..//input.txt");

	string line;
	vector<node*> nodes;

	while (getline(myfile, line))
	{
		char* next_token;
		string a = strtok_s(&line[0], ")", &next_token);
		string b = next_token;

		node* nodea = nodes.empty() ? nullptr : findnode(*nodes.begin(), a);
		node* nodeb = nodes.empty() ? nullptr : findnode(*nodes.begin(), b);

		if (!nodeb)
		{
			nodeb = new node();
			nodeb->mStr = b;
		}

		if (!nodea)
		{
			nodea = new node();
			nodea->mStr = a;

			nodes.push_back(nodea);
		}

		nodeb->mOrbits = nodea->mOrbits + 1;
		nodea->mChild.push_back(nodeb);
	}

	
	int count = 0;
	getOrbits(nodes, count);
	cout << count;
}