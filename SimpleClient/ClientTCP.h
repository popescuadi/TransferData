#pragma once
# include <iostream>
#include <boost\asio.hpp>
using namespace boost;

class ClientTcp
{
private:
	asio::io_service m_ios;
	asio::ip::tcp::endpoint end_point;
	asio::ip::tcp::socket sock;
public:
	ClientTcp(const std::string &raw_ip_adress, unsigned short port_number) :end_point(asio::ip::address::from_string(raw_ip_adress), port_number), sock(m_ios)
	{
		sock.open(end_point.protocol());
	}
public:
	void Connect()
	{
		sock.connect(end_point);
	}
	void Disconnect()
	{
		sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		sock.close();
	}
	std::string EmulatedString(std::string message)
	{
		std::string x = message + "\n";
		SendRequest(x);
		return ReceiveResponse();
	}
private:
	void SendRequest(const std::string request)
	{
		asio::write(sock, asio::buffer(request));
	}
	std::string ReceiveResponse()
	{
		asio::streambuf buf;
		asio::read_until(sock, buf, '\n');
		std::istream is(&buf);
		std::string response;
		response = response + " Asta e raspunsul::Mere!";
		std::getline(is, response);
		return response;
	}
};
