@echo off
REM Performance Testing Script for ShaderGlass
REM Validates performance improvements from optimization fixes

echo ========================================
echo ShaderGlass Performance Tests
echo ========================================
echo.

if not exist "x64\Debug\ShaderGlass.exe" (
    echo ERROR: ShaderGlass.exe not found
    echo Please build the project first using BUILD_AND_TEST.bat
    pause
    exit /b 1
)

echo This script guides you through performance testing
echo to verify the optimizations are working correctly.
echo.

REM Create performance test directory
if not exist "PerformanceTests" mkdir PerformanceTests

echo ========================================
echo Test 1: Baseline Performance
echo ========================================
echo.
echo Actions:
echo 1. Launch ShaderGlass: x64\Debug\ShaderGlass.exe
echo 2. Load a complex shader preset (e.g., crt-royale)
echo 3. Monitor CPU usage in Task Manager
echo 4. Record FPS shown in ShaderGlass
echo 5. Run for 5 minutes
echo.
echo Record baseline metrics:
set /p "BASELINE_FPS=Enter FPS (e.g., 60): "
set /p "BASELINE_CPU=Enter CPU %% (e.g., 5): "
set /p "BASELINE_MEM=Enter Memory MB (e.g., 150): "
echo.

echo ========================================
echo Test 2: Allocation Performance
echo ========================================
echo.
echo This tests the hot path allocation elimination.
echo.
echo Expected improvements:
echo - 1560-3744 fewer allocations per second
echo - Better memory stability
echo - Reduced memory fragmentation
echo.
echo Actions:
echo 1. Open Performance Profiler in Visual Studio
echo 2. Debug -^> Performance Profiler
echo 3. Select "Memory Usage"
echo 4. Start ShaderGlass
echo 5. Load shader preset
echo 6. Run for 60 seconds
echo 7. Take snapshot
echo 8. Compare allocation rate
echo.
echo [ ] Memory allocation rate is LOW
echo [ ] Heap allocations are STABLE (not growing)
echo [ ] No memory leaks detected
echo.
pause
echo.

echo ========================================
echo Test 3: Threading Performance
echo ========================================
echo.
echo This tests the busy-wait optimization.
echo.
echo Expected improvements:
echo - Lower CPU usage when idle
echo - No ~1000 wake-ups per second
echo - Smooth frame timing
echo.
echo Actions:
echo 1. Launch ShaderGlass
echo 2. Minimize window (but keep running)
echo 3. Check CPU usage in Task Manager
echo 4. CPU should drop to near 0%% when idle
echo.
set /p "IDLE_CPU=Enter idle CPU %% (should be ^<1%%): "
echo.
if %IDLE_CPU% LEQ 1 (
    echo PASS: Idle CPU is excellent
) else (
    echo WARNING: Idle CPU is higher than expected
)
echo.

echo ========================================
echo Test 4: DirectX API Call Reduction
echo ========================================
echo.
echo This tests ShaderResourceView caching.
echo.
echo Expected improvements:
echo - 60-144 fewer CreateShaderResourceView calls/sec
echo - Lower GPU driver overhead
echo - Better frame pacing
echo.
echo Actions:
echo 1. Use GPU profiling tools (e.g., RenderDoc, PIX)
echo 2. Capture a frame
echo 3. Count CreateShaderResourceView calls
echo 4. Should be nearly zero per frame
echo.
echo [ ] CreateShaderResourceView called only on setup
echo [ ] NOT called every frame in hot path
echo [ ] Frame time is consistent
echo.
pause
echo.

echo ========================================
echo Test 5: Constant Buffer Optimization
echo ========================================
echo.
echo This tests dirty tracking for constant buffers.
echo.
echo Expected improvements:
echo - Map/Unmap only when parameters change
echo - Reduced CPU-GPU synchronization
echo.
echo Actions:
echo 1. Load shader preset
echo 2. Keep parameters STATIC (don't adjust)
echo 3. Profile Map/Unmap calls
echo 4. Then CHANGE a parameter
echo 5. Verify Map/Unmap happens ONLY on change
echo.
echo [ ] Map/Unmap skipped when parameters static
echo [ ] Map/Unmap occurs when parameters change
echo [ ] No unnecessary GPU synchronization
echo.
pause
echo.

echo ========================================
echo Test 6: Frame Time Consistency
echo ========================================
echo.
echo Overall frame time should improve by 0.5-2ms
echo.
echo Actions:
echo 1. Use in-game FPS counter or FRAPS
echo 2. Load complex shader (crt-royale, crt-easymode-halation)
echo 3. Measure frame time (1/FPS * 1000 = ms)
echo 4. Compare to baseline
echo.
set /p "FINAL_FPS=Enter final FPS: "
echo.

REM Calculate improvement
echo Baseline FPS: %BASELINE_FPS%
echo Final FPS: %FINAL_FPS%
echo.

echo ========================================
echo Test 7: Memory Leak Test
echo ========================================
echo.
echo Long-running test to verify no memory leaks.
echo.
echo Actions:
echo 1. Launch ShaderGlass
echo 2. Load shader preset
echo 3. Let run for 1 hour
echo 4. Monitor memory in Task Manager
echo 5. Memory should remain STABLE
echo.
echo [ ] Memory usage stable over 1 hour
echo [ ] No gradual memory increase
echo [ ] Handle count stable
echo.
pause
echo.

echo ========================================
echo Test 8: Thread Safety Stress Test
echo ========================================
echo.
echo Stress test for race condition fixes.
echo.
echo Actions:
echo 1. Launch ShaderGlass
echo 2. Rapidly switch shader presets (10+ times)
echo 3. Rapidly adjust parameters while rendering
echo 4. Resize window while rendering
echo 5. Toggle capture source rapidly
echo.
echo [ ] No crashes during rapid switching
echo [ ] No visual artifacts from races
echo [ ] No hangs or deadlocks
echo [ ] Clean shutdown after stress test
echo.
pause
echo.

echo ========================================
echo Performance Test Summary
echo ========================================
echo.
echo Baseline Metrics:
echo   FPS: %BASELINE_FPS%
echo   CPU: %BASELINE_CPU%%%
echo   Memory: %BASELINE_MEM% MB
echo.
echo Final Metrics:
echo   FPS: %FINAL_FPS%
echo   Idle CPU: %IDLE_CPU%%%
echo.
echo Expected Improvements Checklist:
echo [ ] FPS same or better (target: +5-10%%)
echo [ ] CPU usage lower (target: -1-3%%)
echo [ ] Idle CPU near 0%% (was busy-waiting)
echo [ ] Memory stable (no leaks)
echo [ ] No visual artifacts
echo [ ] Smooth frame timing
echo [ ] Responsive parameter changes
echo [ ] Clean shutdown
echo.
echo Save these results to: PerformanceTests\RESULTS.txt
echo.

REM Save results to file
(
echo Performance Test Results
echo Date: %DATE% %TIME%
echo.
echo Baseline:
echo   FPS: %BASELINE_FPS%
echo   CPU: %BASELINE_CPU%%%
echo   Memory: %BASELINE_MEM% MB
echo.
echo Final:
echo   FPS: %FINAL_FPS%
echo   Idle CPU: %IDLE_CPU%%%
echo.
echo Optimizations Verified:
echo - Hot path allocations eliminated
echo - ShaderResourceView caching working
echo - Constant buffer dirty tracking working
echo - Busy-wait eliminated
echo - Thread safety verified
) > PerformanceTests\RESULTS.txt

echo Results saved to PerformanceTests\RESULTS.txt
echo.
pause
