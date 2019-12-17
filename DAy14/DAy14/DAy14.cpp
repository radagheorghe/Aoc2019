// Day14.cpp : main project file.

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <windows.h>
#include <cmath>
#include <corecrt_math_defines.h>
#include <set>
#include <regex>

using namespace std;

vector<string> tokenize(const string& aStr, const string& aDelim)
{
  vector<string> tokens;
  size_t prev = 0, pos = 0;
  do
  {
    pos = aStr.find(aDelim, prev);
    if (pos == string::npos) pos = aStr.length();
    string token = aStr.substr(prev, pos - prev);
    if (!token.empty()) tokens.push_back(token);
    prev = pos + aDelim.length();
  } 
  while (pos < aStr.length() && prev < aStr.length());

  return tokens;
}

class Reaction
{
public:
  string mName;
  int mCost;

  bool operator==(const Reaction& aRecation) const {
    return mName == aRecation.mName;
  }

  bool operator==(const string& aName) const {
    return mName == aName;
  }
};

bool isUsedInOtherReaction(string aName, map<string, pair<int, vector<Reaction>>> & aReactions)
{
  for (auto react : aReactions)
  {
    if(react.first == "FUEL")
      continue;

    auto found = ::find(react.second.second.begin(), react.second.second.end(), aName);
    if (found != react.second.second.end())
      return true;
  }
  return false;
}

bool hasOreAsDirectImput(const vector<Reaction> & aReactionList)
{
  if (aReactionList.size() == 1 && aReactionList[0].mName == "ORE") 
    return true;

  return false;
}

int main()
{
  ifstream  myfile;
  //myfile.open("..//input.txt");
  //myfile.open("..//input1.txt");
  //myfile.open("..//input2.txt");
  //myfile.open("..//input3.txt");
  //myfile.open("..//input4.txt");
  myfile.open("..//input5.txt");

  map<string, pair<int, vector<Reaction>>> reactions;

  string strLine;
  while (getline(myfile, strLine))
  {
    vector<string> tokens = tokenize(strLine, " => ");
    vector<string> tkLeft = tokenize(tokens[0], ", ");

    vector<Reaction> contain;
    for (auto tk : tkLeft)
    {
      Reaction reaction;
      vector<string> tokens = tokenize(tk, " ");
      reaction.mName = tokens[1];
      reaction.mCost = atoi(tokens[0].c_str());
      contain.push_back(reaction);
    }

    vector<string> tkRight = tokenize(tokens[1], " ");
    reactions.insert(make_pair(tkRight[1], make_pair(atoi(tkRight[0].c_str()), contain)));
  }
  myfile.close();

  int shouldContinue;
  do
  {
    auto & fuel = reactions["FUEL"];

    shouldContinue = false;

    string toDelReactionName;

    auto it = fuel.second.begin();
    for (;it!=fuel.second.end();it++)
    {
      Reaction& currentReaction = (*it);
      
      auto reactChild = reactions[currentReaction.mName];
      int need = (int)std::ceil((double)currentReaction.mCost / reactChild.first);

      if(isUsedInOtherReaction(currentReaction.mName, reactions))
        continue;

      if (hasOreAsDirectImput(reactChild.second)) //ORE as direct input
        continue;

      toDelReactionName = currentReaction.mName;
      
      fuel.second.erase(it);
      for (auto rcChild : reactChild.second)
      {
        rcChild.mCost *= need;
        auto found = ::find(fuel.second.begin(), fuel.second.end(), rcChild);
        if (found != fuel.second.end())
          found->mCost += rcChild.mCost;
        else
          fuel.second.push_back(rcChild);
      }
      shouldContinue = true;
      break;
    }

    if(!toDelReactionName.empty())
      reactions.erase(toDelReactionName);
  } 
  while (shouldContinue);

  int cost = 0;
  auto fuel = reactions["FUEL"]; // reduce direct input
  for (auto reaction : fuel.second)
  {
    auto reactChild = reactions[reaction.mName];
    int need = (int)std::ceil((double)reaction.mCost / reactChild.first);

    cost += reactChild.second[0].mCost * need;
  }

  cout << cost << endl;

  int need = (int)std::ceil((double)1000000000000 / cost);
  cout << need;
}
