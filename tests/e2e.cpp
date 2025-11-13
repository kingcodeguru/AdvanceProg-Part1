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
    pair<int, string> actual; int exit_status; string actual_output;
    // ------------ TEST 1 ------------
    actual = getOutput(1, ["test1"], "");
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, ""); // empty input - empty output
    // ------------ TEST 2 ------------
    actual = getOutput(1, ["test2"], "add file1 hello world!\nget file1");
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, "hello world!"); // checking if add and get works properly.
    // ------------ TEST 3 ------------
    actual = getOutput(1, ["test3"], "get file");
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, ""); // checks if not crashing and not displaying error message when tying to receive an non-existing file.
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}