#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Parser keys
const std::string kCpuUnitilization{"cpu"};
const std::string kTotalMemory{"MemTotal:"};
const std::string kFreeMemory{"MemFree:"};
const std::string kTotalProcsKey{"processes"};
const std::string kRunningProcsKey{"procs_running"};
const std::string kUserId{"Uid:"};
const std::string kRam{"VmSize:"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

const int kProcsUpTimeIndex{21};
const int kCpuStatesNum{10};

// CPU

enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();

// Processes

enum ProcessStats {
  kUtime_ = 13,
  kStime_ = 14,
  kChildUtime_ = 15,
  kChildStime_ = 16,
  kUpTime_ = 21,
};

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
std::vector<std::string> ProcessStats(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif