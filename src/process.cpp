#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid){};

// Done: Return this process's ID
int Process::Pid() const { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() const {
  float act_time = float(LinuxParser::ActiveJiffies(pid_));
  float up_time = float(LinuxParser::UpTime(pid_));

  float cpu_util = (act_time/up_time)/100.f;
  return cpu_util;

}

// Done: Return the command that generated this process
string Process::Command()  { return LinuxParser::Command(pid_); }

// Done: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_);}

// Done: Return the user (name) that generated this process
string Process::User()  { return LinuxParser::User(pid_); }



// Done: Return the age of this process (in seconds)
long int Process::UpTime()  { return LinuxParser::UpTime(pid_); }

// Done: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return stol(Ram()) < stol(a.Ram());
}
