// Day12.cpp : main project file.

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

using namespace std;

class Point
{
public:

  Point(int aX, int aY, int aZ)
    :mX(aX), mY(aY), mZ(aZ)
  {

  }

  Point(const Point& aPoint)
    :mX(aPoint.mX), mY(aPoint.mY), mZ(aPoint.mZ)
  {

  }

  bool operator==(const Point& aPoint) const
  {
    return mX == aPoint.mX && mY == aPoint.mY && mZ == aPoint.mZ;
  }

  void operator+=(const Point& aPoint)
  {
    mX += aPoint.mX;
    mY += aPoint.mY;
    mZ += aPoint.mZ;
  }

  int mX;
  int mY;
  int mZ;
};

struct Moon
{
public:

  Moon(int aX, int aY, int aZ, int aId)
    :mPosition(aX, aY, aZ), mVelocity(0, 0, 0), mInitialPos(aX, aY, aZ)
  {
  }

  void UpdatePosition()
  {
    mPosition += mVelocity;
  }

  Point mInitialPos;
  Point mPosition;
  Point mVelocity;
};

void simulateUnivers(vector<Moon>& aMoons)
{
  for (size_t i = 0; i < aMoons.size(); i++)
  {
    for (size_t j = i + 1; j < aMoons.size(); j++)
    {
      if (aMoons[i].mPosition.mX < aMoons[j].mPosition.mX)
      {
        aMoons[i].mVelocity.mX += 1;
        aMoons[j].mVelocity.mX -= 1;
      }
      else if (aMoons[i].mPosition.mX > aMoons[j].mPosition.mX)
      {
        aMoons[i].mVelocity.mX -= 1;
        aMoons[j].mVelocity.mX += 1;
      }

      if (aMoons[i].mPosition.mY < aMoons[j].mPosition.mY)
      {
        aMoons[i].mVelocity.mY += 1;
        aMoons[j].mVelocity.mY -= 1;
      }
      else if (aMoons[i].mPosition.mY > aMoons[j].mPosition.mY)
      {
        aMoons[i].mVelocity.mY -= 1;
        aMoons[j].mVelocity.mY += 1;
      }

      if (aMoons[i].mPosition.mZ < aMoons[j].mPosition.mZ)
      {
        aMoons[i].mVelocity.mZ += 1;
        aMoons[j].mVelocity.mZ -= 1;
      }
      else if (aMoons[i].mPosition.mZ > aMoons[j].mPosition.mZ)
      {
        aMoons[i].mVelocity.mZ -= 1;
        aMoons[j].mVelocity.mZ += 1;
      }
    }
  }

  for (size_t i = 0; i < aMoons.size(); i++)
    aMoons[i].UpdatePosition();
}

void printMoonsLocation(vector<Moon>& aMoons)
{
  for (size_t i = 0; i < aMoons.size(); i++)
    cout << '<' << aMoons[i].mPosition.mX << ',' << aMoons[i].mPosition.mY << ',' << aMoons[i].mPosition.mZ << '>';
  cout << endl;
}

void printMoonsVelocity(vector<Moon>& aMoons)
{
  for (size_t i = 0; i < aMoons.size(); i++)
    cout << '<' << aMoons[i].mVelocity.mX << ',' << aMoons[i].mVelocity.mY << ',' << aMoons[i].mVelocity.mZ << '>';
  cout << endl;
}

void simulateUnivers(vector<Moon>& aMoons, int aSteps)
{
  for (int step = 0; step < aSteps; step++)
    simulateUnivers(aMoons);
}

long long gcd(long long a, long long b)
{
  if (b == 0)
    return a;
  return gcd(b, a % b);
}

long long findlcm(int arr[], int n)
{
  long long ans = arr[0];

  for (int i = 1; i < n; i++)
    ans = (arr[i] * ans) / gcd(arr[i], ans);

  return ans;
}

long long simpulateUniversToOrigin(vector<Moon>& aMoons)
{
  int steps = 1;
  map<tuple<int, int, int, int>, int> mSeenx;
  map<tuple<int, int, int, int>, int> mSeeny;
  map<tuple<int, int, int, int>, int> mSeenz;

  while (true)
  {
    simulateUnivers(aMoons);
    steps++;
    
    auto foundx = mSeenx.find(make_tuple(aMoons[0].mInitialPos.mX, 
      aMoons[1].mInitialPos.mX, aMoons[2].mInitialPos.mX, aMoons[3].mInitialPos.mX));
    auto foundy = mSeeny.find(make_tuple(aMoons[0].mInitialPos.mY, 
      aMoons[1].mInitialPos.mY, aMoons[2].mInitialPos.mY, aMoons[3].mInitialPos.mY));
    auto foundz = mSeenz.find(make_tuple(aMoons[0].mInitialPos.mZ, 
      aMoons[1].mInitialPos.mZ, aMoons[2].mInitialPos.mZ, aMoons[3].mInitialPos.mZ));

    if (foundx != mSeenx.end() && foundy != mSeeny.end() && foundz != mSeenz.end())
    {
      int stepsx = foundx->second;
      int stepsy = foundy->second;
      int stepsz = foundz->second;

      int matches[3] = { stepsx, stepsy, stepsz };
      return findlcm(matches, 3);
    }

    mSeenx.insert(make_pair(make_tuple(aMoons[0].mPosition.mX, 
      aMoons[1].mPosition.mX, aMoons[2].mPosition.mX, aMoons[3].mPosition.mX), steps));
    mSeeny.insert(make_pair(make_tuple(aMoons[0].mPosition.mY, 
      aMoons[1].mPosition.mY, aMoons[2].mPosition.mY, aMoons[3].mPosition.mY), steps));
    mSeenz.insert(make_pair(make_tuple(aMoons[0].mPosition.mZ, 
      aMoons[1].mPosition.mZ, aMoons[2].mPosition.mZ, aMoons[3].mPosition.mZ), steps));
  }
  return -1;
}

int main()
{
  ifstream  myfile;
  myfile.open("..//input.txt");
  //myfile.open("..//input1.txt");
  //myfile.open("..//input2.txt");

  //ofstream outpufile;
  //outpufile.open("..//output.txt");
  //std::cout.rdbuf(outpufile.rdbuf());

  vector<Moon> moons;

  string strLine;

  int id = 0;
  while (getline(myfile, strLine))
  {
    int x, y, z;
    sscanf_s(&strLine[0], "<x=%d, y=%d, z=%d>", &x, &y, &z);

    Moon mn(x, y, z, ++id);
    moons.push_back(mn);
  }
  myfile.close();
  /*
  simulateUnivers(moons, 1000);

  int totalEnergy = 0;
  for (size_t i = 0; i < moons.size(); i++)
  {
    int potentialEnergy = abs(moons[i].mPosition.mX) + abs(moons[i].mPosition.mY) + abs(moons[i].mPosition.mZ);
    int kineticEnergy = abs(moons[i].mVelocity.mX) + abs(moons[i].mVelocity.mY) + abs(moons[i].mVelocity.mZ);
    int moonTotalEnergy = potentialEnergy * kineticEnergy;

    totalEnergy += moonTotalEnergy;
  }

  cout << totalEnergy << endl;
  */
  cout << simpulateUniversToOrigin(moons);
}
