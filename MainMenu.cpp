#include <windows.h>
#include "MainMenu.h"

void Screen::Clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

vector<string> Screen::splitstr(string str, string deli)
{
	vector<string> ale;
	int start = 0;
	int end = str.find(deli);
	while (end != -1) {
		ale.push_back(str.substr(start, end - start));
		start = end + deli.size();
		end = str.find(deli, start);
	}
	ale.push_back(str.substr(start, end - start));

	return ale;
}

void Screen::ProgramStart() {
	settings::programShouldEnd = false;

	cout << "Необходимо задать отношение порядка в формате (a-b-c-d-e-f-g...): ";
	string error_message = "\nВы задали некорректное отношение\nПопробуйте еще раз: ";

	vector<string> current_arithmetic;

	bool input_flag = true;
	while (input_flag) {
		current_arithmetic.clear();

		int buffer_size = 64;
		string entered_arithmetic = StrInput(buffer_size);
		
		for (int symb_ind = 0; symb_ind < entered_arithmetic.size(); symb_ind++) {
			if (symb_ind % 2 == 0 && entered_arithmetic[symb_ind] != '-') {
				current_arithmetic.push_back({ entered_arithmetic[symb_ind] });
				input_flag = false;
			}
			else if (symb_ind % 2 == 0 && entered_arithmetic[symb_ind] == '-') {
				cout << error_message;
				input_flag = true;
				break;
			}
			if (symb_ind % 2 != 0 && entered_arithmetic[symb_ind] != '-') {
				input_flag = true;
				cout << error_message;
				break;
			}
		}
	}

	this->calculator = new Calculator(current_arithmetic);
	MainMenu();
}

void Screen::MainMenu() {
	if (settings::programShouldEnd) return;
	Clear();

	cout << *calculator;

	bool global_flag = true;

	while (global_flag) {
		cout << endl << "Введите выражение (возможные действия: +,-,*,/): ";

		int buffer_size = 128;
		string error_message = "\nВы ввели некорректное выражение\nПопробуйте еще раз: ";

		bool input_flag = true;
		while (input_flag) {
			string entered_arithmetic = StrInput(buffer_size);
			vector<string> elements = splitstr(entered_arithmetic);

			if (elements.size() == 1 && elements[0] == "x") {
				global_flag = false;
				settings::programShouldEnd = true;
				break;
			}

			if (elements.size() != 3) {
				cout << error_message;
				continue;
			}

			elements[0].erase(0, elements[0].find_first_not_of(this->calculator->getCurrentArithmetic()[0]));
			elements[2].erase(0, elements[2].find_first_not_of(this->calculator->getCurrentArithmetic()[0]));

			if (elements[0].size() == 0) elements[0] = this->calculator->getCurrentArithmetic()[0];
			if (elements[2].size() == 0) elements[2] = this->calculator->getCurrentArithmetic()[0];

			if (elements[0][0] != '-' && elements[0].size() > 8) {
				cout << "\nВы ввели слишком большое число\nПопробуйте еще раз : ";
				continue;
			}
			if (elements[0][0] == '-' && elements[0].size() > 9) {
				cout << "\nВы ввели слишком маленькое число\nПопробуйте еще раз : ";
				continue;
			}
			if (elements[2][0] != '-' && elements[2].size() > 8) {
				cout << "\nВы ввели слишком большое число\nПопробуйте еще раз : ";
				continue;
			}
			if (elements[2][0] == '-' && elements[2].size() > 9) {
				cout << "\nВы ввели слишком маленькое число\nПопробуйте еще раз : ";
				continue;
			}

			if (!CheckElement(elements[0]) || !CheckElement(elements[2]) || !CheckAction(elements[1])) {
				cout << error_message;
				continue;
			}

			string answer;

			if (elements[1] == "+") answer = calculator->Summ(elements[0], elements[2]);
			if (elements[1] == "-") answer = calculator->Diff(elements[0], elements[2]);
			if (elements[1] == "*") answer = calculator->Compos(elements[0], elements[2]);
			if (elements[1] == "/") {
				if (elements[2] == this->calculator->getCurrentArithmetic()[0] && elements[0] == this->calculator->getCurrentArithmetic()[0]) {
					string max_element = this->calculator->getCurrentArithmetic()[this->calculator->getCurrentArithmetic().size() - 1];
					string ans;
					for (int i = 0; i < 8; i++) {
						ans += max_element;
					}
					cout << "Результат: [-" << ans << "; " << ans << "]" << endl;
					break;
				}
				else if (elements[2] == this->calculator->getCurrentArithmetic()[0]) {
					cout << "Результат: Пустое множество!" << endl;
					break;
				}
				else {
					vector<string> div_answer = calculator->Div(elements[0], elements[2]);
					if (div_answer[0][0] != '-' && div_answer[0].size() > 8) {
						cout << "Результат: Переполнение!";
						break;
					}
					if (div_answer[0][0] == '-' && div_answer[0].size() > 9) {
						cout << "Результат: Переполнение!";
						break;
					}
					if (div_answer[1][0] != '-' && div_answer[1].size() > 8) {
						cout << "Результат: Переполнение!";
						break;
					}
					if (div_answer[1][0] == '-' && div_answer[1].size() > 9) {
						cout << "Результат: Переполнение!";
						break;
					}

					if (div_answer[1] != this->calculator->getCurrentArithmetic()[0]) cout << "Результат: " << div_answer[0] << " Остаток: " << div_answer[1] << endl;
					else cout << "Результат: " << div_answer[0];
					break;
				}
			}

			if (answer[0] != '-' && answer.size() > 8) answer = "Переполнение!";
			if (answer[0] == '-' && answer.size() > 9) answer = "Переполнение!";
			cout << "Результат: " << answer << endl;
			input_flag = false;
		}
	}
	
}

bool Screen::CheckElement(string element) {
	vector<string> valid_characters = this->calculator->getCurrentArithmetic();
	valid_characters.push_back("-");
	bool all_valid = true;

	for (char c : element) {
		string char_as_string(1, c);
		if (std::find(valid_characters.begin(), valid_characters.end(), char_as_string) == valid_characters.end()) {
			all_valid = false;
			break;
		}
	}
	return all_valid;
}

bool Screen::CheckAction(string action) {
	vector<string> valid_characters({"+", "-", "*", "/"});
	bool all_valid = true;
	if (std::find(valid_characters.begin(), valid_characters.end(), action) == valid_characters.end()) {
		all_valid = false;
	}

	return all_valid;
}
