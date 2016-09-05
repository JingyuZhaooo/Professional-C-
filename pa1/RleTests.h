#pragma once
#include "MiniCppUnit.hpp"
#include "RleData.h"
#include <iostream>
#include <iomanip>

// Helper functions
bool buffersAreSame(const char* expected, const char* actual, size_t size);
void runCompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);
void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);

// IMPORTANT NOTE --
// The tests all use "sizeof" to find the size of the input
// buffer. However, remember that this only works because the
// character arrays are statically allocated.
// If you just have a pointer to a C-style string, "sizeof"
// will return the size of the pointer type (either 4 or 8).
class CompressionTests : public TestFixture<CompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(CompressionTests, "Testing Compression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive runs test");
		// TODO: Add more Compression test cases
		TEST_CASE_DESCRIBE(testLongPositiveRuns, "Long Positive runs test"); // A long positive run
		TEST_CASE_DESCRIBE(testLongNegativeRuns, "Long negative runs test"); // A long negative run
		TEST_CASE_DESCRIBE(testAlternatingRuns, "Alternating positive/negative runs test"); // An alternating positive/negative runs
		TEST_CASE_DESCRIBE(tooLongPositiveRuns, "Too long positive runs test"); // A really long positive run that goes greater than the max run size
		TEST_CASE_DESCRIBE(tooLongNegativeRuns, "Too long negative runs test"); // A really long negative run that goes greater than the max run size
		TEST_CASE_DESCRIBE(singleCharAtEndRuns, "A single Character at the end of the run"); //Like aaaaaaaab
		TEST_CASE_DESCRIBE(negative128LengthRuns, "128 unique characters run");
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
			"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testLongPositiveRuns()
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
		char expected[] = "\x28" "a" "\x28" "b"; // 40a 40b
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testLongNegativeRuns()
	{
		char test[] = "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"
			"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm";
		char expected[] = "\x98" "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"; // -104 negative run
			"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm";
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testAlternatingRuns()
	{
		char test[] = "aaaaaaaaaaqwertyuiopbbbbbbbbbbqwertyuiop"
			"aaaaaaaaaaqwertyuiopbbbbbbbbbbqwertyuiop";
		char expected[] = "\x0A" "a" "\xf6" "qwertyuiop" "\x0A" "b" "\xf6" "qwertyuiop"// 10a -10 10b -10
			"\x0A" "a" "\xf6" "qwertyuiop" "\x0A" "b" "\xf6" "qwertyuiop"; // // 10a -10 10b -10
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void tooLongPositiveRuns()
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaa";
		char expected[] = "\x7f" "a" "\x0d" "a"; //127a 13a
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void tooLongNegativeRuns()
	{
		char test[] = "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"
			"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"
			"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm";
		char expected[] = "\x81" "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm" // -127
			"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"
			"qwertyuiopasdfghjklzxcv" "xe3" "bnmqwertyuiopasdfghjklzxcvbnm"; //-29
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void singleCharAtEndRuns()
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaab";
		char expected[] = "\x7e" "a" "\x01" "b"; // 126a 1b
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void negative128LengthRuns()
	{
		char test[] = "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"
			"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"
			"qwertyuiopasdfghjklzxcvb";
		char expected[] = "\x81" "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm" //-127
			"qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm"
			"qwertyuiopasdfghjklzxcv" "\x01" "b";								// 1b
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
};

class DecompressionTests : public TestFixture<DecompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(DecompressionTests, "Testing Decompression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive run test");
		// TODO: Add more Decompression test  cases
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
		runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
};

REGISTER_FIXTURE(CompressionTests);
REGISTER_FIXTURE(DecompressionTests);

bool buffersAreSame(const char* expected, const char* actual, size_t size)
{
	if (actual == nullptr)
	{
		std::cerr << std::endl << "Actual data is null." << std::endl;
		return false;
	}

	bool retVal = true;

	for (size_t i = 0; i < size; i++)
	{
		if (expected[i] != actual[i])
		{
			retVal = false;
			std::cerr << std::endl
				<< "Expected 0x" << std::hex << static_cast<unsigned>(expected[i])
				<< " at index " << std::dec << i << " but saw 0x"
				<< std::hex << static_cast<unsigned>(actual[i]) << std::endl;
			break;
		}
	}

	return retVal;
}

void runCompressionTest(char* test, size_t testSize, 
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Compress(test, testSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}

void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Decompress(test, testSize, expectedSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}
