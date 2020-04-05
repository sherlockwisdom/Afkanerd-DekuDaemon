#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"
#include <iostream>

using namespace std;

int sum(int a, int b ) {
	return a + b;
}

TEST_GROUP(Functions1) {
};
TEST_GROUP(Functions) {
};

TEST(Functions, sum) {
	FAIL("Failed");
}

TEST(Functions, string_com) {
	STRCMP_EQUAL("hello", "world");
}

TEST(Functions1, string_com) {
	STRCMP_EQUAL("hello", "hello");
}

int main(int argc, char** argv) {

	return CommandLineTestRunner::RunAllTests(argc, argv);
}
