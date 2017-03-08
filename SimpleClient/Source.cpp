# include <iostream>
# include "ClientTCP.h"
#include "DataReader.h"
//# include <vector>
char * converter(int x)
{
	char *h;
	h = new char[4];
	itoa(x, h, 10);
	return h;
}
int min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}
void BinaryFormatter(std::vector<string> &a, char *sir, int lenght)
{
	if (lenght < 150000)
	{
		std::string aux;
		for (int i = 0;i < lenght;i++)
		{
			int x = sir[i];
			aux = aux + converter(x);
			if (i == lenght - 1)
				aux = aux + "\n";
			else
				aux = aux + "|";
		}
		a.push_back(aux);
	}
	else
	{
		int k = 0;
		while (lenght != 0)
		{
			std::string aux;
			int n= min(lenght, 150000);
			lenght = lenght - n;
			for (int i = 0;i < n;i++)
			{
				int x = sir[k];
				//std::cout << k << endl;
				k++;
				aux = aux + converter(x);
				if (i == n - 1)
					aux = aux + "\n";
				else
					aux = aux + "|";
			}
			std::cout << k << endl;
			a.push_back(aux);
		}
	}
}
void BinaryFormatterWhitChar(char** &pakets, char *sir, int &lenght)
{
	long int nrtot = 0;
	int x = lenght / 150000;
	if (lenght % 1500 != 0)
		x++;
	pakets = new char*[x];
	int k = 0;
	int poz = 0;
	while (lenght != 0)
	{
		char *p = new char[700000];
		int l = 0;
		int n = min(150000, lenght);
		lenght = lenght - n;
		for (int i = 0;i < n;i++)
		{
			int z = sir[k];
			k++;
			char *ch = converter(z);
			for (int j = 0;j < strlen(ch);j++)
			{
				p[l] = ch[j];
				l++;
			}
			p[l] = '|';
			l++;
		}
		p[l] = '\n';
		l++;
		p[l] = NULL;
		l++;
		pakets[poz] = new char[l];
		for (int i = 0;i < l;i++)
			pakets[poz][i] = p[i];
		poz++;
		std::cout << poz << endl;
	}
	lenght = x;
}
void main()
{
	while (1)
	{
		char choise[50];
		cin.getline(choise, 49);
		if (strcmp(choise, "exit") == 0)
			break;
		const std::string ip = "192.168.0.103";
		unsigned short port = 3333;
		char **x;
		x = new char*[3];
		x[0] = _strdup("Server Status\n");
		x[1] = _strdup("Porneste protocolul de transfer\n");
		char *buf = new char[40];
		int i;
		i = 0;
		std::vector<string> bufffer;
		int lenght = 0;
		char **matrix;
		if (strcmp(choise, "change path") == 0)
		{
			try
			{
				ClientTcp client(ip, port);
				client.Connect();
				std::cout << x[i] << std::endl;
				std::string response = client.EmulatedString("change path\n");
				std::cout << response << std::endl;
				Sleep(2000);
			}
			catch (system::system_error &e)
			{
				std::cout << e.code() << " " << e.what();
				//return e.code().value();
			}
		}
		if (strcmp(choise, "plain text") == 0)
		{
			std::cin >> buf;
			x[2] = _strdup(buf);
			char **mat = NULL;
			int n;
			mat = DataReader::GetMatrix(DataReader::ReadFile(buf), n);
			while (i < 4)
			{
				if (i < 3)
				{
					try
					{
						ClientTcp client(ip, port);
						client.Connect();
						std::cout << x[i] << std::endl;
						std::string response = client.EmulatedString(x[i]);
						std::cout << response << std::endl;
						Sleep(2000);
					}
					catch (system::system_error &e)
					{
						std::cout << e.code() << " " << e.what();
						//return e.code().value();
					}
				}
				else
				{
					int j = 0;
					while (j <= n)
					{
						try
						{
							ClientTcp client(ip, port);
							client.Connect();
							std::string elg;
							elg = mat[j];
							elg += "\n";
							std::string response = client.EmulatedString(elg.c_str());
							std::cout << "pachet " << j << std::endl;
							std::cout << response << std::endl;
						}
						catch (system::system_error &e)
						{
							std::cout << e.code() << " " << e.what();
						}
						j++;
					}
					ClientTcp client(ip, port);
					client.Connect();
					std::string response = client.EmulatedString("eof");
					std::cout << "pachet " << j << " " << response << std::endl;
				}
				i++;
			}
		}
		if (strcmp(choise, "interpetable") == 0)
		{
		std::cin >> buf;
		x[2] = _strdup(buf);
		DataReader::BinaryRead(buf, lenght);
		BinaryFormatterWhitChar(matrix, buf, lenght);
		while (i < 4)
		{
			if (i < 3)
			{
				try
				{
					ClientTcp client(ip, port);
					client.Connect();
					std::cout << x[i] << std::endl;
					std::string response = client.EmulatedString(x[i]);
					std::cout << response << std::endl;
					Sleep(2000);
				}
				catch (system::system_error &e)
				{
					std::cout << e.code() << " " << e.what();
					//return e.code().value();
				}
			}
			else
			{
				int ll = 0;
				int j = 0;
				for (j = 0;j < lenght;j++)
				{
					try
					{
						ClientTcp client(ip, port);
						client.Connect();
						std::string response = client.EmulatedString(matrix[j]);
						std::cout << "pachet " << j << " " << strlen(matrix[j]) << std::endl;
						std::cout << response << std::endl;
						ll = ll + strlen(matrix[j]);
						Sleep(500);
					}
					catch (system::system_error &e)
					{
						std::cout << e.code() << " " << e.what();
					}
				}
				ClientTcp client(ip, port);
				client.Connect();
				std::string response = client.EmulatedString("eof");
				std::cout << "pachet " << j << " " << response << std::endl;
			}
			i++;
		}
		}
	}
	//return 0;
	
}