#include "process.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int p) : pid(p) {
  user = LinuxParser::User(pid);
  command = LinuxParser::Command(pid);
  startTime = LinuxParser::StartTime(pid);
}

// Return this process's ID
int Process::Pid() { return pid; }

// Return the last computed CPU Utilization
float Process::CpuUtil() const { return cpuUtil; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  vector<string> stats = LinuxParser::ProcessStats(pid);
  // Compute CPU Utilization as total time CPU sent on the process / process up
  // time
  if (stats.size() > 0) {
    long totalTime = stoi(stats[LinuxParser::ProcessStats::kUtime_]) +
                     stoi(stats[LinuxParser::ProcessStats::kStime_]) +
                     stoi(stats[LinuxParser::ProcessStats::kChildUtime_]) +
                     stoi(stats[LinuxParser::ProcessStats::kChildStime_]);
    totalTime = totalTime / sysconf(_SC_CLK_TCK);
    float upTime = UpTime();
    if (upTime > 0) cpuUtil = totalTime / upTime;
  }
  return cpuUtil;
}

// Return the command that generated this process
string Process::Command() { return command; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// Return the user (name) that generated this process
string Process::User() { return user; }

// Return the age of this process (in seconds)
long int Process::UpTime() {
  long sysUpTime = LinuxParser::UpTime();
  return sysUpTime - startTime;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtil() > a.CpuUtil();
}