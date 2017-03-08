#include "DataReader.h"



DataReader::DataReader()
{
}

char * DataReader::ReadFile(char * nume)
{
	std::stringstream first_buffer;
	std::ifstream file;
	std::string sec_buffer;
	char* final_buffer;
	file.exceptions(std::ifstream::badbit);
	try {
		file.open(nume,std::ios::binary);
		if (file.is_open())
		{
			first_buffer << file.rdbuf();
			sec_buffer = first_buffer.str();
			final_buffer = _strdup(sec_buffer.c_str());
			return final_buffer;
		}
	}
	catch (std::ifstream::failure e)
	{
		throw new std::string("eroare la citirea bazei de date");
	}
	
}

char ** DataReader::GetMatrix(char * sir,int &number)
{
	char **mat;
	std::string buffer;
	buffer = sir;
	int n = std::count(buffer.begin(), buffer.end(), '\n');
	number = n;
	mat = new char*[n];
	int pos = 0;
	char*token = strtok(sir, "\n");
	while (token != NULL)
	{
		mat[pos] = _strdup(token);
		pos++;
		token = strtok(NULL, "\n");
	}
	return mat;
}

void DataReader::BinaryRead(char * &sir, int & lenght)
{
	std::ifstream file;
	file.open(sir, std::istream::in | std::ios::binary);
	std::streampos fsize = 0;
	fsize = file.tellg();
	file.seekg(0, std::ios::end);
	fsize = file.tellg() - fsize;
	file.close();
	int n = fsize;
	file.open(sir, istream::in | ios::binary);
	std::cout << n << " " << endl;
	int z = 0;
	char *p = new  char[n + 1];
	for (int i = 0;i < n;i++)
	{
	char ch;
	file.get(ch);
	p[i] = ch;
	}
	file.close();
	delete sir;
	lenght = n;
	sir = p;
}


DataReader::~DataReader()
{
}
