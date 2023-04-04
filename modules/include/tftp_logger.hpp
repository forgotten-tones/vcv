#pragma once

#include <plog/Record.h>
#include <plog/Util.h>
#include <iomanip>

using namespace plog;

namespace tftp {
namespace log {

inline const char* sevToStr(plog::Severity severity) {
  switch (severity) {
    case verbose:
      return "TRACE";
    default:
      return plog::severityToString(severity);
  }
}

template <bool useUtcTime>
class TxtFormatterImpl {
 public:
  static util::nstring header() { return util::nstring(); }

  static util::nstring format(const Record& record) {
    tm t;
    useUtcTime ? util::gmtime_s(&t, &record.getTime().time)
               : util::localtime_s(&t, &record.getTime().time);

    util::nostringstream ss;
    ss << PLOG_NSTR("FORGOTTEN TONES - ");
    ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1
       << PLOG_NSTR("-") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday
       << PLOG_NSTR(" ");
    ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":")
       << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":")
       << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".")
       << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int>(record.getTime().millitm)
       << PLOG_NSTR(" ");
    ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left
       << sevToStr(record.getSeverity()) << PLOG_NSTR(" ");
    ss << PLOG_NSTR("[") << record.getTid() << PLOG_NSTR("] ");
    ss << PLOG_NSTR("[") << record.getFunc() << PLOG_NSTR("@") << record.getLine()
       << PLOG_NSTR("] ");
    ss << record.getMessage() << PLOG_NSTR("\n");

    return ss.str();
  }
};

class TxtFormatter : public TxtFormatterImpl<false> {};
class TxtFormatterUtcTime : public TxtFormatterImpl<true> {};
}  // namespace log
}  // namespace tftp
