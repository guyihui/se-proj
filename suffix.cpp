#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<stack>
using namespace std;

int main() {
	int num = 0;
	cout << "input the number of letters of suffix:";
	cin >> num;

	ifstream infile;
	infile.open("EnglishWords.txt", ios::in);

	string word;
	map<string, int> suffixs;
	while (getline(infile, word)) {
		if (word.length()<num){}
		else {
			if (suffixs.find(word.substr(word.length() - num, num))==suffixs.end()) {
				suffixs[word.substr(word.length() - num, num)] = 1;
			}
			else {
				suffixs[word.substr(word.length() - num, num)] += 1;
			}
		}
	}

	double counter = 0;
	string suffix = "";
	for (int i = 0;i < 10;i++) {
		counter = 0;
		suffix="";
		for (map<string, int>::iterator itr = suffixs.begin();itr != suffixs.end();itr++) {
			if (itr->second > counter) {
				counter = itr->second;
				suffix = itr->first;
			}
		}
		suffixs[suffix] = 0;
		cout << i+1 << " " << suffix << " " << counter << endl;
	}
	infile.close();

	stack<string> suff;
	suffix = "";
	cin >> suffix;
	infile.open("EnglishWords.txt", ios::in);

	string word;
	while (getline(infile, word)) {
		if (word.substr(word.length() - num, num) == suffix) {
			suff.push(word);
		}
	}


	cin.get();
	cin.get();

	return 0;
}