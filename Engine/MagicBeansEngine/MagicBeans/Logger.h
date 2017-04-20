#pragma once
#include <string>

#define LOG(message) LogString(__LINE__, __FILE__, __FUNCTION__, message)

void RestartLog();

void LogString(long line, const char* file, const char* function, const std::string& message);