#include <fstream>
#include <sstream>
#include <string>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float ActiveJiffies = LinuxParser::ActiveJiffies();
    float Jiffies = LinuxParser::Jiffies();
    float result =  ActiveJiffies / Jiffies;
    return result; 
}