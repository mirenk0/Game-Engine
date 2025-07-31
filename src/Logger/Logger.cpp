#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

// TODO: Look up log.c or spdlog -- thread safe logging, used by mainline
// engines

std::vector<LogEntry> Logger::messages;

std::string Logger::FormattedTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
  std::string readableTime = std::ctime(&nowTime);

  if (!readableTime.empty() && readableTime.back() == '\n') {
    readableTime.pop_back();
  }

  return readableTime;
}

void Logger::Log(const std::string &message) {
  LogEntry logEntry;
  logEntry.type = LOG_INFO;
  logEntry.message = "Log: [ " + FormattedTime() + " ]: " + message;
  std::cout << "\033[32m" << logEntry.message << "\033[0m" << std::endl;

  messages.push_back(logEntry);
}

void Logger::Err(const std::string &message) {
  LogEntry logEntry;
  logEntry.type = LOG_ERROR;
  logEntry.message = "Error: [ " + FormattedTime() + " ]: " + message;
  std::cout << "\033[31m" << logEntry.message << "\033[0m" << std::endl;

  messages.push_back(logEntry);
}
