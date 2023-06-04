# ShaderGlass FAQ

---

### > I'm seeing black only

Make sure you're using __Windows 10, version 2004__ (build 19041) or __Windows 11__.

---

### > I see yellow border around captured screen/window

It's possible to disable yellow border but only on Windows 11. If you are using Windows 11 and still seeing the border,
open Windows Settings app and search for "screenshot borders". Make sure all the options and ShaderGlass are allowed to
remove the border per below:

![screenshot](images/windows-settings.png)

---

### > I can't capture using OBS

OBS can capture ShaderGlass but only as a Game Capture source (not Window Capture). Use "Capture specific window" within
Game Capture to select ShaderGlass window only.

![screenshot](images/obs.png)

---

## > Can I save a profile?

Yes, you can save shader profiles and even load them on startup via a command line parameter. See [README](README.md) for details.

---

## > Can I...

Please read through [README](README.md) to find functionality you are looking for.
