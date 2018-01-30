#ifndef CMDLINE_DSLINK_LOG_H
#define CMDLINE_DSLINK_LOG_H

#include "term_color.h"

#ifdef _WIN32
#define TERM_COLOR_STREAM termcolor::magenta
#else
#define TERM_COLOR_STREAM termcolor::blue
#endif

namespace cmdlog {

inline
std::ostream &reset(std::ostream &stream) {
  return stream << termcolor::reset;
}

inline
std::ostream &error(std::ostream &stream) {
  return stream << termcolor::reset << termcolor::red
                << "| ERROR |" << termcolor::reset << " ";
}

inline
std::ostream &info(std::ostream &stream) {
  return stream << termcolor::reset << termcolor::reset
                << "| INFO  |" << termcolor::reset << " ";
}

inline
std::ostream &success(std::ostream &stream) {
  return stream << termcolor::reset << termcolor::green
                << "|SUCCESS|" << termcolor::reset << " ";
}

inline
std::ostream &var(std::ostream &stream) {
  return stream << termcolor::reset << termcolor::yellow;
}

inline
std::ostream &stream(std::ostream &stream) {
  return stream << TERM_COLOR_STREAM << "# ";
}

inline
std::ostream &path(std::ostream &stream) {
  return stream << termcolor::reset << termcolor::cyan;
}

inline
std::ostream &endl(std::ostream &stream) {
  return stream << termcolor::reset << std::endl;
}

};
#endif //CMDLINE_DSLINK_LOG_H
