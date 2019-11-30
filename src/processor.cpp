#include <thread>
#include "processor.h"
#include "linux_parser.h"

double Processor::Utilization() {
    long const prev_idle = LinuxParser::IdleJiffies();
    long const prev_nonidle = LinuxParser::ActiveJiffies();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    long idle = LinuxParser::IdleJiffies();
    long nonidle = LinuxParser::ActiveJiffies();
    idle = idle - prev_idle;
    nonidle = nonidle - prev_nonidle;
    return (double) nonidle / (nonidle + idle);
}