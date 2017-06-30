#include <iostream>
#include <string>
#include <map>
#include <queue>
#include "order.h"
#include "orderbook.h"
#include <boost\asio.hpp>
using namespace std;
using namespace boost::asio;
int main(int argc, char* argv[]) {
	orderbook A;
	
	io_service iosev;
	ip::tcp::acceptor acceptor(iosev,ip::tcp::endpoint(ip::tcp::v4(), 1900));
	
	while (true) {
		// socket对象
		ip::tcp::socket socket(iosev);
		// 等待直到客户端连接进来
		acceptor.accept(socket);
		// 显示连接进来的客户端
		cout << socket.remote_endpoint().address() << endl;
		// 向客户端发送hello world!
		boost::system::error_code ec;
		socket.write_some(buffer("hello world!"), ec);
		// 如果出错，打印出错信息
		if (ec)
		{
			cout << boost::system::system_error(ec).what() << endl;
			break;
		}
		
		//while (true) {
			std::array<char, 256> input_buffer;
			std::size_t rsize = socket.read_some(boost::asio::buffer(input_buffer), ec);
			string fixmsg(input_buffer.data(), input_buffer.data() + rsize);
			string backmsg;

			//main process
			order ord(fixmsg);

			if (ord.getmsgtype() == "F") backmsg = A.cancel(ord);
			else if (ord.getmsgtype() == "D") {
				socket.write_some(buffer(ord.check()), ec);
				if (ord.check() == "35=8;150=0;39=0;") {
					backmsg = A.fill(ord);
				}
			}
			socket.write_some(buffer(backmsg), ec);
			if (ec)
			{
				cout << boost::system::system_error(ec).what() << endl;
				break;
			}
			A.print();
		//}	
	}
	return 0;
}