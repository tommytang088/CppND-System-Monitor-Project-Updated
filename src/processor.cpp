#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using LinuxParser::CPUStates;
using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  float idle, nonIdle, total, res;
  vector<string> stats = LinuxParser::CpuUtilization();
  idle = stoi(stats[CPUStates::kIdle_]) + stoi(stats[CPUStates::kIOwait_]);
  nonIdle = stoi(stats[CPUStates::kUser_]) + stoi(stats[CPUStates::kNice_]) +
            stoi(stats[CPUStates::kSystem_]) + stoi(stats[CPUStates::kIRQ_]) +
            stoi(stats[CPUStates::kSoftIRQ_]) + stoi(stats[CPUStates::kSteal_]);
  total = idle + nonIdle;
  // compute CPU percentage based on the difference between new and cached
  res = ((total - total_) - (idle - idle_)) / (total - total_);
  // update cached stats
  total_ = total;
  idle_ = idle;

  return res;
}