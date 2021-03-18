#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <sstream> // to convert string of digits(number) to integer(number)
using namespace std;

// global constants
enum {numSize=11, Size=21};
char file[] = "contacts.bin";

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
	friend int add(tel_directory&);
	friend int del(const char*, const tel_directory&);
	int edit(const fstream&);
	friend int search(const char*, const tel_directory&);
	friend void view(const tel_directory&);
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

// operations
int add(tel_directory& obj) {
	fstream fs;
	// --------------- cin>>obj>>fs>>contacts.bin ------------------------
	fs.open(file, ios::app | ios::out);
	if (fs.fail()) {
		cerr << "Add-" << file << ": Unable to open" << endl;
	}
	else {
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
	}
	return 0;
}

int search(const char* key, const tel_directory& obj) {
	fstream fs;
	bool searchFlag = true;

	fs.open(file, ios::in);
	if (fs.fail()) {
		cerr << "View-" << file << ": Unable to open" << endl;
	}
	else {
		// calculating number of objects in file
		fs.seekg(0, ios::end);
		streamoff numOfObjectsInFile = fs.tellg();
		numOfObjectsInFile /= sizeof(obj);
		// resetting get ptr to the beginning of file for read operation
		fs.seekg(0, ios::beg);
		// searching
		bool flag;
		cout << endl;
		while (numOfObjectsInFile > 0) {
			fs.read((char*)&obj, sizeof(obj));
			flag = true;
			flag = strcmp(key, obj.name);
			if (flag == false) {
				cout << obj << endl;
				searchFlag = false;
			}
			numOfObjectsInFile--;
		}
		if (searchFlag == true)
			cout << "\"" << key << "\" not found\n";
		fs.close();
	}
	return 0;
}

int del(const char* key, const tel_directory& obj) {
	// searching for key
	streamoff loc = 0, numOfObjectsInFile = 0;
	fstream fs, cs;

	fs.open(file, ios::in | ios::out);
	if (fs.fail()) {
		cerr << "Del-" << file << ": Unable to open" << endl;
	}
	else {
		// calculating number of objects in file
		fs.seekg(0, ios::end);
		numOfObjectsInFile = fs.tellg();
		numOfObjectsInFile /= sizeof(obj);
		// resetting get ptr to the beginning of file for read operation
		fs.seekg(0, ios::beg);
		// searching
		bool flag = true;
		streamoff temp = numOfObjectsInFile;
		while (temp > 0) {
			fs.read((char*)&obj, sizeof(obj));
			flag = strcmp(key, obj.name);
			if (flag == false) {
				loc = fs.tellg();
				break;
			}
			temp--;
		}
	}
	fs.clear();
	// deletion
	if (loc != 0) {
		streamoff temp = loc;
		loc -= sizeof(obj);
		cs.open("./cnt.tmp", ios ::out);
		if (cs.fail()) {
			cerr << "del()CSout: Can't open the file" << endl;
		}
		else {
			streamoff temp1 = numOfObjectsInFile;
			// seeking to object next to object which is gonna be deleted
			temp1 -= (temp / sizeof(obj));
			fs.seekg(temp, ios::beg);
			// reading contacts after contact which is gonna be deleted
			// and writing them to a temporary file
			while (temp1 > 0) {
				fs.read((char*)&obj, sizeof(obj));
				cs.write((char*)&obj, sizeof(obj));
				temp1--;
			}
			fs.clear();
			cs.close();
			fs.seekp(loc, ios::beg);
			cs.open("./cnt.tmp", ios::in);
			if (cs.fail()) {
				cerr << "del()CSin: Can't open the file" << endl;
			}
			else {
				// write to contacts.bin from cnt.tmp
				temp1 = numOfObjectsInFile;
				temp1 -= (temp / sizeof(obj));
				while (temp1 > 0) {
					cs.read((char*)&obj, sizeof(obj));
					fs.write((char*)&obj, sizeof(obj));
					temp1--;
				}
			}
			cout << "\n\'" << key << "\' deleted." << endl;
			cs.close();
			fs.close();
		}
		// removing the last item that will be left making multiple
		// copies of it
		fs.open(file, ios::in | ios::out);
		if (fs.fail()) {
			cerr << "DelRem-" << file << ": Unable to open" << endl;
		}
		else {
			cs.open("cnt.tmp", ios::out | ios::in);
			if (cs.fail()) {
				cerr << "del()CSout2: Can't open the file" << endl;
			}
			else {
				temp = numOfObjectsInFile - 1;
				// copying each object leaving the last one
				while (temp > 0) {
					fs.read((char*)&obj, sizeof(obj));
					cs.write((char*)&obj, sizeof(obj));
					temp--;
				}
				cs.clear();
				fs.clear();
				fs.seekp(0, ios::beg);
				cs.seekg(0, ios::beg);
				temp = numOfObjectsInFile - 1;
				while (temp-- > 0) {
					cs.read((char*)&obj, sizeof(obj));
					fs.write((char*)&obj, sizeof(obj));
				}
				fs.close();
				cs.close();
				remove("cnt.tmp");
			}
		}
	}
	else
		cout << "\'" << key << "\' not found" << endl;
	return 0;
}

void view(const tel_directory& obj) {
	fstream fs;
	fs.open(file, ios::in);
	if (fs.fail()) {
		cerr << "View-" << file << ": Unable to open" << endl;
		return;
	}
	// calculating number of objects in file
	fs.seekg(0, ios::end);
	streamoff numOfObjectsInFile = fs.tellg();
	numOfObjectsInFile /= sizeof(obj);
	// resetting get ptr to the beginning of file for read operation
	fs.seekg(0, ios::beg);
	// reading
	cout << "\nContacts Console Application v0.1\n"
		<< "Total " << numOfObjectsInFile << " contacts\n\n  "
		<< setiosflags(ios::left) << setw(15) << "CONTACT_NAME" << resetiosflags(ios::left)
		<< setiosflags(ios::right) << setw(30) << "NUMBER" << resetiosflags(ios::right) << endl;
	while (numOfObjectsInFile > 0) {
		fs.read((char*)&obj, sizeof(obj));
		cout << obj << endl;
		numOfObjectsInFile--;
	}
	fs.close();
}

// driver
int main () {

	tel_directory obj;
	int choice = 0;
	char key[Size];

	// menu
	while (true) {
		cout << " Contacts Console Application v0.1\n -------------- MENU -------------\n\n";
		cout << " 1. Add\n 2. Delete\n 3. Edit\n 4. Search\n 5. View All\n 6. About\n 7. Exit\n" << endl;
		cout << "Choice: "; cin >> choice;
		system("cls");
		if (choice < 8 && choice > 0) {
			switch (choice)
			{
			case 1:
				add(obj);
				cout << endl;
				break;
			case 2:
				cout << "\nEnter key: "; cin >> key;
				del(key, obj);
				cout << endl;
				break;
			case 3:
				// edit();
				break;
			case 4:
				cout << "\nEnter key: "; cin >> key;
				search(key, obj);
				cout << endl;
				break;
			case 5:
				view(obj);
				cout << "\nPress any key..." << endl;
				cin.get();
				cin.get();
				system("cls");
				break;
			case 6:
				about();
				cout << "\nPress any key..." << endl;
				cin.get();
				cin.get();
				system("cls");
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