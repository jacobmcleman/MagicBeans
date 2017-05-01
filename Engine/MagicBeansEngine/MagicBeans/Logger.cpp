#include "Logger.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>

#define MAX_MESSAGE_LENGTH 1024
static char printbuffer[MAX_MESSAGE_LENGTH];

void RestartLog()
{
  std::ofstream logfile("Log.txt");

  if (logfile.is_open())
  {
    time_t timev;
    time(&timev);

    char timeString[32];
    ctime_s(timeString, 32, &timev);

    logfile << "Startup at " << timeString;

    logfile.close();
  }
}

void LogString(long line, const char * file, const char * function, const std::string& message)
{
  time_t timev;
  time(&timev);

  char timeString[32];
  ctime_s(timeString, 32, &timev);
  for (int i = 0; i < 32; ++i)
  {
    if (timeString[i] == '\n')
    {
      timeString[i] = 0;
      break;
    }
  }

  std::string fileName = file;
  fileName = fileName.substr(fileName.find_last_of('\\') + 1);
  
  sprintf_s(printbuffer, MAX_MESSAGE_LENGTH, "%-26s|%-24s(%4d)|%-50s|%s", timeString, fileName.c_str(), line, function, message.c_str());

  std::cout << printbuffer << std::endl;

  std::ofstream logfile("Log.txt", std::ofstream::app);

  if (logfile.is_open())
  {
    logfile << printbuffer << std::endl;
    logfile.close();
  }
}
