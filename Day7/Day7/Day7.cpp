// aoc7.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

class Instruction
{
public:
  enum Opcodes { NONE = 0, SUM, MULI, INPUT, OUTPUT, JMP_TRUE, JMP_FALSE, LESS_THAN, EQUALS, ADJ_BASE };

  static void getOpcode(int aInput, int& aOpcode, vector<int>& aModes)
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

class IAmplifier
{
public:

  virtual int GetInput() = 0;
  virtual void SetOutput(int aOutput) = 0;
};

class IntcodeComputer
{
public:
  enum State { NEED_IMPUT = 0, HALT = 1 };

  IntcodeComputer(vector<int> aProgram, IAmplifier * aAmplifier)
    : mProgram(aProgram), mIp(0), mRelativeBase(0), mAmplifier(aAmplifier)
  {
  }

  int doRun()
  {
    while (true)
    {
      int opcode;
      vector<int> modes{ 0, 0, 0 };
      Instruction::getOpcode(mProgram[mIp], opcode, modes);

      if (opcode == Instruction::SUM)
      {
        int sum = getParamVal(1, modes) + getParamVal(2, modes);
        writeParamAddress(3, modes, sum);
        mIp += 4;
      }
      else if (opcode == Instruction::MULI)
      {
        int multi = getParamVal(1, modes) * getParamVal(2, modes);
        writeParamAddress(3, modes, multi);
        mIp += 4;
      }
      else if (opcode == Instruction::INPUT)
      {
        int input = mAmplifier->GetInput();
        if (input == -1)
          return NEED_IMPUT;

        writeParamAddress(1, modes, input);
        mIp += 2;
      }
      else if (opcode == Instruction::OUTPUT)
      {
        mAmplifier->SetOutput(getParamVal(1, modes));
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
        return HALT;
      }
    }
  }

private:

  int* getParam(int aParam, const vector<int>& aModes)
  {
    int* param = &mProgram[mIp + aParam]; // immediate
    if (aModes[aParam - 1] == 0)
      param = &mProgram[mProgram[mIp + aParam]]; // by position
    else if (aModes[aParam - 1] == 2)
      param = &mProgram[mRelativeBase + mProgram[mIp + aParam]]; // relative position

    return param;
  }

  int getParamVal(int aParam, const vector<int>& aModes)
  {
    return *getParam(aParam, aModes);
  }

  void writeParamAddress(int aParam, const vector<int>& aModes, int aWrite)
  {
    int* param = getParam(aParam, aModes);

    *param = aWrite;
  }

  int mIp;
  int mRelativeBase;
  vector<int> mProgram;
  IAmplifier* mAmplifier;
};

class Amplifier: public IAmplifier
{
public:
  
	Amplifier(int aPhase, vector<int> aProgram)
	  :mPhase(aPhase), mComputer(aProgram, this), mPhaseUsed(false), 
     mInput(0), mOutput(0), mConnected(nullptr)
	{
	}

  void Connect(Amplifier * aConnect)
  {
    mConnected = aConnect;
  }

	void SetInput(int aInput)
	{
		mInput.push_back(aInput);
	}

	int GetInput()
	{
    if (!mPhaseUsed)
    {
      mPhaseUsed = true;
      return mPhase;
    }
    
    if (mInput.empty())
      return -1;

		int input = mInput.back();
    mInput.pop_back();

		return input;
	}

	void SetOutput(int aOutput)
	{
    assert(mConnected);

    mOutput = aOutput;
		mConnected->SetInput(aOutput);
	}

	int GetOutput()
	{
		return mOutput;
	}

  IntcodeComputer mComputer;

private:

	bool mPhaseUsed;
	int  mPhase;

	vector<int> mInput;
  int         mOutput;

	Amplifier * mConnected;
};

int main()
{
	ifstream  myfile;

	myfile.open("..//input.txt");
  //myfile.open("..//input1.txt");

	string line;
	vector<int> program;
	while (getline(myfile, line, ','))
	{
		int code = atoi(&line[0]);
    program.push_back(code);
	}
	
	int maxOutput = 0;
	
  vector<int> phases = {5, 6, 7, 8, 9};
  do
  {
    vector<unique_ptr<Amplifier>> amps;
    for (auto phase : phases)
      amps.push_back(make_unique<Amplifier>(phase, program));

    for(int i = 0; i < 4; i++)
      amps[i]->Connect(amps[i+1].get());
    amps[4]->Connect(amps[0].get());

    amps[0]->SetInput(0);

    int halt = 0;

    int iAmp = 0;
    while (halt <= 4)
    {
      halt += amps[iAmp++]->mComputer.doRun(); //return 1 on HALT

      if (iAmp > 4)
        iAmp = 0;
    }

    if (maxOutput < amps[4]->GetOutput())
      maxOutput = amps[4]->GetOutput();

  } while (std::next_permutation(phases.begin(), phases.end()));
	
	cout << maxOutput;
}