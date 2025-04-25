#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Table {
private:
	map<string, map<string, string>> table;
	string name;

	vector<string> arithmetic_alphabet;

public:
	Table(map<string, map<string, string>>& table, string table_name);
	const map<string, map<string, string>>& getTable() const;
	const string getElementFromTable(string element1, string element2) const;
	const string getElementFromTable(char element1, string element2) const;
	const string getElementFromTable(char element1, char element2) const;

	friend ostream& operator<<(ostream& out, const Table& table);
	void PrintTableLine(bool flag = true) const;
	void PrintLowerTableLine(bool flag = true) const;
};

