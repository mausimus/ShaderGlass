# Deployment Checklist

Before deploying ShaderGlass with the audit fixes, complete this checklist:

## ✅ Build Verification

- [ ] Clean build succeeds in **Debug** configuration
- [ ] Clean build succeeds in **Release** configuration
- [ ] No compiler warnings in Release build
- [ ] All projects build successfully (ShaderGC, ShaderGen, ShaderGlass)

## ✅ Security Testing

- [ ] Test with malicious shader paths containing `../`
- [ ] Test with very long input strings (>10000 chars)
- [ ] Test with shader files containing special characters
- [ ] Verify temp files created with unique names
- [ ] Verify shader path whitelist is enforced

## ✅ Functionality Testing

- [ ] Capture desktop works correctly
- [ ] Capture window works correctly
- [ ] Shader loading works for all test presets
- [ ] Parameter adjustment works without crashes
- [ ] Cursor capture works
- [ ] Frame rate is stable (no degradation)

## ✅ Performance Testing

- [ ] Measure FPS before/after changes (should be same or better)
- [ ] CPU usage is acceptable (< 5% when idle)
- [ ] Memory usage is stable (no leaks over time)
- [ ] Run for 1+ hour without crashes or degradation

## ✅ Thread Safety Testing

- [ ] No crashes during rapid preset switching
- [ ] No crashes when changing parameters while rendering
- [ ] No visual artifacts from race conditions
- [ ] Clean shutdown without hangs

## ✅ Resource Leak Testing

- [ ] Run under Application Verifier (leak detection)
- [ ] Monitor handle count over time (should be stable)
- [ ] Check for memory leaks with Visual Studio Diagnostic Tools
- [ ] Verify COM reference counts are correct

## ✅ Static Analysis

- [ ] Run Clang-Tidy: `clang-tidy -p . ShaderGlass/*.cpp`
- [ ] Run Visual Studio Code Analysis (Alt+F11)
- [ ] Review and address any HIGH/CRITICAL findings
- [ ] Document any intentional suppressions

## ✅ Documentation

- [ ] Update CHANGELOG.md with all fixes
- [ ] Update version number in About dialog
- [ ] Review README.md for accuracy
- [ ] Ensure all new .md files are included in repository

## ✅ Version Control

- [ ] All changes committed with descriptive messages
- [ ] Branch merged to main/master
- [ ] Tag release: `git tag v1.2.4 -m "Security and performance fixes"`
- [ ] Push tags: `git push --tags`

## ✅ Release Build

- [ ] Compile in Release x64 configuration
- [ ] Strip debug symbols (if desired)
- [ ] Include LICENSE and README in distribution
- [ ] Create installer or zip package
- [ ] Test installer on clean system

## ✅ Distribution

- [ ] Upload to GitHub Releases
- [ ] Update download links
- [ ] Announce changes in release notes
- [ ] Notify users of security fixes

## ✅ Post-Deployment

- [ ] Monitor issue tracker for bug reports
- [ ] Enable logging in beta testing
- [ ] Collect performance metrics
- [ ] Plan next security audit (6 months)

---

## Quick Test Commands

### Build
```bash
msbuild ShaderGlass.sln /p:Configuration=Release /p:Platform=x64 /t:Rebuild
```

### Static Analysis
```bash
clang-tidy ShaderGlass/ShaderGlass.cpp -- -std=c++20
```

### Memory Leak Detection (Visual Studio)
1. Debug → Performance Profiler
2. Select "Memory Usage"
3. Start application
4. Take snapshots during execution
5. Compare snapshots for leaks

### Handle Leak Detection (Process Explorer)
```
1. Download Process Explorer from Microsoft
2. Run ShaderGlass
3. Monitor handle count in Process Explorer
4. Should remain stable over time
```

---

## Known Issues (Not Blocking)

Document any known issues that don't block release:

- [ ] None currently

---

## Sign-off

- [ ] **Developer:** Code changes tested and verified
- [ ] **QA:** All test cases pass
- [ ] **Security:** Security fixes verified
- [ ] **Product Owner:** Release approved

**Release Date:** _______________
**Version:** v1.2.4
**Signed:** _______________
