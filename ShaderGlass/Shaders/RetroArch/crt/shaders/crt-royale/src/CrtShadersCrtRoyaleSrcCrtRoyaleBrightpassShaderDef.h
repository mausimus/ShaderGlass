/*
ShaderGlass shader crt-shaders-crt-royale-src\crt-royale-brightpass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/shaders/crt-royale/src/crt-royale-brightpass.slang
See original file for full credits and usage license with excerpts below. 
This file is auto-generated, do not modify directly.

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////
// compatibility macros for transparently converting HLSLisms into GLSLisms
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  These macros and constants can be used across the whole codebase.
//  Unlike the values in user-settings.cgh, end users shouldn't modify these.
//////////////////////////////////  INCLUDES  //////////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
//  IMPORTANT:
//  These constants MUST be set appropriately for the settings in crt-royale.cgp
//  (or whatever related .cgp file you're using).  If they aren't, you're likely
//  to get artifacts, the wrong phosphor mask size, etc.  I wish these could be
//  set directly in the .cgp file to make things easier, but...they can't.
//  PASS SCALES AND RELATED CONSTANTS:
//  Copy the absolute scale_x for BLOOM_APPROX.  There are two major versions of
//  this shader: One does a viewport-scale bloom, and the other skips it.  The
//  latter benefits from a higher bloom_approx_scale_x, so save both separately:
//  Copy the viewport-relative scales of the phosphor mask resize passes
//  (MASK_RESIZE and the pass immediately preceding it):
//  Copy the geom_max_aspect_ratio used to calculate the MASK_RESIZE scales, etc.:
//  PHOSPHOR MASK TEXTURE CONSTANTS:
//  Set the following constants to reflect the properties of the phosphor mask
//  texture named in crt-royale.cgp.  The shader optionally resizes a mask tile
//  based on user settings, then repeats a single tile until filling the screen.
//  The shader must know the input texture size (default 64x64), and to manually
//  resize, it must also know the horizontal triads per tile (default 8).
//  We need the average brightness of the phosphor mask to compensate for the
//  dimming it causes.  The following four values are roughly correct for the
//  masks included with the shader.  Update the value for any LUT texture you
//  change.  [Un]comment "#define PHOSPHOR_MASK_GRILLE14" depending on whether
//  the loaded aperture grille uses 14-pixel or 15-pixel stripes (default 15).
//#define PHOSPHOR_MASK_GRILLE14
//  TileableLinearApertureGrille14Wide7d33Spacing*.png
//  TileableLinearApertureGrille14Wide10And6Spacing*.png
//  TileableLinearApertureGrille15Wide6d33Spacing*.png
//  TileableLinearApertureGrille15Wide8And5d5Spacing*.png
//  TileableLinearSlotMask15Wide9And4d5Horizontal8VerticalSpacing*.png
//  TileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacing*.png
//  TileableLinearShadowMask*.png
//  TileableLinearShadowMaskEDP*.png
///////////////////////////////  FIXED SETTINGS  ///////////////////////////////
//  Avoid dividing by zero; using a macro overloads for float, float2, etc.:
//  Ensure the first pass decodes CRT gamma and the last encodes LCD gamma.
//  Manually tiling a manually resized texture creates texture coord derivative
//  discontinuities and confuses anisotropic filtering, causing discolored tile
//  seams in the phosphor mask.  Workarounds:
//  a.) Using tex2Dlod disables anisotropic filtering for tiled masks.  It's
//      downgraded to tex2Dbias without DRIVERS_ALLOW_TEX2DLOD #defined and
//      disabled without DRIVERS_ALLOW_TEX2DBIAS #defined either.
//  b.) "Tile flat twice" requires drawing two full tiles without border padding
//      to the resized mask FBO, and it's incompatible with same-pass curvature.
//      (Same-pass curvature isn't used but could be in the future...maybe.)
//  c.) "Fix discontinuities" requires derivatives and drawing one tile with
//      border padding to the resized mask FBO, but it works with same-pass
//      curvature.  It's disabled without DRIVERS_ALLOW_DERIVATIVES #defined.
//  Precedence: a, then, b, then c (if multiple strategies are #defined).
//  Also, manually resampling the phosphor mask is slightly blurrier with
//  anisotropic filtering.  (Resampling with mipmapping is even worse: It
//  creates artifacts, but only with the fully bloomed shader.)  The difference
//  is subtle with small triads, but you can fix it for a small cost.
//#define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
//////////////////////////////  DERIVED SETTINGS  //////////////////////////////
//  Intel HD 4000 GPU's can't handle manual mask resizing (for now), setting the
//  geometry mode at runtime, or a 4x4 true Gaussian resize.  Disable
//  incompatible settings ASAP.  (INTEGRATED_GRAPHICS_COMPATIBILITY_MODE may be
//  #defined by either user-settings.h or a wrapper .cg that #includes the
//  current .cg pass.)
//  Mode 2 (4x4 Gaussian resize) won't work, and mode 1 (3x3 blur) is
//  inferior in most cases, so replace 2.0 with 0.0:
//  Disable slow runtime paths if static parameters are used.  Most of these
//  won't be a problem anyway once the params are disabled, but some will.
//  Make tex2Dbias a backup for tex2Dlod for wider compatibility.
//  Rule out unavailable anisotropic compatibility strategies:
//  Prioritize anisotropic tiling compatibility strategies by performance and
//  disable unused strategies.  This concentrates all the nesting in one place.
//  ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE is only compatible with
//  flat texture coords in the same pass, but that's all we use.
//  The tex2Dlod and tex2Dbias strategies share a lot in common, and we can
//  reduce some #ifdef nesting in the next section by essentially OR'ing them:
//  Prioritize anisotropic resampling compatibility strategies the same way:
///////////////////////  DERIVED PHOSPHOR MASK CONSTANTS  //////////////////////
//  If we can use the large mipmapped LUT without mipmapping artifacts, we
//  should: It gives us more options for using fewer samples.
//  TODO: Take advantage of this!
//  tex2D's sampler2D parameter MUST be a uniform global, a uniform input to
//  main_fragment, or a static alias of one of the above.  This makes it hard
//  to select the phosphor mask at runtime: We can't even assign to a uniform
//  global in the vertex shader or select a sampler2D in the vertex shader and
//  pass it to the fragment shader (even with explicit TEXUNIT# bindings),
//  because it just gives us the input texture or a black screen.  However, we
//  can get around these limitations by calling tex2D three times with different
//  uniform samplers (or resizing the phosphor mask three times altogether).
//  With dynamic branches, we can process only one of these branches on top of
//  quickly discarding fragments we don't need (cgc seems able to overcome
//  limigations around dependent texture fetches inside of branches).  Without
//  dynamic branches, we have to process every branch for every fragment...which
//  is slower.  Runtime sampling mode selection is slower without dynamic
//  branches as well.  Let the user's static #defines decide if it's worth it.
//  We need to render some minimum number of tiles in the resize passes.
//  We need at least 1.0 just to repeat a single tile, and we need extra
//  padding beyond that for anisotropic filtering, discontinuitity fixing,
//  antialiasing, same-pass curvature (not currently used), etc.  First
//  determine how many border texels and tiles we need, based on how the result
//  will be sampled:
//  Most antialiasing filters have a base radius of 4.0 pixels:
//  Anisotropic filtering adds about 0.5 to the pixel border:
//  Fixing discontinuities adds 1.0 more to the pixel border:
//  Convert the pixel border to an integer texel border.  Assume same-pass
//  curvature about triples the texel frequency:
//  Convert the texel border to a tile border using worst-case assumptions:
//  Finally, set the number of resized tiles to render to MASK_RESIZE, and set
//  the starting texel (inside borders) for sampling it.
//  Special case: Render two tiles without borders.  Anisotropic
//  filtering doesn't seem to be a problem here.
//  We have to fit mask_resize_num_tiles into an FBO with a viewport scale of
//  mask_resize_viewport_scale.  This limits the maximum final triad size.
//  Estimate the minimum number of triads we can split the screen into in each
//  dimension (we'll be as correct as mask_resize_viewport_scale is):
////////////////////////  COMMON MATHEMATICAL CONSTANTS  ///////////////////////
//  We often want to find the location of the previous texel, e.g.:
//      const float2 curr_texel = uv * texture_size;
//      const float2 prev_texel = floor(curr_texel - float2(0.5)) + float2(0.5);
//      const float2 prev_texel_uv = prev_texel / texture_size;
//  However, many GPU drivers round incorrectly around exact texel locations.
//  We need to subtract a little less than 0.5 before flooring, and some GPU's
//  require this value to be farther from 0.5 than others; define it here.
//      const float2 prev_texel =
//          floor(curr_texel - float2(under_half)) + float2(0.5);
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  These macros and constants can be used across the whole codebase.
//  Unlike the values in user-settings.cgh, end users shouldn't modify these.
//////////////////////////////////  INCLUDES  //////////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
//  IMPORTANT:
//  These constants MUST be set appropriately for the settings in crt-royale.cgp
//  (or whatever related .cgp file you're using).  If they aren't, you're likely
//  to get artifacts, the wrong phosphor mask size, etc.  I wish these could be
//  set directly in the .cgp file to make things easier, but...they can't.
//  PASS SCALES AND RELATED CONSTANTS:
//  Copy the absolute scale_x for BLOOM_APPROX.  There are two major versions of
//  this shader: One does a viewport-scale bloom, and the other skips it.  The
//  latter benefits from a higher bloom_approx_scale_x, so save both separately:
//  Copy the viewport-relative scales of the phosphor mask resize passes
//  (MASK_RESIZE and the pass immediately preceding it):
//  Copy the geom_max_aspect_ratio used to calculate the MASK_RESIZE scales, etc.:
//  PHOSPHOR MASK TEXTURE CONSTANTS:
//  Set the following constants to reflect the properties of the phosphor mask
//  texture named in crt-royale.cgp.  The shader optionally resizes a mask tile
//  based on user settings, then repeats a single tile until filling the screen.
//  The shader must know the input texture size (default 64x64), and to manually
//  resize, it must also know the horizontal triads per tile (default 8).
//  We need the average brightness of the phosphor mask to compensate for the
//  dimming it causes.  The following four values are roughly correct for the
//  masks included with the shader.  Update the value for any LUT texture you
//  change.  [Un]comment "#define PHOSPHOR_MASK_GRILLE14" depending on whether
//  the loaded aperture grille uses 14-pixel or 15-pixel stripes (default 15).
//#define PHOSPHOR_MASK_GRILLE14
//  TileableLinearApertureGrille14Wide7d33Spacing*.png
//  TileableLinearApertureGrille14Wide10And6Spacing*.png
//  TileableLinearApertureGrille15Wide6d33Spacing*.png
//  TileableLinearApertureGrille15Wide8And5d5Spacing*.png
//  TileableLinearSlotMask15Wide9And4d5Horizontal8VerticalSpacing*.png
//  TileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacing*.png
//  TileableLinearShadowMask*.png
//  TileableLinearShadowMaskEDP*.png
///////////////////////////////  FIXED SETTINGS  ///////////////////////////////
//  Avoid dividing by zero; using a macro overloads for float, float2, etc.:
//  Ensure the first pass decodes CRT gamma and the last encodes LCD gamma.
//  Manually tiling a manually resized texture creates texture coord derivative
//  discontinuities and confuses anisotropic filtering, causing discolored tile
//  seams in the phosphor mask.  Workarounds:
//  a.) Using tex2Dlod disables anisotropic filtering for tiled masks.  It's
//      downgraded to tex2Dbias without DRIVERS_ALLOW_TEX2DLOD #defined and
//      disabled without DRIVERS_ALLOW_TEX2DBIAS #defined either.
//  b.) "Tile flat twice" requires drawing two full tiles without border padding
//      to the resized mask FBO, and it's incompatible with same-pass curvature.
//      (Same-pass curvature isn't used but could be in the future...maybe.)
//  c.) "Fix discontinuities" requires derivatives and drawing one tile with
//      border padding to the resized mask FBO, but it works with same-pass
//      curvature.  It's disabled without DRIVERS_ALLOW_DERIVATIVES #defined.
//  Precedence: a, then, b, then c (if multiple strategies are #defined).
//  Also, manually resampling the phosphor mask is slightly blurrier with
//  anisotropic filtering.  (Resampling with mipmapping is even worse: It
//  creates artifacts, but only with the fully bloomed shader.)  The difference
//  is subtle with small triads, but you can fix it for a small cost.
//#define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
//////////////////////////////  DERIVED SETTINGS  //////////////////////////////
//  Intel HD 4000 GPU's can't handle manual mask resizing (for now), setting the
//  geometry mode at runtime, or a 4x4 true Gaussian resize.  Disable
//  incompatible settings ASAP.  (INTEGRATED_GRAPHICS_COMPATIBILITY_MODE may be
//  #defined by either user-settings.h or a wrapper .cg that #includes the
//  current .cg pass.)
//  Mode 2 (4x4 Gaussian resize) won't work, and mode 1 (3x3 blur) is
//  inferior in most cases, so replace 2.0 with 0.0:
//  Disable slow runtime paths if static parameters are used.  Most of these
//  won't be a problem anyway once the params are disabled, but some will.
//  Make tex2Dbias a backup for tex2Dlod for wider compatibility.
//  Rule out unavailable anisotropic compatibility strategies:
//  Prioritize anisotropic tiling compatibility strategies by performance and
//  disable unused strategies.  This concentrates all the nesting in one place.
//  ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE is only compatible with
//  flat texture coords in the same pass, but that's all we use.
//  The tex2Dlod and tex2Dbias strategies share a lot in common, and we can
//  reduce some #ifdef nesting in the next section by essentially OR'ing them:
//  Prioritize anisotropic resampling compatibility strategies the same way:
///////////////////////  DERIVED PHOSPHOR MASK CONSTANTS  //////////////////////
//  If we can use the large mipmapped LUT without mipmapping artifacts, we
//  should: It gives us more options for using fewer samples.
//  TODO: Take advantage of this!
//  tex2D's sampler2D parameter MUST be a uniform global, a uniform input to
//  main_fragment, or a static alias of one of the above.  This makes it hard
//  to select the phosphor mask at runtime: We can't even assign to a uniform
//  global in the vertex shader or select a sampler2D in the vertex shader and
//  pass it to the fragment shader (even with explicit TEXUNIT# bindings),
//  because it just gives us the input texture or a black screen.  However, we
//  can get around these limitations by calling tex2D three times with different
//  uniform samplers (or resizing the phosphor mask three times altogether).
//  With dynamic branches, we can process only one of these branches on top of
//  quickly discarding fragments we don't need (cgc seems able to overcome
//  limigations around dependent texture fetches inside of branches).  Without
//  dynamic branches, we have to process every branch for every fragment...which
//  is slower.  Runtime sampling mode selection is slower without dynamic
//  branches as well.  Let the user's static #defines decide if it's worth it.
//  We need to render some minimum number of tiles in the resize passes.
//  We need at least 1.0 just to repeat a single tile, and we need extra
//  padding beyond that for anisotropic filtering, discontinuitity fixing,
//  antialiasing, same-pass curvature (not currently used), etc.  First
//  determine how many border texels and tiles we need, based on how the result
//  will be sampled:
//  Most antialiasing filters have a base radius of 4.0 pixels:
//  Anisotropic filtering adds about 0.5 to the pixel border:
//  Fixing discontinuities adds 1.0 more to the pixel border:
//  Convert the pixel border to an integer texel border.  Assume same-pass
//  curvature about triples the texel frequency:
//  Convert the texel border to a tile border using worst-case assumptions:
//  Finally, set the number of resized tiles to render to MASK_RESIZE, and set
//  the starting texel (inside borders) for sampling it.
//  Special case: Render two tiles without borders.  Anisotropic
//  filtering doesn't seem to be a problem here.
//  We have to fit mask_resize_num_tiles into an FBO with a viewport scale of
//  mask_resize_viewport_scale.  This limits the maximum final triad size.
//  Estimate the minimum number of triads we can split the screen into in each
//  dimension (we'll be as correct as mask_resize_viewport_scale is):
////////////////////////  COMMON MATHEMATICAL CONSTANTS  ///////////////////////
//  We often want to find the location of the previous texel, e.g.:
//      const float2 curr_texel = uv * texture_size;
//      const float2 prev_texel = floor(curr_texel - float2(0.5)) + float2(0.5);
//      const float2 prev_texel_uv = prev_texel / texture_size;
//  However, many GPU drivers round incorrectly around exact texel locations.
//  We need to subtract a little less than 0.5 before flooring, and some GPU's
//  require this value to be farther from 0.5 than others; define it here.
//      const float2 prev_texel =
//          floor(curr_texel - float2(under_half)) + float2(0.5);
//  Override some parameters for gamma-management.h and tex2Dantialias.h:
//  Disable runtime shader params if the user doesn't explicitly want them.
//  Static constants will be defined in place of uniforms of the same name.
//  Bind option names to shader parameter uniforms or static constants.
//  Use constants from user-settings.h, and limit ranges appropriately:
//  Unpack static vector elements to match scalar uniforms:
//#define interlace_bff global.interlace_bff
//  Provide accessors for vector constants that pack scalar uniforms:
//  Get an aspect ratio vector.  Enforce geom_max_aspect_ratio, and prevent
//  the absolute scale from affecting the uv-mapping for curvature:
//  WARNING: THIS IS EXTREMELY EXPENSIVE.
//  Provide accessors settings which still need "cooking:
///////////////////////////////  VERTEX INCLUDES  ///////////////////////////////
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file provides gamma-aware tex*D*() and encode_output() functions.
//  Requires:   Before #include-ing this file, the including file must #define
//              the following macros when applicable and follow their rules:
//              1.) #define FIRST_PASS if this is the first pass.
//              2.) #define LAST_PASS if this is the last pass.
//              3.) If sRGB is available, set srgb_framebufferN = "true" for
//                  every pass except the last in your .cgp preset.
//              4.) If sRGB isn't available but you want gamma-correctness with
//                  no banding, #define GAMMA_ENCODE_EVERY_FBO each pass.
//              5.) #define SIMULATE_CRT_ON_LCD if desired (precedence over 5-7)
//              6.) #define SIMULATE_GBA_ON_LCD if desired (precedence over 6-7)
//              7.) #define SIMULATE_LCD_ON_CRT if desired (precedence over 7)
//              8.) #define SIMULATE_GBA_ON_CRT if desired (precedence over -)
//              If an option in [5, 8] is #defined in the first or last pass, it
//              should be #defined for both.  It shouldn't make a difference
//              whether it's #defined for intermediate passes or not.
//  Optional:   The including file (or an earlier included file) may optionally
//              #define a number of macros indicating it will override certain
//              macros and associated constants are as follows:
//              static constants with either static or uniform constants.  The
//              1.) OVERRIDE_STANDARD_GAMMA: The user must first define:
//                  static const float ntsc_gamma
//                  static const float pal_gamma
//                  static const float crt_reference_gamma_high
//                  static const float crt_reference_gamma_low
//                  static const float lcd_reference_gamma
//                  static const float crt_office_gamma
//                  static const float lcd_office_gamma
//              2.) OVERRIDE_DEVICE_GAMMA: The user must first define:
//                  static const float crt_gamma
//                  static const float gba_gamma
//                  static const float lcd_gamma
//              3.) OVERRIDE_FINAL_GAMMA: The user must first define:
//                  static const float input_gamma
//                  static const float intermediate_gamma
//                  static const float output_gamma
//                  (intermediate_gamma is for GAMMA_ENCODE_EVERY_FBO.)
//              4.) OVERRIDE_ALPHA_ASSUMPTIONS: The user must first define:
//                  static const bool assume_opaque_alpha
//              The gamma constant overrides must be used in every pass or none,
//              and OVERRIDE_FINAL_GAMMA bypasses all of the SIMULATE* macros.
//              OVERRIDE_ALPHA_ASSUMPTIONS may be set on a per-pass basis.
//  Usage:      After setting macros appropriately, ignore gamma correction and
//              replace all tex*D*() calls with equivalent gamma-aware
//              tex*D*_linearize calls, except:
//              1.) When you read an LUT, use regular tex*D or a gamma-specified
//                  function, depending on its gamma encoding:
//                      tex*D*_linearize_gamma (takes a runtime gamma parameter)
//              2.) If you must read pass0's original input in a later pass, use
//                  tex2D_linearize_ntsc_gamma.  If you want to read pass0's
//                  input with gamma-corrected bilinear filtering, consider
//                  creating a first linearizing pass and reading from the input
//                  of pass1 later.
//              Then, return encode_output(color) from every fragment shader.
//              Finally, use the global gamma_aware_bilinear boolean if you want
//              to statically branch based on whether bilinear filtering is
//              gamma-correct or not (e.g. for placing Gaussian blur samples).
//
//  Detailed Policy:
//  tex*D*_linearize() functions enforce a consistent gamma-management policy
//  based on the FIRST_PASS and GAMMA_ENCODE_EVERY_FBO settings.  They assume
//  their input texture has the same encoding characteristics as the input for
//  the current pass (which doesn't apply to the exceptions listed above).
//  Similarly, encode_output() enforces a policy based on the LAST_PASS and
//  GAMMA_ENCODE_EVERY_FBO settings.  Together, they result in one of the
//  following two pipelines.
//  Typical pipeline with intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = linear_color;     //  Automatic sRGB encoding
//      linear_color = intermediate_output;     //  Automatic sRGB decoding
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Typical pipeline without intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = pow(linear_color, 1.0/intermediate_gamma);
//      linear_color = pow(intermediate_output, intermediate_gamma);
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Using GAMMA_ENCODE_EVERY_FBO is much slower, but it's provided as a way to
//  easily get gamma-correctness without banding on devices where sRGB isn't
//  supported.
//
//  Use This Header to Maximize Code Reuse:
//  The purpose of this header is to provide a consistent interface for texture
//  reads and output gamma-encoding that localizes and abstracts away all the
//  annoying details.  This greatly reduces the amount of code in each shader
//  pass that depends on the pass number in the .cgp preset or whether sRGB
//  FBO's are being used: You can trivially change the gamma behavior of your
//  whole pass by commenting or uncommenting 1-3 #defines.  To reuse the same
//  code in your first, Nth, and last passes, you can even put it all in another
//  header file and #include it from skeleton .cg files that #define the
//  appropriate pass-specific settings.
//
//  Rationale for Using Three Macros:
//  This file uses GAMMA_ENCODE_EVERY_FBO instead of an opposite macro like
//  SRGB_PIPELINE to ensure sRGB is assumed by default, which hopefully imposes
//  a lower maintenance burden on each pass.  At first glance it seems we could
//  accomplish everything with two macros: GAMMA_CORRECT_IN / GAMMA_CORRECT_OUT.
//  This works for simple use cases where input_gamma == output_gamma, but it
//  breaks down for more complex scenarios like CRT simulation, where the pass
//  number determines the gamma encoding of the input and output.
///////////////////////////////  BASE CONSTANTS  ///////////////////////////////
//  Set standard gamma constants, but allow users to override them:
//  Standard encoding gammas:
//  Typical device decoding gammas (only use for emulating devices):
//  CRT/LCD reference gammas are higher than NTSC and Rec.709 video standard
//  gammas: The standards purposely undercorrected for an analog CRT's
//  assumed 2.5 reference display gamma to maintain contrast in assumed
//  [dark] viewing conditions: http://www.poynton.com/PDFs/GammaFAQ.pdf
//  These unstated assumptions about display gamma and perceptual rendering
//  intent caused a lot of confusion, and more modern CRT's seemed to target
//  NTSC 2.2 gamma with circuitry.  LCD displays seem to have followed suit
//  (they struggle near black with 2.5 gamma anyway), especially PC/laptop
//  displays designed to view sRGB in bright environments.  (Standards are
//  also in flux again with BT.1886, but it's underspecified for displays.)
//  Assuming alpha == 1.0 might make it easier for users to avoid some bugs,
//  but only if they're aware of it.
///////////////////////  DERIVED CONSTANTS AS FUNCTIONS  ///////////////////////
//  gamma-management.h should be compatible with overriding gamma values with
//  runtime user parameters, but we can only define other global constants in
//  terms of static constants, not uniform user parameters.  To get around this
//  limitation, we need to define derived constants using functions.
//  Set device gamma constants, but allow users to override them:
//  The user promises to globally define the appropriate constants:
//  Set decoding/encoding gammas for the first/lass passes, but allow overrides:
//  The user promises to globally define the appropriate constants:
//  If we gamma-correct every pass, always use ntsc_gamma between passes to
//  ensure middle passes don't need to care if anything is being simulated:
//  Set decoding/encoding gammas for the current pass.  Use static constants for
//  linearize_input and gamma_encode_output, because they aren't derived, and
//  they let the compiler do dead-code elimination.
//  Users might want to know if bilinear filtering will be gamma-correct:
//////////////////////  COLOR ENCODING/DECODING FUNCTIONS  /////////////////////
//TODO/FIXME: I have no idea why replacing the lookup wrappers with this macro fixes the blurs being offset ¯\_(ツ)_/¯
//#define tex2D_linearize(C, D) decode_input(vec4(texture(C, D)))
// EDIT: it's the 'const' in front of the coords that's doing it
///////////////////////////  TEXTURE LOOKUP WRAPPERS  //////////////////////////
//  "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a wide array of linearizing texture lookup wrapper functions.  The
//  Cg shader spec Retroarch uses only allows for 2D textures, but 1D and 3D
//  lookups are provided for completeness in case that changes someday.  Nobody
//  is likely to use the *fetch and *proj functions, but they're included just
//  in case.  The only tex*D texture sampling functions omitted are:
//      - tex*Dcmpbias
//      - tex*Dcmplod
//      - tex*DARRAY*
//      - tex*DMS*
//      - Variants returning integers
//  Standard line length restrictions are ignored below for vertical brevity.

//  tex1D:
inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

//  tex1Dbias:
inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dbias(tex, tex_coords));   }

inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dbias(tex, tex_coords, texel_off));    }

//  tex1Dfetch:
inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords)
{   return decode_input(tex1Dfetch(tex, tex_coords));  }

inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex1Dfetch(tex, tex_coords, texel_off));   }

//  tex1Dlod:
inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dlod(tex, tex_coords));    }

inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dlod(tex, tex_coords, texel_off));     }

//  tex1Dproj:
inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

//  tex2D:
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//  tex2Dbias:
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords)
//{   return decode_input(tex2Dbias(tex, tex_coords));   }
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dbias(tex, tex_coords, texel_off));    }
//  tex2Dfetch:
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords)
//{   return decode_input(tex2Dfetch(tex, tex_coords));  }
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dfetch(tex, tex_coords, texel_off));   }
//  tex2Dlod:

//  tex2Dproj:
inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }


//  tex3D:
inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords)
{   return decode_input(tex3D(tex, tex_coords));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, texel_off));    }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy)
{   return decode_input(tex3D(tex, tex_coords, dx, dy));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, dx, dy, texel_off));    }

//  tex3Dbias:
inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dbias(tex, tex_coords));   }

inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dbias(tex, tex_coords, texel_off));    }

//  tex3Dfetch:
inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords)
{   return decode_input(tex3Dfetch(tex, tex_coords));  }

inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex3Dfetch(tex, tex_coords, texel_off));   }

//  tex3Dlod:
inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dlod(tex, tex_coords));    }

inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dlod(tex, tex_coords, texel_off));     }

//  tex3Dproj:
inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dproj(tex, tex_coords));   }

inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dproj(tex, tex_coords, texel_off));    }
/////////*

//  NONSTANDARD "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  This narrow selection of nonstandard tex2D* functions can be useful:

//  tex2Dlod0: Automatically fill in the tex2D LOD parameter for mip level 0.
//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0)));   }

//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords, const int texel_off)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0), texel_off));    }


//  MANUALLY LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a narrower selection of tex2D* wrapper functions that decode an
//  input sample with a specified gamma value.  These are useful for reading
//  LUT's and for reading the input of pass0 in a later pass.

//  tex2D:
inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords), gamma);   }

inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords.xy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//  tex2Dbias:
inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords), gamma);   }

inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords, texel_off), gamma);    }

//  tex2Dfetch:
inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords), gamma);  }

inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords, texel_off), gamma);   }

//  tex2Dlod:
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
//////////////////////////////////  INCLUDES  //////////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  These macros and constants can be used across the whole codebase.
//  Unlike the values in user-settings.cgh, end users shouldn't modify these.
//////////////////////////////////  INCLUDES  //////////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
//  IMPORTANT:
//  These constants MUST be set appropriately for the settings in crt-royale.cgp
//  (or whatever related .cgp file you're using).  If they aren't, you're likely
//  to get artifacts, the wrong phosphor mask size, etc.  I wish these could be
//  set directly in the .cgp file to make things easier, but...they can't.
//  PASS SCALES AND RELATED CONSTANTS:
//  Copy the absolute scale_x for BLOOM_APPROX.  There are two major versions of
//  this shader: One does a viewport-scale bloom, and the other skips it.  The
//  latter benefits from a higher bloom_approx_scale_x, so save both separately:
//  Copy the viewport-relative scales of the phosphor mask resize passes
//  (MASK_RESIZE and the pass immediately preceding it):
//  Copy the geom_max_aspect_ratio used to calculate the MASK_RESIZE scales, etc.:
//  PHOSPHOR MASK TEXTURE CONSTANTS:
//  Set the following constants to reflect the properties of the phosphor mask
//  texture named in crt-royale.cgp.  The shader optionally resizes a mask tile
//  based on user settings, then repeats a single tile until filling the screen.
//  The shader must know the input texture size (default 64x64), and to manually
//  resize, it must also know the horizontal triads per tile (default 8).
//  We need the average brightness of the phosphor mask to compensate for the
//  dimming it causes.  The following four values are roughly correct for the
//  masks included with the shader.  Update the value for any LUT texture you
//  change.  [Un]comment "#define PHOSPHOR_MASK_GRILLE14" depending on whether
//  the loaded aperture grille uses 14-pixel or 15-pixel stripes (default 15).
//#define PHOSPHOR_MASK_GRILLE14
//  TileableLinearApertureGrille14Wide7d33Spacing*.png
//  TileableLinearApertureGrille14Wide10And6Spacing*.png
//  TileableLinearApertureGrille15Wide6d33Spacing*.png
//  TileableLinearApertureGrille15Wide8And5d5Spacing*.png
//  TileableLinearSlotMask15Wide9And4d5Horizontal8VerticalSpacing*.png
//  TileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacing*.png
//  TileableLinearShadowMask*.png
//  TileableLinearShadowMaskEDP*.png
///////////////////////////////  FIXED SETTINGS  ///////////////////////////////
//  Avoid dividing by zero; using a macro overloads for float, float2, etc.:
//  Ensure the first pass decodes CRT gamma and the last encodes LCD gamma.
//  Manually tiling a manually resized texture creates texture coord derivative
//  discontinuities and confuses anisotropic filtering, causing discolored tile
//  seams in the phosphor mask.  Workarounds:
//  a.) Using tex2Dlod disables anisotropic filtering for tiled masks.  It's
//      downgraded to tex2Dbias without DRIVERS_ALLOW_TEX2DLOD #defined and
//      disabled without DRIVERS_ALLOW_TEX2DBIAS #defined either.
//  b.) "Tile flat twice" requires drawing two full tiles without border padding
//      to the resized mask FBO, and it's incompatible with same-pass curvature.
//      (Same-pass curvature isn't used but could be in the future...maybe.)
//  c.) "Fix discontinuities" requires derivatives and drawing one tile with
//      border padding to the resized mask FBO, but it works with same-pass
//      curvature.  It's disabled without DRIVERS_ALLOW_DERIVATIVES #defined.
//  Precedence: a, then, b, then c (if multiple strategies are #defined).
//  Also, manually resampling the phosphor mask is slightly blurrier with
//  anisotropic filtering.  (Resampling with mipmapping is even worse: It
//  creates artifacts, but only with the fully bloomed shader.)  The difference
//  is subtle with small triads, but you can fix it for a small cost.
//#define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
//////////////////////////////  DERIVED SETTINGS  //////////////////////////////
//  Intel HD 4000 GPU's can't handle manual mask resizing (for now), setting the
//  geometry mode at runtime, or a 4x4 true Gaussian resize.  Disable
//  incompatible settings ASAP.  (INTEGRATED_GRAPHICS_COMPATIBILITY_MODE may be
//  #defined by either user-settings.h or a wrapper .cg that #includes the
//  current .cg pass.)
//  Mode 2 (4x4 Gaussian resize) won't work, and mode 1 (3x3 blur) is
//  inferior in most cases, so replace 2.0 with 0.0:
//  Disable slow runtime paths if static parameters are used.  Most of these
//  won't be a problem anyway once the params are disabled, but some will.
//  Make tex2Dbias a backup for tex2Dlod for wider compatibility.
//  Rule out unavailable anisotropic compatibility strategies:
//  Prioritize anisotropic tiling compatibility strategies by performance and
//  disable unused strategies.  This concentrates all the nesting in one place.
//  ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE is only compatible with
//  flat texture coords in the same pass, but that's all we use.
//  The tex2Dlod and tex2Dbias strategies share a lot in common, and we can
//  reduce some #ifdef nesting in the next section by essentially OR'ing them:
//  Prioritize anisotropic resampling compatibility strategies the same way:
///////////////////////  DERIVED PHOSPHOR MASK CONSTANTS  //////////////////////
//  If we can use the large mipmapped LUT without mipmapping artifacts, we
//  should: It gives us more options for using fewer samples.
//  TODO: Take advantage of this!
//  tex2D's sampler2D parameter MUST be a uniform global, a uniform input to
//  main_fragment, or a static alias of one of the above.  This makes it hard
//  to select the phosphor mask at runtime: We can't even assign to a uniform
//  global in the vertex shader or select a sampler2D in the vertex shader and
//  pass it to the fragment shader (even with explicit TEXUNIT# bindings),
//  because it just gives us the input texture or a black screen.  However, we
//  can get around these limitations by calling tex2D three times with different
//  uniform samplers (or resizing the phosphor mask three times altogether).
//  With dynamic branches, we can process only one of these branches on top of
//  quickly discarding fragments we don't need (cgc seems able to overcome
//  limigations around dependent texture fetches inside of branches).  Without
//  dynamic branches, we have to process every branch for every fragment...which
//  is slower.  Runtime sampling mode selection is slower without dynamic
//  branches as well.  Let the user's static #defines decide if it's worth it.
//  We need to render some minimum number of tiles in the resize passes.
//  We need at least 1.0 just to repeat a single tile, and we need extra
//  padding beyond that for anisotropic filtering, discontinuitity fixing,
//  antialiasing, same-pass curvature (not currently used), etc.  First
//  determine how many border texels and tiles we need, based on how the result
//  will be sampled:
//  Most antialiasing filters have a base radius of 4.0 pixels:
//  Anisotropic filtering adds about 0.5 to the pixel border:
//  Fixing discontinuities adds 1.0 more to the pixel border:
//  Convert the pixel border to an integer texel border.  Assume same-pass
//  curvature about triples the texel frequency:
//  Convert the texel border to a tile border using worst-case assumptions:
//  Finally, set the number of resized tiles to render to MASK_RESIZE, and set
//  the starting texel (inside borders) for sampling it.
//  Special case: Render two tiles without borders.  Anisotropic
//  filtering doesn't seem to be a problem here.
//  We have to fit mask_resize_num_tiles into an FBO with a viewport scale of
//  mask_resize_viewport_scale.  This limits the maximum final triad size.
//  Estimate the minimum number of triads we can split the screen into in each
//  dimension (we'll be as correct as mask_resize_viewport_scale is):
////////////////////////  COMMON MATHEMATICAL CONSTANTS  ///////////////////////
//  We often want to find the location of the previous texel, e.g.:
//      const float2 curr_texel = uv * texture_size;
//      const float2 prev_texel = floor(curr_texel - float2(0.5)) + float2(0.5);
//      const float2 prev_texel_uv = prev_texel / texture_size;
//  However, many GPU drivers round incorrectly around exact texel locations.
//  We need to subtract a little less than 0.5 before flooring, and some GPU's
//  require this value to be farther from 0.5 than others; define it here.
//      const float2 prev_texel =
//          floor(curr_texel - float2(under_half)) + float2(0.5);
/////////////////////////////  CODEPATH SELECTION  /////////////////////////////
//  Choose a looping strategy based on what's allowed:
//  Dynamic loops not allowed: Use a flat static loop.
//  Dynamic loops accomodated: Coarsely branch around static loops.
//  Dynamic loops assumed allowed: Use a flat dynamic loop.
//////////////////////////////////  CONSTANTS  /////////////////////////////////
//  The larger the resized tile, the fewer samples we'll need for downsizing.
//  See if we can get a static min tile size > mask_min_allowed_tile_size:
//  Limit the number of sinc resize taps by the maximum minification factor:
//  Vectorized loops sample in multiples of 4.  Round up to be safe:
/////////////////////////  RESAMPLING FUNCTION HELPERS  ////////////////////////
//  Requires:   The following global constants must be defined:
//              1.) mask_sinc_lobes
//              2.) max_sinc_resize_samples_m4
//  Returns:    The minimum number of texture samples for a correct downsize
//              at magnification_scale.
//  We're downsizing, so the filter is sized across 2*lobes output pixels
//  (not 2*lobes input texels).  This impacts distance measurements and the
//  minimum number of input samples needed.
//  Simulating loops with branches imposes a 128-sample limit.
//  Requires:   1.) dr == du == 1.0/texture_size.x or
//                  dr == dv == 1.0/texture_size.y
//                  (whichever direction we're resampling in).
//                  It's a scalar to save register space.
//              2.) input_tiles_per_texture_r is the number of input tiles
//                  that can fit in the input texture in the direction we're
//                  resampling this pass.
//              3.) vertical indicates whether we're resampling vertically
//                  this pass (or horizontally).
//  Returns:    Pack and return the first sample's tile_uv coord in [0, 1]
//              and its texel distance from the destination pixel, in the
//              resized dimension only.
//  We'll start with the topmost or leftmost sample and work down or right,
//  so get the first sample location and distance.  Modify both dimensions
//  as if we're doing a one-pass 2D resize; we'll throw away the unneeded
//  (and incorrect) dimension at the end.
//  Convert from tex_uv to tile_uv coords so we can sub fracs for fmods.
//  Project wrapped coordinates to the [0, 1] range.  We'll do this with all
//  samples,but the first texel is special, since it might be negative.
//  Pack the first texel's tile_uv coord and texel distance in 1D:
//return lerp(tile_u_and_dist, tile_v_and_dist, float(vertical));
//  Mipmapping and anisotropic filtering get confused by sinc-resampling.
//  One [slow] workaround is to select the lowest mip level:
//////////////////////////////  LOOP BODY MACROS  //////////////////////////////
//  Using inline functions can exceed the temporary register limit, so we're
//  stuck with #define macros (I'm TRULY sorry).  They're declared here instead
//  of above to be closer to the actual invocation sites.  Steps:
//  1.) Get the exact texel location.
//  2.) Sample the phosphor mask (already assumed encoded in linear RGB).
//  3.) Get the distance from the current pixel and sinc weight:
//          sinc(dist) = sin(pi * dist)/(pi * dist)
//      We can also use the slower/smoother Lanczos instead:
//          L(x) = sinc(dist) * sinc(dist / lobes)
//  4.) Accumulate the weight sum in weights, and accumulate the weighted texels
//      in pixel_color (we'll normalize outside the loop at the end).
//  We vectorize the loop to help reduce the Lanczos window's cost.
//  The r coord is the coord in the dimension we're resizing along (u or v),
//  and first_texel_tile_uv_rrrr is a float4 of the first texel's u or v
//  tile_uv coord in [0, 1].  tex_uv_r will contain the tile_uv u or v coord
//  for four new texel samples.
////////////////////////////  RESAMPLING FUNCTIONS  ////////////////////////////
//  Requires:   1.) dr == du == 1.0/texture_size.x or
//                  dr == dv == 1.0/texture_size.y
//                  (whichever direction we're resampling in).
//                  It's a scalar to save register space.
//              2.) tile_size_uv_r is the number of texels an input tile
//                  takes up in the input texture, in the direction we're
//                  resampling this pass.
//              3.) magnification_scale must be <= 1.0.
//  Returns:    Return a [Lanczos] sinc-resampled pixel of a vertically
//              downsized input tile embedded in an input texture.  (The
//              vertical version is special-cased though: It assumes the
//              tile size equals the [static] texture size, since it's used
//              on an LUT texture input containing one tile.  For more
//              generic use, eliminate the "static" in the parameters.)
//  The "r" in "dr," "tile_size_uv_r," etc. refers to the dimension
//  we're resizing along, e.g. "dy" in this case.
//  A static loop can be faster, but it might blur too much from using
//  more samples than it should.
//  Get the first sample location (scalar tile uv coord along the resized
//  dimension) and distance from the output location (in texels):
//  true = vertical resize:
//  Get the tile sample offset:
//  Sum up each weight and weighted sample color, varying the looping
//  strategy based on our expected dynamic loop capabilities.  See the
//  loop body macros above.
//  Do another 4-sample block for a total of 128 max samples.
//  Normalize so the weight_sum == 1.0, and return:
//  Differences from downsample_horizontal_sinc_tiled:
//  1.) The dr and tile_size_uv_r parameters are not static consts.
//  2.) The "vertical" parameter to get_first_texel_tile_uv_and_dist is
//      set to false instead of true.
//  3.) The horizontal version of the loop body is used.
//  TODO: If we can get guaranteed compile-time dead code elimination,
//  we can combine the vertical/horizontal downsampling functions by:
//  1.) Add an extra static const bool parameter called "vertical.
//  2.) Supply it with the result of get_first_texel_tile_uv_and_dist().
//  3.) Use a conditional assignment in the loop body macro.  This is the
//      tricky part: We DO NOT want to incur the extra conditional
//      assignment in the inner loop at runtime!
//  The "r" in "dr," "tile_size_uv_r," etc. refers to the dimension
//  we're resizing along, e.g. "dx" in this case.
//  If we have to load all samples, we might as well use them.
//  Get the first sample location (scalar tile uv coord along resized
//  dimension) and distance from the output location (in texels):
//  false = horizontal resize:
//  Get the tile sample offset:
//  Sum up each weight and weighted sample color, varying the looping
//  strategy based on our expected dynamic loop capabilities.  See the
//  loop body macros above.
//  Do another 4-sample block for a total of 128 max samples.
//  Normalize so the weight_sum == 1.0, and return:
////////////////////////////  TILE SIZE CALCULATION  ///////////////////////////
//  Requires:   The following global constants must be defined according to
//              certain constraints:
//              1.) mask_resize_num_triads: Must be high enough that our
//                  mask sampling method won't have artifacts later
//                  (long story; see derived-settings-and-constants.h)
//              2.) mask_resize_src_lut_size: Texel size of our mask LUT
//              3.) mask_triads_per_tile: Num horizontal triads in our LUT
//              4.) mask_min_allowed_triad_size: User setting (the more
//                  restrictive it is, the faster the resize will go)
//              5.) mask_min_allowed_tile_size_x < mask_resize_src_lut_size.x
//              6.) mask_triad_size_desired_{runtime, static}
//              7.) mask_num_triads_desired_{runtime, static}
//              8.) mask_specify_num_triads must be 0.0/1.0 (false/true)
//              The function parameters must be defined as follows:
//              1.) estimated_viewport_size == (final viewport size);
//                  If mask_specify_num_triads is 1.0/true and the viewport
//                  estimate is wrong, the number of triads will differ from
//                  the user's preference by about the same factor.
//              2.) estimated_mask_resize_output_size: Must equal the
//                  output size of the MASK_RESIZE pass.
//                  Exception: The x component may be estimated garbage if
//                  and only if the caller throws away the x result.
//              3.) solemnly_swear_same_inputs_for_every_pass: Set to false,
//                  unless you can guarantee that every call across every
//                  pass will use the same sizes for the other parameters.
//              When calling this across multiple passes, always use the
//              same y viewport size/scale, and always use the same x
//              viewport size/scale when using the x result.
//  Returns:    Return the final size of a manually resized mask tile, after
//              constraining the desired size to avoid artifacts.  Under
//              unusual circumstances, tiles may become stretched vertically
//              (see wall of text below).
//  Stated tile properties must be correct:
//  If mask_specify_num_triads is 1.0/true and estimated_viewport_size.x is
//  wrong, the user preference will be misinterpreted:
//  We don't need constraints unless we're sampling MASK_RESIZE.
//  Make sure we're not upsizing:
//  Enforce min_tile_size and max_tile_size in both dimensions:
//  Try to maintain tile_aspect_ratio.  This is the tricky part:
//  If we're currently resizing in the y dimension, the x components
//  could be MEANINGLESS.  (If estimated_mask_resize_output_size.x is
//  bogus, then so is max_tile_size.x and clamped_tile_size.x.)
//  We can't adjust the y size based on clamped_tile_size.x.  If it
//  clamps when it shouldn't, it won't clamp again when later passes
//  call this function with the correct sizes, and the discrepancy will
//  break the sampling coords in MASKED_SCANLINES.  Instead, we'll limit
//  the x size based on the y size, but not vice versa, unless the
//  caller swears the parameters were the same (correct) in every pass.
//  As a result, triads could appear vertically stretched if:
//  a.) mask_resize_src_lut_size.x > mask_resize_src_lut_size.y: Wide
//      LUT's might clamp x more than y (all provided LUT's are square)
//  b.) true_viewport_size.x < true_viewport_size.y: The user is playing
//      with a vertically oriented screen (not accounted for anyway)
//  c.) mask_resize_viewport_scale.x < masked_resize_viewport_scale.y:
//      Viewport scales are equal by default.
//  If any of these are the case, you can fix the stretching by setting:
//      mask_resize_viewport_scale.x = mask_resize_viewport_scale.y *
//          (1.0 / min_expected_aspect_ratio) *
//          (mask_resize_src_lut_size.x / mask_resize_src_lut_size.y)
//  We need integer tile sizes in both directions for tiled sampling to
//  work correctly.  Use floor (to make sure we don't round up), but be
//  careful to avoid a rounding bug where floor decreases whole numbers:
/////////////////////////  FINAL MASK SAMPLING HELPERS  ////////////////////////
//  Requires:   1.) Requirements of get_resized_mask_tile_size() must be
//                  met, particularly regarding global constants.
//              The function parameters must be defined as follows:
//              1.) mask_resize_texture_size == MASK_RESIZE.texture_size
//                  if get_mask_sample_mode() is 0 (otherwise anything)
//              2.) mask_resize_video_size == MASK_RESIZE.video_size
//                  if get_mask_sample_mode() is 0 (otherwise anything)
//              3.) true_viewport_size == IN.output_size for a pass set to
//                  1.0 viewport scale (i.e. it must be correct)
//  Returns:    Return a float4 containing:
//                  xy: tex_uv coords for the start of the mask tile
//                  zw: tex_uv size of the mask tile from start to end
//              mask_tiles_per_screen is an out parameter containing the
//              number of mask tiles that will fit on the screen.
//  First get the final resized tile size.  The viewport size and mask
//  resize viewport scale must be correct, but don't solemnly swear they
//  were correct in both mask resize passes unless you know it's true.
//  (We can better ensure a correct tile aspect ratio if the parameters are
//  guaranteed correct in all passes...but if we lie, we'll get inconsistent
//  sizes across passes, resulting in broken texture coordinates.)
//  Sample MASK_RESIZE: The resized tile is a fraction of the texture
//  size and starts at a nonzero offset to allow for border texels:
//  mask_tiles_per_screen must be based on the *true* viewport size:
//  If we're tiling at the original size (1:1 pixel:texel), redefine a
//  "tile" to be the full texture containing many triads.  Otherwise,
//  we're hardware-resampling an LUT, and the texture truly contains a
//  single unresized phosphor mask tile anyway.
//  Repeat the full LUT at a 1:1 pixel:texel ratio without resizing:
//  Hardware-resize the original LUT:

float2 fix_tiling_discontinuities_normalized(const float2 tile_uv,
float2 duv_dx, float2 duv_dy)
{
//  Requires:   1.) duv_dx == ddx(tile_uv)
//              2.) duv_dy == ddy(tile_uv)
//              3.) tile_uv contains tile-relative uv coords in [0, 1],
//                  such that (0.5, 0.5) is the center of a tile, etc.
//                  ("Tile" can mean texture, the video embedded in the
//                  texture, or some other "tile" embedded in a texture.)
//  Returns:    Return new tile_uv coords that contain no discontinuities
//              across a 2x2 pixel quad.
//  Description:
//  When uv coords wrap from 1.0 to 0.0, they create a discontinuity in the
//  derivatives, which we assume happened if the absolute difference between
//  any fragment in a 2x2 block is > ~half a tile.  If the current block has
//  a u or v discontinuity and the current fragment is in the first half of
//  the tile along that axis (i.e. it wrapped from 1.0 to 0.0), add a tile
//  to that coord to make the 2x2 block continuous.  (It will now have a
//  coord > 1.0 in the padding area beyond the tile.)  This function takes
//  derivatives as parameters so the caller can reuse them.
//  In case we're using high-quality (nVidia-style) derivatives, ensure
//  diagonically opposite fragments see each other for correctness:
duv_dx = abs(duv_dx) + abs(ddy(duv_dx));
duv_dy = abs(duv_dy) + abs(ddx(duv_dy));
const float2 pixel_in_first_half_tile = float2((tile_uv.x < 0.5),(tile_uv.y < 0.5));
const float2 jump_exists = float2(((duv_dx + duv_dy).x > 0.5),((duv_dx + duv_dy).y > 0.5));
return tile_uv + jump_exists * pixel_in_first_half_tile;
}

//  Requires:   1.) tile_uv_wrap contains tile-relative uv coords, where the
//                  tile spans from [0, 1], such that (0.5, 0.5) is at the
//                  tile center.  The input coords can range from [0, inf],
//                  and their fractional parts map to a repeated tile.
//                  ("Tile" can mean texture, the video embedded in the
//                  texture, or some other "tile" embedded in a texture.)
//              2.) mask_tile_start_uv_and_size.xy contains tex_uv coords
//                  for the start of the embedded tile in the full texture.
//              3.) mask_tile_start_uv_and_size.zw contains the [fractional]
//                  tex_uv size of the embedded tile in the full texture.
//  Returns:    Return tex_uv coords (used for texture sampling)
//              corresponding to tile_uv_wrap.
//  Manually repeat the resized mask tile to fill the screen:
//  First get fractional tile_uv coords.  Using frac/fmod on coords
//  confuses anisotropic filtering; fix it as user options dictate.
//  derived-settings-and-constants.h disables incompatible options.
//  The tile is embedded in a padded FBO, and it may start at a
//  nonzero offset if border texels are used to avoid artifacts:
//  Sample from the input phosphor mask texture with hardware tiling.
//  If we're tiling at the original size (mode 2), the "tile" is the
//  whole texture, and it contains a large number of triads mapped with
//  a 1:1 pixel:texel ratio.  OTHERWISE, the texture contains a single
//  unresized tile.  tile_uv_wrap already has correct coords for both!
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
//////////////////////////////////  INCLUDES  //////////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  These macros and constants can be used across the whole codebase.
//  Unlike the values in user-settings.cgh, end users shouldn't modify these.
//////////////////////////////////  INCLUDES  //////////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
//  IMPORTANT:
//  These constants MUST be set appropriately for the settings in crt-royale.cgp
//  (or whatever related .cgp file you're using).  If they aren't, you're likely
//  to get artifacts, the wrong phosphor mask size, etc.  I wish these could be
//  set directly in the .cgp file to make things easier, but...they can't.
//  PASS SCALES AND RELATED CONSTANTS:
//  Copy the absolute scale_x for BLOOM_APPROX.  There are two major versions of
//  this shader: One does a viewport-scale bloom, and the other skips it.  The
//  latter benefits from a higher bloom_approx_scale_x, so save both separately:
//  Copy the viewport-relative scales of the phosphor mask resize passes
//  (MASK_RESIZE and the pass immediately preceding it):
//  Copy the geom_max_aspect_ratio used to calculate the MASK_RESIZE scales, etc.:
//  PHOSPHOR MASK TEXTURE CONSTANTS:
//  Set the following constants to reflect the properties of the phosphor mask
//  texture named in crt-royale.cgp.  The shader optionally resizes a mask tile
//  based on user settings, then repeats a single tile until filling the screen.
//  The shader must know the input texture size (default 64x64), and to manually
//  resize, it must also know the horizontal triads per tile (default 8).
//  We need the average brightness of the phosphor mask to compensate for the
//  dimming it causes.  The following four values are roughly correct for the
//  masks included with the shader.  Update the value for any LUT texture you
//  change.  [Un]comment "#define PHOSPHOR_MASK_GRILLE14" depending on whether
//  the loaded aperture grille uses 14-pixel or 15-pixel stripes (default 15).
//#define PHOSPHOR_MASK_GRILLE14
//  TileableLinearApertureGrille14Wide7d33Spacing*.png
//  TileableLinearApertureGrille14Wide10And6Spacing*.png
//  TileableLinearApertureGrille15Wide6d33Spacing*.png
//  TileableLinearApertureGrille15Wide8And5d5Spacing*.png
//  TileableLinearSlotMask15Wide9And4d5Horizontal8VerticalSpacing*.png
//  TileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacing*.png
//  TileableLinearShadowMask*.png
//  TileableLinearShadowMaskEDP*.png
///////////////////////////////  FIXED SETTINGS  ///////////////////////////////
//  Avoid dividing by zero; using a macro overloads for float, float2, etc.:
//  Ensure the first pass decodes CRT gamma and the last encodes LCD gamma.
//  Manually tiling a manually resized texture creates texture coord derivative
//  discontinuities and confuses anisotropic filtering, causing discolored tile
//  seams in the phosphor mask.  Workarounds:
//  a.) Using tex2Dlod disables anisotropic filtering for tiled masks.  It's
//      downgraded to tex2Dbias without DRIVERS_ALLOW_TEX2DLOD #defined and
//      disabled without DRIVERS_ALLOW_TEX2DBIAS #defined either.
//  b.) "Tile flat twice" requires drawing two full tiles without border padding
//      to the resized mask FBO, and it's incompatible with same-pass curvature.
//      (Same-pass curvature isn't used but could be in the future...maybe.)
//  c.) "Fix discontinuities" requires derivatives and drawing one tile with
//      border padding to the resized mask FBO, but it works with same-pass
//      curvature.  It's disabled without DRIVERS_ALLOW_DERIVATIVES #defined.
//  Precedence: a, then, b, then c (if multiple strategies are #defined).
//  Also, manually resampling the phosphor mask is slightly blurrier with
//  anisotropic filtering.  (Resampling with mipmapping is even worse: It
//  creates artifacts, but only with the fully bloomed shader.)  The difference
//  is subtle with small triads, but you can fix it for a small cost.
//#define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
//////////////////////////////  DERIVED SETTINGS  //////////////////////////////
//  Intel HD 4000 GPU's can't handle manual mask resizing (for now), setting the
//  geometry mode at runtime, or a 4x4 true Gaussian resize.  Disable
//  incompatible settings ASAP.  (INTEGRATED_GRAPHICS_COMPATIBILITY_MODE may be
//  #defined by either user-settings.h or a wrapper .cg that #includes the
//  current .cg pass.)
//  Mode 2 (4x4 Gaussian resize) won't work, and mode 1 (3x3 blur) is
//  inferior in most cases, so replace 2.0 with 0.0:
//  Disable slow runtime paths if static parameters are used.  Most of these
//  won't be a problem anyway once the params are disabled, but some will.
//  Make tex2Dbias a backup for tex2Dlod for wider compatibility.
//  Rule out unavailable anisotropic compatibility strategies:
//  Prioritize anisotropic tiling compatibility strategies by performance and
//  disable unused strategies.  This concentrates all the nesting in one place.
//  ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE is only compatible with
//  flat texture coords in the same pass, but that's all we use.
//  The tex2Dlod and tex2Dbias strategies share a lot in common, and we can
//  reduce some #ifdef nesting in the next section by essentially OR'ing them:
//  Prioritize anisotropic resampling compatibility strategies the same way:
///////////////////////  DERIVED PHOSPHOR MASK CONSTANTS  //////////////////////
//  If we can use the large mipmapped LUT without mipmapping artifacts, we
//  should: It gives us more options for using fewer samples.
//  TODO: Take advantage of this!
//  tex2D's sampler2D parameter MUST be a uniform global, a uniform input to
//  main_fragment, or a static alias of one of the above.  This makes it hard
//  to select the phosphor mask at runtime: We can't even assign to a uniform
//  global in the vertex shader or select a sampler2D in the vertex shader and
//  pass it to the fragment shader (even with explicit TEXUNIT# bindings),
//  because it just gives us the input texture or a black screen.  However, we
//  can get around these limitations by calling tex2D three times with different
//  uniform samplers (or resizing the phosphor mask three times altogether).
//  With dynamic branches, we can process only one of these branches on top of
//  quickly discarding fragments we don't need (cgc seems able to overcome
//  limigations around dependent texture fetches inside of branches).  Without
//  dynamic branches, we have to process every branch for every fragment...which
//  is slower.  Runtime sampling mode selection is slower without dynamic
//  branches as well.  Let the user's static #defines decide if it's worth it.
//  We need to render some minimum number of tiles in the resize passes.
//  We need at least 1.0 just to repeat a single tile, and we need extra
//  padding beyond that for anisotropic filtering, discontinuitity fixing,
//  antialiasing, same-pass curvature (not currently used), etc.  First
//  determine how many border texels and tiles we need, based on how the result
//  will be sampled:
//  Most antialiasing filters have a base radius of 4.0 pixels:
//  Anisotropic filtering adds about 0.5 to the pixel border:
//  Fixing discontinuities adds 1.0 more to the pixel border:
//  Convert the pixel border to an integer texel border.  Assume same-pass
//  curvature about triples the texel frequency:
//  Convert the texel border to a tile border using worst-case assumptions:
//  Finally, set the number of resized tiles to render to MASK_RESIZE, and set
//  the starting texel (inside borders) for sampling it.
//  Special case: Render two tiles without borders.  Anisotropic
//  filtering doesn't seem to be a problem here.
//  We have to fit mask_resize_num_tiles into an FBO with a viewport scale of
//  mask_resize_viewport_scale.  This limits the maximum final triad size.
//  Estimate the minimum number of triads we can split the screen into in each
//  dimension (we'll be as correct as mask_resize_viewport_scale is):
////////////////////////  COMMON MATHEMATICAL CONSTANTS  ///////////////////////
//  We often want to find the location of the previous texel, e.g.:
//      const float2 curr_texel = uv * texture_size;
//      const float2 prev_texel = floor(curr_texel - float2(0.5)) + float2(0.5);
//      const float2 prev_texel_uv = prev_texel / texture_size;
//  However, many GPU drivers round incorrectly around exact texel locations.
//  We need to subtract a little less than 0.5 before flooring, and some GPU's
//  require this value to be farther from 0.5 than others; define it here.
//      const float2 prev_texel =
//          floor(curr_texel - float2(under_half)) + float2(0.5);
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file implements the following mathematical special functions:
//  1.) erf() = 2/sqrt(pi) * indefinite_integral(e**(-x**2))
//  2.) gamma(s), a real-numbered extension of the integer factorial function
//  It also implements normalized_ligamma(s, z), a normalized lower incomplete
//  gamma function for s < 0.5 only.  Both gamma() and normalized_ligamma() can
//  be called with an _impl suffix to use an implementation version with a few
//  extra precomputed parameters (which may be useful for the caller to reuse).
//  See below for details.
//
//  Design Rationale:
//  Pretty much every line of code in this file is duplicated four times for
//  different input types (float4/float3/float2/float).  This is unfortunate,
//  but Cg doesn't allow function templates.  Macros would be far less verbose,
//  but they would make the code harder to document and read.  I don't expect
//  these functions will require a whole lot of maintenance changes unless
//  someone ever has need for more robust incomplete gamma functions, so code
//  duplication seems to be the lesser evil in this case.
///////////////////////////  GAUSSIAN ERROR FUNCTION  //////////////////////////
//  Requires:   x is the standard parameter to erf().
//  Returns:    Return an Abramowitz/Stegun approximation of erf(), where:
//                  erf(x) = 2/sqrt(pi) * integral(e**(-x**2))
//              This approximation has a max absolute error of 2.5*10**-5
//              with solid numerical robustness and efficiency.  See:
//                  https://en.wikipedia.org/wiki/Error_function#Approximation_with_elementary_functions
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   x is the standard parameter to erf().
//  Returns:    Approximate erf() with the hyperbolic tangent.  The error is
//              visually noticeable, but it's blazing fast and perceptually
//              close...at least on ATI hardware.  See:
//                  http://www.maplesoft.com/applications/view.aspx?SID=5525&view=html
//  Warning:    Only use this if your hardware drivers correctly implement
//              tanh(): My nVidia 8800GTS returns garbage output.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   x is the standard parameter to erf().
//  Returns:    Some approximation of erf(x), depending on user settings.
//  Float3 version:
//  Float2 version:
//  Float version:
///////////////////////////  COMPLETE GAMMA FUNCTION  //////////////////////////
//  Requires:   1.) s is the standard parameter to the gamma function, and
//                  it should lie in the [0, 36] range.
//              2.) s_inv = 1.0/s.  This implementation function requires
//                  the caller to precompute this value, giving users the
//                  opportunity to reuse it.
//  Returns:    Return approximate gamma function (real-numbered factorial)
//              output using the Lanczos approximation with two coefficients
//              calculated using Paul Godfrey's method here:
//                  http://my.fit.edu/~gabdo/gamma.txt
//              An optimal g value for s in [0, 36] is ~1.12906830989, with
//              a maximum relative error of 0.000463 for 2**16 equally
//              evals.  We could use three coeffs (0.0000346 error) without
//              hurting latency, but this allows more parallelism with
//              outside instructions.
//  gamma(s + 1) = base**sph * lanczos_sum; divide by s for gamma(s).
//  This has less error for small s's than (s -= 1.0) at the beginning.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   s is the standard parameter to the gamma function, and it
//              should lie in the [0, 36] range.
//  Returns:    Return approximate gamma function output with a maximum
//              relative error of 0.000463.  See gamma_impl for details.
//  Float3 version:
//  Float2 version:
//  Float version:
////////////////  INCOMPLETE GAMMA FUNCTIONS (RESTRICTED INPUT)  ///////////////
//  Lower incomplete gamma function for small s and z (implementation):
//  Requires:   1.) s < ~0.5
//              2.) z <= ~0.775075
//              3.) s_inv = 1.0/s (precomputed for outside reuse)
//  Returns:    A series representation for the lower incomplete gamma
//              function for small s and small z (4 terms).
//  The actual "rolled up" summation looks like:
//      last_sign = 1.0; last_pow = 1.0; last_factorial = 1.0;
//      sum = last_sign * last_pow / ((s + k) * last_factorial)
//      for(int i = 0; i < 4; ++i)
//      {
//          last_sign *= -1.0; last_pow *= z; last_factorial *= i;
//          sum += last_sign * last_pow / ((s + k) * last_factorial);
//      }
//  Unrolled, constant-unfolded and arranged for madds and parallelism:
//  Summation iterations 1, 2, and 3:
//float4 denom4 = 24.0*s + float4(96.0);
//sum += z_sq * z_sq / denom4;
//  Scale and return:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Upper incomplete gamma function for small s and large z (implementation):
//  Requires:   1.) s < ~0.5
//              2.) z > ~0.775075
//  Returns:    Gauss's continued fraction representation for the upper
//              incomplete gamma function (4 terms).
//  The "rolled up" continued fraction looks like this.  The denominator
//  is truncated, and it's calculated "from the bottom up:
//      denom = float4('inf');
//      float4 one = float4(1.0);
//      for(int i = 4; i > 0; --i)
//      {
//          denom = ((i * 2.0) - one) + z - s + (i * (s - i))/denom;
//      }
//  Unrolled and constant-unfolded for madds and parallelism:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Normalized lower incomplete gamma function for small s (implementation):
//  Requires:   1.) s < ~0.5
//              2.) s_inv = 1/s (precomputed for outside reuse)
//              3.) gamma_s_inv = 1/gamma(s) (precomputed for outside reuse)
//  Returns:    Approximate the normalized lower incomplete gamma function
//              for s < 0.5.  Since we only care about s < 0.5, we only need
//              to evaluate two branches (not four) based on z.  Each branch
//              uses four terms, with a max relative error of ~0.00182.  The
//              branch threshold and specifics were adapted for fewer terms
//              from Gil/Segura/Temme's paper here:
//                  http://oai.cwi.nl/oai/asset/20433/20433B.pdf
//  Evaluate both branches: Real branches test slower even when available.
//  Combine the results from both branches:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Normalized lower incomplete gamma function for small s:
//  Requires:   s < ~0.5
//  Returns:    Approximate the normalized lower incomplete gamma function
//              for s < 0.5.  See normalized_ligamma_impl() for details.
//  Float3 version:
//  Float2 version:
//  Float version:
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file provides gamma-aware tex*D*() and encode_output() functions.
//  Requires:   Before #include-ing this file, the including file must #define
//              the following macros when applicable and follow their rules:
//              1.) #define FIRST_PASS if this is the first pass.
//              2.) #define LAST_PASS if this is the last pass.
//              3.) If sRGB is available, set srgb_framebufferN = "true" for
//                  every pass except the last in your .cgp preset.
//              4.) If sRGB isn't available but you want gamma-correctness with
//                  no banding, #define GAMMA_ENCODE_EVERY_FBO each pass.
//              5.) #define SIMULATE_CRT_ON_LCD if desired (precedence over 5-7)
//              6.) #define SIMULATE_GBA_ON_LCD if desired (precedence over 6-7)
//              7.) #define SIMULATE_LCD_ON_CRT if desired (precedence over 7)
//              8.) #define SIMULATE_GBA_ON_CRT if desired (precedence over -)
//              If an option in [5, 8] is #defined in the first or last pass, it
//              should be #defined for both.  It shouldn't make a difference
//              whether it's #defined for intermediate passes or not.
//  Optional:   The including file (or an earlier included file) may optionally
//              #define a number of macros indicating it will override certain
//              macros and associated constants are as follows:
//              static constants with either static or uniform constants.  The
//              1.) OVERRIDE_STANDARD_GAMMA: The user must first define:
//                  static const float ntsc_gamma
//                  static const float pal_gamma
//                  static const float crt_reference_gamma_high
//                  static const float crt_reference_gamma_low
//                  static const float lcd_reference_gamma
//                  static const float crt_office_gamma
//                  static const float lcd_office_gamma
//              2.) OVERRIDE_DEVICE_GAMMA: The user must first define:
//                  static const float crt_gamma
//                  static const float gba_gamma
//                  static const float lcd_gamma
//              3.) OVERRIDE_FINAL_GAMMA: The user must first define:
//                  static const float input_gamma
//                  static const float intermediate_gamma
//                  static const float output_gamma
//                  (intermediate_gamma is for GAMMA_ENCODE_EVERY_FBO.)
//              4.) OVERRIDE_ALPHA_ASSUMPTIONS: The user must first define:
//                  static const bool assume_opaque_alpha
//              The gamma constant overrides must be used in every pass or none,
//              and OVERRIDE_FINAL_GAMMA bypasses all of the SIMULATE* macros.
//              OVERRIDE_ALPHA_ASSUMPTIONS may be set on a per-pass basis.
//  Usage:      After setting macros appropriately, ignore gamma correction and
//              replace all tex*D*() calls with equivalent gamma-aware
//              tex*D*_linearize calls, except:
//              1.) When you read an LUT, use regular tex*D or a gamma-specified
//                  function, depending on its gamma encoding:
//                      tex*D*_linearize_gamma (takes a runtime gamma parameter)
//              2.) If you must read pass0's original input in a later pass, use
//                  tex2D_linearize_ntsc_gamma.  If you want to read pass0's
//                  input with gamma-corrected bilinear filtering, consider
//                  creating a first linearizing pass and reading from the input
//                  of pass1 later.
//              Then, return encode_output(color) from every fragment shader.
//              Finally, use the global gamma_aware_bilinear boolean if you want
//              to statically branch based on whether bilinear filtering is
//              gamma-correct or not (e.g. for placing Gaussian blur samples).
//
//  Detailed Policy:
//  tex*D*_linearize() functions enforce a consistent gamma-management policy
//  based on the FIRST_PASS and GAMMA_ENCODE_EVERY_FBO settings.  They assume
//  their input texture has the same encoding characteristics as the input for
//  the current pass (which doesn't apply to the exceptions listed above).
//  Similarly, encode_output() enforces a policy based on the LAST_PASS and
//  GAMMA_ENCODE_EVERY_FBO settings.  Together, they result in one of the
//  following two pipelines.
//  Typical pipeline with intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = linear_color;     //  Automatic sRGB encoding
//      linear_color = intermediate_output;     //  Automatic sRGB decoding
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Typical pipeline without intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = pow(linear_color, 1.0/intermediate_gamma);
//      linear_color = pow(intermediate_output, intermediate_gamma);
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Using GAMMA_ENCODE_EVERY_FBO is much slower, but it's provided as a way to
//  easily get gamma-correctness without banding on devices where sRGB isn't
//  supported.
//
//  Use This Header to Maximize Code Reuse:
//  The purpose of this header is to provide a consistent interface for texture
//  reads and output gamma-encoding that localizes and abstracts away all the
//  annoying details.  This greatly reduces the amount of code in each shader
//  pass that depends on the pass number in the .cgp preset or whether sRGB
//  FBO's are being used: You can trivially change the gamma behavior of your
//  whole pass by commenting or uncommenting 1-3 #defines.  To reuse the same
//  code in your first, Nth, and last passes, you can even put it all in another
//  header file and #include it from skeleton .cg files that #define the
//  appropriate pass-specific settings.
//
//  Rationale for Using Three Macros:
//  This file uses GAMMA_ENCODE_EVERY_FBO instead of an opposite macro like
//  SRGB_PIPELINE to ensure sRGB is assumed by default, which hopefully imposes
//  a lower maintenance burden on each pass.  At first glance it seems we could
//  accomplish everything with two macros: GAMMA_CORRECT_IN / GAMMA_CORRECT_OUT.
//  This works for simple use cases where input_gamma == output_gamma, but it
//  breaks down for more complex scenarios like CRT simulation, where the pass
//  number determines the gamma encoding of the input and output.
///////////////////////////////  BASE CONSTANTS  ///////////////////////////////
//  Set standard gamma constants, but allow users to override them:
//  Standard encoding gammas:
//  Typical device decoding gammas (only use for emulating devices):
//  CRT/LCD reference gammas are higher than NTSC and Rec.709 video standard
//  gammas: The standards purposely undercorrected for an analog CRT's
//  assumed 2.5 reference display gamma to maintain contrast in assumed
//  [dark] viewing conditions: http://www.poynton.com/PDFs/GammaFAQ.pdf
//  These unstated assumptions about display gamma and perceptual rendering
//  intent caused a lot of confusion, and more modern CRT's seemed to target
//  NTSC 2.2 gamma with circuitry.  LCD displays seem to have followed suit
//  (they struggle near black with 2.5 gamma anyway), especially PC/laptop
//  displays designed to view sRGB in bright environments.  (Standards are
//  also in flux again with BT.1886, but it's underspecified for displays.)
//  Assuming alpha == 1.0 might make it easier for users to avoid some bugs,
//  but only if they're aware of it.
///////////////////////  DERIVED CONSTANTS AS FUNCTIONS  ///////////////////////
//  gamma-management.h should be compatible with overriding gamma values with
//  runtime user parameters, but we can only define other global constants in
//  terms of static constants, not uniform user parameters.  To get around this
//  limitation, we need to define derived constants using functions.
//  Set device gamma constants, but allow users to override them:
//  The user promises to globally define the appropriate constants:
//  Set decoding/encoding gammas for the first/lass passes, but allow overrides:
//  The user promises to globally define the appropriate constants:
//  If we gamma-correct every pass, always use ntsc_gamma between passes to
//  ensure middle passes don't need to care if anything is being simulated:
//  Set decoding/encoding gammas for the current pass.  Use static constants for
//  linearize_input and gamma_encode_output, because they aren't derived, and
//  they let the compiler do dead-code elimination.
//  Users might want to know if bilinear filtering will be gamma-correct:
//////////////////////  COLOR ENCODING/DECODING FUNCTIONS  /////////////////////
//TODO/FIXME: I have no idea why replacing the lookup wrappers with this macro fixes the blurs being offset ¯\_(ツ)_/¯
//#define tex2D_linearize(C, D) decode_input(vec4(texture(C, D)))
// EDIT: it's the 'const' in front of the coords that's doing it
///////////////////////////  TEXTURE LOOKUP WRAPPERS  //////////////////////////
//  "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a wide array of linearizing texture lookup wrapper functions.  The
//  Cg shader spec Retroarch uses only allows for 2D textures, but 1D and 3D
//  lookups are provided for completeness in case that changes someday.  Nobody
//  is likely to use the *fetch and *proj functions, but they're included just
//  in case.  The only tex*D texture sampling functions omitted are:
//      - tex*Dcmpbias
//      - tex*Dcmplod
//      - tex*DARRAY*
//      - tex*DMS*
//      - Variants returning integers
//  Standard line length restrictions are ignored below for vertical brevity.

//  tex1D:
inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

//  tex1Dbias:
inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dbias(tex, tex_coords));   }

inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dbias(tex, tex_coords, texel_off));    }

//  tex1Dfetch:
inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords)
{   return decode_input(tex1Dfetch(tex, tex_coords));  }

inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex1Dfetch(tex, tex_coords, texel_off));   }

//  tex1Dlod:
inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dlod(tex, tex_coords));    }

inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dlod(tex, tex_coords, texel_off));     }

//  tex1Dproj:
inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

//  tex2D:
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//  tex2Dbias:
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords)
//{   return decode_input(tex2Dbias(tex, tex_coords));   }
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dbias(tex, tex_coords, texel_off));    }
//  tex2Dfetch:
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords)
//{   return decode_input(tex2Dfetch(tex, tex_coords));  }
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dfetch(tex, tex_coords, texel_off));   }
//  tex2Dlod:

//  tex2Dproj:
inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }


//  tex3D:
inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords)
{   return decode_input(tex3D(tex, tex_coords));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, texel_off));    }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy)
{   return decode_input(tex3D(tex, tex_coords, dx, dy));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, dx, dy, texel_off));    }

//  tex3Dbias:
inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dbias(tex, tex_coords));   }

inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dbias(tex, tex_coords, texel_off));    }

//  tex3Dfetch:
inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords)
{   return decode_input(tex3Dfetch(tex, tex_coords));  }

inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex3Dfetch(tex, tex_coords, texel_off));   }

//  tex3Dlod:
inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dlod(tex, tex_coords));    }

inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dlod(tex, tex_coords, texel_off));     }

//  tex3Dproj:
inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dproj(tex, tex_coords));   }

inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dproj(tex, tex_coords, texel_off));    }
/////////*

//  NONSTANDARD "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  This narrow selection of nonstandard tex2D* functions can be useful:

//  tex2Dlod0: Automatically fill in the tex2D LOD parameter for mip level 0.
//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0)));   }

//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords, const int texel_off)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0), texel_off));    }


//  MANUALLY LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a narrower selection of tex2D* wrapper functions that decode an
//  input sample with a specified gamma value.  These are useful for reading
//  LUT's and for reading the input of pass0 in a later pass.

//  tex2D:
inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords), gamma);   }

inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords.xy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//  tex2Dbias:
inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords), gamma);   }

inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords, texel_off), gamma);    }

//  tex2Dfetch:
inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords), gamma);  }

inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords, texel_off), gamma);   }

//  tex2Dlod:
/////////////////////////////  SCANLINE FUNCTIONS  /////////////////////////////
//  Requires:   Globals:
//              1.) beam_min_sigma and beam_max_sigma are global floats
//                  containing the desired minimum and maximum beam standard
//                  deviations, for dim and bright colors respectively.
//              2.) beam_max_sigma must be > 0.0
//              3.) beam_min_sigma must be in (0.0, beam_max_sigma]
//              4.) beam_spot_power must be defined as a global float.
//              Parameters:
//              1.) color is the underlying source color along a scanline
//              2.) sigma_range = beam_max_sigma - beam_min_sigma; we take
//                  sigma_range as a parameter to avoid repeated computation
//                  when beam_{min, max}_sigma are runtime shader parameters
//  Optional:   Users may set beam_spot_shape_function to 1 to define the
//              inner f(color) subfunction (see below) as:
//                  f(color) = sqrt(1.0 - (color - 1.0)*(color - 1.0))
//              Otherwise (technically, if beam_spot_shape_function < 0.5):
//                  f(color) = pow(color, beam_spot_power)
//  Returns:    The standard deviation of the Gaussian beam for "color:
//                  sigma = beam_min_sigma + sigma_range * f(color)
//  Details/Discussion:
//  The beam's spot shape vaguely resembles an aspect-corrected f() in the
//  range [0, 1] (not quite, but it's related).  f(color) = color makes
//  spots look like diamonds, and a spherical function or cube balances
//  between variable width and a soft/realistic shape.   A beam_spot_power
//  > 1.0 can produce an ugly spot shape and more initial clipping, but the
//  final shape also differs based on the horizontal resampling filter and
//  the phosphor bloom.  For instance, resampling horizontally in nonlinear
//  light and/or with a sharp (e.g. Lanczos) filter will sharpen the spot
//  shape, but a sixth root is still quite soft.  A power function (default
//  1.0/3.0 beam_spot_power) is most flexible, but a fixed spherical curve
//  has the highest variability without an awful spot shape.
//
//  beam_min_sigma affects scanline sharpness/aliasing in dim areas, and its
//  difference from beam_max_sigma affects beam width variability.  It only
//  affects clipping [for pure Gaussians] if beam_spot_power > 1.0 (which is
//  a conservative estimate for a more complex constraint).
//
//  beam_max_sigma affects clipping and increasing scanline width/softness
//  as color increases.  The wider this is, the more scanlines need to be
//  evaluated to avoid distortion.  For a pure Gaussian, the max_beam_sigma
//  at which the first unused scanline always has a weight < 1.0/255.0 is:
//      num scanlines = 2, max_beam_sigma = 0.2089; distortions begin ~0.34
//      num scanlines = 3, max_beam_sigma = 0.3879; distortions begin ~0.52
//      num scanlines = 4, max_beam_sigma = 0.5723; distortions begin ~0.70
//      num scanlines = 5, max_beam_sigma = 0.7591; distortions begin ~0.89
//      num scanlines = 6, max_beam_sigma = 0.9483; distortions begin ~1.08
//  Generalized Gaussians permit more leeway here as steepness increases.
//  Use a power function:
//  Use a spherical function:
//  Requires:   Globals:
//              1.) beam_min_shape and beam_max_shape are global floats
//                  containing the desired min/max generalized Gaussian
//                  beta parameters, for dim and bright colors respectively.
//              2.) beam_max_shape must be >= 2.0
//              3.) beam_min_shape must be in [2.0, beam_max_shape]
//              4.) beam_shape_power must be defined as a global float.
//              Parameters:
//              1.) color is the underlying source color along a scanline
//              2.) shape_range = beam_max_shape - beam_min_shape; we take
//                  shape_range as a parameter to avoid repeated computation
//                  when beam_{min, max}_shape are runtime shader parameters
//  Returns:    The type-I generalized Gaussian "shape" parameter beta for
//              the given color.
//  Details/Discussion:
//  Beta affects the scanline distribution as follows:
//  a.) beta < 2.0 narrows the peak to a spike with a discontinuous slope
//  b.) beta == 2.0 just degenerates to a Gaussian
//  c.) beta > 2.0 flattens and widens the peak, then drops off more steeply
//      than a Gaussian.  Whereas high sigmas widen and soften peaks, high
//      beta widen and sharpen peaks at the risk of aliasing.
//  Unlike high beam_spot_powers, high beam_shape_powers actually soften shape
//  transitions, whereas lower ones sharpen them (at the risk of aliasing).
//  Requires:   1.) dist is the distance of the [potentially separate R/G/B]
//                  point(s) from a scanline in units of scanlines, where
//                  1.0 means the sample point straddles the next scanline.
//              2.) color is the underlying source color along a scanline.
//              3.) pixel_height is the output pixel height in scanlines.
//              4.) Requirements of get_gaussian_sigma() must be met.
//  Returns:    Return a scanline's light output over a given pixel.
//  Details:
//  The CRT beam profile follows a roughly Gaussian distribution which is
//  wider for bright colors than dark ones.  The integral over the full
//  range of a Gaussian function is always 1.0, so we can vary the beam
//  with a standard deviation without affecting brightness.  'x' = distance:
//      gaussian sample = 1/(sigma*sqrt(2*pi)) * e**(-(x**2)/(2*sigma**2))
//      gaussian integral = 0.5 (1.0 + erf(x/(sigma * sqrt(2))))
//  Use a numerical approximation of the "error function" (the Gaussian
//  indefinite integral) to find the definite integral of the scanline's
//  average brightness over a given pixel area.  Even if curved coords were
//  used in this pass, a flat scalar pixel height works almost as well as a
//  pixel height computed from a full pixel-space to scanline-space matrix.
//  Requires:   1.) Requirements of scanline_gaussian_integral_contrib()
//                  must be met.
//              2.) Requirements of get_gaussian_sigma() must be met.
//              3.) Requirements of get_generalized_gaussian_beta() must be
//                  met.
//  Returns:    Return a scanline's light output over a given pixel.
//  A generalized Gaussian distribution allows the shape (beta) to vary
//  as well as the width (alpha).  "gamma" refers to the gamma function:
//      generalized sample =
//          beta/(2*alpha*gamma(1/beta)) * e**(-(|x|/alpha)**beta)
//  ligamma(s, z) is the lower incomplete gamma function, for which we only
//  implement two of four branches (because we keep 1/beta <= 0.5):
//      generalized integral = 0.5 + 0.5* sign(x) *
//          ligamma(1/beta, (|x|/alpha)**beta)/gamma(1/beta)
//  See get_generalized_gaussian_beta() for a discussion of beta.
//  We base alpha on the intended Gaussian sigma, but it only strictly
//  models models standard deviation at beta == 2, because the standard
//  deviation depends on both alpha and beta (keeping alpha independent is
//  faster and preserves intuitive behavior and a full spectrum of results).
//  Pass beta to gamma_impl to avoid repeated divides.  Similarly pass
//  beta (i.e. 1/s) and 1/gamma(s) to normalized_ligamma_impl.
//  See scanline_gaussian integral_contrib() for detailed comments!
//  gaussian sample = 1/(sigma*sqrt(2*pi)) * e**(-(x**2)/(2*sigma**2))
//  Avoid repeated divides:
//  Sample 1/3 pixel away in each direction as well:
//  Average three pure Gaussian samples:
//  See scanline_generalized_gaussian_integral_contrib() for details!
//  generalized sample =
//      beta/(2*alpha*gamma(1/beta)) * e**(-(|x|/alpha)**beta)
//  Avoid repeated divides:
//  Sample 1/3 pixel closer to and farther from the scanline too.
//  Average three generalized Gaussian samples:
//  Requires:   1.) Requirements of scanline_gaussian_integral_contrib()
//                  must be met.
//              2.) Requirements of get_gaussian_sigma() must be met.
//              3.) Requirements of get_generalized_gaussian_beta() must be
//                  met.
//  Returns:    Return a scanline's light output over a given pixel, using
//              a generalized or pure Gaussian distribution and sampling or
//              integrals as desired by user codepath choices.
//  Use max to avoid bizarre artifacts from negative colors:
//  Requires:   1.) Requirements of include/gamma-management.h must be met:
//                  intermediate_gamma must be globally defined, and input
//                  colors are interpreted as linear RGB unless you #define
//                  GAMMA_ENCODE_EVERY_FBO (in which case they are
//                  interpreted as gamma-encoded with intermediate_gamma).
//              2.) color0-3 are colors sampled from a texture with tex2D().
//                  They are interpreted as defined in requirement 1.
//              3.) weights contains weights for each color, summing to 1.0.
//              4.) beam_horiz_linear_rgb_weight must be defined as a global
//                  float in [0.0, 1.0] describing how much blending should
//                  be done in linear RGB (rest is gamma-corrected RGB).
//              5.) RUNTIME_SCANLINES_HORIZ_FILTER_COLORSPACE must be #defined
//                  if beam_horiz_linear_rgb_weight is anything other than a
//                  static constant, or we may try branching at runtime
//                  without dynamic branches allowed (slow).
//  Returns:    Return an interpolated color lookup between the four input
//              colors based on the weights in weights.  The final color will
//              be a linear RGB value, but the blending will be done as
//              indicated above.
//  Branch if beam_horiz_linear_rgb_weight is static (for free) or if the
//  profile allows dynamic branches (faster than computing extra pows):
//  beam_horiz_linear_rgb_weight is static, so we can branch:
//  Inputs: color0-3 are colors in gamma-encoded RGB.
//  Inputs: color0-3 are colors in linear RGB.
// wtf fixme
//			const float beam_horiz_linear_rgb_weight1 = 1.0;
//  Requires:   1.) scanline_uv must be vertically snapped to the caller's
//                  desired line or scanline and horizontally snapped to the
//                  texel just left of the output pixel (color1)
//              2.) uv_step_x must contain the horizontal uv distance
//                  between texels.
//              3.) weights must contain interpolation filter weights for
//                  color0, color1, color2, and color3, where color1 is just
//                  left of the output pixel.
//  Returns:    Return a horizontally interpolated texture lookup using 2-4
//              nearby texels, according to weights and the conventions of
//              get_interpolated_linear_color().
//  We can ignore the outside texture lookups for Quilez resampling.
//  Sample the texture as-is, whether it's linear or gamma-encoded:
//  get_interpolated_linear_color() will handle the difference.
//  TODO: Add function requirements.
//  Snap to the previous texel and get sample dists from 2/4 nearby texels:
//  Use under_half to fix a rounding bug right around exact texel locations.
//  Get Quilez, Lanczos2, or Gaussian resize weights for 2/4 nearby texels:
//  Quilez:
//  Gaussian:
//  Lanczos2:
//  Ensure the weight sum == 1.0:
//  Get the interpolated horizontal scanline color:
//  TODO: Add function requirements.
//  Rely on a helper to make convergence easier.
//  Compute texture coords for the last/upper scanline, accounting for
//  interlacing: With interlacing, only consider even/odd scanlines every
//  other frame.  Top-field first (TFF) order puts even scanlines on even
//  frames, and BFF order puts them on odd frames.  Texels are centered at:
//      frac(tex_uv * tex_size) == x.5
//  Caution: If these coordinates ever seem incorrect, first make sure it's
//  not because anisotropic filtering is blurring across field boundaries.
//  Note: TFF/BFF won't matter for sources that double-weave or similar.
// wtf fixme
//	const float interlace_bff1 = 1.0;
//  Use under_half to fix a rounding bug right around exact texel locations.
//  Snap to the center of the previous scanline in the current field:
//  Save the sample's distance from the scanline, in units of scanlines:
//  Detect interlacing based on the number of lines in the source.
//  NTSC: 525 lines, 262.5/field; 486 active (2 half-lines), 243/field
//  NTSC Emulators: Typically 224 or 240 lines
//  PAL: 625 lines, 312.5/field; 576 active (typical), 288/field
//  PAL Emulators: ?
//  ATSC: 720p, 1080i, 1080p
//  Where do we place our cutoffs?  Assumptions:
//  1.) We only need to care about active lines.
//  2.) Anything > 288 and <= 576 lines is probably interlaced.
//  3.) Anything > 576 lines is probably not interlaced...
//  4.) ...except 1080 lines, which is a crapshoot (user decision).
//  5.) Just in case the main program uses calculated video sizes,
//      we should nudge the float thresholds a bit.
// copied from bloom-functions.h
//  Requires:   1.) triad_size is the final phosphor triad size in pixels
//              2.) thresh is the max desired pixel difference in the
//                  blurred triad (e.g. 1.0/256.0).
//  Returns:    Return the minimum sigma that will fully blur a phosphor
//              triad on the screen to an even color, within thresh.
//              This closed-form function was found by curve-fitting data.
//  Estimate: max error = ~0.086036, mean sq. error = ~0.0013387:
//  Estimate: max error = ~0.16486, mean sq. error = ~0.0041041:
//return 0.5985*triad_size - triad_size*sqrt(thresh)
//  Our various input textures use different coords:
//video_uv = video_uv;
//  Calculate a runtime bloom_sigma in case it's needed:
//////////////////////////////  FRAGMENT INCLUDES  //////////////////////////////
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  These utility functions and constants help several passes determine the
//  size and center texel weight of the phosphor bloom in a uniform manner.
//////////////////////////////////  INCLUDES  //////////////////////////////////
//  We need to calculate the correct blur sigma using some .cgp constants:
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-royale: A full-featured CRT shader, with cheese.
//  Copyright (C) 2014 TroggleMonkey <trogglemonkey@gmx.com>
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//  Place, Suite 330, Boston, MA 02111-1307 USA
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  These macros and constants can be used across the whole codebase.
//  Unlike the values in user-settings.cgh, end users shouldn't modify these.
//////////////////////////////////  INCLUDES  //////////////////////////////////
/////////////////////////////  DRIVER CAPABILITIES  ////////////////////////////
//  The Cg compiler uses different "profiles" with different capabilities.
//  This shader requires a Cg compilation profile >= arbfp1, but a few options
//  require higher profiles like fp30 or fp40.  The shader can't detect profile
//  or driver capabilities, so instead you must comment or uncomment the lines
//  below with "//" before "#define."  Disable an option if you get compilation
//  errors resembling those listed.  Generally speaking, all of these options
//  will run on nVidia cards, but only DRIVERS_ALLOW_TEX2DBIAS (if that) is
//  likely to run on ATI/AMD, due to the Cg compiler's profile limitations.
//  Derivatives: Unsupported on fp20, ps_1_1, ps_1_2, ps_1_3, and arbfp1.
//  Among other things, derivatives help us fix anisotropic filtering artifacts
//  with curved manually tiled phosphor mask coords.  Related errors:
//  error C3004: function "float2 ddx(float2);" not supported in this profile
//  error C3004: function "float2 ddy(float2);" not supported in this profile
//#define DRIVERS_ALLOW_DERIVATIVES
//  Fine derivatives: Unsupported on older ATI cards.
//  Fine derivatives enable 2x2 fragment block communication, letting us perform
//  fast single-pass blur operations.  If your card uses coarse derivatives and
//  these are enabled, blurs could look broken.  Derivatives are a prerequisite.
//  Dynamic looping: Requires an fp30 or newer profile.
//  This makes phosphor mask resampling faster in some cases.  Related errors:
//  error C5013: profile does not support "for" statements and "for" could not
//  be unrolled
//#define DRIVERS_ALLOW_DYNAMIC_BRANCHES
//  Without DRIVERS_ALLOW_DYNAMIC_BRANCHES, we need to use unrollable loops.
//  Using one static loop avoids overhead if the user is right, but if the user
//  is wrong (loops are allowed), breaking a loop into if-blocked pieces with a
//  binary search can potentially save some iterations.  However, it may fail:
//  error C6001: Temporary register limit of 32 exceeded; 35 registers
//  needed to compile program
//#define ACCOMODATE_POSSIBLE_DYNAMIC_LOOPS
//  tex2Dlod: Requires an fp40 or newer profile.  This can be used to disable
//  anisotropic filtering, thereby fixing related artifacts.  Related errors:
//  error C3004: function "float4 tex2Dlod(sampler2D, float4);" not supported in
//  this profile
//#define DRIVERS_ALLOW_TEX2DLOD
//  tex2Dbias: Requires an fp30 or newer profile.  This can be used to alleviate
//  artifacts from anisotropic filtering and mipmapping.  Related errors:
//  error C3004: function "float4 tex2Dbias(sampler2D, float4);" not supported
//  in this profile
//#define DRIVERS_ALLOW_TEX2DBIAS
//  Integrated graphics compatibility: Integrated graphics like Intel HD 4000
//  impose stricter limitations on register counts and instructions.  Enable
//  INTEGRATED_GRAPHICS_COMPATIBILITY_MODE if you still see error C6001 or:
//  error C6002: Instruction limit of 1024 exceeded: 1523 instructions needed
//  to compile program.
//  Enabling integrated graphics compatibility mode will automatically disable:
//  1.) PHOSPHOR_MASK_MANUALLY_RESIZE: The phosphor mask will be softer.
//      (This may be reenabled in a later release.)
//  2.) RUNTIME_GEOMETRY_MODE
//  3.) The high-quality 4x4 Gaussian resize for the bloom approximation
//#define INTEGRATED_GRAPHICS_COMPATIBILITY_MODE
////////////////////////////  USER CODEPATH OPTIONS  ///////////////////////////
//  To disable a #define option, turn its line into a comment with "//.
//  RUNTIME VS. COMPILE-TIME OPTIONS (Major Performance Implications):
//  Enable runtime shader parameters in the Retroarch (etc.) GUI?  They override
//  many of the options in this file and allow real-time tuning, but many of
//  them are slower.  Disabling them and using this text file will boost FPS.
//  Specify the phosphor bloom sigma at runtime?  This option is 10% slower, but
//  it's the only way to do a wide-enough full bloom with a runtime dot pitch.
//  Specify antialiasing weight parameters at runtime?  (Costs ~20% with cubics)
//  Specify subpixel offsets at runtime? (WARNING: EXTREMELY EXPENSIVE!)
//#define RUNTIME_ANTIALIAS_SUBPIXEL_OFFSETS
//  Make beam_horiz_filter and beam_horiz_linear_rgb_weight into runtime shader
//  parameters?  This will require more math or dynamic branching.
//  Specify the tilt at runtime?  This makes things about 3% slower.
//  Specify the geometry mode at runtime?
//  Specify the phosphor mask type (aperture grille, slot mask, shadow mask) and
//  mode (Lanczos-resize, hardware resize, or tile 1:1) at runtime, even without
//  dynamic branches?  This is cheap if mask_resize_viewport_scale is small.
//  PHOSPHOR MASK:
//  Manually resize the phosphor mask for best results (slower)?  Disabling this
//  removes the option to do so, but it may be faster without dynamic branches.
//  If we sinc-resize the mask, should we Lanczos-window it (slower but better)?
//  Larger blurs are expensive, but we need them to blur larger triads.  We can
//  detect the right blur if the triad size is static or our profile allows
//  dynamic branches, but otherwise we use the largest blur the user indicates
//  they might need:
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_6_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_9_PIXELS
//#define PHOSPHOR_BLOOM_TRIADS_LARGER_THAN_12_PIXELS
//  Here's a helpful chart:
//  MaxTriadSize    BlurSize    MinTriadCountsByResolution
//  3.0             9.0         480/640/960/1920 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  6.0             17.0        240/320/480/960 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  9.0             25.0        160/213/320/640 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  12.0            31.0        120/160/240/480 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
//  18.0            43.0        80/107/160/320 triads at 1080p/1440p/2160p/4320p, 4:3 aspect
///////////////////////////////  USER PARAMETERS  //////////////////////////////
//  Note: Many of these static parameters are overridden by runtime shader
//  parameters when those are enabled.  However, many others are static codepath
//  options that were cleaner or more convert to code as static constants.
//  GAMMA:
//  LEVELS MANAGEMENT:
//  Control the final multiplicative image contrast:
//  We auto-dim to avoid clipping between passes and restore brightness
//  later.  Control the dim factor here: Lower values clip less but crush
//  blacks more (static only for now).
//  HALATION/DIFFUSION/BLOOM:
//  Halation weight: How much energy should be lost to electrons bounding
//  around under the CRT glass and exciting random phosphors?
//  Refractive diffusion weight: How much light should spread/diffuse from
//  refracting through the CRT glass?
//  Underestimate brightness: Bright areas bloom more, but we can base the
//  bloom brightpass on a lower brightness to sharpen phosphors, or a higher
//  brightness to soften them.  Low values clip, but >= 0.8 looks okay.
//  Blur all colors more than necessary for a softer phosphor bloom?
//  The BLOOM_APPROX pass approximates a phosphor blur early on with a small
//  blurred resize of the input (convergence offsets are applied as well).
//  There are three filter options (static option only for now):
//  0.) Bilinear resize: A fast, close approximation to a 4x4 resize
//      if min_allowed_viewport_triads and the BLOOM_APPROX resolution are sane
//      and beam_max_sigma is low.
//  1.) 3x3 resize blur: Medium speed, soft/smeared from bilinear blurring,
//      always uses a static sigma regardless of beam_max_sigma or
//      mask_num_triads_desired.
//  2.) True 4x4 Gaussian resize: Slowest, technically correct.
//  These options are more pronounced for the fast, unbloomed shader version.
//  ELECTRON BEAM SCANLINE DISTRIBUTION:
//  How many scanlines should contribute light to each pixel?  Using more
//  scanlines is slower (especially for a generalized Gaussian) but less
//  distorted with larger beam sigmas (especially for a pure Gaussian).  The
//  max_beam_sigma at which the closest unused weight is guaranteed <
//  1.0/255.0 (for a 3x antialiased pure Gaussian) is:
//      2 scanlines: max_beam_sigma = 0.2089; distortions begin ~0.34; 141.7 FPS pure, 131.9 FPS generalized
//      3 scanlines, max_beam_sigma = 0.3879; distortions begin ~0.52; 137.5 FPS pure; 123.8 FPS generalized
//      4 scanlines, max_beam_sigma = 0.5723; distortions begin ~0.70; 134.7 FPS pure; 117.2 FPS generalized
//      5 scanlines, max_beam_sigma = 0.7591; distortions begin ~0.89; 131.6 FPS pure; 112.1 FPS generalized
//      6 scanlines, max_beam_sigma = 0.9483; distortions begin ~1.08; 127.9 FPS pure; 105.6 FPS generalized
//  A generalized Gaussian beam varies shape with color too, now just width.
//  It's slower but more flexible (static option only for now).
//  What kind of scanline antialiasing do you want?
//  0: Sample weights at 1x; 1: Sample weights at 3x; 2: Compute an integral
//  Integrals are slow (especially for generalized Gaussians) and rarely any
//  better than 3x antialiasing (static option only for now).
//  Min/max standard deviations for scanline beams: Higher values widen and
//  soften scanlines.  Depending on other options, low min sigmas can alias.
//  Beam width varies as a function of color: A power function (0) is more
//  configurable, but a spherical function (1) gives the widest beam
//  variability without aliasing (static option only for now).
//  Spot shape power: Powers <= 1 give smoother spot shapes but lower
//  sharpness.  Powers >= 1.0 are awful unless mix/max sigmas are close.
//  Generalized Gaussian max shape parameters: Higher values give flatter
//  scanline plateaus and steeper dropoffs, simultaneously widening and
//  sharpening scanlines at the cost of aliasing.  2.0 is pure Gaussian, and
//  values > ~40.0 cause artifacts with integrals.
//  Generalized Gaussian shape power: Affects how quickly the distribution
//  changes shape from Gaussian to steep/plateaued as color increases from 0
//  to 1.0.  Higher powers appear softer for most colors, and lower powers
//  appear sharper for most colors.
//  What filter should be used to sample scanlines horizontally?
//  0: Quilez (fast), 1: Gaussian (configurable), 2: Lanczos2 (sharp)
//  Standard deviation for horizontal Gaussian resampling:
//  Do horizontal scanline sampling in linear RGB (correct light mixing),
//  gamma-encoded RGB (darker, hard spot shape, may better match bandwidth-
//  limiting circuitry in some CRT's), or a weighted avg.?
//  Simulate scanline misconvergence?  This needs 3x horizontal texture
//  samples and 3x texture samples of BLOOM_APPROX and HALATION_BLUR in
//  later passes (static option only for now).
//  Convergence offsets in x/y directions for R/G/B scanline beams in units
//  of scanlines.  Positive offsets go right/down; ranges [-2, 2]
//  Detect interlacing (static option only for now)?
//  Assume 1080-line sources are interlaced?
//  For interlaced sources, assume TFF (top-field first) or BFF order?
//  (Whether this matters depends on the nature of the interlaced input.)
//  ANTIALIASING:
//  What AA level do you want for curvature/overscan/subpixels?  Options:
//  0x (none), 1x (sample subpixels), 4x, 5x, 6x, 7x, 8x, 12x, 16x, 20x, 24x
//  (Static option only for now)
//  What antialiasing filter do you want (static option only)?  Options:
//  0: Box (separable), 1: Box (cylindrical),
//  2: Tent (separable), 3: Tent (cylindrical),
//  4: Gaussian (separable), 5: Gaussian (cylindrical),
//  6: Cubic* (separable), 7: Cubic* (cylindrical, poor)
//  8: Lanczos Sinc (separable), 9: Lanczos Jinc (cylindrical, poor)
//      * = Especially slow with RUNTIME_ANTIALIAS_WEIGHTS
//  Flip the sample grid on odd/even frames (static option only for now)?
//  Use RGB subpixel offsets for antialiasing?  The pixel is at green, and
//  the blue offset is the negative r offset; range [0, 0.5]
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Standard deviation for Gaussian antialiasing: Try 0.5/aa_pixel_diameter.
//  PHOSPHOR MASK:
//  Mask type: 0 = aperture grille, 1 = slot mask, 2 = EDP shadow mask
//  We can sample the mask three ways.  Pick 2/3 from: Pretty/Fast/Flexible.
//  0.) Sinc-resize to the desired dot pitch manually (pretty/slow/flexible).
//      This requires PHOSPHOR_MASK_MANUALLY_RESIZE to be #defined.
//  1.) Hardware-resize to the desired dot pitch (ugly/fast/flexible).  This
//      is halfway decent with LUT mipmapping but atrocious without it.
//  2.) Tile it without resizing at a 1:1 texel:pixel ratio for flat coords
//      (pretty/fast/inflexible).  Each input LUT has a fixed dot pitch.
//      This mode reuses the same masks, so triads will be enormous unless
//      you change the mask LUT filenames in your .cgp file.
//  Prefer setting the triad size (0.0) or number on the screen (1.0)?
//  If RUNTIME_PHOSPHOR_BLOOM_SIGMA isn't #defined, the specified triad size
//  will always be used to calculate the full bloom sigma statically.
//  Specify the phosphor triad size, in pixels.  Each tile (usually with 8
//  triads) will be rounded to the nearest integer tile size and clamped to
//  obey minimum size constraints (imposed to reduce downsize taps) and
//  maximum size constraints (imposed to have a sane MASK_RESIZE FBO size).
//  To increase the size limit, double the viewport-relative scales for the
//  two MASK_RESIZE passes in crt-royale.cgp and user-cgp-contants.h.
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  If mask_specify_num_triads is 1.0/true, we'll go by this instead (the
//  final size will be rounded and constrained as above); default 480.0
//  How many lobes should the sinc/Lanczos resizer use?  More lobes require
//  more samples and avoid moire a bit better, but some is unavoidable
//  depending on the destination size (static option for now).
//  The mask is resized using a variable number of taps in each dimension,
//  but some Cg profiles always fetch a constant number of taps no matter
//  what (no dynamic branching).  We can limit the maximum number of taps if
//  we statically limit the minimum phosphor triad size.  Larger values are
//  faster, but the limit IS enforced (static option only, forever);
//      range [1, mask_texture_small_size/mask_triads_per_tile]
//  TODO: Make this 1.0 and compensate with smarter sampling!
//  GEOMETRY:
//  Geometry mode:
//  0: Off (default), 1: Spherical mapping (like cgwg's),
//  2: Alt. spherical mapping (more bulbous), 3: Cylindrical/Trinitron
//  Radius of curvature: Measured in units of your viewport's diagonal size.
//  View dist is the distance from the player to their physical screen, in
//  units of the viewport's diagonal size.  It controls the field of view.
//  Tilt angle in radians (clockwise around up and right vectors):
//  Aspect ratio: When the true viewport size is unknown, this value is used
//  to help convert between the phosphor triad size and count, along with
//  the mask_resize_viewport_scale constant from user-cgp-constants.h.  Set
//  this equal to Retroarch's display aspect ratio (DAR) for best results;
//  range [1, geom_max_aspect_ratio from user-cgp-constants.h];
//  default (256/224)*(54/47) = 1.313069909 (see below)
//  Before getting into overscan, here's some general aspect ratio info:
//  - DAR = display aspect ratio = SAR * PAR; as in your Retroarch setting
//  - SAR = storage aspect ratio = DAR / PAR; square pixel emulator frame AR
//  - PAR = pixel aspect ratio   = DAR / SAR; holds regardless of cropping
//  Geometry processing has to "undo" the screen-space 2D DAR to calculate
//  3D view vectors, then reapplies the aspect ratio to the simulated CRT in
//  uv-space.  To ensure the source SAR is intended for a ~4:3 DAR, either:
//  a.) Enable Retroarch's "Crop Overscan
//  b.) Readd horizontal padding: Set overscan to e.g. N*(1.0, 240.0/224.0)
//  Real consoles use horizontal black padding in the signal, but emulators
//  often crop this without cropping the vertical padding; a 256x224 [S]NES
//  frame (8:7 SAR) is intended for a ~4:3 DAR, but a 256x240 frame is not.
//  The correct [S]NES PAR is 54:47, found by blargg and NewRisingSun:
//      http://board.zsnes.com/phpBB3/viewtopic.php?f=22&t=11928&start=50
//      http://forums.nesdev.com/viewtopic.php?p=24815#p24815
//  For flat output, it's okay to set DAR = [existing] SAR * [correct] PAR
//  without doing a. or b., but horizontal image borders will be tighter
//  than vertical ones, messing up curvature and overscan.  Fixing the
//  padding first corrects this.
//  Overscan: Amount to "zoom in" before cropping.  You can zoom uniformly
//  or adjust x/y independently to e.g. readd horizontal padding, as noted
//  above: Values < 1.0 zoom out; range (0, inf)
//  Compute a proper pixel-space to texture-space matrix even without ddx()/
//  ddy()?  This is ~8.5% slower but improves antialiasing/subpixel filtering
//  with strong curvature (static option only for now).
//  BORDERS:
//  Rounded border size in texture uv coords:
//  Border darkness: Moderate values darken the border smoothly, and high
//  values make the image very dark just inside the border:
//  Border compression: High numbers compress border transitions, narrowing
//  the dark border area.
//  IMPORTANT:
//  These constants MUST be set appropriately for the settings in crt-royale.cgp
//  (or whatever related .cgp file you're using).  If they aren't, you're likely
//  to get artifacts, the wrong phosphor mask size, etc.  I wish these could be
//  set directly in the .cgp file to make things easier, but...they can't.
//  PASS SCALES AND RELATED CONSTANTS:
//  Copy the absolute scale_x for BLOOM_APPROX.  There are two major versions of
//  this shader: One does a viewport-scale bloom, and the other skips it.  The
//  latter benefits from a higher bloom_approx_scale_x, so save both separately:
//  Copy the viewport-relative scales of the phosphor mask resize passes
//  (MASK_RESIZE and the pass immediately preceding it):
//  Copy the geom_max_aspect_ratio used to calculate the MASK_RESIZE scales, etc.:
//  PHOSPHOR MASK TEXTURE CONSTANTS:
//  Set the following constants to reflect the properties of the phosphor mask
//  texture named in crt-royale.cgp.  The shader optionally resizes a mask tile
//  based on user settings, then repeats a single tile until filling the screen.
//  The shader must know the input texture size (default 64x64), and to manually
//  resize, it must also know the horizontal triads per tile (default 8).
//  We need the average brightness of the phosphor mask to compensate for the
//  dimming it causes.  The following four values are roughly correct for the
//  masks included with the shader.  Update the value for any LUT texture you
//  change.  [Un]comment "#define PHOSPHOR_MASK_GRILLE14" depending on whether
//  the loaded aperture grille uses 14-pixel or 15-pixel stripes (default 15).
//#define PHOSPHOR_MASK_GRILLE14
//  TileableLinearApertureGrille14Wide7d33Spacing*.png
//  TileableLinearApertureGrille14Wide10And6Spacing*.png
//  TileableLinearApertureGrille15Wide6d33Spacing*.png
//  TileableLinearApertureGrille15Wide8And5d5Spacing*.png
//  TileableLinearSlotMask15Wide9And4d5Horizontal8VerticalSpacing*.png
//  TileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacing*.png
//  TileableLinearShadowMask*.png
//  TileableLinearShadowMaskEDP*.png
///////////////////////////////  FIXED SETTINGS  ///////////////////////////////
//  Avoid dividing by zero; using a macro overloads for float, float2, etc.:
//  Ensure the first pass decodes CRT gamma and the last encodes LCD gamma.
//  Manually tiling a manually resized texture creates texture coord derivative
//  discontinuities and confuses anisotropic filtering, causing discolored tile
//  seams in the phosphor mask.  Workarounds:
//  a.) Using tex2Dlod disables anisotropic filtering for tiled masks.  It's
//      downgraded to tex2Dbias without DRIVERS_ALLOW_TEX2DLOD #defined and
//      disabled without DRIVERS_ALLOW_TEX2DBIAS #defined either.
//  b.) "Tile flat twice" requires drawing two full tiles without border padding
//      to the resized mask FBO, and it's incompatible with same-pass curvature.
//      (Same-pass curvature isn't used but could be in the future...maybe.)
//  c.) "Fix discontinuities" requires derivatives and drawing one tile with
//      border padding to the resized mask FBO, but it works with same-pass
//      curvature.  It's disabled without DRIVERS_ALLOW_DERIVATIVES #defined.
//  Precedence: a, then, b, then c (if multiple strategies are #defined).
//  Also, manually resampling the phosphor mask is slightly blurrier with
//  anisotropic filtering.  (Resampling with mipmapping is even worse: It
//  creates artifacts, but only with the fully bloomed shader.)  The difference
//  is subtle with small triads, but you can fix it for a small cost.
//#define ANISOTROPIC_RESAMPLING_COMPAT_TEX2DLOD
//////////////////////////////  DERIVED SETTINGS  //////////////////////////////
//  Intel HD 4000 GPU's can't handle manual mask resizing (for now), setting the
//  geometry mode at runtime, or a 4x4 true Gaussian resize.  Disable
//  incompatible settings ASAP.  (INTEGRATED_GRAPHICS_COMPATIBILITY_MODE may be
//  #defined by either user-settings.h or a wrapper .cg that #includes the
//  current .cg pass.)
//  Mode 2 (4x4 Gaussian resize) won't work, and mode 1 (3x3 blur) is
//  inferior in most cases, so replace 2.0 with 0.0:
//  Disable slow runtime paths if static parameters are used.  Most of these
//  won't be a problem anyway once the params are disabled, but some will.
//  Make tex2Dbias a backup for tex2Dlod for wider compatibility.
//  Rule out unavailable anisotropic compatibility strategies:
//  Prioritize anisotropic tiling compatibility strategies by performance and
//  disable unused strategies.  This concentrates all the nesting in one place.
//  ANISOTROPIC_TILING_COMPAT_TILE_FLAT_TWICE is only compatible with
//  flat texture coords in the same pass, but that's all we use.
//  The tex2Dlod and tex2Dbias strategies share a lot in common, and we can
//  reduce some #ifdef nesting in the next section by essentially OR'ing them:
//  Prioritize anisotropic resampling compatibility strategies the same way:
///////////////////////  DERIVED PHOSPHOR MASK CONSTANTS  //////////////////////
//  If we can use the large mipmapped LUT without mipmapping artifacts, we
//  should: It gives us more options for using fewer samples.
//  TODO: Take advantage of this!
//  tex2D's sampler2D parameter MUST be a uniform global, a uniform input to
//  main_fragment, or a static alias of one of the above.  This makes it hard
//  to select the phosphor mask at runtime: We can't even assign to a uniform
//  global in the vertex shader or select a sampler2D in the vertex shader and
//  pass it to the fragment shader (even with explicit TEXUNIT# bindings),
//  because it just gives us the input texture or a black screen.  However, we
//  can get around these limitations by calling tex2D three times with different
//  uniform samplers (or resizing the phosphor mask three times altogether).
//  With dynamic branches, we can process only one of these branches on top of
//  quickly discarding fragments we don't need (cgc seems able to overcome
//  limigations around dependent texture fetches inside of branches).  Without
//  dynamic branches, we have to process every branch for every fragment...which
//  is slower.  Runtime sampling mode selection is slower without dynamic
//  branches as well.  Let the user's static #defines decide if it's worth it.
//  We need to render some minimum number of tiles in the resize passes.
//  We need at least 1.0 just to repeat a single tile, and we need extra
//  padding beyond that for anisotropic filtering, discontinuitity fixing,
//  antialiasing, same-pass curvature (not currently used), etc.  First
//  determine how many border texels and tiles we need, based on how the result
//  will be sampled:
//  Most antialiasing filters have a base radius of 4.0 pixels:
//  Anisotropic filtering adds about 0.5 to the pixel border:
//  Fixing discontinuities adds 1.0 more to the pixel border:
//  Convert the pixel border to an integer texel border.  Assume same-pass
//  curvature about triples the texel frequency:
//  Convert the texel border to a tile border using worst-case assumptions:
//  Finally, set the number of resized tiles to render to MASK_RESIZE, and set
//  the starting texel (inside borders) for sampling it.
//  Special case: Render two tiles without borders.  Anisotropic
//  filtering doesn't seem to be a problem here.
//  We have to fit mask_resize_num_tiles into an FBO with a viewport scale of
//  mask_resize_viewport_scale.  This limits the maximum final triad size.
//  Estimate the minimum number of triads we can split the screen into in each
//  dimension (we'll be as correct as mask_resize_viewport_scale is):
////////////////////////  COMMON MATHEMATICAL CONSTANTS  ///////////////////////
//  We often want to find the location of the previous texel, e.g.:
//      const float2 curr_texel = uv * texture_size;
//      const float2 prev_texel = floor(curr_texel - float2(0.5)) + float2(0.5);
//      const float2 prev_texel_uv = prev_texel / texture_size;
//  However, many GPU drivers round incorrectly around exact texel locations.
//  We need to subtract a little less than 0.5 before flooring, and some GPU's
//  require this value to be farther from 0.5 than others; define it here.
//      const float2 prev_texel =
//          floor(curr_texel - float2(under_half)) + float2(0.5);
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file provides reusable one-pass and separable (two-pass) blurs.
//  Requires:   All blurs share these requirements (dxdy requirement is split):
//              1.) All requirements of gamma-management.h must be satisfied!
//              2.) filter_linearN must == "true" in your .cgp preset unless
//                  you're using tex2DblurNresize at 1x scale.
//              3.) mipmap_inputN must == "true" in your .cgp preset if
//                  IN.output_size < IN.video_size.
//              4.) IN.output_size == IN.video_size / pow(2, M), where M is some
//                  positive integer.  tex2Dblur*resize can resize arbitrarily
//                  (and the blur will be done after resizing), but arbitrary
//                  resizes "fail" with other blurs due to the way they mix
//                  static weights with bilinear sample exploitation.
//              5.) In general, dxdy should contain the uv pixel spacing:
//                      dxdy = (IN.video_size/IN.output_size)/IN.texture_size
//              6.) For separable blurs (tex2DblurNresize and tex2DblurNfast),
//                  zero out the dxdy component in the unblurred dimension:
//                      dxdy = float2(dxdy.x, 0.0) or float2(0.0, dxdy.y)
//              Many blurs share these requirements:
//              1.) One-pass blurs require scale_xN == scale_yN or scales > 1.0,
//                  or they will blur more in the lower-scaled dimension.
//              2.) One-pass shared sample blurs require ddx(), ddy(), and
//                  tex2Dlod() to be supported by the current Cg profile, and
//                  the drivers must support high-quality derivatives.
//              3.) One-pass shared sample blurs require:
//                      tex_uv.w == log2(IN.video_size/IN.output_size).y;
//              Non-wrapper blurs share this requirement:
//              1.) sigma is the intended standard deviation of the blur
//              Wrapper blurs share this requirement, which is automatically
//              met (unless OVERRIDE_BLUR_STD_DEVS is #defined; see below):
//              1.) blurN_std_dev must be global static const float values
//                  specifying standard deviations for Nx blurs in units
//                  of destination pixels
//  Optional:   1.) The including file (or an earlier included file) may
//                  optionally #define USE_BINOMIAL_BLUR_STD_DEVS to replace
//                  default standard deviations with those matching a binomial
//                  distribution.  (See below for details/properties.)
//              2.) The including file (or an earlier included file) may
//                  optionally #define OVERRIDE_BLUR_STD_DEVS and override:
//                      static const float blur3_std_dev
//                      static const float blur4_std_dev
//                      static const float blur5_std_dev
//                      static const float blur6_std_dev
//                      static const float blur7_std_dev
//                      static const float blur8_std_dev
//                      static const float blur9_std_dev
//                      static const float blur10_std_dev
//                      static const float blur11_std_dev
//                      static const float blur12_std_dev
//                      static const float blur17_std_dev
//                      static const float blur25_std_dev
//                      static const float blur31_std_dev
//                      static const float blur43_std_dev
//              3.) The including file (or an earlier included file) may
//                  optionally #define OVERRIDE_ERROR_BLURRING and override:
//                      static const float error_blurring
//                  This tuning value helps mitigate weighting errors from one-
//                  pass shared-sample blurs sharing bilinear samples between
//                  fragments.  Values closer to 0.0 have "correct" blurriness
//                  but allow more artifacts, and values closer to 1.0 blur away
//                  artifacts by sampling closer to halfway between texels.
//              UPDATE 6/21/14: The above static constants may now be overridden
//              by non-static uniform constants.  This permits exposing blur
//              standard deviations as runtime GUI shader parameters.  However,
//              using them keeps weights from being statically computed, and the
//              speed hit depends on the blur: On my machine, uniforms kill over
//              53% of the framerate with tex2Dblur12x12shared, but they only
//              drop the framerate by about 18% with tex2Dblur11fast.
//  Quality and Performance Comparisons:
//  For the purposes of the following discussion, "no sRGB" means
//  GAMMA_ENCODE_EVERY_FBO is #defined, and "sRGB" means it isn't.
//  1.) tex2DblurNfast is always faster than tex2DblurNresize.
//  2.) tex2DblurNresize functions are the only ones that can arbitrarily resize
//      well, because they're the only ones that don't exploit bilinear samples.
//      This also means they're the only functions which can be truly gamma-
//      correct without linear (or sRGB FBO) input, but only at 1x scale.
//  3.) One-pass shared sample blurs only have a speed advantage without sRGB.
//      They also have some inaccuracies due to their shared-[bilinear-]sample
//      design, which grow increasingly bothersome for smaller blurs and higher-
//      frequency source images (relative to their resolution).  I had high
//      hopes for them, but their most realistic use case is limited to quickly
//      reblurring an already blurred input at full resolution.  Otherwise:
//      a.) If you're blurring a low-resolution source, you want a better blur.
//      b.) If you're blurring a lower mipmap, you want a better blur.
//      c.) If you're blurring a high-resolution, high-frequency source, you
//          want a better blur.
//  4.) The one-pass blurs without shared samples grow slower for larger blurs,
//      but they're competitive with separable blurs at 5x5 and smaller, and
//      even tex2Dblur7x7 isn't bad if you're wanting to conserve passes.
//  Here are some framerates from a GeForce 8800GTS.  The first pass resizes to
//  viewport size (4x in this test) and linearizes for sRGB codepaths, and the
//  remaining passes perform 6 full blurs.  Mipmapped tests are performed at the
//  same scale, so they just measure the cost of mipmapping each FBO (only every
//  other FBO is mipmapped for separable blurs, to mimic realistic usage).
//  Mipmap      Neither     sRGB+Mipmap sRGB        Function
//  76.0        92.3        131.3       193.7       tex2Dblur3fast
//  63.2        74.4        122.4       175.5       tex2Dblur3resize
//  93.7        121.2       159.3       263.2       tex2Dblur3x3
//  59.7        68.7        115.4       162.1       tex2Dblur3x3resize
//  63.2        74.4        122.4       175.5       tex2Dblur5fast
//  49.3        54.8        100.0       132.7       tex2Dblur5resize
//  59.7        68.7        115.4       162.1       tex2Dblur5x5
//  64.9        77.2        99.1        137.2       tex2Dblur6x6shared
//  55.8        63.7        110.4       151.8       tex2Dblur7fast
//  39.8        43.9        83.9        105.8       tex2Dblur7resize
//  40.0        44.2        83.2        104.9       tex2Dblur7x7
//  56.4        65.5        71.9        87.9        tex2Dblur8x8shared
//  49.3        55.1        99.9        132.5       tex2Dblur9fast
//  33.3        36.2        72.4        88.0        tex2Dblur9resize
//  27.8        29.7        61.3        72.2        tex2Dblur9x9
//  37.2        41.1        52.6        60.2        tex2Dblur10x10shared
//  44.4        49.5        91.3        117.8       tex2Dblur11fast
//  28.8        30.8        63.6        75.4        tex2Dblur11resize
//  33.6        36.5        40.9        45.5        tex2Dblur12x12shared
//  TODO: Fill in benchmarks for new untested blurs.
//                                                  tex2Dblur17fast
//                                                  tex2Dblur25fast
//                                                  tex2Dblur31fast
//                                                  tex2Dblur43fast
//                                                  tex2Dblur3x3resize
/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////
//  Set static standard deviations, but allow users to override them with their
//  own constants (even non-static uniforms if they're okay with the speed hit):
//  blurN_std_dev values are specified in terms of dxdy strides.
//  By request, we can define standard deviations corresponding to a
//  binomial distribution with p = 0.5 (related to Pascal's triangle).
//  This distribution works such that blurring multiple times should
//  have the same result as a single larger blur.  These values are
//  larger than default for blurs up to 6x and smaller thereafter.
//  The defaults are the largest values that keep the largest unused
//  blur term on each side <= 1.0/256.0.  (We could get away with more
//  or be more conservative, but this compromise is pretty reasonable.)
//  error_blurring should be in [0.0, 1.0].  Higher values reduce ringing
//  in shared-sample blurs but increase blurring and feature shifting.
//////////////////////////////////  INCLUDES  //////////////////////////////////
//  gamma-management.h relies on pass-specific settings to guide its behavior:
//  FIRST_PASS, LAST_PASS, GAMMA_ENCODE_EVERY_FBO, etc.  See it for details.
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file provides gamma-aware tex*D*() and encode_output() functions.
//  Requires:   Before #include-ing this file, the including file must #define
//              the following macros when applicable and follow their rules:
//              1.) #define FIRST_PASS if this is the first pass.
//              2.) #define LAST_PASS if this is the last pass.
//              3.) If sRGB is available, set srgb_framebufferN = "true" for
//                  every pass except the last in your .cgp preset.
//              4.) If sRGB isn't available but you want gamma-correctness with
//                  no banding, #define GAMMA_ENCODE_EVERY_FBO each pass.
//              5.) #define SIMULATE_CRT_ON_LCD if desired (precedence over 5-7)
//              6.) #define SIMULATE_GBA_ON_LCD if desired (precedence over 6-7)
//              7.) #define SIMULATE_LCD_ON_CRT if desired (precedence over 7)
//              8.) #define SIMULATE_GBA_ON_CRT if desired (precedence over -)
//              If an option in [5, 8] is #defined in the first or last pass, it
//              should be #defined for both.  It shouldn't make a difference
//              whether it's #defined for intermediate passes or not.
//  Optional:   The including file (or an earlier included file) may optionally
//              #define a number of macros indicating it will override certain
//              macros and associated constants are as follows:
//              static constants with either static or uniform constants.  The
//              1.) OVERRIDE_STANDARD_GAMMA: The user must first define:
//                  static const float ntsc_gamma
//                  static const float pal_gamma
//                  static const float crt_reference_gamma_high
//                  static const float crt_reference_gamma_low
//                  static const float lcd_reference_gamma
//                  static const float crt_office_gamma
//                  static const float lcd_office_gamma
//              2.) OVERRIDE_DEVICE_GAMMA: The user must first define:
//                  static const float crt_gamma
//                  static const float gba_gamma
//                  static const float lcd_gamma
//              3.) OVERRIDE_FINAL_GAMMA: The user must first define:
//                  static const float input_gamma
//                  static const float intermediate_gamma
//                  static const float output_gamma
//                  (intermediate_gamma is for GAMMA_ENCODE_EVERY_FBO.)
//              4.) OVERRIDE_ALPHA_ASSUMPTIONS: The user must first define:
//                  static const bool assume_opaque_alpha
//              The gamma constant overrides must be used in every pass or none,
//              and OVERRIDE_FINAL_GAMMA bypasses all of the SIMULATE* macros.
//              OVERRIDE_ALPHA_ASSUMPTIONS may be set on a per-pass basis.
//  Usage:      After setting macros appropriately, ignore gamma correction and
//              replace all tex*D*() calls with equivalent gamma-aware
//              tex*D*_linearize calls, except:
//              1.) When you read an LUT, use regular tex*D or a gamma-specified
//                  function, depending on its gamma encoding:
//                      tex*D*_linearize_gamma (takes a runtime gamma parameter)
//              2.) If you must read pass0's original input in a later pass, use
//                  tex2D_linearize_ntsc_gamma.  If you want to read pass0's
//                  input with gamma-corrected bilinear filtering, consider
//                  creating a first linearizing pass and reading from the input
//                  of pass1 later.
//              Then, return encode_output(color) from every fragment shader.
//              Finally, use the global gamma_aware_bilinear boolean if you want
//              to statically branch based on whether bilinear filtering is
//              gamma-correct or not (e.g. for placing Gaussian blur samples).
//
//  Detailed Policy:
//  tex*D*_linearize() functions enforce a consistent gamma-management policy
//  based on the FIRST_PASS and GAMMA_ENCODE_EVERY_FBO settings.  They assume
//  their input texture has the same encoding characteristics as the input for
//  the current pass (which doesn't apply to the exceptions listed above).
//  Similarly, encode_output() enforces a policy based on the LAST_PASS and
//  GAMMA_ENCODE_EVERY_FBO settings.  Together, they result in one of the
//  following two pipelines.
//  Typical pipeline with intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = linear_color;     //  Automatic sRGB encoding
//      linear_color = intermediate_output;     //  Automatic sRGB decoding
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Typical pipeline without intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = pow(linear_color, 1.0/intermediate_gamma);
//      linear_color = pow(intermediate_output, intermediate_gamma);
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Using GAMMA_ENCODE_EVERY_FBO is much slower, but it's provided as a way to
//  easily get gamma-correctness without banding on devices where sRGB isn't
//  supported.
//
//  Use This Header to Maximize Code Reuse:
//  The purpose of this header is to provide a consistent interface for texture
//  reads and output gamma-encoding that localizes and abstracts away all the
//  annoying details.  This greatly reduces the amount of code in each shader
//  pass that depends on the pass number in the .cgp preset or whether sRGB
//  FBO's are being used: You can trivially change the gamma behavior of your
//  whole pass by commenting or uncommenting 1-3 #defines.  To reuse the same
//  code in your first, Nth, and last passes, you can even put it all in another
//  header file and #include it from skeleton .cg files that #define the
//  appropriate pass-specific settings.
//
//  Rationale for Using Three Macros:
//  This file uses GAMMA_ENCODE_EVERY_FBO instead of an opposite macro like
//  SRGB_PIPELINE to ensure sRGB is assumed by default, which hopefully imposes
//  a lower maintenance burden on each pass.  At first glance it seems we could
//  accomplish everything with two macros: GAMMA_CORRECT_IN / GAMMA_CORRECT_OUT.
//  This works for simple use cases where input_gamma == output_gamma, but it
//  breaks down for more complex scenarios like CRT simulation, where the pass
//  number determines the gamma encoding of the input and output.
///////////////////////////////  BASE CONSTANTS  ///////////////////////////////
//  Set standard gamma constants, but allow users to override them:
//  Standard encoding gammas:
//  Typical device decoding gammas (only use for emulating devices):
//  CRT/LCD reference gammas are higher than NTSC and Rec.709 video standard
//  gammas: The standards purposely undercorrected for an analog CRT's
//  assumed 2.5 reference display gamma to maintain contrast in assumed
//  [dark] viewing conditions: http://www.poynton.com/PDFs/GammaFAQ.pdf
//  These unstated assumptions about display gamma and perceptual rendering
//  intent caused a lot of confusion, and more modern CRT's seemed to target
//  NTSC 2.2 gamma with circuitry.  LCD displays seem to have followed suit
//  (they struggle near black with 2.5 gamma anyway), especially PC/laptop
//  displays designed to view sRGB in bright environments.  (Standards are
//  also in flux again with BT.1886, but it's underspecified for displays.)
//  Assuming alpha == 1.0 might make it easier for users to avoid some bugs,
//  but only if they're aware of it.
///////////////////////  DERIVED CONSTANTS AS FUNCTIONS  ///////////////////////
//  gamma-management.h should be compatible with overriding gamma values with
//  runtime user parameters, but we can only define other global constants in
//  terms of static constants, not uniform user parameters.  To get around this
//  limitation, we need to define derived constants using functions.
//  Set device gamma constants, but allow users to override them:
//  The user promises to globally define the appropriate constants:
//  Set decoding/encoding gammas for the first/lass passes, but allow overrides:
//  The user promises to globally define the appropriate constants:
//  If we gamma-correct every pass, always use ntsc_gamma between passes to
//  ensure middle passes don't need to care if anything is being simulated:
//  Set decoding/encoding gammas for the current pass.  Use static constants for
//  linearize_input and gamma_encode_output, because they aren't derived, and
//  they let the compiler do dead-code elimination.
//  Users might want to know if bilinear filtering will be gamma-correct:
//////////////////////  COLOR ENCODING/DECODING FUNCTIONS  /////////////////////
//TODO/FIXME: I have no idea why replacing the lookup wrappers with this macro fixes the blurs being offset ¯\_(ツ)_/¯
//#define tex2D_linearize(C, D) decode_input(vec4(texture(C, D)))
// EDIT: it's the 'const' in front of the coords that's doing it
///////////////////////////  TEXTURE LOOKUP WRAPPERS  //////////////////////////
//  "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a wide array of linearizing texture lookup wrapper functions.  The
//  Cg shader spec Retroarch uses only allows for 2D textures, but 1D and 3D
//  lookups are provided for completeness in case that changes someday.  Nobody
//  is likely to use the *fetch and *proj functions, but they're included just
//  in case.  The only tex*D texture sampling functions omitted are:
//      - tex*Dcmpbias
//      - tex*Dcmplod
//      - tex*DARRAY*
//      - tex*DMS*
//      - Variants returning integers
//  Standard line length restrictions are ignored below for vertical brevity.

//  tex1D:
inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

//  tex1Dbias:
inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dbias(tex, tex_coords));   }

inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dbias(tex, tex_coords, texel_off));    }

//  tex1Dfetch:
inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords)
{   return decode_input(tex1Dfetch(tex, tex_coords));  }

inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex1Dfetch(tex, tex_coords, texel_off));   }

//  tex1Dlod:
inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dlod(tex, tex_coords));    }

inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dlod(tex, tex_coords, texel_off));     }

//  tex1Dproj:
inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

//  tex2D:
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//  tex2Dbias:
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords)
//{   return decode_input(tex2Dbias(tex, tex_coords));   }
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dbias(tex, tex_coords, texel_off));    }
//  tex2Dfetch:
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords)
//{   return decode_input(tex2Dfetch(tex, tex_coords));  }
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dfetch(tex, tex_coords, texel_off));   }
//  tex2Dlod:

//  tex2Dproj:
inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }


//  tex3D:
inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords)
{   return decode_input(tex3D(tex, tex_coords));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, texel_off));    }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy)
{   return decode_input(tex3D(tex, tex_coords, dx, dy));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, dx, dy, texel_off));    }

//  tex3Dbias:
inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dbias(tex, tex_coords));   }

inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dbias(tex, tex_coords, texel_off));    }

//  tex3Dfetch:
inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords)
{   return decode_input(tex3Dfetch(tex, tex_coords));  }

inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex3Dfetch(tex, tex_coords, texel_off));   }

//  tex3Dlod:
inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dlod(tex, tex_coords));    }

inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dlod(tex, tex_coords, texel_off));     }

//  tex3Dproj:
inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dproj(tex, tex_coords));   }

inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dproj(tex, tex_coords, texel_off));    }
/////////*

//  NONSTANDARD "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  This narrow selection of nonstandard tex2D* functions can be useful:

//  tex2Dlod0: Automatically fill in the tex2D LOD parameter for mip level 0.
//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0)));   }

//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords, const int texel_off)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0), texel_off));    }


//  MANUALLY LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a narrower selection of tex2D* wrapper functions that decode an
//  input sample with a specified gamma value.  These are useful for reading
//  LUT's and for reading the input of pass0 in a later pass.

//  tex2D:
inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords), gamma);   }

inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords.xy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//  tex2Dbias:
inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords), gamma);   }

inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords, texel_off), gamma);    }

//  tex2Dfetch:
inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords), gamma);  }

inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords, texel_off), gamma);   }

//  tex2Dlod:
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey*
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DISCLAIMER  /////////////////////////////////
//  *This code was inspired by "Shader Amortization using Pixel Quad Message
//  Passing" by Eric Penner, published in GPU Pro 2, Chapter VI.2.  My intent
//  is not to plagiarize his fundamentally similar code and assert my own
//  copyright, but the algorithmic helper functions require so little code that
//  implementations can't vary by much except bugfixes and conventions.  I just
//  wanted to license my own particular code here to avoid ambiguity and make it
//  clear that as far as I'm concerned, people can do as they please with it.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  Given screen pixel numbers, derive a "quad vector" describing a fragment's
//  position in its 2x2 pixel quad.  Given that vector, obtain the values of any
//  variable at neighboring fragments.
//  Requires:   Using this file in general requires:
//              1.) ddx() and ddy() are present in the current Cg profile.
//              2.) The GPU driver is using fine/high-quality derivatives.
//                  Functions will give incorrect results if this is not true,
//                  so a test function is included.
/////////////////////  QUAD-PIXEL COMMUNICATION PRIMITIVES  ////////////////////
//  Requires:   Two measures of the current fragment's output pixel number
//              in the range ([0, IN.output_size.x), [0, IN.output_size.y)):
//              1.) output_pixel_num_wrt_uvxy.xy increase with uv coords.
//              2.) output_pixel_num_wrt_uvxy.zw increase with screen xy.
//  Returns:    Two measures of the fragment's position in its 2x2 quad:
//              1.) The .xy components are its 2x2 placement with respect to
//                  uv direction (the origin (0, 0) is at the top-left):
//                  top-left     = (-1.0, -1.0) top-right    = ( 1.0, -1.0)
//                  bottom-left  = (-1.0,  1.0) bottom-right = ( 1.0,  1.0)
//                  You need this to arrange/weight shared texture samples.
//              2.) The .zw components are its 2x2 placement with respect to
//                  screen xy direction (IN.position); the origin varies.
//                  quad_gather needs this measure to work correctly.
//              Note: quad_vector.zw = quad_vector.xy * float2(
//                      ddx(output_pixel_num_wrt_uvxy.x),
//                      ddy(output_pixel_num_wrt_uvxy.y));
//  Caveats:    This function assumes the GPU driver always starts 2x2 pixel
//              quads at even pixel numbers.  This assumption can be wrong
//              for odd output resolutions (nondeterministically so).
//  Requires:   Same as get_quad_vector_naive() (see that first).
//  Returns:    Same as get_quad_vector_naive() (see that first), but it's
//              correct even if the 2x2 pixel quad starts at an odd pixel,
//              which can occur at odd resolutions.
//  If quad_vector_guess.zw doesn't increase with screen xy, we know
//  the 2x2 pixel quad starts at an odd pixel:
//  Requires:   1.) ddx() and ddy() are present in the current Cg profile.
//              2.) output_pixel_num_wrt_uv must increase with uv coords and
//                  measure the current fragment's output pixel number in:
//                      ([0, IN.output_size.x), [0, IN.output_size.y))
//  Returns:    Same as get_quad_vector_naive() (see that first), but it's
//              correct even if the 2x2 pixel quad starts at an odd pixel,
//              which can occur at odd resolutions.
//  Caveats:    This function requires less information than the version
//              taking a float4, but it's potentially slower.
//  Do screen coords increase with or against uv?  Get the direction
//  with respect to (uv.x, uv.y) for (screen.x, screen.y) in {-1, 1}.
//  If quad_vector_screen_guess doesn't increase with screen xy, we know
//  the 2x2 pixel quad starts at an odd pixel:
//  Requires:   1.) ddx() and ddy() are present in the current Cg profile.
//              2.) The GPU driver is using fine/high-quality derivatives.
//              3.) quad_vector describes the current fragment's location in
//                  its 2x2 pixel quad using get_quad_vector()'s conventions.
//              4.) curr is any vector you wish to get neighboring values of.
//  Returns:    Values of an input vector (curr) at neighboring fragments
//              adjacent x, adjacent y, and diagonal (via out parameters).
//  Float3 version
//  Float2 version
//  Float version:
//  Returns:    return.x == current
//              return.y == adjacent x
//              return.z == adjacent y
//              return.w == diagonal
//  Requires:   Same as quad_gather()
//  Returns:    Sum of an input vector (curr) at all fragments in a quad.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   1.) ddx() and ddy() are present in the current Cg profile.
//              2.) quad_vector describes the current fragment's location in
//                  its 2x2 pixel quad using get_quad_vector()'s conventions.
//              3.) curr must be a test vector with non-constant derivatives
//                  (its value should change nonlinearly across fragments).
//  Returns:    true if fine/hybrid/high-quality derivatives are used, or
//              false if coarse derivatives are used or inconclusive
//  Usage:      Test whether quad-pixel communication is working!
//  Method:     We can confirm fine derivatives are used if the following
//              holds (ever, for any value at any fragment):
//                  (ddy(curr) != ddy(adjx)) or (ddx(curr) != ddx(adjy))
//              The more values we test (e.g. test a float4 two ways), the
//              easier it is to demonstrate fine derivatives are working.
//  TODO: Check for floating point exact comparison issues!
//  Requires:   Same as fine_derivatives_working()
//  Returns:    Same as fine_derivatives_working()
//  Usage:      This is faster than fine_derivatives_working() but more
//              likely to return false negatives, so it's less useful for
//              offline testing/debugging.  It's also useless as the basis
//              for dynamic runtime branching as of May 2014: Derivatives
//              (and quad-pixel communication) are currently disallowed in
//              branches.  However, future GPU's may allow you to use them
//              in dynamic branches if you promise the branch condition
//              evaluates the same for every fragment in the quad (and/or if
//              the driver enforces that promise by making a single fragment
//              control branch decisions).  If that ever happens, this
//              version may become a more economical choice.
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file implements the following mathematical special functions:
//  1.) erf() = 2/sqrt(pi) * indefinite_integral(e**(-x**2))
//  2.) gamma(s), a real-numbered extension of the integer factorial function
//  It also implements normalized_ligamma(s, z), a normalized lower incomplete
//  gamma function for s < 0.5 only.  Both gamma() and normalized_ligamma() can
//  be called with an _impl suffix to use an implementation version with a few
//  extra precomputed parameters (which may be useful for the caller to reuse).
//  See below for details.
//
//  Design Rationale:
//  Pretty much every line of code in this file is duplicated four times for
//  different input types (float4/float3/float2/float).  This is unfortunate,
//  but Cg doesn't allow function templates.  Macros would be far less verbose,
//  but they would make the code harder to document and read.  I don't expect
//  these functions will require a whole lot of maintenance changes unless
//  someone ever has need for more robust incomplete gamma functions, so code
//  duplication seems to be the lesser evil in this case.
///////////////////////////  GAUSSIAN ERROR FUNCTION  //////////////////////////
//  Requires:   x is the standard parameter to erf().
//  Returns:    Return an Abramowitz/Stegun approximation of erf(), where:
//                  erf(x) = 2/sqrt(pi) * integral(e**(-x**2))
//              This approximation has a max absolute error of 2.5*10**-5
//              with solid numerical robustness and efficiency.  See:
//                  https://en.wikipedia.org/wiki/Error_function#Approximation_with_elementary_functions
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   x is the standard parameter to erf().
//  Returns:    Approximate erf() with the hyperbolic tangent.  The error is
//              visually noticeable, but it's blazing fast and perceptually
//              close...at least on ATI hardware.  See:
//                  http://www.maplesoft.com/applications/view.aspx?SID=5525&view=html
//  Warning:    Only use this if your hardware drivers correctly implement
//              tanh(): My nVidia 8800GTS returns garbage output.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   x is the standard parameter to erf().
//  Returns:    Some approximation of erf(x), depending on user settings.
//  Float3 version:
//  Float2 version:
//  Float version:
///////////////////////////  COMPLETE GAMMA FUNCTION  //////////////////////////
//  Requires:   1.) s is the standard parameter to the gamma function, and
//                  it should lie in the [0, 36] range.
//              2.) s_inv = 1.0/s.  This implementation function requires
//                  the caller to precompute this value, giving users the
//                  opportunity to reuse it.
//  Returns:    Return approximate gamma function (real-numbered factorial)
//              output using the Lanczos approximation with two coefficients
//              calculated using Paul Godfrey's method here:
//                  http://my.fit.edu/~gabdo/gamma.txt
//              An optimal g value for s in [0, 36] is ~1.12906830989, with
//              a maximum relative error of 0.000463 for 2**16 equally
//              evals.  We could use three coeffs (0.0000346 error) without
//              hurting latency, but this allows more parallelism with
//              outside instructions.
//  gamma(s + 1) = base**sph * lanczos_sum; divide by s for gamma(s).
//  This has less error for small s's than (s -= 1.0) at the beginning.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   s is the standard parameter to the gamma function, and it
//              should lie in the [0, 36] range.
//  Returns:    Return approximate gamma function output with a maximum
//              relative error of 0.000463.  See gamma_impl for details.
//  Float3 version:
//  Float2 version:
//  Float version:
////////////////  INCOMPLETE GAMMA FUNCTIONS (RESTRICTED INPUT)  ///////////////
//  Lower incomplete gamma function for small s and z (implementation):
//  Requires:   1.) s < ~0.5
//              2.) z <= ~0.775075
//              3.) s_inv = 1.0/s (precomputed for outside reuse)
//  Returns:    A series representation for the lower incomplete gamma
//              function for small s and small z (4 terms).
//  The actual "rolled up" summation looks like:
//      last_sign = 1.0; last_pow = 1.0; last_factorial = 1.0;
//      sum = last_sign * last_pow / ((s + k) * last_factorial)
//      for(int i = 0; i < 4; ++i)
//      {
//          last_sign *= -1.0; last_pow *= z; last_factorial *= i;
//          sum += last_sign * last_pow / ((s + k) * last_factorial);
//      }
//  Unrolled, constant-unfolded and arranged for madds and parallelism:
//  Summation iterations 1, 2, and 3:
//float4 denom4 = 24.0*s + float4(96.0);
//sum += z_sq * z_sq / denom4;
//  Scale and return:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Upper incomplete gamma function for small s and large z (implementation):
//  Requires:   1.) s < ~0.5
//              2.) z > ~0.775075
//  Returns:    Gauss's continued fraction representation for the upper
//              incomplete gamma function (4 terms).
//  The "rolled up" continued fraction looks like this.  The denominator
//  is truncated, and it's calculated "from the bottom up:
//      denom = float4('inf');
//      float4 one = float4(1.0);
//      for(int i = 4; i > 0; --i)
//      {
//          denom = ((i * 2.0) - one) + z - s + (i * (s - i))/denom;
//      }
//  Unrolled and constant-unfolded for madds and parallelism:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Normalized lower incomplete gamma function for small s (implementation):
//  Requires:   1.) s < ~0.5
//              2.) s_inv = 1/s (precomputed for outside reuse)
//              3.) gamma_s_inv = 1/gamma(s) (precomputed for outside reuse)
//  Returns:    Approximate the normalized lower incomplete gamma function
//              for s < 0.5.  Since we only care about s < 0.5, we only need
//              to evaluate two branches (not four) based on z.  Each branch
//              uses four terms, with a max relative error of ~0.00182.  The
//              branch threshold and specifics were adapted for fewer terms
//              from Gil/Segura/Temme's paper here:
//                  http://oai.cwi.nl/oai/asset/20433/20433B.pdf
//  Evaluate both branches: Real branches test slower even when available.
//  Combine the results from both branches:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Normalized lower incomplete gamma function for small s:
//  Requires:   s < ~0.5
//  Returns:    Approximate the normalized lower incomplete gamma function
//              for s < 0.5.  See normalized_ligamma_impl() for details.
//  Float3 version:
//  Float2 version:
//  Float version:
///////////////////////////////////  HELPERS  //////////////////////////////////
//  Make a float2 uv offset safe for adding to float4 tex2Dlod coords:
//  Make a length squared helper macro (for usage with static constants):
//  We can use the Gaussian integral to calculate the asymptotic weight for
//  the center pixel.  Since the unnormalized center pixel weight is 1.0,
//  the normalized weight is the same as the weight sum inverse.  Given a
//  large enough blur (9+), the asymptotic weight sum is close and faster:
//      center_weight = 0.5 *
//          (erf(0.5/(sigma*sqrt(2.0))) - erf(-0.5/(sigma*sqrt(2.0))))
//      erf(-x) == -erf(x), so we get 0.5 * (2.0 * erf(blah blah)):
//  However, we can get even faster results with curve-fitting.  These are
//  also closer than the asymptotic results, because they were constructed
//  from 64 blurs sizes from [3, 131) and 255 equally-spaced sigmas from
//  (0, blurN_std_dev), so the results for smaller sigmas are biased toward
//  smaller blurs.  The max error is 0.0031793913.
//  Relative FPS: 134.3 with erf, 135.8 with curve-fitting.
//static const float temp = 0.5/sqrt(2.0);
//return erf(temp/sigma);
////////////////////  ARBITRARILY RESIZABLE SEPARABLE BLURS  ///////////////////
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 11x Gaussian blurred texture lookup using a 11-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  Calculate Gaussian blur kernel weights and a normalization factor for
//  distances of 0-4, ignoring constant factors (since we're normalizing).
//  Statically normalize weights, sum weighted samples, and return.  Blurs are
//  currently optimized for dynamic weights.
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 9x Gaussian blurred texture lookup using a 9-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 7x Gaussian blurred texture lookup using a 7-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 5x Gaussian blurred texture lookup using a 5-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 3x Gaussian blurred texture lookup using a 3-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
///////////////////////////  FAST SEPARABLE BLURS  ///////////////////////////
//  Requires:   1.) Global requirements must be met (see file description).
//              2.) filter_linearN must = "true" in your .cgp file.
//              3.) For gamma-correct bilinear filtering, global
//                  gamma_aware_bilinear == true (from gamma-management.h)
//  Returns:    A 1D 11x Gaussian blurred texture lookup using 6 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 9x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 4 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 7x Gaussian blurred texture lookup using 4 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 5x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 2 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 3x Gaussian blurred texture lookup using 2 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Weights for all samples are the same, so just average them:
////////////////////////////  HUGE SEPARABLE BLURS  ////////////////////////////
//  Huge separable blurs come only in "fast" versions.
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 43x Gaussian blurred texture lookup using 22 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 /
//    (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + w10 + w11 +
//        w12 + w13 + w14 + w15 + w16 + w17 + w18 + w19 + w20 + w21));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 31x Gaussian blurred texture lookup using 16 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 /
//    (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 +
//        w9 + w10 + w11 + w12 + w13 + w14 + w15));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 25x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 12 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 / (w0 + 2.0 * (
//    w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + w10 + w11 + w12));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 17x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 8 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 / (w0 + 2.0 * (
//    w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
////////////////////  ARBITRARILY RESIZABLE ONE-PASS BLURS  ////////////////////
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 3x3 Gaussian blurred mipmapped texture lookup of the
//              resized input.
//  Description:
//  This is the only arbitrarily resizable one-pass blur; tex2Dblur5x5resize
//  would perform like tex2Dblur9x9, MUCH slower than tex2Dblur5resize.
//  Load each sample.  We need all 3x3 samples.  Quad-pixel communication
//  won't help either: This should perform like tex2Dblur5x5, but sharing a
//  4x4 sample field would perform more like tex2Dblur8x8shared (worse).
//  Statically compute Gaussian sample weights:
//  Weight and sum the samples:
////////////////////////////  FASTER ONE-PASS BLURS  ///////////////////////////
//  Perform a 1-pass 9x9 blur with 5x5 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 9x9 Gaussian blurred mipmapped texture lookup composed of
//              5x5 carefully selected bilinear samples.
//  Description:
//  Perform a 1-pass 9x9 blur with 5x5 bilinear samples.  Adjust the
//  bilinear sample location to reflect the true Gaussian weights for each
//  underlying texel.  The following diagram illustrates the relative
//  locations of bilinear samples.  Each sample with the same number has the
//  same weight (notice the symmetry).  The letters a, b, c, d distinguish
//  quadrants, and the letters U, D, L, R, C (up, down, left, right, center)
//  distinguish 1D directions along the line containing the pixel center:
//      6a 5a 2U 5b 6b
//      4a 3a 1U 3b 4b
//      2L 1L 0C 1R 2R
//      4c 3c 1D 3d 4d
//      6c 5c 2D 5d 6d
//  The following diagram illustrates the underlying equally spaced texels,
//  named after the sample that accesses them and subnamed by their location
//  within their 2x2, 2x1, 1x2, or 1x1 texel block:
//      6a4 6a3 5a4 5a3 2U2 5b3 5b4 6b3 6b4
//      6a2 6a1 5a2 5a1 2U1 5b1 5b2 6b1 6b2
//      4a4 4a3 3a4 3a3 1U2 3b3 3b4 4b3 4b4
//      4a2 4a1 3a2 3a1 1U1 3b1 3b2 4b1 4b2
//      2L2 2L1 1L2 1L1 0C1 1R1 1R2 2R1 2R2
//      4c2 4c1 3c2 3c1 1D1 3d1 3d2 4d1 4d2
//      4c4 4c3 3c4 3c3 1D2 3d3 3d4 4d3 4d4
//      6c2 6c1 5c2 5c1 2D1 5d1 5d2 6d1 6d2
//      6c4 6c3 5c4 5c3 2D2 5d3 5d4 6d3 6d4
//  Note there is only one C texel and only two texels for each U, D, L, or
//  R sample.  The center sample is effectively a nearest neighbor sample,
//  and the U/D/L/R samples use 1D linear filtering.  All other texels are
//  read with bilinear samples somewhere within their 2x2 texel blocks.
//  COMPUTE TEXTURE COORDS:
//  Statically compute sampling offsets within each 2x2 texel block, based
//  on 1D sampling ratios between texels [1, 2] and [3, 4] texels away from
//  the center, and reuse them independently for both dimensions.  Compute
//  these offsets based on the relative 1D Gaussian weights of the texels
//  in question.  (w1off means "Gaussian weight for the texel 1.0 texels
//  away from the pixel center," etc.).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including x-axis-aligned:
//  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
//  Statically compute Gaussian texel weights for the bottom-right quadrant.
//  Read underscores as "and.
//  Statically add texel weights in each sample to get sample weights:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES:
//  Load all 25 samples (1 nearest, 8 linear, 16 bilinear) using symmetry:
//  Sampling order doesn't seem to affect performance, so just be clear:
//  SUM WEIGHTED SAMPLES:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Perform a 1-pass 7x7 blur with 5x5 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 7x7 Gaussian blurred mipmapped texture lookup composed of
//              4x4 carefully selected bilinear samples.
//  Description:
//  First see the descriptions for tex2Dblur9x9() and tex2Dblur7().  This
//  blur mixes concepts from both.  The sample layout is as follows:
//      4a 3a 3b 4b
//      2a 1a 1b 2b
//      2c 1c 1d 2d
//      4c 3c 3d 4d
//  The texel layout is as follows.  Note that samples 3a/3b, 1a/1b, 1c/1d,
//  and 3c/3d share a vertical column of texels, and samples 2a/2c, 1a/1c,
//  1b/1d, and 2b/2d share a horizontal row of texels (all sample1's share
//  the center texel):
//      4a4  4a3  3a4  3ab3 3b4  4b3  4b4
//      4a2  4a1  3a2  3ab1 3b2  4b1  4b2
//      2a4  2a3  1a4  1ab3 1b4  2b3  2b4
//      2ac2 2ac1 1ac2 1*   1bd2 2bd1 2bd2
//      2c4  2c3  1c4  1cd3 1d4  2d3  2d4
//      4c2  4c1  3c2  3cd1 3d2  4d1  4d2
//      4c4  4c3  3c4  3cd3 3d4  4d3  4d4
//  COMPUTE TEXTURE COORDS:
//  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including axis-aligned:
//  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
//  Statically compute Gaussian texel weights for the bottom-right quadrant.
//  Read underscores as "and.
//  Statically add texel weights in each sample to get sample weights.
//  Split weights for shared texels between samples sharing them:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES:
//  Load all 16 samples using symmetry:
//  SUM WEIGHTED SAMPLES:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Perform a 1-pass 5x5 blur with 3x3 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 5x5 Gaussian blurred mipmapped texture lookup composed of
//              3x3 carefully selected bilinear samples.
//  Description:
//  First see the description for tex2Dblur9x9().  This blur uses the same
//  concept and sample/texel locations except on a smaller scale.  Samples:
//      2a 1U 2b
//      1L 0C 1R
//      2c 1D 2d
//  Texels:
//      2a4 2a3 1U2 2b3 2b4
//      2a2 2a1 1U1 2b1 2b2
//      1L2 1L1 0C1 1R1 1R2
//      2c2 2c1 1D1 2d1 2d2
//      2c4 2c3 1D2 2d3 2d4
//  COMPUTE TEXTURE COORDS:
//  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including x-axis-aligned:
//  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
//  Statically compute Gaussian texel weights for the bottom-right quadrant.
//  Read underscores as "and.
//  Statically add texel weights in each sample to get sample weights:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES:
//  Load all 9 samples (1 nearest, 4 linear, 4 bilinear) using symmetry:
//  SUM WEIGHTED SAMPLES:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Perform a 1-pass 3x3 blur with 5x5 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 3x3 Gaussian blurred mipmapped texture lookup composed of
//              2x2 carefully selected bilinear samples.
//  Description:
//  First see the descriptions for tex2Dblur9x9() and tex2Dblur7().  This
//  blur mixes concepts from both.  The sample layout is as follows:
//      0a 0b
//      0c 0d
//  The texel layout is as follows.  Note that samples 0a/0b and 0c/0d share
//  a vertical column of texels, and samples 0a/0c and 0b/0d share a
//  horizontal row of texels (all samples share the center texel):
//      0a3  0ab2 0b3
//      0ac1 0*0  0bd1
//      0c3  0cd2 0d3
//  COMPUTE TEXTURE COORDS:
//  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including axis-aligned:
//  LOAD TEXTURE SAMPLES:
//  Load all 4 samples using symmetry:
//  SUM WEIGHTED SAMPLES:
//  Weights for all samples are the same, so just average them:
//////////////////  LINEAR ONE-PASS BLURS WITH SHARED SAMPLES  /////////////////
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   1.) Same as tex2Dblur9()
//              2.) ddx() and ddy() are present in the current Cg profile.
//              3.) The GPU driver is using fine/high-quality derivatives.
//              4.) quad_vector *correctly* describes the current fragment's
//                  location in its pixel quad, by the conventions noted in
//                  get_quad_vector[_naive].
//              5.) tex_uv.w = log2(IN.video_size/IN.output_size).y
//              6.) tex2Dlod() is present in the current Cg profile.
//  Optional:   Tune artifacts vs. excessive blurriness with the global
//              float error_blurring.
//  Returns:    A blurred texture lookup using a "virtual" 12x12 Gaussian
//              blur (a 6x6 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  Perform a 1-pass blur with shared texture lookups across a pixel quad.
//  We'll get neighboring samples with high-quality ddx/ddy derivatives, as
//  in GPU Pro 2, Chapter VI.2, "Shader Amortization using Pixel Quad
//  Message Passing" by Eric Penner.
//
//  Our "virtual" 12x12 blur will be comprised of ((6 - 1)^2)/4 + 3 = 12
//  bilinear samples, where bilinear sampling positions are computed from
//  the relative Gaussian weights of the 4 surrounding texels.  The catch is
//  that the appropriate texel weights and sample coords differ for each
//  fragment, but we're reusing most of the same samples across a quad of
//  destination fragments.  (We do use unique coords for the four nearest
//  samples at each fragment.)  Mixing bilinear filtering and sample-sharing
//  therefore introduces some error into the weights, and this can get nasty
//  when the source image is small or high-frequency.  Computing bilinear
//  ratios based on weights at the sample field center results in sharpening
//  and ringing artifacts, but we can move samples closer to halfway between
//  texels to try blurring away the error (which can move features around by
//  a texel or so).  Tune this with the global float "error_blurring".
//
//  The pixel quad's sample field covers 12x12 texels, accessed through 6x6
//  bilinear (2x2 texel) taps.  Each fragment depends on a window of 10x10
//  texels (5x5 bilinear taps), and each fragment is responsible for loading
//  a 6x6 texel quadrant as a 3x3 block of bilinear taps, plus 3 more taps
//  to use unique bilinear coords for sample0* for each fragment.  This
//  diagram illustrates the relative locations of bilinear samples 1-9 for
//  each quadrant a, b, c, d (note samples will not be equally spaced):
//      8a 7a 6a 6b 7b 8b
//      5a 4a 3a 3b 4b 5b
//      2a 1a 0a 0b 1b 2b
//      2c 1c 0c 0d 1d 2d
//      5c 4c 3c 3d 4d 5d
//      8c 7c 6c 6d 7d 8d
//  The following diagram illustrates the underlying equally spaced texels,
//  named after the sample that accesses them and subnamed by their location
//  within their 2x2 texel block:
//      8a3 8a2 7a3 7a2 6a3 6a2 6b2 6b3 7b2 7b3 8b2 8b3
//      8a1 8a0 7a1 7a0 6a1 6a0 6b0 6b1 7b0 7b1 8b0 8b1
//      5a3 5a2 4a3 4a2 3a3 3a2 3b2 3b3 4b2 4b3 5b2 5b3
//      5a1 5a0 4a1 4a0 3a1 3a0 3b0 3b1 4b0 4b1 5b0 5b1
//      2a3 2a2 1a3 1a2 0a3 0a2 0b2 0b3 1b2 1b3 2b2 2b3
//      2a1 2a0 1a1 1a0 0a1 0a0 0b0 0b1 1b0 1b1 2b0 2b1
//      2c1 2c0 1c1 1c0 0c1 0c0 0d0 0d1 1d0 1d1 2d0 2d1
//      2c3 2c2 1c3 1c2 0c3 0c2 0d2 0d3 1d2 1d3 2d2 2d3
//      5c1 5c0 4c1 4c0 3c1 3c0 3d0 3d1 4d0 4d1 5d0 5d1
//      5c3 5c2 4c3 4c2 3c3 3c2 3d2 3d3 4d2 4d3 5d2 5d3
//      8c1 8c0 7c1 7c0 6c1 6c0 6d0 6d1 7d0 7d1 8d0 8d1
//      8c3 8c2 7c3 7c2 6c3 6c2 6d2 6d3 7d2 7d3 8d2 8d3
//  With this symmetric arrangement, we don't have to know which absolute
//  quadrant a sample lies in to assign kernel weights; it's enough to know
//  the sample number and the relative quadrant of the sample (relative to
//  the current quadrant):
//      {current, adjacent x, adjacent y, diagonal}
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute sampling offsets within each 2x2 texel block, based
//  on appropriate 1D Gaussian sampling ratio between texels [0, 1], [2, 3],
//  and [4, 5] away from the fragment, and reuse them independently for both
//  dimensions.  Use the sample field center as the estimated destination,
//  but nudge the result closer to halfway between texels to blur error.
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  based on the sum of their 4 underlying texel weights.  Assume a same-
//  resolution blur, so each symmetrically named sample weight will compute
//  the same at every fragment in the pixel quad: We can therefore compute
//  texel weights based only on the bottom-right quadrant (fragment at 0d0).
//  Too avoid too much boilerplate code, use a macro to get all 4 texel
//  weights for a bilinear sample based on the offset of its top-left texel:
//  Statically pack weights for runtime:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result:
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   Same as tex2Dblur12x12shared()
//  Returns:    A blurred texture lookup using a "virtual" 10x10 Gaussian
//              blur (a 5x5 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  First see the description for tex2Dblur12x12shared().  This
//  function shares the same concept and sample placement, but each fragment
//  only uses 25 of the 36 samples taken across the pixel quad (to cover a
//  5x5 sample area, or 10x10 texel area), and it uses a lower standard
//  deviation to compensate.  Thanks to symmetry, the 11 omitted samples
//  are always the "same:
//      8adjx, 2adjx, 5adjx,
//      6adjy, 7adjy, 8adjy,
//      2diag, 5diag, 6diag, 7diag, 8diag
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute bilinear sampling offsets (details in tex2Dblur12x12shared).
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  from the sum of their 4 texel weights (details in tex2Dblur12x12shared).
//  We only need 25 of the 36 sample weights.  Skip the following weights:
//      8adjx, 2adjx, 5adjx,
//      6adjy, 7adjy, 8adjy,
//      2diag, 5diag, 6diag, 7diag, 8diag
//  Get the weight sum inverse (normalization factor):
//  Statically pack most weights for runtime.  Note the mixed packing:
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad in order of need:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result.  First do the simple ones:
//  Now do the mixed-sample ones:
//  Normalize the sum (so the weights add to 1.0) and return:
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   Same as tex2Dblur12x12shared()
//  Returns:    A blurred texture lookup using a "virtual" 8x8 Gaussian
//              blur (a 4x4 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  First see the description for tex2Dblur12x12shared().  This function
//  shares the same concept and a similar sample placement, except each
//  quadrant contains 4x4 texels and 2x2 samples instead of 6x6 and 3x3
//  respectively.  There could be a total of 16 samples, 4 of which each
//  fragment is responsible for, but each fragment loads 0a/0b/0c/0d with
//  its own offset to reduce shared sample artifacts, bringing the sample
//  count for each fragment to 7.  Sample placement:
//      3a 2a 2b 3b
//      1a 0a 0b 1b
//      1c 0c 0d 1d
//      3c 2c 2d 3d
//  Texel placement:
//      3a3 3a2 2a3 2a2 2b2 2b3 3b2 3b3
//      3a1 3a0 2a1 2a0 2b0 2b1 3b0 3b1
//      1a3 1a2 0a3 0a2 0b2 0b3 1b2 1b3
//      1a1 1a0 0a1 0a0 0b0 0b1 1b0 1b1
//      1c1 1c0 0c1 0c0 0d0 0d1 1d0 1d1
//      1c3 1c2 0c3 0c2 0d2 0d3 1d2 1d3
//      3c1 3c0 2c1 2c0 2d0 2d1 3d0 4d1
//      3c3 3c2 2c3 2c2 2d2 2d3 3d2 4d3
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute bilinear sampling offsets (details in tex2Dblur12x12shared).
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  from the sum of their 4 texel weights (details in tex2Dblur12x12shared).
//  Statically pack weights for runtime:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result:
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   Same as tex2Dblur12x12shared()
//  Returns:    A blurred texture lookup using a "virtual" 6x6 Gaussian
//              blur (a 3x3 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be some inaccuracies,subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  First see the description for tex2Dblur8x8shared().  This
//  function shares the same concept and sample placement, but each fragment
//  only uses 9 of the 16 samples taken across the pixel quad (to cover a
//  3x3 sample area, or 6x6 texel area), and it uses a lower standard
//  deviation to compensate.  Thanks to symmetry, the 7 omitted samples
//  are always the "same:
//      1adjx, 3adjx
//      2adjy, 3adjy
//      1diag, 2diag, 3diag
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute bilinear sampling offsets (details in tex2Dblur12x12shared).
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  from the sum of their 4 texel weights (details in tex2Dblur12x12shared).
//  We only need 9 of the 16 sample weights.  Skip the following weights:
//      1adjx, 3adjx
//      2adjy, 3adjy
//      1diag, 2diag, 3diag
//  Get the weight sum inverse (normalization factor):
//  Statically pack some weights for runtime:
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result for sample1*, and handle the rest
//  of the weights more directly/verbosely:
///////////////////////  MAX OPTIMAL SIGMA BLUR WRAPPERS  //////////////////////
//  The following blurs are static wrappers around the dynamic blurs above.
//  HOPEFULLY, the compiler will be smart enough to do constant-folding.
//  Resizable separable blurs:
//  Fast separable blurs:
//  Huge, "fast" separable blurs:
//  Resizable one-pass blurs:
//  "Fast" one-pass blurs:
//  "Fast" shared-sample one-pass blurs:
///////////////////////////////  BLOOM CONSTANTS  //////////////////////////////
//  Compute constants with manual inlines of the functions below:
///////////////////////////////////  HELPERS  //////////////////////////////////
//  Requires:   1.) triad_size is the final phosphor triad size in pixels
//              2.) thresh is the max desired pixel difference in the
//                  blurred triad (e.g. 1.0/256.0).
//  Returns:    Return the minimum sigma that will fully blur a phosphor
//              triad on the screen to an even color, within thresh.
//              This closed-form function was found by curve-fitting data.
//  Estimate: max error = ~0.086036, mean sq. error = ~0.0013387:
//  Estimate: max error = ~0.16486, mean sq. error = ~0.0041041:
//return 0.5985*triad_size - triad_size*sqrt(thresh)
//  Requires:   1.) min_expected_triads must be a global float.  The number
//                  of horizontal phosphor triads in the final image must be
//                  >= min_allowed_viewport_triads.x for realistic results.
//              2.) bloom_approx_scale_x must be a global float equal to the
//                  absolute horizontal scale of BLOOM_APPROX.
//              3.) bloom_approx_scale_x/min_allowed_viewport_triads.x
//                  should be <= 1.1658025090 to keep the final result <
//                  0.62666015625 (the largest sigma ensuring the largest
//                  unused texel weight stays < 1.0/256.0 for a 3x3 blur).
//              4.) thresh is the max desired pixel difference in the
//                  blurred triad (e.g. 1.0/256.0).
//  Returns:    Return the minimum Gaussian sigma that will blur the pass
//              output as much as it would have taken to blur away
//              bloom_approx_scale_x horizontal phosphor triads.
//  Description:
//  BLOOM_APPROX should look like a downscaled phosphor blur.  Ideally, we'd
//  use the same blur sigma as the actual phosphor bloom and scale it down
//  to the current resolution with (bloom_approx_scale_x/viewport_size_x), but
//  we don't know the viewport size in this pass.  Instead, we'll blur as
//  much as it would take to blur away min_allowed_viewport_triads.x.  This
//  will blur "more than necessary" if the user actually uses more triads,
//  but that's not terrible either, because blurring a constant fraction of
//  the viewport may better resemble a true optical bloom anyway (since the
//  viewport will generally be about the same fraction of each player's
//  field of view, regardless of screen size and resolution).
//  Assume an extremely large viewport size for asymptotic results.
//  Given a Gaussian blur sigma, get the blur weight for the center texel.
//  Note: If the implementation uses a smaller blur than the max allowed,
//  the worst case scenario is that the center weight will be overestimated,
//  so we'll put a bit more energy into the brightpass...no huge deal.
//  Then again, if the implementation uses a larger blur than the max
//  "allowed" because of dynamic branching, the center weight could be
//  underestimated, which is more of a problem...consider always using
//  43x blur:
//  31x blur:
//  25x blur:
//  17x blur:
//  9x blur:
//  If sigma is static, we can safely branch and use the smallest blur
//  that's big enough.  Ignore #define hints, because we'll only use a
//  large blur if we actually need it, and the branches cost nothing.
//  It's still worth branching if the profile supports dynamic branches:
//  It's much faster than using a hugely excessive blur, but each branch
//  eats ~1% FPS.
//  Failed optimization notes:
//  I originally created a same-size mipmapped 5-tap separable blur10 that
//  could handle any sigma by reaching into lower mip levels.  It was
//  as fast as blur25fast for runtime sigmas and a tad faster than
//  blur31fast for static sigmas, but mipmapping two viewport-size passes
//  ate 10% of FPS across all codepaths, so it wasn't worth it.
//  If we can't afford to branch, we can only guess at what blur
//  size we need.  Therefore, use the largest blur allowed.
//  Requires:   1.) output_size_x_runtime == BLOOM_APPROX.output_size.x.
//                  This is included for dynamic codepaths just in case the
//                  following two globals are incorrect:
//              2.) bloom_approx_size_x_for_skip should == the same
//                  if PHOSPHOR_BLOOM_FAKE is #defined
//              3.) bloom_approx_size_x should == the same otherwise
//  Returns:    For gaussian4x4, return a dynamic small bloom sigma that's
//              as close to optimal as possible given available information.
//              For blur3x3, return the a static small bloom sigma that
//              works well for typical cases.  Otherwise, we're using simple
//              bilinear filtering, so use static calculations.
//  Assume the default static value.  This is a compromise that ensures
//  typical triads are blurred, even if unusually large ones aren't.
//  Assume an extremely large viewport size for asymptotic results:
//  Use the runtime num triads and output size:
//  The BLOOM_APPROX input has to be ORIG_LINEARIZED to avoid moire, but
//  account for the Gaussian scanline sigma from the last pass too.
//  The bloom will be too wide horizontally but tall enough vertically.
//  We're either using blur3x3 or bilinear filtering.  The biggest
//  reason to choose blur3x3 is to avoid dynamic weights, so use a
//  static calculation.
//  The BLOOM_APPROX input has to be ORIG_LINEARIZED to avoid moire, but
//  try accounting for the Gaussian scanline sigma from the last pass
//  too; use the static default value:
//  Requires:   1.) bloom_sigma_runtime is a precalculated sigma that's
//                  optimal for the [known] triad size.
//              2.) Call this from a fragment shader (not a vertex shader),
//                  or blurring with static sigmas won't be constant-folded.
//  Returns:    Return the optimistic static sigma if the triad size is
//              known at compile time.  Otherwise return the optimal runtime
//              sigma (10% slower) or an implementation-specific compromise
//              between an optimistic or pessimistic static sigma.
//  Notes:      Call this from the fragment shader, NOT the vertex shader,
//              so static sigmas can be constant-folded!
//  Overblurring looks as bad as underblurring, so assume average-size
//  triads, not worst-case huge triads:
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file provides reusable one-pass and separable (two-pass) blurs.
//  Requires:   All blurs share these requirements (dxdy requirement is split):
//              1.) All requirements of gamma-management.h must be satisfied!
//              2.) filter_linearN must == "true" in your .cgp preset unless
//                  you're using tex2DblurNresize at 1x scale.
//              3.) mipmap_inputN must == "true" in your .cgp preset if
//                  IN.output_size < IN.video_size.
//              4.) IN.output_size == IN.video_size / pow(2, M), where M is some
//                  positive integer.  tex2Dblur*resize can resize arbitrarily
//                  (and the blur will be done after resizing), but arbitrary
//                  resizes "fail" with other blurs due to the way they mix
//                  static weights with bilinear sample exploitation.
//              5.) In general, dxdy should contain the uv pixel spacing:
//                      dxdy = (IN.video_size/IN.output_size)/IN.texture_size
//              6.) For separable blurs (tex2DblurNresize and tex2DblurNfast),
//                  zero out the dxdy component in the unblurred dimension:
//                      dxdy = float2(dxdy.x, 0.0) or float2(0.0, dxdy.y)
//              Many blurs share these requirements:
//              1.) One-pass blurs require scale_xN == scale_yN or scales > 1.0,
//                  or they will blur more in the lower-scaled dimension.
//              2.) One-pass shared sample blurs require ddx(), ddy(), and
//                  tex2Dlod() to be supported by the current Cg profile, and
//                  the drivers must support high-quality derivatives.
//              3.) One-pass shared sample blurs require:
//                      tex_uv.w == log2(IN.video_size/IN.output_size).y;
//              Non-wrapper blurs share this requirement:
//              1.) sigma is the intended standard deviation of the blur
//              Wrapper blurs share this requirement, which is automatically
//              met (unless OVERRIDE_BLUR_STD_DEVS is #defined; see below):
//              1.) blurN_std_dev must be global static const float values
//                  specifying standard deviations for Nx blurs in units
//                  of destination pixels
//  Optional:   1.) The including file (or an earlier included file) may
//                  optionally #define USE_BINOMIAL_BLUR_STD_DEVS to replace
//                  default standard deviations with those matching a binomial
//                  distribution.  (See below for details/properties.)
//              2.) The including file (or an earlier included file) may
//                  optionally #define OVERRIDE_BLUR_STD_DEVS and override:
//                      static const float blur3_std_dev
//                      static const float blur4_std_dev
//                      static const float blur5_std_dev
//                      static const float blur6_std_dev
//                      static const float blur7_std_dev
//                      static const float blur8_std_dev
//                      static const float blur9_std_dev
//                      static const float blur10_std_dev
//                      static const float blur11_std_dev
//                      static const float blur12_std_dev
//                      static const float blur17_std_dev
//                      static const float blur25_std_dev
//                      static const float blur31_std_dev
//                      static const float blur43_std_dev
//              3.) The including file (or an earlier included file) may
//                  optionally #define OVERRIDE_ERROR_BLURRING and override:
//                      static const float error_blurring
//                  This tuning value helps mitigate weighting errors from one-
//                  pass shared-sample blurs sharing bilinear samples between
//                  fragments.  Values closer to 0.0 have "correct" blurriness
//                  but allow more artifacts, and values closer to 1.0 blur away
//                  artifacts by sampling closer to halfway between texels.
//              UPDATE 6/21/14: The above static constants may now be overridden
//              by non-static uniform constants.  This permits exposing blur
//              standard deviations as runtime GUI shader parameters.  However,
//              using them keeps weights from being statically computed, and the
//              speed hit depends on the blur: On my machine, uniforms kill over
//              53% of the framerate with tex2Dblur12x12shared, but they only
//              drop the framerate by about 18% with tex2Dblur11fast.
//  Quality and Performance Comparisons:
//  For the purposes of the following discussion, "no sRGB" means
//  GAMMA_ENCODE_EVERY_FBO is #defined, and "sRGB" means it isn't.
//  1.) tex2DblurNfast is always faster than tex2DblurNresize.
//  2.) tex2DblurNresize functions are the only ones that can arbitrarily resize
//      well, because they're the only ones that don't exploit bilinear samples.
//      This also means they're the only functions which can be truly gamma-
//      correct without linear (or sRGB FBO) input, but only at 1x scale.
//  3.) One-pass shared sample blurs only have a speed advantage without sRGB.
//      They also have some inaccuracies due to their shared-[bilinear-]sample
//      design, which grow increasingly bothersome for smaller blurs and higher-
//      frequency source images (relative to their resolution).  I had high
//      hopes for them, but their most realistic use case is limited to quickly
//      reblurring an already blurred input at full resolution.  Otherwise:
//      a.) If you're blurring a low-resolution source, you want a better blur.
//      b.) If you're blurring a lower mipmap, you want a better blur.
//      c.) If you're blurring a high-resolution, high-frequency source, you
//          want a better blur.
//  4.) The one-pass blurs without shared samples grow slower for larger blurs,
//      but they're competitive with separable blurs at 5x5 and smaller, and
//      even tex2Dblur7x7 isn't bad if you're wanting to conserve passes.
//  Here are some framerates from a GeForce 8800GTS.  The first pass resizes to
//  viewport size (4x in this test) and linearizes for sRGB codepaths, and the
//  remaining passes perform 6 full blurs.  Mipmapped tests are performed at the
//  same scale, so they just measure the cost of mipmapping each FBO (only every
//  other FBO is mipmapped for separable blurs, to mimic realistic usage).
//  Mipmap      Neither     sRGB+Mipmap sRGB        Function
//  76.0        92.3        131.3       193.7       tex2Dblur3fast
//  63.2        74.4        122.4       175.5       tex2Dblur3resize
//  93.7        121.2       159.3       263.2       tex2Dblur3x3
//  59.7        68.7        115.4       162.1       tex2Dblur3x3resize
//  63.2        74.4        122.4       175.5       tex2Dblur5fast
//  49.3        54.8        100.0       132.7       tex2Dblur5resize
//  59.7        68.7        115.4       162.1       tex2Dblur5x5
//  64.9        77.2        99.1        137.2       tex2Dblur6x6shared
//  55.8        63.7        110.4       151.8       tex2Dblur7fast
//  39.8        43.9        83.9        105.8       tex2Dblur7resize
//  40.0        44.2        83.2        104.9       tex2Dblur7x7
//  56.4        65.5        71.9        87.9        tex2Dblur8x8shared
//  49.3        55.1        99.9        132.5       tex2Dblur9fast
//  33.3        36.2        72.4        88.0        tex2Dblur9resize
//  27.8        29.7        61.3        72.2        tex2Dblur9x9
//  37.2        41.1        52.6        60.2        tex2Dblur10x10shared
//  44.4        49.5        91.3        117.8       tex2Dblur11fast
//  28.8        30.8        63.6        75.4        tex2Dblur11resize
//  33.6        36.5        40.9        45.5        tex2Dblur12x12shared
//  TODO: Fill in benchmarks for new untested blurs.
//                                                  tex2Dblur17fast
//                                                  tex2Dblur25fast
//                                                  tex2Dblur31fast
//                                                  tex2Dblur43fast
//                                                  tex2Dblur3x3resize
/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////
//  Set static standard deviations, but allow users to override them with their
//  own constants (even non-static uniforms if they're okay with the speed hit):
//  blurN_std_dev values are specified in terms of dxdy strides.
//  By request, we can define standard deviations corresponding to a
//  binomial distribution with p = 0.5 (related to Pascal's triangle).
//  This distribution works such that blurring multiple times should
//  have the same result as a single larger blur.  These values are
//  larger than default for blurs up to 6x and smaller thereafter.
//  The defaults are the largest values that keep the largest unused
//  blur term on each side <= 1.0/256.0.  (We could get away with more
//  or be more conservative, but this compromise is pretty reasonable.)
//  error_blurring should be in [0.0, 1.0].  Higher values reduce ringing
//  in shared-sample blurs but increase blurring and feature shifting.
//////////////////////////////////  INCLUDES  //////////////////////////////////
//  gamma-management.h relies on pass-specific settings to guide its behavior:
//  FIRST_PASS, LAST_PASS, GAMMA_ENCODE_EVERY_FBO, etc.  See it for details.
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file provides gamma-aware tex*D*() and encode_output() functions.
//  Requires:   Before #include-ing this file, the including file must #define
//              the following macros when applicable and follow their rules:
//              1.) #define FIRST_PASS if this is the first pass.
//              2.) #define LAST_PASS if this is the last pass.
//              3.) If sRGB is available, set srgb_framebufferN = "true" for
//                  every pass except the last in your .cgp preset.
//              4.) If sRGB isn't available but you want gamma-correctness with
//                  no banding, #define GAMMA_ENCODE_EVERY_FBO each pass.
//              5.) #define SIMULATE_CRT_ON_LCD if desired (precedence over 5-7)
//              6.) #define SIMULATE_GBA_ON_LCD if desired (precedence over 6-7)
//              7.) #define SIMULATE_LCD_ON_CRT if desired (precedence over 7)
//              8.) #define SIMULATE_GBA_ON_CRT if desired (precedence over -)
//              If an option in [5, 8] is #defined in the first or last pass, it
//              should be #defined for both.  It shouldn't make a difference
//              whether it's #defined for intermediate passes or not.
//  Optional:   The including file (or an earlier included file) may optionally
//              #define a number of macros indicating it will override certain
//              macros and associated constants are as follows:
//              static constants with either static or uniform constants.  The
//              1.) OVERRIDE_STANDARD_GAMMA: The user must first define:
//                  static const float ntsc_gamma
//                  static const float pal_gamma
//                  static const float crt_reference_gamma_high
//                  static const float crt_reference_gamma_low
//                  static const float lcd_reference_gamma
//                  static const float crt_office_gamma
//                  static const float lcd_office_gamma
//              2.) OVERRIDE_DEVICE_GAMMA: The user must first define:
//                  static const float crt_gamma
//                  static const float gba_gamma
//                  static const float lcd_gamma
//              3.) OVERRIDE_FINAL_GAMMA: The user must first define:
//                  static const float input_gamma
//                  static const float intermediate_gamma
//                  static const float output_gamma
//                  (intermediate_gamma is for GAMMA_ENCODE_EVERY_FBO.)
//              4.) OVERRIDE_ALPHA_ASSUMPTIONS: The user must first define:
//                  static const bool assume_opaque_alpha
//              The gamma constant overrides must be used in every pass or none,
//              and OVERRIDE_FINAL_GAMMA bypasses all of the SIMULATE* macros.
//              OVERRIDE_ALPHA_ASSUMPTIONS may be set on a per-pass basis.
//  Usage:      After setting macros appropriately, ignore gamma correction and
//              replace all tex*D*() calls with equivalent gamma-aware
//              tex*D*_linearize calls, except:
//              1.) When you read an LUT, use regular tex*D or a gamma-specified
//                  function, depending on its gamma encoding:
//                      tex*D*_linearize_gamma (takes a runtime gamma parameter)
//              2.) If you must read pass0's original input in a later pass, use
//                  tex2D_linearize_ntsc_gamma.  If you want to read pass0's
//                  input with gamma-corrected bilinear filtering, consider
//                  creating a first linearizing pass and reading from the input
//                  of pass1 later.
//              Then, return encode_output(color) from every fragment shader.
//              Finally, use the global gamma_aware_bilinear boolean if you want
//              to statically branch based on whether bilinear filtering is
//              gamma-correct or not (e.g. for placing Gaussian blur samples).
//
//  Detailed Policy:
//  tex*D*_linearize() functions enforce a consistent gamma-management policy
//  based on the FIRST_PASS and GAMMA_ENCODE_EVERY_FBO settings.  They assume
//  their input texture has the same encoding characteristics as the input for
//  the current pass (which doesn't apply to the exceptions listed above).
//  Similarly, encode_output() enforces a policy based on the LAST_PASS and
//  GAMMA_ENCODE_EVERY_FBO settings.  Together, they result in one of the
//  following two pipelines.
//  Typical pipeline with intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = linear_color;     //  Automatic sRGB encoding
//      linear_color = intermediate_output;     //  Automatic sRGB decoding
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Typical pipeline without intermediate sRGB framebuffers:
//      linear_color = pow(pass0_encoded_color, input_gamma);
//      intermediate_output = pow(linear_color, 1.0/intermediate_gamma);
//      linear_color = pow(intermediate_output, intermediate_gamma);
//      final_output = pow(intermediate_output, 1.0/output_gamma);
//  Using GAMMA_ENCODE_EVERY_FBO is much slower, but it's provided as a way to
//  easily get gamma-correctness without banding on devices where sRGB isn't
//  supported.
//
//  Use This Header to Maximize Code Reuse:
//  The purpose of this header is to provide a consistent interface for texture
//  reads and output gamma-encoding that localizes and abstracts away all the
//  annoying details.  This greatly reduces the amount of code in each shader
//  pass that depends on the pass number in the .cgp preset or whether sRGB
//  FBO's are being used: You can trivially change the gamma behavior of your
//  whole pass by commenting or uncommenting 1-3 #defines.  To reuse the same
//  code in your first, Nth, and last passes, you can even put it all in another
//  header file and #include it from skeleton .cg files that #define the
//  appropriate pass-specific settings.
//
//  Rationale for Using Three Macros:
//  This file uses GAMMA_ENCODE_EVERY_FBO instead of an opposite macro like
//  SRGB_PIPELINE to ensure sRGB is assumed by default, which hopefully imposes
//  a lower maintenance burden on each pass.  At first glance it seems we could
//  accomplish everything with two macros: GAMMA_CORRECT_IN / GAMMA_CORRECT_OUT.
//  This works for simple use cases where input_gamma == output_gamma, but it
//  breaks down for more complex scenarios like CRT simulation, where the pass
//  number determines the gamma encoding of the input and output.
///////////////////////////////  BASE CONSTANTS  ///////////////////////////////
//  Set standard gamma constants, but allow users to override them:
//  Standard encoding gammas:
//  Typical device decoding gammas (only use for emulating devices):
//  CRT/LCD reference gammas are higher than NTSC and Rec.709 video standard
//  gammas: The standards purposely undercorrected for an analog CRT's
//  assumed 2.5 reference display gamma to maintain contrast in assumed
//  [dark] viewing conditions: http://www.poynton.com/PDFs/GammaFAQ.pdf
//  These unstated assumptions about display gamma and perceptual rendering
//  intent caused a lot of confusion, and more modern CRT's seemed to target
//  NTSC 2.2 gamma with circuitry.  LCD displays seem to have followed suit
//  (they struggle near black with 2.5 gamma anyway), especially PC/laptop
//  displays designed to view sRGB in bright environments.  (Standards are
//  also in flux again with BT.1886, but it's underspecified for displays.)
//  Assuming alpha == 1.0 might make it easier for users to avoid some bugs,
//  but only if they're aware of it.
///////////////////////  DERIVED CONSTANTS AS FUNCTIONS  ///////////////////////
//  gamma-management.h should be compatible with overriding gamma values with
//  runtime user parameters, but we can only define other global constants in
//  terms of static constants, not uniform user parameters.  To get around this
//  limitation, we need to define derived constants using functions.
//  Set device gamma constants, but allow users to override them:
//  The user promises to globally define the appropriate constants:
//  Set decoding/encoding gammas for the first/lass passes, but allow overrides:
//  The user promises to globally define the appropriate constants:
//  If we gamma-correct every pass, always use ntsc_gamma between passes to
//  ensure middle passes don't need to care if anything is being simulated:
//  Set decoding/encoding gammas for the current pass.  Use static constants for
//  linearize_input and gamma_encode_output, because they aren't derived, and
//  they let the compiler do dead-code elimination.
//  Users might want to know if bilinear filtering will be gamma-correct:
//////////////////////  COLOR ENCODING/DECODING FUNCTIONS  /////////////////////
//TODO/FIXME: I have no idea why replacing the lookup wrappers with this macro fixes the blurs being offset ¯\_(ツ)_/¯
//#define tex2D_linearize(C, D) decode_input(vec4(texture(C, D)))
// EDIT: it's the 'const' in front of the coords that's doing it
///////////////////////////  TEXTURE LOOKUP WRAPPERS  //////////////////////////
//  "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a wide array of linearizing texture lookup wrapper functions.  The
//  Cg shader spec Retroarch uses only allows for 2D textures, but 1D and 3D
//  lookups are provided for completeness in case that changes someday.  Nobody
//  is likely to use the *fetch and *proj functions, but they're included just
//  in case.  The only tex*D texture sampling functions omitted are:
//      - tex*Dcmpbias
//      - tex*Dcmplod
//      - tex*DARRAY*
//      - tex*DMS*
//      - Variants returning integers
//  Standard line length restrictions are ignored below for vertical brevity.

//  tex1D:
inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1D(tex, tex_coords));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy)
{   return decode_input(tex1D(tex, tex_coords, dx, dy));   }

inline float4 tex1D_linearize(const sampler1D tex, const float tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

inline float4 tex1D_linearize(const sampler1D tex, const float2 tex_coords, const float dx, const float dy, const int texel_off)
{   return decode_input(tex1D(tex, tex_coords, dx, dy, texel_off));    }

//  tex1Dbias:
inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dbias(tex, tex_coords));   }

inline float4 tex1Dbias_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dbias(tex, tex_coords, texel_off));    }

//  tex1Dfetch:
inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords)
{   return decode_input(tex1Dfetch(tex, tex_coords));  }

inline float4 tex1Dfetch_linearize(const sampler1D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex1Dfetch(tex, tex_coords, texel_off));   }

//  tex1Dlod:
inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords)
{   return decode_input(tex1Dlod(tex, tex_coords));    }

inline float4 tex1Dlod_linearize(const sampler1D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex1Dlod(tex, tex_coords, texel_off));     }

//  tex1Dproj:
inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords)
{   return decode_input(tex1Dproj(tex, tex_coords));   }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float2 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

inline float4 tex1Dproj_linearize(const sampler1D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex1Dproj(tex, tex_coords, texel_off));    }

//  tex2D:
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy)
//{   return decode_input(texture(tex, tex_coords, dx, dy));   }
//inline float4 tex2D_linearize(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//inline float4 tex2D_linearize(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off)
//{   return decode_input(texture(tex, tex_coords, dx, dy, texel_off));    }
//  tex2Dbias:
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords)
//{   return decode_input(tex2Dbias(tex, tex_coords));   }
//inline float4 tex2Dbias_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dbias(tex, tex_coords, texel_off));    }
//  tex2Dfetch:
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords)
//{   return decode_input(tex2Dfetch(tex, tex_coords));  }
//inline float4 tex2Dfetch_linearize(const sampler2D tex, const int4 tex_coords, const int texel_off)
//{   return decode_input(tex2Dfetch(tex, tex_coords, texel_off));   }
//  tex2Dlod:

//  tex2Dproj:
inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords)
{   return decode_input(tex2Dproj(tex, tex_coords));   }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }

inline float4 tex2Dproj_linearize(const sampler2D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex2Dproj(tex, tex_coords, texel_off));    }


//  tex3D:
inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords)
{   return decode_input(tex3D(tex, tex_coords));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, texel_off));    }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy)
{   return decode_input(tex3D(tex, tex_coords, dx, dy));   }

inline float4 tex3D_linearize(const sampler3D tex, const float3 tex_coords, const float3 dx, const float3 dy, const int texel_off)
{   return decode_input(tex3D(tex, tex_coords, dx, dy, texel_off));    }

//  tex3Dbias:
inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dbias(tex, tex_coords));   }

inline float4 tex3Dbias_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dbias(tex, tex_coords, texel_off));    }

//  tex3Dfetch:
inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords)
{   return decode_input(tex3Dfetch(tex, tex_coords));  }

inline float4 tex3Dfetch_linearize(const sampler3D tex, const int4 tex_coords, const int texel_off)
{   return decode_input(tex3Dfetch(tex, tex_coords, texel_off));   }

//  tex3Dlod:
inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dlod(tex, tex_coords));    }

inline float4 tex3Dlod_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dlod(tex, tex_coords, texel_off));     }

//  tex3Dproj:
inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords)
{   return decode_input(tex3Dproj(tex, tex_coords));   }

inline float4 tex3Dproj_linearize(const sampler3D tex, const float4 tex_coords, const int texel_off)
{   return decode_input(tex3Dproj(tex, tex_coords, texel_off));    }
/////////*

//  NONSTANDARD "SMART" LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  This narrow selection of nonstandard tex2D* functions can be useful:

//  tex2Dlod0: Automatically fill in the tex2D LOD parameter for mip level 0.
//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0)));   }

//inline float4 tex2Dlod0_linearize(const sampler2D tex, const float2 tex_coords, const int texel_off)
//{   return decode_input(tex2Dlod(tex, float4(tex_coords, 0.0, 0.0), texel_off));    }


//  MANUALLY LINEARIZING TEXTURE LOOKUP FUNCTIONS:
//  Provide a narrower selection of tex2D* wrapper functions that decode an
//  input sample with a specified gamma value.  These are useful for reading
//  LUT's and for reading the input of pass0 in a later pass.

//  tex2D:
inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords), gamma);   }

inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float3 gamma)
{   return decode_gamma_input(texture(tex, tex_coords.xy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy), gamma);   }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float2 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//inline float4 tex2D_linearize_gamma(const sampler2D tex, const float3 tex_coords, const float2 dx, const float2 dy, const int texel_off, const float3 gamma)
//{   return decode_gamma_input(texture(tex, tex_coords, dx, dy, texel_off), gamma);    }

//  tex2Dbias:
inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords), gamma);   }

inline float4 tex2Dbias_linearize_gamma(const sampler2D tex, const float4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dbias(tex, tex_coords, texel_off), gamma);    }

//  tex2Dfetch:
inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords), gamma);  }

inline float4 tex2Dfetch_linearize_gamma(const sampler2D tex, const int4 tex_coords, const int texel_off, const float3 gamma)
{   return decode_gamma_input(tex2Dfetch(tex, tex_coords, texel_off), gamma);   }

//  tex2Dlod:
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey*
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DISCLAIMER  /////////////////////////////////
//  *This code was inspired by "Shader Amortization using Pixel Quad Message
//  Passing" by Eric Penner, published in GPU Pro 2, Chapter VI.2.  My intent
//  is not to plagiarize his fundamentally similar code and assert my own
//  copyright, but the algorithmic helper functions require so little code that
//  implementations can't vary by much except bugfixes and conventions.  I just
//  wanted to license my own particular code here to avoid ambiguity and make it
//  clear that as far as I'm concerned, people can do as they please with it.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  Given screen pixel numbers, derive a "quad vector" describing a fragment's
//  position in its 2x2 pixel quad.  Given that vector, obtain the values of any
//  variable at neighboring fragments.
//  Requires:   Using this file in general requires:
//              1.) ddx() and ddy() are present in the current Cg profile.
//              2.) The GPU driver is using fine/high-quality derivatives.
//                  Functions will give incorrect results if this is not true,
//                  so a test function is included.
/////////////////////  QUAD-PIXEL COMMUNICATION PRIMITIVES  ////////////////////
//  Requires:   Two measures of the current fragment's output pixel number
//              in the range ([0, IN.output_size.x), [0, IN.output_size.y)):
//              1.) output_pixel_num_wrt_uvxy.xy increase with uv coords.
//              2.) output_pixel_num_wrt_uvxy.zw increase with screen xy.
//  Returns:    Two measures of the fragment's position in its 2x2 quad:
//              1.) The .xy components are its 2x2 placement with respect to
//                  uv direction (the origin (0, 0) is at the top-left):
//                  top-left     = (-1.0, -1.0) top-right    = ( 1.0, -1.0)
//                  bottom-left  = (-1.0,  1.0) bottom-right = ( 1.0,  1.0)
//                  You need this to arrange/weight shared texture samples.
//              2.) The .zw components are its 2x2 placement with respect to
//                  screen xy direction (IN.position); the origin varies.
//                  quad_gather needs this measure to work correctly.
//              Note: quad_vector.zw = quad_vector.xy * float2(
//                      ddx(output_pixel_num_wrt_uvxy.x),
//                      ddy(output_pixel_num_wrt_uvxy.y));
//  Caveats:    This function assumes the GPU driver always starts 2x2 pixel
//              quads at even pixel numbers.  This assumption can be wrong
//              for odd output resolutions (nondeterministically so).
//  Requires:   Same as get_quad_vector_naive() (see that first).
//  Returns:    Same as get_quad_vector_naive() (see that first), but it's
//              correct even if the 2x2 pixel quad starts at an odd pixel,
//              which can occur at odd resolutions.
//  If quad_vector_guess.zw doesn't increase with screen xy, we know
//  the 2x2 pixel quad starts at an odd pixel:
//  Requires:   1.) ddx() and ddy() are present in the current Cg profile.
//              2.) output_pixel_num_wrt_uv must increase with uv coords and
//                  measure the current fragment's output pixel number in:
//                      ([0, IN.output_size.x), [0, IN.output_size.y))
//  Returns:    Same as get_quad_vector_naive() (see that first), but it's
//              correct even if the 2x2 pixel quad starts at an odd pixel,
//              which can occur at odd resolutions.
//  Caveats:    This function requires less information than the version
//              taking a float4, but it's potentially slower.
//  Do screen coords increase with or against uv?  Get the direction
//  with respect to (uv.x, uv.y) for (screen.x, screen.y) in {-1, 1}.
//  If quad_vector_screen_guess doesn't increase with screen xy, we know
//  the 2x2 pixel quad starts at an odd pixel:
//  Requires:   1.) ddx() and ddy() are present in the current Cg profile.
//              2.) The GPU driver is using fine/high-quality derivatives.
//              3.) quad_vector describes the current fragment's location in
//                  its 2x2 pixel quad using get_quad_vector()'s conventions.
//              4.) curr is any vector you wish to get neighboring values of.
//  Returns:    Values of an input vector (curr) at neighboring fragments
//              adjacent x, adjacent y, and diagonal (via out parameters).
//  Float3 version
//  Float2 version
//  Float version:
//  Returns:    return.x == current
//              return.y == adjacent x
//              return.z == adjacent y
//              return.w == diagonal
//  Requires:   Same as quad_gather()
//  Returns:    Sum of an input vector (curr) at all fragments in a quad.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   1.) ddx() and ddy() are present in the current Cg profile.
//              2.) quad_vector describes the current fragment's location in
//                  its 2x2 pixel quad using get_quad_vector()'s conventions.
//              3.) curr must be a test vector with non-constant derivatives
//                  (its value should change nonlinearly across fragments).
//  Returns:    true if fine/hybrid/high-quality derivatives are used, or
//              false if coarse derivatives are used or inconclusive
//  Usage:      Test whether quad-pixel communication is working!
//  Method:     We can confirm fine derivatives are used if the following
//              holds (ever, for any value at any fragment):
//                  (ddy(curr) != ddy(adjx)) or (ddx(curr) != ddx(adjy))
//              The more values we test (e.g. test a float4 two ways), the
//              easier it is to demonstrate fine derivatives are working.
//  TODO: Check for floating point exact comparison issues!
//  Requires:   Same as fine_derivatives_working()
//  Returns:    Same as fine_derivatives_working()
//  Usage:      This is faster than fine_derivatives_working() but more
//              likely to return false negatives, so it's less useful for
//              offline testing/debugging.  It's also useless as the basis
//              for dynamic runtime branching as of May 2014: Derivatives
//              (and quad-pixel communication) are currently disallowed in
//              branches.  However, future GPU's may allow you to use them
//              in dynamic branches if you promise the branch condition
//              evaluates the same for every fragment in the quad (and/or if
//              the driver enforces that promise by making a single fragment
//              control branch decisions).  If that ever happens, this
//              version may become a more economical choice.
/////////////////////////////////  MIT LICENSE  ////////////////////////////////
//  Copyright (C) 2014 TroggleMonkey
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file implements the following mathematical special functions:
//  1.) erf() = 2/sqrt(pi) * indefinite_integral(e**(-x**2))
//  2.) gamma(s), a real-numbered extension of the integer factorial function
//  It also implements normalized_ligamma(s, z), a normalized lower incomplete
//  gamma function for s < 0.5 only.  Both gamma() and normalized_ligamma() can
//  be called with an _impl suffix to use an implementation version with a few
//  extra precomputed parameters (which may be useful for the caller to reuse).
//  See below for details.
//
//  Design Rationale:
//  Pretty much every line of code in this file is duplicated four times for
//  different input types (float4/float3/float2/float).  This is unfortunate,
//  but Cg doesn't allow function templates.  Macros would be far less verbose,
//  but they would make the code harder to document and read.  I don't expect
//  these functions will require a whole lot of maintenance changes unless
//  someone ever has need for more robust incomplete gamma functions, so code
//  duplication seems to be the lesser evil in this case.
///////////////////////////  GAUSSIAN ERROR FUNCTION  //////////////////////////
//  Requires:   x is the standard parameter to erf().
//  Returns:    Return an Abramowitz/Stegun approximation of erf(), where:
//                  erf(x) = 2/sqrt(pi) * integral(e**(-x**2))
//              This approximation has a max absolute error of 2.5*10**-5
//              with solid numerical robustness and efficiency.  See:
//                  https://en.wikipedia.org/wiki/Error_function#Approximation_with_elementary_functions
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   x is the standard parameter to erf().
//  Returns:    Approximate erf() with the hyperbolic tangent.  The error is
//              visually noticeable, but it's blazing fast and perceptually
//              close...at least on ATI hardware.  See:
//                  http://www.maplesoft.com/applications/view.aspx?SID=5525&view=html
//  Warning:    Only use this if your hardware drivers correctly implement
//              tanh(): My nVidia 8800GTS returns garbage output.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   x is the standard parameter to erf().
//  Returns:    Some approximation of erf(x), depending on user settings.
//  Float3 version:
//  Float2 version:
//  Float version:
///////////////////////////  COMPLETE GAMMA FUNCTION  //////////////////////////
//  Requires:   1.) s is the standard parameter to the gamma function, and
//                  it should lie in the [0, 36] range.
//              2.) s_inv = 1.0/s.  This implementation function requires
//                  the caller to precompute this value, giving users the
//                  opportunity to reuse it.
//  Returns:    Return approximate gamma function (real-numbered factorial)
//              output using the Lanczos approximation with two coefficients
//              calculated using Paul Godfrey's method here:
//                  http://my.fit.edu/~gabdo/gamma.txt
//              An optimal g value for s in [0, 36] is ~1.12906830989, with
//              a maximum relative error of 0.000463 for 2**16 equally
//              evals.  We could use three coeffs (0.0000346 error) without
//              hurting latency, but this allows more parallelism with
//              outside instructions.
//  gamma(s + 1) = base**sph * lanczos_sum; divide by s for gamma(s).
//  This has less error for small s's than (s -= 1.0) at the beginning.
//  Float3 version:
//  Float2 version:
//  Float version:
//  Requires:   s is the standard parameter to the gamma function, and it
//              should lie in the [0, 36] range.
//  Returns:    Return approximate gamma function output with a maximum
//              relative error of 0.000463.  See gamma_impl for details.
//  Float3 version:
//  Float2 version:
//  Float version:
////////////////  INCOMPLETE GAMMA FUNCTIONS (RESTRICTED INPUT)  ///////////////
//  Lower incomplete gamma function for small s and z (implementation):
//  Requires:   1.) s < ~0.5
//              2.) z <= ~0.775075
//              3.) s_inv = 1.0/s (precomputed for outside reuse)
//  Returns:    A series representation for the lower incomplete gamma
//              function for small s and small z (4 terms).
//  The actual "rolled up" summation looks like:
//      last_sign = 1.0; last_pow = 1.0; last_factorial = 1.0;
//      sum = last_sign * last_pow / ((s + k) * last_factorial)
//      for(int i = 0; i < 4; ++i)
//      {
//          last_sign *= -1.0; last_pow *= z; last_factorial *= i;
//          sum += last_sign * last_pow / ((s + k) * last_factorial);
//      }
//  Unrolled, constant-unfolded and arranged for madds and parallelism:
//  Summation iterations 1, 2, and 3:
//float4 denom4 = 24.0*s + float4(96.0);
//sum += z_sq * z_sq / denom4;
//  Scale and return:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Upper incomplete gamma function for small s and large z (implementation):
//  Requires:   1.) s < ~0.5
//              2.) z > ~0.775075
//  Returns:    Gauss's continued fraction representation for the upper
//              incomplete gamma function (4 terms).
//  The "rolled up" continued fraction looks like this.  The denominator
//  is truncated, and it's calculated "from the bottom up:
//      denom = float4('inf');
//      float4 one = float4(1.0);
//      for(int i = 4; i > 0; --i)
//      {
//          denom = ((i * 2.0) - one) + z - s + (i * (s - i))/denom;
//      }
//  Unrolled and constant-unfolded for madds and parallelism:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Normalized lower incomplete gamma function for small s (implementation):
//  Requires:   1.) s < ~0.5
//              2.) s_inv = 1/s (precomputed for outside reuse)
//              3.) gamma_s_inv = 1/gamma(s) (precomputed for outside reuse)
//  Returns:    Approximate the normalized lower incomplete gamma function
//              for s < 0.5.  Since we only care about s < 0.5, we only need
//              to evaluate two branches (not four) based on z.  Each branch
//              uses four terms, with a max relative error of ~0.00182.  The
//              branch threshold and specifics were adapted for fewer terms
//              from Gil/Segura/Temme's paper here:
//                  http://oai.cwi.nl/oai/asset/20433/20433B.pdf
//  Evaluate both branches: Real branches test slower even when available.
//  Combine the results from both branches:
//  Float3 version:
//  Float2 version:
//  Float version:
//  Normalized lower incomplete gamma function for small s:
//  Requires:   s < ~0.5
//  Returns:    Approximate the normalized lower incomplete gamma function
//              for s < 0.5.  See normalized_ligamma_impl() for details.
//  Float3 version:
//  Float2 version:
//  Float version:
///////////////////////////////////  HELPERS  //////////////////////////////////
//  Make a float2 uv offset safe for adding to float4 tex2Dlod coords:
//  Make a length squared helper macro (for usage with static constants):
//  We can use the Gaussian integral to calculate the asymptotic weight for
//  the center pixel.  Since the unnormalized center pixel weight is 1.0,
//  the normalized weight is the same as the weight sum inverse.  Given a
//  large enough blur (9+), the asymptotic weight sum is close and faster:
//      center_weight = 0.5 *
//          (erf(0.5/(sigma*sqrt(2.0))) - erf(-0.5/(sigma*sqrt(2.0))))
//      erf(-x) == -erf(x), so we get 0.5 * (2.0 * erf(blah blah)):
//  However, we can get even faster results with curve-fitting.  These are
//  also closer than the asymptotic results, because they were constructed
//  from 64 blurs sizes from [3, 131) and 255 equally-spaced sigmas from
//  (0, blurN_std_dev), so the results for smaller sigmas are biased toward
//  smaller blurs.  The max error is 0.0031793913.
//  Relative FPS: 134.3 with erf, 135.8 with curve-fitting.
//static const float temp = 0.5/sqrt(2.0);
//return erf(temp/sigma);
////////////////////  ARBITRARILY RESIZABLE SEPARABLE BLURS  ///////////////////
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 11x Gaussian blurred texture lookup using a 11-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  Calculate Gaussian blur kernel weights and a normalization factor for
//  distances of 0-4, ignoring constant factors (since we're normalizing).
//  Statically normalize weights, sum weighted samples, and return.  Blurs are
//  currently optimized for dynamic weights.
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 9x Gaussian blurred texture lookup using a 9-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 7x Gaussian blurred texture lookup using a 7-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 5x Gaussian blurred texture lookup using a 5-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 1D 3x Gaussian blurred texture lookup using a 3-tap blur.
//              It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Statically normalize weights, sum weighted samples, and return:
///////////////////////////  FAST SEPARABLE BLURS  ///////////////////////////
//  Requires:   1.) Global requirements must be met (see file description).
//              2.) filter_linearN must = "true" in your .cgp file.
//              3.) For gamma-correct bilinear filtering, global
//                  gamma_aware_bilinear == true (from gamma-management.h)
//  Returns:    A 1D 11x Gaussian blurred texture lookup using 6 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 9x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 4 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 7x Gaussian blurred texture lookup using 4 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 5x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 2 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 3x Gaussian blurred texture lookup using 2 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Weights for all samples are the same, so just average them:
////////////////////////////  HUGE SEPARABLE BLURS  ////////////////////////////
//  Huge separable blurs come only in "fast" versions.
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 43x Gaussian blurred texture lookup using 22 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 /
//    (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + w10 + w11 +
//        w12 + w13 + w14 + w15 + w16 + w17 + w18 + w19 + w20 + w21));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 31x Gaussian blurred texture lookup using 16 linear
//              taps.  It may be mipmapped depending on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 /
//    (w0 + 2.0 * (w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 +
//        w9 + w10 + w11 + w12 + w13 + w14 + w15));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  The center texel (with weight w0) is used twice, so halve its weight.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 25x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 12 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 / (w0 + 2.0 * (
//    w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + w10 + w11 + w12));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
//  Requires:   Same as tex2Dblur11()
//  Returns:    A 1D 17x Gaussian blurred texture lookup using 1 nearest
//              neighbor and 8 linear taps.  It may be mipmapped depending
//              on settings and dxdy.
//  First get the texel weights and normalization factor as above.
//const float weight_sum_inv = 1.0 / (w0 + 2.0 * (
//    w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8));
//  Calculate combined weights and linear sample ratios between texel pairs.
//  Statically normalize weights, sum weighted samples, and return:
////////////////////  ARBITRARILY RESIZABLE ONE-PASS BLURS  ////////////////////
//  Requires:   Global requirements must be met (see file description).
//  Returns:    A 3x3 Gaussian blurred mipmapped texture lookup of the
//              resized input.
//  Description:
//  This is the only arbitrarily resizable one-pass blur; tex2Dblur5x5resize
//  would perform like tex2Dblur9x9, MUCH slower than tex2Dblur5resize.
//  Load each sample.  We need all 3x3 samples.  Quad-pixel communication
//  won't help either: This should perform like tex2Dblur5x5, but sharing a
//  4x4 sample field would perform more like tex2Dblur8x8shared (worse).
//  Statically compute Gaussian sample weights:
//  Weight and sum the samples:
////////////////////////////  FASTER ONE-PASS BLURS  ///////////////////////////
//  Perform a 1-pass 9x9 blur with 5x5 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 9x9 Gaussian blurred mipmapped texture lookup composed of
//              5x5 carefully selected bilinear samples.
//  Description:
//  Perform a 1-pass 9x9 blur with 5x5 bilinear samples.  Adjust the
//  bilinear sample location to reflect the true Gaussian weights for each
//  underlying texel.  The following diagram illustrates the relative
//  locations of bilinear samples.  Each sample with the same number has the
//  same weight (notice the symmetry).  The letters a, b, c, d distinguish
//  quadrants, and the letters U, D, L, R, C (up, down, left, right, center)
//  distinguish 1D directions along the line containing the pixel center:
//      6a 5a 2U 5b 6b
//      4a 3a 1U 3b 4b
//      2L 1L 0C 1R 2R
//      4c 3c 1D 3d 4d
//      6c 5c 2D 5d 6d
//  The following diagram illustrates the underlying equally spaced texels,
//  named after the sample that accesses them and subnamed by their location
//  within their 2x2, 2x1, 1x2, or 1x1 texel block:
//      6a4 6a3 5a4 5a3 2U2 5b3 5b4 6b3 6b4
//      6a2 6a1 5a2 5a1 2U1 5b1 5b2 6b1 6b2
//      4a4 4a3 3a4 3a3 1U2 3b3 3b4 4b3 4b4
//      4a2 4a1 3a2 3a1 1U1 3b1 3b2 4b1 4b2
//      2L2 2L1 1L2 1L1 0C1 1R1 1R2 2R1 2R2
//      4c2 4c1 3c2 3c1 1D1 3d1 3d2 4d1 4d2
//      4c4 4c3 3c4 3c3 1D2 3d3 3d4 4d3 4d4
//      6c2 6c1 5c2 5c1 2D1 5d1 5d2 6d1 6d2
//      6c4 6c3 5c4 5c3 2D2 5d3 5d4 6d3 6d4
//  Note there is only one C texel and only two texels for each U, D, L, or
//  R sample.  The center sample is effectively a nearest neighbor sample,
//  and the U/D/L/R samples use 1D linear filtering.  All other texels are
//  read with bilinear samples somewhere within their 2x2 texel blocks.
//  COMPUTE TEXTURE COORDS:
//  Statically compute sampling offsets within each 2x2 texel block, based
//  on 1D sampling ratios between texels [1, 2] and [3, 4] texels away from
//  the center, and reuse them independently for both dimensions.  Compute
//  these offsets based on the relative 1D Gaussian weights of the texels
//  in question.  (w1off means "Gaussian weight for the texel 1.0 texels
//  away from the pixel center," etc.).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including x-axis-aligned:
//  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
//  Statically compute Gaussian texel weights for the bottom-right quadrant.
//  Read underscores as "and.
//  Statically add texel weights in each sample to get sample weights:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES:
//  Load all 25 samples (1 nearest, 8 linear, 16 bilinear) using symmetry:
//  Sampling order doesn't seem to affect performance, so just be clear:
//  SUM WEIGHTED SAMPLES:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Perform a 1-pass 7x7 blur with 5x5 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 7x7 Gaussian blurred mipmapped texture lookup composed of
//              4x4 carefully selected bilinear samples.
//  Description:
//  First see the descriptions for tex2Dblur9x9() and tex2Dblur7().  This
//  blur mixes concepts from both.  The sample layout is as follows:
//      4a 3a 3b 4b
//      2a 1a 1b 2b
//      2c 1c 1d 2d
//      4c 3c 3d 4d
//  The texel layout is as follows.  Note that samples 3a/3b, 1a/1b, 1c/1d,
//  and 3c/3d share a vertical column of texels, and samples 2a/2c, 1a/1c,
//  1b/1d, and 2b/2d share a horizontal row of texels (all sample1's share
//  the center texel):
//      4a4  4a3  3a4  3ab3 3b4  4b3  4b4
//      4a2  4a1  3a2  3ab1 3b2  4b1  4b2
//      2a4  2a3  1a4  1ab3 1b4  2b3  2b4
//      2ac2 2ac1 1ac2 1*   1bd2 2bd1 2bd2
//      2c4  2c3  1c4  1cd3 1d4  2d3  2d4
//      4c2  4c1  3c2  3cd1 3d2  4d1  4d2
//      4c4  4c3  3c4  3cd3 3d4  4d3  4d4
//  COMPUTE TEXTURE COORDS:
//  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including axis-aligned:
//  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
//  Statically compute Gaussian texel weights for the bottom-right quadrant.
//  Read underscores as "and.
//  Statically add texel weights in each sample to get sample weights.
//  Split weights for shared texels between samples sharing them:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES:
//  Load all 16 samples using symmetry:
//  SUM WEIGHTED SAMPLES:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Perform a 1-pass 5x5 blur with 3x3 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 5x5 Gaussian blurred mipmapped texture lookup composed of
//              3x3 carefully selected bilinear samples.
//  Description:
//  First see the description for tex2Dblur9x9().  This blur uses the same
//  concept and sample/texel locations except on a smaller scale.  Samples:
//      2a 1U 2b
//      1L 0C 1R
//      2c 1D 2d
//  Texels:
//      2a4 2a3 1U2 2b3 2b4
//      2a2 2a1 1U1 2b1 2b2
//      1L2 1L1 0C1 1R1 1R2
//      2c2 2c1 1D1 2d1 2d2
//      2c4 2c3 1D2 2d3 2d4
//  COMPUTE TEXTURE COORDS:
//  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including x-axis-aligned:
//  CALCULATE KERNEL WEIGHTS FOR ALL SAMPLES:
//  Statically compute Gaussian texel weights for the bottom-right quadrant.
//  Read underscores as "and.
//  Statically add texel weights in each sample to get sample weights:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES:
//  Load all 9 samples (1 nearest, 4 linear, 4 bilinear) using symmetry:
//  SUM WEIGHTED SAMPLES:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Perform a 1-pass 3x3 blur with 5x5 bilinear samples.
//  Requires:   Same as tex2Dblur9()
//  Returns:    A 3x3 Gaussian blurred mipmapped texture lookup composed of
//              2x2 carefully selected bilinear samples.
//  Description:
//  First see the descriptions for tex2Dblur9x9() and tex2Dblur7().  This
//  blur mixes concepts from both.  The sample layout is as follows:
//      0a 0b
//      0c 0d
//  The texel layout is as follows.  Note that samples 0a/0b and 0c/0d share
//  a vertical column of texels, and samples 0a/0c and 0b/0d share a
//  horizontal row of texels (all samples share the center texel):
//      0a3  0ab2 0b3
//      0ac1 0*0  0bd1
//      0c3  0cd2 0d3
//  COMPUTE TEXTURE COORDS:
//  Statically compute bilinear sampling offsets (details in tex2Dblur9x9).
//  Statically compute texel offsets from the fragment center to each
//  bilinear sample in the bottom-right quadrant, including axis-aligned:
//  LOAD TEXTURE SAMPLES:
//  Load all 4 samples using symmetry:
//  SUM WEIGHTED SAMPLES:
//  Weights for all samples are the same, so just average them:
//////////////////  LINEAR ONE-PASS BLURS WITH SHARED SAMPLES  /////////////////
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   1.) Same as tex2Dblur9()
//              2.) ddx() and ddy() are present in the current Cg profile.
//              3.) The GPU driver is using fine/high-quality derivatives.
//              4.) quad_vector *correctly* describes the current fragment's
//                  location in its pixel quad, by the conventions noted in
//                  get_quad_vector[_naive].
//              5.) tex_uv.w = log2(IN.video_size/IN.output_size).y
//              6.) tex2Dlod() is present in the current Cg profile.
//  Optional:   Tune artifacts vs. excessive blurriness with the global
//              float error_blurring.
//  Returns:    A blurred texture lookup using a "virtual" 12x12 Gaussian
//              blur (a 6x6 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  Perform a 1-pass blur with shared texture lookups across a pixel quad.
//  We'll get neighboring samples with high-quality ddx/ddy derivatives, as
//  in GPU Pro 2, Chapter VI.2, "Shader Amortization using Pixel Quad
//  Message Passing" by Eric Penner.
//
//  Our "virtual" 12x12 blur will be comprised of ((6 - 1)^2)/4 + 3 = 12
//  bilinear samples, where bilinear sampling positions are computed from
//  the relative Gaussian weights of the 4 surrounding texels.  The catch is
//  that the appropriate texel weights and sample coords differ for each
//  fragment, but we're reusing most of the same samples across a quad of
//  destination fragments.  (We do use unique coords for the four nearest
//  samples at each fragment.)  Mixing bilinear filtering and sample-sharing
//  therefore introduces some error into the weights, and this can get nasty
//  when the source image is small or high-frequency.  Computing bilinear
//  ratios based on weights at the sample field center results in sharpening
//  and ringing artifacts, but we can move samples closer to halfway between
//  texels to try blurring away the error (which can move features around by
//  a texel or so).  Tune this with the global float "error_blurring".
//
//  The pixel quad's sample field covers 12x12 texels, accessed through 6x6
//  bilinear (2x2 texel) taps.  Each fragment depends on a window of 10x10
//  texels (5x5 bilinear taps), and each fragment is responsible for loading
//  a 6x6 texel quadrant as a 3x3 block of bilinear taps, plus 3 more taps
//  to use unique bilinear coords for sample0* for each fragment.  This
//  diagram illustrates the relative locations of bilinear samples 1-9 for
//  each quadrant a, b, c, d (note samples will not be equally spaced):
//      8a 7a 6a 6b 7b 8b
//      5a 4a 3a 3b 4b 5b
//      2a 1a 0a 0b 1b 2b
//      2c 1c 0c 0d 1d 2d
//      5c 4c 3c 3d 4d 5d
//      8c 7c 6c 6d 7d 8d
//  The following diagram illustrates the underlying equally spaced texels,
//  named after the sample that accesses them and subnamed by their location
//  within their 2x2 texel block:
//      8a3 8a2 7a3 7a2 6a3 6a2 6b2 6b3 7b2 7b3 8b2 8b3
//      8a1 8a0 7a1 7a0 6a1 6a0 6b0 6b1 7b0 7b1 8b0 8b1
//      5a3 5a2 4a3 4a2 3a3 3a2 3b2 3b3 4b2 4b3 5b2 5b3
//      5a1 5a0 4a1 4a0 3a1 3a0 3b0 3b1 4b0 4b1 5b0 5b1
//      2a3 2a2 1a3 1a2 0a3 0a2 0b2 0b3 1b2 1b3 2b2 2b3
//      2a1 2a0 1a1 1a0 0a1 0a0 0b0 0b1 1b0 1b1 2b0 2b1
//      2c1 2c0 1c1 1c0 0c1 0c0 0d0 0d1 1d0 1d1 2d0 2d1
//      2c3 2c2 1c3 1c2 0c3 0c2 0d2 0d3 1d2 1d3 2d2 2d3
//      5c1 5c0 4c1 4c0 3c1 3c0 3d0 3d1 4d0 4d1 5d0 5d1
//      5c3 5c2 4c3 4c2 3c3 3c2 3d2 3d3 4d2 4d3 5d2 5d3
//      8c1 8c0 7c1 7c0 6c1 6c0 6d0 6d1 7d0 7d1 8d0 8d1
//      8c3 8c2 7c3 7c2 6c3 6c2 6d2 6d3 7d2 7d3 8d2 8d3
//  With this symmetric arrangement, we don't have to know which absolute
//  quadrant a sample lies in to assign kernel weights; it's enough to know
//  the sample number and the relative quadrant of the sample (relative to
//  the current quadrant):
//      {current, adjacent x, adjacent y, diagonal}
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute sampling offsets within each 2x2 texel block, based
//  on appropriate 1D Gaussian sampling ratio between texels [0, 1], [2, 3],
//  and [4, 5] away from the fragment, and reuse them independently for both
//  dimensions.  Use the sample field center as the estimated destination,
//  but nudge the result closer to halfway between texels to blur error.
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  based on the sum of their 4 underlying texel weights.  Assume a same-
//  resolution blur, so each symmetrically named sample weight will compute
//  the same at every fragment in the pixel quad: We can therefore compute
//  texel weights based only on the bottom-right quadrant (fragment at 0d0).
//  Too avoid too much boilerplate code, use a macro to get all 4 texel
//  weights for a bilinear sample based on the offset of its top-left texel:
//  Statically pack weights for runtime:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result:
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   Same as tex2Dblur12x12shared()
//  Returns:    A blurred texture lookup using a "virtual" 10x10 Gaussian
//              blur (a 5x5 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  First see the description for tex2Dblur12x12shared().  This
//  function shares the same concept and sample placement, but each fragment
//  only uses 25 of the 36 samples taken across the pixel quad (to cover a
//  5x5 sample area, or 10x10 texel area), and it uses a lower standard
//  deviation to compensate.  Thanks to symmetry, the 11 omitted samples
//  are always the "same:
//      8adjx, 2adjx, 5adjx,
//      6adjy, 7adjy, 8adjy,
//      2diag, 5diag, 6diag, 7diag, 8diag
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute bilinear sampling offsets (details in tex2Dblur12x12shared).
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  from the sum of their 4 texel weights (details in tex2Dblur12x12shared).
//  We only need 25 of the 36 sample weights.  Skip the following weights:
//      8adjx, 2adjx, 5adjx,
//      6adjy, 7adjy, 8adjy,
//      2diag, 5diag, 6diag, 7diag, 8diag
//  Get the weight sum inverse (normalization factor):
//  Statically pack most weights for runtime.  Note the mixed packing:
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad in order of need:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result.  First do the simple ones:
//  Now do the mixed-sample ones:
//  Normalize the sum (so the weights add to 1.0) and return:
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   Same as tex2Dblur12x12shared()
//  Returns:    A blurred texture lookup using a "virtual" 8x8 Gaussian
//              blur (a 4x4 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  First see the description for tex2Dblur12x12shared().  This function
//  shares the same concept and a similar sample placement, except each
//  quadrant contains 4x4 texels and 2x2 samples instead of 6x6 and 3x3
//  respectively.  There could be a total of 16 samples, 4 of which each
//  fragment is responsible for, but each fragment loads 0a/0b/0c/0d with
//  its own offset to reduce shared sample artifacts, bringing the sample
//  count for each fragment to 7.  Sample placement:
//      3a 2a 2b 3b
//      1a 0a 0b 1b
//      1c 0c 0d 1d
//      3c 2c 2d 3d
//  Texel placement:
//      3a3 3a2 2a3 2a2 2b2 2b3 3b2 3b3
//      3a1 3a0 2a1 2a0 2b0 2b1 3b0 3b1
//      1a3 1a2 0a3 0a2 0b2 0b3 1b2 1b3
//      1a1 1a0 0a1 0a0 0b0 0b1 1b0 1b1
//      1c1 1c0 0c1 0c0 0d0 0d1 1d0 1d1
//      1c3 1c2 0c3 0c2 0d2 0d3 1d2 1d3
//      3c1 3c0 2c1 2c0 2d0 2d1 3d0 4d1
//      3c3 3c2 2c3 2c2 2d2 2d3 3d2 4d3
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute bilinear sampling offsets (details in tex2Dblur12x12shared).
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  from the sum of their 4 texel weights (details in tex2Dblur12x12shared).
//  Statically pack weights for runtime:
//  Get the weight sum inverse (normalization factor):
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result:
//  Perform a 1-pass mipmapped blur with shared samples across a pixel quad.
//  Requires:   Same as tex2Dblur12x12shared()
//  Returns:    A blurred texture lookup using a "virtual" 6x6 Gaussian
//              blur (a 3x3 blur of carefully selected bilinear samples)
//              of the given mip level.  There will be some inaccuracies,subtle inaccuracies,
//              especially for small or high-frequency detailed sources.
//  Description:
//  First see the description for tex2Dblur8x8shared().  This
//  function shares the same concept and sample placement, but each fragment
//  only uses 9 of the 16 samples taken across the pixel quad (to cover a
//  3x3 sample area, or 6x6 texel area), and it uses a lower standard
//  deviation to compensate.  Thanks to symmetry, the 7 omitted samples
//  are always the "same:
//      1adjx, 3adjx
//      2adjy, 3adjy
//      1diag, 2diag, 3diag
//  COMPUTE COORDS FOR TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Statically compute bilinear sampling offsets (details in tex2Dblur12x12shared).
//  We don't share sample0*, so use the nearest destination fragment:
//  Statically compute texel offsets from the bottom-right fragment to each
//  bilinear sample in the bottom-right quadrant:
//  CALCULATE KERNEL WEIGHTS:
//  Statically compute bilinear sample weights at each destination fragment
//  from the sum of their 4 texel weights (details in tex2Dblur12x12shared).
//  We only need 9 of the 16 sample weights.  Skip the following weights:
//      1adjx, 3adjx
//      2adjy, 3adjy
//      1diag, 2diag, 3diag
//  Get the weight sum inverse (normalization factor):
//  Statically pack some weights for runtime:
//  LOAD TEXTURE SAMPLES THIS FRAGMENT IS RESPONSIBLE FOR:
//  Get a uv vector from texel 0q0 of this quadrant to texel 0q3:
//  Load bilinear samples for the current quadrant (for this fragment):
//  GATHER NEIGHBORING SAMPLES AND SUM WEIGHTED SAMPLES:
//  Fetch the samples from other fragments in the 2x2 quad:
//  Statically normalize weights (so total = 1.0), and sum weighted samples.
//  Fill each row of a matrix with an rgb sample and pre-multiply by the
//  weights to obtain a weighted result for sample1*, and handle the rest
//  of the weights more directly/verbosely:
///////////////////////  MAX OPTIMAL SIGMA BLUR WRAPPERS  //////////////////////
//  The following blurs are static wrappers around the dynamic blurs above.
//  HOPEFULLY, the compiler will be smart enough to do constant-folding.
//  Resizable separable blurs:
//  Fast separable blurs:
//  Huge, "fast" separable blurs:
//  Resizable one-pass blurs:
//  "Fast" one-pass blurs:
//  "Fast" shared-sample one-pass blurs:
//  Sample the masked scanlines:
//  Get the full intensity, including auto-undimming, and mask compensation:
//  Sample BLOOM_APPROX to estimate what a straight blur of masked scanlines
//  would look like, so we can estimate how much energy we'll receive from
//  blooming neighbors:
//  Compute the blur weight for the center texel and the maximum energy we
//  expect to receive from neighbors:
//  Assume neighbors will blur 100% of their intensity (blur_ratio = 1.0),
//  because it actually gets better results (on top of being very simple),
//  but adjust all intensities for the user's desired underestimate factor:
//  Calculate the blur_ratio, the ratio of intensity we want to blur:
//  This area-based version changes blur_ratio more smoothly and blurs
//  more, clipping less but offering less phosphor differentiation:
//  Calculate the brightpass based on the auto-dimmed, unamplified, masked
//  scanlines, encode if necessary, and return!

*/

#pragma once

namespace RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,9,196,
10,175,184,153,7,124,
234,238,187,114,239,168,
122,229,1,0,0,0,
116,20,0,0,5,0,
0,0,52,0,0,0,
52,15,0,0,128,15,
0,0,8,16,0,0,
216,19,0,0,82,68,
69,70,248,14,0,0,
2,0,0,0,136,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
208,14,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
124,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,1,0,
0,0,128,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,1,0,0,0,
0,0,0,0,85,66,
79,0,80,117,115,104,
0,171,171,171,124,0,
0,0,47,0,0,0,
184,0,0,0,0,1,
0,0,0,0,0,0,
0,0,0,0,128,0,
0,0,6,0,0,0,
8,13,0,0,96,0,
0,0,0,0,0,0,
0,0,0,0,16,8,
0,0,0,0,0,0,
64,0,0,0,2,0,
0,0,36,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,72,8,0,0,
64,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
132,8,0,0,68,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,149,8,
0,0,72,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,172,8,0,0,
76,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
195,8,0,0,80,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,219,8,
0,0,84,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,253,8,0,0,
88,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
17,9,0,0,92,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,39,9,
0,0,96,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,61,9,0,0,
100,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
84,9,0,0,104,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,106,9,
0,0,108,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,128,9,0,0,
112,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
152,9,0,0,116,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,177,9,
0,0,120,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,201,9,0,0,
124,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
237,9,0,0,128,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,11,10,
0,0,132,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,41,10,0,0,
136,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
71,10,0,0,140,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,101,10,
0,0,144,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,131,10,0,0,
148,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
161,10,0,0,152,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,178,10,
0,0,156,0,0,0,
4,0,0,0,2,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,210,10,0,0,
160,0,0,0,4,0,
0,0,2,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
241,10,0,0,164,0,
0,0,4,0,0,0,
2,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,16,11,
0,0,168,0,0,0,
4,0,0,0,2,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,47,11,0,0,
172,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
85,11,0,0,176,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,123,11,
0,0,180,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,141,11,0,0,
184,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
163,11,0,0,188,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,188,11,
0,0,192,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,207,11,0,0,
196,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
229,11,0,0,200,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,254,11,
0,0,204,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,23,12,0,0,
208,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
50,12,0,0,212,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,77,12,
0,0,216,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,100,12,0,0,
220,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
123,12,0,0,224,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,142,12,
0,0,228,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,165,12,0,0,
232,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
188,12,0,0,236,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,209,12,
0,0,240,0,0,0,
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,232,12,0,0,
244,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
103,108,111,98,97,108,
95,77,86,80,0,102,
108,111,97,116,52,120,
52,0,2,0,3,0,
4,0,4,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,27,8,
0,0,103,108,111,98,
97,108,95,99,114,116,
95,103,97,109,109,97,
0,102,108,111,97,116,
0,171,0,0,3,0,
1,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,89,8,
0,0,103,108,111,98,
97,108,95,108,99,100,
95,103,97,109,109,97,
0,103,108,111,98,97,
108,95,108,101,118,101,
108,115,95,99,111,110,
116,114,97,115,116,0,
103,108,111,98,97,108,
95,104,97,108,97,116,
105,111,110,95,119,101,
105,103,104,116,0,103,
108,111,98,97,108,95,
100,105,102,102,117,115,
105,111,110,95,119,101,
105,103,104,116,0,103,
108,111,98,97,108,95,
98,108,111,111,109,95,
117,110,100,101,114,101,
115,116,105,109,97,116,
101,95,108,101,118,101,
108,115,0,103,108,111,
98,97,108,95,98,108,
111,111,109,95,101,120,
99,101,115,115,0,103,
108,111,98,97,108,95,
98,101,97,109,95,109,
105,110,95,115,105,103,
109,97,0,103,108,111,
98,97,108,95,98,101,
97,109,95,109,97,120,
95,115,105,103,109,97,
0,103,108,111,98,97,
108,95,98,101,97,109,
95,115,112,111,116,95,
112,111,119,101,114,0,
103,108,111,98,97,108,
95,98,101,97,109,95,
109,105,110,95,115,104,
97,112,101,0,103,108,
111,98,97,108,95,98,
101,97,109,95,109,97,
120,95,115,104,97,112,
101,0,103,108,111,98,
97,108,95,98,101,97,
109,95,115,104,97,112,
101,95,112,111,119,101,
114,0,103,108,111,98,
97,108,95,98,101,97,
109,95,104,111,114,105,
122,95,102,105,108,116,
101,114,0,103,108,111,
98,97,108,95,98,101,
97,109,95,104,111,114,
105,122,95,115,105,103,
109,97,0,103,108,111,
98,97,108,95,98,101,
97,109,95,104,111,114,
105,122,95,108,105,110,
101,97,114,95,114,103,
98,95,119,101,105,103,
104,116,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,120,
95,114,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,120,
95,103,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,120,
95,98,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,121,
95,114,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,121,
95,103,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,121,
95,98,0,103,108,111,
98,97,108,95,109,97,
115,107,95,116,121,112,
101,0,103,108,111,98,
97,108,95,109,97,115,
107,95,115,97,109,112,
108,101,95,109,111,100,
101,95,100,101,115,105,
114,101,100,0,103,108,
111,98,97,108,95,109,
97,115,107,95,110,117,
109,95,116,114,105,97,
100,115,95,100,101,115,
105,114,101,100,0,103,
108,111,98,97,108,95,
109,97,115,107,95,116,
114,105,97,100,95,115,
105,122,101,95,100,101,
115,105,114,101,100,0,
103,108,111,98,97,108,
95,109,97,115,107,95,
115,112,101,99,105,102,
121,95,110,117,109,95,
116,114,105,97,100,115,
0,103,108,111,98,97,
108,95,97,97,95,115,
117,98,112,105,120,101,
108,95,114,95,111,102,
102,115,101,116,95,120,
95,114,117,110,116,105,
109,101,0,103,108,111,
98,97,108,95,97,97,
95,115,117,98,112,105,
120,101,108,95,114,95,
111,102,102,115,101,116,
95,121,95,114,117,110,
116,105,109,101,0,103,
108,111,98,97,108,95,
97,97,95,99,117,98,
105,99,95,99,0,103,
108,111,98,97,108,95,
97,97,95,103,97,117,
115,115,95,115,105,103,
109,97,0,103,108,111,
98,97,108,95,103,101,
111,109,95,109,111,100,
101,95,114,117,110,116,
105,109,101,0,103,108,
111,98,97,108,95,103,
101,111,109,95,114,97,
100,105,117,115,0,103,
108,111,98,97,108,95,
103,101,111,109,95,118,
105,101,119,95,100,105,
115,116,0,103,108,111,
98,97,108,95,103,101,
111,109,95,116,105,108,
116,95,97,110,103,108,
101,95,120,0,103,108,
111,98,97,108,95,103,
101,111,109,95,116,105,
108,116,95,97,110,103,
108,101,95,121,0,103,
108,111,98,97,108,95,
103,101,111,109,95,97,
115,112,101,99,116,95,
114,97,116,105,111,95,
120,0,103,108,111,98,
97,108,95,103,101,111,
109,95,97,115,112,101,
99,116,95,114,97,116,
105,111,95,121,0,103,
108,111,98,97,108,95,
103,101,111,109,95,111,
118,101,114,115,99,97,
110,95,120,0,103,108,
111,98,97,108,95,103,
101,111,109,95,111,118,
101,114,115,99,97,110,
95,121,0,103,108,111,
98,97,108,95,98,111,
114,100,101,114,95,115,
105,122,101,0,103,108,
111,98,97,108,95,98,
111,114,100,101,114,95,
100,97,114,107,110,101,
115,115,0,103,108,111,
98,97,108,95,98,111,
114,100,101,114,95,99,
111,109,112,114,101,115,
115,0,103,108,111,98,
97,108,95,105,110,116,
101,114,108,97,99,101,
95,98,102,102,0,103,
108,111,98,97,108,95,
105,110,116,101,114,108,
97,99,101,95,49,48,
56,48,105,0,103,108,
111,98,97,108,95,105,
110,116,101,114,108,97,
99,101,95,100,101,116,
101,99,116,95,116,111,
103,103,108,101,0,171,
248,13,0,0,0,0,
0,0,16,0,0,0,
2,0,0,0,20,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,56,14,
0,0,16,0,0,0,
16,0,0,0,0,0,
0,0,20,14,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,76,14,0,0,
32,0,0,0,16,0,
0,0,2,0,0,0,
20,14,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
94,14,0,0,48,0,
0,0,4,0,0,0,
0,0,0,0,120,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,156,14,
0,0,64,0,0,0,
16,0,0,0,2,0,
0,0,20,14,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,184,14,0,0,
80,0,0,0,16,0,
0,0,2,0,0,0,
20,14,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
112,97,114,97,109,115,
95,83,111,117,114,99,
101,83,105,122,101,0,
102,108,111,97,116,52,
0,171,171,171,1,0,
3,0,1,0,4,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
10,14,0,0,112,97,
114,97,109,115,95,79,
114,105,103,105,110,97,
108,83,105,122,101,0,
112,97,114,97,109,115,
95,79,117,116,112,117,
116,83,105,122,101,0,
112,97,114,97,109,115,
95,70,114,97,109,101,
67,111,117,110,116,0,
100,119,111,114,100,0,
171,171,0,0,19,0,
1,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,112,14,
0,0,112,97,114,97,
109,115,95,77,65,83,
75,69,68,95,83,67,
65,78,76,73,78,69,
83,83,105,122,101,0,
112,97,114,97,109,115,
95,66,76,79,79,77,
95,65,80,80,82,79,
88,83,105,122,101,0,
77,105,99,114,111,115,
111,102,116,32,40,82,
41,32,72,76,83,76,
32,83,104,97,100,101,
114,32,67,111,109,112,
105,108,101,114,32,49,
48,46,49,0,73,83,
71,78,68,0,0,0,
2,0,0,0,8,0,
0,0,56,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,15,15,
0,0,56,0,0,0,
1,0,0,0,0,0,
0,0,3,0,0,0,
1,0,0,0,3,3,
0,0,84,69,88,67,
79,79,82,68,0,171,
171,171,79,83,71,78,
128,0,0,0,4,0,
0,0,8,0,0,0,
104,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,12,0,0,
104,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,3,0,0,
104,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,1,14,0,0,
113,0,0,0,0,0,
0,0,1,0,0,0,
3,0,0,0,2,0,
0,0,15,0,0,0,
84,69,88,67,79,79,
82,68,0,83,86,95,
80,111,115,105,116,105,
111,110,0,171,171,171,
83,72,69,88,200,3,
0,0,80,0,1,0,
242,0,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,11,0,0,0,
89,0,0,4,70,142,
32,0,1,0,0,0,
6,0,0,0,95,0,
0,3,242,16,16,0,
0,0,0,0,95,0,
0,3,50,16,16,0,
1,0,0,0,101,0,
0,3,50,32,16,0,
0,0,0,0,101,0,
0,3,194,32,16,0,
0,0,0,0,101,0,
0,3,18,32,16,0,
1,0,0,0,103,0,
0,4,242,32,16,0,
2,0,0,0,1,0,
0,0,104,0,0,2,
1,0,0,0,56,0,
0,8,50,0,16,0,
0,0,0,0,70,16,
16,0,1,0,0,0,
70,128,32,0,1,0,
0,0,0,0,0,0,
14,0,0,8,50,0,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,70,128,32,0,
1,0,0,0,0,0,
0,0,56,0,0,8,
194,0,16,0,0,0,
0,0,6,4,16,0,
0,0,0,0,6,132,
32,0,1,0,0,0,
4,0,0,0,56,0,
0,8,50,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
70,128,32,0,1,0,
0,0,5,0,0,0,
14,0,0,8,194,32,
16,0,0,0,0,0,
6,4,16,0,0,0,
0,0,6,132,32,0,
1,0,0,0,5,0,
0,0,14,0,0,8,
50,32,16,0,0,0,
0,0,230,10,16,0,
0,0,0,0,70,128,
32,0,1,0,0,0,
4,0,0,0,14,0,
0,9,18,0,16,0,
0,0,0,0,10,128,
32,0,1,0,0,0,
2,0,0,0,10,128,
32,0,0,0,0,0,
10,0,0,0,0,0,
0,9,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
26,128,32,128,65,0,
0,0,0,0,0,0,
10,0,0,0,50,0,
0,11,18,0,16,0,
0,0,0,0,42,128,
32,0,0,0,0,0,
10,0,0,0,10,0,
16,0,0,0,0,0,
26,128,32,0,0,0,
0,0,10,0,0,0,
56,0,0,7,18,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,1,64,0,0,
0,0,0,65,51,0,
0,7,34,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,66,52,0,0,7,
34,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,1,64,
0,0,0,0,128,65,
56,0,0,11,194,0,
16,0,0,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,61,0,0,
0,61,6,132,32,0,
1,0,0,0,2,0,
0,0,51,0,0,7,
98,0,16,0,0,0,
0,0,166,11,16,0,
0,0,0,0,86,5,
16,0,0,0,0,0,
51,0,0,7,34,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,0,0,
0,7,34,0,16,0,
0,0,0,0,26,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,55,65,0,0,5,
34,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,29,0,
0,8,66,0,16,0,
0,0,0,0,1,64,
0,0,0,0,0,63,
58,128,32,0,0,0,
0,0,9,0,0,0,
55,0,0,9,18,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
50,0,0,9,34,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,1,64,0,0,
40,126,156,61,1,64,
0,0,104,174,83,189,
50,0,0,10,18,32,
16,0,1,0,0,0,
10,0,16,128,65,0,
0,0,0,0,0,0,
1,64,0,0,224,17,
23,60,26,0,16,0,
0,0,0,0,56,0,
0,8,242,0,16,0,
0,0,0,0,86,21,
16,0,0,0,0,0,
70,142,32,0,0,0,
0,0,1,0,0,0,
50,0,0,10,242,0,
16,0,0,0,0,0,
6,16,16,0,0,0,
0,0,70,142,32,0,
0,0,0,0,0,0,
0,0,70,14,16,0,
0,0,0,0,50,0,
0,10,242,0,16,0,
0,0,0,0,166,26,
16,0,0,0,0,0,
70,142,32,0,0,0,
0,0,2,0,0,0,
70,14,16,0,0,0,
0,0,50,0,0,10,
242,32,16,0,2,0,
0,0,246,31,16,0,
0,0,0,0,70,142,
32,0,0,0,0,0,
3,0,0,0,70,14,
16,0,0,0,0,0,
62,0,0,1,83,84,
65,84,148,0,0,0,
26,0,0,0,1,0,
0,0,0,0,0,0,
6,0,0,0,24,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0
};


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,188,132,
205,245,210,241,53,216,
122,20,216,83,140,27,
251,158,1,0,0,0,
104,19,0,0,5,0,
0,0,52,0,0,0,
252,13,0,0,96,14,
0,0,148,14,0,0,
204,18,0,0,82,68,
69,70,192,13,0,0,
1,0,0,0,48,1,
0,0,5,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
151,13,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
220,0,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
1,0,0,0,1,0,
0,0,246,0,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,1,0,0,0,
1,0,0,0,12,1,
0,0,2,0,0,0,
5,0,0,0,4,0,
0,0,255,255,255,255,
2,0,0,0,1,0,
0,0,13,0,0,0,
29,1,0,0,2,0,
0,0,5,0,0,0,
4,0,0,0,255,255,
255,255,3,0,0,0,
1,0,0,0,13,0,
0,0,42,1,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
1,0,0,0,95,77,
65,83,75,69,68,95,
83,67,65,78,76,73,
78,69,83,95,115,97,
109,112,108,101,114,0,
95,66,76,79,79,77,
95,65,80,80,82,79,
88,95,115,97,109,112,
108,101,114,0,77,65,
83,75,69,68,95,83,
67,65,78,76,73,78,
69,83,0,66,76,79,
79,77,95,65,80,80,
82,79,88,0,85,66,
79,0,171,171,42,1,
0,0,47,0,0,0,
72,1,0,0,0,1,
0,0,0,0,0,0,
0,0,0,0,160,8,
0,0,0,0,0,0,
64,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,216,8,0,0,
64,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
20,9,0,0,68,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,37,9,
0,0,72,0,0,0,
4,0,0,0,2,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,60,9,0,0,
76,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
83,9,0,0,80,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,107,9,
0,0,84,0,0,0,
4,0,0,0,2,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,141,9,0,0,
88,0,0,0,4,0,
0,0,2,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
161,9,0,0,92,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,183,9,
0,0,96,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,205,9,0,0,
100,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
228,9,0,0,104,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,250,9,
0,0,108,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,16,10,0,0,
112,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
40,10,0,0,116,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,65,10,
0,0,120,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,89,10,0,0,
124,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
125,10,0,0,128,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,155,10,
0,0,132,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,185,10,0,0,
136,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
215,10,0,0,140,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,245,10,
0,0,144,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,19,11,0,0,
148,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
49,11,0,0,152,0,
0,0,4,0,0,0,
2,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,66,11,
0,0,156,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,98,11,0,0,
160,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
129,11,0,0,164,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,160,11,
0,0,168,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,191,11,0,0,
172,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
229,11,0,0,176,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,11,12,
0,0,180,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,29,12,0,0,
184,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
51,12,0,0,188,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,76,12,
0,0,192,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,95,12,0,0,
196,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
117,12,0,0,200,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,142,12,
0,0,204,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,167,12,0,0,
208,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
194,12,0,0,212,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,221,12,
0,0,216,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,244,12,0,0,
220,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
11,13,0,0,224,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,30,13,
0,0,228,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,53,13,0,0,
232,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
76,13,0,0,236,0,
0,0,4,0,0,0,
0,0,0,0,240,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,97,13,
0,0,240,0,0,0,
4,0,0,0,0,0,
0,0,240,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,120,13,0,0,
244,0,0,0,4,0,
0,0,0,0,0,0,
240,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
103,108,111,98,97,108,
95,77,86,80,0,102,
108,111,97,116,52,120,
52,0,2,0,3,0,
4,0,4,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,171,8,
0,0,103,108,111,98,
97,108,95,99,114,116,
95,103,97,109,109,97,
0,102,108,111,97,116,
0,171,0,0,3,0,
1,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,233,8,
0,0,103,108,111,98,
97,108,95,108,99,100,
95,103,97,109,109,97,
0,103,108,111,98,97,
108,95,108,101,118,101,
108,115,95,99,111,110,
116,114,97,115,116,0,
103,108,111,98,97,108,
95,104,97,108,97,116,
105,111,110,95,119,101,
105,103,104,116,0,103,
108,111,98,97,108,95,
100,105,102,102,117,115,
105,111,110,95,119,101,
105,103,104,116,0,103,
108,111,98,97,108,95,
98,108,111,111,109,95,
117,110,100,101,114,101,
115,116,105,109,97,116,
101,95,108,101,118,101,
108,115,0,103,108,111,
98,97,108,95,98,108,
111,111,109,95,101,120,
99,101,115,115,0,103,
108,111,98,97,108,95,
98,101,97,109,95,109,
105,110,95,115,105,103,
109,97,0,103,108,111,
98,97,108,95,98,101,
97,109,95,109,97,120,
95,115,105,103,109,97,
0,103,108,111,98,97,
108,95,98,101,97,109,
95,115,112,111,116,95,
112,111,119,101,114,0,
103,108,111,98,97,108,
95,98,101,97,109,95,
109,105,110,95,115,104,
97,112,101,0,103,108,
111,98,97,108,95,98,
101,97,109,95,109,97,
120,95,115,104,97,112,
101,0,103,108,111,98,
97,108,95,98,101,97,
109,95,115,104,97,112,
101,95,112,111,119,101,
114,0,103,108,111,98,
97,108,95,98,101,97,
109,95,104,111,114,105,
122,95,102,105,108,116,
101,114,0,103,108,111,
98,97,108,95,98,101,
97,109,95,104,111,114,
105,122,95,115,105,103,
109,97,0,103,108,111,
98,97,108,95,98,101,
97,109,95,104,111,114,
105,122,95,108,105,110,
101,97,114,95,114,103,
98,95,119,101,105,103,
104,116,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,120,
95,114,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,120,
95,103,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,120,
95,98,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,121,
95,114,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,121,
95,103,0,103,108,111,
98,97,108,95,99,111,
110,118,101,114,103,101,
110,99,101,95,111,102,
102,115,101,116,95,121,
95,98,0,103,108,111,
98,97,108,95,109,97,
115,107,95,116,121,112,
101,0,103,108,111,98,
97,108,95,109,97,115,
107,95,115,97,109,112,
108,101,95,109,111,100,
101,95,100,101,115,105,
114,101,100,0,103,108,
111,98,97,108,95,109,
97,115,107,95,110,117,
109,95,116,114,105,97,
100,115,95,100,101,115,
105,114,101,100,0,103,
108,111,98,97,108,95,
109,97,115,107,95,116,
114,105,97,100,95,115,
105,122,101,95,100,101,
115,105,114,101,100,0,
103,108,111,98,97,108,
95,109,97,115,107,95,
115,112,101,99,105,102,
121,95,110,117,109,95,
116,114,105,97,100,115,
0,103,108,111,98,97,
108,95,97,97,95,115,
117,98,112,105,120,101,
108,95,114,95,111,102,
102,115,101,116,95,120,
95,114,117,110,116,105,
109,101,0,103,108,111,
98,97,108,95,97,97,
95,115,117,98,112,105,
120,101,108,95,114,95,
111,102,102,115,101,116,
95,121,95,114,117,110,
116,105,109,101,0,103,
108,111,98,97,108,95,
97,97,95,99,117,98,
105,99,95,99,0,103,
108,111,98,97,108,95,
97,97,95,103,97,117,
115,115,95,115,105,103,
109,97,0,103,108,111,
98,97,108,95,103,101,
111,109,95,109,111,100,
101,95,114,117,110,116,
105,109,101,0,103,108,
111,98,97,108,95,103,
101,111,109,95,114,97,
100,105,117,115,0,103,
108,111,98,97,108,95,
103,101,111,109,95,118,
105,101,119,95,100,105,
115,116,0,103,108,111,
98,97,108,95,103,101,
111,109,95,116,105,108,
116,95,97,110,103,108,
101,95,120,0,103,108,
111,98,97,108,95,103,
101,111,109,95,116,105,
108,116,95,97,110,103,
108,101,95,121,0,103,
108,111,98,97,108,95,
103,101,111,109,95,97,
115,112,101,99,116,95,
114,97,116,105,111,95,
120,0,103,108,111,98,
97,108,95,103,101,111,
109,95,97,115,112,101,
99,116,95,114,97,116,
105,111,95,121,0,103,
108,111,98,97,108,95,
103,101,111,109,95,111,
118,101,114,115,99,97,
110,95,120,0,103,108,
111,98,97,108,95,103,
101,111,109,95,111,118,
101,114,115,99,97,110,
95,121,0,103,108,111,
98,97,108,95,98,111,
114,100,101,114,95,115,
105,122,101,0,103,108,
111,98,97,108,95,98,
111,114,100,101,114,95,
100,97,114,107,110,101,
115,115,0,103,108,111,
98,97,108,95,98,111,
114,100,101,114,95,99,
111,109,112,114,101,115,
115,0,103,108,111,98,
97,108,95,105,110,116,
101,114,108,97,99,101,
95,98,102,102,0,103,
108,111,98,97,108,95,
105,110,116,101,114,108,
97,99,101,95,49,48,
56,48,105,0,103,108,
111,98,97,108,95,105,
110,116,101,114,108,97,
99,101,95,100,101,116,
101,99,116,95,116,111,
103,103,108,101,0,77,
105,99,114,111,115,111,
102,116,32,40,82,41,
32,72,76,83,76,32,
83,104,97,100,101,114,
32,67,111,109,112,105,
108,101,114,32,49,48,
46,49,0,171,73,83,
71,78,92,0,0,0,
3,0,0,0,8,0,
0,0,80,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,3,3,
0,0,80,0,0,0,
1,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,12,12,
0,0,80,0,0,0,
2,0,0,0,0,0,
0,0,3,0,0,0,
1,0,0,0,1,1,
0,0,84,69,88,67,
79,79,82,68,0,171,
171,171,79,83,71,78,
44,0,0,0,1,0,
0,0,8,0,0,0,
32,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,15,0,0,0,
83,86,95,84,97,114,
103,101,116,0,171,171,
83,72,69,88,48,4,
0,0,80,0,0,0,
12,1,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,10,0,0,0,
90,0,0,3,0,96,
16,0,2,0,0,0,
90,0,0,3,0,96,
16,0,3,0,0,0,
88,24,0,4,0,112,
16,0,2,0,0,0,
85,85,0,0,88,24,
0,4,0,112,16,0,
3,0,0,0,85,85,
0,0,98,16,0,3,
50,16,16,0,0,0,
0,0,98,16,0,3,
194,16,16,0,0,0,
0,0,98,16,0,3,
18,16,16,0,1,0,
0,0,101,0,0,3,
242,32,16,0,0,0,
0,0,104,0,0,2,
4,0,0,0,0,0,
0,7,18,0,16,0,
0,0,0,0,10,16,
16,0,1,0,0,0,
1,64,0,0,143,63,
176,189,14,0,0,7,
18,0,16,0,0,0,
0,0,1,64,0,0,
185,90,178,62,10,0,
16,0,0,0,0,0,
56,0,0,7,18,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,1,64,0,0,
59,170,184,63,25,0,
0,5,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
56,0,0,7,18,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,1,64,0,0,
59,170,184,63,25,0,
0,5,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
14,0,0,7,34,0,
16,0,0,0,0,0,
1,64,0,0,149,117,
204,62,10,16,16,0,
1,0,0,0,51,0,
0,7,18,0,16,0,
0,0,0,0,26,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,49,0,0,11,
98,0,16,0,0,0,
0,0,166,138,32,0,
0,0,0,0,9,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,63,0,0,192,63,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
1,64,0,0,44,100,
177,64,1,64,0,0,
62,6,199,64,55,0,
0,9,34,0,16,0,
0,0,0,0,26,0,
16,0,0,0,0,0,
1,64,0,0,87,246,
153,64,42,0,16,0,
0,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,1,0,0,0,
70,16,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
56,0,0,7,226,0,
16,0,0,0,0,0,
86,5,16,0,0,0,
0,0,6,9,16,0,
1,0,0,0,56,0,
0,8,226,0,16,0,
0,0,0,0,86,14,
16,0,0,0,0,0,
166,138,32,0,0,0,
0,0,4,0,0,0,
0,0,0,7,226,0,
16,0,0,0,0,0,
86,14,16,0,0,0,
0,0,86,14,16,0,
0,0,0,0,56,0,
0,7,114,0,16,0,
2,0,0,0,6,0,
16,0,0,0,0,0,
150,7,16,0,0,0,
0,0,56,0,0,8,
226,0,16,0,0,0,
0,0,86,14,16,0,
0,0,0,0,86,133,
32,0,0,0,0,0,
5,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,3,0,0,0,
230,26,16,0,0,0,
0,0,70,126,16,0,
3,0,0,0,0,96,
16,0,3,0,0,0,
50,0,0,11,114,0,
16,0,2,0,0,0,
70,2,16,0,3,0,
0,0,166,138,32,0,
0,0,0,0,4,0,
0,0,70,2,16,128,
65,0,0,0,2,0,
0,0,52,0,0,10,
114,0,16,0,2,0,
0,0,70,2,16,0,
2,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
50,0,0,14,114,0,
16,0,2,0,0,0,
70,2,16,128,65,0,
0,0,2,0,0,0,
86,133,32,0,0,0,
0,0,5,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
0,0,14,0,0,7,
226,0,16,0,0,0,
0,0,6,9,16,0,
2,0,0,0,86,14,
16,0,0,0,0,0,
0,0,0,10,242,0,
16,0,0,0,0,0,
70,14,16,0,0,0,
0,0,2,64,0,0,
0,0,128,191,0,0,
128,191,0,0,128,191,
0,0,128,191,14,32,
0,7,114,0,16,0,
0,0,0,0,150,7,
16,0,0,0,0,0,
6,0,16,0,0,0,
0,0,0,0,0,11,
114,0,16,0,2,0,
0,0,70,2,16,128,
65,0,0,0,0,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,0,0,50,0,
0,10,114,0,16,0,
0,0,0,0,166,138,
32,0,0,0,0,0,
5,0,0,0,70,2,
16,0,2,0,0,0,
70,2,16,0,0,0,
0,0,56,0,0,7,
114,32,16,0,0,0,
0,0,70,2,16,0,
0,0,0,0,70,2,
16,0,1,0,0,0,
54,0,0,5,130,32,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,62,0,0,1,
83,84,65,84,148,0,
0,0,29,0,0,0,
4,0,0,0,0,0,
0,0,4,0,0,0,
23,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,2,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,2,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0
};

}

namespace RetroArch
{
class CrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDef : public ShaderDef
{
public:
	CrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDef() : ShaderDef{}
	{
		Name = "crt-royale-brightpass";
		VertexByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDefs::sFragmentByteCode);
		Params.push_back(ShaderParam("crt_gamma", 0, 64, 4, 1.000000f, 5.000000f, 2.500000f, 0.025000f, "Simulated CRT Gamma"));
		Params.push_back(ShaderParam("lcd_gamma", 0, 68, 4, 1.000000f, 5.000000f, 2.200000f, 0.025000f, "Your Display Gamma"));
		Params.push_back(ShaderParam("levels_contrast", 0, 72, 4, 0.000000f, 4.000000f, 1.000000f, 0.015625f, "Contrast"));
		Params.push_back(ShaderParam("halation_weight", 0, 76, 4, 0.000000f, 1.000000f, 0.000000f, 0.005000f, "Halation Weight"));
		Params.push_back(ShaderParam("diffusion_weight", 0, 80, 4, 0.000000f, 1.000000f, 0.075000f, 0.005000f, "Diffusion Weight"));
		Params.push_back(ShaderParam("bloom_underestimate_levels", 0, 84, 4, 0.000000f, 5.000000f, 0.800000f, 0.010000f, "Bloom - Underestimate Levels"));
		Params.push_back(ShaderParam("bloom_excess", 0, 88, 4, 0.000000f, 1.000000f, 0.000000f, 0.005000f, "Bloom - Excess"));
		Params.push_back(ShaderParam("beam_min_sigma", 0, 92, 4, 0.005000f, 1.000000f, 0.020000f, 0.005000f, "Beam - Min Sigma"));
		Params.push_back(ShaderParam("beam_max_sigma", 0, 96, 4, 0.005000f, 1.000000f, 0.300000f, 0.005000f, "Beam - Max Sigma"));
		Params.push_back(ShaderParam("beam_spot_power", 0, 100, 4, 0.010000f, 16.000000f, 0.330000f, 0.010000f, "Beam - Spot Power"));
		Params.push_back(ShaderParam("beam_min_shape", 0, 104, 4, 2.000000f, 32.000000f, 2.000000f, 0.100000f, "Beam - Min Shape"));
		Params.push_back(ShaderParam("beam_max_shape", 0, 108, 4, 2.000000f, 32.000000f, 4.000000f, 0.100000f, "Beam - Max Shape"));
		Params.push_back(ShaderParam("beam_shape_power", 0, 112, 4, 0.010000f, 16.000000f, 0.250000f, 0.010000f, "Beam - Shape Power"));
		Params.push_back(ShaderParam("beam_horiz_filter", 0, 116, 4, 0.000000f, 2.000000f, 0.000000f, 1.000000f, "Beam - Horiz Filter"));
		Params.push_back(ShaderParam("beam_horiz_sigma", 0, 120, 4, 0.000000f, 0.670000f, 0.350000f, 0.005000f, "Beam - Horiz Sigma"));
		Params.push_back(ShaderParam("beam_horiz_linear_rgb_weight", 0, 124, 4, 0.000000f, 1.000000f, 1.000000f, 0.010000f, "Beam - Horiz Linear RGB Weight"));
		Params.push_back(ShaderParam("convergence_offset_x_r", 0, 128, 4, -4.000000f, 4.000000f, 0.000000f, 0.050000f, "Convergence - Offset X Red"));
		Params.push_back(ShaderParam("convergence_offset_x_g", 0, 132, 4, -4.000000f, 4.000000f, 0.000000f, 0.050000f, "Convergence - Offset X Green"));
		Params.push_back(ShaderParam("convergence_offset_x_b", 0, 136, 4, -4.000000f, 4.000000f, 0.000000f, 0.050000f, "Convergence - Offset X Blue"));
		Params.push_back(ShaderParam("convergence_offset_y_r", 0, 140, 4, -2.000000f, 2.000000f, 0.000000f, 0.050000f, "Convergence - Offset Y Red"));
		Params.push_back(ShaderParam("convergence_offset_y_g", 0, 144, 4, -2.000000f, 2.000000f, 0.000000f, 0.050000f, "Convergence - Offset Y Green"));
		Params.push_back(ShaderParam("convergence_offset_y_b", 0, 148, 4, -2.000000f, 2.000000f, 0.000000f, 0.050000f, "Convergence - Offset Y Blue"));
		Params.push_back(ShaderParam("mask_type", 0, 152, 4, 0.000000f, 2.000000f, 1.000000f, 1.000000f, "Mask - Type"));
		Params.push_back(ShaderParam("mask_sample_mode_desired", 0, 156, 4, 0.000000f, 2.000000f, 0.000000f, 1.000000f, "Mask - Sample Mode"));
		Params.push_back(ShaderParam("mask_specify_num_triads", 0, 168, 4, 0.000000f, 1.000000f, 0.000000f, 1.000000f, "Mask - Specify Number of Triads"));
		Params.push_back(ShaderParam("mask_triad_size_desired", 0, 164, 4, 1.000000f, 18.000000f, 3.000000f, 0.125000f, "Mask - Triad Size Desired"));
		Params.push_back(ShaderParam("mask_num_triads_desired", 0, 160, 4, 342.000000f, 1920.000000f, 480.000000f, 1.000000f, "Mask - Number of Triads Desired"));
		Params.push_back(ShaderParam("aa_subpixel_r_offset_x_runtime", 0, 172, 4, -0.333333f, 0.333333f, -0.333333f, 0.333333f, "AA - Subpixel R Offset X"));
		Params.push_back(ShaderParam("aa_subpixel_r_offset_y_runtime", 0, 176, 4, -0.333333f, 0.333333f, 0.000000f, 0.333333f, "AA - Subpixel R Offset Y"));
		Params.push_back(ShaderParam("aa_cubic_c", 0, 180, 4, 0.000000f, 4.000000f, 0.500000f, 0.015625f, "AA - Cubic Sharpness"));
		Params.push_back(ShaderParam("aa_gauss_sigma", 0, 184, 4, 0.062500f, 1.000000f, 0.500000f, 0.015625f, "AA - Gaussian Sigma"));
		Params.push_back(ShaderParam("geom_mode_runtime", 0, 188, 4, 0.000000f, 3.000000f, 0.000000f, 1.000000f, "Geometry - Mode"));
		Params.push_back(ShaderParam("geom_radius", 0, 192, 4, 0.160000f, 1024.000000f, 2.000000f, 0.100000f, "Geometry - Radius"));
		Params.push_back(ShaderParam("geom_view_dist", 0, 196, 4, 0.500000f, 1024.000000f, 2.000000f, 0.250000f, "Geometry - View Distance"));
		Params.push_back(ShaderParam("geom_tilt_angle_x", 0, 200, 4, -3.141593f, 3.141593f, 0.000000f, 0.017453f, "Geometry - Tilt Angle X"));
		Params.push_back(ShaderParam("geom_tilt_angle_y", 0, 204, 4, -3.141593f, 3.141593f, 0.000000f, 0.017453f, "Geometry - Tilt Angle Y"));
		Params.push_back(ShaderParam("geom_aspect_ratio_x", 0, 208, 4, 1.000000f, 512.000000f, 432.000000f, 1.000000f, "Geometry - Aspect Ratio X"));
		Params.push_back(ShaderParam("geom_aspect_ratio_y", 0, 212, 4, 1.000000f, 512.000000f, 329.000000f, 1.000000f, "Geometry - Aspect Ratio Y"));
		Params.push_back(ShaderParam("geom_overscan_x", 0, 216, 4, 0.003906f, 4.000000f, 1.000000f, 0.003906f, "Geometry - Overscan X"));
		Params.push_back(ShaderParam("geom_overscan_y", 0, 220, 4, 0.003906f, 4.000000f, 1.000000f, 0.003906f, "Geometry - Overscan Y"));
		Params.push_back(ShaderParam("border_size", 0, 224, 4, 0.000000f, 0.500000f, 0.015000f, 0.005000f, "Border - Size"));
		Params.push_back(ShaderParam("border_darkness", 0, 228, 4, 0.000000f, 16.000000f, 2.000000f, 0.062500f, "Border - Darkness"));
		Params.push_back(ShaderParam("border_compress", 0, 232, 4, 1.000000f, 64.000000f, 2.500000f, 0.062500f, "Border - Compression"));
		Params.push_back(ShaderParam("interlace_detect_toggle", 0, 244, 4, 0.000000f, 1.000000f, 1.000000f, 1.000000f, "Interlacing - Toggle"));
		Params.push_back(ShaderParam("interlace_bff", 0, 236, 4, 0.000000f, 1.000000f, 0.000000f, 1.000000f, "Interlacing - Bottom Field First"));
		Params.push_back(ShaderParam("interlace_1080i", 0, 240, 4, 0.000000f, 1.000000f, 0.000000f, 1.000000f, "Interlace - Detect 1080i"));
		Params.push_back(ShaderParam("MVP", 0, 0, 64, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("SourceSize", -1, 0, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OriginalSize", -1, 16, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OutputSize", -1, 32, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("FrameCount", -1, 48, 4, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("MASKED_SCANLINESSize", -1, 64, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("BLOOM_APPROXSize", -1, 80, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Samplers.push_back(ShaderSampler("MASKED_SCANLINES", 2));
		Samplers.push_back(ShaderSampler("BLOOM_APPROX", 3));
/*
VertexSource = %*VERTEX_SOURCE*%;
*/
/*
FragmentSource = %*FRAGMENT_SOURCE*%;
*/
	}
};
}
