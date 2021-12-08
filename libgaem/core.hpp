/**
 * Core header file that most things should include
 */

#pragma once

#include <libgaem/log.hpp>

// Dynamic Library
#if defined(LINUX)
#ifdef BUILD
#define GAEM_API __attribute__((visibility("default")))
#else
#define GAEM_API
#endif
#elif defined(WINDOWS)
#ifdef BUILD
#define GAEM_API __declspec(dllexport)
#else
#define GAEM_API __declspec(dllimport)
#endif
#else
#pragma warning Unknown dynamic link import / export semantics.
#define GAEM_API
#endif
