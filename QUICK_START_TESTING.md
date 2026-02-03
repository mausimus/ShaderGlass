# Quick Start Testing Guide

Follow these steps in order to validate all audit fixes.

## Prerequisites

- ✅ Visual Studio 2026 installed
- ✅ All source files saved
- ✅ No pending changes to review

## Step-by-Step Process

### 1️⃣ Build the Project (5 minutes)

**Option A: Automated Script**
```bash
# Run the build script
BUILD_AND_TEST.bat

# This will:
# - Find MSBuild automatically
# - Clean previous builds
# - Build Debug x64 configuration
# - Run basic validation checks
# - Report success/failure
```

**Option B: Visual Studio**
```
1. Open ShaderGlass.sln in Visual Studio 2026
2. Select Debug | x64 configuration
3. Build → Rebuild Solution (Ctrl+Shift+B)
4. Check Output window for errors
5. Verify: x64\Debug\ShaderGlass.exe exists
```

**Expected Result:**
- ✅ Build succeeds with 0 errors
- ✅ Warnings are acceptable (document any new ones)
- ✅ Executable created: x64\Debug\ShaderGlass.exe

---

### 2️⃣ Basic Functionality Test (10 minutes)

**Launch and Test:**
```
1. Run: x64\Debug\ShaderGlass.exe
2. Verify window opens without crash
3. Load a shader preset (e.g., CRT-Royale)
4. Adjust a parameter (brightness, contrast, etc.)
5. Toggle capture source (desktop/window)
6. Resize window
7. Close application
```

**Checklist:**
- [ ] Application launches successfully
- [ ] Shader presets load without errors
- [ ] Parameters adjust smoothly
- [ ] No crashes during operation
- [ ] Clean shutdown (no hang)

---

### 3️⃣ Security Testing (15 minutes)

**Run Security Test Script:**
```bash
SECURITY_TESTS.bat
```

**Manual Tests:**

#### Test 1: Path Traversal
```
1. Try to import shader with path: ..\..\..\Windows\System32\cmd.exe
2. Expected: Path rejected with error message
3. Verify: No file access outside allowed directories
```

#### Test 2: Command Injection
```
1. Create shader file named: shader&calc.slangp
2. Try to import it
3. Expected: Special characters escaped, no command execution
4. Verify: Calculator doesn't launch
```

#### Test 3: Integer Overflow
```
1. Try setting texture width to: 9999999999
2. Expected: Value rejected or clamped to max (16384)
3. Verify: No crash or memory corruption
```

#### Test 4: DoS Protection
```
1. Try loading shader with 100+ passes
2. Expected: Rejected with "Too many passes" error
3. Verify: Application remains responsive
```

**Security Checklist:**
- [ ] Path traversal blocked
- [ ] Command injection prevented
- [ ] Integer overflow protected
- [ ] DoS limits enforced
- [ ] No crashes from malicious input

---

### 4️⃣ Performance Testing (20 minutes)

**Run Performance Test Script:**
```bash
PERFORMANCE_TESTS.bat
```

**Manual Tests:**

#### Test 1: Baseline Measurement
```
1. Launch ShaderGlass
2. Load complex shader (crt-royale or crt-easymode-halation)
3. Record FPS (check status bar or overlay)
4. Record CPU % (Task Manager)
5. Record Memory (Task Manager)
```

#### Test 2: Idle CPU Test
```
1. Minimize ShaderGlass (keep running)
2. Wait 30 seconds
3. Check CPU usage in Task Manager
4. Expected: <1% CPU (down from ~3% with busy-wait)
```

#### Test 3: Memory Stability
```
1. Launch ShaderGlass
2. Load shader preset
3. Let run for 30 minutes
4. Check memory every 5 minutes
5. Expected: Memory stays constant (±5 MB)
```

#### Test 4: Stress Test
```
1. Rapidly switch shader presets (10+ times fast)
2. Rapidly adjust parameters while rendering
3. Resize window repeatedly
4. Expected: No crashes, smooth operation
```

**Performance Checklist:**
- [ ] FPS same or better than baseline
- [ ] Idle CPU < 1%
- [ ] Memory stable over time
- [ ] No leaks after 30+ minutes
- [ ] Smooth parameter changes
- [ ] No stuttering or hitching

---

### 5️⃣ Thread Safety Validation (10 minutes)

**Concurrent Operations Test:**
```
1. Launch ShaderGlass
2. Start capture (desktop or window)
3. While rendering, perform ALL these actions rapidly:
   - Switch shader presets
   - Adjust multiple parameters
   - Resize window
   - Change capture source
   - Toggle options
4. Continue for 2-3 minutes
5. Close application
```

**Thread Safety Checklist:**
- [ ] No crashes during concurrent operations
- [ ] No visual artifacts (tearing, corruption)
- [ ] No deadlocks or hangs
- [ ] Parameters update correctly
- [ ] Clean shutdown after stress test

---

### 6️⃣ Code Review Validation (15 minutes)

**Review Documentation:**
```
Read: CODE_REVIEW_CHECKLIST.md
```

**Verify Key Changes:**
```
1. Check Shader.h - dirty tracking added
2. Check ShaderGlass.h - atomics instead of volatile
3. Check CaptureSession.h - mutex added
4. Check ShaderPass.cpp - dirty flag checks
5. Check CaptureManager.cpp - INFINITE wait
```

**Code Review Checklist:**
- [ ] Atomic variables used correctly
- [ ] Mutex usage looks safe (no deadlocks)
- [ ] Cached keys populated correctly
- [ ] No obvious bugs in logic
- [ ] Comments are helpful and accurate

---

## Expected Results Summary

### ✅ What Should Work
- Application builds successfully
- All core functionality works
- Security attacks are blocked
- Performance is same or better
- Thread safety improved
- No crashes or hangs

### ⚠️ Acceptable Issues
- Minor visual differences (from optimizations)
- Debug build might be slightly slower (OK in Release)
- Some compiler warnings (document them)

### 🔴 Blocking Issues
- Application crashes on startup
- Security fixes don't work
- Performance significantly worse
- Frequent crashes or hangs
- Visual corruption

---

## If Tests Fail

### Build Failures
```
1. Check Visual Studio version (need 2026)
2. Check Windows SDK (need 10.0.26100)
3. Check for missing dependencies
4. Review build output for specific errors
5. Post error messages for help
```

### Runtime Crashes
```
1. Run under debugger (F5 in Visual Studio)
2. Note crash location and stack trace
3. Check for null pointers
4. Review recent changes to that file
5. May need to revert specific change
```

### Performance Regressions
```
1. Profile with Performance Profiler
2. Identify slow functions
3. Check if optimization was applied correctly
4. Review dirty tracking logic
5. Verify cache hit rate
```

### Security Test Failures
```
1. Verify security functions are called
2. Add breakpoints in SanitizePath, SafeParseInt, etc.
3. Step through code to see where blocked
4. Check error messages
5. Verify test input is actually malicious
```

---

## Recording Results

### Create Test Report
```
File: TEST_RESULTS.txt

Date: [Fill in]
Tester: [Your name]

Build Test:
✅ Build succeeded
⚠️ Warnings: [List any]

Functionality Test:
✅ All features working
❌ Issues: [List any]

Security Test:
✅ Path traversal blocked
✅ Command injection prevented
✅ Integer overflow protected
✅ DoS limits enforced

Performance Test:
Baseline: XX FPS, XX% CPU
Final: XX FPS, XX% CPU
Improvement: [Better/Same/Worse]

Thread Safety:
✅ No crashes during stress test
✅ Clean shutdown

Overall: ✅ PASS / ⚠️ PASS WITH ISSUES / ❌ FAIL
```

---

## Next Steps After Testing

### If All Tests Pass ✅
1. Proceed to Release build testing
2. Run same tests on Release configuration
3. Consider deployment (see DEPLOYMENT_CHECKLIST.md)
4. Update version number
5. Prepare release notes

### If Some Tests Fail ⚠️
1. Document failing tests
2. Prioritize by severity (crash > performance > visual)
3. Review specific changes for failing areas
4. Consider reverting problematic changes
5. Fix and retest

### If Critical Tests Fail 🔴
1. Do NOT deploy
2. Roll back changes if needed
3. Debug specific failures
4. Review audit fixes in detail
5. Request help if needed

---

## Time Estimate

- Build: 5 minutes
- Functionality: 10 minutes
- Security: 15 minutes
- Performance: 20 minutes
- Thread Safety: 10 minutes
- Code Review: 15 minutes

**Total: ~75 minutes for complete validation**

---

## Support

If you encounter issues:
1. Check error messages carefully
2. Review CODE_REVIEW_CHECKLIST.md
3. Check specific file changes
4. Document the issue clearly
5. Include error messages and steps to reproduce

**Good luck with testing! 🚀**
