#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int p);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtil() const;
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid;
  std::string user;
  std::string command;
  long startTime;
  float cpuUtil = 0;
};

#endif