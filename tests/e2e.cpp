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
    actual = getOutput(1, ["test1"], "\n");
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, ""); // empty input - empty output
    // ------------ TEST 2 ------------
    actual = getOutput(1, ["test2"], "add file1 hello world!\nget file1\n");
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, "hello world!"); // checking if add and get works properly.
    // ------------ TEST 3 ------------
    actual = getOutput(1, ["test3"], "get file\n");
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, ""); // checks if not crashing and not displaying error message when tying to receive an non-existing file.
    // ------------ TEST 4 ------------
    actual = getOutput(1, ["test4"], ""
        "add file1 hello, how are you?\n"
        "add file2 I said to him: hello\n"
        "add file3 this sentence doesn't contain the h-word\n"
        "add file4 why don't you say hello to me\n"
        "search hello\n"
    );
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, "file1\nfile2\n\nfile4\n"); // test search function
    // ------------ TEST 5 ------------
    actual = getOutput(1, ["test5"], ""
        "add file1 text1\n"
        "add file1 text2\n"
        "get file1\n"
        "get file2\n"
    );
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, "text2\n"); // test overwriting and non-existing files
    // ------------ TEST 6 ------------
    actual = getOutput(1, ["test6"], ""
        "add\n"
        "add file1\n"
        "get\n"
        "search\n"
    );
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, "text2\n"); // test non-matching number of arguments
    // ------------ TEST 7 ------------
    actual = getOutput(1, ["test7"], ""
        "add file1 aaaaaaaaaa\n"
        "add file2 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
        "get file1\n"
        "get file2\n"
    );
    exit_status = actual.first;
    actual_output = actual.second;

    EXPECT_EQ(exit_status, 0);
    EXPECT_EQ(actual_output, "aaaaaaaaaa\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    // test if works on >= 10 chars and on >= 100 chars (because of compression method).
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}