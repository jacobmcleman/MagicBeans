#include "Helpers.h"
#include <string>
#include <sstream>
#include "Logger.h"
void GLDebug::glErrorCheck_(const char *file, int line)
{
#ifdef _NDEBUG
  return;
#endif
  GLenum errorCode;

  while ((errorCode = glGetError()) != GL_NO_ERROR)
  {
    std::string error;
    std::stringstream errorMessage;
    switch (errorCode)
    {
    case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
    case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
    case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
    case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
    case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
    case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
    default:							   error = "UNKNOWN ERROR"; break;
    }
    errorMessage << error << " | " << file << " (" << line << ")";
    LOG(errorMessage.str());
  }
}