# ShaderGlass Tests

This directory contains unit tests, integration tests, and performance tests for ShaderGlass.

## Quick Start

```bash
# Build and run all tests
msbuild ShaderGlass_Tests.sln /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\ShaderGlass_Tests.exe

# Run specific test suite
.\x64\Debug\ShaderGlass_Tests.exe --gtest_filter=SafeParsingTest.*

# Run with verbose output
.\x64\Debug\ShaderGlass_Tests.exe --gtest_verbose
```

## Test Structure

```
Tests/
├── ShaderGC_Tests/          # Tests for shader compiler library
│   ├── SafeParsingTests.cpp
│   ├── PresetDefTests.cpp
│   └── ShaderDefTests.cpp
├── ShaderGlass_Tests/       # Tests for main application
│   ├── ShaderTests.cpp
│   ├── CaptureSessionTests.cpp
│   └── ShaderPassTests.cpp
├── Util_Tests/              # Tests for utility functions
│   ├── ErrorHandlingTests.cpp
│   └── PathSecurityTests.cpp
└── Integration_Tests/       # End-to-end integration tests
    └── ShaderPipelineTests.cpp
```

## Test Coverage Goals

- **Security Functions**: 100% coverage
- **Core Rendering**: 80% coverage
- **Utilities**: 90% coverage
- **UI Code**: 50% coverage (lower priority)

## Writing New Tests

1. Choose appropriate directory based on component
2. Create test file: `ComponentNameTests.cpp`
3. Follow naming convention: `TEST(ComponentName, Method_Scenario_ExpectedResult)`
4. Add to test project in Visual Studio
5. Run tests to verify

See [TESTING.md](../TESTING.md) for detailed guidelines.

## CI Integration

Tests run automatically on:
- Every push to main/master
- All pull requests
- Scheduled nightly builds

## Test Results

View test results in:
- Visual Studio Test Explorer
- GitHub Actions workflow runs
- Generated XML reports in `TestResults/`
