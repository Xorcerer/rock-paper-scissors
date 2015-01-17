// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <winsock2.h>

#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>
#include <functional>
#include <atomic>

#include "Log.h"
#include "consts.h"

#define NONCOPYABLE(__CLASSNAME__) 	\
	__CLASSNAME__(__CLASSNAME__ const &_) = delete; \
	__CLASSNAME__ &operator=(__CLASSNAME__ const &_) = delete;