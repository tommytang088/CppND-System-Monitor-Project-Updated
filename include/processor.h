#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  float total_ = 0;  // latest CPU total time
  float idle_ = 0;   // latest CPU idle time
};

#endif