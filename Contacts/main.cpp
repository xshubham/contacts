#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <sstream> // to convert string of digits(number) to integer(number)
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
	// operations
	int add(const fstream&);
	int del(const fstream&);
	int edit(const fstream&);
	int search(const fstream&);
	void view(const fstream&);
};

// about application
void about(void);

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
	unsigned int x = 0;
	while (true) {
		cout << "Input name: "; in >> obj.name;
		cout << "Input number: "; in >> obj.number;
		stringstream convert(obj.number); // converting string to integer
		convert >> x;
		if (x != 0) {
			break;
		}
		else {
			cerr << "Invalid number entered.\nTry again...\n" << endl;
		}
	}
	return in;
}

// driver
int main () {

	fstream fs;
	tel_directory obj;
	int choice = 0;

	// menu
	while (true) {
		cout << "1. Add\n2. Delete\n3. Edit\n4. Search\n5. View All\n6. About\n7. Exit" << endl;
		cin >> choice;
		if (choice < 8 && choice > 0) {
			switch (choice)
			{
			case 1:
				// add();
				break;
			case 2:
				// del();
				break;
			case 3:
				// edit();
				break;
			case 4:
				// search();
				break;
			case 5:
				// view();
				break;
			case 6:
				about();
				break;
			case 7:
				cout << "\nGood Bye!!!" << endl;
				return 0;
			default:
				break;
			}
		}
	}
	return 0;
}

// about application
void about(void) {
	cout << "\nContacts Console Application v0.1\nDeveloped By Shubham Kumar\n"
		<< "For any suggestion, e-mail me on \"shubhamxst@gmail.com\"\n"
		<< "If you want to contribute to this project, you can fork the repository"
		<< " from https://github.com/xshubham/contacts.git" << endl;
	return;
}