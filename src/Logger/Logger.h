#ifndef LOGGER_H
#define LOGGER_H

// TODO: Look up log.c or spdlog -- thread safe logging, used by mainline
// engines

#include <string>
#include <vector>

enum LogType { LOG_INFO, LOG_WARNING, LOG_ERROR };

struct LogEntry {
  LogType type;
  std::string message;
};

class Logger {
private:
  static std::string FormattedTime();

public:
  static std::vector<LogEntry> messages;
  static void Log(const std::string &message);
  static void Err(const std::string &message);
};

#endif
