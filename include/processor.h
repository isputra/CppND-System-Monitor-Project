#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    std::vector<std::string> prev_cpus {10};
    std::vector<std::string> now_cpus {10};
};

#endif