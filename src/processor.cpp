#include "processor.h"
#include "linux_parser.h"
// Done: Return the aggregate CPU utilization
float Processor::Utilization() {
  long total = LinuxParser::Jiffies();
  long active = LinuxParser::ActiveJiffies();
  float util =  float(active)/float(total);
  return util;
}
