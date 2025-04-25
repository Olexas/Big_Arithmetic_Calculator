#pragma once

#include <iostream>
#include "settings.h"

using namespace std;

bool InputClear();
char* StrInput(int& variable_size);
char* IncreaseBuffer(char* buffer, int& buffer_size);
bool CorrectIntInput(char* input_str);
int ProgramStartInputs(int min_limit, int max_limit, string& error_message, int buffer_size);
