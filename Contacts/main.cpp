#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
using namespace std;

// global enumeration constant
enum {numSize=11, Size=21};

// blueprint for telephone directory
class tel_directory {
private:
	char name[Size];
	char number[numSize];
public:
	// ctors
	tel_directory() {
		name[0] = '\0';
		number[0] = '\0';
	}
	tel_directory(const char*, const char*);
	tel_directory (const tel_directory& obj) : tel_directory(obj.name, obj.number) {}
	// dtor
	~tel_directory() {}
	// overloaded operators
	friend ostream& operator<<(ostream&, const tel_directory&);
	friend istream& operator>>(istream&, tel_directory&);

};

// constructor
tel_directory::tel_directory(const char* Name, const char* num) : tel_directory() {
	if (Name != NULL && num != NULL) {
		int lenName = strlen(Name);
		int lenNum = strlen(num);
		(lenName < Size) ? strcpy_s(name, lenName+1, Name) : 0;
		(lenNum == 10) ? strcpy_s(number, lenNum+1, num) : 0;
	}
}

// overloaded operators
ostream& operator<<(ostream& out, const tel_directory& obj) {
	out << "  " << setiosflags(ios::left) << setw(15) << obj.name << resetiosflags(ios::left);
	out << setiosflags(ios::right) << setw(30) << obj.number << resetiosflags(ios::right);
	return out;
}

istream& operator>>(istream& in, tel_directory& obj) {
	cout << "Input name: "; in >> obj.name;
	cout << "Input number: "; in >> obj.number;
	return in;
}
// driver
int main () {

	fstream fs;
	tel_directory obj;

	// --------------- cin>>obj>>fs>>contacts.txt ------------------------
	fs.open("contacts.txt", ios::app | ios::out );
	if (fs.fail()) {
		cerr << "1-contacts.txt: Unable to open" << endl;
	}
	// writing
	while (true) {
		cin >> obj;
		fs.write((char*)&obj, sizeof(obj));
		cin.get(); // eating \n character
		char ch;
		cout << "Continue(y/n): "; cin.get(ch);
		if (ch == 'n' || ch == 'N')
			break;
		fflush(stdin); // flushing standard input buffer
	}
	fs.close();
	// -------------- contacts.txt>>fs>>obj>>cout ------------------------
	fs.open("./contacts.txt", ios::in);
	if (fs.fail()) {
		cerr << "2-contacts.txt: Unable to open" << endl;
	}
	// calculating number of objects in file
	fs.seekg(0, ios::end);
	int numOfObjectsInFile = fs.tellg();
	numOfObjectsInFile /= sizeof(obj);
	// resetting get ptr to the beginning of file for read operation
	fs.seekg(0, ios::beg);
	// reading
	while (numOfObjectsInFile > 0) {
		fs.read((char*)&obj, sizeof(obj));
		cout << obj << endl;
		numOfObjectsInFile--;
	}
	fs.close();
	return 0;
}