/*
ShaderGlass shader blurs\blur5fast-vertical imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/blurs/blur5fast-vertical.slang
See original file for full credits and usage license with excerpts below. 
This file is auto-generated, do not modify directly.

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
/////////////////////////////  SETTINGS MANAGEMENT  ////////////////////////////
//  PASS SETTINGS:
//  gamma-management.h needs to know what kind of pipeline we're using and
//  what pass this is in that pipeline.  This will become obsolete if/when we
//  can #define things like this in the .cgp preset file.
//#define GAMMA_ENCODE_EVERY_FBO
//#define FIRST_PASS
//#define LAST_PASS
//#define SIMULATE_CRT_ON_LCD
//#define SIMULATE_GBA_ON_LCD
//#define SIMULATE_LCD_ON_CRT
//#define SIMULATE_GBA_ON_CRT
///////////////////////////////  VERTEX INCLUDES  ///////////////////////////////
// compatibility macros for transparently converting HLSLisms into GLSLisms
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
//////////////////////////////////  INCLUDES  //////////////////////////////////
//#include "../include/gamma-management.h
//#include "../include/blur-functions.h
//  Get the uv sample distance between output pixels.  Blurs are not generic
//  Gaussian resizers, and correct blurs require:
//  1.) OutputSize.xy == SourceSize.xy * 2^m, where m is an integer <= 0.
//  2.) mipmap_inputN = "true" for this pass in .cgp preset if m != 0
//  3.) filter_linearN = "true" except for 1x scale nearest neighbor blurs
//  Gaussian resizers would upsize using the distance between input texels
//  (not output pixels), but we avoid this and consistently blur at the
//  destination size.  Otherwise, combining statically calculated weights
//  with bilinear sample exploitation would result in terrible artifacts.
//  This blur is vertical-only, so zero out the horizontal offset:
///////////////////////////////  FRAGMENT SHADER  //////////////////////////////
/////////////////////////////  FRAGMENT INCLUDES  /////////////////////////////
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
//  Encode and output the blurred image:

*/

#pragma once

namespace RetroArchBlursBlur5fastVerticalShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,75,64,
207,68,175,42,136,2,
134,153,105,2,120,2,
250,219,1,0,0,0,
156,5,0,0,5,0,
0,0,52,0,0,0,
192,2,0,0,12,3,
0,0,124,3,0,0,
0,5,0,0,82,68,
69,70,132,2,0,0,
2,0,0,0,136,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
92,2,0,0,82,68,
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
0,0,1,0,0,0,
184,0,0,0,64,0,
0,0,0,0,0,0,
0,0,0,0,128,0,
0,0,4,0,0,0,
24,1,0,0,64,0,
0,0,0,0,0,0,
0,0,0,0,224,0,
0,0,0,0,0,0,
64,0,0,0,2,0,
0,0,244,0,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,103,108,111,98,
97,108,95,77,86,80,
0,102,108,111,97,116,
52,120,52,0,2,0,
3,0,4,0,4,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
235,0,0,0,184,1,
0,0,0,0,0,0,
16,0,0,0,2,0,
0,0,212,1,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,248,1,0,0,
16,0,0,0,16,0,
0,0,0,0,0,0,
212,1,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
12,2,0,0,32,0,
0,0,16,0,0,0,
2,0,0,0,212,1,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,30,2,
0,0,48,0,0,0,
4,0,0,0,0,0,
0,0,56,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,112,97,114,97,
109,115,95,83,111,117,
114,99,101,83,105,122,
101,0,102,108,111,97,
116,52,0,171,171,171,
1,0,3,0,1,0,
4,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,202,1,0,0,
112,97,114,97,109,115,
95,79,114,105,103,105,
110,97,108,83,105,122,
101,0,112,97,114,97,
109,115,95,79,117,116,
112,117,116,83,105,122,
101,0,112,97,114,97,
109,115,95,70,114,97,
109,101,67,111,117,110,
116,0,100,119,111,114,
100,0,171,171,0,0,
19,0,1,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
48,2,0,0,77,105,
99,114,111,115,111,102,
116,32,40,82,41,32,
72,76,83,76,32,83,
104,97,100,101,114,32,
67,111,109,112,105,108,
101,114,32,49,48,46,
49,0,73,83,71,78,
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
79,83,71,78,104,0,
0,0,3,0,0,0,
8,0,0,0,80,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
3,12,0,0,80,0,
0,0,1,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
12,3,0,0,89,0,
0,0,0,0,0,0,
1,0,0,0,3,0,
0,0,1,0,0,0,
15,0,0,0,84,69,
88,67,79,79,82,68,
0,83,86,95,80,111,
115,105,116,105,111,110,
0,171,171,171,83,72,
69,88,124,1,0,0,
80,0,1,0,95,0,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,0,0,0,0,
4,0,0,0,89,0,
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
0,0,103,0,0,4,
242,32,16,0,1,0,
0,0,1,0,0,0,
104,0,0,2,1,0,
0,0,14,0,0,9,
18,0,16,0,0,0,
0,0,26,128,32,0,
1,0,0,0,0,0,
0,0,26,128,32,0,
1,0,0,0,2,0,
0,0,14,0,0,8,
130,32,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,26,128,
32,0,1,0,0,0,
0,0,0,0,54,0,
0,5,50,32,16,0,
0,0,0,0,70,16,
16,0,1,0,0,0,
54,0,0,5,66,32,
16,0,0,0,0,0,
1,64,0,0,0,0,
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
16,0,1,0,0,0,
246,31,16,0,0,0,
0,0,70,142,32,0,
0,0,0,0,3,0,
0,0,70,14,16,0,
0,0,0,0,62,0,
0,1,83,84,65,84,
148,0,0,0,9,0,
0,0,1,0,0,0,
0,0,0,0,5,0,
0,0,6,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
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
0,0,0,0,0,0,
0,0
};


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,86,41,
181,83,247,48,219,80,
7,108,0,208,27,51,
196,79,1,0,0,0,
248,3,0,0,5,0,
0,0,52,0,0,0,
248,0,0,0,68,1,
0,0,120,1,0,0,
92,3,0,0,82,68,
69,70,188,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
147,0,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
124,0,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
1,0,0,0,1,0,
0,0,140,0,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,2,0,
0,0,1,0,0,0,
13,0,0,0,95,83,
111,117,114,99,101,95,
115,97,109,112,108,101,
114,0,83,111,117,114,
99,101,0,77,105,99,
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
0,0,3,3,0,0,
56,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,12,0,0,
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
69,88,220,1,0,0,
80,0,0,0,119,0,
0,0,106,8,0,1,
90,0,0,3,0,96,
16,0,2,0,0,0,
88,24,0,4,0,112,
16,0,2,0,0,0,
85,85,0,0,98,16,
0,3,50,16,16,0,
0,0,0,0,98,16,
0,3,194,16,16,0,
0,0,0,0,101,0,
0,3,242,32,16,0,
0,0,0,0,104,0,
0,2,2,0,0,0,
50,0,0,13,50,0,
16,0,0,0,0,0,
230,26,16,128,65,0,
0,0,0,0,0,0,
2,64,0,0,160,117,
150,63,160,117,150,63,
0,0,0,0,0,0,
0,0,70,16,16,0,
0,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,1,0,
0,0,70,16,16,0,
0,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,50,0,0,12,
114,0,16,0,0,0,
0,0,70,2,16,0,
0,0,0,0,2,64,
0,0,10,93,57,63,
10,93,57,63,10,93,
57,63,0,0,0,0,
70,2,16,0,1,0,
0,0,50,0,0,12,
50,0,16,0,1,0,
0,0,230,26,16,0,
0,0,0,0,2,64,
0,0,160,117,150,63,
160,117,150,63,0,0,
0,0,0,0,0,0,
70,16,16,0,0,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
1,0,0,0,70,0,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,12,114,0,16,0,
0,0,0,0,70,2,
16,0,1,0,0,0,
2,64,0,0,10,93,
57,63,10,93,57,63,
10,93,57,63,0,0,
0,0,70,2,16,0,
0,0,0,0,56,0,
0,10,114,32,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
2,64,0,0,42,35,
209,62,42,35,209,62,
42,35,209,62,0,0,
0,0,54,0,0,5,
130,32,16,0,0,0,
0,0,1,64,0,0,
0,0,128,63,62,0,
0,1,83,84,65,84,
148,0,0,0,10,0,
0,0,2,0,0,0,
0,0,0,0,3,0,
0,0,5,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
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
0,0,0,0,0,0,
0,0
};

}

namespace RetroArch
{
class BlursBlur5fastVerticalShaderDef : public ShaderDef
{
public:
	BlursBlur5fastVerticalShaderDef() : ShaderDef{}
	{
		Name = "blur5fast-vertical";
		VertexByteCode = RetroArchBlursBlur5fastVerticalShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchBlursBlur5fastVerticalShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchBlursBlur5fastVerticalShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchBlursBlur5fastVerticalShaderDefs::sFragmentByteCode);
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
