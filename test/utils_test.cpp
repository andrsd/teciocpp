#include <gmock/gmock.h>
#include "teciocpp/utils.h"

using namespace teciocpp;

TEST(UtilsTest, join_int)
{
    auto str = utils::join<int>(" ", { 2, 6, 11 });
    EXPECT_EQ(str, "2 6 11");

    EXPECT_EQ(utils::join<int>(" ", {}), "");
}

TEST(UtilsTest, join_str)
{
    auto str = utils::join<std::string>(" ", { "2", "6", "11" });
    EXPECT_EQ(str, "2 6 11");

    EXPECT_EQ(utils::join<std::string>(" ", {}), "");
}
