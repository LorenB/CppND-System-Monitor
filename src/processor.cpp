#include <fstream>
#include <sstream>
#include <string>

#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::string line;
    std::string key;
    float cpu_user;
    float cpu_nice;
    float cpu_system;
    float cpu_idle;
    float cpu_iowait;
    float cpu_irq;
    float cpu_softirq;

    const std::string kProcDirectory{"/proc/"};
    const std::string kStatFilename{"/stat"};
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        if (line.rfind("cpu ", 0) == 0) {
          std::istringstream linestream(line);
          linestream >> key >> cpu_user >> cpu_nice >> cpu_system >> cpu_idle >> cpu_iowait >> cpu_irq >> cpu_softirq;
        }
      }
    }
    float result = 1 -  cpu_idle  / ( cpu_user + cpu_nice + cpu_system + cpu_idle + cpu_iowait + cpu_irq + cpu_softirq );
    return result; 
}