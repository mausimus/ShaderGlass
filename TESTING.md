# Unit Testing Guide for ShaderGlass

This guide covers setting up and writing unit tests for ShaderGlass components.

## Testing Framework: Google Test (Recommended)

We recommend **Google Test (gtest)** for its maturity, Windows support, and Visual Studio integration.

## Setup Instructions

### Option 1: vcpkg (Recommended)

```bash
# Install vcpkg (if not already installed)
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat

# Install Google Test
.\vcpkg\install gtest:x64-windows

# Integrate with Visual Studio
.\vcpkg\integrate install
```

### Option 2: Manual Setup

1. Download Google Test from https://github.com/google/googletest/releases
2. Build or use pre-built binaries
3. Add to project:
   - Include directories: `googletest/include`
   - Library directories: `googletest/lib`
   - Link: `gtest.lib`, `gtest_main.lib`

### Option 3: Git Submodule

```bash
cd ShaderGlass
git submodule add https://github.com/google/googletest.git External/googletest
git submodule update --init --recursive
```

## Project Structure

```
ShaderGlass/
├── ShaderGlass/          # Main application
├── ShaderGC/             # Shader compiler library
├── ShaderGen/            # Code generation tool
├── Tests/                # NEW: Unit tests
│   ├── ShaderGC_Tests/   # Tests for ShaderGC
│   ├── Util_Tests/       # Tests for utilities
│   └── Integration_Tests/# Integration tests
└── TESTING.md            # This file
```

## Creating Test Project

### Visual Studio 2026

1. File → Add → New Project
2. Select "Native Unit Test Project" or "Google Test"
3. Name: `ShaderGlass_Tests`
4. Add references to ShaderGC, ShaderGlass projects
5. Configure test project:
   - C++ Language Standard: C++20
   - Platform: x64
   - Include directories: Add ShaderGC, ShaderGlass paths

## Writing Tests

### Example: Testing SafeParsing Functions

Create `Tests/ShaderGC_Tests/SafeParsingTests.cpp`:

```cpp
#include <gtest/gtest.h>
#include "../../ShaderGC/SafeParsing.h"

// Test fixture for SafeParsing tests
class SafeParsingTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Setup code before each test
    }

    void TearDown() override
    {
        // Cleanup after each test
    }
};

// Test SafeParseInt with valid input
TEST_F(SafeParsingTest, SafeParseInt_ValidInput)
{
    int result;
    bool success = SafeParseInt("12345", result);

    EXPECT_TRUE(success);
    EXPECT_EQ(result, 12345);
}

// Test SafeParseInt with overflow
TEST_F(SafeParsingTest, SafeParseInt_Overflow)
{
    int result;
    bool success = SafeParseInt("999999999999999", result);

    EXPECT_FALSE(success);
}

// Test SafeParseInt with negative numbers
TEST_F(SafeParsingTest, SafeParseInt_Negative)
{
    int result;
    bool success = SafeParseInt("-12345", result);

    EXPECT_TRUE(success);
    EXPECT_EQ(result, -12345);
}

// Test SafeParseInt with invalid input
TEST_F(SafeParsingTest, SafeParseInt_InvalidInput)
{
    int result;
    bool success = SafeParseInt("not_a_number", result);

    EXPECT_FALSE(success);
}

// Test SafeParseFloat
TEST_F(SafeParsingTest, SafeParseFloat_ValidInput)
{
    float result;
    bool success = SafeParseFloat("3.14159", result);

    EXPECT_TRUE(success);
    EXPECT_FLOAT_EQ(result, 3.14159f);
}

// Test path sanitization
TEST(PathSecurityTest, SanitizePath_TraversalAttempt)
{
    std::string malicious = "../../../etc/passwd";
    std::string sanitized = SanitizePath(malicious);

    EXPECT_FALSE(sanitized.find("..") != std::string::npos);
}

TEST(PathSecurityTest, SanitizePath_ValidPath)
{
    std::string valid = "shaders/crt/crt-royale.slangp";
    std::string sanitized = SanitizePath(valid);

    EXPECT_EQ(sanitized, valid);
}
```

### Example: Testing Shader Parameter Handling

Create `Tests/ShaderGlass_Tests/ShaderTests.cpp`:

```cpp
#include <gtest/gtest.h>
#include "../../ShaderGlass/Shader.h"

class ShaderTest : public ::testing::Test
{
protected:
    std::unique_ptr<ShaderDef> shaderDef;

    void SetUp() override
    {
        shaderDef = std::make_unique<ShaderDef>();
        // Initialize with test data
    }
};

TEST_F(ShaderTest, SetParam_UpdatesValue)
{
    Shader shader(*shaderDef);

    ShaderParam param;
    param.name = "test_param";
    param.size = sizeof(float);
    param.buffer = UBO_BUFFER;

    float testValue = 3.14f;
    shader.SetParam(&param, &testValue);

    EXPECT_FLOAT_EQ(param.currentValue, testValue);
}

TEST_F(ShaderTest, DirtyTracking_MarksDirtyOnUpdate)
{
    Shader shader(*shaderDef);

    EXPECT_FALSE(shader.IsDirty(UBO_BUFFER));

    ShaderParam param;
    param.buffer = UBO_BUFFER;
    shader.SetParam(&param, &testValue);

    EXPECT_TRUE(shader.IsDirty(UBO_BUFFER));
}

TEST_F(ShaderTest, DirtyTracking_ClearsDirtyFlag)
{
    Shader shader(*shaderDef);
    shader.MarkDirty(UBO_BUFFER);

    EXPECT_TRUE(shader.IsDirty(UBO_BUFFER));

    shader.ClearDirty(UBO_BUFFER);

    EXPECT_FALSE(shader.IsDirty(UBO_BUFFER));
}
```

### Example: Testing Thread Safety

```cpp
#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include "../../ShaderGlass/CaptureSession.h"

TEST(ThreadSafetyTest, InputFrame_ConcurrentAccess)
{
    // Test that multiple threads can access without crashes
    std::atomic<int> threadsDone{0};
    const int numThreads = 10;

    auto worker = [&]() {
        for(int i = 0; i < 1000; i++)
        {
            // Simulate concurrent frame access
            session.ProcessInput();
        }
        threadsDone++;
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < numThreads; i++)
    {
        threads.emplace_back(worker);
    }

    for(auto& t : threads)
    {
        t.join();
    }

    EXPECT_EQ(threadsDone, numThreads);
}
```

## Running Tests

### Visual Studio

1. **Test Explorer**: View → Test Explorer (Ctrl+E, T)
2. **Run All**: Click "Run All" in Test Explorer
3. **Run Specific**: Right-click test → Run
4. **Debug**: Right-click test → Debug

### Command Line

```bash
# Build tests
msbuild ShaderGlass_Tests.vcxproj /p:Configuration=Debug /p:Platform=x64

# Run tests
.\x64\Debug\ShaderGlass_Tests.exe

# Run with filter
.\x64\Debug\ShaderGlass_Tests.exe --gtest_filter=SafeParsingTest.*

# Run with verbose output
.\x64\Debug\ShaderGlass_Tests.exe --gtest_verbose
```

### GitHub Actions CI

Create `.github/workflows/tests.yml`:

```yaml
name: Unit Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: windows-latest

    steps:
      - uses: actions/checkout@v3

      - name: Setup MSBuild
        uses: microsoft/setup-msbuild@v1

      - name: Install vcpkg dependencies
        run: |
          vcpkg install gtest:x64-windows
          vcpkg integrate install

      - name: Build Tests
        run: msbuild ShaderGlass_Tests.sln /p:Configuration=Release /p:Platform=x64

      - name: Run Tests
        run: .\x64\Release\ShaderGlass_Tests.exe --gtest_output=xml:test_results.xml

      - name: Publish Test Results
        uses: EnricoMi/publish-unit-test-result-action@v2
        if: always()
        with:
          files: test_results.xml
```

## Test Categories

### Unit Tests (Fast, Isolated)
- Test individual functions/classes
- No external dependencies (DirectX, files)
- Mock dependencies
- Should run in < 1 second

### Integration Tests (Slower, Real Dependencies)
- Test component interactions
- May use DirectX, files, etc.
- More realistic scenarios
- Can take several seconds

### Performance Tests
- Measure execution time
- Detect performance regressions
- Compare against baselines

## Best Practices

### DO ✅
- **One assertion per test**: Easy to identify failures
- **Descriptive test names**: `TEST(Class, Method_Scenario_ExpectedBehavior)`
- **Test edge cases**: Empty strings, null pointers, overflow
- **Use fixtures**: Share setup code across tests
- **Mock dependencies**: Isolate unit under test
- **Test error paths**: Not just happy path

### DON'T ❌
- **Test implementation details**: Test behavior, not internals
- **Depend on test order**: Each test should be independent
- **Use real resources**: Mock files, network, DirectX
- **Ignore flaky tests**: Fix or remove unstable tests
- **Skip testing**: Even "simple" code can have bugs

## Assertion Cheat Sheet

```cpp
// Boolean
EXPECT_TRUE(condition);
EXPECT_FALSE(condition);

// Equality
EXPECT_EQ(expected, actual);
EXPECT_NE(val1, val2);

// Comparison
EXPECT_LT(val1, val2);  // Less than
EXPECT_LE(val1, val2);  // Less or equal
EXPECT_GT(val1, val2);  // Greater than
EXPECT_GE(val1, val2);  // Greater or equal

// Floating point
EXPECT_FLOAT_EQ(expected, actual);
EXPECT_DOUBLE_EQ(expected, actual);
EXPECT_NEAR(val1, val2, abs_error);

// Strings
EXPECT_STREQ(str1, str2);
EXPECT_STRNE(str1, str2);

// Exceptions
EXPECT_THROW(statement, exception_type);
EXPECT_NO_THROW(statement);

// Fatal assertions (stops test)
ASSERT_TRUE(condition);   // Use sparingly
ASSERT_EQ(expected, actual);
```

## Mocking with Google Mock

```cpp
#include <gmock/gmock.h>

class MockDevice
{
public:
    MOCK_METHOD(HRESULT, CreateTexture2D, (const D3D11_TEXTURE2D_DESC*, const D3D11_SUBRESOURCE_DATA*, ID3D11Texture2D**));
    MOCK_METHOD(void, CopyResource, (ID3D11Resource*, ID3D11Resource*));
};

TEST(ShaderGlassTest, ProcessFrame_UsesMockDevice)
{
    MockDevice mockDevice;

    // Expect CreateTexture2D to be called once
    EXPECT_CALL(mockDevice, CreateTexture2D(_, _, _))
        .Times(1)
        .WillOnce(Return(S_OK));

    // Test code using mockDevice
}
```

## Code Coverage

### Visual Studio Enterprise

1. Test → Analyze Code Coverage for All Tests
2. View coverage in Code Coverage Results window
3. Aim for > 80% coverage on critical paths

### OpenCppCoverage (Free)

```bash
# Install
choco install opencppcoverage

# Run with coverage
OpenCppCoverage.exe --sources ShaderGC --sources ShaderGlass ^
  -- ShaderGlass_Tests.exe

# Generate HTML report
OpenCppCoverage.exe --export_type html:coverage ^
  --sources ShaderGC -- ShaderGlass_Tests.exe
```

## Priority Test Areas

Based on the audit, prioritize testing:

1. **Security Functions** (SafeParsing, path validation)
2. **Resource Management** (RAII wrappers, COM references)
3. **Thread Safety** (Atomic operations, mutexes)
4. **Shader Parameter Handling** (Dirty tracking, buffer updates)
5. **Error Handling** (Exceptions, HRESULT checks)

## Example Test Plan

| Component | Tests | Priority |
|-----------|-------|----------|
| SafeParsing | 15 tests (int, float, overflow, invalid) | HIGH |
| PathSecurity | 10 tests (traversal, absolute, whitelist) | HIGH |
| Shader | 20 tests (params, dirty flags, buffers) | MEDIUM |
| CaptureSession | 15 tests (threading, frame handling) | MEDIUM |
| ResourceWrappers | 10 tests (RAII, move semantics) | LOW |

## Resources

- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [Google Mock](https://google.github.io/googletest/gmock_for_dummies.html)
- [C++ Testing Best Practices](https://github.com/cpp-best-practices/testing)
- [Visual Studio Testing](https://learn.microsoft.com/en-us/visualstudio/test/getting-started-with-unit-testing)
