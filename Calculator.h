#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include "Table.h"

using namespace std;

class Calculator {
private:
	vector<string> current_arithmetic;

	Table* summ_table;
	Table* summ_trans_table;

	Table* diff_table;
	Table* diff_trans_table;

	Table* compos_table;
	Table* compos_trans_table;

	void GenerateSummTable();
	void GenerateDiffTable();
	void GenerateComposTable();

public:
	Calculator(vector<string> arithmetic_vector);
	~Calculator();

	bool LessComparison(string first_num, string second_num);
	bool MoreComparison(string first_num, string second_num);

	string Summ(string first_num, string second_num);
	string Diff(string first_num, string second_num);
	string Compos(string first_num, string second_num); 
	vector<string> Div(string first_num, string second_num);

	const vector<string>& getCurrentArithmetic() const;

	friend ostream& operator<< (ostream& out, const Calculator& calc);
};