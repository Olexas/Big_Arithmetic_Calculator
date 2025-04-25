#include "Table.h"


Table::Table(map<string, map<string, string>>& table, string table_name) {
	this->table = move(table);
	this->name = table_name;

	for (const auto& [key, row] : this->table) {
		arithmetic_alphabet.push_back(key);
	}
}

const map<string, map<string, string>>& Table::getTable() const{
	return this->table;
}

const string Table::getElementFromTable(string element1, string element2) const {
	return this->table.at(element2).at(element1);
}

const string Table::getElementFromTable(char element1, string element2) const {
	return this->table.at(element2).at({ element1 });
}

const string Table::getElementFromTable(char element1, char element2) const {
	return this->table.at({ element2 }).at({ element1 });
}

ostream& operator<<(ostream& out, const Table& table) {
	out << table.name << "||";
	for (auto& el : table.arithmetic_alphabet) {
		out << " " << el << " |";
	}

	table.PrintLowerTableLine();
	
	for (const auto& [key1, row] : table.table) {
		out << "  " << key1 << "  ||";
		for (const auto& [key2, coloumn_elemnet] : row) {
			out << " " << coloumn_elemnet << " |";
		}
		table.PrintTableLine();
	}

	return out;
}

void Table::PrintTableLine(bool flag) const{
	cout << endl << "-------";
	for (auto& el : this->arithmetic_alphabet) {
		cout << "----";
	}
	if (flag) cout << endl;
}

void Table::PrintLowerTableLine(bool flag) const {
	cout << endl << "=======";
	for (auto& el : this->arithmetic_alphabet) {
		cout << "====";
	}
	if (flag) cout << endl;
}
