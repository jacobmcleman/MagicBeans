#pragma once
#include <../glad/include/glad/glad.h>

namespace GLDebug
{
#define glErrorCheck() GLDebug::glErrorCheck_(__FILE__, __LINE__)

  void glErrorCheck_(const char *file, int line);
}