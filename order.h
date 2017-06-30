#pragma once
#include<iostream>
#include<string>
#include<sstream>
using namespace std;
class order {
private:
	string fixmsg;
	//根据tag增加成员
	string msgtype;      //35
	string ClOrdID;      //11  Unique identifier for Order
	string OrigClOrdID;  //41
	char ExecType;       //150
	char OrdStatus;      //39  当前状态
	char OrdType;        //40  ---------??????  X
	double Price;        //44  share单价
	double LastPx;       //31  上一次交易价格
	int LastShares;      //32  上一次交易数量
	int LeavesQty;       //151 可交易数量
	double AvgPx;        //6   平均交易价格
	int CumQty;          //14  已交易数量
	char Side;           //54  买/卖
	int OrderQty;        //38  总share数
	string Symbol;       //55  Ticker symbol证券识别码  X
public:
	//order();
	order(string msg):
		ExecType('0'),OrdStatus('0'),
		LastPx(0),LastShares(0),LeavesQty(0),AvgPx(0),CumQty(0)//-----------
	{
		fixmsg = msg;
		generate();
	}
	
	string getfixmsg()const { return fixmsg; }
	string getmsgtype()const { return msgtype; }
	string getClOrdID()const { return ClOrdID; }
	string getOrigClOrdID()const { return OrigClOrdID; }
	char getExecType()const { return ExecType; }
	char getOrdStatus()const { return OrdStatus; }
	char getOrdType()const { return OrdType; }
	double getPrice()const { return Price; }
	double getLastPx()const { return LastPx; }
	int getLastShares()const { return LastShares; }
	int getLeavesQty()const { return LeavesQty; }
	double getAvgPx()const { return AvgPx; }
	int getCumQty()const { return CumQty; }
	char getSide()const { return Side; }
	int getOrderQty()const { return OrderQty; }
	string getSymbol()const { return Symbol; }
	
	void generate();//set other tags according to fixmsg
	void set(string stag, string value);//set a single tag
	void print();
	void fill(int qty,double px);//(0,0)=(LeavesQty,Price)//------------------------
	string check();
	string getmsg();
	
	order& operator=(order &ord) {
		order ordx(ord.getmsg());
		return ordx;
	}
};
void order::generate() {
	stringstream ss(fixmsg);
	while (ss) {
		string tag;
		string value;
		getline(ss, tag, '=');
		getline(ss, value, ';');
		set(tag, value);
	}
	LeavesQty = OrderQty;
}
string order::getmsg() {
	stringstream ss("");
	ss << "35=" << getmsgtype() << ";";
	ss << "11=" << getClOrdID() << ";";
	if (getmsgtype() == "F")ss << "41=" << getOrigClOrdID() << ";";
	ss << "150=" << getExecType() << ";";
	ss << "39=" << getOrdStatus() << ";";
	ss << "44=" << getPrice() << ";";
	ss << "31=" << getLastPx() << ";";
	ss << "32=" << getLastShares() << ";";
	ss << "151=" << getLeavesQty() << ";";
	ss << "6=" << getAvgPx() << ";";
	ss << "14=" << getCumQty() << ";";
	ss << "54=" << getSide() << ";";
	ss << "38=" << getOrderQty() << ";";
	return ss.str();
}
void order::set(string stag, string value) {
	stringstream s;
	int tag = 0;
	s.str(stag);
	s >> tag;
	s.clear();
	if (tag == 35)msgtype = value;
	else if (tag == 11)ClOrdID = value;
	else if (tag == 41)OrigClOrdID = value;
	else if (tag == 150) {
		char val;
		val =value[0];
		ExecType = val;
	}
	else if (tag == 39) {
		char val;
		val = value[0];
		OrdStatus = val;
	}
	else if (tag == 44) {
		s.str(value);
		double val;
		s >> val;
		Price = val;
	}
	else if (tag == 31) {
		s.str(value);
		double val;
		s >> val;
		LastPx = val;
	}
	else if (tag == 32) {
		s.str(value);
		int val;
		s >> val;
		LastShares = val;
	}
	else if (tag == 151) {
		s.str(value);
		int val;
		s >> val;
		LeavesQty = val;
	}
	else if (tag == 6) {
		s.str(value);
		double val;
		s >> val;
		AvgPx = val;
	}
	else if (tag == 14) {
		s.str(value);
		int val;
		s >> val;
		CumQty = val;
	}
	else if (tag == 54) {
		char val;
		val = value[0];
		Side = val;
	}
	else if (tag == 38) {
		s.str(value);
		int val;
		s >> val;
		OrderQty = val;
	}/*
	else if (tag == 55)Symbol = value;
	else if (tag == 40) {
	char val;
	val = value[0];
	OrdType = val;
	}*/
}
void order::print() {
	cout << "fixmsg:" << fixmsg << endl;
	cout << "msgtype:" << msgtype << endl;
	cout << "ClOrdID:" << ClOrdID << endl;
	cout << "OrigClOrdID:" << OrigClOrdID << endl;
	cout << "ExecType:" << ExecType << endl;
	cout << "OrdStatus:" << OrdStatus << endl;
	//cout << "OrdType:" << OrdType << endl;
	cout << "Price:" << Price << endl;
	cout << "LastPx:" << LastPx << endl;
	cout << "LastShares:" << LastShares << endl;
	cout << "LeavesQty:" << LeavesQty << endl;
	cout << "AvgPx:" << AvgPx << endl;
	cout << "CumQty:" << CumQty << endl;
	cout << "Side:" << Side << endl;
	cout << "OrderQty:" << OrderQty << endl;
	//cout << "Symbol:" << Symbol << endl;
}
void order::fill(int qty,double px) {
	if (qty == 0)qty = LeavesQty;
	if (px == 0)px = Price;
	LastShares = qty;
	LastPx = px;
	AvgPx = (AvgPx*CumQty + qty*px) / (CumQty + qty);
	LeavesQty -= qty;
	CumQty += qty;
	if (LeavesQty == 0) {
		ExecType = '2';
		OrdStatus = '2';
	}
	else {
		ExecType = '1';
		OrdStatus = '1';
	}
}
string order::check() {
	return "35=8;150=0;39=0;";
}