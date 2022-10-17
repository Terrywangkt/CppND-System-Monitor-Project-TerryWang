#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;                               //
  std::string User()  ;                      // Done: See src/process.cpp
  std::string Command() ;                   // Done: See src/process.cpp
  float CpuUtilization() const;                  // Done: See src/process.cpp
  std::string Ram() const;                       // Done: See src/process.cpp
  long int UpTime() ;                       // Done: See src/process.cpp
  bool operator<(Process const& ai100) const;  // Done: See src/process.cpp

  // Done: Declare any necessary private members
 private:
  int pid_;

};

#endif
