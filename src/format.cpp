#include "format.h"

#include <ostream>
#include <string>

using std::string;

string Sanitize(string s) { return s.size() < 2 ? "0" + s : s; }

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string h, m, s;
  h = Sanitize(std::to_string(seconds / 3600));
  m = Sanitize(std::to_string(seconds / 60 % 60));
  s = Sanitize(std::to_string(seconds % 60));
  return h + ":" + m + ":" + s;
}
