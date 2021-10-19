/*
ShaderGlass shader crt-shaders-crt-royale-src\crt-royale-scanlines-vertical-interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/shaders/crt-royale/src/crt-royale-scanlines-vertical-interlacing.slang
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
//////////////////////////////////  INCLUDES  //////////////////////////////////
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
//  Detect interlacing: il_step_multiple indicates the step multiple between
//  lines: 1 is for progressive sources, and 2 is for interlaced sources.
//  Get the uv tex coords step between one texel (x) and scanline (y):
//  If shader parameters are used, {min, max}_{sigma, shape} are runtime
//  values.  Compute {sigma, shape}_range outside of scanline_contrib() so
//  they aren't computed once per scanline (6 times per fragment and up to
//  18 times per vertex):
//  TODO/FIXME: if these aren't used, why are they calculated? commenting for now
//    const floatsigma_range = max(beam_max_sigma, beam_min_sigma) -
//        beam_min_sigma;
//    const float shape_range = max(beam_max_shape, beam_min_shape) -
//        beam_min_shape;
//  We need the pixel height in scanlines for antialiased/integral sampling:
//  This pass: Sample multiple (misconverged?) scanlines to the final
//  vertical resolution.  Temporarily auto-dim the output to avoid clipping.
//  Read some attributes into local variables:
//const float2 uv_step = uv_step;
//const float2 il_step_multiple = il_step_multiple;
//  Get the uv coords of the previous scanline (in this field), and the
//  scanline's distance from this sample, in scanlines.
//  Consider 2, 3, 4, or 6 scanlines numbered 0-5: The previous and next
//  scanlines are numbered 2 and 3.  Get scanline colors colors (ignore
//  horizontal sampling, since since IN.output_size.x = video_size.x).
//  NOTE: Anisotropic filtering creates interlacing artifacts, which is why
//  ORIG_LINEARIZED bobbed any interlaced input before this pass.
//  Use scanlines 0, 1, 4, and 5 for a total of 6 scanlines:
//  Use scanlines 1, 4, and either 0 or 5 for a total of 5 scanlines:
//  dist is in [0, 1]
//  Call this "scanline_outside_color" to cope with the conditional
//  scanline number:
//  Use scanlines 1 and 4 for a total of 4 scanlines:
//  Use scanline 1 or 4 for a total of 3 scanlines:
//  dist is in [0, 1]
//  Compute scanline contributions, accounting for vertical convergence.
//  Vertical convergence offsets are in units of current-field scanlines.
//  dist2 means "positive sample distance from scanline 2, in scanlines:
//  Calculate {sigma, shape}_range outside of scanline_contrib so it's only
//  done once per pixel (not 6 times) with runtime params.  Don't reuse the
//  vertex shader calculations, so static versions can be constant-folded.
//  Calculate and sum final scanline contributions, starting with lines 2/3.
//  There is no normalization step, because we're not interpolating a
//  continuous signal.  Instead, each scanline is an additive light source.
//  Auto-dim the image to avoid clipping, encode if necessary, and output.
//  My original idea was to compute a minimal auto-dim factor and put it in
//  the alpha channel, but it wasn't working, at least not reliably.  This
//  is faster anyway, levels_autodim_temp = 0.5 isn't causing banding.

*/

#pragma once

namespace RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,8,77,
194,22,11,64,216,48,
31,193,8,238,165,209,
160,15,1,0,0,0,
48,19,0,0,5,0,
0,0,52,0,0,0,
176,14,0,0,252,14,
0,0,156,15,0,0,
148,18,0,0,82,68,
69,70,116,14,0,0,
2,0,0,0,136,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
76,14,0,0,82,68,
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
0,0,4,0,0,0,
8,13,0,0,64,0,
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
4,0,0,0,2,0,
0,0,96,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,232,12,0,0,
244,0,0,0,4,0,
0,0,2,0,0,0,
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
168,13,0,0,0,0,
0,0,16,0,0,0,
2,0,0,0,196,13,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,232,13,
0,0,16,0,0,0,
16,0,0,0,0,0,
0,0,196,13,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,252,13,0,0,
32,0,0,0,16,0,
0,0,2,0,0,0,
196,13,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
14,14,0,0,48,0,
0,0,4,0,0,0,
0,0,0,0,40,14,
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
0,0,0,0,186,13,
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
0,0,32,14,0,0,
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
152,0,0,0,5,0,
0,0,8,0,0,0,
128,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,12,0,0,
128,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,3,0,0,
128,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,3,12,0,0,
128,0,0,0,3,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,4,11,0,0,
137,0,0,0,0,0,
0,0,1,0,0,0,
3,0,0,0,2,0,
0,0,15,0,0,0,
84,69,88,67,79,79,
82,68,0,83,86,95,
80,111,115,105,116,105,
111,110,0,171,171,171,
83,72,69,88,240,2,
0,0,80,0,1,0,
188,0,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,16,0,0,0,
89,0,0,4,70,142,
32,0,1,0,0,0,
3,0,0,0,95,0,
0,3,242,16,16,0,
0,0,0,0,95,0,
0,3,50,16,16,0,
1,0,0,0,101,0,
0,3,50,32,16,0,
0,0,0,0,101,0,
0,3,194,32,16,0,
0,0,0,0,101,0,
0,3,50,32,16,0,
1,0,0,0,101,0,
0,3,66,32,16,0,
1,0,0,0,103,0,
0,4,242,32,16,0,
2,0,0,0,1,0,
0,0,104,0,0,2,
2,0,0,0,49,0,
0,11,50,0,16,0,
0,0,0,0,2,64,
0,0,0,64,144,67,
0,240,134,68,0,0,
0,0,0,0,0,0,
86,133,32,0,1,0,
0,0,0,0,0,0,
57,0,0,11,194,0,
16,0,0,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,86,129,32,0,
0,0,0,0,15,0,
0,0,1,0,0,7,
34,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
49,0,0,11,50,0,
16,0,1,0,0,0,
86,133,32,0,1,0,
0,0,0,0,0,0,
2,64,0,0,0,32,
16,68,0,16,135,68,
0,0,0,0,0,0,
0,0,1,0,0,7,
50,0,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,70,0,
16,0,1,0,0,0,
60,0,0,7,18,0,
16,0,0,0,0,0,
26,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,1,0,
0,7,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,55,0,0,9,
34,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,1,64,
0,0,0,0,0,64,
1,64,0,0,0,0,
128,63,54,0,0,5,
18,0,16,0,0,0,
0,0,1,64,0,0,
0,0,128,63,14,0,
0,8,194,32,16,0,
0,0,0,0,6,4,
16,0,0,0,0,0,
6,132,32,0,1,0,
0,0,0,0,0,0,
54,0,0,5,50,32,
16,0,1,0,0,0,
70,0,16,0,0,0,
0,0,56,0,0,10,
50,32,16,0,0,0,
0,0,70,16,16,0,
1,0,0,0,2,64,
0,0,84,0,128,63,
84,0,128,63,0,0,
0,0,0,0,0,0,
14,0,0,9,18,0,
16,0,0,0,0,0,
26,128,32,0,1,0,
0,0,0,0,0,0,
26,128,32,0,1,0,
0,0,2,0,0,0,
14,0,0,7,66,32,
16,0,1,0,0,0,
10,0,16,0,0,0,
0,0,26,0,16,0,
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
19,0,0,0,2,0,
0,0,0,0,0,0,
7,0,0,0,11,0,
0,0,0,0,0,0,
4,0,0,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,2,0,
0,0,1,0,0,0,
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
0,0,0,0
};


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,76,230,
132,67,25,254,198,78,
28,10,169,180,109,236,
100,134,1,0,0,0,
100,40,0,0,5,0,
0,0,52,0,0,0,
4,15,0,0,128,15,
0,0,180,15,0,0,
200,39,0,0,82,68,
69,70,200,14,0,0,
2,0,0,0,220,0,
0,0,4,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
160,14,0,0,82,68,
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
0,0,4,0,0,0,
92,13,0,0,64,0,
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
0,0,0,0,180,8,
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
2,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,123,9,
0,0,96,0,0,0,
4,0,0,0,2,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,145,9,0,0,
100,0,0,0,4,0,
0,0,2,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
168,9,0,0,104,0,
0,0,4,0,0,0,
2,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,190,9,
0,0,108,0,0,0,
4,0,0,0,2,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,212,9,0,0,
112,0,0,0,4,0,
0,0,2,0,0,0,
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
2,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,185,10,
0,0,144,0,0,0,
4,0,0,0,2,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,215,10,0,0,
148,0,0,0,4,0,
0,0,2,0,0,0,
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
4,0,0,0,0,0,
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
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,16,12,
0,0,192,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,35,12,0,0,
196,0,0,0,4,0,
0,0,0,0,0,0,
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
0,0,0,0,180,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,226,12,
0,0,228,0,0,0,
4,0,0,0,0,0,
0,0,180,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,249,12,0,0,
232,0,0,0,4,0,
0,0,0,0,0,0,
180,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
16,13,0,0,236,0,
0,0,4,0,0,0,
2,0,0,0,180,8,
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
252,13,0,0,0,0,
0,0,16,0,0,0,
2,0,0,0,24,14,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,60,14,
0,0,16,0,0,0,
16,0,0,0,0,0,
0,0,24,14,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,80,14,0,0,
32,0,0,0,16,0,
0,0,0,0,0,0,
24,14,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
98,14,0,0,48,0,
0,0,4,0,0,0,
2,0,0,0,124,14,
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
0,0,0,0,14,14,
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
0,0,116,14,0,0,
77,105,99,114,111,115,
111,102,116,32,40,82,
41,32,72,76,83,76,
32,83,104,97,100,101,
114,32,67,111,109,112,
105,108,101,114,32,49,
48,46,49,0,73,83,
71,78,116,0,0,0,
4,0,0,0,8,0,
0,0,104,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,3,3,
0,0,104,0,0,0,
1,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,12,8,
0,0,104,0,0,0,
2,0,0,0,0,0,
0,0,3,0,0,0,
1,0,0,0,3,2,
0,0,104,0,0,0,
3,0,0,0,0,0,
0,0,3,0,0,0,
1,0,0,0,4,4,
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
83,72,69,88,12,24,
0,0,80,0,0,0,
3,6,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,15,0,0,0,
89,0,0,4,70,142,
32,0,1,0,0,0,
4,0,0,0,90,0,
0,3,0,96,16,0,
2,0,0,0,88,24,
0,4,0,112,16,0,
2,0,0,0,85,85,
0,0,98,16,0,3,
50,16,16,0,0,0,
0,0,98,16,0,3,
130,16,16,0,0,0,
0,0,98,16,0,3,
34,16,16,0,1,0,
0,0,98,16,0,3,
66,16,16,0,1,0,
0,0,101,0,0,3,
242,32,16,0,0,0,
0,0,104,0,0,2,
12,0,0,0,86,0,
0,6,18,0,16,0,
0,0,0,0,10,128,
32,0,1,0,0,0,
3,0,0,0,0,0,
0,8,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
58,128,32,0,0,0,
0,0,14,0,0,0,
56,0,0,7,34,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,1,64,0,0,
0,0,0,63,65,0,
0,5,34,0,16,0,
0,0,0,0,26,0,
16,0,0,0,0,0,
50,0,0,10,18,0,
16,0,0,0,0,0,
26,0,16,128,65,0,
0,0,0,0,0,0,
1,64,0,0,0,0,
0,64,10,0,16,0,
0,0,0,0,56,0,
0,7,34,0,16,0,
0,0,0,0,26,16,
16,0,1,0,0,0,
1,64,0,0,0,0,
64,63,50,0,0,13,
194,0,16,0,0,0,
0,0,6,20,16,0,
0,0,0,0,6,132,
32,0,1,0,0,0,
0,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,119,190,
255,190,119,190,255,190,
65,0,0,5,226,0,
16,0,0,0,0,0,
86,14,16,0,0,0,
0,0,50,0,0,9,
18,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
58,0,16,0,0,0,
0,0,14,0,0,7,
34,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,26,16,
16,0,1,0,0,0,
65,0,0,5,34,0,
16,0,0,0,0,0,
26,0,16,0,0,0,
0,0,50,0,0,10,
34,0,16,0,1,0,
0,0,26,16,16,128,
65,0,0,0,1,0,
0,0,26,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
54,0,0,8,82,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,8,
50,0,16,0,0,0,
0,0,230,10,16,0,
0,0,0,0,70,0,
16,128,65,0,0,0,
1,0,0,0,0,0,
0,10,50,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
2,64,0,0,0,0,
0,63,0,0,0,63,
0,0,0,0,0,0,
0,0,54,0,0,5,
130,0,16,0,1,0,
0,0,58,16,16,0,
0,0,0,0,14,0,
0,11,194,0,16,0,
0,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,128,63,
6,132,32,0,1,0,
0,0,0,0,0,0,
50,0,0,9,50,0,
16,0,1,0,0,0,
70,0,16,0,0,0,
0,0,230,10,16,0,
0,0,0,0,230,10,
16,0,1,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,2,0,
0,0,70,0,16,0,
1,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,47,0,0,5,
114,0,16,0,3,0,
0,0,70,2,16,0,
2,0,0,0,56,0,
0,8,114,0,16,0,
4,0,0,0,70,2,
16,0,3,0,0,0,
6,128,32,0,0,0,
0,0,7,0,0,0,
56,0,0,8,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,86,133,32,0,
0,0,0,0,6,0,
0,0,25,0,0,5,
114,0,16,0,3,0,
0,0,70,2,16,0,
3,0,0,0,25,0,
0,5,114,0,16,0,
4,0,0,0,70,2,
16,0,4,0,0,0,
52,0,0,9,18,0,
16,0,1,0,0,0,
42,128,32,0,0,0,
0,0,6,0,0,0,
58,128,32,0,0,0,
0,0,6,0,0,0,
0,0,0,9,18,0,
16,0,1,0,0,0,
10,0,16,0,1,0,
0,0,42,128,32,128,
65,0,0,0,0,0,
0,0,6,0,0,0,
50,0,0,10,114,0,
16,0,4,0,0,0,
70,2,16,0,4,0,
0,0,6,0,16,0,
1,0,0,0,166,138,
32,0,0,0,0,0,
6,0,0,0,14,0,
0,10,114,0,16,0,
5,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,128,63,
70,2,16,0,4,0,
0,0,0,0,0,10,
114,0,16,0,6,0,
0,0,70,2,16,0,
5,0,0,0,2,64,
0,0,79,133,208,63,
79,133,208,63,79,133,
208,63,0,0,0,0,
56,0,0,10,114,0,
16,0,6,0,0,0,
70,2,16,0,6,0,
0,0,2,64,0,0,
178,90,188,62,178,90,
188,62,178,90,188,62,
0,0,0,0,47,0,
0,5,114,0,16,0,
6,0,0,0,70,2,
16,0,6,0,0,0,
0,0,0,10,114,0,
16,0,7,0,0,0,
70,2,16,0,5,0,
0,0,2,64,0,0,
0,0,0,63,0,0,
0,63,0,0,0,63,
0,0,0,0,0,0,
0,10,114,0,16,0,
5,0,0,0,70,2,
16,0,5,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
0,0,14,0,0,10,
114,0,16,0,5,0,
0,0,2,64,0,0,
17,48,246,62,17,48,
246,62,17,48,246,62,
0,0,0,0,70,2,
16,0,5,0,0,0,
0,0,0,10,114,0,
16,0,5,0,0,0,
70,2,16,0,5,0,
0,0,2,64,0,0,
237,151,79,63,237,151,
79,63,237,151,79,63,
0,0,0,0,56,0,
0,7,114,0,16,0,
6,0,0,0,70,2,
16,0,6,0,0,0,
70,2,16,0,7,0,
0,0,25,0,0,5,
114,0,16,0,6,0,
0,0,70,2,16,0,
6,0,0,0,56,0,
0,7,114,0,16,0,
5,0,0,0,70,2,
16,0,5,0,0,0,
70,2,16,0,6,0,
0,0,56,0,0,7,
114,0,16,0,5,0,
0,0,70,2,16,0,
4,0,0,0,70,2,
16,0,5,0,0,0,
56,0,0,7,114,0,
16,0,2,0,0,0,
70,2,16,0,2,0,
0,0,70,2,16,0,
4,0,0,0,52,0,
0,9,34,0,16,0,
1,0,0,0,58,128,
32,0,0,0,0,0,
5,0,0,0,10,128,
32,0,0,0,0,0,
6,0,0,0,0,0,
0,9,34,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
58,128,32,128,65,0,
0,0,0,0,0,0,
5,0,0,0,50,0,
0,10,114,0,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
86,5,16,0,1,0,
0,0,246,143,32,0,
0,0,0,0,5,0,
0,0,56,0,0,10,
114,0,16,0,3,0,
0,0,70,2,16,0,
3,0,0,0,2,64,
0,0,243,4,181,63,
243,4,181,63,243,4,
181,63,0,0,0,0,
14,0,0,10,114,0,
16,0,3,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,70,2,16,0,
3,0,0,0,56,0,
0,7,114,0,16,0,
2,0,0,0,70,2,
16,0,2,0,0,0,
70,2,16,0,3,0,
0,0,56,0,0,10,
114,0,16,0,2,0,
0,0,70,2,16,0,
2,0,0,0,2,64,
0,0,0,0,0,63,
0,0,0,63,0,0,
0,63,0,0,0,0,
14,0,0,7,114,0,
16,0,2,0,0,0,
70,2,16,0,2,0,
0,0,70,2,16,0,
5,0,0,0,56,0,
0,10,114,0,16,0,
2,0,0,0,70,2,
16,0,2,0,0,0,
2,64,0,0,171,170,
170,62,171,170,170,62,
171,170,170,62,0,0,
0,0,50,0,0,11,
130,0,16,0,2,0,
0,0,26,16,16,0,
0,0,0,0,26,128,
32,0,1,0,0,0,
0,0,0,0,26,0,
16,128,65,0,0,0,
0,0,0,0,14,0,
0,7,130,0,16,0,
2,0,0,0,58,0,
16,0,2,0,0,0,
26,16,16,0,1,0,
0,0,54,0,0,7,
18,0,16,0,5,0,
0,0,58,128,32,128,
65,0,0,0,0,0,
0,0,8,0,0,0,
54,0,0,7,98,0,
16,0,5,0,0,0,
6,129,32,128,65,0,
0,0,0,0,0,0,
9,0,0,0,0,0,
0,7,114,0,16,0,
5,0,0,0,246,15,
16,0,2,0,0,0,
70,2,16,0,5,0,
0,0,64,0,0,5,
18,0,16,0,6,0,
0,0,58,0,16,0,
2,0,0,0,0,0,
0,11,114,0,16,0,
7,0,0,0,70,2,
16,128,65,0,0,0,
5,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,0,0,
50,0,0,13,114,0,
16,0,8,0,0,0,
166,26,16,0,1,0,
0,0,2,64,0,0,
171,170,170,62,171,170,
170,62,171,170,170,62,
0,0,0,0,70,2,
16,128,129,0,0,0,
7,0,0,0,56,0,
0,7,114,0,16,0,
8,0,0,0,70,2,
16,0,3,0,0,0,
70,2,16,0,8,0,
0,0,47,0,0,6,
114,0,16,0,8,0,
0,0,70,2,16,128,
129,0,0,0,8,0,
0,0,56,0,0,7,
114,0,16,0,8,0,
0,0,70,2,16,0,
4,0,0,0,70,2,
16,0,8,0,0,0,
25,0,0,5,114,0,
16,0,8,0,0,0,
70,2,16,0,8,0,
0,0,56,0,0,10,
114,0,16,0,8,0,
0,0,70,2,16,0,
8,0,0,0,2,64,
0,0,59,170,184,191,
59,170,184,191,59,170,
184,191,0,0,0,0,
25,0,0,5,114,0,
16,0,8,0,0,0,
70,2,16,0,8,0,
0,0,56,0,0,8,
114,0,16,0,9,0,
0,0,70,2,16,0,
3,0,0,0,70,2,
16,128,129,0,0,0,
7,0,0,0,50,0,
0,14,114,0,16,0,
7,0,0,0,166,26,
16,128,65,0,0,0,
1,0,0,0,2,64,
0,0,171,170,170,62,
171,170,170,62,171,170,
170,62,0,0,0,0,
70,2,16,128,129,0,
0,0,7,0,0,0,
56,0,0,8,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,70,2,16,128,
129,0,0,0,7,0,
0,0,47,0,0,6,
114,0,16,0,3,0,
0,0,70,2,16,128,
129,0,0,0,3,0,
0,0,56,0,0,7,
114,0,16,0,3,0,
0,0,70,2,16,0,
3,0,0,0,70,2,
16,0,4,0,0,0,
25,0,0,5,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,56,0,0,10,
114,0,16,0,3,0,
0,0,70,2,16,0,
3,0,0,0,2,64,
0,0,59,170,184,191,
59,170,184,191,59,170,
184,191,0,0,0,0,
25,0,0,5,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,47,0,0,6,
114,0,16,0,7,0,
0,0,70,2,16,128,
129,0,0,0,9,0,
0,0,56,0,0,7,
114,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,70,2,
16,0,7,0,0,0,
25,0,0,5,114,0,
16,0,4,0,0,0,
70,2,16,0,4,0,
0,0,56,0,0,10,
114,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,2,64,
0,0,59,170,184,191,
59,170,184,191,59,170,
184,191,0,0,0,0,
25,0,0,5,114,0,
16,0,4,0,0,0,
70,2,16,0,4,0,
0,0,0,0,0,7,
114,0,16,0,4,0,
0,0,70,2,16,0,
8,0,0,0,70,2,
16,0,4,0,0,0,
0,0,0,7,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,70,2,16,0,
4,0,0,0,56,0,
0,7,114,0,16,0,
2,0,0,0,70,2,
16,0,2,0,0,0,
70,2,16,0,3,0,
0,0,56,0,0,7,
50,0,16,0,3,0,
0,0,230,10,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,3,0,
0,0,70,0,16,0,
3,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,47,0,0,5,
114,0,16,0,4,0,
0,0,70,2,16,0,
3,0,0,0,56,0,
0,8,114,0,16,0,
7,0,0,0,70,2,
16,0,4,0,0,0,
86,133,32,0,0,0,
0,0,6,0,0,0,
56,0,0,8,114,0,
16,0,4,0,0,0,
70,2,16,0,4,0,
0,0,6,128,32,0,
0,0,0,0,7,0,
0,0,25,0,0,5,
114,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,50,0,
0,10,114,0,16,0,
4,0,0,0,70,2,
16,0,4,0,0,0,
6,0,16,0,1,0,
0,0,166,138,32,0,
0,0,0,0,6,0,
0,0,25,0,0,5,
114,0,16,0,7,0,
0,0,70,2,16,0,
7,0,0,0,50,0,
0,10,114,0,16,0,
7,0,0,0,70,2,
16,0,7,0,0,0,
86,5,16,0,1,0,
0,0,246,143,32,0,
0,0,0,0,5,0,
0,0,56,0,0,10,
114,0,16,0,7,0,
0,0,70,2,16,0,
7,0,0,0,2,64,
0,0,243,4,181,63,
243,4,181,63,243,4,
181,63,0,0,0,0,
14,0,0,10,114,0,
16,0,7,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,70,2,16,0,
7,0,0,0,50,0,
0,12,114,0,16,0,
8,0,0,0,166,26,
16,0,1,0,0,0,
2,64,0,0,171,170,
170,62,171,170,170,62,
171,170,170,62,0,0,
0,0,70,2,16,0,
5,0,0,0,56,0,
0,7,114,0,16,0,
8,0,0,0,70,2,
16,0,7,0,0,0,
70,2,16,0,8,0,
0,0,47,0,0,6,
114,0,16,0,8,0,
0,0,70,2,16,128,
129,0,0,0,8,0,
0,0,56,0,0,7,
114,0,16,0,8,0,
0,0,70,2,16,0,
4,0,0,0,70,2,
16,0,8,0,0,0,
25,0,0,5,114,0,
16,0,8,0,0,0,
70,2,16,0,8,0,
0,0,56,0,0,10,
114,0,16,0,8,0,
0,0,70,2,16,0,
8,0,0,0,2,64,
0,0,59,170,184,191,
59,170,184,191,59,170,
184,191,0,0,0,0,
25,0,0,5,114,0,
16,0,8,0,0,0,
70,2,16,0,8,0,
0,0,56,0,0,7,
114,0,16,0,9,0,
0,0,70,2,16,0,
5,0,0,0,70,2,
16,0,7,0,0,0,
47,0,0,6,114,0,
16,0,9,0,0,0,
70,2,16,128,129,0,
0,0,9,0,0,0,
56,0,0,7,114,0,
16,0,9,0,0,0,
70,2,16,0,4,0,
0,0,70,2,16,0,
9,0,0,0,25,0,
0,5,114,0,16,0,
9,0,0,0,70,2,
16,0,9,0,0,0,
56,0,0,10,114,0,
16,0,9,0,0,0,
70,2,16,0,9,0,
0,0,2,64,0,0,
59,170,184,191,59,170,
184,191,59,170,184,191,
0,0,0,0,25,0,
0,5,114,0,16,0,
9,0,0,0,70,2,
16,0,9,0,0,0,
0,0,0,7,114,0,
16,0,8,0,0,0,
70,2,16,0,8,0,
0,0,70,2,16,0,
9,0,0,0,50,0,
0,13,114,0,16,0,
9,0,0,0,166,26,
16,128,65,0,0,0,
1,0,0,0,2,64,
0,0,171,170,170,62,
171,170,170,62,171,170,
170,62,0,0,0,0,
70,2,16,0,5,0,
0,0,56,0,0,8,
114,0,16,0,9,0,
0,0,70,2,16,0,
7,0,0,0,70,2,
16,128,129,0,0,0,
9,0,0,0,47,0,
0,6,114,0,16,0,
9,0,0,0,70,2,
16,128,129,0,0,0,
9,0,0,0,56,0,
0,7,114,0,16,0,
9,0,0,0,70,2,
16,0,4,0,0,0,
70,2,16,0,9,0,
0,0,25,0,0,5,
114,0,16,0,9,0,
0,0,70,2,16,0,
9,0,0,0,56,0,
0,10,114,0,16,0,
9,0,0,0,70,2,
16,0,9,0,0,0,
2,64,0,0,59,170,
184,191,59,170,184,191,
59,170,184,191,0,0,
0,0,25,0,0,5,
114,0,16,0,9,0,
0,0,70,2,16,0,
9,0,0,0,0,0,
0,7,114,0,16,0,
8,0,0,0,70,2,
16,0,8,0,0,0,
70,2,16,0,9,0,
0,0,14,0,0,10,
114,0,16,0,9,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,70,2,
16,0,4,0,0,0,
0,0,0,10,114,0,
16,0,10,0,0,0,
70,2,16,0,9,0,
0,0,2,64,0,0,
79,133,208,63,79,133,
208,63,79,133,208,63,
0,0,0,0,56,0,
0,10,114,0,16,0,
10,0,0,0,70,2,
16,0,10,0,0,0,
2,64,0,0,178,90,
188,62,178,90,188,62,
178,90,188,62,0,0,
0,0,47,0,0,5,
114,0,16,0,10,0,
0,0,70,2,16,0,
10,0,0,0,0,0,
0,10,114,0,16,0,
11,0,0,0,70,2,
16,0,9,0,0,0,
2,64,0,0,0,0,
0,63,0,0,0,63,
0,0,0,63,0,0,
0,0,0,0,0,10,
114,0,16,0,9,0,
0,0,70,2,16,0,
9,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,0,0,
14,0,0,10,114,0,
16,0,9,0,0,0,
2,64,0,0,17,48,
246,62,17,48,246,62,
17,48,246,62,0,0,
0,0,70,2,16,0,
9,0,0,0,0,0,
0,10,114,0,16,0,
9,0,0,0,70,2,
16,0,9,0,0,0,
2,64,0,0,237,151,
79,63,237,151,79,63,
237,151,79,63,0,0,
0,0,56,0,0,7,
114,0,16,0,10,0,
0,0,70,2,16,0,
10,0,0,0,70,2,
16,0,11,0,0,0,
25,0,0,5,114,0,
16,0,10,0,0,0,
70,2,16,0,10,0,
0,0,56,0,0,7,
114,0,16,0,9,0,
0,0,70,2,16,0,
9,0,0,0,70,2,
16,0,10,0,0,0,
56,0,0,7,114,0,
16,0,9,0,0,0,
70,2,16,0,4,0,
0,0,70,2,16,0,
9,0,0,0,56,0,
0,7,114,0,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
70,2,16,0,4,0,
0,0,56,0,0,7,
114,0,16,0,3,0,
0,0,70,2,16,0,
7,0,0,0,70,2,
16,0,3,0,0,0,
56,0,0,10,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,2,64,0,0,
0,0,0,63,0,0,
0,63,0,0,0,63,
0,0,0,0,14,0,
0,7,114,0,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
70,2,16,0,9,0,
0,0,56,0,0,10,
114,0,16,0,3,0,
0,0,70,2,16,0,
3,0,0,0,2,64,
0,0,171,170,170,62,
171,170,170,62,171,170,
170,62,0,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,3,0,
0,0,70,2,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
56,0,0,7,34,0,
16,0,6,0,0,0,
10,0,16,0,6,0,
0,0,58,16,16,0,
0,0,0,0,50,0,
0,13,194,0,16,0,
1,0,0,0,6,4,
16,0,6,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
64,64,166,14,16,128,
65,0,0,0,1,0,
0,0,50,0,0,9,
50,0,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,230,10,
16,0,0,0,0,0,
230,10,16,0,1,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,47,0,
0,5,114,0,16,0,
3,0,0,0,70,2,
16,0,0,0,0,0,
56,0,0,8,114,0,
16,0,4,0,0,0,
70,2,16,0,3,0,
0,0,86,133,32,0,
0,0,0,0,6,0,
0,0,56,0,0,8,
114,0,16,0,3,0,
0,0,70,2,16,0,
3,0,0,0,6,128,
32,0,0,0,0,0,
7,0,0,0,25,0,
0,5,114,0,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
50,0,0,10,210,0,
16,0,1,0,0,0,
6,9,16,0,3,0,
0,0,6,0,16,0,
1,0,0,0,166,138,
32,0,0,0,0,0,
6,0,0,0,25,0,
0,5,114,0,16,0,
3,0,0,0,70,2,
16,0,4,0,0,0,
50,0,0,10,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,86,5,16,0,
1,0,0,0,246,143,
32,0,0,0,0,0,
5,0,0,0,56,0,
0,10,114,0,16,0,
3,0,0,0,70,2,
16,0,3,0,0,0,
2,64,0,0,243,4,
181,63,243,4,181,63,
243,4,181,63,0,0,
0,0,14,0,0,10,
114,0,16,0,3,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,70,2,
16,0,3,0,0,0,
0,0,0,11,114,0,
16,0,4,0,0,0,
70,2,16,128,65,0,
0,0,5,0,0,0,
2,64,0,0,0,0,
0,64,0,0,0,64,
0,0,0,64,0,0,
0,0,0,0,0,10,
114,0,16,0,5,0,
0,0,70,2,16,0,
5,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,0,0,
0,0,0,8,114,0,
16,0,4,0,0,0,
70,2,16,0,4,0,
0,0,70,2,16,128,
65,0,0,0,5,0,
0,0,50,0,0,9,
114,0,16,0,4,0,
0,0,6,0,16,0,
6,0,0,0,70,2,
16,0,4,0,0,0,
70,2,16,0,5,0,
0,0,50,0,0,12,
114,0,16,0,5,0,
0,0,166,26,16,0,
1,0,0,0,2,64,
0,0,171,170,170,62,
171,170,170,62,171,170,
170,62,0,0,0,0,
70,2,16,0,4,0,
0,0,56,0,0,7,
114,0,16,0,5,0,
0,0,70,2,16,0,
3,0,0,0,70,2,
16,0,5,0,0,0,
47,0,0,6,114,0,
16,0,5,0,0,0,
70,2,16,128,129,0,
0,0,5,0,0,0,
56,0,0,7,114,0,
16,0,5,0,0,0,
134,3,16,0,1,0,
0,0,70,2,16,0,
5,0,0,0,25,0,
0,5,114,0,16,0,
5,0,0,0,70,2,
16,0,5,0,0,0,
56,0,0,10,114,0,
16,0,5,0,0,0,
70,2,16,0,5,0,
0,0,2,64,0,0,
59,170,184,191,59,170,
184,191,59,170,184,191,
0,0,0,0,25,0,
0,5,114,0,16,0,
5,0,0,0,70,2,
16,0,5,0,0,0,
56,0,0,7,114,0,
16,0,6,0,0,0,
70,2,16,0,3,0,
0,0,70,2,16,0,
4,0,0,0,50,0,
0,13,114,0,16,0,
4,0,0,0,166,26,
16,128,65,0,0,0,
1,0,0,0,2,64,
0,0,171,170,170,62,
171,170,170,62,171,170,
170,62,0,0,0,0,
70,2,16,0,4,0,
0,0,56,0,0,8,
114,0,16,0,4,0,
0,0,70,2,16,0,
3,0,0,0,70,2,
16,128,129,0,0,0,
4,0,0,0,47,0,
0,6,114,0,16,0,
4,0,0,0,70,2,
16,128,129,0,0,0,
4,0,0,0,56,0,
0,7,114,0,16,0,
4,0,0,0,134,3,
16,0,1,0,0,0,
70,2,16,0,4,0,
0,0,25,0,0,5,
114,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,56,0,
0,10,114,0,16,0,
4,0,0,0,70,2,
16,0,4,0,0,0,
2,64,0,0,59,170,
184,191,59,170,184,191,
59,170,184,191,0,0,
0,0,25,0,0,5,
114,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,47,0,
0,6,114,0,16,0,
6,0,0,0,70,2,
16,128,129,0,0,0,
6,0,0,0,56,0,
0,7,114,0,16,0,
6,0,0,0,134,3,
16,0,1,0,0,0,
70,2,16,0,6,0,
0,0,25,0,0,5,
114,0,16,0,6,0,
0,0,70,2,16,0,
6,0,0,0,56,0,
0,10,114,0,16,0,
6,0,0,0,70,2,
16,0,6,0,0,0,
2,64,0,0,59,170,
184,191,59,170,184,191,
59,170,184,191,0,0,
0,0,25,0,0,5,
114,0,16,0,6,0,
0,0,70,2,16,0,
6,0,0,0,0,0,
0,7,114,0,16,0,
5,0,0,0,70,2,
16,0,5,0,0,0,
70,2,16,0,6,0,
0,0,0,0,0,7,
114,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,70,2,
16,0,5,0,0,0,
56,0,0,7,114,0,
16,0,0,0,0,0,
70,2,16,0,0,0,
0,0,134,3,16,0,
1,0,0,0,56,0,
0,7,114,0,16,0,
0,0,0,0,70,2,
16,0,3,0,0,0,
70,2,16,0,0,0,
0,0,56,0,0,10,
114,0,16,0,0,0,
0,0,70,2,16,0,
0,0,0,0,2,64,
0,0,0,0,0,63,
0,0,0,63,0,0,
0,63,0,0,0,0,
14,0,0,10,114,0,
16,0,3,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,134,3,16,0,
1,0,0,0,0,0,
0,10,114,0,16,0,
5,0,0,0,70,2,
16,0,3,0,0,0,
2,64,0,0,79,133,
208,63,79,133,208,63,
79,133,208,63,0,0,
0,0,56,0,0,10,
114,0,16,0,5,0,
0,0,70,2,16,0,
5,0,0,0,2,64,
0,0,178,90,188,62,
178,90,188,62,178,90,
188,62,0,0,0,0,
47,0,0,5,114,0,
16,0,5,0,0,0,
70,2,16,0,5,0,
0,0,0,0,0,10,
114,0,16,0,6,0,
0,0,70,2,16,0,
3,0,0,0,2,64,
0,0,0,0,0,63,
0,0,0,63,0,0,
0,63,0,0,0,0,
0,0,0,10,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,0,0,14,0,
0,10,114,0,16,0,
3,0,0,0,2,64,
0,0,17,48,246,62,
17,48,246,62,17,48,
246,62,0,0,0,0,
70,2,16,0,3,0,
0,0,0,0,0,10,
114,0,16,0,3,0,
0,0,70,2,16,0,
3,0,0,0,2,64,
0,0,237,151,79,63,
237,151,79,63,237,151,
79,63,0,0,0,0,
56,0,0,7,114,0,
16,0,5,0,0,0,
70,2,16,0,5,0,
0,0,70,2,16,0,
6,0,0,0,25,0,
0,5,114,0,16,0,
5,0,0,0,70,2,
16,0,5,0,0,0,
56,0,0,7,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,70,2,16,0,
5,0,0,0,56,0,
0,7,114,0,16,0,
1,0,0,0,134,3,
16,0,1,0,0,0,
70,2,16,0,3,0,
0,0,14,0,0,7,
114,0,16,0,0,0,
0,0,70,2,16,0,
0,0,0,0,70,2,
16,0,1,0,0,0,
56,0,0,10,114,0,
16,0,0,0,0,0,
70,2,16,0,0,0,
0,0,2,64,0,0,
171,170,170,62,171,170,
170,62,171,170,170,62,
0,0,0,0,50,0,
0,9,114,0,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
70,2,16,0,4,0,
0,0,70,2,16,0,
2,0,0,0,56,0,
0,10,114,32,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
2,64,0,0,0,0,
0,63,0,0,0,63,
0,0,0,63,0,0,
0,0,54,0,0,5,
130,32,16,0,0,0,
0,0,1,64,0,0,
0,0,128,63,62,0,
0,1,83,84,65,84,
148,0,0,0,190,0,
0,0,12,0,0,0,
0,0,0,0,5,0,
0,0,182,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,5,0,
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
0,0
};

}

namespace RetroArch
{
class CrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDef : public ShaderDef
{
public:
	CrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDef() : ShaderDef{}
	{
		Name = "crt-royale-scanlines-vertical-interlacing";
		VertexByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDefs::sFragmentByteCode);
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
