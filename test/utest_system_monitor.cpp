#include <gtest/gtest.h>
#include <iostream>

#include "linux_parser.h"

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

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}