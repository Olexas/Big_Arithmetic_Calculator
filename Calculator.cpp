#include "Calculator.h"

Calculator::Calculator(vector<string> arithmetic_vector) {
	this->current_arithmetic = arithmetic_vector;

	GenerateSummTable();
	GenerateDiffTable();
	GenerateComposTable();
}

Calculator::~Calculator() {
	delete summ_table;
	delete summ_trans_table;
	delete diff_table;
	delete diff_trans_table;
	delete this->compos_table;
	delete this->compos_trans_table;
}

bool Calculator::LessComparison(string first_num, string second_num) {
	if (first_num.size() < second_num.size()) return true;
	if (first_num.size() > second_num.size()) return false;

	for (int symb_ind = 0; symb_ind < first_num.size(); symb_ind++) {
		string ch = { first_num[symb_ind] };
		string ch2 = { second_num[symb_ind] };
		auto ind1 = distance(this->current_arithmetic.begin(), find(this->current_arithmetic.begin(), this->current_arithmetic.end(), ch));
		auto ind2 = distance(this->current_arithmetic.begin(), find(this->current_arithmetic.begin(), this->current_arithmetic.end(), ch2));

		if (ind1 < ind2) return true;

		if (ind1 > ind2) return false;
	}

	return false;
}

bool Calculator::MoreComparison(string first_num, string second_num) {
	if (first_num.size() > second_num.size()) return true;
	if (first_num.size() < second_num.size()) return false;

	for (int symb_ind = 0; symb_ind < first_num.size(); symb_ind++) {
		string ch = { first_num[symb_ind] };
		string ch2 = { second_num[symb_ind] };
		auto ind1 = distance(this->current_arithmetic.begin(), find(this->current_arithmetic.begin(), this->current_arithmetic.end(), ch));
		auto ind2 = distance(this->current_arithmetic.begin(), find(this->current_arithmetic.begin(), this->current_arithmetic.end(), ch2));

		if (ind1 > ind2) return true;

		if (ind1 < ind2) return false;
	}

	return false;
}

void Calculator::GenerateSummTable() {
	map<string, map<string, string>> temp_map;
	map<string, map<string, string>> temp_trans_map;

	for (size_t i = 0; i != this->current_arithmetic.size(); ++i) {
		for (size_t j = i; j != this->current_arithmetic.size(); ++j) {
			string result = this->current_arithmetic[(i + j) % this->current_arithmetic.size()];
			string element1 = this->current_arithmetic[i]; 
			string element2 = this->current_arithmetic[j];

			temp_map[element1][element2] = result;
			temp_map[element2][element1] = result;

			bool is_trans = i + j >= this->current_arithmetic.size();
			temp_trans_map[element1][element2] = is_trans ? current_arithmetic[1] : current_arithmetic[0];
			temp_trans_map[element2][element1] = is_trans ? current_arithmetic[1] : current_arithmetic[0];
		}
	}

	this->summ_table = new Table(temp_map, "  +  ");
	this->summ_trans_table = new Table(temp_trans_map, "  +1 ");
}

string Calculator::Summ(string first_num, string second_num) {
	bool is_first_num_minus = first_num[0] == '-';
	bool is_second_num_minus = second_num[0] == '-';
	bool minus_flag = false;

	if (is_first_num_minus && is_second_num_minus) {
		minus_flag = true;
		first_num.erase(0, 1);
		second_num.erase(0, 1);
	}
	else if (is_first_num_minus) {
		first_num.erase(0, 1);
		return this->Diff(second_num, first_num);
	}
	else if (is_second_num_minus) {
		second_num.erase(0, 1);
		return this->Diff(first_num, second_num);
	}

	int max_num_size;
	if (first_num.size() < second_num.size()) {
		max_num_size = second_num.size();
		int first_num_size = first_num.size();
		for (int i = 0; i < max_num_size - first_num_size; i++) {
			first_num = this->current_arithmetic[0] + first_num;
		}
	}

	else if (first_num.size() != second_num.size()) {
		max_num_size = first_num.size();
		int second_num_size = second_num.size();
		for (int i = 0; i < max_num_size - second_num_size; i++) {
			second_num = this->current_arithmetic[0] + second_num;
		}
	}

	else max_num_size = first_num.size();

	string result;
	string trans = this->current_arithmetic[0]; string trans1;

	for (int symb_ind = max_num_size - 1; symb_ind >= 0; symb_ind--) {
		string summ_without_trans = this->summ_table->getElementFromTable(first_num[symb_ind], second_num[symb_ind]);
		result = this->summ_table->getElementFromTable(summ_without_trans, trans) + result;

		trans1 = this->summ_trans_table->getElementFromTable(summ_without_trans, trans);
		trans = this->summ_trans_table->getElementFromTable(first_num[symb_ind], second_num[symb_ind]);

		if (trans1 == current_arithmetic[1]) trans = current_arithmetic[1];
	}

	if (trans != this->current_arithmetic[0]) result = trans + result;
	if (minus_flag) result = "-" + result;

	return result;
}

void Calculator::GenerateDiffTable() {
	map<string, map<string, string>> temp_map;
	map<string, map<string, string>> temp_trans_map;

	for (const auto& [key1, row] : this->summ_table->getTable()) {
		for (const auto& [key2, coloumn_elemnet] : row) {
			temp_map[key1][coloumn_elemnet] = key2;
			temp_trans_map[key1][coloumn_elemnet] = this->summ_trans_table->getElementFromTable(key2, key1);
		}
	}

	this->diff_table = new Table(temp_map, "  -  ");
	this->diff_trans_table = new Table(temp_trans_map, "  -1 ");
}

string Calculator::Diff(string first_num, string second_num) {
	int max_num_size;
	bool minus_flag = false;

	bool is_first_num_minus = first_num[0] == '-';
	bool is_second_num_minus = second_num[0] == '-';

	if (is_first_num_minus && is_second_num_minus) {
		first_num.erase(0, 1);
		second_num.erase(0, 1);
		string tmp = first_num;
		first_num = second_num;
		second_num = tmp;
	}
	else if (is_first_num_minus) {
		first_num.erase(0, 1);
		minus_flag = true;
		return "-" + this->Summ(second_num, first_num);
	}
	else if (is_second_num_minus) {
		second_num.erase(0, 1);
		return this->Summ(first_num, second_num);
	}

	if (LessComparison(first_num, second_num)) {
		max_num_size = second_num.size();
		int first_num_size = first_num.size();
		for (int i = 0; i < max_num_size - first_num_size; i++) {
			first_num = this->current_arithmetic[0] + first_num;
		}
		string tmp = first_num;
		first_num = second_num;
		second_num = tmp;
		minus_flag = true;
	}

	else if (first_num.size() != second_num.size()) {
		max_num_size = first_num.size();
		int second_num_size = second_num.size();
		for (int i = 0; i < max_num_size - second_num_size; i++) {
			second_num = this->current_arithmetic[0] + second_num;
		}
	}

	else max_num_size = first_num.size();

	string result;
	string trans = this->current_arithmetic[0]; string trans1;

	for (int symb_ind = max_num_size - 1; symb_ind >= 0; symb_ind--) {
		string diff_without_trans = this->diff_table->getElementFromTable(first_num[symb_ind], second_num[symb_ind]);
		result = this->diff_table->getElementFromTable(diff_without_trans, trans) + result;

		trans1 = this->diff_trans_table->getElementFromTable(diff_without_trans, trans);
		trans = this->diff_trans_table->getElementFromTable(first_num[symb_ind], second_num[symb_ind]);

		if (trans1 == current_arithmetic[1]) trans = current_arithmetic[1];
	}

	if (trans != this->current_arithmetic[0]) result = trans + result;
	result.erase(0, result.find_first_not_of(current_arithmetic[0]));

	if (result.size() == 0) result = current_arithmetic[0];
	else if (minus_flag) result = "-" + result;

	return result;
}

void Calculator::GenerateComposTable() {
	map<string, map<string, string>> temp_map;
	map<string, map<string, string>> temp_trans_map;

	for (size_t i = 0; i != this->current_arithmetic.size(); ++i) {
		for (size_t j = i; j != this->current_arithmetic.size(); ++j) {
			string result = this->current_arithmetic[(i * j) % this->current_arithmetic.size()];
			string element1 = this->current_arithmetic[i];
			string element2 = this->current_arithmetic[j];

			temp_map[element1][element2] = result;
			temp_map[element2][element1] = result;

			bool is_trans = i * j >= this->current_arithmetic.size();
			temp_trans_map[element1][element2] = is_trans ? current_arithmetic[i * j / this->current_arithmetic.size()] : current_arithmetic[0];
			temp_trans_map[element2][element1] = is_trans ? current_arithmetic[i * j / this->current_arithmetic.size()] : current_arithmetic[0];
		}
	}

	this->compos_table = new Table(temp_map, "  *  ");
	this->compos_trans_table = new Table(temp_trans_map, "  *1 ");
}

string Calculator::Compos(string first_num, string second_num) {
	if (first_num == this->current_arithmetic[0] || second_num == this->current_arithmetic[0]) return this->current_arithmetic[0];

	bool is_first_num_minus = first_num[0] == '-';
	bool is_second_num_minus = second_num[0] == '-';
	bool minus_flag = false;
	
	if (is_first_num_minus && is_second_num_minus) {
		first_num.erase(0, 1);
		second_num.erase(0, 1);
	}
	else if (is_first_num_minus) {
		first_num.erase(0, 1);
		minus_flag = true;
	}
	else if (is_second_num_minus) {
		second_num.erase(0, 1);
		minus_flag = true;
	}

	string result = this->current_arithmetic[0];
	string trans; string trans1;

	for (int symb_ind2 = second_num.size() - 1; symb_ind2 >= 0; symb_ind2--) {
		trans = this->current_arithmetic[0];
		
		string pod_result;
		for (int i = 0; i < second_num.size() - 1 - symb_ind2; i++) {
			pod_result += current_arithmetic[0];
		}
		
		for (int symb_ind1 = first_num.size() - 1; symb_ind1 >= 0; symb_ind1--) {
			string compos_without_trans = this->compos_table->getElementFromTable(first_num[symb_ind1], second_num[symb_ind2]);
			pod_result = this->summ_table->getElementFromTable(compos_without_trans, trans) + pod_result;

			trans1 = this->summ_trans_table->getElementFromTable(compos_without_trans, trans);
			trans = this->compos_trans_table->getElementFromTable(first_num[symb_ind1], second_num[symb_ind2]);

			trans = this->Summ(trans, trans1);
		}
		if (trans != this->current_arithmetic[0]) pod_result = trans + pod_result;

		result = this->Summ(result, pod_result);
	}

	if (minus_flag) result = "-" + result;

	return result;
}

vector<string> Calculator::Div(string first_num, string second_num) {
	bool is_first_num_minus = first_num[0] == '-';
	bool is_second_num_minus = second_num[0] == '-';
	bool minus_flag = false;
	bool dop_minus_flag = false;

	if (is_first_num_minus && is_second_num_minus) {
		dop_minus_flag = true;
		first_num.erase(0, 1);
		second_num.erase(0, 1);
	}
	else if (is_first_num_minus) {
		first_num.erase(0, 1);
		dop_minus_flag = true;
		minus_flag = true;
	}
	else if (is_second_num_minus) {
		second_num.erase(0, 1);
		minus_flag = true;
	}

	string temp_second_num = second_num;
	string multiplier = this->current_arithmetic[1];
	while (LessComparison(temp_second_num, first_num)) {
		temp_second_num = Summ(temp_second_num, second_num);
		multiplier = Summ(multiplier, this->current_arithmetic[1]);
	}

	if (multiplier == this->current_arithmetic[1] && !dop_minus_flag) minus_flag;

	if (MoreComparison(temp_second_num, first_num)) {
		string ans;
		string trans;
		if (dop_minus_flag) {
			ans = multiplier;
			trans = Diff(Compos(multiplier, second_num), first_num);
		}
		else {
			ans = Diff(multiplier, this->current_arithmetic[1]);
			trans = Diff(first_num, Diff(temp_second_num, second_num));
		}
		return minus_flag ? vector<string>({ "-" + ans, trans }) : vector<string>({ ans, trans });
		//return Diff(multiplier, this->current_arithmetic[1]);
	}
	else return minus_flag ? vector<string>({ "-" + multiplier, this->current_arithmetic[0] }) : vector<string>({ multiplier, this->current_arithmetic[0] });
}

const vector<string>& Calculator::getCurrentArithmetic() const{
	return this->current_arithmetic;
}

ostream& operator<<(ostream& out, const Calculator& calc) {
	out << *calc.summ_table << endl;
	out << *calc.summ_trans_table << endl;
	out << *calc.diff_table << endl;
	out << *calc.diff_trans_table << endl;
	out << *calc.compos_table << endl;
	out << *calc.compos_trans_table << endl;

	return out;
}
