#include <array>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "order.h"
#include "orderbook.h"
#include <sstream>
using namespace boost::asio;
using namespace std;
using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
	orderbook A;

	cin.get();
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("127.0.0.1", "63159");//------
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	boost::system::error_code error;

	std::array<char, 256> input_buffer;
	std::size_t rsize = socket.read_some(boost::asio::buffer(input_buffer), error);
	if (error)
	{
		std::cout << boost::system::system_error(error).what() << std::endl;
		cin.get();
	}
	string logmsg(input_buffer.data(), input_buffer.data() + rsize);

	//main process
	while (true) {
		std::array<char, 256> input_bufferx;
		std::size_t rsizex = socket.read_some(boost::asio::buffer(input_bufferx), error);
		if (error)
		{
			std::cout << boost::system::system_error(error).what() << std::endl;
			cin.get();
		}
		string fixmsg(input_bufferx.data(), input_bufferx.data() + rsizex);

		order ord(fixmsg);

		if (ord.getmsgtype() == "F") A.cancel(ord);
		else if (ord.getmsgtype() == "D") {
			if (ord.check() == "35=8;150=0;39=0;") {
				A.fill(ord);
			}
		}
		A.print();
	}
	
	return 0;
}