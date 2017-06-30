#pragma once
#include<string>
#include<map>
#include<vector>
#include"order.h"
using namespace std;
class orderbook {
private:
	map<double, vector<order>> buybook;
	map<double, vector<order>> sellbook;
	map<double, int> book;
	double lowest;//lowest sell price
	double highest;//highest buy price
public:
	orderbook() :lowest(0), highest(0) {}
	void printAll();//print every order
	void print();//show the orderbook
	void updateGap();
	void add(order ord);//can't buy/sell at once
	string cancel(order ord);
	string fill(order &ord);
	
};
void orderbook::printAll() {
	map<double, vector<order>>::iterator itr;
	for (itr = buybook.begin();itr != buybook.end();itr++) {
		for (int i = 0;i < itr->second.size();i++) {
			itr->second[i].print();
			cout << endl;
		}
	}
	map<double, vector<order>>::iterator itrx;
	for (itrx = sellbook.begin();itrx != sellbook.end();itrx++) {
		for (int i = 0;i < itrx->second.size();i++) {
			itrx->second[i].print();
			cout << endl;
		}
	}
}
void orderbook::print() {
	book.clear();
	map<double, vector<order>>::iterator itr;
	for (itr = buybook.begin();itr != buybook.end();itr++) {
		if (book.count(itr->first))book[itr->first] = 0;
		for (int i = 0;i < itr->second.size();i++) {
			book[itr->first] += itr->second[i].getLeavesQty();
		}
	}
	map<double, vector<order>>::iterator itrx;
	for (itrx = sellbook.begin();itrx != sellbook.end();itrx++) {
		if (book.count(itrx->first))book[itrx->first] = 0;
		for (int i = 0;i < itrx->second.size();i++) {
			book[itrx->first] += itrx->second[i].getLeavesQty();
		}
	}
	//counted.
	map<double, int>::iterator bookitr;
	cout << endl << "buy:" << endl;
	for (bookitr = book.begin();bookitr != book.end();bookitr++) {
		if (bookitr->first == lowest)cout << "sell:" << endl;
		cout << bookitr->first << "  " << bookitr->second << endl;
	}
	cout << "highest:" << highest << endl << "lowest:" << lowest << endl << endl;
}
void orderbook::updateGap() {
	map<double, vector<order>>::iterator s_itr;
	if (sellbook.size()) {
		s_itr = sellbook.begin();
		lowest = s_itr->first;
	}
	else {
		lowest = 0;
	}
	map<double, vector<order>>::iterator b_itr = buybook.begin();
	if (buybook.size()) {
		for (int i = 0;i < buybook.size() - 1;i++) {
			b_itr++;
		}
		highest = b_itr->first;
	}
	else {
		highest = 0;
	}
}
void orderbook::add(order ord) {
	if (ord.getSide() == '1') {  // 54=1 buy
		if (!buybook.count(ord.getPrice())) {
			vector<order> vec;
			vec.push_back(ord);
			buybook[ord.getPrice()] = vec;
			if (ord.getPrice() > highest) {
				highest = ord.getPrice();
			}
		}
		else {
			buybook[ord.getPrice()].push_back(ord);
		}
	}
	else if(ord.getSide() == '2'){
		if (!sellbook.count(ord.getPrice())) {
			vector<order> vec;
			vec.push_back(ord);
			sellbook[ord.getPrice()] = vec;
			if (ord.getPrice() < lowest||lowest == 0) {
				lowest = ord.getPrice();
			}
		}
		else {
			sellbook[ord.getPrice()].push_back(ord);
		}
	}
}
string orderbook::cancel(order ord) {
	string id = ord.getOrigClOrdID();//tag41
	char side = ord.getSide();//tag54
	bool is_canceled = false;
	string back;
	if (side == '1') {
		map<double, vector<order>>::iterator itr;
		for (itr = buybook.begin();itr != buybook.end();itr++) {
			for (int i = 0;i < itr->second.size();i++) {
				if (itr->second[i].getClOrdID() == id) {
					itr->second[i].set("150", "4");
					itr->second[i].set("39", "4");
					back = itr->second[i].getmsg();
					vector<order>::iterator itrv = itr->second.begin() + i;
					itr->second.erase(itrv);
					is_canceled = true;
					break;
				}
			}
			if (itr->second.size() == 0)itr = buybook.erase(itr);
			if (is_canceled)break;
		}
	}
	else if (side == '2') {
		map<double, vector<order>>::iterator itr;
		for (itr = sellbook.begin();itr != sellbook.end();itr++) {
			for (int i = 0;i < itr->second.size();i++) {
				if (itr->second[i].getClOrdID() == id) {
					itr->second[i].set("150", "4");
					itr->second[i].set("39", "4");
					back = itr->second[i].getmsg();
					vector<order>::iterator itrv = itr->second.begin() + i;
					itr->second.erase(itrv);
					is_canceled = true;
					break;
				}
			}
			if (itr->second.size() == 0)itr = sellbook.erase(itr);
			if (is_canceled)break;
		}
	}
	updateGap();
	if (!is_canceled)back = "35=9;39=8;";
	return back;
}
string orderbook::fill(order &ord) {
	if (ord.getSide() == '1') {  // 54=1 buy
		if (ord.getPrice()<lowest || lowest == 0) {
			add(ord);
			return ord.getmsg();//cannot fill
		}
		else {
			while (ord.getPrice() >= lowest&&lowest!=0) {
				for (int i = 0;i < sellbook[lowest].size();i++) {
					if (!ord.getLeavesQty())return ord.getmsg();
					else {
						int sLeavesQty = sellbook[lowest][i].getLeavesQty();
						int bLeavesQty = ord.getLeavesQty();
						if (sLeavesQty <= bLeavesQty) {
							sellbook[lowest][i].fill(0, 0);
							ord.fill(sLeavesQty, lowest);
							vector<order>::iterator sitrv = sellbook[lowest].begin() + i;
							sitrv = sellbook[lowest].erase(sitrv);
							i--;
						}
						else {
							sellbook[lowest][i].fill(bLeavesQty, 0);
							ord.fill(0, lowest);
							return ord.getmsg();
						}
					}
				}
				if (sellbook.size()!=0&&sellbook[lowest].size() == 0) {
					map<double, vector<order>>::iterator s_itr = sellbook.begin();
					s_itr = sellbook.erase(s_itr);
					updateGap();
				}
			}
		}
	}
	else if (ord.getSide() == '2') {//54=2 sell
		if (ord.getPrice() > highest || highest == 0) {
			add(ord);
			return ord.getmsg();//cannot fill
		}
		else {
			while (ord.getPrice() <= highest) {
				for (int i = 0;i < buybook[highest].size();i++) {
					if (!ord.getLeavesQty())return ord.getmsg();
					else {
						int bLeavesQty = buybook[highest][i].getLeavesQty();
						int sLeavesQty = ord.getLeavesQty();
						if (bLeavesQty <= sLeavesQty) {
							buybook[highest][i].fill(0, 0);
							ord.fill(bLeavesQty, highest);
							vector<order>::iterator itrv = buybook[highest].begin()+i;
							itrv = buybook[highest].erase(itrv);
							i--;
						}
						else {
							buybook[highest][i].fill(sLeavesQty, 0);
							ord.fill(0, highest);
							return ord.getmsg();
						}
					}
				}
				if (buybook.size()!=0&&buybook[highest].size() == 0) {
					map<double, vector<order>>::iterator itr = buybook.begin();
					for (int i = 0;i < buybook.size() - 1;i++) {
						itr++;
					}
					itr = buybook.erase(itr);
					updateGap();
				}
			}
		}
	}
	if (ord.getLeavesQty()) {
		add(ord);
	}
	updateGap();
	return ord.getmsg();
}