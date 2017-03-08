#pragma once
# include <iostream>
#include <string>
using namespace std;
struct Status
{
	int x = 0;
	char * GetStatus()
	{
		if (x = 0)
			return "Serverul e online";
		if (x == 1)
			return "Serverul porneste protocolul de transfer";
		if (x == 2)
			return "Serverul a terminat transferul";
		if (x == 3)
			return "Serverul a intampinat o problema";
		x = 0;
		return "Something is broken";

	}
	void Reset()
	{
		if (x==2||x==3)
			x = 0;
	}
};