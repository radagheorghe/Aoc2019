// Day10.cpp : main project file.

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

using namespace std;

struct Point
{
  Point(double aX, double aY)
    :mX(aX), mY(aY)
  {

  }

  bool operator==(const Point& aPoint) const
  {
    return mX == aPoint.mX && mY == aPoint.mY;
  }

  double mX;
  double mY;
};

struct Line
{
  enum lType { DIAGONAL = 0, VERTICAL = 1 };

  Line(Point aP1, Point aP2)
    :mP1(aP1), mP2(aP2)
  {

  }

  Point mP1;
  Point mP2;
};

void calcLineEq(Point aP1, Point aP2, double& aM, double& aB)
{
  aM = (aP2.mY - aP1.mY) / (aP2.mX - aP1.mX);
  aB = aP1.mY - aM * aP1.mX;
}

bool pointIsOnALine(const Point& aPoint, const Line& aLinie)
{
  if (aLinie.mP1.mX == aLinie.mP2.mX)
    return aPoint.mX == aLinie.mP1.mX;

  double m, b;
  calcLineEq(aLinie.mP1, aLinie.mP2, m, b);

  double diff = abs(aPoint.mY - (aPoint.mX * m + b));
  return diff <= 0.0001;
}

double calcDist(const Point& aP1, const Point& aP2)
{
  return std::sqrt(std::pow(abs(aP1.mX - aP2.mX), 2) + std::pow(abs(aP1.mY - aP2.mY), 2));
}

bool isBetwen(const Point& aA, const Point& aB, const Point& aC)
{
  return calcDist(aB, aA) < calcDist(aB, aC) && calcDist(aA, aC) < calcDist(aB, aC);
}

int main()
{
  ifstream  myfile;
  myfile.open("..//input.txt");
  //myfile.open("..//input1.txt");
  //myfile.open("..//input2.txt");
  //myfile.open("..//input3.txt");
  //myfile.open("..//input4.txt");
  //myfile.open("..//input5.txt");

  vector<Point> surface;

  int lineNr = 0;
  string strLine;

  int lineSize = 0;
  while (getline(myfile, strLine))
  {
    lineSize = strLine.length();

    auto it = strLine.begin();
    for (int i = 0; it != strLine.end(); it++, i++)
    {
      if (*it == '#')
        surface.push_back(Point(i, lineNr));
    }
    lineNr++;
  }
  myfile.close();

  size_t max = 0;
  Point start(0, 0);
  vector<Point> directAsterioids;
  for (auto point : surface)
  {
    vector<Point> detect;
    for (auto otherPoint : surface)
    {
      if (point == otherPoint)
        continue;

      Line ln(point, otherPoint);

      bool blockingPoins = false;
      for (auto blockingPoint : surface)
      {
        if (blockingPoint == ln.mP1 || blockingPoint == ln.mP2)
          continue;

        if (pointIsOnALine(blockingPoint, ln) && isBetwen(blockingPoint, ln.mP1, ln.mP2))
        {
          blockingPoins = true;
          break;
        }
      }
      if (!blockingPoins)
        detect.push_back(otherPoint);
    }
    if (max < detect.size())
    {
      max = detect.size();
      start = point;
      directAsterioids = detect;
    }
  }

  cout << max << '\n';

  auto compare_angle = [&](Point aFist, Point aSecond)
  {
    double angle1 = std::atan2(aFist.mX - start.mX, aFist.mY - start.mY) * 180 / M_PI;
    double angle2 = std::atan2(aSecond.mX - start.mX, aSecond.mY - start.mY) * 180 / M_PI;

    return angle1 > angle2;
  };

  sort(directAsterioids.begin(), directAsterioids.end(), compare_angle);

  cout << directAsterioids[199].mX * 100 + directAsterioids[199].mY << endl;

  map<double, map<double, Point>> reduce;
  for (auto pt : surface)
  {
    if (pt == start)
      continue;

    double angle = std::atan2(pt.mY - start.mY, pt.mX - start.mX) * 180 / M_PI;

    double dist = calcDist(start, pt);
    auto found = reduce.find(angle);
    if (found != reduce.end())
      found->second.insert(make_pair(dist, pt));
    else
    {
      map<double, Point> points;
      points.insert(make_pair(dist, pt));
      reduce.insert(make_pair(angle, points));
    }
  }

  int vaporized = 1;
  auto it = reduce.find(-90.00);
  while (true)
  {
    if (it == reduce.end())
      it = reduce.begin();

    if (vaporized == 200)
    {
      cout << (*it).second.begin()->second.mX * 100 + (*it).second.begin()->second.mY;
      break;
    }
    if (!(*it).second.empty())
    {
      (*it).second.erase((*it).second.begin());
      vaporized++;
    }
    it++;
  }
}