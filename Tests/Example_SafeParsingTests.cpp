/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

// Example unit test file demonstrating best practices
// This file shows how to test the SafeParsing functions

#include <gtest/gtest.h>
#include <limits>

// NOTE: Adjust this path based on your project structure
// #include "../../ShaderGC/SafeParsing.h"

// Mock the SafeParsing functions for this example
// In real tests, include the actual header
namespace Example {

bool SafeParseInt(const std::string& str, int& result)
{
    try {
        size_t pos;
        long long value = std::stoll(str, &pos);
        if(pos != str.length())
            return false;
        if(value > INT_MAX || value < INT_MIN)
            return false;
        result = static_cast<int>(value);
        return true;
    }
    catch(...) {
        return false;
    }
}

bool SafeParseFloat(const std::string& str, float& result)
{
    try {
        size_t pos;
        result = std::stof(str, &pos);
        return pos == str.length();
    }
    catch(...) {
        return false;
    }
}

} // namespace Example

// Test fixture provides shared setup/teardown
class SafeParsingTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Called before each test
        // Initialize any shared resources
    }

    void TearDown() override
    {
        // Called after each test
        // Clean up resources
    }

    // Helper methods for tests
    void ExpectParseSuccess(const std::string& input, int expected)
    {
        int result;
        EXPECT_TRUE(Example::SafeParseInt(input, result)) << "Failed to parse: " << input;
        EXPECT_EQ(result, expected) << "Wrong value for: " << input;
    }

    void ExpectParseFailure(const std::string& input)
    {
        int result;
        EXPECT_FALSE(Example::SafeParseInt(input, result)) << "Should have failed: " << input;
    }
};

// ============================================================================
// SafeParseInt Tests
// ============================================================================

TEST_F(SafeParsingTest, SafeParseInt_ValidPositive)
{
    ExpectParseSuccess("12345", 12345);
    ExpectParseSuccess("0", 0);
    ExpectParseSuccess("999", 999);
}

TEST_F(SafeParsingTest, SafeParseInt_ValidNegative)
{
    ExpectParseSuccess("-12345", -12345);
    ExpectParseSuccess("-1", -1);
}

TEST_F(SafeParsingTest, SafeParseInt_Overflow)
{
    // Test integer overflow
    std::string tooLarge = "99999999999999999999";
    ExpectParseFailure(tooLarge);

    std::string tooSmall = "-99999999999999999999";
    ExpectParseFailure(tooSmall);
}

TEST_F(SafeParsingTest, SafeParseInt_InvalidCharacters)
{
    ExpectParseFailure("abc");
    ExpectParseFailure("12.34");  // Decimal point
    ExpectParseFailure("12a34");  // Mixed
    ExpectParseFailure("0x123");  // Hex notation
}

TEST_F(SafeParsingTest, SafeParseInt_EmptyString)
{
    ExpectParseFailure("");
}

TEST_F(SafeParsingTest, SafeParseInt_Whitespace)
{
    ExpectParseFailure(" 123");   // Leading space
    ExpectParseFailure("123 ");   // Trailing space
    ExpectParseFailure(" 123 ");  // Both
    ExpectParseFailure("\t123");  // Tab
}

TEST_F(SafeParsingTest, SafeParseInt_SpecialCases)
{
    ExpectParseFailure("+123");       // Leading plus
    ExpectParseFailure("--123");      // Double negative
    ExpectParseFailure("1e5");        // Scientific notation
    ExpectParseFailure("∞");          // Unicode
}

// ============================================================================
// SafeParseFloat Tests
// ============================================================================

TEST_F(SafeParsingTest, SafeParseFloat_ValidPositive)
{
    float result;
    ASSERT_TRUE(Example::SafeParseFloat("3.14159", result));
    EXPECT_FLOAT_EQ(result, 3.14159f);
}

TEST_F(SafeParsingTest, SafeParseFloat_ValidNegative)
{
    float result;
    ASSERT_TRUE(Example::SafeParseFloat("-2.5", result));
    EXPECT_FLOAT_EQ(result, -2.5f);
}

TEST_F(SafeParsingTest, SafeParseFloat_ValidInteger)
{
    float result;
    ASSERT_TRUE(Example::SafeParseFloat("42", result));
    EXPECT_FLOAT_EQ(result, 42.0f);
}

TEST_F(SafeParsingTest, SafeParseFloat_ScientificNotation)
{
    float result;
    ASSERT_TRUE(Example::SafeParseFloat("1.5e-3", result));
    EXPECT_NEAR(result, 0.0015f, 0.00001f);
}

TEST_F(SafeParsingTest, SafeParseFloat_InvalidInput)
{
    float result;
    EXPECT_FALSE(Example::SafeParseFloat("not_a_number", result));
    EXPECT_FALSE(Example::SafeParseFloat("", result));
    EXPECT_FALSE(Example::SafeParseFloat("NaN", result));
}

// ============================================================================
// Performance Tests (Optional)
// ============================================================================

TEST_F(SafeParsingTest, Performance_ParseManyIntegers)
{
    // Ensure parsing is fast enough for real-time use
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 10000; i++)
    {
        int result;
        Example::SafeParseInt("12345", result);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Should parse 10000 integers in less than 100ms
    EXPECT_LT(duration.count(), 100) << "Parsing too slow: " << duration.count() << "ms";
}

// ============================================================================
// Main function (optional - can use gtest_main library instead)
// ============================================================================

// int main(int argc, char** argv)
// {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
