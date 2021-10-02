#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::setPid(int pid) {
    pid_ = pid;
}

// Return this process's ID
int Process::Pid() {
    return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    long hertz = sysconf(_SC_CLK_TCK);
    string line;
    string key;
    string val_raw;
    float val;
    int vals_idx = 1;
    std::deque<int> metric_idxs{14, 15, 16, 17, 22};
    int metrics_idx = metric_idxs.front();
    metric_idxs.pop_front();
    vector<long> metrics;
    string pidDirectory = std::to_string(pid_);
    std::ifstream stream(LinuxParser::kProcDirectory + pidDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while(linestream >> val_raw) {
            if(vals_idx == metrics_idx) {
                val = std::stof(val_raw);
                metrics_idx = metric_idxs.front();
                metric_idxs.pop_front();
                metrics.emplace_back(val);
            }
            vals_idx++;
        }
    }
    float utime = metrics[0];
    float stime = metrics[1];
    float cutime = metrics[2];
    float cstime = metrics[3];
    float starttime = metrics[4];


    float uptime;
    std::ifstream stream_uptime(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
    if(stream_uptime.is_open()) {
        std::getline(stream_uptime, line);
        std::istringstream linestream_uptime(line);
        linestream_uptime >> val_raw;
        uptime = std::stof(val_raw);
    }

    float total_time = utime + stime + cstime + cutime;
    float seconds = uptime - (starttime / hertz);
    float ret = (total_time / hertz) / seconds;
    return ret;
}

// Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid_);
}

// Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(pid_);
}

// Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid_);
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return pid_ < a.pid_;
}