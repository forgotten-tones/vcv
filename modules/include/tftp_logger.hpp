#pragma once

#include <plog/Record.h>
#include <plog/Util.h>
#include <iomanip>
#include <termcolor/termcolor.hpp>

using namespace plog;

namespace tftp {
namespace log {

template <bool useUtcTime>
class TxtFormatterImpl {
 public:
  static util::nstring header() { return util::nstring(); }

  static util::nstring format(const Record& record) {
    tm t;
    useUtcTime ? util::gmtime_s(&t, &record.getTime().time)
               : util::localtime_s(&t, &record.getTime().time);
    util::nostringstream ss;
    ss << termcolor::colorize;
    ss << termcolor::green << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0'))
       << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-") << std::setfill(PLOG_NSTR('0'))
       << std::setw(2) << t.tm_mday << PLOG_NSTR(" ");
    ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":")
       << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":")
       << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".")
       << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int>(record.getTime().millitm)
       << termcolor::reset << PLOG_NSTR(" ");
    switch (record.getSeverity()) {
      case fatal:
        ss << termcolor::grey << termcolor::on_red << PLOG_NSTR("FATAL") << termcolor::reset;
        break;
      case error:
        ss << termcolor::bright_red << PLOG_NSTR("ERROR") << termcolor::reset;
        break;
      case warning:
        ss << termcolor::bright_yellow << PLOG_NSTR("WARN") << termcolor::reset;
        break;
      case info:
        ss << termcolor::bright_green << PLOG_NSTR("INFO") << termcolor::reset;
        break;
      case debug:
        ss << termcolor::bright_cyan << "DEBUG" << termcolor::reset;
        break;
      case verbose:
        ss << termcolor::cyan << "TRACE" << termcolor::reset;
        break;
      default:
        ss << "";
    }
    ss << PLOG_NSTR(" ");
    ss << termcolor::green << PLOG_NSTR("[") << termcolor::reset << termcolor::yellow
       << record.getFile() << termcolor::reset;
    ss << termcolor::green << PLOG_NSTR(":") << termcolor::reset << termcolor::yellow
       << record.getLine() << termcolor::reset << PLOG_NSTR(" ");
    ss << termcolor::bright_yellow << record.getFunc() << termcolor::reset << termcolor::green
       << PLOG_NSTR(" (") << termcolor::reset << termcolor::yellow << record.getTid()
       << termcolor::reset << termcolor::green << PLOG_NSTR(")") << PLOG_NSTR("]")
       << termcolor::reset;
    ss << termcolor::cyan << PLOG_NSTR(" ▶ ") << termcolor::reset << termcolor::bright_green
       << record.getMessage() << termcolor::reset << PLOG_NSTR("\n");

    return ss.str();
  }
};

class TxtFormatter : public TxtFormatterImpl<false> {};
class TxtFormatterUtcTime : public TxtFormatterImpl<true> {};
}  // namespace log
}  // namespace tftp
