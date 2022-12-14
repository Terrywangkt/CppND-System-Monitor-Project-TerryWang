#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value, total, free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          total = value;
        }
        if (key == "MemFree:") {
          free = value;
        }
      }
    }
  }
  return (stof(total) - stof(free)) / stof(total);
}


// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime,line;
  long uptimeNum;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>uptime;
  }
  uptimeNum = std::stol(uptime);
  return uptimeNum;
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies();
}

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    while (linestream >> value){
      values.push_back(value);
    }
  }
  long jeffies = stol(values[13]) + stol(values [14]) + stol(values[15]) + stol(values[16]);
  return jeffies;
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();
  return (stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
          stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates:: kIRQ_]) +
          stol(jiffies[CPUStates::kSoftIRQ_]) + stol(jiffies[CPUStates::kSteal_]) +
          stol(jiffies[CPUStates :: kGuest_]) + stol(jiffies[CPUStates::kGuestNice_]));

}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  return (stol(jiffies[CPUStates::kIdle_])+ stol(jiffies[CPUStates::kIOwait_]));
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key, value;
  vector<string> CpuValues;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (getline(stream,line)){
      std::istringstream linestream(line);
      while(linestream >> key){
        if (key == "cpu"){
          while (linestream >> value) {
            CpuValues.push_back(value);
          }
        }
      }

    }
  }
  return CpuValues;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  int valueNum = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes"){
          valueNum = stoi(value);
        }
      }
    }
  }
  return valueNum;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int valueNum = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running"){
          valueNum = stoi(value);
        }
      }
    }
  }
  return valueNum;
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream,line);

  }
  return line;
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  string RamValue = "0";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      while (linestream >> key){
        if (key == "VmSize:"){
          if(linestream >> value){
            RamValue =  to_string(stoi(value)/1024);
          }
        }
      }
    }
  }
  return RamValue;
}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      while (linestream >> key){
        if (key == "Uid:"){
          if(linestream >> value){
            return value;
          }
        }
      }
    }
  }
  return "";

}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line,user,x,uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream,line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid ){
        if (uid == Uid(pid)){
          return user;
        }
      }
    }
  }
  return "";
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  long uptime;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value) {
      values.push_back(value);
    }
  }
  long system_uptime = UpTime();
  uptime = system_uptime - stol(values[21])/100;
  return uptime;
}

