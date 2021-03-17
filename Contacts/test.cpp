/*
#include <iostream>
#include <cstring>
using namespace std;

int main()
{
	char* name = new char[20];
	cin >> name;
	cout << name;
	int len = strlen(name);
	char* another_name = new char[len+1];
	strcpy_s(another_name, len+1, name);
	cout << another_name;
	return 0;
}
*/