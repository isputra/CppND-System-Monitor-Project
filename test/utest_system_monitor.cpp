#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "linux_parser.h"
#include "format.h"
#include "processor.h"
#include "system.h"

TEST(LinuxParserTest, OperatingSystem)
{
    ASSERT_EQ(LinuxParser::OperatingSystem(), "Ubuntu 19.04");
}

TEST(LinuxParserTest, Kernel)
{
    ASSERT_EQ(LinuxParser::Kernel(), "5.0.0-13-generic");
}

TEST(LinuxParserTest, Pids)
{
    ASSERT_GT(LinuxParser::Pids().size(), 0);
}

TEST(LinuxParserTest, MemoryUtilization)
{
    ASSERT_GT(LinuxParser::MemoryUtilization(), 0);
}

TEST(LinuxParserTest, TotalProcesses)
{
    ASSERT_GT(LinuxParser::TotalProcesses(), 0);
}

TEST(LinuxParserTest, RunningProcesses)
{
    ASSERT_GE(LinuxParser::RunningProcesses(), 0);
}

TEST(LinuxParserTest, UpTime)
{
    ASSERT_GE(LinuxParser::UpTime(), 0);
}

TEST(FormatTest, ElapsedTime)
{
    EXPECT_EQ(Format::ElapsedTime(3661), "01:01:01");
    EXPECT_EQ(Format::ElapsedTime(32949), "09:09:09");
    EXPECT_EQ(Format::ElapsedTime(36610), "10:10:10");
}

TEST(LinuxParserTest, CpuUtilization)
{
    ASSERT_EQ(LinuxParser::CpuUtilization().size(), 10);
}

TEST(ProcessorTest, Utilization)
{
    Processor procesor;
    ASSERT_GT(procesor.Utilization(), 0);

}

TEST(LinuxParserTest, Uid)
{
    ASSERT_EQ(LinuxParser::Uid(3746), "1000");
}

TEST(LinuxParserTest, User)
{
    ASSERT_EQ(LinuxParser::User(3746), "ikhsan");
}

TEST(LinuxParserTest, Command)
{
    ASSERT_GT(LinuxParser::Command(3746).size(), 0);
}

TEST(LinuxParserTest, Ram)
{
    ASSERT_GT(std::stoi(LinuxParser::Ram(3746)), 0);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}