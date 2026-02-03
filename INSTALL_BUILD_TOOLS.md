# Installing Build Tools for ShaderGlass

You don't need the full Visual Studio IDE to build ShaderGlass. The free **Build Tools** are sufficient.

## Option 1: Visual Studio Build Tools (Recommended - Free)

### Download & Install
1. Go to: https://visualstudio.microsoft.com/downloads/
2. Scroll down to "Tools for Visual Studio"
3. Download **Build Tools for Visual Studio 2026**
4. Run the installer

### Select Components
When the installer opens, select:
- ✅ **Desktop development with C++**
  - MSVC v145 (or latest)
  - Windows 11 SDK (10.0.26100.0)
  - C++ CMake tools
  - C++ ATL for latest build tools

### Installation Size
- Download: ~2 GB
- Installed: ~7 GB
- Time: 15-30 minutes

### After Installation
MSBuild will be located at:
```
C:\Program Files (x86)\Microsoft Visual Studio\2026\BuildTools\MSBuild\Current\Bin\MSBuild.exe
```

Then you can run:
```bash
BUILD_AND_TEST.bat
```

---

## Option 2: Visual Studio Community (Full IDE - Free)

If you want the full IDE experience:

### Download & Install
1. Go to: https://visualstudio.microsoft.com/vs/community/
2. Download **Visual Studio Community 2026** (free)
3. Run the installer

### Select Workloads
- ✅ **Desktop development with C++**

### After Installation
Open ShaderGlass.sln directly in Visual Studio:
```
1. File → Open → Project/Solution
2. Navigate to ShaderGlass.sln
3. Select Debug | x64
4. Build → Rebuild Solution (Ctrl+Shift+B)
```

---

## Option 3: Use Existing Installation (If You Have VS)

If you already have Visual Studio installed somewhere:

### Find MSBuild Manually
```powershell
# Search for MSBuild
Get-ChildItem -Path "C:\" -Filter "MSBuild.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object FullName
```

### Update BUILD_AND_TEST.bat
Edit line ~13 to point to your MSBuild location:
```batch
set "MSBUILD_PATH=C:\Path\To\Your\MSBuild.exe"
```

---

## Option 4: Review Without Building (Quick Alternative)

If you want to verify the code changes without building:

### Static Code Review
1. **Read CODE_REVIEW_CHECKLIST.md** - Complete review of all changes
2. **Check syntax** - Open files in any text editor
3. **Verify logic** - Review changed lines manually

### Files to Review
```
ShaderGlass/Shader.h                (dirty tracking)
ShaderGlass/Shader.cpp              (mark dirty)
ShaderGlass/ShaderGlass.h           (atomics, cache)
ShaderGlass/ShaderGlass.cpp         (optimizations)
ShaderGlass/ShaderPass.cpp          (dirty checks)
ShaderGlass/CaptureSession.h/.cpp   (mutex)
ShaderGlass/CaptureManager.cpp      (INFINITE wait)
```

### What to Check
- ✅ Syntax looks correct (no obvious typos)
- ✅ Braces match
- ✅ Logic makes sense
- ✅ Thread safety patterns correct

---

## Quick Comparison

| Option | Time | Size | Features |
|--------|------|------|----------|
| **Build Tools** | 30 min | 7 GB | Build only |
| **VS Community** | 45 min | 15 GB | Full IDE |
| **Find Existing** | 5 min | 0 | Use what you have |
| **Review Only** | 15 min | 0 | No build needed |

---

## Recommended Path

**For Testing Code Changes:**
→ Install **Build Tools** (Option 1)

**For Long-term Development:**
→ Install **VS Community** (Option 2)

**For Quick Verification:**
→ **Review Only** (Option 4)

---

## Next Steps After Installation

Once Build Tools or Visual Studio is installed:

```bash
# 1. Open Command Prompt or PowerShell
cd C:\Users\Blake\Documents\GitHub\ShaderGlass

# 2. Run the build script
BUILD_AND_TEST.bat

# 3. If successful, test the executable
x64\Debug\ShaderGlass.exe
```

---

## Already Have Visual Studio Installed Elsewhere?

Let me know and I can help you find it!
