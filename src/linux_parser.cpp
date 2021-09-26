#include <deque>
#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string os, kernel, version;
  string line;
  string key;
  string value;
  string mem_total_str;
  string mem_free_str;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if(key == "MemTotal:") {
          mem_total_str = value;
        } else if (key == "MemFree:") {
            mem_free_str = value;
        }
      }
    }
  }  
  return 1 - stof(mem_free_str)/stof(mem_total_str);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string key;
  string value;
  string uptime;
  string idletime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return stol(uptime); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    std::string line;
    std::string key;
    float cpu_user;
    float cpu_nice;
    float cpu_system;
    float cpu_idle;
    float cpu_iowait;
    float cpu_irq;
    float cpu_softirq;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        if (line.rfind("cpu ", 0) == 0) {
          std::istringstream linestream(line);
          linestream >> key >> cpu_user >> cpu_nice >> cpu_system >> cpu_idle >> cpu_iowait >> cpu_irq >> cpu_softirq;
        }
      }
    }
  float result = cpu_user + cpu_nice + cpu_system + cpu_idle + cpu_iowait + cpu_irq + cpu_softirq;
  return result;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long result = 0;
  string line;
  string val;
  int val_lng;
  int vals_idx = 1;
  std::deque<int> metric_idxs{14, 15, 16, 17};
  string metric_keys[] = {"stime", "utime", "cutime", "cstime"};
  vector<long> metrics;
  string pidDirectory = "/" + std::to_string(pid);
  std::ifstream stream(kProcDirectory +  pidDirectory + kStatFilename);
  int metrics_idx = metric_idxs.front();
  metric_idxs.pop_front();
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> val) {
      if(vals_idx == metrics_idx) {
        val_lng = std::stoi(val);
        metrics_idx = metric_idxs.front();
        metric_idxs.pop_front();
        metrics.push_back(val_lng);
      }
      vals_idx += 1;
    }
  }

  for (auto& n : metrics) {
    result += n;
  }

  return result;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::string line;
  std::string key;
  float cpu_user;
  float cpu_nice;
  float cpu_system;
  float cpu_idle;
  float cpu_iowait;
  float cpu_irq;
  float cpu_softirq;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.rfind("cpu ", 0) == 0) {
        std::istringstream linestream(line);
        linestream >> key >> cpu_user >> cpu_nice >> cpu_system >> cpu_idle >> cpu_iowait >> cpu_irq >> cpu_softirq;
      }
    }
  }
  float result = cpu_user + cpu_nice + cpu_system  + cpu_iowait + cpu_irq + cpu_softirq;
  return result;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::string line;
  std::string key;
  float cpu_user;
  float cpu_nice;
  float cpu_system;
  float cpu_idle;
  float cpu_iowait;
  float cpu_irq;
  float cpu_softirq;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.rfind("cpu ", 0) == 0) {
        std::istringstream linestream(line);
        linestream >> key >> cpu_user >> cpu_nice >> cpu_system >> cpu_idle >> cpu_iowait >> cpu_irq >> cpu_softirq;
      }
    }
  }
  float result = cpu_idle;
  return result;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.rfind("processes ", 0) == 0) { // pos=0 limits the search to the prefix
        std::istringstream linesteam(line);
        linesteam >> key >> val;
        return val;
      }
    }

  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.rfind("procs_running ", 0) == 0) { // pos=0 limits the search to the prefix
        std::istringstream linesteam(line);
        linesteam >> key >> val;
        return val;
      }
    }

  }
  return 0;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string pidDirectory = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pidDirectory + kCmdlineFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string("unkown");
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string result;
  string line;
  string key;
  string val;
  string units;
  string pidDirectory = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pidDirectory + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.rfind("VmSize:", 0) == 0) {
        std::istringstream linestream(line);
        linestream >> key >> val >> units;
        return val + " " + units;
      }
    }
  }
  return string("?");
}


// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string val;
  string pidDirectory = std::to_string(pid);
  std::ifstream stream(kProcDirectory + pidDirectory + kStatusFilename);
  if(stream.is_open()) {
     while(std::getline(stream, line)) {
        if (line.rfind("Uid:", 0) == 0) {
          std::istringstream linestream(line);
          linestream >> key >> val;
          return val;
        }
      }
  }
  return string();
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string result;
  string line;
  string key;
  string val;
  std::ifstream stream(kPasswordPath);
  size_t pos;
  string delimiter = ":";
  int token_num;
  string uid = LinuxParser::Uid(pid);

  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      pos = 0;
      token_num = 0;
      std::string token;
      while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        if(token_num == 0) {
          result = token;
        }
        if (token_num == 2 && token == uid) {
          return result;
        }
        line.erase(0, pos + delimiter.length());
        token_num++;
      }
    }
  }
  return string("unkown");
}


// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
