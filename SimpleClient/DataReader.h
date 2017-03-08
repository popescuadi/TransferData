#pragma once
# include <iostream>
# include <fstream>
# include <sstream>
using namespace std;
static class DataReader
{
public:
	DataReader();
	static char* ReadFile(char *nume);
	static char **GetMatrix(char* sir,int &number);
	static void BinaryRead(char *&sir, int &lenght);
	~DataReader();
};

