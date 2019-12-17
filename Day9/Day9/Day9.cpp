// aoc9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <algorithm>
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
  IntcodeComputer(vector<int64> aProgram)
    : mProgram(aProgram), mIp(0), mRelativeBase(0)
  {
    mProgram.resize(1000000);
  }

  void doRun(int aInput)
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
        cout << getParamVal(1, modes) << '\n';
        mIp += 2;
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
        break;
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

  IntcodeComputer comp(program);
  comp.doRun(2);
}