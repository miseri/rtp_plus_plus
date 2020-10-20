#pragma once

// To prevent double inclusion of winsock on windows
#ifdef _WIN32
// To be able to use std::max
#define NOMINMAX
#include <WinSock2.h>
#endif

#ifdef _WIN32
  // To get around compile error on windows: ERROR macro is defined
  #define GLOG_NO_ABBREVIATED_SEVERITIES
#endif
#include <glog/logging.h>

// Define this directive in both CorePch.h AND the application to be debugged
// #define BOOST_ASIO_ENABLE_HANDLER_TRACKING



