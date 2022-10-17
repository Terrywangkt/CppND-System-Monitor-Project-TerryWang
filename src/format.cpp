#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int HH, MM, SS;
  HH = (seconds / 3600);
  MM = (seconds / 60) % 60;
  SS = seconds % 60;
  return std::to_string(HH)+ ":" + std::to_string(MM) + ":" + std::to_string(SS);



}