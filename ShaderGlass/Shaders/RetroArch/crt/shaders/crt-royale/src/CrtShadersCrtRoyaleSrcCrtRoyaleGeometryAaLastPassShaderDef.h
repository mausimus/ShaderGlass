/*
ShaderGlass shader crt-shaders-crt-royale-src\crt-royale-geometry-aa-last-pass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/shaders/crt-royale/src/crt-royale-geometry-aa-last-pass.slang
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
//  Create a local-to-global rotation matrix for the CRT's coordinate frame
//  and its global-to-local inverse.  See the vertex shader for details.
//  It's faster to compute these statically if possible.
//////////////////////////////////  INCLUDES  //////////////////////////////////
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
/////////////////////////////////  DESCRIPTION  ////////////////////////////////
//  This file provides antialiased and subpixel-aware tex2D lookups.
//  Requires:   All functions share these requirements:
//              1.) All requirements of gamma-management.h must be satisfied!
//              2.) pixel_to_tex_uv must be a 2x2 matrix that transforms pixe-
//                  space offsets to texture uv offsets.  You can get this with:
//                      const float2 duv_dx = ddx(tex_uv);
//                      const float2 duv_dy = ddy(tex_uv);
//                      const float2x2 pixel_to_tex_uv = float2x2(
//                          duv_dx.x, duv_dy.x,
//                          duv_dx.y, duv_dy.y);
//                  This is left to the user in case the current Cg profile
//                  doesn't support ddx()/ddy().  Ideally, the user could find
//                  calculate a distorted tangent-space mapping analytically.
//                  If not, a simple flat mapping can be obtained with:
//                      const float2 xy_to_uv_scale = IN.output_size *
//                          IN.video_size/IN.texture_size;
//                      const float2x2 pixel_to_tex_uv = float2x2(
//                          xy_to_uv_scale.x, 0.0,
//                          0.0, xy_to_uv_scale.y);
//  Optional:   To set basic AA settings, #define ANTIALIAS_OVERRIDE_BASICS and:
//              1.) Set an antialiasing level:
//                      static const float aa_level = {0 (none),
//                          1 (sample subpixels), 4, 5, 6, 7, 8, 12, 16, 20, 24}
//              2.) Set a filter type:
//                      static const float aa_filter = {
//                          0 (Box, Separable), 1 (Box, Cylindrical),
//                          2 (Tent, Separable), 3 (Tent, Cylindrical)
//                          4 (Gaussian, Separable), 5 (Gaussian, Cylindrical)
//                          6 (Cubic, Separable), 7 (Cubic, Cylindrical)
//                          8 (Lanczos Sinc, Separable),
//                          9 (Lanczos Jinc, Cylindrical)}
//                  If the input is unknown, a separable box filter is used.
//                  Note: Lanczos Jinc is terrible for sparse sampling, and
//                  using aa_axis_importance (see below) defeats the purpose.
//              3.) Mirror the sample pattern on odd frames?
//                      static const bool aa_temporal = {true, false]
//                  This helps rotational invariance but can look "fluttery.
//              The user may #define ANTIALIAS_OVERRIDE_PARAMETERS to override
//              (all of) the following default parameters with static or uniform
//              constants (or an accessor function for subpixel offsets):
//              1.) Cubic parameters:
//                      static const float aa_cubic_c = 0.5;
//                  See http://www.imagemagick.org/Usage/filter/#mitchell
//              2.) Gaussian parameters:
//                      static const float aa_gauss_sigma =
//                          0.5/aa_pixel_diameter;
//              3.) Set subpixel offsets.  This requires an accessor function
//                  for compatibility with scalar runtime shader params.  Return
//                  a float2 pixel offset in [-0.5, 0.5] for the red subpixel:
//                      float2 get_aa_subpixel_r_offset()
//              The user may also #define ANTIALIAS_OVERRIDE_STATIC_CONSTANTS to
//              override (all of) the following default static values.  However,
//              the file's structure requires them to be declared static const:
//              1.) static const float aa_lanczos_lobes = 3.0;
//              2.) static const float aa_gauss_support = 1.0/aa_pixel_diameter;
//                  Note the default tent/Gaussian support radii may appear
//                  arbitrary, but extensive testing found them nearly optimal
//                  for tough cases like strong distortion at low AA levels.
//                  (The Gaussian default is only best for practical gauss_sigma
//                  values; much larger gauss_sigmas ironically prefer slightly
//                  smaller support given sparse sampling, and vice versa.)
//              3.) static const float aa_tent_support = 1.0 / aa_pixel_diameter;
//              4.) static const float2 aa_xy_axis_importance:
//                  The sparse N-queens sampling grid interacts poorly with
//                  negative-lobed 2D filters.  However, if aliasing is much
//                  stronger in one direction (e.g. horizontally with a phosphor
//                  mask), it can be useful to downplay sample offsets along the
//                  other axis.  The support radius in each direction scales with
//                  aa_xy_axis_importance down to a minimum of 0.5 (box support),
//                  after which point only the offsets used for calculating
//                  weights continue to scale downward.  This works as follows:
//                  If aa_xy_axis_importance = float2(1.0, 1.0/support_radius),
//                  the vertical support radius will drop to 1.0, and we'll just
//                  filter vertical offsets with the first filter lobe, while
//                  horizontal offsets go through the full multi-lobe filter.
//                  If aa_xy_axis_importance = float2(1.0, 0.0), the vertical
//                  support radius will drop to box support, and the vertical
//                  offsets will be ignored entirely (essentially giving us a
//                  box filter vertically).  The former is potentially smoother
//                  (but less predictable) and the default behavior of Lanczos
//                  jinc, whereas the latter is sharper and the default behavior
//                  of cubics and Lanczos sinc.
//              5.) static const float aa_pixel_diameter: You can expand the
//                  pixel diameter to e.g. sqrt(2.0), which may be a better
//                  support range for cylindrical filters (they don't
//                  currently discard out-of-circle samples though).
//              Finally, there are two miscellaneous options:
//              1.) If you want to antialias a manually tiled texture, you can
//                  #define ANTIALIAS_DISABLE_ANISOTROPIC to use tex2Dlod() to
//                  fix incompatibilities with anisotropic filtering.  This is
//                  slower, and the Cg profile must support tex2Dlod().
//              2.) If aa_cubic_c is a runtime uniform, you can #define
//                  RUNTIME_ANTIALIAS_WEIGHTS to evaluate cubic weights once per
//                  fragment instead of at the usage site (which is used by
//                  default, because it enables static evaluation).
//  Description:
//  Each antialiased lookup follows these steps:
//  1.) Define a sample pattern of pixel offsets in the range of [-0.5, 0.5]
//      pixels, spanning the diameter of a rectangular box filter.
//  2.) Scale these offsets by the support diameter of the user's chosen filter.
//  3.) Using these pixel offsets from the pixel center, compute the offsets to
//      predefined subpixel locations.
//  4.) Compute filter weights based on subpixel offsets.
//  Much of that can often be done at compile-time.  At runtime:
//  1.) Project pixel-space offsets into uv-space with a matrix multiplication
//      to get the uv offsets for each sample.  Rectangular pixels have a
//      diameter of 1.0.  Circular pixels are not currently supported, but they
//      might be better with a diameter of sqrt(2.0) to ensure there are no gaps
//      between them.
//  2.) Load, weight, and sum samples.
//  We use a sparse bilinear sampling grid, so there are two major implications:
//  1.) We can directly project the pixel-space support box into uv-space even
//      if we're upsizing.  This wouldn't be the case for nearest neighbor,
//      where we'd have to expand the uv-space diameter to at least the support
//      size to ensure sufficient filter support.  In our case, this allows us
//      to treat upsizing the same as downsizing and use static weighting. :)
//  2.) For decent results, negative-lobed filters must be computed based on
//      separable weights, not radial distances, because the sparse sampling
//      makes no guarantees about radial distributions.  Even then, it's much
//      better to set aa_xy_axis_importance to e.g. float2(1.0, 0.0) to use e.g.
//      Lanczos2 horizontally and a box filter vertically.  This is mainly due
//      to the sparse N-queens sampling and a statistically enormous positive or
//      negative covariance between horizontal and vertical weights.
//
//  Design Decision Comments:
//  "aa_temporal" mirrors the sample pattern on odd frames along the axis that
//  keeps subpixel weights constant.  This helps with rotational invariance, but
//  it can cause distracting fluctuations, and horizontal and vertical edges
//  will look the same.  Using a different pattern on a shifted grid would
//  exploit temporal AA better, but it would require a dynamic branch or a lot
//  of conditional moves, so it's prohibitively slow for the minor benefit.
/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////
//  The following settings must be static constants:
//  Users may override these parameters, but the file structure requires
//  them to be static constants; see the descriptions above.
//  If we're using a negative-lobed filter, default to using it horizontally
//  only, and use only the first lobe vertically or a box filter, over a
//  correspondingly smaller range.  This compensates for the sparse sampling
//  grid's typically large positive/negative x/y covariance.
//  Users may override these values with their own uniform or static consts.
//  Cubics: See http://www.imagemagick.org/Usage/filter/#mitchell
//  1.) "Keys cubics" with B = 1 - 2C are considered the highest quality.
//  2.) C = 0.5 (default) is Catmull-Rom; higher C's apply sharpening.
//  3.) C = 1.0/3.0 is the Mitchell-Netravali filter.
//  4.) C = 0.0 is a soft spline filter.
//  Users may override the subpixel offset accessor function with their own.
//  A function is used for compatibility with scalar runtime shader params.
//////////////////////////////////  INCLUDES  //////////////////////////////////
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
//////////////////////////////////  CONSTANTS  /////////////////////////////////
////////////////////////////  GLOBAL NON-CONSTANTS  ////////////////////////////
//  We'll want to define these only once per fragment at most.
///////////////////////////////////  HELPERS  //////////////////////////////////
//  Compute cubic coefficients on demand at runtime, and save them to global
//  uniforms.  The B parameter is computed from C, because "Keys cubics
//  with B = 1 - 2C are considered the highest quality.
//  Statically select the base support radius:
//  Expand the filter support for subpixel filtering.
//  Ignore aa_xy_axis_importance for box filtering.
//  Scale the support window by aa_xy_axis_importance, but don't narrow
//  it further than box support.  This allows decent vertical AA without
//  messing up horizontal weights or using something silly like Lanczos4
//  horizontally with a huge vertical average over an 8-pixel radius.
//  Adjust aa_xy_axis_importance to compensate for what's already done:
///////////////////////////  FILTER WEIGHT FUNCTIONS  //////////////////////////
//  Compute coefficients like assign_aa_cubic_constants(), but statically.
//  When runtime weights are used, these values are instead written to
//  global uniforms at the beginning of each tex2Daa* call.
//  Compute the cubic based on the Horner's method formula in:
//  http://www.cs.utexas.edu/users/fussell/courses/cs384g/lectures/mitchell/Mitchell.pdf
//  This is faster than using a specific float2 version:
//  It's faster to let the caller handle the zero case, or at least it
//  was when I used macros and the shader preset took a full minute to load.
//  Note: For sparse sampling, you really need to pick an axis to use
//  Lanczos along (e.g. set aa_xy_axis_importance = float2(1.0, 0.0)).
//  This is a Jinc approximation for x in [0, 45).  We'll use x in range
//  [0, 4*pi) or so.  There are faster/closer approximations based on
//  piecewise cubics from [0, 45) and asymptotic approximations beyond that,
//  but this has a maximum absolute error < 1/512, and it's simpler/faster
//  for shaders...not that it's all that useful for sparse sampling anyway.
//  Requires:   1.) final_axis_impportance must be computed according to
//                  get_subpixel_support_diam_and_final_axis_importance().
//              2.) aa_filter must be a global constant.
//              3.) offset must be an xy pixel offset in the range:
//                      ([-subpixel_support_diameter.x/2,
//                      subpixel_support_diameter.x/2],
//                      [-subpixel_support_diameter.y/2,
//                      subpixel_support_diameter.y/2])
//  Returns:    Sample weights at R/G/B destination subpixels for the
//              given xy pixel offset.
//  Statically select a filter:
//  Default to a box, because Lanczos Jinc is so bad. ;)
//////////////////////////////  HELPER FUNCTIONS  //////////////////////////////
//  If we're manually tiling a texture, anisotropic filtering can get
//  confused.  This is one workaround:
//  TODO: Use tex2Dlod_linearize with a calculated mip level.
//  Mirror the sampling pattern for odd frames in a direction that
//  lets us keep the same subpixel sample weights:
/////////////////////////  ANTIALIASED TEXTURE LOOKUPS  ////////////////////////
//  This function is unlike the others: Just perform a single independent
//  lookup for each subpixel.  It may be very aliased.
//  The tex2Daa* functions compile very slowly due to all the macros and
//  compile-time math, so only include the ones we'll actually use!
//  Use an RGMS4 pattern (4-queens):
//  . . Q .  : off =(-1.5, -1.5)/4 + (2.0, 0.0)/4
//  Q . . .  : off =(-1.5, -1.5)/4 + (0.0, 1.0)/4
//  . . . Q  : off =(-1.5, -1.5)/4 + (3.0, 2.0)/4
//  . Q . .  : off =(-1.5, -1.5)/4 + (1.0, 3.0)/4
//  Static screenspace sample offsets (compute some implicitly):
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 5-queens pattern:
//  . Q . . .  : off =(-2.0, -2.0)/5 + (1.0, 0.0)/5
//  . . . . Q  : off =(-2.0, -2.0)/5 + (4.0, 1.0)/5
//  . . Q . .  : off =(-2.0, -2.0)/5 + (2.0, 2.0)/5
//  Q . . . .  : off =(-2.0, -2.0)/5 + (0.0, 3.0)/5
//  . . . Q .  : off =(-2.0, -2.0)/5 + (3.0, 4.0)/5
//  Static screenspace sample offsets (compute some implicitly):
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 6-queens pattern with a stronger horizontal
//  than vertical slant:
//  . . . . Q .  : off =(-2.5, -2.5)/6 + (4.0, 0.0)/6
//  . . Q . . .  : off =(-2.5, -2.5)/6 + (2.0, 1.0)/6
//  Q . . . . .  : off =(-2.5, -2.5)/6 + (0.0, 2.0)/6
//  . . . . . Q  : off =(-2.5, -2.5)/6 + (5.0, 3.0)/6
//  . . . Q . .  : off =(-2.5, -2.5)/6 + (3.0, 4.0)/6
//  . Q . . . .  : off =(-2.5, -2.5)/6 + (1.0, 5.0)/6
//  Static screenspace sample offsets (compute some implicitly):
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 7-queens pattern with a queen in the center:
//  . Q . . . . .  : off =(-3.0, -3.0)/7 + (1.0, 0.0)/7
//  . . . . Q . .  : off =(-3.0, -3.0)/7 + (4.0, 1.0)/7
//  Q . . . . . .  : off =(-3.0, -3.0)/7 + (0.0, 2.0)/7
//  . . . Q . . .  : off =(-3.0, -3.0)/7 + (3.0, 3.0)/7
//  . . . . . . Q  : off =(-3.0, -3.0)/7 + (6.0, 4.0)/7
//  . . Q . . . .  : off =(-3.0, -3.0)/7 + (2.0, 5.0)/7
//  . . . . . Q .  : off =(-3.0, -3.0)/7 + (5.0, 6.0)/7
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 8-queens pattern.
//  . . Q . . . . .  : off =(-3.5, -3.5)/8 + (2.0, 0.0)/8
//  . . . . Q . . .  : off =(-3.5, -3.5)/8 + (4.0, 1.0)/8
//  . Q . . . . . .  : off =(-3.5, -3.5)/8 + (1.0, 2.0)/8
//  . . . . . . . Q  : off =(-3.5, -3.5)/8 + (7.0, 3.0)/8
//  Q . . . . . . .  : off =(-3.5, -3.5)/8 + (0.0, 4.0)/8
//  . . . . . . Q .  : off =(-3.5, -3.5)/8 + (6.0, 5.0)/8
//  . . . Q . . . .  : off =(-3.5, -3.5)/8 + (3.0, 6.0)/8
//  . . . . . Q . .  : off =(-3.5, -3.5)/8 + (5.0, 7.0)/8
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, and mirror on odd frames if directed:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 12-superqueens pattern where no 3 points are
//  exactly collinear.
//  . . . Q . . . . . . . .  : off =(-5.5, -5.5)/12 + (3.0, 0.0)/12
//  . . . . . . . . . Q . .  : off =(-5.5, -5.5)/12 + (9.0, 1.0)/12
//  . . . . . . Q . . . . .  : off =(-5.5, -5.5)/12 + (6.0, 2.0)/12
//  . Q . . . . . . . . . .  : off =(-5.5, -5.5)/12 + (1.0, 3.0)/12
//  . . . . . . . . . . . Q  : off =(-5.5, -5.5)/12 + (11.0, 4.0)/12
//  . . . . Q . . . . . . .  : off =(-5.5, -5.5)/12 + (4.0, 5.0)/12
//  . . . . . . . Q . . . .  : off =(-5.5, -5.5)/12 + (7.0, 6.0)/12
//  Q . . . . . . . . . . .  : off =(-5.5, -5.5)/12 + (0.0, 7.0)/12
//  . . . . . . . . . . Q .  : off =(-5.5, -5.5)/12 + (10.0, 8.0)/12
//  . . . . . Q . . . . . .  : off =(-5.5, -5.5)/12 + (5.0, 9.0)/12
//  . . Q . . . . . . . . .  : off =(-5.5, -5.5)/12 + (2.0, 10.0)/12
//  . . . . . . . . Q . . .  : off =(-5.5, -5.5)/12 + (8.0, 11.0)/12
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 16-superqueens pattern where no 3 points are
//  exactly collinear.
//  . . Q . . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (2.0, 0.0)/16
//  . . . . . . . . . Q . . . . . .  : off =(-7.5, -7.5)/16 + (9.0, 1.0)/16
//  . . . . . . . . . . . . Q . . .  : off =(-7.5, -7.5)/16 + (12.0, 2.0)/16
//  . . . . Q . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (4.0, 3.0)/16
//  . . . . . . . . Q . . . . . . .  : off =(-7.5, -7.5)/16 + (8.0, 4.0)/16
//  . . . . . . . . . . . . . . Q .  : off =(-7.5, -7.5)/16 + (14.0, 5.0)/16
//  Q . . . . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (0.0, 6.0)/16
//  . . . . . . . . . . Q . . . . .  : off =(-7.5, -7.5)/16 + (10.0, 7.0)/16
//  . . . . . Q . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (5.0, 8.0)/16
//  . . . . . . . . . . . . . . . Q  : off =(-7.5, -7.5)/16 + (15.0, 9.0)/16
//  . Q . . . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (1.0, 10.0)/16
//  . . . . . . . Q . . . . . . . .  : off =(-7.5, -7.5)/16 + (7.0, 11.0)/16
//  . . . . . . . . . . . Q . . . .  : off =(-7.5, -7.5)/16 + (11.0, 12.0)/16
//  . . . Q . . . . . . . . . . . .  : off =(-7.5, -7.5)/16 + (3.0, 13.0)/16
//  . . . . . . Q . . . . . . . . .  : off =(-7.5, -7.5)/16 + (6.0, 14.0)/16
//  . . . . . . . . . . . . . Q . .  : off =(-7.5, -7.5)/16 + (13.0, 15.0)/16
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 20-superqueens pattern where no 3 points are
//  exactly collinear and superqueens have a squared attack radius of 13.
//  . . . . . . . Q . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (7.0, 0.0)/20
//  . . . . . . . . . . . . . . . . Q . . .  : off =(-9.5, -9.5)/20 + (16.0, 1.0)/20
//  . . . . . . . . . . . Q . . . . . . . .  : off =(-9.5, -9.5)/20 + (11.0, 2.0)/20
//  . Q . . . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (1.0, 3.0)/20
//  . . . . . Q . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (5.0, 4.0)/20
//  . . . . . . . . . . . . . . . Q . . . .  : off =(-9.5, -9.5)/20 + (15.0, 5.0)/20
//  . . . . . . . . . . Q . . . . . . . . .  : off =(-9.5, -9.5)/20 + (10.0, 6.0)/20
//  . . . . . . . . . . . . . . . . . . . Q  : off =(-9.5, -9.5)/20 + (19.0, 7.0)/20
//  . . Q . . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (2.0, 8.0)/20
//  . . . . . . Q . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (6.0, 9.0)/20
//  . . . . . . . . . . . . . Q . . . . . .  : off =(-9.5, -9.5)/20 + (13.0, 10.0)/20
//  . . . . . . . . . . . . . . . . . Q . .  : off =(-9.5, -9.5)/20 + (17.0, 11.0)/20
//  Q . . . . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (0.0, 12.0)/20
//  . . . . . . . . . Q . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (9.0, 13.0)/20
//  . . . . Q . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (4.0, 14.0)/20
//  . . . . . . . . . . . . . . Q . . . . .  : off =(-9.5, -9.5)/20 + (14.0, 15.0)/20
//  . . . . . . . . . . . . . . . . . . Q .  : off =(-9.5, -9.5)/20 + (18.0, 16.0)/20
//  . . . . . . . . Q . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (8.0, 17.0)/20
//  . . . Q . . . . . . . . . . . . . . . .  : off =(-9.5, -9.5)/20 + (3.0, 18.0)/20
//  . . . . . . . . . . . . Q . . . . . . .  : off =(-9.5, -9.5)/20 + (12.0, 19.0)/20
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Use a diagonally symmetric 24-superqueens pattern where no 3 points are
//  exactly collinear and superqueens have a squared attack radius of 13.
//  . . . . . . Q . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (6.0, 0.0)/24
//  . . . . . . . . . . . . . . . . Q . . . . . . .  : off =(-11.5, -11.5)/24 + (16.0, 1.0)/24
//  . . . . . . . . . . Q . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (10.0, 2.0)/24
//  . . . . . . . . . . . . . . . . . . . . . Q . .  : off =(-11.5, -11.5)/24 + (21.0, 3.0)/24
//  . . . . . Q . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (5.0, 4.0)/24
//  . . . . . . . . . . . . . . . Q . . . . . . . .  : off =(-11.5, -11.5)/24 + (15.0, 5.0)/24
//  . Q . . . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (1.0, 6.0)/24
//  . . . . . . . . . . . Q . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (11.0, 7.0)/24
//  . . . . . . . . . . . . . . . . . . . Q . . . .  : off =(-11.5, -11.5)/24 + (19.0, 8.0)/24
//  . . . . . . . . . . . . . . . . . . . . . . . Q  : off =(-11.5, -11.5)/24 + (23.0, 9.0)/24
//  . . . Q . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (3.0, 10.0)/24
//  . . . . . . . . . . . . . . Q . . . . . . . . .  : off =(-11.5, -11.5)/24 + (14.0, 11.0)/24
//  . . . . . . . . . Q . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (9.0, 12.0)/24
//  . . . . . . . . . . . . . . . . . . . . Q . . .  : off =(-11.5, -11.5)/24 + (20.0, 13.0)/24
//  Q . . . . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (0.0, 14.0)/24
//  . . . . Q . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (4.0, 15.0)/24
//  . . . . . . . . . . . . Q . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (12.0, 16.0)/24
//  . . . . . . . . . . . . . . . . . . . . . . Q .  : off =(-11.5, -11.5)/24 + (22.0, 17.0)/24
//  . . . . . . . . Q . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (8.0, 18.0)/24
//  . . . . . . . . . . . . . . . . . . Q . . . . .  : off =(-11.5, -11.5)/24 + (18.0, 19.0)/24
//  . . Q . . . . . . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (2.0, 20.0)/24
//  . . . . . . . . . . . . . Q . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (13.0, 21.0)/24
//  . . . . . . . Q . . . . . . . . . . . . . . . .  : off =(-11.5, -11.5)/24 + (7.0, 22.0)/24
//  . . . . . . . . . . . . . . . . . Q . . . . . .  : off =(-11.5, -11.5)/24 + (17.0, 23.0)/24
//  Get the xy offset of each sample.  Exploit diagonal symmetry:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, mirror on odd frames if directed, and exploit
//  diagonal symmetry:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  Sample on a regular 4x4 grid.  This is mainly for testing.
//  Get the xy offset of each sample:
//  Compute subpixel weights, and exploit diagonal symmetry for speed.
//  (We can't exploit vertical or horizontal symmetry due to uncertain
//  subpixel offsets.  We could fix that by rotating xy offsets with the
//  subpixel structure, but...no.)
//  Get the weight sum to normalize the total to 1.0 later:
//  Scale the pixel-space to texture offset matrix by the pixel diameter.
//  Get uv sample offsets, taking advantage of row alignment:
//  Load samples, linearizing if necessary, etc.:
//  Sum weighted samples (weight sum must equal 1.0 for each channel):
//  This function is for testing only: Use an NxN grid with dynamic weights.
//  Compute xy sample offsets and subpixel weights:
//  Weights based on xy distances:
//  Get uv offset vectors along x and y directions:
//  Get a starting sample location:
//  Load, weight, and sum [linearized] samples:
///////////////////////  ANTIALIASING CODEPATH SELECTION  //////////////////////
//  Statically switch between antialiasing modes/levels:
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
////////////////////////////  MACROS AND CONSTANTS  ////////////////////////////
//  Curvature-related constants:
/////////////////////////////  CURVATURE FUNCTIONS /////////////////////////////
//  Requires:   1.) a, b, and c are quadratic formula coefficients
//              2.) b_over_2 = b/2.0 (simplifies terms to factor 2 out)
//              3.) b_over_2 must be guaranteed < 0.0 (avoids a branch)
//  Returns:    Returns float2(first_solution, discriminant), so the caller
//              can choose how to handle the "no intersection" case.  The
//              Kahan or Citardauq formula is used for numerical robustness.
//  Requires:   1.) view_vec and eye_pos_vec are 3D vectors in the sphere's
//                  local coordinate frame (eye_pos_vec is a position, i.e.
//                  a vector from the origin to the eye/camera)
//              2.) geom_radius is a global containing the sphere's radius
//  Returns:    Cast a ray of direction view_vec from eye_pos_vec at a
//              sphere of radius geom_radius, and return the distance to
//              the first intersection in units of length(view_vec).
//              http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
//  Quadratic formula coefficients (b_over_2 is guaranteed negative):
//  Requires:   1.) view_vec and eye_pos_vec are 3D vectors in the sphere's
//                  local coordinate frame (eye_pos_vec is a position, i.e.
//                  a vector from the origin to the eye/camera)
//              2.) geom_radius is a global containing the cylinder's radius
//  Returns:    Cast a ray of direction view_vec from eye_pos_vec at a
//              cylinder of radius geom_radius, and return the distance to
//              the first intersection in units of length(view_vec).  The
//              derivation of the coefficients is in Christer Ericson's
//              Real-Time Collision Detection, p. 195-196, and this version
//              uses LaGrange's identity to reduce operations.
//  Arbitrary "cylinder top" reference point for an infinite cylinder:
//  Quadratic formula coefficients (b_over_2 is guaranteed negative):
//  Requires:   An xyz intersection position on a cylinder.
//  Returns:    video_uv coords mapped to range [-0.5, 0.5]
//  Mapping:    Define square_uv.x to be the signed arc length in xz-space,
//              and define square_uv.y = -intersection_pos_local.y (+v = -y).
//  Start with a numerically robust arc length calculation.
//  Get a uv-mapping where [-0.5, 0.5] maps to a "square" area, then divide
//  by the aspect ratio to stretch the mapping appropriately:
//  Requires:   video_uv coords mapped to range [-0.5, 0.5]
//  Returns:    An xyz intersection position on a cylinder.  This is the
//              inverse of cylinder_xyz_to_uv().
//  Expand video_uv by the aspect ratio to get proportionate x/y lengths,
//  then calculate an xyz position for the cylindrical mapping above.
//  Or: z = sqrt(geom_radius**2 - x**2)
//  Or: z = geom_radius/sqrt(1.0 + tan(angle)**2), x = z * tan(angle)
//  Requires:   An xyz intersection position on a sphere.
//  Returns:    video_uv coords mapped to range [-0.5, 0.5]
//  Mapping:    First define square_uv.x/square_uv.y ==
//              intersection_pos_local.x/intersection_pos_local.y.  Then,
//              length(square_uv) is the arc length from the image center
//              at (0.0, 0.0, geom_radius) along the tangent great circle.
//              Credit for this mapping goes to cgwg: I never managed to
//              understand his code, but he told me his mapping was based on
//              great circle distances when I asked him about it, which
//              informed this very similar (almost identical) mapping.
//  Start with a numerically robust arc length calculation between the ray-
//  sphere intersection point and the image center using a method posted by
//  Roger Stafford on comp.soft-sys.matlab:
//  https://groups.google.com/d/msg/comp.soft-sys.matlab/zNbUui3bjcA/c0HV_bHSx9cJ
//  Get a uv-mapping where [-0.5, 0.5] maps to a "square" area, then divide
//  by the aspect ratio to stretch the mapping appropriately:
//  Requires:   video_uv coords mapped to range [-0.5, 0.5]
//  Returns:    An xyz intersection position on a sphere.  This is the
//              inverse of sphere_xyz_to_uv().
//  Expand video_uv by the aspect ratio to get proportionate x/y lengths,
//  then calculate an xyz position for the spherical mapping above.
//  Using length or sqrt here butchers the framerate on my 8800GTS if
//  this function is called too many times, and so does taking the max
//  component of square_uv/square_uv_unit (program length threshold?).
//float arc_len = length(square_uv);
//float2 xy_pos = xy_dist_from_sphere_center * (square_uv/FIX_ZERO(arc_len));
//  Requires:   An xyz intersection position on a cylinder.
//  Returns:    video_uv coords mapped to range [-0.5, 0.5]
//  Mapping:    Define square_uv.x to be the signed arc length in xz-space,
//              and define square_uv.y == signed arc length in yz-space.
//  See cylinder_xyz_to_uv() for implementation details (very similar).
//  Requires:   video_uv coords mapped to range [-0.5, 0.5]
//  Returns:    An xyz intersection position on a sphere.  This is the
//              inverse of sphere_alt_xyz_to_uv().
//  See cylinder_uv_to_xyz() for implementation details (very similar).
//  Get the intersection point on the primitive, given an eye position
//  and view vector already in its local coordinate frame:
//  Save the intersection position to an output parameter:
//  Transform into uv coords, but give out-of-range coords if the
//  view ray doesn't intersect the primitive in the first place:
//  Requires:   Parameters:
//              1.) Starting eye_pos is a global 3D position at which the
//                  camera contains all points in global_coords[] in its FOV
//              2.) geom_aspect = get_aspect_vector(
//                      IN.output_size.x / IN.output_size.y);
//              3.) global_coords is a point cloud containing global xyz
//                  coords of extreme points on the simulated CRT screen.
//              Globals:
//              1.) geom_view_dist must be > 0.0.  It controls the "near
//                  plane" used to interpret flat_video_uv as a view
//                  vector, which controls the field of view (FOV).
//              Eyespace coordinate frame: +x = right, +y = up, +z = back
//  Returns:    Return an eye position at which the point cloud spans as
//              much of the screen as possible (given the FOV controlled by
//              geom_view_dist) without being cropped or sheared.
//  Algorithm:
//  1.) Move the eye laterally to a point which attempts to maximize the
//      the amount we can move forward without clipping the CRT screen.
//  2.) Move forward by as much as possible without clipping the CRT.
//  Get the allowed movement range by solving for the eye_pos offsets
//  that result in each point being projected to a screen edge/corner in
//  pseudo-normalized device coords (where xy ranges from [-0.5, 0.5]
//  and z = eyespace z):
//      pndc_coord = float3(float2(eyespace_xyz.x, -eyespace_xyz.y)*
//      geom_view_dist / (geom_aspect * -eyespace_xyz.z), eyespace_xyz.z);
//  Notes:
//  The field of view is controlled by geom_view_dist's magnitude relative to
//  the view vector's x and y components:
//      view_vec.xy ranges from [-0.5, 0.5] * geom_aspect
//      view_vec.z = -geom_view_dist
//  But for the purposes of perspective divide, it should be considered:
//      view_vec.xy ranges from [-0.5, 0.5] * geom_aspect / geom_view_dist
//      view_vec.z = -1.0
//  0.) Get the eyespace coordinates of our point cloud:
//  1a.)For each point, find out how far we can move eye_pos in each
//      lateral direction without the point clipping the frustum.
//      Eyespace +y = up, screenspace +y = down, so flip y after
//      applying the eyespace offset (on the way to "clip space").
//  Solve for two offsets per point based on:
//      (eyespace_xyz.xy - offset_dr) * float2(1.0, -1.0) *
//      geom_view_dist / (geom_aspect * -eyespace_xyz.z) = float2(-0.5)
//      (eyespace_xyz.xy - offset_dr) * float2(1.0, -1.0) *
//      geom_view_dist / (geom_aspect * -eyespace_xyz.z) = float2(0.5)
//  offset_ul and offset_dr represent the farthest we can move the
//  eye_pos up-left and down-right.  Save the min of all offset_dr's
//  and the max of all offset_ul's (since it's negative).
//  1b.)Update eye_pos: Adding the average of offset_ul_max and
//      offset_dr_min gives it equal leeway on the top vs. bottom
//      and left vs. right.  Recalculate eyespace_coords accordingly.
//  2a.)For each point, find out how far we can move eye_pos forward
//      without the point clipping the frustum.  Flip the y
//      direction in advance (matters for a later step, not here).
//      Solve for four offsets per point based on:
//      eyespace_xyz_flipy.x * geom_view_dist /
//          (geom_aspect.x * (offset_z - eyespace_xyz_flipy.z)) =-0.5
//      eyespace_xyz_flipy.y * geom_view_dist /
//          (geom_aspect.y * (offset_z - eyespace_xyz_flipy.z)) =-0.5
//      eyespace_xyz_flipy.x * geom_view_dist /
//          (geom_aspect.x * (offset_z - eyespace_xyz_flipy.z)) = 0.5
//      eyespace_xyz_flipy.y * geom_view_dist /
//          (geom_aspect.y * (offset_z - eyespace_xyz_flipy.z)) = 0.5
//      We'll vectorize the actual computation.  Take the maximum of
//      these four for a single offset, and continue taking the max
//      for every point (use max because offset.z is negative).
//  Ignore offsets that push positive x/y values to opposite
//  boundaries, and vice versa, and don't let the camera move
//  past a point in the dead center of the screen:
//  2b.)Update eye_pos: Add the maximum (smallest negative) z offset.
//  Start with an initial eye_pos that includes the entire primitive
//  (sphere or cylinder) in its field-of-view:
//  Trigonometry/similar triangles say distance = geom_radius/sin(fov/2):
//  Get global xyz coords of extreme sample points on the simulated CRT
//  screen.  Start with the center, edge centers, and corners of the
//  video image.  We can't ignore backfacing points: They're occluded
//  by closer points on the primitive, but they may NOT be occluded by
//  the convex hull of the remaining samples (i.e. the remaining convex
//  hull might not envelope points that do occlude a back-facing point.)
//  Adding more inner image points could help in extreme cases, but too many
//  points will kille the framerate.  For safety, default to the initial
//  eye_pos if any z coords are negative:
//  Outsource the optimized eye_pos calculation:
//  Requires:   See get_curved_video_uv_coords_and_tangent_matrix for
//              descriptions of each parameter.
//  Returns:    Return a transformation matrix from 2D pixel-space vectors
//              (where (+1.0, +1.0) is a vector to one pixel down-right,
//              i.e. same directionality as uv texels) to 3D object-space
//              vectors in the CRT's local coordinate frame (right-handed)
//              ***which are tangent to the CRT surface at the intersection
//              position.***  (Basically, we want to convert pixel-space
//              vectors to 3D vectors along the CRT's surface, for later
//              conversion to uv vectors.)
//  Shorthand inputs:
//  Get a piecewise-linear matrix transforming from "pixelspace" offset
//  vectors (1.0 = one pixel) to object space vectors in the tangent
//  plane (faster than finding 3 view-object intersections).
//  1.) Get the local view vecs for the pixels to the right and down:
//  2.) Using the true intersection point, intersect the neighboring
//      view vectors with the tangent plane:
//  3.) Subtract the original intersection pos from its neighbors; the
//      resulting vectors are object-space vectors tangent to the plane.
//      These vectors are the object-space transformations of (1.0, 0.0)
//      and (0.0, 1.0) pixel offsets, so they form the first two basis
//      vectors of a pixelspace to object space transformation.  This
//      transformation is 2D to 3D, so use (0, 0, 0) for the third vector.
//  Requires:   See get_curved_video_uv_coords_and_tangent_matrix for
//              descriptions of each parameter.
//  Returns:    Return a transformation matrix from 3D object-space vectors
//              in the CRT's local coordinate frame (right-handed, +y = up)
//              to 2D video_uv vectors (+v = down).
//  Description:
//  The TBN matrix formed by the [tangent, bitangent, normal] basis
//  vectors transforms ordinary vectors from tangent->object space.
//  The cotangent matrix formed by the [cotangent, cobitangent, normal]
//  basis vectors transforms normal vectors (covectors) from
//  tangent->object space.  It's the inverse-transpose of the TBN matrix.
//  We want the inverse of the TBN matrix (transpose of the cotangent
//  matrix), which transforms ordinary vectors from object->tangent space.
//  Start by calculating the relevant basis vectors in accordance with
//  Christian Schüler's blog post "Followup: Normal Mapping Without
//  Precomputed Tangents":  http://www.thetenthplanet.de/archives/1180
//  With our particular uv mapping, the scale of the u and v directions
//  is determined entirely by the aspect ratio for cylindrical and ordinary
//  spherical mappings, and so tangent and bitangent lengths are also
//  determined by it (the alternate mapping is more complex).  Therefore, we
//  must ensure appropriate cotangent and cobitangent lengths as well.
//  Base these off the uv<=>xyz mappings for each primitive.
//  The tangent and bitangent vectors correspond with increasing u and v,
//  respectively.  Mathematically we'd base the cotangent/cobitangent on
//  those, but we'll compute the cotangent/cobitangent directly when we can.
//  geom_mode should be constant-folded without RUNTIME_GEOMETRY_MODE.
//  Sphere:
//  tangent = normalize(cross(normal, cross(x_vec, pos))) * geom_aspect.x
//  bitangent = normalize(cross(cross(y_vec, pos), normal)) * geom_aspect.y
//  inv_determinant = 1.0/length(cross(bitangent, tangent))
//  cotangent = cross(normal, bitangent) * inv_determinant
//            == normalize(cross(y_vec, pos)) * geom_aspect.y * inv_determinant
//  cobitangent = cross(tangent, normal) * inv_determinant
//            == normalize(cross(x_vec, pos)) * geom_aspect.x * inv_determinant
//  Simplified (scale by inv_determinant below):
//  Sphere, alternate mapping:
//  This mapping works a bit like the cylindrical mapping in two
//  directions, which makes the lengths and directions more complex.
//  Unfortunately, I can't find much of a shortcut:
//  Cylinder:
//  tangent = normalize(cross(y_vec, normal)) * geom_aspect.x;
//  bitangent = float3(0.0, -geom_aspect.y, 0.0);
//  inv_determinant = 1.0/length(cross(bitangent, tangent))
//  cotangent = cross(normal, bitangent) * inv_determinant
//            == normalize(cross(y_vec, pos)) * geom_aspect.y * inv_determinant
//  cobitangent = cross(tangent, normal) * inv_determinant
//            == float3(0.0, -geom_aspect.x, 0.0) * inv_determinant
//  The [cotangent, cobitangent, normal] column vecs form the cotangent
//  frame, i.e. the inverse-transpose TBN matrix.  Get its transpose:
//  Requires:   Parameters:
//              1.) flat_video_uv coords are in range [0.0, 1.0], where
//                  (0.0, 0.0) is the top-left corner of the screen and
//                  (1.0, 1.0) is the bottom-right corner.
//              2.) eye_pos_local is the 3D camera position in the simulated
//                  CRT's local coordinate frame.  For best results, it must
//                  be computed based on the same geom_view_dist used here.
//              3.) output_size_inv = float2(1.0)/IN.output_size
//              4.) geom_aspect = get_aspect_vector(
//                      IN.output_size.x / IN.output_size.y);
//              5.) geom_mode is a static or runtime mode setting:
//                  0 = off, 1 = sphere, 2 = sphere alt., 3 = cylinder
//              6.) global_to_local is a 3x3 matrix transforming (ordinary)
//                  worldspace vectors to the CRT's local coordinate frame
//              Globals:
//              1.) geom_view_dist must be > 0.0.  It controls the "near
//                  plane" used to interpret flat_video_uv as a view
//                  vector, which controls the field of view (FOV).
//  Returns:    Return final uv coords in [0.0, 1.0], and return a pixel-
//              space to video_uv tangent-space matrix in the out parameter.
//              (This matrix assumes pixel-space +y = down, like +v = down.)
//              We'll transform flat_video_uv into a view vector, project
//              the view vector from the camera/eye, intersect with a sphere
//              or cylinder representing the simulated CRT, and convert the
//              intersection position into final uv coords and a local
//              transformation matrix.
//  First get the 3D view vector (geom_aspect and geom_view_dist are globals):
//  1.) Center uv around (0.0, 0.0) and make (-0.5, -0.5) and (0.5, 0.5)
//      correspond to the top-left/bottom-right output screen corners.
//  2.) Multiply by geom_aspect to preemptively "undo" Retroarch's screen-
//      space 2D aspect correction.  We'll reapply it in uv-space.
//  3.) (x, y) = (u, -v), because +v is down in 2D screenspace, but +y
//      is up in 3D worldspace (enforce a right-handed system).
//  4.) The view vector z controls the "near plane" distance and FOV.
//      For the effect of "looking through a window" at a CRT, it should be
//      set equal to the user's distance from their physical screen, in
//      units of the viewport's physical diagonal size.
//  Transform the view vector into the CRT's local coordinate frame, convert
//  to video_uv coords, and get the local 3D intersection position:
//  Get a pixel-to-tangent-video-uv matrix.  The caller could deal with
//  all but one of these cases, but that would be more complicated.
//  Derivatives obtain a matrix very fast, but the direction of pixel-
//  space +y seems to depend on the pass.  Enforce the correct direction
//  on a best-effort basis (but it shouldn't matter for antialiasing).
//  Manually define a transformation matrix.  We'll assume pixel-space
//  +y = down, just like +v = down.
//  Get the surface normal based on the local intersection position:
//  Get pixel-to-object and object-to-tangent matrices and combine
//  them into a 2x2 pixel-to-tangent matrix for video_uv offsets:
//  Ignore curvature, and just consider flat scaling.  The
//  difference is only apparent with strong curvature:
//  COPYRIGHT NOTE FOR THIS FUNCTION:
//  Copyright (C) 2010-2012 cgwg, 2014 TroggleMonkey
//  This function uses an algorithm first coded in several of cgwg's GPL-
//  licensed lines in crt-geom-curved.cg and its ancestors.  The line
//  between algorithm and code is nearly indistinguishable here, so it's
//  unclear whether I could even release this project under a non-GPL
//  license with this function included.
//  Calculate border_dim_factor from the proximity to uv-space image
//  borders; geom_aspect/border_size/border/darkness/border_compress are globals:
///////////////////////////////////  HELPERS  //////////////////////////////////
//float2x2 scale_matrix = float2x2(scale.x, 0.0, 0.0, scale.y);
//return mul(scale_matrix, matrix);
//  Get aspect/overscan vectors from scalar parameters (likely uniforms):
//  Create a local-to-global rotation matrix for the CRT's coordinate
//  frame and its global-to-local inverse.  Rotate around the x axis
//  first (pitch) and then the y axis (yaw) with yucky Euler angles.
//  Positive angles go clockwise around the right-vec and up-vec.
//  Runtime shader parameters prevent us from computing these globally,
//  but we can still combine the pitch/yaw matrices by hand to cut a
//  few instructions.  Note that cg matrices fill row1 first, then row2,
//  etc. (row-major order).
//  Conceptual breakdown:
              static const float3x3 global_to_local =
transpose(local_to_global);
const float3x3 local_to_global = float3x3(
cos_tilt.x, sin_tilt.y*sin_tilt.x, cos_tilt.y*sin_tilt.x,
0.0, cos_tilt.y, sin_tilt.y,
sin_tilt.x, sin_tilt.y*cos_tilt.x, cos_tilt.y*cos_tilt.x);
//  Decompose the matrix into 3 float3's for output:
//  Get an optimal eye position based on geom_view_dist, viewport_aspect,
//  and CRT radius/rotation:
//  Localize some parameters:
//const float2 output_size_inv = output_size_inv;
//  Get flat and curved texture coords for the current fragment point sample
//  and a pixel_to_tangent_video_uv matrix for transforming pixel offsets:
//  video_uv = relative position in video frame, mapped to [0.0, 1.0] range
//  tex_uv = relative position in padded texture, mapped to [0.0, 1.0] range
//  Correct for overscan here (not in curvature code):
//  Get a matrix transforming pixel vectors to tex_uv vectors:
//  Sample!  Skip antialiasing if aa_level < 0.5 or both of these hold:
//  1.) Geometry/curvature isn't used
//  2.) Overscan == float2(1.0, 1.0)
//  Skipping AA is sharper, but it's only faster with dynamic branches.
// this next check seems to always return true, even when it shouldn't so disabling it for now
//  Sample the input with antialiasing (due to sharp phosphors, etc.):
//  Sample at each subpixel location:
//  Dim borders and output the final result:

*/

#pragma once

namespace RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,244,191,
217,24,53,241,24,31,
152,204,86,18,35,62,
96,143,1,0,0,0,
48,72,0,0,5,0,
0,0,52,0,0,0,
116,15,0,0,192,15,
0,0,192,16,0,0,
148,71,0,0,82,68,
69,70,56,15,0,0,
2,0,0,0,136,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
15,15,0,0,82,68,
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
0,0,7,0,0,0,
8,13,0,0,112,0,
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
4,0,0,0,0,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,210,10,0,0,
160,0,0,0,4,0,
0,0,0,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
241,10,0,0,164,0,
0,0,4,0,0,0,
0,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,16,11,
0,0,168,0,0,0,
4,0,0,0,0,0,
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
2,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,188,11,
0,0,192,0,0,0,
4,0,0,0,2,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,207,11,0,0,
196,0,0,0,4,0,
0,0,2,0,0,0,
96,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
229,11,0,0,200,0,
0,0,4,0,0,0,
2,0,0,0,96,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,254,11,
0,0,204,0,0,0,
4,0,0,0,2,0,
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
4,0,0,0,2,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,100,12,0,0,
220,0,0,0,4,0,
0,0,2,0,0,0,
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
32,14,0,0,0,0,
0,0,16,0,0,0,
2,0,0,0,60,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,96,14,
0,0,16,0,0,0,
16,0,0,0,0,0,
0,0,60,14,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,116,14,0,0,
32,0,0,0,16,0,
0,0,2,0,0,0,
60,14,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
134,14,0,0,48,0,
0,0,4,0,0,0,
0,0,0,0,160,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,196,14,
0,0,64,0,0,0,
16,0,0,0,0,0,
0,0,60,14,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,224,14,0,0,
80,0,0,0,16,0,
0,0,0,0,0,0,
60,14,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
249,14,0,0,96,0,
0,0,16,0,0,0,
0,0,0,0,60,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,112,97,
114,97,109,115,95,83,
111,117,114,99,101,83,
105,122,101,0,102,108,
111,97,116,52,0,171,
171,171,1,0,3,0,
1,0,4,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,50,14,
0,0,112,97,114,97,
109,115,95,79,114,105,
103,105,110,97,108,83,
105,122,101,0,112,97,
114,97,109,115,95,79,
117,116,112,117,116,83,
105,122,101,0,112,97,
114,97,109,115,95,70,
114,97,109,101,67,111,
117,110,116,0,100,119,
111,114,100,0,171,171,
0,0,19,0,1,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,152,14,0,0,
112,97,114,97,109,115,
95,77,65,83,75,69,
68,95,83,67,65,78,
76,73,78,69,83,83,
105,122,101,0,112,97,
114,97,109,115,95,72,
65,76,65,84,73,79,
78,95,66,76,85,82,
83,105,122,101,0,112,
97,114,97,109,115,95,
66,82,73,71,72,84,
80,65,83,83,83,105,
122,101,0,77,105,99,
114,111,115,111,102,116,
32,40,82,41,32,72,
76,83,76,32,83,104,
97,100,101,114,32,67,
111,109,112,105,108,101,
114,32,49,48,46,49,
0,171,73,83,71,78,
68,0,0,0,2,0,
0,0,8,0,0,0,
56,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,15,15,0,0,
56,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,3,3,0,0,
84,69,88,67,79,79,
82,68,0,171,171,171,
79,83,71,78,248,0,
0,0,9,0,0,0,
8,0,0,0,224,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
3,12,0,0,224,0,
0,0,2,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
12,3,0,0,224,0,
0,0,1,0,0,0,
0,0,0,0,3,0,
0,0,1,0,0,0,
15,0,0,0,224,0,
0,0,3,0,0,0,
0,0,0,0,3,0,
0,0,2,0,0,0,
7,8,0,0,224,0,
0,0,4,0,0,0,
0,0,0,0,3,0,
0,0,3,0,0,0,
15,0,0,0,224,0,
0,0,5,0,0,0,
0,0,0,0,3,0,
0,0,4,0,0,0,
7,8,0,0,224,0,
0,0,6,0,0,0,
0,0,0,0,3,0,
0,0,5,0,0,0,
7,8,0,0,224,0,
0,0,7,0,0,0,
0,0,0,0,3,0,
0,0,6,0,0,0,
7,8,0,0,233,0,
0,0,0,0,0,0,
1,0,0,0,3,0,
0,0,7,0,0,0,
15,0,0,0,84,69,
88,67,79,79,82,68,
0,83,86,95,80,111,
115,105,116,105,111,110,
0,171,171,171,83,72,
69,88,204,54,0,0,
80,0,1,0,179,13,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,0,0,0,0,
14,0,0,0,89,0,
0,4,70,142,32,0,
1,0,0,0,3,0,
0,0,95,0,0,3,
242,16,16,0,0,0,
0,0,95,0,0,3,
50,16,16,0,1,0,
0,0,101,0,0,3,
50,32,16,0,0,0,
0,0,101,0,0,3,
194,32,16,0,0,0,
0,0,101,0,0,3,
242,32,16,0,1,0,
0,0,101,0,0,3,
114,32,16,0,2,0,
0,0,101,0,0,3,
242,32,16,0,3,0,
0,0,101,0,0,3,
114,32,16,0,4,0,
0,0,101,0,0,3,
114,32,16,0,5,0,
0,0,101,0,0,3,
114,32,16,0,6,0,
0,0,103,0,0,4,
242,32,16,0,7,0,
0,0,1,0,0,0,
104,0,0,2,26,0,
0,0,56,0,0,8,
242,0,16,0,0,0,
0,0,86,21,16,0,
0,0,0,0,70,142,
32,0,0,0,0,0,
1,0,0,0,50,0,
0,10,242,0,16,0,
0,0,0,0,6,16,
16,0,0,0,0,0,
70,142,32,0,0,0,
0,0,0,0,0,0,
70,14,16,0,0,0,
0,0,50,0,0,10,
242,0,16,0,0,0,
0,0,166,26,16,0,
0,0,0,0,70,142,
32,0,0,0,0,0,
2,0,0,0,70,14,
16,0,0,0,0,0,
50,0,0,10,242,32,
16,0,7,0,0,0,
246,31,16,0,0,0,
0,0,70,142,32,0,
0,0,0,0,3,0,
0,0,70,14,16,0,
0,0,0,0,14,0,
0,11,242,32,16,0,
1,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,128,63,
70,132,32,0,1,0,
0,0,0,0,0,0,
14,0,0,11,194,32,
16,0,0,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,6,132,32,0,
1,0,0,0,2,0,
0,0,14,0,0,9,
18,0,16,0,0,0,
0,0,10,128,32,0,
1,0,0,0,2,0,
0,0,26,128,32,0,
1,0,0,0,2,0,
0,0,51,0,0,7,
18,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,1,64,
0,0,171,170,170,63,
54,0,0,5,34,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,15,0,0,7,
66,0,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
68,0,0,5,18,0,
16,0,1,0,0,0,
42,0,16,0,0,0,
0,0,56,0,0,7,
50,0,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,6,0,
16,0,1,0,0,0,
77,0,0,8,50,0,
16,0,2,0,0,0,
50,0,16,0,3,0,
0,0,230,138,32,0,
0,0,0,0,12,0,
0,0,54,0,0,6,
50,0,16,0,4,0,
0,0,70,0,16,128,
65,0,0,0,2,0,
0,0,56,0,0,7,
18,0,16,0,5,0,
0,0,26,0,16,0,
2,0,0,0,10,0,
16,0,2,0,0,0,
56,0,0,7,98,0,
16,0,5,0,0,0,
6,1,16,0,2,0,
0,0,86,4,16,0,
3,0,0,0,56,0,
0,7,66,0,16,0,
4,0,0,0,26,0,
16,0,3,0,0,0,
10,0,16,0,3,0,
0,0,54,0,0,6,
34,0,16,0,1,0,
0,0,26,128,32,0,
0,0,0,0,12,0,
0,0,54,0,0,6,
66,0,16,0,1,0,
0,0,26,0,16,128,
65,0,0,0,1,0,
0,0,15,0,0,7,
130,0,16,0,0,0,
0,0,134,0,16,0,
1,0,0,0,134,0,
16,0,1,0,0,0,
15,0,0,8,34,0,
16,0,1,0,0,0,
134,0,16,0,1,0,
0,0,70,0,16,128,
65,0,0,0,1,0,
0,0,14,0,0,7,
130,0,16,0,0,0,
0,0,26,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
0,0,0,8,34,0,
16,0,1,0,0,0,
58,0,16,128,193,0,
0,0,0,0,0,0,
1,64,0,0,0,0,
128,63,75,0,0,5,
34,0,16,0,1,0,
0,0,26,0,16,0,
1,0,0,0,50,0,
0,10,66,0,16,0,
1,0,0,0,58,0,
16,128,129,0,0,0,
0,0,0,0,1,64,
0,0,48,110,153,188,
1,64,0,0,39,22,
152,61,50,0,0,10,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,58,0,
16,128,129,0,0,0,
0,0,0,0,1,64,
0,0,132,52,89,190,
50,0,0,10,66,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,58,0,16,128,
129,0,0,0,0,0,
0,0,1,64,0,0,
164,13,201,63,56,0,
0,7,130,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,50,0,0,9,
130,0,16,0,1,0,
0,0,58,0,16,0,
1,0,0,0,1,64,
0,0,0,0,0,192,
1,64,0,0,219,15,
73,64,49,0,0,8,
130,0,16,0,0,0,
0,0,58,0,16,0,
0,0,0,0,58,0,
16,128,65,0,0,0,
0,0,0,0,1,0,
0,7,130,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
58,0,16,0,1,0,
0,0,50,0,0,9,
130,0,16,0,0,0,
0,0,42,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,56,0,0,7,
130,0,16,0,0,0,
0,0,58,0,16,0,
0,0,0,0,1,64,
0,0,0,0,0,63,
77,0,0,6,130,0,
16,0,0,0,0,0,
0,208,0,0,58,0,
16,0,0,0,0,0,
14,0,0,8,130,0,
16,0,1,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
58,0,16,0,0,0,
0,0,49,0,0,11,
50,0,16,0,2,0,
0,0,246,143,32,0,
0,0,0,0,11,0,
0,0,2,64,0,0,
0,0,32,64,0,0,
192,63,0,0,0,0,
0,0,0,0,52,0,
0,8,130,0,16,0,
2,0,0,0,58,0,
16,0,1,0,0,0,
26,128,32,0,0,0,
0,0,12,0,0,0,
54,0,0,5,66,0,
16,0,1,0,0,0,
1,64,0,0,0,0,
0,0,54,0,0,5,
66,0,16,0,2,0,
0,0,1,64,0,0,
0,0,0,0,55,0,
0,9,194,0,16,0,
6,0,0,0,6,0,
16,0,2,0,0,0,
166,14,16,0,1,0,
0,0,166,14,16,0,
2,0,0,0,14,0,
0,8,130,0,16,0,
0,0,0,0,1,64,
0,0,0,0,192,255,
10,128,32,0,0,0,
0,0,12,0,0,0,
77,0,0,6,0,208,
0,0,34,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
56,0,0,8,66,0,
16,0,7,0,0,0,
26,0,16,0,1,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,54,0,0,7,
66,0,16,0,8,0,
0,0,10,128,32,128,
129,0,0,0,0,0,
0,0,12,0,0,0,
54,0,0,8,50,0,
16,0,8,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,128,
0,0,0,0,0,0,
0,0,54,0,0,8,
50,0,16,0,9,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,128,0,0,0,0,
0,0,0,0,54,0,
0,6,66,0,16,0,
9,0,0,0,10,128,
32,0,0,0,0,0,
12,0,0,0,55,0,
0,9,226,0,16,0,
1,0,0,0,6,0,
16,0,2,0,0,0,
6,9,16,0,8,0,
0,0,6,9,16,0,
9,0,0,0,54,0,
0,8,50,0,16,0,
7,0,0,0,2,64,
0,0,0,0,193,255,
0,0,193,127,0,0,
0,0,0,0,0,0,
55,0,0,9,226,0,
16,0,1,0,0,0,
86,5,16,0,2,0,
0,0,6,9,16,0,
7,0,0,0,86,14,
16,0,1,0,0,0,
54,0,0,5,130,0,
16,0,4,0,0,0,
42,0,16,0,5,0,
0,0,16,0,0,7,
66,0,16,0,9,0,
0,0,198,2,16,0,
4,0,0,0,150,7,
16,0,1,0,0,0,
49,0,0,7,66,0,
16,0,2,0,0,0,
42,0,16,0,9,0,
0,0,1,64,0,0,
0,0,0,0,31,0,
4,3,42,0,16,0,
2,0,0,0,54,0,
0,5,114,0,16,0,
6,0,0,0,166,11,
16,0,6,0,0,0,
18,0,0,1,56,0,
0,9,194,0,16,0,
2,0,0,0,6,128,
32,0,0,0,0,0,
12,0,0,0,6,132,
32,0,0,0,0,0,
12,0,0,0,54,0,
0,5,130,0,16,0,
5,0,0,0,10,0,
16,0,3,0,0,0,
16,0,0,7,18,0,
16,0,9,0,0,0,
54,13,16,0,5,0,
0,0,150,7,16,0,
1,0,0,0,54,0,
0,5,18,0,16,0,
8,0,0,0,26,0,
16,0,3,0,0,0,
54,0,0,5,34,0,
16,0,8,0,0,0,
26,0,16,0,4,0,
0,0,15,0,0,7,
34,0,16,0,9,0,
0,0,70,0,16,0,
8,0,0,0,230,10,
16,0,1,0,0,0,
56,0,0,10,98,0,
16,0,1,0,0,0,
6,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,191,0,0,0,63,
0,0,0,0,56,0,
0,7,50,0,16,0,
10,0,0,0,150,5,
16,0,1,0,0,0,
150,5,16,0,1,0,
0,0,68,0,0,5,
50,0,16,0,10,0,
0,0,70,0,16,0,
10,0,0,0,56,0,
0,7,50,0,16,0,
10,0,0,0,150,5,
16,0,1,0,0,0,
70,0,16,0,10,0,
0,0,14,0,0,7,
194,0,16,0,10,0,
0,0,86,9,16,0,
1,0,0,0,6,4,
16,0,10,0,0,0,
14,0,0,8,194,0,
16,0,10,0,0,0,
166,14,16,0,10,0,
0,0,6,128,32,0,
0,0,0,0,12,0,
0,0,77,0,0,7,
18,0,16,0,11,0,
0,0,18,0,16,0,
12,0,0,0,42,0,
16,0,10,0,0,0,
56,0,0,8,130,0,
16,0,1,0,0,0,
10,0,16,0,11,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,56,0,0,7,
130,0,16,0,1,0,
0,0,58,0,16,0,
1,0,0,0,10,0,
16,0,10,0,0,0,
56,0,0,8,34,0,
16,0,11,0,0,0,
10,0,16,0,12,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,54,0,0,6,
18,0,16,0,11,0,
0,0,58,0,16,128,
65,0,0,0,1,0,
0,0,14,0,0,8,
82,0,16,0,10,0,
0,0,86,6,16,0,
1,0,0,0,6,128,
32,0,0,0,0,0,
12,0,0,0,77,0,
0,6,82,0,16,0,
10,0,0,0,0,208,
0,0,6,2,16,0,
10,0,0,0,56,0,
0,8,82,0,16,0,
10,0,0,0,6,2,
16,0,10,0,0,0,
6,128,32,0,0,0,
0,0,12,0,0,0,
50,0,0,10,194,0,
16,0,11,0,0,0,
6,8,16,128,65,0,
0,0,10,0,0,0,
6,8,16,0,10,0,
0,0,166,10,16,0,
2,0,0,0,75,0,
0,5,194,0,16,0,
12,0,0,0,166,14,
16,0,11,0,0,0,
54,0,0,6,50,0,
16,0,12,0,0,0,
134,0,16,128,65,0,
0,0,10,0,0,0,
54,0,0,6,50,0,
16,0,13,0,0,0,
150,5,16,128,65,0,
0,0,1,0,0,0,
54,0,0,6,66,0,
16,0,13,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
55,0,0,9,242,0,
16,0,12,0,0,0,
6,0,16,0,2,0,
0,0,134,13,16,0,
12,0,0,0,134,9,
16,0,13,0,0,0,
55,0,0,9,98,0,
16,0,1,0,0,0,
86,5,16,0,2,0,
0,0,6,1,16,0,
11,0,0,0,6,1,
16,0,12,0,0,0,
15,0,0,7,18,0,
16,0,11,0,0,0,
70,0,16,0,5,0,
0,0,150,5,16,0,
1,0,0,0,15,0,
0,7,34,0,16,0,
11,0,0,0,70,0,
16,0,8,0,0,0,
150,5,16,0,1,0,
0,0,15,0,0,7,
66,0,16,0,11,0,
0,0,182,15,16,0,
4,0,0,0,150,5,
16,0,1,0,0,0,
77,0,0,7,18,0,
16,0,10,0,0,0,
18,0,16,0,12,0,
0,0,58,0,16,0,
10,0,0,0,56,0,
0,8,34,0,16,0,
1,0,0,0,10,0,
16,0,10,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
56,0,0,7,34,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,26,0,16,0,
10,0,0,0,56,0,
0,8,34,0,16,0,
10,0,0,0,10,0,
16,0,12,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
54,0,0,6,18,0,
16,0,10,0,0,0,
26,0,16,128,65,0,
0,0,1,0,0,0,
55,0,0,9,98,0,
16,0,1,0,0,0,
86,5,16,0,2,0,
0,0,6,1,16,0,
10,0,0,0,166,11,
16,0,12,0,0,0,
15,0,0,7,18,0,
16,0,10,0,0,0,
70,0,16,0,5,0,
0,0,150,5,16,0,
1,0,0,0,15,0,
0,7,34,0,16,0,
10,0,0,0,70,0,
16,0,8,0,0,0,
150,5,16,0,1,0,
0,0,15,0,0,7,
66,0,16,0,10,0,
0,0,182,15,16,0,
4,0,0,0,150,5,
16,0,1,0,0,0,
56,0,0,10,98,0,
16,0,1,0,0,0,
6,0,16,0,0,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,191,0,0,0,63,
0,0,0,0,56,0,
0,7,50,0,16,0,
12,0,0,0,150,5,
16,0,1,0,0,0,
150,5,16,0,1,0,
0,0,68,0,0,5,
50,0,16,0,12,0,
0,0,70,0,16,0,
12,0,0,0,56,0,
0,7,50,0,16,0,
12,0,0,0,150,5,
16,0,1,0,0,0,
70,0,16,0,12,0,
0,0,77,0,0,6,
130,0,16,0,0,0,
0,0,0,208,0,0,
58,0,16,0,0,0,
0,0,56,0,0,8,
130,0,16,0,0,0,
0,0,58,0,16,0,
0,0,0,0,10,128,
32,0,0,0,0,0,
12,0,0,0,56,0,
0,7,50,0,16,0,
7,0,0,0,246,15,
16,0,0,0,0,0,
70,0,16,0,12,0,
0,0,14,0,0,8,
98,0,16,0,1,0,
0,0,86,6,16,0,
1,0,0,0,6,128,
32,0,0,0,0,0,
12,0,0,0,77,0,
0,7,18,0,16,0,
12,0,0,0,18,0,
16,0,13,0,0,0,
26,0,16,0,1,0,
0,0,56,0,0,8,
18,0,16,0,12,0,
0,0,10,0,16,0,
12,0,0,0,10,128,
32,0,0,0,0,0,
12,0,0,0,50,0,
0,10,130,0,16,0,
0,0,0,0,10,0,
16,128,65,0,0,0,
12,0,0,0,10,0,
16,0,12,0,0,0,
42,0,16,0,2,0,
0,0,75,0,0,5,
34,0,16,0,12,0,
0,0,58,0,16,0,
0,0,0,0,56,0,
0,8,66,0,16,0,
12,0,0,0,10,0,
16,0,13,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
55,0,0,9,162,0,
16,0,1,0,0,0,
6,0,16,0,2,0,
0,0,6,4,16,0,
12,0,0,0,6,8,
16,0,12,0,0,0,
55,0,0,9,162,0,
16,0,1,0,0,0,
86,5,16,0,2,0,
0,0,6,8,16,0,
7,0,0,0,86,13,
16,0,1,0,0,0,
15,0,0,7,18,0,
16,0,12,0,0,0,
118,15,16,0,5,0,
0,0,214,5,16,0,
1,0,0,0,56,0,
0,7,34,0,16,0,
12,0,0,0,58,0,
16,0,1,0,0,0,
26,0,16,0,4,0,
0,0,15,0,0,7,
66,0,16,0,12,0,
0,0,134,0,16,0,
4,0,0,0,214,5,
16,0,1,0,0,0,
77,0,0,7,18,0,
16,0,7,0,0,0,
18,0,16,0,13,0,
0,0,42,0,16,0,
1,0,0,0,56,0,
0,8,18,0,16,0,
14,0,0,0,10,0,
16,0,7,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
50,0,0,10,130,0,
16,0,0,0,0,0,
10,0,16,128,65,0,
0,0,14,0,0,0,
10,0,16,0,14,0,
0,0,42,0,16,0,
2,0,0,0,75,0,
0,5,34,0,16,0,
14,0,0,0,58,0,
16,0,0,0,0,0,
56,0,0,8,66,0,
16,0,14,0,0,0,
10,0,16,0,13,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,55,0,0,9,
98,0,16,0,1,0,
0,0,6,0,16,0,
2,0,0,0,6,1,
16,0,14,0,0,0,
6,2,16,0,14,0,
0,0,55,0,0,9,
98,0,16,0,1,0,
0,0,86,5,16,0,
2,0,0,0,86,6,
16,0,7,0,0,0,
86,6,16,0,1,0,
0,0,15,0,0,7,
18,0,16,0,7,0,
0,0,118,15,16,0,
5,0,0,0,150,5,
16,0,1,0,0,0,
56,0,0,7,34,0,
16,0,7,0,0,0,
42,0,16,0,1,0,
0,0,26,0,16,0,
4,0,0,0,15,0,
0,7,66,0,16,0,
7,0,0,0,134,0,
16,0,4,0,0,0,
150,5,16,0,1,0,
0,0,54,0,0,5,
66,0,16,0,0,0,
0,0,10,0,16,0,
1,0,0,0,56,0,
0,10,242,0,16,0,
1,0,0,0,134,8,
16,0,0,0,0,0,
2,64,0,0,0,0,
0,191,0,0,0,191,
0,0,0,63,0,0,
0,191,15,0,0,7,
130,0,16,0,0,0,
0,0,70,0,16,0,
1,0,0,0,70,0,
16,0,1,0,0,0,
68,0,0,5,130,0,
16,0,0,0,0,0,
58,0,16,0,0,0,
0,0,56,0,0,7,
50,0,16,0,13,0,
0,0,246,15,16,0,
0,0,0,0,70,0,
16,0,1,0,0,0,
14,0,0,7,130,0,
16,0,0,0,0,0,
26,0,16,0,1,0,
0,0,26,0,16,0,
13,0,0,0,14,0,
0,8,130,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
77,0,0,7,18,0,
16,0,14,0,0,0,
18,0,16,0,15,0,
0,0,58,0,16,0,
0,0,0,0,56,0,
0,8,130,0,16,0,
0,0,0,0,10,0,
16,0,14,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
56,0,0,7,50,0,
16,0,13,0,0,0,
246,15,16,0,0,0,
0,0,70,0,16,0,
13,0,0,0,56,0,
0,8,66,0,16,0,
13,0,0,0,10,0,
16,0,15,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
14,0,0,8,242,0,
16,0,14,0,0,0,
70,14,16,0,1,0,
0,0,6,128,32,0,
0,0,0,0,12,0,
0,0,77,0,0,6,
242,0,16,0,15,0,
0,0,0,208,0,0,
70,14,16,0,14,0,
0,0,56,0,0,8,
50,0,16,0,16,0,
0,0,70,0,16,0,
15,0,0,0,6,128,
32,0,0,0,0,0,
12,0,0,0,15,0,
0,7,130,0,16,0,
0,0,0,0,70,0,
16,0,16,0,0,0,
70,0,16,0,16,0,
0,0,50,0,0,12,
130,0,16,0,0,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,58,0,16,128,
65,0,0,0,0,0,
0,0,75,0,0,5,
66,0,16,0,16,0,
0,0,58,0,16,0,
0,0,0,0,77,0,
0,6,0,208,0,0,
50,0,16,0,14,0,
0,0,134,0,16,0,
14,0,0,0,56,0,
0,8,66,0,16,0,
17,0,0,0,10,0,
16,0,14,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
54,0,0,6,34,0,
16,0,17,0,0,0,
26,0,16,128,65,0,
0,0,1,0,0,0,
54,0,0,6,130,0,
16,0,16,0,0,0,
26,0,16,128,65,0,
0,0,16,0,0,0,
54,0,0,5,18,0,
16,0,17,0,0,0,
10,0,16,0,16,0,
0,0,55,0,0,9,
210,0,16,0,14,0,
0,0,6,0,16,0,
2,0,0,0,6,11,
16,0,16,0,0,0,
6,9,16,0,17,0,
0,0,54,0,0,6,
130,0,16,0,13,0,
0,0,26,0,16,128,
65,0,0,0,13,0,
0,0,55,0,0,9,
114,0,16,0,13,0,
0,0,86,5,16,0,
2,0,0,0,198,2,
16,0,13,0,0,0,
134,3,16,0,14,0,
0,0,16,0,0,7,
18,0,16,0,16,0,
0,0,54,13,16,0,
5,0,0,0,70,2,
16,0,13,0,0,0,
15,0,0,7,34,0,
16,0,16,0,0,0,
70,0,16,0,8,0,
0,0,150,5,16,0,
13,0,0,0,16,0,
0,7,66,0,16,0,
16,0,0,0,198,2,
16,0,4,0,0,0,
70,2,16,0,13,0,
0,0,15,0,0,7,
130,0,16,0,0,0,
0,0,230,10,16,0,
1,0,0,0,230,10,
16,0,1,0,0,0,
68,0,0,5,130,0,
16,0,0,0,0,0,
58,0,16,0,0,0,
0,0,56,0,0,7,
50,0,16,0,1,0,
0,0,246,15,16,0,
0,0,0,0,230,10,
16,0,1,0,0,0,
14,0,0,7,130,0,
16,0,0,0,0,0,
58,0,16,0,1,0,
0,0,26,0,16,0,
1,0,0,0,14,0,
0,8,130,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
77,0,0,7,18,0,
16,0,13,0,0,0,
18,0,16,0,14,0,
0,0,58,0,16,0,
0,0,0,0,56,0,
0,8,130,0,16,0,
0,0,0,0,10,0,
16,0,13,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
56,0,0,7,50,0,
16,0,13,0,0,0,
246,15,16,0,0,0,
0,0,70,0,16,0,
1,0,0,0,56,0,
0,8,66,0,16,0,
13,0,0,0,10,0,
16,0,14,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
56,0,0,8,50,0,
16,0,15,0,0,0,
230,10,16,0,15,0,
0,0,6,128,32,0,
0,0,0,0,12,0,
0,0,15,0,0,7,
130,0,16,0,0,0,
0,0,70,0,16,0,
15,0,0,0,70,0,
16,0,15,0,0,0,
50,0,0,12,130,0,
16,0,0,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
58,0,16,128,65,0,
0,0,0,0,0,0,
75,0,0,5,66,0,
16,0,15,0,0,0,
58,0,16,0,0,0,
0,0,56,0,0,8,
66,0,16,0,1,0,
0,0,26,0,16,0,
14,0,0,0,10,128,
32,0,0,0,0,0,
12,0,0,0,54,0,
0,6,34,0,16,0,
1,0,0,0,58,0,
16,128,65,0,0,0,
1,0,0,0,54,0,
0,6,130,0,16,0,
15,0,0,0,26,0,
16,128,65,0,0,0,
15,0,0,0,54,0,
0,5,18,0,16,0,
1,0,0,0,10,0,
16,0,15,0,0,0,
55,0,0,9,114,0,
16,0,1,0,0,0,
6,0,16,0,2,0,
0,0,198,2,16,0,
15,0,0,0,70,2,
16,0,1,0,0,0,
54,0,0,6,130,0,
16,0,13,0,0,0,
26,0,16,128,65,0,
0,0,13,0,0,0,
55,0,0,9,114,0,
16,0,1,0,0,0,
86,5,16,0,2,0,
0,0,198,2,16,0,
13,0,0,0,70,2,
16,0,1,0,0,0,
16,0,0,7,18,0,
16,0,13,0,0,0,
54,13,16,0,5,0,
0,0,70,2,16,0,
1,0,0,0,15,0,
0,7,34,0,16,0,
13,0,0,0,70,0,
16,0,8,0,0,0,
150,5,16,0,1,0,
0,0,16,0,0,7,
66,0,16,0,13,0,
0,0,198,2,16,0,
4,0,0,0,70,2,
16,0,1,0,0,0,
56,0,0,10,242,0,
16,0,1,0,0,0,
134,8,16,0,0,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,63,0,0,0,63,
0,0,0,63,15,0,
0,7,130,0,16,0,
0,0,0,0,70,0,
16,0,1,0,0,0,
70,0,16,0,1,0,
0,0,68,0,0,5,
130,0,16,0,0,0,
0,0,58,0,16,0,
0,0,0,0,56,0,
0,7,50,0,16,0,
14,0,0,0,246,15,
16,0,0,0,0,0,
70,0,16,0,1,0,
0,0,14,0,0,7,
130,0,16,0,0,0,
0,0,26,0,16,0,
1,0,0,0,26,0,
16,0,14,0,0,0,
14,0,0,8,130,0,
16,0,0,0,0,0,
58,0,16,0,0,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,77,0,0,7,
18,0,16,0,15,0,
0,0,18,0,16,0,
17,0,0,0,58,0,
16,0,0,0,0,0,
56,0,0,8,130,0,
16,0,0,0,0,0,
10,0,16,0,15,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,56,0,0,7,
50,0,16,0,14,0,
0,0,246,15,16,0,
0,0,0,0,70,0,
16,0,14,0,0,0,
56,0,0,8,66,0,
16,0,14,0,0,0,
10,0,16,0,17,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,14,0,0,8,
242,0,16,0,15,0,
0,0,70,14,16,0,
1,0,0,0,6,128,
32,0,0,0,0,0,
12,0,0,0,77,0,
0,6,242,0,16,0,
17,0,0,0,0,208,
0,0,70,14,16,0,
15,0,0,0,56,0,
0,8,50,0,16,0,
18,0,0,0,70,0,
16,0,17,0,0,0,
6,128,32,0,0,0,
0,0,12,0,0,0,
15,0,0,7,130,0,
16,0,0,0,0,0,
70,0,16,0,18,0,
0,0,70,0,16,0,
18,0,0,0,50,0,
0,12,130,0,16,0,
0,0,0,0,10,128,
32,0,0,0,0,0,
12,0,0,0,10,128,
32,0,0,0,0,0,
12,0,0,0,58,0,
16,128,65,0,0,0,
0,0,0,0,75,0,
0,5,66,0,16,0,
18,0,0,0,58,0,
16,0,0,0,0,0,
77,0,0,6,0,208,
0,0,50,0,16,0,
15,0,0,0,134,0,
16,0,15,0,0,0,
56,0,0,8,66,0,
16,0,19,0,0,0,
10,0,16,0,15,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,54,0,0,6,
34,0,16,0,19,0,
0,0,26,0,16,128,
65,0,0,0,1,0,
0,0,54,0,0,6,
130,0,16,0,18,0,
0,0,26,0,16,128,
65,0,0,0,18,0,
0,0,54,0,0,5,
18,0,16,0,19,0,
0,0,10,0,16,0,
18,0,0,0,55,0,
0,9,210,0,16,0,
15,0,0,0,6,0,
16,0,2,0,0,0,
6,11,16,0,18,0,
0,0,6,9,16,0,
19,0,0,0,54,0,
0,6,130,0,16,0,
14,0,0,0,26,0,
16,128,65,0,0,0,
14,0,0,0,55,0,
0,9,114,0,16,0,
14,0,0,0,86,5,
16,0,2,0,0,0,
198,2,16,0,14,0,
0,0,134,3,16,0,
15,0,0,0,16,0,
0,7,18,0,16,0,
18,0,0,0,54,13,
16,0,5,0,0,0,
70,2,16,0,14,0,
0,0,15,0,0,7,
34,0,16,0,18,0,
0,0,70,0,16,0,
8,0,0,0,150,5,
16,0,14,0,0,0,
16,0,0,7,66,0,
16,0,18,0,0,0,
198,2,16,0,4,0,
0,0,70,2,16,0,
14,0,0,0,15,0,
0,7,130,0,16,0,
0,0,0,0,230,10,
16,0,1,0,0,0,
230,10,16,0,1,0,
0,0,68,0,0,5,
130,0,16,0,0,0,
0,0,58,0,16,0,
0,0,0,0,56,0,
0,7,50,0,16,0,
1,0,0,0,246,15,
16,0,0,0,0,0,
230,10,16,0,1,0,
0,0,14,0,0,7,
130,0,16,0,0,0,
0,0,58,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
14,0,0,8,130,0,
16,0,0,0,0,0,
58,0,16,0,0,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,77,0,0,7,
18,0,16,0,14,0,
0,0,18,0,16,0,
15,0,0,0,58,0,
16,0,0,0,0,0,
56,0,0,8,130,0,
16,0,0,0,0,0,
10,0,16,0,14,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,56,0,0,7,
50,0,16,0,14,0,
0,0,246,15,16,0,
0,0,0,0,70,0,
16,0,1,0,0,0,
56,0,0,8,66,0,
16,0,14,0,0,0,
10,0,16,0,15,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,56,0,0,8,
50,0,16,0,17,0,
0,0,230,10,16,0,
17,0,0,0,6,128,
32,0,0,0,0,0,
12,0,0,0,15,0,
0,7,130,0,16,0,
0,0,0,0,70,0,
16,0,17,0,0,0,
70,0,16,0,17,0,
0,0,50,0,0,12,
130,0,16,0,0,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,58,0,16,128,
65,0,0,0,0,0,
0,0,75,0,0,5,
66,0,16,0,17,0,
0,0,58,0,16,0,
0,0,0,0,56,0,
0,8,66,0,16,0,
1,0,0,0,26,0,
16,0,15,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
54,0,0,6,34,0,
16,0,1,0,0,0,
58,0,16,128,65,0,
0,0,1,0,0,0,
54,0,0,6,130,0,
16,0,17,0,0,0,
26,0,16,128,65,0,
0,0,17,0,0,0,
54,0,0,5,18,0,
16,0,1,0,0,0,
10,0,16,0,17,0,
0,0,55,0,0,9,
114,0,16,0,1,0,
0,0,6,0,16,0,
2,0,0,0,198,2,
16,0,17,0,0,0,
70,2,16,0,1,0,
0,0,54,0,0,6,
130,0,16,0,14,0,
0,0,26,0,16,128,
65,0,0,0,14,0,
0,0,55,0,0,9,
114,0,16,0,1,0,
0,0,86,5,16,0,
2,0,0,0,198,2,
16,0,14,0,0,0,
70,2,16,0,1,0,
0,0,16,0,0,7,
18,0,16,0,2,0,
0,0,54,13,16,0,
5,0,0,0,70,2,
16,0,1,0,0,0,
15,0,0,7,34,0,
16,0,2,0,0,0,
70,0,16,0,8,0,
0,0,150,5,16,0,
1,0,0,0,16,0,
0,7,66,0,16,0,
2,0,0,0,198,2,
16,0,4,0,0,0,
70,2,16,0,1,0,
0,0,0,0,0,8,
114,0,16,0,1,0,
0,0,166,11,16,128,
65,0,0,0,6,0,
0,0,70,2,16,0,
9,0,0,0,0,0,
0,8,178,0,16,0,
8,0,0,0,166,14,
16,128,65,0,0,0,
6,0,0,0,70,8,
16,0,11,0,0,0,
0,0,0,8,114,0,
16,0,14,0,0,0,
166,11,16,128,65,0,
0,0,6,0,0,0,
70,2,16,0,10,0,
0,0,0,0,0,8,
114,0,16,0,15,0,
0,0,166,11,16,128,
65,0,0,0,6,0,
0,0,70,2,16,0,
12,0,0,0,0,0,
0,8,114,0,16,0,
17,0,0,0,166,11,
16,128,65,0,0,0,
6,0,0,0,70,2,
16,0,7,0,0,0,
0,0,0,8,114,0,
16,0,19,0,0,0,
166,11,16,128,65,0,
0,0,6,0,0,0,
70,2,16,0,16,0,
0,0,0,0,0,8,
114,0,16,0,20,0,
0,0,166,11,16,128,
65,0,0,0,6,0,
0,0,70,2,16,0,
13,0,0,0,0,0,
0,8,114,0,16,0,
21,0,0,0,166,11,
16,128,65,0,0,0,
6,0,0,0,70,2,
16,0,18,0,0,0,
0,0,0,8,114,0,
16,0,22,0,0,0,
166,11,16,128,65,0,
0,0,6,0,0,0,
70,2,16,0,2,0,
0,0,56,0,0,10,
50,0,16,0,23,0,
0,0,166,10,16,0,
8,0,0,0,2,64,
0,0,0,0,32,65,
0,0,32,193,0,0,
0,0,0,0,0,0,
56,0,0,8,242,0,
16,0,24,0,0,0,
70,4,16,0,0,0,
0,0,166,10,16,128,
65,0,0,0,1,0,
0,0,56,0,0,10,
242,0,16,0,24,0,
0,0,70,14,16,0,
24,0,0,0,2,64,
0,0,0,0,0,191,
0,0,0,191,0,0,
0,63,0,0,0,63,
56,0,0,11,242,0,
16,0,25,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,191,
0,0,128,63,0,0,
128,191,86,133,32,0,
0,0,0,0,12,0,
0,0,14,0,0,7,
242,0,16,0,24,0,
0,0,70,14,16,0,
24,0,0,0,70,14,
16,0,25,0,0,0,
0,0,0,8,242,0,
16,0,1,0,0,0,
70,4,16,0,1,0,
0,0,70,14,16,128,
65,0,0,0,24,0,
0,0,51,0,0,7,
50,0,16,0,1,0,
0,0,70,0,16,0,
1,0,0,0,6,0,
16,0,23,0,0,0,
52,0,0,7,194,0,
16,0,1,0,0,0,
166,14,16,0,1,0,
0,0,86,5,16,0,
23,0,0,0,56,0,
0,8,242,0,16,0,
23,0,0,0,70,4,
16,0,0,0,0,0,
246,15,16,128,65,0,
0,0,8,0,0,0,
56,0,0,10,242,0,
16,0,23,0,0,0,
70,14,16,0,23,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,191,0,0,0,63,
0,0,0,63,14,0,
0,7,242,0,16,0,
23,0,0,0,70,14,
16,0,23,0,0,0,
70,14,16,0,25,0,
0,0,0,0,0,8,
242,0,16,0,8,0,
0,0,70,4,16,0,
8,0,0,0,70,14,
16,128,65,0,0,0,
23,0,0,0,51,0,
0,7,50,0,16,0,
1,0,0,0,70,0,
16,0,1,0,0,0,
70,0,16,0,8,0,
0,0,52,0,0,7,
194,0,16,0,1,0,
0,0,166,14,16,0,
1,0,0,0,166,14,
16,0,8,0,0,0,
56,0,0,8,242,0,
16,0,8,0,0,0,
70,4,16,0,0,0,
0,0,166,10,16,128,
65,0,0,0,14,0,
0,0,56,0,0,10,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,2,64,
0,0,0,0,0,191,
0,0,0,191,0,0,
0,63,0,0,0,63,
14,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
25,0,0,0,0,0,
0,8,242,0,16,0,
8,0,0,0,70,14,
16,128,65,0,0,0,
8,0,0,0,70,4,
16,0,14,0,0,0,
51,0,0,7,50,0,
16,0,1,0,0,0,
70,0,16,0,1,0,
0,0,70,0,16,0,
8,0,0,0,52,0,
0,7,194,0,16,0,
1,0,0,0,166,14,
16,0,1,0,0,0,
166,14,16,0,8,0,
0,0,56,0,0,8,
242,0,16,0,8,0,
0,0,70,4,16,0,
0,0,0,0,166,10,
16,128,65,0,0,0,
15,0,0,0,56,0,
0,10,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
2,64,0,0,0,0,
0,191,0,0,0,191,
0,0,0,63,0,0,
0,63,14,0,0,7,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,70,14,
16,0,25,0,0,0,
0,0,0,8,242,0,
16,0,8,0,0,0,
70,14,16,128,65,0,
0,0,8,0,0,0,
70,4,16,0,15,0,
0,0,51,0,0,7,
50,0,16,0,1,0,
0,0,70,0,16,0,
1,0,0,0,70,0,
16,0,8,0,0,0,
52,0,0,7,194,0,
16,0,1,0,0,0,
166,14,16,0,1,0,
0,0,166,14,16,0,
8,0,0,0,56,0,
0,8,242,0,16,0,
8,0,0,0,70,4,
16,0,0,0,0,0,
166,10,16,128,65,0,
0,0,17,0,0,0,
56,0,0,10,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,191,0,0,0,63,
0,0,0,63,14,0,
0,7,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
70,14,16,0,25,0,
0,0,0,0,0,8,
242,0,16,0,8,0,
0,0,70,14,16,128,
65,0,0,0,8,0,
0,0,70,4,16,0,
17,0,0,0,51,0,
0,7,50,0,16,0,
1,0,0,0,70,0,
16,0,1,0,0,0,
70,0,16,0,8,0,
0,0,52,0,0,7,
194,0,16,0,1,0,
0,0,166,14,16,0,
1,0,0,0,166,14,
16,0,8,0,0,0,
56,0,0,8,242,0,
16,0,8,0,0,0,
70,4,16,0,0,0,
0,0,166,10,16,128,
65,0,0,0,19,0,
0,0,56,0,0,10,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,2,64,
0,0,0,0,0,191,
0,0,0,191,0,0,
0,63,0,0,0,63,
14,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
25,0,0,0,0,0,
0,8,242,0,16,0,
8,0,0,0,70,14,
16,128,65,0,0,0,
8,0,0,0,70,4,
16,0,19,0,0,0,
51,0,0,7,50,0,
16,0,1,0,0,0,
70,0,16,0,1,0,
0,0,70,0,16,0,
8,0,0,0,52,0,
0,7,194,0,16,0,
1,0,0,0,166,14,
16,0,1,0,0,0,
166,14,16,0,8,0,
0,0,56,0,0,8,
242,0,16,0,8,0,
0,0,70,4,16,0,
0,0,0,0,166,10,
16,128,65,0,0,0,
20,0,0,0,56,0,
0,10,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
2,64,0,0,0,0,
0,191,0,0,0,191,
0,0,0,63,0,0,
0,63,14,0,0,7,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,70,14,
16,0,25,0,0,0,
0,0,0,8,242,0,
16,0,8,0,0,0,
70,14,16,128,65,0,
0,0,8,0,0,0,
70,4,16,0,20,0,
0,0,51,0,0,7,
50,0,16,0,1,0,
0,0,70,0,16,0,
1,0,0,0,70,0,
16,0,8,0,0,0,
52,0,0,7,194,0,
16,0,1,0,0,0,
166,14,16,0,1,0,
0,0,166,14,16,0,
8,0,0,0,56,0,
0,8,242,0,16,0,
8,0,0,0,70,4,
16,0,0,0,0,0,
166,10,16,128,65,0,
0,0,21,0,0,0,
56,0,0,10,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,191,0,0,0,63,
0,0,0,63,14,0,
0,7,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
70,14,16,0,25,0,
0,0,0,0,0,8,
242,0,16,0,8,0,
0,0,70,14,16,128,
65,0,0,0,8,0,
0,0,70,4,16,0,
21,0,0,0,51,0,
0,7,50,0,16,0,
1,0,0,0,70,0,
16,0,1,0,0,0,
70,0,16,0,8,0,
0,0,52,0,0,7,
194,0,16,0,1,0,
0,0,166,14,16,0,
1,0,0,0,166,14,
16,0,8,0,0,0,
56,0,0,8,242,0,
16,0,8,0,0,0,
70,4,16,0,0,0,
0,0,166,10,16,128,
65,0,0,0,22,0,
0,0,56,0,0,10,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,2,64,
0,0,0,0,0,191,
0,0,0,191,0,0,
0,63,0,0,0,63,
14,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
25,0,0,0,0,0,
0,8,242,0,16,0,
8,0,0,0,70,14,
16,128,65,0,0,0,
8,0,0,0,70,4,
16,0,22,0,0,0,
51,0,0,7,50,0,
16,0,1,0,0,0,
70,0,16,0,1,0,
0,0,70,0,16,0,
8,0,0,0,52,0,
0,7,194,0,16,0,
1,0,0,0,166,14,
16,0,1,0,0,0,
166,14,16,0,8,0,
0,0,0,0,0,7,
50,0,16,0,1,0,
0,0,70,0,16,0,
1,0,0,0,230,10,
16,0,1,0,0,0,
56,0,0,10,50,0,
16,0,6,0,0,0,
70,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,63,0,0,
0,63,0,0,0,0,
0,0,0,0,0,0,
0,8,114,0,16,0,
1,0,0,0,70,3,
16,128,65,0,0,0,
6,0,0,0,70,2,
16,0,9,0,0,0,
0,0,0,8,114,0,
16,0,8,0,0,0,
70,3,16,128,65,0,
0,0,6,0,0,0,
70,2,16,0,11,0,
0,0,0,0,0,8,
114,0,16,0,9,0,
0,0,70,3,16,128,
65,0,0,0,6,0,
0,0,70,2,16,0,
10,0,0,0,0,0,
0,8,114,0,16,0,
10,0,0,0,70,3,
16,128,65,0,0,0,
6,0,0,0,70,2,
16,0,12,0,0,0,
0,0,0,8,114,0,
16,0,7,0,0,0,
70,3,16,128,65,0,
0,0,6,0,0,0,
70,2,16,0,7,0,
0,0,0,0,0,8,
114,0,16,0,11,0,
0,0,70,3,16,128,
65,0,0,0,6,0,
0,0,70,2,16,0,
16,0,0,0,0,0,
0,8,114,0,16,0,
12,0,0,0,70,3,
16,128,65,0,0,0,
6,0,0,0,70,2,
16,0,13,0,0,0,
0,0,0,8,114,0,
16,0,13,0,0,0,
70,3,16,128,65,0,
0,0,6,0,0,0,
70,2,16,0,18,0,
0,0,0,0,0,8,
114,0,16,0,2,0,
0,0,70,3,16,128,
65,0,0,0,6,0,
0,0,70,2,16,0,
2,0,0,0,56,0,
0,7,130,0,16,0,
0,0,0,0,58,0,
16,0,2,0,0,0,
1,64,0,0,0,0,
32,193,54,0,0,8,
82,0,16,0,14,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
0,0,0,0,128,63,
0,0,0,0,54,0,
0,6,34,0,16,0,
14,0,0,0,26,128,
32,0,0,0,0,0,
12,0,0,0,56,0,
0,7,114,0,16,0,
15,0,0,0,70,2,
16,0,1,0,0,0,
70,2,16,0,14,0,
0,0,54,0,0,6,
82,0,16,0,16,0,
0,0,86,133,32,0,
0,0,0,0,12,0,
0,0,54,0,0,8,
162,0,16,0,16,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
128,191,0,0,0,0,
0,0,128,191,56,0,
0,7,242,0,16,0,
17,0,0,0,70,4,
16,0,15,0,0,0,
230,14,16,0,16,0,
0,0,56,0,0,10,
242,0,16,0,18,0,
0,0,134,8,16,0,
0,0,0,0,2,64,
0,0,0,0,0,191,
0,0,0,191,0,0,
0,63,0,0,0,63,
14,0,0,7,242,0,
16,0,17,0,0,0,
70,14,16,0,17,0,
0,0,70,14,16,0,
18,0,0,0,50,0,
0,9,242,0,16,0,
17,0,0,0,166,10,
16,0,1,0,0,0,
166,10,16,0,14,0,
0,0,70,14,16,0,
17,0,0,0,49,0,
0,10,194,0,16,0,
1,0,0,0,6,4,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,128,52,0,0,7,
66,0,16,0,0,0,
0,0,58,0,16,0,
0,0,0,0,10,0,
16,0,17,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
42,0,16,0,1,0,
0,0,42,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
49,0,0,10,50,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,128,0,0,0,0,
0,0,0,0,0,0,
0,0,22,5,16,0,
1,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,26,0,
16,0,17,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,10,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,42,0,16,0,
17,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
26,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
58,0,16,0,17,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,58,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,66,0,16,0,
0,0,0,0,42,0,
16,0,15,0,0,0,
42,0,16,0,0,0,
0,0,56,0,0,7,
114,0,16,0,1,0,
0,0,70,2,16,0,
8,0,0,0,70,2,
16,0,14,0,0,0,
56,0,0,7,242,0,
16,0,15,0,0,0,
230,14,16,0,16,0,
0,0,70,4,16,0,
1,0,0,0,14,0,
0,7,242,0,16,0,
15,0,0,0,70,14,
16,0,15,0,0,0,
70,14,16,0,18,0,
0,0,50,0,0,9,
242,0,16,0,15,0,
0,0,166,10,16,0,
8,0,0,0,166,10,
16,0,14,0,0,0,
70,14,16,0,15,0,
0,0,49,0,0,10,
50,0,16,0,1,0,
0,0,70,0,16,0,
8,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,128,0,0,
0,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,10,0,16,0,
15,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,10,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,49,0,
0,10,146,0,16,0,
1,0,0,0,2,64,
0,0,0,0,0,128,
0,0,0,0,0,0,
0,0,0,0,0,0,
86,1,16,0,8,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,26,0,16,0,
15,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
42,0,16,0,15,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,58,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,58,0,
16,0,15,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,26,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
66,0,16,0,0,0,
0,0,42,0,16,0,
1,0,0,0,42,0,
16,0,0,0,0,0,
56,0,0,7,114,0,
16,0,1,0,0,0,
70,2,16,0,9,0,
0,0,70,2,16,0,
14,0,0,0,56,0,
0,7,242,0,16,0,
8,0,0,0,230,14,
16,0,16,0,0,0,
70,4,16,0,1,0,
0,0,14,0,0,7,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,70,14,
16,0,18,0,0,0,
50,0,0,9,242,0,
16,0,8,0,0,0,
166,10,16,0,9,0,
0,0,166,10,16,0,
14,0,0,0,70,14,
16,0,8,0,0,0,
49,0,0,10,50,0,
16,0,1,0,0,0,
70,0,16,0,9,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,128,0,0,0,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
10,0,16,0,8,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,10,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,49,0,0,10,
146,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,128,0,0,
0,0,0,0,0,0,
0,0,0,0,86,1,
16,0,9,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
26,0,16,0,8,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,10,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,42,0,
16,0,8,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,58,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,58,0,16,0,
8,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
26,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,66,0,
16,0,0,0,0,0,
42,0,16,0,1,0,
0,0,42,0,16,0,
0,0,0,0,56,0,
0,7,114,0,16,0,
1,0,0,0,70,2,
16,0,10,0,0,0,
70,2,16,0,14,0,
0,0,56,0,0,7,
242,0,16,0,8,0,
0,0,230,14,16,0,
16,0,0,0,70,4,
16,0,1,0,0,0,
14,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
18,0,0,0,50,0,
0,9,242,0,16,0,
8,0,0,0,166,10,
16,0,10,0,0,0,
166,10,16,0,14,0,
0,0,70,14,16,0,
8,0,0,0,49,0,
0,10,50,0,16,0,
1,0,0,0,70,0,
16,0,10,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,128,
0,0,0,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,10,0,
16,0,8,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
49,0,0,10,146,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,128,0,0,0,0,
0,0,0,0,0,0,
0,0,86,1,16,0,
10,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,26,0,
16,0,8,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,10,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,42,0,16,0,
8,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
58,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
58,0,16,0,8,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,26,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,66,0,16,0,
0,0,0,0,42,0,
16,0,1,0,0,0,
42,0,16,0,0,0,
0,0,56,0,0,7,
114,0,16,0,1,0,
0,0,70,2,16,0,
7,0,0,0,70,2,
16,0,14,0,0,0,
56,0,0,7,242,0,
16,0,8,0,0,0,
230,14,16,0,16,0,
0,0,70,4,16,0,
1,0,0,0,14,0,
0,7,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
70,14,16,0,18,0,
0,0,50,0,0,9,
242,0,16,0,8,0,
0,0,166,10,16,0,
7,0,0,0,166,10,
16,0,14,0,0,0,
70,14,16,0,8,0,
0,0,49,0,0,10,
50,0,16,0,1,0,
0,0,70,0,16,0,
7,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,128,0,0,
0,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,10,0,16,0,
8,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,10,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,49,0,
0,10,146,0,16,0,
1,0,0,0,2,64,
0,0,0,0,0,128,
0,0,0,0,0,0,
0,0,0,0,0,0,
86,1,16,0,7,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,26,0,16,0,
8,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
42,0,16,0,8,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,58,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,58,0,
16,0,8,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,26,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
66,0,16,0,0,0,
0,0,42,0,16,0,
1,0,0,0,42,0,
16,0,0,0,0,0,
56,0,0,7,114,0,
16,0,1,0,0,0,
70,2,16,0,11,0,
0,0,70,2,16,0,
14,0,0,0,56,0,
0,7,242,0,16,0,
7,0,0,0,230,14,
16,0,16,0,0,0,
70,4,16,0,1,0,
0,0,14,0,0,7,
242,0,16,0,7,0,
0,0,70,14,16,0,
7,0,0,0,70,14,
16,0,18,0,0,0,
50,0,0,9,242,0,
16,0,7,0,0,0,
166,10,16,0,11,0,
0,0,166,10,16,0,
14,0,0,0,70,14,
16,0,7,0,0,0,
49,0,0,10,50,0,
16,0,1,0,0,0,
70,0,16,0,11,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,128,0,0,0,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
10,0,16,0,7,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,10,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,49,0,0,10,
146,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,128,0,0,
0,0,0,0,0,0,
0,0,0,0,86,1,
16,0,11,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
26,0,16,0,7,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,10,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,42,0,
16,0,7,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,58,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,58,0,16,0,
7,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
26,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,66,0,
16,0,0,0,0,0,
42,0,16,0,1,0,
0,0,42,0,16,0,
0,0,0,0,56,0,
0,7,114,0,16,0,
1,0,0,0,70,2,
16,0,12,0,0,0,
70,2,16,0,14,0,
0,0,56,0,0,7,
242,0,16,0,7,0,
0,0,230,14,16,0,
16,0,0,0,70,4,
16,0,1,0,0,0,
14,0,0,7,242,0,
16,0,7,0,0,0,
70,14,16,0,7,0,
0,0,70,14,16,0,
18,0,0,0,50,0,
0,9,242,0,16,0,
7,0,0,0,166,10,
16,0,12,0,0,0,
166,10,16,0,14,0,
0,0,70,14,16,0,
7,0,0,0,49,0,
0,10,50,0,16,0,
1,0,0,0,70,0,
16,0,12,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,128,
0,0,0,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,10,0,
16,0,7,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
49,0,0,10,146,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,128,0,0,0,0,
0,0,0,0,0,0,
0,0,86,1,16,0,
12,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,26,0,
16,0,7,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,10,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,42,0,16,0,
7,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
58,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
58,0,16,0,7,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,26,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,66,0,16,0,
0,0,0,0,42,0,
16,0,1,0,0,0,
42,0,16,0,0,0,
0,0,56,0,0,7,
114,0,16,0,1,0,
0,0,70,2,16,0,
13,0,0,0,70,2,
16,0,14,0,0,0,
56,0,0,7,242,0,
16,0,7,0,0,0,
230,14,16,0,16,0,
0,0,70,4,16,0,
1,0,0,0,14,0,
0,7,242,0,16,0,
7,0,0,0,70,14,
16,0,7,0,0,0,
70,14,16,0,18,0,
0,0,50,0,0,9,
242,0,16,0,7,0,
0,0,166,10,16,0,
13,0,0,0,166,10,
16,0,14,0,0,0,
70,14,16,0,7,0,
0,0,49,0,0,10,
50,0,16,0,1,0,
0,0,70,0,16,0,
13,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,128,0,0,
0,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,10,0,16,0,
7,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,10,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,49,0,
0,10,146,0,16,0,
1,0,0,0,2,64,
0,0,0,0,0,128,
0,0,0,0,0,0,
0,0,0,0,0,0,
86,1,16,0,13,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,26,0,16,0,
7,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
42,0,16,0,7,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,58,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,58,0,
16,0,7,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,26,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
66,0,16,0,0,0,
0,0,42,0,16,0,
1,0,0,0,42,0,
16,0,0,0,0,0,
56,0,0,7,114,0,
16,0,1,0,0,0,
70,2,16,0,2,0,
0,0,70,2,16,0,
14,0,0,0,56,0,
0,7,242,0,16,0,
7,0,0,0,70,14,
16,0,16,0,0,0,
70,4,16,0,1,0,
0,0,14,0,0,7,
242,0,16,0,7,0,
0,0,70,14,16,0,
7,0,0,0,70,14,
16,0,18,0,0,0,
50,0,0,9,242,0,
16,0,7,0,0,0,
166,10,16,0,2,0,
0,0,166,10,16,0,
14,0,0,0,70,14,
16,0,7,0,0,0,
49,0,0,10,50,0,
16,0,1,0,0,0,
70,0,16,0,2,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,128,0,0,0,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
10,0,16,0,7,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,10,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,49,0,0,10,
146,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,128,0,0,
0,0,0,0,0,0,
0,0,0,0,86,1,
16,0,2,0,0,0,
52,0,0,7,130,0,
16,0,0,0,0,0,
26,0,16,0,7,0,
0,0,42,0,16,0,
0,0,0,0,55,0,
0,9,66,0,16,0,
0,0,0,0,10,0,
16,0,1,0,0,0,
58,0,16,0,0,0,
0,0,42,0,16,0,
0,0,0,0,52,0,
0,7,130,0,16,0,
0,0,0,0,42,0,
16,0,7,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
66,0,16,0,0,0,
0,0,58,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,52,0,0,7,
130,0,16,0,0,0,
0,0,58,0,16,0,
7,0,0,0,42,0,
16,0,0,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
26,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,42,0,
16,0,0,0,0,0,
52,0,0,7,66,0,
16,0,0,0,0,0,
42,0,16,0,1,0,
0,0,42,0,16,0,
0,0,0,0,0,0,
0,7,66,0,16,0,
6,0,0,0,42,0,
16,0,0,0,0,0,
58,0,16,0,6,0,
0,0,21,0,0,1,
54,0,0,5,18,0,
16,0,1,0,0,0,
10,0,16,0,3,0,
0,0,54,0,0,5,
34,0,16,0,1,0,
0,0,10,0,16,0,
4,0,0,0,15,0,
0,7,18,32,16,0,
2,0,0,0,70,0,
16,0,1,0,0,0,
134,0,16,0,6,0,
0,0,54,0,0,5,
82,0,16,0,3,0,
0,0,6,2,16,0,
5,0,0,0,16,0,
0,7,34,32,16,0,
2,0,0,0,70,2,
16,0,3,0,0,0,
70,2,16,0,6,0,
0,0,54,0,0,5,
18,0,16,0,4,0,
0,0,26,0,16,0,
5,0,0,0,16,0,
0,7,66,32,16,0,
2,0,0,0,70,2,
16,0,4,0,0,0,
70,2,16,0,6,0,
0,0,54,0,0,5,
50,32,16,0,0,0,
0,0,70,16,16,0,
1,0,0,0,54,0,
0,5,50,32,16,0,
3,0,0,0,70,0,
16,0,0,0,0,0,
54,0,0,6,194,32,
16,0,3,0,0,0,
166,142,32,0,0,0,
0,0,13,0,0,0,
54,0,0,5,34,32,
16,0,4,0,0,0,
1,64,0,0,0,0,
0,0,54,0,0,5,
82,32,16,0,4,0,
0,0,6,1,16,0,
1,0,0,0,54,0,
0,5,114,32,16,0,
5,0,0,0,70,2,
16,0,3,0,0,0,
54,0,0,5,114,32,
16,0,6,0,0,0,
70,2,16,0,4,0,
0,0,62,0,0,1,
83,84,65,84,148,0,
0,0,202,1,0,0,
26,0,0,0,0,0,
0,0,11,0,0,0,
99,1,0,0,0,0,
0,0,1,0,0,0,
2,0,0,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
34,0,0,0,54,0,
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


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,63,101,
41,33,253,174,159,200,
172,224,6,176,240,222,
158,112,1,0,0,0,
176,61,0,0,5,0,
0,0,52,0,0,0,
200,15,0,0,164,16,
0,0,216,16,0,0,
20,61,0,0,82,68,
69,70,140,15,0,0,
2,0,0,0,220,0,
0,0,4,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
99,15,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
188,0,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
1,0,0,0,1,0,
0,0,204,0,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,2,0,
0,0,1,0,0,0,
13,0,0,0,211,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,1,0,0,0,
215,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
1,0,0,0,0,0,
0,0,95,83,111,117,
114,99,101,95,115,97,
109,112,108,101,114,0,
83,111,117,114,99,101,
0,85,66,79,0,80,
117,115,104,0,211,0,
0,0,47,0,0,0,
12,1,0,0,0,1,
0,0,0,0,0,0,
0,0,0,0,215,0,
0,0,7,0,0,0,
92,13,0,0,112,0,
0,0,0,0,0,0,
0,0,0,0,100,8,
0,0,0,0,0,0,
64,0,0,0,0,0,
0,0,120,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,156,8,0,0,
64,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
216,8,0,0,68,0,
0,0,4,0,0,0,
2,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,233,8,
0,0,72,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,0,9,0,0,
76,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
23,9,0,0,80,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,47,9,
0,0,84,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,81,9,0,0,
88,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
101,9,0,0,92,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,123,9,
0,0,96,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,145,9,0,0,
100,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
168,9,0,0,104,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,190,9,
0,0,108,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,212,9,0,0,
112,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
236,9,0,0,116,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,5,10,
0,0,120,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,29,10,0,0,
124,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
65,10,0,0,128,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,95,10,
0,0,132,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,125,10,0,0,
136,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
155,10,0,0,140,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,185,10,
0,0,144,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,215,10,0,0,
148,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
245,10,0,0,152,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,6,11,
0,0,156,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,38,11,0,0,
160,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
69,11,0,0,164,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,100,11,
0,0,168,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,131,11,0,0,
172,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
169,11,0,0,176,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,207,11,
0,0,180,0,0,0,
4,0,0,0,2,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,225,11,0,0,
184,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
247,11,0,0,188,0,
0,0,4,0,0,0,
2,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,16,12,
0,0,192,0,0,0,
4,0,0,0,2,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,35,12,0,0,
196,0,0,0,4,0,
0,0,2,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
57,12,0,0,200,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,82,12,
0,0,204,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,107,12,0,0,
208,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
134,12,0,0,212,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,161,12,
0,0,216,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,184,12,0,0,
220,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
207,12,0,0,224,0,
0,0,4,0,0,0,
2,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,226,12,
0,0,228,0,0,0,
4,0,0,0,2,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,249,12,0,0,
232,0,0,0,4,0,
0,0,2,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
16,13,0,0,236,0,
0,0,4,0,0,0,
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,37,13,
0,0,240,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,60,13,0,0,
244,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
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
0,0,0,0,111,8,
0,0,103,108,111,98,
97,108,95,99,114,116,
95,103,97,109,109,97,
0,102,108,111,97,116,
0,171,0,0,3,0,
1,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,173,8,
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
116,14,0,0,0,0,
0,0,16,0,0,0,
2,0,0,0,144,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,180,14,
0,0,16,0,0,0,
16,0,0,0,0,0,
0,0,144,14,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,200,14,0,0,
32,0,0,0,16,0,
0,0,0,0,0,0,
144,14,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
218,14,0,0,48,0,
0,0,4,0,0,0,
0,0,0,0,244,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,24,15,
0,0,64,0,0,0,
16,0,0,0,0,0,
0,0,144,14,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,52,15,0,0,
80,0,0,0,16,0,
0,0,0,0,0,0,
144,14,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
77,15,0,0,96,0,
0,0,16,0,0,0,
0,0,0,0,144,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,112,97,
114,97,109,115,95,83,
111,117,114,99,101,83,
105,122,101,0,102,108,
111,97,116,52,0,171,
171,171,1,0,3,0,
1,0,4,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,134,14,
0,0,112,97,114,97,
109,115,95,79,114,105,
103,105,110,97,108,83,
105,122,101,0,112,97,
114,97,109,115,95,79,
117,116,112,117,116,83,
105,122,101,0,112,97,
114,97,109,115,95,70,
114,97,109,101,67,111,
117,110,116,0,100,119,
111,114,100,0,171,171,
0,0,19,0,1,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,236,14,0,0,
112,97,114,97,109,115,
95,77,65,83,75,69,
68,95,83,67,65,78,
76,73,78,69,83,83,
105,122,101,0,112,97,
114,97,109,115,95,72,
65,76,65,84,73,79,
78,95,66,76,85,82,
83,105,122,101,0,112,
97,114,97,109,115,95,
66,82,73,71,72,84,
80,65,83,83,83,105,
122,101,0,77,105,99,
114,111,115,111,102,116,
32,40,82,41,32,72,
76,83,76,32,83,104,
97,100,101,114,32,67,
111,109,112,105,108,101,
114,32,49,48,46,49,
0,171,73,83,71,78,
212,0,0,0,8,0,
0,0,8,0,0,0,
200,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,3,0,0,
200,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,12,0,0,
200,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,15,15,0,0,
200,0,0,0,3,0,
0,0,0,0,0,0,
3,0,0,0,2,0,
0,0,7,7,0,0,
200,0,0,0,4,0,
0,0,0,0,0,0,
3,0,0,0,3,0,
0,0,15,15,0,0,
200,0,0,0,5,0,
0,0,0,0,0,0,
3,0,0,0,4,0,
0,0,7,7,0,0,
200,0,0,0,6,0,
0,0,0,0,0,0,
3,0,0,0,5,0,
0,0,7,7,0,0,
200,0,0,0,7,0,
0,0,0,0,0,0,
3,0,0,0,6,0,
0,0,7,7,0,0,
84,69,88,67,79,79,
82,68,0,171,171,171,
79,83,71,78,44,0,
0,0,1,0,0,0,
8,0,0,0,32,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
15,0,0,0,83,86,
95,84,97,114,103,101,
116,0,171,171,83,72,
69,88,52,44,0,0,
80,0,0,0,13,11,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,0,0,0,0,
15,0,0,0,89,0,
0,4,70,142,32,0,
1,0,0,0,1,0,
0,0,90,0,0,3,
0,96,16,0,2,0,
0,0,88,24,0,4,
0,112,16,0,2,0,
0,0,85,85,0,0,
98,16,0,3,50,16,
16,0,0,0,0,0,
98,16,0,3,194,16,
16,0,0,0,0,0,
98,16,0,3,242,16,
16,0,1,0,0,0,
98,16,0,3,114,16,
16,0,2,0,0,0,
98,16,0,3,242,16,
16,0,3,0,0,0,
98,16,0,3,114,16,
16,0,4,0,0,0,
98,16,0,3,114,16,
16,0,5,0,0,0,
98,16,0,3,114,16,
16,0,6,0,0,0,
101,0,0,3,242,32,
16,0,0,0,0,0,
104,0,0,2,20,0,
0,0,56,0,0,8,
242,0,16,0,0,0,
0,0,70,30,16,0,
1,0,0,0,70,132,
32,0,1,0,0,0,
0,0,0,0,56,0,
0,7,50,0,16,0,
1,0,0,0,70,0,
16,0,0,0,0,0,
70,16,16,0,0,0,
0,0,49,0,0,8,
66,0,16,0,1,0,
0,0,1,64,0,0,
0,0,0,63,58,128,
32,0,0,0,0,0,
11,0,0,0,31,0,
4,3,42,0,16,0,
1,0,0,0,50,0,
0,12,50,0,16,0,
0,0,0,0,70,16,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,191,0,0,0,0,
0,0,0,0,56,0,
0,7,50,0,16,0,
2,0,0,0,70,0,
16,0,0,0,0,0,
70,16,16,0,3,0,
0,0,54,0,0,6,
66,0,16,0,2,0,
0,0,26,0,16,128,
65,0,0,0,2,0,
0,0,54,0,0,7,
130,0,16,0,2,0,
0,0,26,128,32,128,
65,0,0,0,0,0,
0,0,12,0,0,0,
16,0,0,7,18,0,
16,0,3,0,0,0,
70,18,16,0,4,0,
0,0,134,3,16,0,
2,0,0,0,16,0,
0,7,34,0,16,0,
3,0,0,0,70,18,
16,0,5,0,0,0,
134,3,16,0,2,0,
0,0,16,0,0,7,
66,0,16,0,3,0,
0,0,70,18,16,0,
6,0,0,0,134,3,
16,0,2,0,0,0,
49,0,0,11,50,0,
16,0,0,0,0,0,
246,143,32,0,0,0,
0,0,11,0,0,0,
2,64,0,0,0,0,
32,64,0,0,192,63,
0,0,0,0,0,0,
0,0,16,0,0,7,
66,0,16,0,1,0,
0,0,70,2,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
16,0,0,7,130,0,
16,0,1,0,0,0,
70,2,16,0,3,0,
0,0,70,18,16,0,
2,0,0,0,16,0,
0,7,34,0,16,0,
2,0,0,0,70,18,
16,0,2,0,0,0,
70,18,16,0,2,0,
0,0,50,0,0,12,
34,0,16,0,2,0,
0,0,10,128,32,128,
65,0,0,0,0,0,
0,0,12,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
26,0,16,0,2,0,
0,0,56,0,0,7,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,26,0,
16,0,2,0,0,0,
50,0,0,10,34,0,
16,0,4,0,0,0,
58,0,16,0,1,0,
0,0,58,0,16,0,
1,0,0,0,42,0,
16,128,65,0,0,0,
1,0,0,0,75,0,
0,5,66,0,16,0,
1,0,0,0,26,0,
16,0,4,0,0,0,
0,0,0,8,66,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,58,0,16,128,
65,0,0,0,1,0,
0,0,14,0,0,7,
18,0,16,0,4,0,
0,0,26,0,16,0,
2,0,0,0,42,0,
16,0,1,0,0,0,
54,0,0,5,18,0,
16,0,5,0,0,0,
1,64,0,0,0,0,
0,0,54,0,0,6,
34,0,16,0,5,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,0,0,0,8,
242,0,16,0,6,0,
0,0,70,1,16,128,
65,0,0,0,5,0,
0,0,102,17,16,0,
2,0,0,0,56,0,
0,10,194,0,16,0,
1,0,0,0,86,1,
16,0,3,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
128,63,50,0,0,13,
194,0,16,0,1,0,
0,0,166,6,16,0,
3,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
128,63,0,0,0,0,
166,14,16,128,65,0,
0,0,1,0,0,0,
56,0,0,10,194,0,
16,0,4,0,0,0,
166,14,16,0,6,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,128,63,50,0,
0,13,194,0,16,0,
4,0,0,0,6,4,
16,0,6,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,128,63,0,0,
0,0,166,14,16,128,
65,0,0,0,4,0,
0,0,15,0,0,7,
34,0,16,0,2,0,
0,0,230,10,16,0,
1,0,0,0,230,10,
16,0,1,0,0,0,
15,0,0,7,66,0,
16,0,1,0,0,0,
230,10,16,0,4,0,
0,0,230,10,16,0,
1,0,0,0,15,0,
0,7,130,0,16,0,
1,0,0,0,230,10,
16,0,4,0,0,0,
230,10,16,0,4,0,
0,0,50,0,0,12,
130,0,16,0,1,0,
0,0,10,128,32,128,
65,0,0,0,0,0,
0,0,12,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
58,0,16,0,1,0,
0,0,56,0,0,7,
34,0,16,0,2,0,
0,0,58,0,16,0,
1,0,0,0,26,0,
16,0,2,0,0,0,
50,0,0,10,34,0,
16,0,6,0,0,0,
42,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,26,0,
16,128,65,0,0,0,
2,0,0,0,75,0,
0,5,34,0,16,0,
2,0,0,0,26,0,
16,0,6,0,0,0,
0,0,0,8,66,0,
16,0,1,0,0,0,
42,0,16,128,65,0,
0,0,1,0,0,0,
26,0,16,0,2,0,
0,0,14,0,0,7,
18,0,16,0,6,0,
0,0,58,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
55,0,0,9,194,0,
16,0,1,0,0,0,
6,0,16,0,0,0,
0,0,6,4,16,0,
4,0,0,0,6,4,
16,0,6,0,0,0,
56,0,0,7,114,0,
16,0,4,0,0,0,
166,10,16,0,1,0,
0,0,70,2,16,0,
3,0,0,0,50,0,
0,9,114,0,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
166,10,16,0,1,0,
0,0,70,18,16,0,
2,0,0,0,49,0,
0,7,66,0,16,0,
1,0,0,0,1,64,
0,0,10,215,163,59,
58,0,16,0,1,0,
0,0,31,0,4,3,
42,0,16,0,1,0,
0,0,49,0,0,8,
66,0,16,0,1,0,
0,0,58,128,32,0,
0,0,0,0,11,0,
0,0,1,64,0,0,
0,0,192,63,31,0,
4,3,42,0,16,0,
1,0,0,0,56,0,
0,7,194,0,16,0,
1,0,0,0,6,4,
16,0,5,0,0,0,
166,2,16,0,3,0,
0,0,50,0,0,10,
194,0,16,0,1,0,
0,0,86,9,16,0,
3,0,0,0,86,1,
16,0,5,0,0,0,
166,14,16,128,65,0,
0,0,1,0,0,0,
15,0,0,7,66,0,
16,0,1,0,0,0,
230,10,16,0,1,0,
0,0,230,10,16,0,
1,0,0,0,75,0,
0,5,66,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
56,0,0,8,130,0,
16,0,1,0,0,0,
42,0,16,0,3,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,51,0,0,8,
34,0,16,0,2,0,
0,0,58,0,16,128,
129,0,0,0,1,0,
0,0,42,0,16,0,
1,0,0,0,52,0,
0,8,130,0,16,0,
4,0,0,0,58,0,
16,128,129,0,0,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
14,0,0,10,130,0,
16,0,4,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,58,0,16,0,
4,0,0,0,56,0,
0,7,34,0,16,0,
2,0,0,0,26,0,
16,0,2,0,0,0,
58,0,16,0,4,0,
0,0,56,0,0,7,
130,0,16,0,4,0,
0,0,26,0,16,0,
2,0,0,0,26,0,
16,0,2,0,0,0,
50,0,0,9,18,0,
16,0,5,0,0,0,
58,0,16,0,4,0,
0,0,1,64,0,0,
95,174,170,60,1,64,
0,0,54,90,174,189,
50,0,0,9,18,0,
16,0,5,0,0,0,
58,0,16,0,4,0,
0,0,10,0,16,0,
5,0,0,0,1,64,
0,0,226,118,56,62,
50,0,0,9,18,0,
16,0,5,0,0,0,
58,0,16,0,4,0,
0,0,10,0,16,0,
5,0,0,0,1,64,
0,0,4,29,169,190,
50,0,0,9,130,0,
16,0,4,0,0,0,
58,0,16,0,4,0,
0,0,10,0,16,0,
5,0,0,0,1,64,
0,0,56,247,127,63,
56,0,0,7,18,0,
16,0,5,0,0,0,
26,0,16,0,2,0,
0,0,58,0,16,0,
4,0,0,0,49,0,
0,8,34,0,16,0,
5,0,0,0,58,0,
16,128,129,0,0,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
50,0,0,9,18,0,
16,0,5,0,0,0,
10,0,16,0,5,0,
0,0,1,64,0,0,
0,0,0,192,1,64,
0,0,219,15,201,63,
1,0,0,7,18,0,
16,0,5,0,0,0,
26,0,16,0,5,0,
0,0,10,0,16,0,
5,0,0,0,50,0,
0,9,34,0,16,0,
2,0,0,0,26,0,
16,0,2,0,0,0,
58,0,16,0,4,0,
0,0,10,0,16,0,
5,0,0,0,49,0,
0,8,130,0,16,0,
4,0,0,0,58,0,
16,0,1,0,0,0,
58,0,16,128,65,0,
0,0,1,0,0,0,
1,0,0,7,130,0,
16,0,4,0,0,0,
58,0,16,0,4,0,
0,0,1,64,0,0,
219,15,73,192,0,0,
0,7,34,0,16,0,
2,0,0,0,26,0,
16,0,2,0,0,0,
58,0,16,0,4,0,
0,0,51,0,0,7,
130,0,16,0,4,0,
0,0,58,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
52,0,0,7,66,0,
16,0,1,0,0,0,
58,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,49,0,
0,8,130,0,16,0,
1,0,0,0,58,0,
16,0,4,0,0,0,
58,0,16,128,65,0,
0,0,4,0,0,0,
29,0,0,8,66,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,42,0,16,128,
65,0,0,0,1,0,
0,0,1,0,0,7,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,58,0,
16,0,1,0,0,0,
55,0,0,10,66,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,26,0,16,128,
65,0,0,0,2,0,
0,0,26,0,16,0,
2,0,0,0,56,0,
0,8,66,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
10,128,32,0,0,0,
0,0,12,0,0,0,
54,0,0,6,130,0,
16,0,3,0,0,0,
26,0,16,128,65,0,
0,0,3,0,0,0,
15,0,0,7,130,0,
16,0,1,0,0,0,
198,0,16,0,3,0,
0,0,198,0,16,0,
3,0,0,0,68,0,
0,5,130,0,16,0,
1,0,0,0,58,0,
16,0,1,0,0,0,
56,0,0,7,50,0,
16,0,5,0,0,0,
246,15,16,0,1,0,
0,0,198,0,16,0,
3,0,0,0,56,0,
0,7,194,0,16,0,
1,0,0,0,166,10,
16,0,1,0,0,0,
6,4,16,0,5,0,
0,0,14,0,0,7,
194,0,16,0,1,0,
0,0,166,14,16,0,
1,0,0,0,6,20,
16,0,3,0,0,0,
18,0,0,1,31,0,
4,3,10,0,16,0,
0,0,0,0,56,0,
0,10,50,0,16,0,
5,0,0,0,70,0,
16,0,3,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,191,
0,0,0,0,0,0,
0,0,51,0,0,9,
194,0,16,0,5,0,
0,0,166,10,16,128,
129,0,0,0,3,0,
0,0,6,4,16,128,
129,0,0,0,5,0,
0,0,52,0,0,9,
50,0,16,0,6,0,
0,0,166,10,16,128,
129,0,0,0,3,0,
0,0,70,0,16,128,
129,0,0,0,5,0,
0,0,14,0,0,10,
50,0,16,0,6,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,70,0,
16,0,6,0,0,0,
56,0,0,7,194,0,
16,0,5,0,0,0,
166,14,16,0,5,0,
0,0,6,4,16,0,
6,0,0,0,56,0,
0,7,50,0,16,0,
6,0,0,0,230,10,
16,0,5,0,0,0,
230,10,16,0,5,0,
0,0,50,0,0,15,
194,0,16,0,6,0,
0,0,6,4,16,0,
6,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,95,174,
170,60,95,174,170,60,
2,64,0,0,0,0,
0,0,0,0,0,0,
54,90,174,189,54,90,
174,189,50,0,0,12,
194,0,16,0,6,0,
0,0,6,4,16,0,
6,0,0,0,166,14,
16,0,6,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
226,118,56,62,226,118,
56,62,50,0,0,12,
194,0,16,0,6,0,
0,0,6,4,16,0,
6,0,0,0,166,14,
16,0,6,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
4,29,169,190,4,29,
169,190,50,0,0,12,
50,0,16,0,6,0,
0,0,70,0,16,0,
6,0,0,0,230,10,
16,0,6,0,0,0,
2,64,0,0,56,247,
127,63,56,247,127,63,
0,0,0,0,0,0,
0,0,56,0,0,7,
194,0,16,0,6,0,
0,0,166,14,16,0,
5,0,0,0,6,4,
16,0,6,0,0,0,
49,0,0,9,50,0,
16,0,7,0,0,0,
166,10,16,128,129,0,
0,0,3,0,0,0,
70,0,16,128,129,0,
0,0,5,0,0,0,
50,0,0,15,194,0,
16,0,6,0,0,0,
166,14,16,0,6,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,192,
0,0,0,192,2,64,
0,0,0,0,0,0,
0,0,0,0,219,15,
201,63,219,15,201,63,
1,0,0,7,194,0,
16,0,6,0,0,0,
6,4,16,0,7,0,
0,0,166,14,16,0,
6,0,0,0,50,0,
0,9,194,0,16,0,
5,0,0,0,166,14,
16,0,5,0,0,0,
6,4,16,0,6,0,
0,0,166,14,16,0,
6,0,0,0,49,0,
0,8,34,0,16,0,
2,0,0,0,42,0,
16,0,3,0,0,0,
42,0,16,128,65,0,
0,0,3,0,0,0,
1,0,0,7,34,0,
16,0,2,0,0,0,
26,0,16,0,2,0,
0,0,1,64,0,0,
219,15,73,192,0,0,
0,7,194,0,16,0,
5,0,0,0,86,5,
16,0,2,0,0,0,
166,14,16,0,5,0,
0,0,51,0,0,7,
50,0,16,0,6,0,
0,0,166,10,16,0,
3,0,0,0,70,0,
16,0,5,0,0,0,
52,0,0,7,50,0,
16,0,5,0,0,0,
166,10,16,0,3,0,
0,0,70,0,16,0,
5,0,0,0,49,0,
0,8,50,0,16,0,
6,0,0,0,70,0,
16,0,6,0,0,0,
70,0,16,128,65,0,
0,0,6,0,0,0,
29,0,0,8,50,0,
16,0,5,0,0,0,
70,0,16,0,5,0,
0,0,70,0,16,128,
65,0,0,0,5,0,
0,0,1,0,0,7,
50,0,16,0,5,0,
0,0,70,0,16,0,
5,0,0,0,70,0,
16,0,6,0,0,0,
55,0,0,10,50,0,
16,0,5,0,0,0,
70,0,16,0,5,0,
0,0,230,10,16,128,
65,0,0,0,5,0,
0,0,230,10,16,0,
5,0,0,0,56,0,
0,8,50,0,16,0,
5,0,0,0,70,0,
16,0,5,0,0,0,
6,128,32,0,0,0,
0,0,12,0,0,0,
14,0,0,7,194,0,
16,0,1,0,0,0,
6,4,16,0,5,0,
0,0,6,20,16,0,
3,0,0,0,18,0,
0,1,54,0,0,6,
98,0,16,0,5,0,
0,0,166,9,16,128,
65,0,0,0,3,0,
0,0,51,0,0,9,
34,0,16,0,2,0,
0,0,42,0,16,128,
129,0,0,0,3,0,
0,0,10,0,16,128,
129,0,0,0,3,0,
0,0,52,0,0,9,
130,0,16,0,3,0,
0,0,42,0,16,128,
129,0,0,0,3,0,
0,0,10,0,16,128,
129,0,0,0,3,0,
0,0,14,0,0,10,
130,0,16,0,3,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,58,0,
16,0,3,0,0,0,
56,0,0,7,34,0,
16,0,2,0,0,0,
26,0,16,0,2,0,
0,0,58,0,16,0,
3,0,0,0,56,0,
0,7,130,0,16,0,
3,0,0,0,26,0,
16,0,2,0,0,0,
26,0,16,0,2,0,
0,0,50,0,0,9,
130,0,16,0,4,0,
0,0,58,0,16,0,
3,0,0,0,1,64,
0,0,95,174,170,60,
1,64,0,0,54,90,
174,189,50,0,0,9,
130,0,16,0,4,0,
0,0,58,0,16,0,
3,0,0,0,58,0,
16,0,4,0,0,0,
1,64,0,0,226,118,
56,62,50,0,0,9,
130,0,16,0,4,0,
0,0,58,0,16,0,
3,0,0,0,58,0,
16,0,4,0,0,0,
1,64,0,0,4,29,
169,190,50,0,0,9,
130,0,16,0,3,0,
0,0,58,0,16,0,
3,0,0,0,58,0,
16,0,4,0,0,0,
1,64,0,0,56,247,
127,63,56,0,0,7,
130,0,16,0,4,0,
0,0,26,0,16,0,
2,0,0,0,58,0,
16,0,3,0,0,0,
49,0,0,9,130,0,
16,0,5,0,0,0,
42,0,16,128,129,0,
0,0,3,0,0,0,
10,0,16,128,129,0,
0,0,3,0,0,0,
50,0,0,9,130,0,
16,0,4,0,0,0,
58,0,16,0,4,0,
0,0,1,64,0,0,
0,0,0,192,1,64,
0,0,219,15,201,63,
1,0,0,7,130,0,
16,0,4,0,0,0,
58,0,16,0,5,0,
0,0,58,0,16,0,
4,0,0,0,50,0,
0,9,34,0,16,0,
2,0,0,0,26,0,
16,0,2,0,0,0,
58,0,16,0,3,0,
0,0,58,0,16,0,
4,0,0,0,49,0,
0,7,130,0,16,0,
3,0,0,0,42,0,
16,0,3,0,0,0,
26,0,16,0,5,0,
0,0,1,0,0,7,
130,0,16,0,3,0,
0,0,58,0,16,0,
3,0,0,0,1,64,
0,0,219,15,73,192,
0,0,0,7,34,0,
16,0,2,0,0,0,
26,0,16,0,2,0,
0,0,58,0,16,0,
3,0,0,0,51,0,
0,7,130,0,16,0,
3,0,0,0,42,0,
16,0,3,0,0,0,
10,0,16,0,3,0,
0,0,52,0,0,7,
130,0,16,0,4,0,
0,0,42,0,16,0,
3,0,0,0,10,0,
16,0,3,0,0,0,
49,0,0,8,130,0,
16,0,3,0,0,0,
58,0,16,0,3,0,
0,0,58,0,16,128,
65,0,0,0,3,0,
0,0,29,0,0,8,
130,0,16,0,4,0,
0,0,58,0,16,0,
4,0,0,0,58,0,
16,128,65,0,0,0,
4,0,0,0,1,0,
0,7,130,0,16,0,
3,0,0,0,58,0,
16,0,3,0,0,0,
58,0,16,0,4,0,
0,0,55,0,0,10,
34,0,16,0,2,0,
0,0,58,0,16,0,
3,0,0,0,26,0,
16,128,65,0,0,0,
2,0,0,0,26,0,
16,0,2,0,0,0,
56,0,0,8,18,0,
16,0,5,0,0,0,
26,0,16,0,2,0,
0,0,10,128,32,0,
0,0,0,0,12,0,
0,0,14,0,0,7,
194,0,16,0,1,0,
0,0,6,8,16,0,
5,0,0,0,6,20,
16,0,3,0,0,0,
21,0,0,1,21,0,
0,1,18,0,0,1,
54,0,0,8,194,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,128,63,0,0,
128,63,21,0,0,1,
0,0,0,10,50,0,
16,0,1,0,0,0,
230,10,16,0,1,0,
0,0,2,64,0,0,
0,0,0,63,0,0,
0,63,0,0,0,0,
0,0,0,0,1,0,
0,7,18,0,16,0,
5,0,0,0,10,0,
16,0,0,0,0,0,
26,0,16,0,3,0,
0,0,54,0,0,5,
98,0,16,0,5,0,
0,0,166,8,16,0,
3,0,0,0,16,0,
0,7,66,0,16,0,
1,0,0,0,70,2,
16,0,5,0,0,0,
70,2,16,0,5,0,
0,0,68,0,0,5,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,56,0,
0,7,114,0,16,0,
5,0,0,0,166,10,
16,0,1,0,0,0,
70,2,16,0,5,0,
0,0,54,0,0,5,
18,0,16,0,6,0,
0,0,42,16,16,0,
0,0,0,0,54,0,
0,8,226,0,16,0,
6,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,7,114,0,
16,0,6,0,0,0,
134,3,16,0,2,0,
0,0,70,2,16,0,
6,0,0,0,54,0,
0,8,210,0,16,0,
7,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
54,0,0,6,34,0,
16,0,7,0,0,0,
58,16,16,128,65,0,
0,0,0,0,0,0,
0,0,0,7,114,0,
16,0,2,0,0,0,
134,3,16,0,2,0,
0,0,70,2,16,0,
7,0,0,0,16,0,
0,7,18,0,16,0,
7,0,0,0,70,18,
16,0,4,0,0,0,
70,2,16,0,6,0,
0,0,16,0,0,7,
34,0,16,0,7,0,
0,0,70,18,16,0,
5,0,0,0,70,2,
16,0,6,0,0,0,
16,0,0,7,66,0,
16,0,7,0,0,0,
70,18,16,0,6,0,
0,0,70,2,16,0,
6,0,0,0,16,0,
0,7,18,0,16,0,
6,0,0,0,70,18,
16,0,4,0,0,0,
70,2,16,0,2,0,
0,0,16,0,0,7,
34,0,16,0,6,0,
0,0,70,18,16,0,
5,0,0,0,70,2,
16,0,2,0,0,0,
16,0,0,7,66,0,
16,0,6,0,0,0,
70,18,16,0,6,0,
0,0,70,2,16,0,
2,0,0,0,16,0,
0,7,66,0,16,0,
1,0,0,0,150,4,
16,0,4,0,0,0,
70,2,16,0,5,0,
0,0,16,0,0,7,
130,0,16,0,1,0,
0,0,150,4,16,0,
7,0,0,0,70,2,
16,0,5,0,0,0,
14,0,0,7,130,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,58,0,16,0,
1,0,0,0,50,0,
0,9,114,0,16,0,
2,0,0,0,246,15,
16,0,1,0,0,0,
70,2,16,0,7,0,
0,0,70,18,16,0,
2,0,0,0,16,0,
0,7,130,0,16,0,
1,0,0,0,150,4,
16,0,6,0,0,0,
70,2,16,0,5,0,
0,0,14,0,0,7,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,58,0,
16,0,1,0,0,0,
50,0,0,9,114,0,
16,0,4,0,0,0,
166,10,16,0,1,0,
0,0,70,2,16,0,
6,0,0,0,70,18,
16,0,2,0,0,0,
0,0,0,8,114,0,
16,0,2,0,0,0,
70,2,16,128,65,0,
0,0,3,0,0,0,
70,2,16,0,2,0,
0,0,0,0,0,8,
114,0,16,0,4,0,
0,0,70,2,16,128,
65,0,0,0,3,0,
0,0,70,2,16,0,
4,0,0,0,56,0,
0,10,114,0,16,0,
6,0,0,0,38,9,
16,0,3,0,0,0,
2,64,0,0,0,0,
128,63,0,0,0,0,
0,0,0,0,0,0,
0,0,56,0,0,10,
114,0,16,0,7,0,
0,0,150,4,16,0,
3,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
128,63,0,0,0,0,
50,0,0,13,114,0,
16,0,8,0,0,0,
38,9,16,0,3,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
0,0,0,0,0,0,
0,0,0,0,70,2,
16,128,65,0,0,0,
7,0,0,0,15,0,
0,7,66,0,16,0,
1,0,0,0,134,0,
16,0,8,0,0,0,
134,0,16,0,8,0,
0,0,68,0,0,5,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,56,0,
0,7,114,0,16,0,
8,0,0,0,166,10,
16,0,1,0,0,0,
70,2,16,0,8,0,
0,0,56,0,0,7,
114,0,16,0,8,0,
0,0,70,2,16,0,
8,0,0,0,86,21,
16,0,3,0,0,0,
56,0,0,10,82,0,
16,0,9,0,0,0,
166,9,16,0,3,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,128,63,
0,0,0,0,50,0,
0,13,114,0,16,0,
3,0,0,0,38,9,
16,0,3,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,128,63,0,0,
0,0,38,9,16,128,
65,0,0,0,6,0,
0,0,15,0,0,7,
66,0,16,0,1,0,
0,0,150,5,16,0,
3,0,0,0,150,5,
16,0,3,0,0,0,
68,0,0,5,66,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,56,0,0,7,
114,0,16,0,3,0,
0,0,166,10,16,0,
1,0,0,0,70,2,
16,0,3,0,0,0,
56,0,0,7,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,6,16,16,0,
3,0,0,0,0,0,
0,8,114,0,16,0,
7,0,0,0,54,13,
16,0,6,0,0,0,
230,9,16,128,65,0,
0,0,7,0,0,0,
15,0,0,7,66,0,
16,0,1,0,0,0,
70,0,16,0,7,0,
0,0,70,0,16,0,
7,0,0,0,68,0,
0,5,66,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
56,0,0,7,114,0,
16,0,7,0,0,0,
166,10,16,0,1,0,
0,0,70,2,16,0,
7,0,0,0,56,0,
0,7,114,0,16,0,
7,0,0,0,70,2,
16,0,7,0,0,0,
6,16,16,0,3,0,
0,0,54,0,0,5,
130,0,16,0,9,0,
0,0,1,64,0,0,
0,0,0,0,0,0,
0,8,114,0,16,0,
6,0,0,0,198,2,
16,128,65,0,0,0,
6,0,0,0,182,12,
16,0,9,0,0,0,
15,0,0,7,66,0,
16,0,1,0,0,0,
70,0,16,0,6,0,
0,0,70,0,16,0,
6,0,0,0,68,0,
0,5,66,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
56,0,0,7,114,0,
16,0,6,0,0,0,
166,10,16,0,1,0,
0,0,70,2,16,0,
6,0,0,0,56,0,
0,7,114,0,16,0,
6,0,0,0,70,2,
16,0,6,0,0,0,
86,21,16,0,3,0,
0,0,56,0,0,7,
114,0,16,0,9,0,
0,0,150,4,16,0,
5,0,0,0,70,2,
16,0,6,0,0,0,
50,0,0,10,114,0,
16,0,6,0,0,0,
70,2,16,0,5,0,
0,0,150,4,16,0,
6,0,0,0,70,2,
16,128,65,0,0,0,
9,0,0,0,56,0,
0,7,114,0,16,0,
9,0,0,0,70,2,
16,0,5,0,0,0,
70,2,16,0,7,0,
0,0,50,0,0,10,
114,0,16,0,7,0,
0,0,38,9,16,0,
7,0,0,0,150,4,
16,0,5,0,0,0,
70,2,16,128,65,0,
0,0,9,0,0,0,
56,0,0,10,114,0,
16,0,9,0,0,0,
70,2,16,0,5,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,128,63,
0,0,0,0,50,0,
0,13,114,0,16,0,
5,0,0,0,150,4,
16,0,5,0,0,0,
2,64,0,0,0,0,
128,63,0,0,0,0,
0,0,0,0,0,0,
0,0,70,2,16,128,
65,0,0,0,9,0,
0,0,56,0,0,7,
114,0,16,0,5,0,
0,0,70,2,16,0,
5,0,0,0,86,21,
16,0,3,0,0,0,
55,0,0,9,114,0,
16,0,5,0,0,0,
6,0,16,0,0,0,
0,0,70,2,16,0,
6,0,0,0,70,2,
16,0,5,0,0,0,
54,0,0,8,82,0,
16,0,6,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,54,0,0,6,
34,0,16,0,6,0,
0,0,10,16,16,128,
65,0,0,0,3,0,
0,0,55,0,0,9,
114,0,16,0,6,0,
0,0,6,0,16,0,
0,0,0,0,70,2,
16,0,7,0,0,0,
70,2,16,0,6,0,
0,0,55,0,0,9,
114,0,16,0,5,0,
0,0,86,5,16,0,
0,0,0,0,70,2,
16,0,8,0,0,0,
70,2,16,0,5,0,
0,0,55,0,0,9,
114,0,16,0,3,0,
0,0,86,5,16,0,
0,0,0,0,70,2,
16,0,3,0,0,0,
70,2,16,0,6,0,
0,0,56,0,0,7,
114,0,16,0,6,0,
0,0,150,4,16,0,
5,0,0,0,38,9,
16,0,3,0,0,0,
50,0,0,10,114,0,
16,0,6,0,0,0,
150,4,16,0,3,0,
0,0,38,9,16,0,
5,0,0,0,70,2,
16,128,65,0,0,0,
6,0,0,0,16,0,
0,7,18,0,16,0,
0,0,0,0,70,2,
16,0,6,0,0,0,
70,2,16,0,6,0,
0,0,68,0,0,5,
18,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,56,0,
0,7,114,0,16,0,
5,0,0,0,6,0,
16,0,0,0,0,0,
70,2,16,0,5,0,
0,0,56,0,0,7,
114,0,16,0,3,0,
0,0,6,0,16,0,
0,0,0,0,70,2,
16,0,3,0,0,0,
16,0,0,7,18,0,
16,0,6,0,0,0,
70,2,16,0,5,0,
0,0,70,2,16,0,
2,0,0,0,16,0,
0,7,34,0,16,0,
6,0,0,0,70,2,
16,0,5,0,0,0,
70,2,16,0,4,0,
0,0,16,0,0,7,
66,0,16,0,6,0,
0,0,70,2,16,0,
3,0,0,0,70,2,
16,0,2,0,0,0,
16,0,0,7,130,0,
16,0,6,0,0,0,
70,2,16,0,3,0,
0,0,70,2,16,0,
4,0,0,0,18,0,
0,1,54,0,0,5,
146,0,16,0,2,0,
0,0,166,30,16,0,
0,0,0,0,54,0,
0,5,34,0,16,0,
2,0,0,0,1,64,
0,0,0,0,0,0,
54,0,0,5,242,0,
16,0,6,0,0,0,
70,13,16,0,2,0,
0,0,21,0,0,1,
0,0,0,10,50,0,
16,0,0,0,0,0,
70,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,191,0,0,0,0,
0,0,0,0,14,0,
0,7,50,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
230,26,16,0,3,0,
0,0,0,0,0,10,
50,0,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,2,64,
0,0,0,0,0,63,
0,0,0,63,0,0,
0,0,0,0,0,0,
56,0,0,7,50,0,
16,0,1,0,0,0,
230,10,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,29,0,
0,8,66,0,16,0,
1,0,0,0,1,64,
0,0,0,0,0,63,
58,128,32,0,0,0,
0,0,11,0,0,0,
57,0,0,10,50,0,
16,0,2,0,0,0,
230,26,16,0,3,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,0,0,
0,0,0,0,60,0,
0,7,130,0,16,0,
1,0,0,0,26,0,
16,0,2,0,0,0,
10,0,16,0,2,0,
0,0,55,0,0,9,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,58,0,
16,0,1,0,0,0,
1,64,0,0,255,255,
255,255,31,0,4,3,
42,0,16,0,1,0,
0,0,14,0,0,7,
242,0,16,0,2,0,
0,0,166,15,16,0,
0,0,0,0,166,31,
16,0,3,0,0,0,
56,0,0,7,242,0,
16,0,2,0,0,0,
70,14,16,0,2,0,
0,0,70,14,16,0,
6,0,0,0,50,0,
0,11,66,0,16,0,
1,0,0,0,26,128,
32,128,65,0,0,0,
0,0,0,0,11,0,
0,0,1,64,0,0,
0,0,0,64,1,64,
0,0,0,0,128,63,
56,0,0,10,50,0,
16,0,3,0,0,0,
166,10,16,0,1,0,
0,0,2,64,0,0,
0,0,64,65,0,0,
192,64,0,0,0,0,
0,0,0,0,50,0,
0,10,130,0,16,0,
1,0,0,0,42,0,
16,128,65,0,0,0,
1,0,0,0,1,64,
0,0,0,0,16,65,
1,64,0,0,0,0,
64,65,56,0,0,11,
194,0,16,0,3,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,64,66,
0,0,192,65,86,133,
32,0,0,0,0,0,
11,0,0,0,50,0,
0,11,130,0,16,0,
1,0,0,0,26,128,
32,128,65,0,0,0,
0,0,0,0,11,0,
0,0,1,64,0,0,
0,0,192,64,58,0,
16,0,1,0,0,0,
50,0,0,13,50,0,
16,0,3,0,0,0,
86,133,32,0,0,0,
0,0,11,0,0,0,
2,64,0,0,0,0,
192,64,0,0,240,65,
0,0,0,0,0,0,
0,0,70,0,16,0,
3,0,0,0,0,0,
0,7,18,0,16,0,
3,0,0,0,10,0,
16,0,3,0,0,0,
1,64,0,0,0,0,
144,193,50,0,0,10,
18,0,16,0,4,0,
0,0,42,0,16,128,
65,0,0,0,1,0,
0,0,1,64,0,0,
0,0,0,64,1,64,
0,0,0,0,192,64,
50,0,0,12,34,0,
16,0,4,0,0,0,
26,128,32,128,65,0,
0,0,0,0,0,0,
11,0,0,0,1,64,
0,0,0,0,192,64,
42,0,16,128,65,0,
0,0,1,0,0,0,
50,0,0,10,66,0,
16,0,3,0,0,0,
42,0,16,0,1,0,
0,0,1,64,0,0,
0,0,64,193,42,0,
16,128,65,0,0,0,
3,0,0,0,50,0,
0,9,66,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
1,64,0,0,0,0,
0,65,58,0,16,0,
3,0,0,0,50,0,
0,12,242,0,16,0,
5,0,0,0,246,15,
16,0,1,0,0,0,
2,64,0,0,54,142,
35,63,140,227,120,63,
118,28,7,63,128,28,
71,62,6,0,16,0,
3,0,0,0,50,0,
0,12,242,0,16,0,
5,0,0,0,70,14,
16,0,5,0,0,0,
2,64,0,0,208,252,
208,62,170,249,113,63,
15,158,142,62,82,221,
26,61,6,0,16,0,
4,0,0,0,56,0,
0,7,130,0,16,0,
3,0,0,0,10,0,
16,0,5,0,0,0,
1,64,0,0,171,170,
42,62,56,0,0,7,
66,0,16,0,4,0,
0,0,10,0,16,0,
4,0,0,0,1,64,
0,0,171,170,42,62,
56,0,0,7,66,0,
16,0,6,0,0,0,
58,0,16,0,3,0,
0,0,42,0,16,0,
4,0,0,0,56,0,
0,7,114,0,16,0,
5,0,0,0,166,10,
16,0,4,0,0,0,
150,7,16,0,5,0,
0,0,50,0,0,12,
242,0,16,0,7,0,
0,0,86,5,16,0,
4,0,0,0,2,64,
0,0,113,28,167,63,
143,227,216,63,228,56,
174,63,57,142,131,63,
86,5,16,0,3,0,
0,0,50,0,0,12,
242,0,16,0,7,0,
0,0,70,14,16,0,
7,0,0,0,2,64,
0,0,113,28,167,63,
143,227,216,63,228,56,
174,63,57,142,131,63,
166,10,16,0,3,0,
0,0,50,0,0,12,
242,0,16,0,7,0,
0,0,70,14,16,0,
7,0,0,0,2,64,
0,0,113,28,167,63,
143,227,216,63,228,56,
174,63,57,142,131,63,
166,10,16,0,1,0,
0,0,56,0,0,7,
242,0,16,0,7,0,
0,0,166,10,16,0,
4,0,0,0,70,14,
16,0,7,0,0,0,
56,0,0,10,242,0,
16,0,8,0,0,0,
70,14,16,0,7,0,
0,0,2,64,0,0,
171,170,42,62,171,170,
42,62,171,170,42,62,
171,170,42,62,56,0,
0,10,114,0,16,0,
5,0,0,0,70,2,
16,0,5,0,0,0,
2,64,0,0,171,170,
42,62,171,170,42,62,
171,170,42,62,0,0,
0,0,50,0,0,12,
242,0,16,0,9,0,
0,0,246,15,16,0,
1,0,0,0,2,64,
0,0,214,56,14,62,
250,255,127,62,84,85,
21,63,170,170,106,63,
6,0,16,0,3,0,
0,0,50,0,0,12,
242,0,16,0,9,0,
0,0,70,14,16,0,
9,0,0,0,2,64,
0,0,52,6,158,60,
244,255,127,61,224,56,
174,62,113,28,87,63,
6,0,16,0,4,0,
0,0,56,0,0,7,
242,0,16,0,9,0,
0,0,166,10,16,0,
4,0,0,0,70,14,
16,0,9,0,0,0,
50,0,0,12,178,0,
16,0,3,0,0,0,
86,5,16,0,4,0,
0,0,2,64,0,0,
85,85,181,63,0,0,
224,63,0,0,0,0,
113,28,231,63,86,5,
16,0,3,0,0,0,
50,0,0,12,114,0,
16,0,3,0,0,0,
70,3,16,0,3,0,
0,0,2,64,0,0,
85,85,181,63,0,0,
224,63,113,28,231,63,
0,0,0,0,166,10,
16,0,3,0,0,0,
50,0,0,12,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,2,64,0,0,
85,85,181,63,0,0,
224,63,113,28,231,63,
0,0,0,0,166,10,
16,0,1,0,0,0,
56,0,0,7,114,0,
16,0,3,0,0,0,
166,10,16,0,4,0,
0,0,70,2,16,0,
3,0,0,0,56,0,
0,10,114,0,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
2,64,0,0,171,170,
42,62,171,170,42,62,
171,170,42,62,0,0,
0,0,56,0,0,10,
242,0,16,0,4,0,
0,0,70,14,16,0,
9,0,0,0,2,64,
0,0,171,170,42,62,
171,170,42,62,171,170,
42,62,171,170,42,62,
54,0,0,5,34,0,
16,0,6,0,0,0,
10,0,16,0,5,0,
0,0,54,0,0,5,
18,0,16,0,6,0,
0,0,10,0,16,0,
8,0,0,0,50,0,
0,12,114,0,16,0,
7,0,0,0,150,7,
16,0,7,0,0,0,
2,64,0,0,171,170,
42,62,171,170,42,62,
171,170,42,62,0,0,
0,0,102,8,16,0,
6,0,0,0,54,0,
0,5,130,0,16,0,
5,0,0,0,10,0,
16,0,4,0,0,0,
0,0,0,7,114,0,
16,0,7,0,0,0,
150,7,16,0,5,0,
0,0,70,2,16,0,
7,0,0,0,54,0,
0,5,130,0,16,0,
3,0,0,0,1,64,
0,0,0,0,0,0,
0,0,0,7,114,0,
16,0,7,0,0,0,
70,3,16,0,3,0,
0,0,70,2,16,0,
7,0,0,0,0,0,
0,7,114,0,16,0,
7,0,0,0,246,14,
16,0,3,0,0,0,
70,2,16,0,7,0,
0,0,50,0,0,12,
114,0,16,0,7,0,
0,0,150,7,16,0,
9,0,0,0,2,64,
0,0,171,170,42,62,
171,170,42,62,171,170,
42,62,0,0,0,0,
70,2,16,0,7,0,
0,0,0,0,0,7,
114,0,16,0,7,0,
0,0,102,10,16,0,
7,0,0,0,70,0,
16,0,7,0,0,0,
14,0,0,10,114,0,
16,0,7,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,70,2,16,0,
7,0,0,0,15,0,
0,10,18,0,16,0,
9,0,0,0,70,0,
16,0,2,0,0,0,
2,64,0,0,140,227,
120,191,171,170,234,190,
0,0,0,0,0,0,
0,0,15,0,0,10,
34,0,16,0,9,0,
0,0,230,10,16,0,
2,0,0,0,2,64,
0,0,140,227,120,191,
171,170,234,190,0,0,
0,0,0,0,0,0,
15,0,0,10,18,0,
16,0,10,0,0,0,
70,0,16,0,2,0,
0,0,2,64,0,0,
228,56,174,63,0,0,
192,190,0,0,0,0,
0,0,0,0,15,0,
0,10,34,0,16,0,
10,0,0,0,230,10,
16,0,2,0,0,0,
2,64,0,0,228,56,
174,63,0,0,192,190,
0,0,0,0,0,0,
0,0,15,0,0,10,
18,0,16,0,11,0,
0,0,70,0,16,0,
2,0,0,0,2,64,
0,0,128,28,71,62,
86,85,149,190,0,0,
0,0,0,0,0,0,
15,0,0,10,34,0,
16,0,11,0,0,0,
230,10,16,0,2,0,
0,0,2,64,0,0,
128,28,71,62,86,85,
149,190,0,0,0,0,
0,0,0,0,15,0,
0,10,18,0,16,0,
12,0,0,0,70,0,
16,0,2,0,0,0,
2,64,0,0,0,0,
224,191,86,85,85,190,
0,0,0,0,0,0,
0,0,15,0,0,10,
34,0,16,0,12,0,
0,0,230,10,16,0,
2,0,0,0,2,64,
0,0,0,0,224,191,
86,85,85,190,0,0,
0,0,0,0,0,0,
15,0,0,10,18,0,
16,0,13,0,0,0,
70,0,16,0,2,0,
0,0,2,64,0,0,
142,227,8,64,0,0,
0,190,0,0,0,0,
0,0,0,0,15,0,
0,10,34,0,16,0,
13,0,0,0,230,10,
16,0,2,0,0,0,
2,64,0,0,142,227,
8,64,0,0,0,190,
0,0,0,0,0,0,
0,0,15,0,0,10,
18,0,16,0,2,0,
0,0,70,0,16,0,
2,0,0,0,2,64,
0,0,84,85,21,191,
168,170,42,189,0,0,
0,0,0,0,0,0,
15,0,0,10,34,0,
16,0,2,0,0,0,
230,10,16,0,2,0,
0,0,2,64,0,0,
84,85,21,191,168,170,
42,189,0,0,0,0,
0,0,0,0,50,0,
0,9,194,0,16,0,
1,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,0,
9,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,14,0,0,0,
230,10,16,0,1,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
50,0,0,9,194,0,
16,0,1,0,0,0,
6,4,16,0,0,0,
0,0,166,14,16,0,
0,0,0,0,6,4,
16,0,10,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,15,0,
0,0,230,10,16,0,
1,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,50,0,0,9,
194,0,16,0,1,0,
0,0,6,4,16,0,
0,0,0,0,166,14,
16,0,0,0,0,0,
6,4,16,0,11,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
16,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,9,194,0,16,0,
1,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,0,
12,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,17,0,0,0,
230,10,16,0,1,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
50,0,0,9,194,0,
16,0,1,0,0,0,
6,4,16,0,0,0,
0,0,166,14,16,0,
0,0,0,0,6,4,
16,0,13,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,18,0,
0,0,230,10,16,0,
1,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,50,0,0,9,
194,0,16,0,1,0,
0,0,6,4,16,0,
0,0,0,0,166,14,
16,0,0,0,0,0,
6,4,16,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
19,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,10,194,0,16,0,
1,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,128,
65,0,0,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
2,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,10,194,0,16,0,
1,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,128,
65,0,0,0,13,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
13,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,10,194,0,16,0,
1,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,128,
65,0,0,0,12,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
12,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,10,194,0,16,0,
1,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,128,
65,0,0,0,11,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
11,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,10,194,0,16,0,
1,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,128,
65,0,0,0,10,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
10,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,10,194,0,16,0,
0,0,0,0,6,4,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,6,4,16,128,
65,0,0,0,9,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
9,0,0,0,230,10,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,56,0,
0,7,114,0,16,0,
15,0,0,0,150,7,
16,0,8,0,0,0,
70,2,16,0,15,0,
0,0,50,0,0,9,
114,0,16,0,14,0,
0,0,102,8,16,0,
6,0,0,0,70,2,
16,0,14,0,0,0,
70,2,16,0,15,0,
0,0,50,0,0,9,
114,0,16,0,14,0,
0,0,150,7,16,0,
5,0,0,0,70,2,
16,0,16,0,0,0,
70,2,16,0,14,0,
0,0,50,0,0,9,
114,0,16,0,14,0,
0,0,70,3,16,0,
3,0,0,0,70,2,
16,0,17,0,0,0,
70,2,16,0,14,0,
0,0,50,0,0,9,
114,0,16,0,14,0,
0,0,246,14,16,0,
3,0,0,0,70,2,
16,0,18,0,0,0,
70,2,16,0,14,0,
0,0,50,0,0,9,
114,0,16,0,14,0,
0,0,150,7,16,0,
4,0,0,0,70,2,
16,0,19,0,0,0,
70,2,16,0,14,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,182,13,16,0,
4,0,0,0,70,2,
16,0,2,0,0,0,
70,2,16,0,14,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,230,11,16,0,
3,0,0,0,70,2,
16,0,13,0,0,0,
70,2,16,0,2,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,118,12,16,0,
3,0,0,0,70,2,
16,0,12,0,0,0,
70,2,16,0,2,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,182,13,16,0,
5,0,0,0,70,2,
16,0,11,0,0,0,
70,2,16,0,2,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,182,13,16,0,
8,0,0,0,70,2,
16,0,10,0,0,0,
70,2,16,0,2,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,70,2,16,0,
6,0,0,0,70,2,
16,0,9,0,0,0,
70,2,16,0,2,0,
0,0,56,0,0,7,
114,0,16,0,2,0,
0,0,70,2,16,0,
2,0,0,0,70,2,
16,0,7,0,0,0,
18,0,0,1,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,2,0,0,0,
70,0,16,0,1,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
21,0,0,1,0,0,
0,11,194,0,16,0,
0,0,0,0,6,4,
16,128,65,0,0,0,
0,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
128,63,0,0,128,63,
51,0,0,7,50,0,
16,0,0,0,0,0,
230,10,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,50,0,
0,11,50,0,16,0,
0,0,0,0,70,0,
16,128,65,0,0,0,
0,0,0,0,70,16,
16,0,3,0,0,0,
6,128,32,0,0,0,
0,0,14,0,0,0,
52,0,0,10,50,0,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,15,0,
0,7,18,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,75,0,0,5,
18,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,14,0,
0,8,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
10,128,32,0,0,0,
0,0,14,0,0,0,
0,0,0,8,18,0,
16,0,0,0,0,0,
10,0,16,128,65,0,
0,0,0,0,0,0,
1,64,0,0,0,0,
128,63,52,0,0,7,
18,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,1,64,
0,0,0,0,0,0,
47,0,0,5,18,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,56,0,0,8,
18,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,26,128,
32,0,0,0,0,0,
14,0,0,0,25,0,
0,5,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
52,0,0,8,34,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,42,128,32,0,
0,0,0,0,14,0,
0,0,56,0,0,7,
18,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
51,0,0,7,18,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,1,64,0,0,
0,0,128,63,56,0,
0,7,114,0,16,0,
0,0,0,0,6,0,
16,0,0,0,0,0,
70,2,16,0,2,0,
0,0,14,0,0,11,
130,0,16,0,0,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,26,128,
32,0,0,0,0,0,
4,0,0,0,47,0,
0,5,114,0,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
56,0,0,7,114,0,
16,0,0,0,0,0,
70,2,16,0,0,0,
0,0,246,15,16,0,
0,0,0,0,25,0,
0,5,114,32,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
54,0,0,5,130,32,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,62,0,0,1,
83,84,65,84,148,0,
0,0,90,1,0,0,
20,0,0,0,0,0,
0,0,9,0,0,0,
24,1,0,0,0,0,
0,0,11,0,0,0,
6,0,0,0,5,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,13,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
17,0,0,0,9,0,
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
class CrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDef : public ShaderDef
{
public:
	CrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDef() : ShaderDef{}
	{
		Name = "crt-royale-geometry-aa-last-pass";
		VertexByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDefs::sFragmentByteCode);
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
		Params.push_back(ShaderParam("mask_num_triads_desired", 0, 160, 4, 342.000000f, 1920.000000f, 480.000000f, 1.000000f, "Mask - Number of Triads Desired"));
		Params.push_back(ShaderParam("mask_triad_size_desired", 0, 164, 4, 1.000000f, 18.000000f, 3.000000f, 0.125000f, "Mask - Triad Size Desired"));
		Params.push_back(ShaderParam("mask_specify_num_triads", 0, 168, 4, 0.000000f, 1.000000f, 0.000000f, 1.000000f, "Mask - Specify Number of Triads"));
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
		Params.push_back(ShaderParam("interlace_bff", 0, 236, 4, 0.000000f, 1.000000f, 0.000000f, 1.000000f, "Interlacing - Bottom Field First"));
		Params.push_back(ShaderParam("interlace_1080i", 0, 240, 4, 0.000000f, 1.000000f, 0.000000f, 1.000000f, "Interlace - Detect 1080i"));
		Params.push_back(ShaderParam("interlace_detect_toggle", 0, 244, 4, 0.000000f, 1.000000f, 1.000000f, 1.000000f, "Interlacing - Toggle"));
		Params.push_back(ShaderParam("MASKED_SCANLINESSize", -1, 64, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("HALATION_BLURSize", -1, 80, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("BRIGHTPASSSize", -1, 96, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Samplers.push_back(ShaderSampler("Source", 2));
/*
VertexSource = %*VERTEX_SOURCE*%;
*/
/*
FragmentSource = %*FRAGMENT_SOURCE*%;
*/
	}
};
}
