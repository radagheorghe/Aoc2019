// Day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

class FFT
{
public:

  FFT(const string & aInput)
  {
    for (auto ch : aInput)
      mSignal.push_back(ch - '0');
  }

  void Transform(int aPhase)
  {
    for (int phase = 0; phase < aPhase; phase++)
    {
      vector<int> output(mSignal.size());
      for (size_t i = 0; i < mSignal.size(); i++)
      {
        vector<int> pattern = GetPattern(i + 1);

        for (size_t j = 0; j < mSignal.size(); j++)
          output[i] += mSignal[j] * pattern[j + 1];

        output[i] = abs(output[i]) % 10;
      }
      mSignal = output;
    }
  }

  int GetOffset()
  {
    int x = 1000000;

    int offset = 0;
    for (int i = 0; i < 7; i++)
    {
      offset += mSignal[i] * x;
      x = x / 10;
    }

    return offset;
  }

  const vector<int> & GetSignal()
  {
    return mSignal;
  }

private:

  vector<int> GetPattern(int aIdx)
  {
    vector<int> base{ 0, 1, 0, -1 };

    int crBase = 0;
    vector<int> pattern;
    while(pattern.size() < mSignal.size() + 1)
    {
      for (int i = 0; i < aIdx; i++)
        pattern.push_back(base[crBase]);

      crBase++;
      if (crBase > 3)
        crBase = 0;
    }

    return pattern;
  }

  vector<int> mSignal;
};

int main()
{
  ifstream  myfile;

  //myfile.open("..//input.txt");
  myfile.open("..//input1.txt");
    
  string strLine;
  getline(myfile, strLine);

  FFT fft(strLine);

  int offset = fft.GetOffset();

  fft.Transform(100);

  vector<int> signal = fft.GetSignal();
  for (int i = 0; i < 8; i++)
    cout << signal[i];
}
