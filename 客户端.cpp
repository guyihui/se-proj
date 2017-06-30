#include <array>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "order.h"
#include <sstream>
using boost::asio::ip::tcp;
using namespace std;
string getmsg() {
	string fixmsg("");
	string DorF;
	cout << "35=";
	cin >> DorF;
	fixmsg += "35=" + DorF + ";";
	string id;
	cout << "11=";
	cin >> id;
	fixmsg += "11=" + id + ";";
	string side;
	cout << "54=";
	cin >> side;
	fixmsg += "54=" + side + ";";
	if (DorF == "D") {
		string price;
		cout << "44=";
		cin >> price;
		fixmsg += "44=" + price + ";";
		string qty;
		cout << "38=";
		cin >> qty;
		fixmsg += "38=" + qty + ";";
	}
	else if (DorF == "F") {
		string oid;
		cout << "41=";
		cin >> oid;
		fixmsg += "41=" + oid + ";";
	}
	return fixmsg;
}
int main(int argc, char* argv[])
{
	vector<order> ords;
	while (true) {
		cin.get();
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("127.0.0.1", "1900");//------
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
		cout << logmsg << endl;

		string fixmsg = getmsg();
		order ord(fixmsg);
		boost::asio::write(socket, boost::asio::buffer(fixmsg), error);
		
		std::array<char, 256> input_bufferxx;
		std::size_t rsizexx = socket.read_some(boost::asio::buffer(input_bufferxx), error);
		if (error)
		{
			std::cout << boost::system::system_error(error).what() << std::endl;
			cin.get();
		}
		string msgxx(input_bufferxx.data(), input_bufferxx.data() + rsizexx);
		order ordback(msgxx);
		cout << msgxx << endl;
		if (ordback.getExecType() == '0' && ordback.getOrdStatus() == '0') {
			ords.push_back(ordback);//35=8;150=0;39=0;
			cout << "New Ack." << endl;
			
			std::array<char, 256> input_bufferx;
			std::size_t rsizex = socket.read_some(boost::asio::buffer(input_bufferx), error);
			if (error)
			{
				std::cout << boost::system::system_error(error).what() << std::endl;
				cin.get();
			}
			string msgx(input_bufferx.data(), input_bufferx.data() + rsizex);
			order ordx(msgx);
			cout << msgx << endl;
			ords.pop_back();
			ords.push_back(ordx);
			ordx.print();

		}
		else if (ordback.getExecType() == '4' && ordback.getOrdStatus() == '4') {
			string id = ordback.getOrigClOrdID();
			for (int i = 0;i < ords.size();i++) {
				if (ords[i].getClOrdID() == id) {
					ords[i].set("150", "4");
					ords[i].set("39", "4");
					break;
				}
			}
			cout << "canceled." << endl;
		}
		else if (ordback.getmsgtype() == "9"&&ordback.getOrdStatus() == '8') {
			cout << "cancellation rejected." << endl;
		}
	}
	return 0;
}