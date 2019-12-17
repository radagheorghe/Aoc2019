// aoc15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <algorithm>
#include <queue>
using namespace std;

typedef long long int64;

class Instruction
{
public:
  enum Opcodes { NONE = 0, SUM, MULI, INPUT, OUTPUT, JMP_TRUE, JMP_FALSE, LESS_THAN, EQUALS, ADJ_BASE };

  static void getOpcode(int64 aInput, int& aOpcode, vector<int>& aModes)
  {
    vector<int> digits;
    while (aInput != 0)
    {
      digits.push_back(aInput % 10);
      aInput = aInput / 10;
    }

    for (size_t i = digits.size(); i < 5; i++)
      digits.push_back(0);

    aOpcode = digits[1] * 10 + digits[0];
    aModes[0] = digits[2];
    aModes[1] = digits[3];
    aModes[2] = digits[4];
  }
};

class IntcodeComputer
{
public:
  enum State { HALT = -111 };

  IntcodeComputer(vector<int64> aProgram)
    : mProgram(aProgram), mIp(0), mRelativeBase(0)
  {
    mProgram.resize(1000000);
  }

  int doRun(int64 aInput)
  {
    while (true)
    {
      int opcode;
      vector<int> modes{ 0, 0, 0 };
      Instruction::getOpcode(mProgram[mIp], opcode, modes);

      if (opcode == Instruction::SUM)
      {
        int64 sum = getParamVal(1, modes) + getParamVal(2, modes);
        writeParamAddress(3, modes, sum);
        mIp += 4;
      }
      else if (opcode == Instruction::MULI)
      {
        int64 multi = getParamVal(1, modes) * getParamVal(2, modes);
        writeParamAddress(3, modes, multi);
        mIp += 4;
      }
      else if (opcode == Instruction::INPUT)
      {
        writeParamAddress(1, modes, aInput);
        mIp += 2;
      }
      else if (opcode == Instruction::OUTPUT)
      {
        int output = (int)getParamVal(1, modes);
        mIp += 2;
        return output;
      }
      else if (opcode == Instruction::JMP_TRUE)
      {
        if (getParamVal(1, modes) != 0)
          mIp = getParamVal(2, modes);
        else
          mIp += 3;
      }
      else if (opcode == Instruction::JMP_FALSE)
      {
        if (getParamVal(1, modes) == 0)
          mIp = getParamVal(2, modes);
        else
          mIp += 3;
      }
      else if (opcode == Instruction::LESS_THAN)
      {
        if (getParamVal(1, modes) < getParamVal(2, modes))
          writeParamAddress(3, modes, 1);
        else
          writeParamAddress(3, modes, 0);
        mIp += 4;
      }
      else if (opcode == Instruction::EQUALS)
      {
        if (getParamVal(1, modes) == getParamVal(2, modes))
          writeParamAddress(3, modes, 1);
        else
          writeParamAddress(3, modes, 0);
        mIp += 4;
      }
      else if (opcode == Instruction::ADJ_BASE)
      {
        mRelativeBase += getParamVal(1, modes);
        mIp += 2;
      }
      else if (opcode == 99)
      {
        return HALT;
      }
    }
  }

private:

  int64* getParam(int64 aParam, const vector<int>& aModes)
  {
    int64* param = &mProgram[mIp + aParam]; // immediate
    if (aModes[aParam - 1] == 0)
      param = &mProgram[mProgram[mIp + aParam]]; // by position
    else if (aModes[aParam - 1] == 2)
      param = &mProgram[mRelativeBase + mProgram[mIp + aParam]]; // relative position

    return param;
  }

  int64 getParamVal(int64 aParam, const vector<int>& aModes)
  {
    return *getParam(aParam, aModes);
  }

  void writeParamAddress(int64 aParam, const vector<int>& aModes, int64 aWrite)
  {
    int64* param = getParam(aParam, aModes);

    *param = aWrite;
  }

  int64 mIp;
  int64 mRelativeBase;
  vector<int64> mProgram;
};

struct Point
{
  Point(int aX, int aY)
    :mX(aX), mY(aY)
  {
  }

  Point(int aX, int aY, int aDist)
    :mX(aX), mY(aY), mDist(aDist)
  {
  }

  bool operator==(const Point& aPoint) const
  {
    return mX == aPoint.mX && mY == aPoint.mY;
  }

  bool operator!=(const Point& aPoint) const
  {
    return mX != aPoint.mX || mY != aPoint.mY;
  }

  int mX;
  int mY;
  int mDist{ 0 };
};

class Droid
{
public:
  enum Direction { NORTH = 1, SOUTH, WEST, EAST };

  Droid(const Point& aPoint)
    :mPosition(aPoint)
  {
  }

  void SetPosition(const Point& aPosition)
  {
    mPosition = aPosition;
  }

  Point GetPosition()
  {
    return mPosition;
  }

  Point GetNextPosition(int aDirection)
  {
    Point nextPosition = mPosition;

    switch (aDirection)
    {
    case NORTH:
      nextPosition.mY++;
      break;
    case SOUTH:
      nextPosition.mY--;
      break;
    case WEST:
      nextPosition.mX--;
      break;
    case EAST:
      nextPosition.mX++;
      break;
    }
    return nextPosition;
  }

  int GetDirFromPosition(const Point& aNew)
  {
    int southNorth = mPosition.mY - aNew.mY;
    int westEast = mPosition.mX - aNew.mX;

    switch (southNorth)
    {
    case -1:
      return NORTH;
    case 1:
      return SOUTH;
    };

    switch (westEast)
    {
    case -1:
      return EAST;
    case 1:
      return WEST;
    };

    return 0;
  }

private:

  Point mPosition;
};

int calcDistance(const Point& aStart, const Point& aDest, const char aSurface[50][50])
{
  bool visited[50][50];
  memset(visited, 0, 50 * 50);

  for (int j = 0; j < 50; j++)
  {
    for (int i = 0; i < 50; i++)
    {
      if (aSurface[i][j] == '#')
        visited[i][j] = true;
    }
  }

  queue<Point> q;
  q.push(aStart);
  visited[aStart.mX][aStart.mY] = true;

  while (!q.empty())
  {
    Point cr = q.front();
    q.pop();

    // Destination found;
    if (cr.mX == aDest.mX && cr.mY == aDest.mY)
      return cr.mDist;

    // moving left
    if (cr.mX - 1 >= 0 && visited[cr.mX - 1][cr.mY] == false)
    {
      q.push(Point(cr.mX - 1, cr.mY, cr.mDist + 1));
      visited[cr.mX - 1][cr.mY] = true;
    }

    // moving right
    if (cr.mX + 1 < 50 && visited[cr.mX + 1][cr.mY] == false)
    {
      q.push(Point(cr.mX + 1, cr.mY, cr.mDist + 1));
      visited[cr.mX + 1][cr.mY] = true;
    }

    // moving down
    if (cr.mY - 1 >= 0 && visited[cr.mX][cr.mY - 1] == false)
    {
      q.push(Point(cr.mX, cr.mY - 1, cr.mDist + 1));
      visited[cr.mX][cr.mY - 1] = true;
    }

    // moving up
    if (cr.mY + 1 < 50 && visited[cr.mX][cr.mY + 1] == false)
    {
      q.push(Point(cr.mX, cr.mY + 1, cr.mDist + 1));
      visited[cr.mX][cr.mY + 1] = true;
    }
  }
  return -1;
}

int main()
{
  ifstream  myfile;

  myfile.open("..//input.txt");
  //myfile.open("..//input1.txt");

  string line;
  vector<int64> program;

  while (getline(myfile, line, ','))
  {
    int64 code = stoull(&line[0]);
    program.push_back(code);
  }

  char surface[50][50];
  memset(surface, 'x', 50 * 50);

  IntcodeComputer comp(program);

  Point start(25, 25);
  Droid wallE(start);

  int direction = Droid::SOUTH;
  Point nextPos = wallE.GetNextPosition(direction);

  Point wallEBack(start);
  vector<Point> deadEnd;

  Point oxySysLocation(start);

  while (true)
  {
    int output = comp.doRun(direction);

    if (output == 0)
    {
      surface[nextPos.mX][nextPos.mY] = '#';
    }
    else if (output == 1)
    {
      surface[nextPos.mX][nextPos.mY] = '.';
      wallEBack = wallE.GetPosition();
      wallE.SetPosition(nextPos);
    }
    else if (output == 2 || output == IntcodeComputer::HALT)
    {
      if (output == 2)
      {
        oxySysLocation = nextPos;
        wallEBack = wallE.GetPosition();
        wallE.SetPosition(nextPos);
      };
      break;
    }

    for (direction = 1; direction <= 4; direction++)
    {
      Point candidate = wallE.GetNextPosition(direction);

      auto found = ::find(deadEnd.begin(), deadEnd.end(), candidate);
      if (found != deadEnd.end())
        continue;
      
      if(candidate == wallEBack)
        continue;

      if (surface[candidate.mX][candidate.mY] == '#')
        continue;
      
      nextPos = candidate;
      break;
    }

    if (direction == 5) //we must go back
    {
      deadEnd.push_back(wallE.GetPosition()); //once we go black we never go back
      nextPos = wallEBack;
      direction = wallE.GetDirFromPosition(nextPos);
    }
  }

  surface[oxySysLocation.mX-1][oxySysLocation.mY] = '#'; //XXX: bug do not exit on output 2
  surface[start.mX][start.mY] = '.';
  surface[oxySysLocation.mX][oxySysLocation.mY] = 'o';

  int open = 0;
  for (int i = 0; i < 50; i++)
  {
    for (int j = 0; j < 50; j++)
    {
      open += surface[i][j] == '.' ? 1 : 0;
      cout << surface[i][j];
    }
    cout << endl;
  }

  cout << calcDistance(start, oxySysLocation, surface) << endl;

  int countMinutes = 0;
  do 
  {
    vector<Point> fill;
    for (int i = 0; i < 50; i++)
    {
      for (int j = 0; j < 50; j++)
      {
        if (surface[i][j] == 'o')
        {
          if (surface[i + 1][j] == '.')
            fill.push_back(Point(i + 1, j));
           
          if (surface[i - 1][j] == '.')
            fill.push_back(Point(i - 1, j));

          if (surface[i][j - 1] == '.')
            fill.push_back(Point(i, j - 1));

          if (surface[i][j + 1] == '.')
            fill.push_back(Point(i, j + 1));
        }
      }
    }
    for (auto point : fill)
    {
      surface[point.mX][point.mY] = 'o';
      open--;
    }
    countMinutes++;
  } while (open > 0);
 
  cout << countMinutes;
}