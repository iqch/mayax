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

// RI

#include <ri.h>
#include <rx.h>