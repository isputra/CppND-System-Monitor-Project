#include <gtest/gtest.h>
#include "../include/linux_parser.h"
#include <iostream>

TEST(LinuxParserTest, OperatingSystem)
{
    ASSERT_EQ(LinuxParser::OperatingSystem(), "Debian GNU/Linux 10 (buster)");
}

TEST(LinuxParserTest, Kernel)
{
    ASSERT_EQ(LinuxParser::Kernel(), "4.19.0-6-amd64");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}