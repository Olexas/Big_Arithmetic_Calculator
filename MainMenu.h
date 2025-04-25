#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "settings.h"
#include "Calculator.h"
#include "Input.h"

using namespace std;

class Screen {
private:
	Calculator* calculator;

public:
	vector<string> splitstr(string str, string deli = " ");

	void Clear();
	void ProgramStart();
	void MainMenu();

	bool CheckElement(string element);
	bool CheckAction(string action);
};


