/*
ShaderGlass shader crt-shaders-crt-royale-src\crt-royale-mask-resize-horizontal imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/shaders/crt-royale/src/crt-royale-mask-resize-horizontal.slang
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
//////////////////////////////////  INCLUDES  //////////////////////////////////
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
//  First estimate the viewport size (the user will get the wrong number of
//  triads if it's wrong and mask_specify_num_triads is 1.0/true).
//  Find the final size of our resized phosphor mask tiles.  We probably
//  estimated the viewport size and MASK_RESIZE output size differently last
//  pass, so do not swear they were the same. ;)
//  We'll render resized tiles until filling the output FBO or meeting a
//  limit, so compute [wrapped] tile uv coords based on the output uv coords
//  and the number of tiles that will fit in the FBO.
//  Get the texel size of an input tile and related values:
//  Derive [wrapped] texture uv coords from [wrapped] tile uv coords and
//  the tile size in uv coords, and save frac() for the fragment shader.
//  Output the values we need, including the magnification scale and step:
//tile_uv_wrap = tile_uv_wrap;
//src_tex_uv_wrap = src_tex_uv_wrap;
//tile_size_uv = tile_size_uv;
//input_tiles_per_texture = input_tiles_per_texture;
//  The input contains one mask tile horizontally and a number vertically.
//  Resize the tile horizontally to its final screen size and repeat it
//  until drawing at least mask_resize_num_tiles, leaving it unchanged
//  vertically.  Lanczos-resizing the phosphor mask achieves much sharper
//  results than mipmapping, outputting >= mask_resize_num_tiles makes for
//  easier tiled sampling later.
//  Discard unneeded fragments in case our profile allows real branches.
//  The input LUT was linear RGB, and so is our output:

*/

#pragma once

namespace RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,135,200,
226,254,162,7,183,214,
255,240,239,129,156,94,
218,81,1,0,0,0,
212,20,0,0,5,0,
0,0,52,0,0,0,
176,14,0,0,252,14,
0,0,204,15,0,0,
56,20,0,0,82,68,
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
200,0,0,0,7,0,
0,0,8,0,0,0,
176,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,12,0,0,
176,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,3,0,0,
176,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,3,12,0,0,
176,0,0,0,3,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,12,3,0,0,
176,0,0,0,4,0,
0,0,0,0,0,0,
3,0,0,0,2,0,
0,0,3,12,0,0,
176,0,0,0,5,0,
0,0,0,0,0,0,
3,0,0,0,2,0,
0,0,12,3,0,0,
185,0,0,0,0,0,
0,0,1,0,0,0,
3,0,0,0,3,0,
0,0,15,0,0,0,
84,69,88,67,79,79,
82,68,0,83,86,95,
80,111,115,105,116,105,
111,110,0,171,171,171,
83,72,69,88,100,4,
0,0,80,0,1,0,
25,1,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,11,0,0,0,
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
0,3,194,32,16,0,
1,0,0,0,101,0,
0,3,50,32,16,0,
2,0,0,0,101,0,
0,3,194,32,16,0,
2,0,0,0,103,0,
0,4,242,32,16,0,
3,0,0,0,1,0,
0,0,104,0,0,2,
2,0,0,0,56,0,
0,11,114,0,16,0,
0,0,0,0,2,64,
0,0,0,0,128,65,
0,0,0,63,0,0,
0,63,0,0,0,0,
6,129,32,0,1,0,
0,0,2,0,0,0,
14,0,0,8,18,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,10,128,32,0,
0,0,0,0,10,0,
0,0,0,0,0,9,
18,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,26,128,
32,128,65,0,0,0,
0,0,0,0,10,0,
0,0,50,0,0,11,
18,0,16,0,0,0,
0,0,42,128,32,0,
0,0,0,0,10,0,
0,0,10,0,16,0,
0,0,0,0,26,128,
32,0,0,0,0,0,
10,0,0,0,56,0,
0,7,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
0,65,51,0,0,7,
130,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,1,64,
0,0,0,0,128,66,
52,0,0,7,130,0,
16,0,0,0,0,0,
58,0,16,0,0,0,
0,0,1,64,0,0,
0,0,128,65,51,0,
0,7,98,0,16,0,
1,0,0,0,86,6,
16,0,0,0,0,0,
246,15,16,0,0,0,
0,0,51,0,0,7,
18,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
0,0,0,10,98,0,
16,0,0,0,0,0,
6,2,16,0,1,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
128,55,0,0,128,55,
0,0,0,0,65,0,
0,5,98,0,16,0,
0,0,0,0,86,6,
16,0,0,0,0,0,
29,0,0,8,130,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
0,63,58,128,32,0,
0,0,0,0,9,0,
0,0,55,0,0,9,
98,0,16,0,0,0,
0,0,246,15,16,0,
0,0,0,0,86,6,
16,0,0,0,0,0,
6,0,16,0,0,0,
0,0,14,0,0,8,
50,0,16,0,1,0,
0,0,70,128,32,0,
1,0,0,0,2,0,
0,0,150,5,16,0,
0,0,0,0,56,0,
0,8,194,0,16,0,
1,0,0,0,6,20,
16,0,1,0,0,0,
6,132,32,0,1,0,
0,0,0,0,0,0,
14,0,0,8,194,0,
16,0,1,0,0,0,
166,14,16,0,1,0,
0,0,6,132,32,0,
1,0,0,0,0,0,
0,0,56,0,0,7,
50,0,16,0,1,0,
0,0,70,0,16,0,
1,0,0,0,230,10,
16,0,1,0,0,0,
54,0,0,5,194,32,
16,0,0,0,0,0,
6,4,16,0,1,0,
0,0,51,0,0,8,
18,0,16,0,0,0,
0,0,1,64,0,0,
0,0,128,66,10,128,
32,0,1,0,0,0,
0,0,0,0,14,0,
0,8,194,0,16,0,
1,0,0,0,6,8,
16,0,0,0,0,0,
6,132,32,0,1,0,
0,0,0,0,0,0,
56,0,0,7,50,32,
16,0,0,0,0,0,
230,10,16,0,1,0,
0,0,70,0,16,0,
1,0,0,0,54,0,
0,5,50,32,16,0,
2,0,0,0,230,10,
16,0,1,0,0,0,
14,0,0,7,50,32,
16,0,1,0,0,0,
150,5,16,0,0,0,
0,0,134,0,16,0,
0,0,0,0,14,0,
0,8,194,32,16,0,
2,0,0,0,6,132,
32,0,1,0,0,0,
0,0,0,0,6,8,
16,0,0,0,0,0,
14,0,0,11,66,32,
16,0,1,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,10,128,32,0,
1,0,0,0,0,0,
0,0,54,0,0,5,
130,32,16,0,1,0,
0,0,1,64,0,0,
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
242,32,16,0,3,0,
0,0,246,31,16,0,
0,0,0,0,70,142,
32,0,0,0,0,0,
3,0,0,0,70,14,
16,0,0,0,0,0,
62,0,0,1,83,84,
65,84,148,0,0,0,
31,0,0,0,2,0,
0,0,0,0,0,0,
9,0,0,0,26,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
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
68,88,66,67,245,210,
209,49,102,149,14,64,
9,10,42,127,95,126,
115,0,1,0,0,0,
136,38,0,0,5,0,
0,0,52,0,0,0,
4,15,0,0,176,15,
0,0,228,15,0,0,
236,37,0,0,82,68,
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
4,0,0,0,2,0,
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
0,0,0,0,124,14,
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
71,78,164,0,0,0,
6,0,0,0,8,0,
0,0,152,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,3,3,
0,0,152,0,0,0,
1,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,12,12,
0,0,152,0,0,0,
2,0,0,0,0,0,
0,0,3,0,0,0,
1,0,0,0,3,1,
0,0,152,0,0,0,
3,0,0,0,0,0,
0,0,3,0,0,0,
1,0,0,0,12,4,
0,0,152,0,0,0,
4,0,0,0,0,0,
0,0,3,0,0,0,
2,0,0,0,3,1,
0,0,152,0,0,0,
5,0,0,0,0,0,
0,0,3,0,0,0,
2,0,0,0,12,0,
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
83,72,69,88,0,22,
0,0,80,0,0,0,
128,5,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,10,0,0,0,
89,0,0,4,70,142,
32,0,1,0,0,0,
1,0,0,0,90,0,
0,3,0,96,16,0,
2,0,0,0,88,24,
0,4,0,112,16,0,
2,0,0,0,85,85,
0,0,98,16,0,3,
50,16,16,0,0,0,
0,0,98,16,0,3,
194,16,16,0,0,0,
0,0,98,16,0,3,
18,16,16,0,1,0,
0,0,98,16,0,3,
66,16,16,0,1,0,
0,0,98,16,0,3,
18,16,16,0,2,0,
0,0,101,0,0,3,
242,32,16,0,0,0,
0,0,104,0,0,2,
12,0,0,0,49,0,
0,8,18,0,16,0,
0,0,0,0,58,128,
32,0,0,0,0,0,
9,0,0,0,1,64,
0,0,0,0,0,63,
52,0,0,7,34,0,
16,0,0,0,0,0,
58,16,16,0,0,0,
0,0,42,16,16,0,
0,0,0,0,29,0,
0,7,34,0,16,0,
0,0,0,0,1,64,
0,0,0,0,0,64,
26,0,16,0,0,0,
0,0,1,0,0,7,
18,0,16,0,0,0,
0,0,26,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
31,0,4,3,10,0,
16,0,0,0,0,0,
26,0,0,5,50,0,
16,0,0,0,0,0,
70,16,16,0,0,0,
0,0,14,0,0,10,
18,0,16,0,1,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,10,16,
16,0,2,0,0,0,
50,0,0,10,34,0,
16,0,1,0,0,0,
10,0,16,0,0,0,
0,0,10,128,32,0,
1,0,0,0,0,0,
0,0,1,64,0,0,
119,190,255,190,65,0,
0,5,34,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
0,0,0,7,34,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,1,64,0,0,
0,0,40,193,56,0,
0,7,66,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
42,16,16,0,1,0,
0,0,50,0,0,11,
34,0,16,0,1,0,
0,0,10,0,16,0,
0,0,0,0,10,128,
32,0,1,0,0,0,
0,0,0,0,26,0,
16,128,65,0,0,0,
1,0,0,0,56,0,
0,7,66,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,49,0,0,7,
130,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,1,64,
0,0,0,0,0,0,
1,0,0,7,130,0,
16,0,1,0,0,0,
58,0,16,0,1,0,
0,0,1,64,0,0,
0,0,128,63,26,0,
0,5,66,0,16,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
0,0,0,7,66,0,
16,0,1,0,0,0,
58,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,56,0,
0,7,18,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
42,16,16,0,1,0,
0,0,50,0,0,12,
242,0,16,0,2,0,
0,0,6,0,16,0,
1,0,0,0,2,64,
0,0,0,0,0,0,
0,0,128,63,0,0,
0,64,0,0,64,64,
166,10,16,0,1,0,
0,0,26,0,0,5,
242,0,16,0,2,0,
0,0,70,14,16,0,
2,0,0,0,56,0,
0,7,242,0,16,0,
2,0,0,0,70,14,
16,0,2,0,0,0,
6,16,16,0,2,0,
0,0,54,0,0,5,
194,0,16,0,0,0,
0,0,6,4,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,3,0,0,0,
102,10,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,4,0,
0,0,118,15,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,54,0,0,5,
82,0,16,0,0,0,
0,0,166,11,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,2,0,0,0,
70,0,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,5,0,
0,0,102,10,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,0,0,0,10,
242,0,16,0,6,0,
0,0,86,5,16,0,
1,0,0,0,2,64,
0,0,0,0,0,128,
0,0,128,191,0,0,
0,192,0,0,64,192,
56,0,0,8,242,0,
16,0,6,0,0,0,
70,14,16,128,129,0,
0,0,6,0,0,0,
6,16,16,0,1,0,
0,0,56,0,0,10,
242,0,16,0,7,0,
0,0,70,14,16,0,
6,0,0,0,2,64,
0,0,219,15,73,64,
219,15,73,64,219,15,
73,64,219,15,73,64,
56,0,0,10,242,0,
16,0,6,0,0,0,
70,14,16,0,6,0,
0,0,2,64,0,0,
146,10,134,63,146,10,
134,63,146,10,134,63,
146,10,134,63,77,0,
0,6,242,0,16,0,
8,0,0,0,0,208,
0,0,70,14,16,0,
7,0,0,0,77,0,
0,6,242,0,16,0,
9,0,0,0,0,208,
0,0,70,14,16,0,
6,0,0,0,56,0,
0,7,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
70,14,16,0,9,0,
0,0,56,0,0,7,
242,0,16,0,6,0,
0,0,70,14,16,0,
6,0,0,0,70,14,
16,0,7,0,0,0,
14,0,0,7,242,0,
16,0,6,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
6,0,0,0,51,0,
0,10,242,0,16,0,
6,0,0,0,70,14,
16,0,6,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,56,0,0,7,
114,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,86,5,
16,0,6,0,0,0,
50,0,0,9,114,0,
16,0,3,0,0,0,
70,2,16,0,3,0,
0,0,6,0,16,0,
6,0,0,0,70,2,
16,0,4,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,2,0,
0,0,166,10,16,0,
6,0,0,0,70,2,
16,0,3,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,5,0,
0,0,246,15,16,0,
6,0,0,0,70,2,
16,0,2,0,0,0,
50,0,0,12,242,0,
16,0,3,0,0,0,
6,0,16,0,1,0,
0,0,2,64,0,0,
0,0,128,64,0,0,
160,64,0,0,192,64,
0,0,224,64,166,10,
16,0,1,0,0,0,
26,0,0,5,242,0,
16,0,3,0,0,0,
70,14,16,0,3,0,
0,0,56,0,0,7,
242,0,16,0,3,0,
0,0,70,14,16,0,
3,0,0,0,6,16,
16,0,2,0,0,0,
54,0,0,5,130,0,
16,0,0,0,0,0,
10,0,16,0,3,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
4,0,0,0,118,15,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,54,0,
0,5,210,0,16,0,
0,0,0,0,86,14,
16,0,3,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,3,0,
0,0,70,0,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
5,0,0,0,102,10,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,7,0,0,0,
118,15,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
0,0,0,10,242,0,
16,0,8,0,0,0,
86,5,16,0,1,0,
0,0,2,64,0,0,
0,0,128,192,0,0,
160,192,0,0,192,192,
0,0,224,192,56,0,
0,8,242,0,16,0,
8,0,0,0,70,14,
16,128,129,0,0,0,
8,0,0,0,6,16,
16,0,1,0,0,0,
56,0,0,10,242,0,
16,0,9,0,0,0,
70,14,16,0,8,0,
0,0,2,64,0,0,
219,15,73,64,219,15,
73,64,219,15,73,64,
219,15,73,64,56,0,
0,10,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
2,64,0,0,146,10,
134,63,146,10,134,63,
146,10,134,63,146,10,
134,63,77,0,0,6,
242,0,16,0,10,0,
0,0,0,208,0,0,
70,14,16,0,9,0,
0,0,77,0,0,6,
242,0,16,0,11,0,
0,0,0,208,0,0,
70,14,16,0,8,0,
0,0,56,0,0,7,
242,0,16,0,10,0,
0,0,70,14,16,0,
10,0,0,0,70,14,
16,0,11,0,0,0,
56,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
9,0,0,0,14,0,
0,7,242,0,16,0,
8,0,0,0,70,14,
16,0,10,0,0,0,
70,14,16,0,8,0,
0,0,51,0,0,10,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,128,63,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,4,0,
0,0,6,0,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,3,0,
0,0,86,5,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,5,0,
0,0,166,10,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,7,0,
0,0,246,15,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
0,0,0,7,242,0,
16,0,3,0,0,0,
70,14,16,0,6,0,
0,0,70,14,16,0,
8,0,0,0,50,0,
0,12,242,0,16,0,
4,0,0,0,6,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,65,0,0,16,65,
0,0,32,65,0,0,
48,65,166,10,16,0,
1,0,0,0,26,0,
0,5,242,0,16,0,
4,0,0,0,70,14,
16,0,4,0,0,0,
56,0,0,7,242,0,
16,0,4,0,0,0,
70,14,16,0,4,0,
0,0,6,16,16,0,
2,0,0,0,54,0,
0,5,210,0,16,0,
0,0,0,0,6,9,
16,0,4,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,4,0,
0,0,70,0,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
5,0,0,0,102,10,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,6,0,0,0,
118,15,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
54,0,0,5,18,0,
16,0,0,0,0,0,
58,0,16,0,4,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
7,0,0,0,70,0,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,0,0,
0,10,242,0,16,0,
8,0,0,0,86,5,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,193,0,0,16,193,
0,0,32,193,0,0,
48,193,56,0,0,8,
242,0,16,0,8,0,
0,0,70,14,16,128,
129,0,0,0,8,0,
0,0,6,16,16,0,
1,0,0,0,56,0,
0,10,242,0,16,0,
9,0,0,0,70,14,
16,0,8,0,0,0,
2,64,0,0,219,15,
73,64,219,15,73,64,
219,15,73,64,219,15,
73,64,56,0,0,10,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,2,64,
0,0,146,10,134,63,
146,10,134,63,146,10,
134,63,146,10,134,63,
77,0,0,6,242,0,
16,0,10,0,0,0,
0,208,0,0,70,14,
16,0,9,0,0,0,
77,0,0,6,242,0,
16,0,11,0,0,0,
0,208,0,0,70,14,
16,0,8,0,0,0,
56,0,0,7,242,0,
16,0,10,0,0,0,
70,14,16,0,10,0,
0,0,70,14,16,0,
11,0,0,0,56,0,
0,7,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
70,14,16,0,9,0,
0,0,14,0,0,7,
242,0,16,0,8,0,
0,0,70,14,16,0,
10,0,0,0,70,14,
16,0,8,0,0,0,
51,0,0,10,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,50,0,
0,9,114,0,16,0,
2,0,0,0,70,2,
16,0,4,0,0,0,
6,0,16,0,8,0,
0,0,70,2,16,0,
2,0,0,0,50,0,
0,9,114,0,16,0,
2,0,0,0,70,2,
16,0,5,0,0,0,
86,5,16,0,8,0,
0,0,70,2,16,0,
2,0,0,0,50,0,
0,9,114,0,16,0,
2,0,0,0,70,2,
16,0,6,0,0,0,
166,10,16,0,8,0,
0,0,70,2,16,0,
2,0,0,0,50,0,
0,9,114,0,16,0,
2,0,0,0,70,2,
16,0,7,0,0,0,
246,15,16,0,8,0,
0,0,70,2,16,0,
2,0,0,0,0,0,
0,7,242,0,16,0,
3,0,0,0,70,14,
16,0,3,0,0,0,
70,14,16,0,8,0,
0,0,50,0,0,12,
242,0,16,0,4,0,
0,0,6,0,16,0,
1,0,0,0,2,64,
0,0,0,0,64,65,
0,0,80,65,0,0,
96,65,0,0,112,65,
166,10,16,0,1,0,
0,0,26,0,0,5,
242,0,16,0,4,0,
0,0,70,14,16,0,
4,0,0,0,56,0,
0,7,242,0,16,0,
4,0,0,0,70,14,
16,0,4,0,0,0,
6,16,16,0,2,0,
0,0,54,0,0,5,
194,0,16,0,0,0,
0,0,6,4,16,0,
4,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,5,0,0,0,
102,10,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,6,0,
0,0,118,15,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,54,0,0,5,
82,0,16,0,0,0,
0,0,166,11,16,0,
4,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,4,0,0,0,
70,0,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,7,0,
0,0,102,10,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,0,0,0,10,
242,0,16,0,8,0,
0,0,86,5,16,0,
1,0,0,0,2,64,
0,0,0,0,64,193,
0,0,80,193,0,0,
96,193,0,0,112,193,
56,0,0,8,242,0,
16,0,8,0,0,0,
70,14,16,128,129,0,
0,0,8,0,0,0,
6,16,16,0,1,0,
0,0,56,0,0,10,
242,0,16,0,9,0,
0,0,70,14,16,0,
8,0,0,0,2,64,
0,0,219,15,73,64,
219,15,73,64,219,15,
73,64,219,15,73,64,
56,0,0,10,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,2,64,0,0,
146,10,134,63,146,10,
134,63,146,10,134,63,
146,10,134,63,77,0,
0,6,242,0,16,0,
10,0,0,0,0,208,
0,0,70,14,16,0,
9,0,0,0,77,0,
0,6,242,0,16,0,
11,0,0,0,0,208,
0,0,70,14,16,0,
8,0,0,0,56,0,
0,7,242,0,16,0,
10,0,0,0,70,14,
16,0,10,0,0,0,
70,14,16,0,11,0,
0,0,56,0,0,7,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,70,14,
16,0,9,0,0,0,
14,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,10,0,
0,0,70,14,16,0,
8,0,0,0,51,0,
0,10,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,50,0,0,9,
114,0,16,0,2,0,
0,0,70,2,16,0,
5,0,0,0,6,0,
16,0,8,0,0,0,
70,2,16,0,2,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,70,2,16,0,
6,0,0,0,86,5,
16,0,8,0,0,0,
70,2,16,0,2,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,70,2,16,0,
4,0,0,0,166,10,
16,0,8,0,0,0,
70,2,16,0,2,0,
0,0,50,0,0,9,
114,0,16,0,2,0,
0,0,70,2,16,0,
7,0,0,0,246,15,
16,0,8,0,0,0,
70,2,16,0,2,0,
0,0,0,0,0,7,
242,0,16,0,3,0,
0,0,70,14,16,0,
3,0,0,0,70,14,
16,0,8,0,0,0,
50,0,0,12,242,0,
16,0,4,0,0,0,
6,0,16,0,1,0,
0,0,2,64,0,0,
0,0,128,65,0,0,
136,65,0,0,144,65,
0,0,152,65,166,10,
16,0,1,0,0,0,
26,0,0,5,242,0,
16,0,4,0,0,0,
70,14,16,0,4,0,
0,0,56,0,0,7,
242,0,16,0,4,0,
0,0,70,14,16,0,
4,0,0,0,6,16,
16,0,2,0,0,0,
54,0,0,5,130,0,
16,0,0,0,0,0,
10,0,16,0,4,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
5,0,0,0,118,15,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,54,0,
0,5,210,0,16,0,
0,0,0,0,86,14,
16,0,4,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,4,0,
0,0,70,0,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
6,0,0,0,102,10,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,7,0,0,0,
118,15,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
0,0,0,10,242,0,
16,0,8,0,0,0,
86,5,16,0,1,0,
0,0,2,64,0,0,
0,0,128,193,0,0,
136,193,0,0,144,193,
0,0,152,193,56,0,
0,8,242,0,16,0,
8,0,0,0,70,14,
16,128,129,0,0,0,
8,0,0,0,6,16,
16,0,1,0,0,0,
56,0,0,10,242,0,
16,0,9,0,0,0,
70,14,16,0,8,0,
0,0,2,64,0,0,
219,15,73,64,219,15,
73,64,219,15,73,64,
219,15,73,64,56,0,
0,10,242,0,16,0,
8,0,0,0,70,14,
16,0,8,0,0,0,
2,64,0,0,146,10,
134,63,146,10,134,63,
146,10,134,63,146,10,
134,63,77,0,0,6,
242,0,16,0,10,0,
0,0,0,208,0,0,
70,14,16,0,9,0,
0,0,77,0,0,6,
242,0,16,0,11,0,
0,0,0,208,0,0,
70,14,16,0,8,0,
0,0,56,0,0,7,
242,0,16,0,10,0,
0,0,70,14,16,0,
10,0,0,0,70,14,
16,0,11,0,0,0,
56,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
9,0,0,0,14,0,
0,7,242,0,16,0,
8,0,0,0,70,14,
16,0,10,0,0,0,
70,14,16,0,8,0,
0,0,51,0,0,10,
242,0,16,0,8,0,
0,0,70,14,16,0,
8,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,128,63,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,5,0,
0,0,6,0,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,4,0,
0,0,86,5,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,6,0,
0,0,166,10,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
50,0,0,9,114,0,
16,0,2,0,0,0,
70,2,16,0,7,0,
0,0,246,15,16,0,
8,0,0,0,70,2,
16,0,2,0,0,0,
0,0,0,7,242,0,
16,0,3,0,0,0,
70,14,16,0,3,0,
0,0,70,14,16,0,
8,0,0,0,50,0,
0,12,242,0,16,0,
4,0,0,0,6,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
160,65,0,0,168,65,
0,0,176,65,0,0,
184,65,166,10,16,0,
1,0,0,0,26,0,
0,5,242,0,16,0,
4,0,0,0,70,14,
16,0,4,0,0,0,
56,0,0,7,242,0,
16,0,4,0,0,0,
70,14,16,0,4,0,
0,0,6,16,16,0,
2,0,0,0,54,0,
0,5,210,0,16,0,
0,0,0,0,6,9,
16,0,4,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
210,0,16,0,1,0,
0,0,70,0,16,0,
0,0,0,0,198,121,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
4,0,0,0,102,10,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,5,0,0,0,
118,15,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
54,0,0,5,18,0,
16,0,0,0,0,0,
58,0,16,0,4,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,0,0,
0,10,242,0,16,0,
6,0,0,0,86,5,
16,0,1,0,0,0,
2,64,0,0,0,0,
160,193,0,0,168,193,
0,0,176,193,0,0,
184,193,56,0,0,8,
242,0,16,0,6,0,
0,0,70,14,16,128,
129,0,0,0,6,0,
0,0,6,16,16,0,
1,0,0,0,56,0,
0,10,242,0,16,0,
7,0,0,0,70,14,
16,0,6,0,0,0,
2,64,0,0,219,15,
73,64,219,15,73,64,
219,15,73,64,219,15,
73,64,56,0,0,10,
242,0,16,0,6,0,
0,0,70,14,16,0,
6,0,0,0,2,64,
0,0,146,10,134,63,
146,10,134,63,146,10,
134,63,146,10,134,63,
77,0,0,6,242,0,
16,0,8,0,0,0,
0,208,0,0,70,14,
16,0,7,0,0,0,
77,0,0,6,242,0,
16,0,9,0,0,0,
0,208,0,0,70,14,
16,0,6,0,0,0,
56,0,0,7,242,0,
16,0,8,0,0,0,
70,14,16,0,8,0,
0,0,70,14,16,0,
9,0,0,0,56,0,
0,7,242,0,16,0,
6,0,0,0,70,14,
16,0,6,0,0,0,
70,14,16,0,7,0,
0,0,14,0,0,7,
242,0,16,0,6,0,
0,0,70,14,16,0,
8,0,0,0,70,14,
16,0,6,0,0,0,
51,0,0,10,242,0,
16,0,6,0,0,0,
70,14,16,0,6,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,128,63,50,0,
0,9,114,0,16,0,
1,0,0,0,134,3,
16,0,1,0,0,0,
6,0,16,0,6,0,
0,0,70,2,16,0,
2,0,0,0,50,0,
0,9,114,0,16,0,
1,0,0,0,70,2,
16,0,4,0,0,0,
86,5,16,0,6,0,
0,0,70,2,16,0,
1,0,0,0,50,0,
0,9,114,0,16,0,
1,0,0,0,70,2,
16,0,5,0,0,0,
166,10,16,0,6,0,
0,0,70,2,16,0,
1,0,0,0,50,0,
0,9,114,0,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
246,15,16,0,6,0,
0,0,70,2,16,0,
1,0,0,0,0,0,
0,7,242,0,16,0,
1,0,0,0,70,14,
16,0,3,0,0,0,
70,14,16,0,6,0,
0,0,0,0,0,7,
50,0,16,0,1,0,
0,0,230,10,16,0,
1,0,0,0,70,0,
16,0,1,0,0,0,
0,0,0,7,130,0,
16,0,0,0,0,0,
26,0,16,0,1,0,
0,0,10,0,16,0,
1,0,0,0,14,0,
0,7,114,32,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
246,15,16,0,0,0,
0,0,54,0,0,5,
130,32,16,0,0,0,
0,0,1,64,0,0,
0,0,128,63,18,0,
0,1,13,0,4,3,
1,64,0,0,255,255,
255,255,54,0,0,8,
242,32,16,0,0,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,21,0,
0,1,62,0,0,1,
83,84,65,84,148,0,
0,0,170,0,0,0,
12,0,0,0,0,0,
0,0,6,0,0,0,
125,0,0,0,0,0,
0,0,2,0,0,0,
2,0,0,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,24,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
14,0,0,0,0,0,
0,0,9,0,0,0,
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
class CrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDef : public ShaderDef
{
public:
	CrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDef() : ShaderDef{}
	{
		Name = "crt-royale-mask-resize-horizontal";
		VertexByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchCrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDefs::sFragmentByteCode);
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
