#pragma once

#define SSRT_DEBUG_MODE

#ifdef SSRT_DEBUG_MODE
  #define SSRT_DBG_OUTPUT(message) std::cout << "[Debug] " << message << std::endl
#else
  #define SSRT_DBG_OUTPUT(message)
#endif