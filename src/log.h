/**
 * spdlog wrapper. Will only log if DEBUG is defined by the compiler
 */

#pragma once

#ifdef DEBUG

#include <spdlog/spdlog.h>

#define LOG_INIT spdlog::set_pattern("%^[%L](%T):%v%$")
#define LOG_LEVEL spdlog::set_level(spdlog::level::debug)

#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)

#else

#define LOG_INIT
#define LOG_LEVEL
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_CRITICAL(...)
#define LOG_ERROR(...)
#define LOG_DEBUG(...)
#define LOG_TRACE(...)

#endif
