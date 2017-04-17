#include<fstream>
#include<string>
using namespace std;
int main() {
	ifstream infile;
	infile.open("read.txt", ios::in);
	ofstream fout("copy.txt", ios::app);
	string str;
	while (getline(infile, str)) {
		fout << str << endl;
	}
	return 0;
}