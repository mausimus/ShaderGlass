# Static Analysis Guide

This document describes how to run static analysis tools on the ShaderGlass codebase to catch bugs, security issues, and code quality problems.

## Tools Overview

### 1. Clang-Tidy (Recommended)

**Installation:**
```bash
# Clang-Tidy comes with LLVM/Clang
# Download from: https://releases.llvm.org/
# Or via Visual Studio installer (C++ Clang tools for Windows)
```

**Usage:**
```bash
# Analyze a single file
clang-tidy ShaderGlass/ShaderGlass.cpp -- -std=c++20

# Analyze entire project (requires compile_commands.json)
# 1. Generate compilation database with CMake:
#    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
# 2. Run clang-tidy:
run-clang-tidy -p . -header-filter='.*' ShaderGlass/

# Fix issues automatically (use with caution)
clang-tidy -fix ShaderGlass/ShaderGlass.cpp -- -std=c++20
```

**Configuration:**
- `.clang-tidy` file at project root configures checks
- Focus areas: concurrency bugs, performance, modernization

### 2. Visual Studio Code Analysis

**Built-in Static Analysis:**

1. Open `ShaderGlass.sln` in Visual Studio 2026
2. Right-click solution → Properties
3. Code Analysis → Enable "Microsoft C++ Code Analysis"
4. Build → Run Code Analysis on Solution (Alt+F11)

**Recommended Settings:**
- Ruleset: "Microsoft All Rules" or "Microsoft Security Rules"
- Enable "Run on Build" for continuous checking

**Key Rules to Enable:**
- C6011: Dereferencing NULL pointer
- C6031: Return value ignored
- C6387: Parameter validation
- C26400-26499: Core Guidelines checks (lifetime, bounds)
- C26800-26899: Concurrency checks

### 3. PVS-Studio (Commercial, Free for Open Source)

**Installation:**
```
Download from: https://pvs-studio.com/
```

**Usage:**
1. Open solution in Visual Studio with PVS-Studio extension
2. Extensions → PVS-Studio → Check Solution
3. Review findings in "PVS-Studio Output" window

**License:**
- Free for open-source projects (requires comment header)
- Commercial license required for proprietary use

### 4. CppCheck (Free, Open Source)

**Installation:**
```bash
# Download from: https://cppcheck.sourceforge.io/
# Or via chocolatey:
choco install cppcheck
```

**Usage:**
```bash
# Analyze entire project
cppcheck --enable=all --inconclusive --std=c++20 ^
  --suppress=missingIncludeSystem ^
  --suppress=unmatchedSuppression ^
  -I ShaderGlass -I ShaderGC ^
  ShaderGlass/ ShaderGC/ ShaderGen/

# Generate HTML report
cppcheck --enable=all --std=c++20 --xml ^
  ShaderGlass/ 2> cppcheck-report.xml
cppcheck-htmlreport --file=cppcheck-report.xml --report-dir=cppcheck-html
```

## Continuous Integration

### GitHub Actions Example

Create `.github/workflows/static-analysis.yml`:

```yaml
name: Static Analysis

on: [push, pull_request]

jobs:
  clang-tidy:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install LLVM
        run: choco install llvm
      - name: Run clang-tidy
        run: |
          clang-tidy --version
          # Add actual analysis commands
```

## Issue Priority Guide

### CRITICAL (Fix Immediately)
- Null pointer dereferences
- Use-after-free
- Race conditions / data races
- Buffer overflows
- Command injection / path traversal

### HIGH (Fix Soon)
- Resource leaks
- Integer overflows
- Uninitialized variables
- Incorrect error handling
- Missing input validation

### MEDIUM (Fix When Convenient)
- Performance issues
- Code duplication
- Non-const references
- Magic numbers
- Complex functions

### LOW (Nice to Have)
- Style/naming conventions
- Comment formatting
- Modernization suggestions
- Minor readability improvements

## Suppressing False Positives

### Clang-Tidy
```cpp
// NOLINTNEXTLINE(check-name)
potentially_problematic_code();

// NOLINTBEGIN(check-name)
multiple_lines_of_code();
// NOLINTEND(check-name)
```

### Visual Studio
```cpp
#pragma warning(push)
#pragma warning(disable: C6011)
potentially_problematic_code();
#pragma warning(pop)
```

## Best Practices

1. **Run before commits**: Integrate into pre-commit hooks
2. **Fix incrementally**: Address critical issues first
3. **Document suppressions**: Always comment why a warning is suppressed
4. **Regular scans**: Run full analysis weekly
5. **Review together**: Discuss findings during code review

## Resources

- [Clang-Tidy Checks List](https://clang.llvm.org/extra/clang-tidy/checks/list.html)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Visual Studio Code Analysis](https://learn.microsoft.com/en-us/cpp/code-quality/)
- [CppCheck Manual](https://cppcheck.sourceforge.io/manual.pdf)
