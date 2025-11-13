#include <gtest/gtest.h>
#include "App.h"
#include <utility> // Required for pair

using namespace usd;

pair<int, string> getOutput(int argc, char *argv[], string input) {
    // 1. Save original buffer
    streambuf* old_buf_o = cout.rdbuf();
    streambuf* old_buf_i =  cin.rdbuf();
    // 2. Redirect to stringstream
    stringstream ss_o;
    stringstream ss_i(input);
    cout.rdbuf(ss_o.rdbuf());
    cin.rdbuf(ss_i.rdbuf());

    int ret = initializeAndRun(argc, argv);

    // 3. Restore original buffer
    cout.rdbuf(old_buf_o);
    cin.rdbuf(old_buf_i);
    return {ret, ss_o.str};
}

TEST(AdditionTest, PositiveNumbers) {
    auto actual = getOutput(1, ["."], "");
    int exit_status = actual.first;
    string actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}