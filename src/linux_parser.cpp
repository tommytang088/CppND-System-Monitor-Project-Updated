#include "linux_parser.h"

#include <dirent.h>
#include <format.h>
#include <unistd.h>

#include <string>
#include <vector>

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
  string os, version, kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float mem_total{-1.0}, mem_free{-1.0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && (mem_total < 0 || mem_free < 0)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == kTotalMemory)
        mem_total = stof(value);
      else if (key == kFreeMemory)
        mem_free = stof(value);
    }
  }
  return (mem_total - mem_free) / mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string time_str;
  long time;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> time_str;
    time = stoi(time_str);
  }
  return time;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  vector<string> stats{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if (key == kCpuUnitilization) {
      for (int i = 0; i < kCpuStatesNum; i++) {
        linestream >> value;
        stats.emplace_back(value);
      }
    }
  }
  return stats;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  int n_procs{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == kTotalProcsKey) {
        n_procs = stoi(value);
        break;
      }
    }
  }
  return n_procs;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int n_procs{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == kRunningProcsKey) {
        n_procs = stoi(value);
        break;
      }
    }
  }
  return n_procs;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd = string();
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, cmd);
  }
  filestream.close();
  return cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key;
  string ram = string();
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == kRam) {
        linestream >> ram;
        ram = to_string(stoi(ram) / 1024);
        filestream.close();
        break;
      }
    };
  }
  return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key;
  string uid = string();
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == kUserId) {
        linestream >> uid;
        filestream.close();
        break;
      }
    };
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, dummy, uid;
  string user = string();
  string targetId = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> dummy >> uid;
      if (uid == targetId) {
        filestream.close();
        break;
      }
    }
  }
  return user;
}

// Return and return process stats
vector<string> LinuxParser::ProcessStats(int pid) {
  string line, value;
  vector<string> stats{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) stats.emplace_back(value);
    filestream.close();
  }
  return stats;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long time = 0;
  vector<string> stats = ProcessStats(pid);
  if (stats.size() > ProcessStats::kUpTime_)
    time = stoi(stats[ProcessStats::kUpTime_]) / sysconf(_SC_CLK_TCK);
  return time;
}
