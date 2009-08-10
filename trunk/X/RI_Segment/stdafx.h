#pragma once

#ifndef LINUX
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#define _USE_MATH_DEFINES

// CRT
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
using namespace std;

// RI

#include <ri.h>
#include <rx.h>