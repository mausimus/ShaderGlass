/*
ShaderGlass shader anti-aliasing-shaders-smaa\smaa-pass2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/anti-aliasing/shaders/smaa/smaa-pass2.slang
See original file for full credits and usage license with excerpts below. 
This file is auto-generated, do not modify directly.

// SPDX-License-Identifier: Unlicense
//-----------------------------------------------------------------------------
// Neighborhood Blending Shader (Third Pass)
*
* Copyright (C) 2013 Jorge Jimenez (jorge@iryoku.com)
* Copyright (C) 2013 Jose I. Echevarria (joseignacioechevarria@gmail.com)
* Copyright (C) 2013 Belen Masia (bmasia@unizar.es)
* Copyright (C) 2013 Fernando Navarro (fernandn@microsoft.com)
* Copyright (C) 2013 Diego Gutierrez (diegog@unizar.es)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to
* do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software. As clarification, there
* is no requirement that the copyright notice and permission be included in
* binary distributions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.

*
*                  _______  ___  ___       ___           ___
*                 /       ||   \/   |     /   \         /   \
*                |   (---- |  \  /  |    /  ^  \       /  ^  \
*                 \   \    |  |\/|  |   /  /_\  \     /  /_\  \
*              ----)   |   |  |  |  |  /  _____  \   /  _____  \
*             |_______/    |__|  |__| /__/     \__\ /__/     \__\
*
*                               E N H A N C E D
*       S U B P I X E L   M O R P H O L O G I C A L   A N T I A L I A S I N G
*
*                         http://www.iryoku.com/smaa/
*
* Hi, welcome aboard!
*
* Here you'll find instructions to get the shader up and running as fast as
* possible.
*
* IMPORTANTE NOTICE: when updating, remember to update both this file and the
* precomputed textures! They may change from version to version.
*
* The shader has three passes, chained together as follows:
*
*                           |input|------------------·
*                              v                     |
*                    [ SMAA*EdgeDetection ]          |
*                              v                     |
*                          |edgesTex|                |
*                              v                     |
*              [ SMAABlendingWeightCalculation ]     |
*                              v                     |
*                          |blendTex|                |
*                              v                     |
*                [ SMAANeighborhoodBlending ] <------·
*                              v
*                           |output|
*
* Note that each [pass] has its own vertex and pixel shader. Remember to use
* oversized triangles instead of quads to avoid overshading along the
* diagonal.
*
* You've three edge detection methods to choose from: luma, color or depth.
* They represent different quality/performance and anti-aliasing/sharpness
* tradeoffs, so our recommendation is for you to choose the one that best
* suits your particular scenario:
*
* - Depth edge detection is usually the fastest but it may miss some edges.
*
* - Luma edge detection is usually more expensive than depth edge detection,
*   but catches visible edges that depth edge detection can miss.
*
* - Color edge detection is usually the most expensive one but catches
*   chroma-only edges.
*
* For quickstarters: just use luma edge detection.
*
* The general advice is to not rush the integration process and ensure each
* step is done correctly (don't try to integrate SMAA T2x with predicated edge
* detection from the start!). Ok then, let's go!
*
*  1. The first step is to create two RGBA temporal render targets for holding
*     |edgesTex| and |blendTex|.
*
*     In DX10 or DX11, you can use a RG render target for the edges texture.
*     In the case of NVIDIA GPUs, using RG render targets seems to actually be
*     slower.
*
*     On the Xbox 360, you can use the same render target for resolving both
*     |edgesTex| and |blendTex|, as they aren't needed simultaneously.
*
*  2. Both temporal render targets |edgesTex| and |blendTex| must be cleared
*     each frame. Do not forget to clear the alpha channel!
*
*  3. The next step is loading the two supporting precalculated textures,
*     'areaTex' and 'searchTex'. You'll find them in the 'Textures' folder as
*     C++ headers, and also as regular DDS files. They'll be needed for the
*     'SMAABlendingWeightCalculation' pass.
*
*     If you use the C++ headers, be sure to load them in the format specified
*     inside of them.
*
*     You can also compress 'areaTex' and 'searchTex' using BC5 and BC4
*     respectively, if you have that option in your content processor pipeline.
*     When compressing then, you get a non-perceptible quality decrease, and a
*     marginal performance increase.
*
*  4. All samplers must be set to linear filtering and clamp.
*
*     After you get the technique working, remember that 64-bit inputs have
*     half-rate linear filtering on GCN.
*
*     If SMAA is applied to 64-bit color buffers, switching to point filtering
*     when accesing them will increase the performance. Search for
*     'SMAASamplePoint' to see which textures may benefit from point
*     filtering, and where (which is basically the color input in the edge
*     detection and resolve passes).
*
*  5. All texture reads and buffer writes must be non-sRGB, with the exception
*     of the input read and the output write in
*     'SMAANeighborhoodBlending' (and only in this pass!). If sRGB reads in
*     this last pass are not possible, the technique will work anyway, but
*     will perform antialiasing in gamma space.
*
*     IMPORTANT: for best results the input read for the color/luma edge
*     detection should *NOT* be sRGB.
*
*  6. Before including SMAA.h you'll have to setup the render target metrics,
*     the target and any optional configuration defines. Optionally you can
*     use a preset.
*
*     You have the following targets available:
*         SMAA_HLSL_3
*         SMAA_HLSL_4
*         SMAA_HLSL_4_1
*         SMAA_GLSL_3 *
*         SMAA_GLSL_4 *
*
*         * (See SMAA_INCLUDE_VS and SMAA_INCLUDE_PS below).
*
*     And four presets:
*         SMAA_PRESET_LOW          (%60 of the quality)
*         SMAA_PRESET_MEDIUM       (%80 of the quality)
*         SMAA_PRESET_HIGH         (%95 of the quality)
*         SMAA_PRESET_ULTRA        (%99 of the quality)
*
*     For example:
*         #define SMAA_RT_METRICS float4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0)
*         #define SMAA_HLSL_4
*         #define SMAA_PRESET_HIGH
*         #include "SMAA.h
*
*     Note that SMAA_RT_METRICS doesn't need to be a macro, it can be a
*     uniform variable. The code is designed to minimize the impact of not
*     using a constant value, but it is still better to hardcode it.
*
*     Depending on how you encoded 'areaTex' and 'searchTex', you may have to
*     add (and customize) the following defines before including SMAA.h:
*          #define SMAA_AREATEX_SELECT(sample) sample.rg
*          #define SMAA_SEARCHTEX_SELECT(sample) sample.r
*
*     If your engine is already using porting macros, you can define
*     SMAA_CUSTOM_SL, and define the porting functions by yourself.
*
*  7. Then, you'll have to setup the passes as indicated in the scheme above.
*     You can take a look into SMAA.fx, to see how we did it for our demo.
*     Checkout the function wrappers, you may want to copy-paste them!
*
*  8. It's recommended to validate the produced |edgesTex| and |blendTex|.
*     You can use a screenshot from your engine to compare the |edgesTex|
*     and |blendTex| produced inside of the engine with the results obtained
*     with the reference demo.
*
*  9. After you get the last pass to work, it's time to optimize. You'll have
*     to initialize a stencil buffer in the first pass (discard is already in
*     the code), then mask execution by using it the second pass. The last
*     pass should be executed in all pixels.
*
*
* After this point you can choose to enable predicated thresholding,
* temporal supersampling and motion blur integration:
*
* a) If you want to use predicated thresholding, take a look into
*    SMAA_PREDICATION; you'll need to pass an extra texture in the edge
*    detection pass.
*
* b) If you want to enable temporal supersampling (SMAA T2x):
*
* 1. The first step is to render using subpixel jitters. I won't go into
*    detail, but it's as simple as moving each vertex position in the
*    vertex shader, you can check how we do it in our DX10 demo.
*
* 2. Then, you must setup the temporal resolve. You may want to take a look
*    into SMAAResolve for resolving 2x modes. After you get it working, you'll
*    probably see ghosting everywhere. But fear not, you can enable the
*    CryENGINE temporal reprojection by setting the SMAA_REPROJECTION macro.
*    Check out SMAA_DECODE_VELOCITY if your velocity buffer is encoded.
*
* 3. The next step is to apply SMAA to each subpixel jittered frame, just as
*    done for 1x.
*
* 4. At this point you should already have something usable, but for best
*    results the proper area textures must be set depending on current jitter.
*    For this, the parameter 'subsampleIndices' of
*    'SMAABlendingWeightCalculationPS' must be set as follows, for our T2x
*    mode:
*
*    @SUBSAMPLE_INDICES
*
*    | S# |  Camera Jitter   |  subsampleIndices    |
*    +----+------------------+---------------------+
*    |  0 |  ( 0.25, -0.25)  |  float4(1, 1, 1, 0)  |
*    |  1 |  (-0.25,  0.25)  |  float4(2, 2, 2, 0)  |
*
*    These jitter positions assume a bottom-to-top y axis. S# stands for the
*    sample number.
*
* More information about temporal supersampling here:
*    http://iryoku.com/aacourse/downloads/13-Anti-Aliasing-Methods-in-CryENGINE-3.pdf
*
* c) If you want to enable spatial multisampling (SMAA S2x):
*
* 1. The scene must be rendered using MSAA 2x. The MSAA 2x buffer must be
*    created with:
*      - DX10:     see below (*)
*      - DX10.1:   D3D10_STANDARD_MULTISAMPLE_PATTERN or
*      - DX11:     D3D11_STANDARD_MULTISAMPLE_PATTERN
*
*    This allows to ensure that the subsample order matches the table in
*    @SUBSAMPLE_INDICES.
*
*    (*) In the case of DX10, we refer the reader to:
*      - SMAA::detectMSAAOrder and
*      - SMAA::msaaReorder
*
*    These functions allow to match the standard multisample patterns by
*    detecting the subsample order for a specific GPU, and reordering
*    them appropriately.
*
* 2. A shader must be run to output each subsample into a separate buffer
*    (DX10 is required). You can use SMAASeparate for this purpose, or just do
*    it in an existing pass (for example, in the tone mapping pass, which has
*    the advantage of feeding tone mapped subsamples to SMAA, which will yield
*    better results).
*
* 3. The full SMAA 1x pipeline must be run for each separated buffer, storing
*    the results in the final buffer. The second run should alpha blend with
*    the existing final buffer using a blending factor of 0.5.
*    'subsampleIndices' must be adjusted as in the SMAA T2x case (see point
*    b).
*
* d) If you want to enable temporal supersampling on top of SMAA S2x
*    (which actually is SMAA 4x):
*
* 1. SMAA 4x consists on temporally jittering SMAA S2x, so the first step is
*    to calculate SMAA S2x for current frame. In this case, 'subsampleIndices'
*    must be set as follows:
*
*    | F# | S# |   Camera Jitter    |    Net Jitter     |   subsampleIndices   |
*    +----+----+--------------------+-------------------+----------------------+
*    |  0 |  0 |  ( 0.125,  0.125)  |  ( 0.375, -0.125) |  float4(5, 3, 1, 3)  |
*    |  0 |  1 |  ( 0.125,  0.125)  |  (-0.125,  0.375) |  float4(4, 6, 2, 3)  |
*    +----+----+--------------------+-------------------+----------------------+
*    |  1 |  2 |  (-0.125, -0.125)  |  ( 0.125, -0.375) |  float4(3, 5, 1, 4)  |
*    |  1 |  3 |  (-0.125, -0.125)  |  (-0.375,  0.125) |  float4(6, 4, 2, 4)  |
*
*    These jitter positions assume a bottom-to-top y axis. F# stands for the
*    frame number. S# stands for the sample number.
*
* 2. After calculating SMAA S2x for current frame (with the new subsample
*    indices), previous frame must be reprojected as in SMAA T2x mode (see
*    point b).
*
* e) If motion blur is used, you may want to do the edge detection pass
*    together with motion blur. This has two advantages:
*
* 1. Pixels under heavy motion can be omitted from the edge detection process.
*    For these pixels we can just store "no edge", as motion blur will take
*    care of them.
* 2. The center pixel tap is reused.
*
* Note that in this case depth testing should be used instead of stenciling,
* as we have to write all the pixels in the motion blur pass.
*
* That's it!

//-----------------------------------------------------------------------------
// SMAA Presets
*
* Note that if you use one of these presets, the following configuration
* macros will be ignored if set in the "Configurable Defines" section.

//-----------------------------------------------------------------------------
// Configurable Defines
*
* SMAA_THRESHOLD specifies the threshold or sensitivity to edges.
* Lowering this value you will be able to detect more edges at the expense of
* performance.
*
* Range: [0, 0.5]
*   0.1 is a reasonable value, and allows to catch most visible edges.
*   0.05 is a rather overkill value, that allows to catch 'em all.
*
*   If temporal supersampling is used, 0.2 could be a reasonable value, as low
*   contrast edges are properly filtered by just 2x.

*
* SMAA_DEPTH_THRESHOLD specifies the threshold for depth edge detection.
*
* Range: depends on the depth range of the scene.

*
* SMAA_MAX_SEARCH_STEPS specifies the maximum steps performed in the
* horizontal/vertical pattern searches, at each side of the pixel.
*
* In number of pixels, it's actually the double. So the maximum line length
* perfectly handled by, for example 16, is 64 (by perfectly, we meant that
* longer lines won't look as good, but still antialiased).
*
* Range: [0, 112]

*
* SMAA_MAX_SEARCH_STEPS_DIAG specifies the maximum steps performed in the
* diagonal pattern searches, at each side of the pixel. In this case we jump
* one pixel at time, instead of two.
*
* Range: [0, 20]
*
* On high-end machines it is cheap (between a 0.8x and 0.9x slower for 16
* steps), but it can have a significant impact on older machines.
*
* Define SMAA_DISABLE_DIAG_DETECTION to disable diagonal processing.

*
* SMAA_CORNER_ROUNDING specifies how much sharp corners will be rounded.
*
* Range: [0, 100]
*
* Define SMAA_DISABLE_CORNER_DETECTION to disable corner processing.

*
* If there is an neighbor edge that has SMAA_LOCAL_CONTRAST_FACTOR times
* bigger contrast than current edge, current edge will be discarded.
*
* This allows to eliminate spurious crossing edges, and is based on the fact
* that, if there is too much contrast in a direction, that will hide
* perceptually contrast in the other neighbors.

*
* Predicated thresholding allows to better preserve texture details and to
* improve performance, by decreasing the number of detected edges using an
* additional buffer like the light accumulation buffer, object ids or even the
* depth buffer (the depth buffer usage may be limited to indoor or short range
* scenes).
*
* It locally decreases the luma or color threshold if an edge is found in an
* additional buffer (so the global threshold can be higher).
*
* This method was developed by Playstation EDGE MLAA team, and used in
* Killzone 3, by using the light accumulation buffer. More information here:
*     http://iryoku.com/aacourse/downloads/06-MLAA-on-PS3.pptx

*
* Threshold to be used in the additional predication buffer.
*
* Range: depends on the input, so you'll have to find the magic number that
* works for you.

*
* How much to scale the global threshold used for luma or color edge
* detection when using predication.
*
* Range: [1, 5]

*
* How much to locally decrease the threshold.
*
* Range: [0, 1]

*
* Temporal reprojection allows to remove ghosting artifacts when using
* temporal supersampling. We use the CryEngine 3 method which also introduces
* velocity weighting. This feature is of extreme importance for totally
* removing ghosting. More information here:
*    http://iryoku.com/aacourse/downloads/13-Anti-Aliasing-Methods-in-CryENGINE-3.pdf
*
* Note that you'll need to setup a velocity buffer for enabling reprojection.
* For static geometry, saving the previous depth buffer is a viable
* alternative.

*
* SMAA_REPROJECTION_WEIGHT_SCALE controls the velocity weighting. It allows to
* remove ghosting trails behind the moving object, which are not removed by
* just using reprojection. Using low values will exhibit ghosting, while using
* high values will disable temporal supersampling under motion.
*
* Behind the scenes, velocity weighting removes temporal supersampling when
* the velocity of the subsamples differs (meaning they are different objects).
*
* Range: [0, 80]

*
* On some compilers, discard cannot be used in vertex shaders. Thus, they need
* to be compiled separately.

//-----------------------------------------------------------------------------
// Texture Access Defines
//-----------------------------------------------------------------------------
// Non-Configurable Defines
//-----------------------------------------------------------------------------
// Porting Functions
//-----------------------------------------------------------------------------
// Misc functions
*
* Gathers current pixel, and the top-left neighbors.

*
* Adjusts the threshold by means of predication.

*
* Conditional move:

//-----------------------------------------------------------------------------
// Vertex Shaders
*
* Edge Detection Vertex Shader

*
* Blend Weight Calculation Vertex Shader

// We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
// And these for the searches, they indicate the ends of the loops:
*
* Neighborhood Blending Vertex Shader

//-----------------------------------------------------------------------------
// Edge Detection Pixel Shaders (First Pass)
*
* Luma Edge Detection
*
* IMPORTANT NOTICE: luma edge detection requires gamma-corrected colors, and
* thus 'colorTex' should be a non-sRGB texture.

// Calculate the threshold:
// Calculate lumas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
*
* Color Edge Detection
*
* IMPORTANT NOTICE: color edge detection requires gamma-corrected colors, and
* thus 'colorTex' should be a non-sRGB texture.

// Calculate the threshold:
// Calculate color deltas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
*
* Depth Edge Detection

//-----------------------------------------------------------------------------
// Diagonal Search Functions
*
* Allows to decode two binary values from a bilinear-filtered access.

// Bilinear access for fetching 'e' have a 0.25 offset, and we are
// interested in the R and G edges:
//
// +---G---+-------+
// |   x o R   x   |
// +-------+-------+
//
// Then, if one of these edge is enabled:
//   Red:   (0.75 * X + 0.25 * 1) => 0.25 or 1.0
//   Green: (0.75 * 1 + 0.25 * X) => 0.75 or 1.0
//
// This function will unpack the values (mad + mul + round):
// wolframalpha.com: round(x * abs(5 * x - 5 * 0.75)) plot 0 to 1
*
* These functions allows to perform diagonal pattern searches.

// @SearchDiag2Optimization
// Fetch both edges at once using bilinear filtering:
// Non-optimized version:
// e.g = SMAASampleLevelZero(edgesTex, coord.xy).g;
// e.r = SMAASampleLevelZeroOffset(edgesTex, coord.xy, int2(1, 0)).r;
*
* Similar to SMAAArea, this calculates the area corresponding to a certain
* diagonal distance and crossing edges 'e'.

// We do a scale and bias for mapping to texel space:
// Diagonal areas are on the second half of the texture:
// Move to proper place, according to the subpixel offset:
// Do it!
*
* This searches for diagonal patterns and returns the corresponding weights.

// Search for the line ends:
// Fetch the crossing edges:
// Non-optimized version:
// float4 coords = mad(float4(-d.x, d.x, d.y, -d.y), SMAA_RT_METRICS.xyxy, texcoord.xyxy);
// float4 c;
// c.x = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(-1,  0)).g;
// c.y = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2( 0,  0)).r;
// c.z = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1,  0)).g;
// c.w = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1, -1)).r;
// Merge crossing edges at each side into a single value:
// Remove the crossing edge if we didn't found the end of the line:
// Fetch the areas for this line:
// Search for the line ends:
// Fetch the crossing edges:
// Remove the crossing edge if we didn't found the end of the line:
// Fetch the areas for this line:
//-----------------------------------------------------------------------------
// Horizontal/Vertical Search Functions
*
* This allows to determine how much length should we add in the last step
* of the searches. It takes the bilinearly interpolated edge (see
* @PSEUDO_GATHER4), and adds 0, 1 or 2, depending on which edges and
* crossing edges are active.

// The texture is flipped vertically, with left and right cases taking half
// of the space horizontally:
// Scale and bias to access texel centers:
// Convert from pixel coordinates to texcoords:
// (We use SMAA_SEARCHTEX_PACKED_SIZE because the texture is cropped)
// Lookup the search texture:
*
* Horizontal/vertical search functions for the 2nd pass.

*
* @PSEUDO_GATHER4
* This texcoord has been offset by (-0.25, -0.125) in the vertex shader to
* sample between edge, thus fetching four edges in a row.
* Sampling with different offsets in each direction allows to disambiguate
* which edges are active from the four fetched ones.

// Non-optimized version:
// We correct the previous (-0.25, -0.125) offset we applied:
// texcoord.x += 0.25 * SMAA_RT_METRICS.x;
// The searches are bias by 1, so adjust the coords accordingly:
// texcoord.x += SMAA_RT_METRICS.x;
// Disambiguate the length added by the last step:
// texcoord.x += 2.0 * SMAA_RT_METRICS.x; // Undo last step
// texcoord.x -= SMAA_RT_METRICS.x * (255.0 / 127.0) * SMAASearchLength(SMAATexturePass2D(searchTex), e, 0.0);
// return mad(SMAA_RT_METRICS.x, offset, texcoord.x);
*
* Ok, we have the distance and both crossing edges. So, what are the areas
* at each side of current edge?

// Rounding prevents precision errors of bilinear filtering:
// We do a scale and bias for mapping to texel space:
// Move to proper place, according to the subpixel offset:
// Do it!
//-----------------------------------------------------------------------------
// Corner Detection Functions
//-----------------------------------------------------------------------------
// Blending Weight Calculation Pixel Shader (Second Pass)
// Diagonals have both north and west edges, so searching for them in
// one of the boundaries is enough.
// We give priority to diagonals, so if we find a diagonal we skip
// horizontal/vertical processing.
// Find the distance to the left:
// Now fetch the left crossing edges, two at a time using bilinear
// filtering. Sampling at -0.25 (see @CROSSING_OFFSET) enables to
// discern what value each edge has:
// Find the distance to the right:
// We want the distances to be in pixel units (doing this here allow to
// better interleave arithmetic and memory accesses):
// SMAAArea below needs a sqrt, as the areas texture is compressed
// quadratically:
// Fetch the right crossing edges:
// Ok, we know how this pattern looks like, now it is time for getting
// the actual area:
// Fix corners:
// Find the distance to the top:
// Fetch the top crossing edges:
// Find the distance to the bottom:
// We want the distances to be in pixel units:
// SMAAArea below needs a sqrt, as the areas texture is compressed
// quadratically:
// Fetch the bottom crossing edges:
// Get the area for this direction:
// Fix corners:
//-----------------------------------------------------------------------------
// Neighborhood Blending Pixel Shader (Third Pass)
// Fetch the blending weights for current pixel:
// Is there any blending weight with a value greater than 0.0?
// Pack velocity into the alpha channel:
// Calculate the blending offsets:
// Calculate the texture coordinates:
// We exploit bilinear filtering to mix current pixel with the chosen
// neighbor:
// Antialias velocity for proper reprojection in a later stage:
// Pack velocity into the alpha channel:
//-----------------------------------------------------------------------------
// Temporal Resolve Pixel Shader (Optional Pass)
// Velocity is assumed to be calculated for motion blur, so we need to
// inverse it for reprojection:
// Fetch current pixel:
// Reproject current coordinates and fetch previous pixel:
// Attenuate the previous pixel if the velocity is different:
// Blend the pixels according to the calculated weight:
// Just blend the pixels:
//-----------------------------------------------------------------------------
// Separate Multisamples Pixel Shader (Optional Pass)
//-----------------------------------------------------------------------------
*
* Copyright (C) 2013 Jorge Jimenez (jorge@iryoku.com)
* Copyright (C) 2013 Jose I. Echevarria (joseignacioechevarria@gmail.com)
* Copyright (C) 2013 Belen Masia (bmasia@unizar.es)
* Copyright (C) 2013 Fernando Navarro (fernandn@microsoft.com)
* Copyright (C) 2013 Diego Gutierrez (diegog@unizar.es)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to
* do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software. As clarification, there
* is no requirement that the copyright notice and permission be included in
* binary distributions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.

*
*                  _______  ___  ___       ___           ___
*                 /       ||   \/   |     /   \         /   \
*                |   (---- |  \  /  |    /  ^  \       /  ^  \
*                 \   \    |  |\/|  |   /  /_\  \     /  /_\  \
*              ----)   |   |  |  |  |  /  _____  \   /  _____  \
*             |_______/    |__|  |__| /__/     \__\ /__/     \__\
*
*                               E N H A N C E D
*       S U B P I X E L   M O R P H O L O G I C A L   A N T I A L I A S I N G
*
*                         http://www.iryoku.com/smaa/
*
* Hi, welcome aboard!
*
* Here you'll find instructions to get the shader up and running as fast as
* possible.
*
* IMPORTANTE NOTICE: when updating, remember to update both this file and the
* precomputed textures! They may change from version to version.
*
* The shader has three passes, chained together as follows:
*
*                           |input|------------------·
*                              v                     |
*                    [ SMAA*EdgeDetection ]          |
*                              v                     |
*                          |edgesTex|                |
*                              v                     |
*              [ SMAABlendingWeightCalculation ]     |
*                              v                     |
*                          |blendTex|                |
*                              v                     |
*                [ SMAANeighborhoodBlending ] <------·
*                              v
*                           |output|
*
* Note that each [pass] has its own vertex and pixel shader. Remember to use
* oversized triangles instead of quads to avoid overshading along the
* diagonal.
*
* You've three edge detection methods to choose from: luma, color or depth.
* They represent different quality/performance and anti-aliasing/sharpness
* tradeoffs, so our recommendation is for you to choose the one that best
* suits your particular scenario:
*
* - Depth edge detection is usually the fastest but it may miss some edges.
*
* - Luma edge detection is usually more expensive than depth edge detection,
*   but catches visible edges that depth edge detection can miss.
*
* - Color edge detection is usually the most expensive one but catches
*   chroma-only edges.
*
* For quickstarters: just use luma edge detection.
*
* The general advice is to not rush the integration process and ensure each
* step is done correctly (don't try to integrate SMAA T2x with predicated edge
* detection from the start!). Ok then, let's go!
*
*  1. The first step is to create two RGBA temporal render targets for holding
*     |edgesTex| and |blendTex|.
*
*     In DX10 or DX11, you can use a RG render target for the edges texture.
*     In the case of NVIDIA GPUs, using RG render targets seems to actually be
*     slower.
*
*     On the Xbox 360, you can use the same render target for resolving both
*     |edgesTex| and |blendTex|, as they aren't needed simultaneously.
*
*  2. Both temporal render targets |edgesTex| and |blendTex| must be cleared
*     each frame. Do not forget to clear the alpha channel!
*
*  3. The next step is loading the two supporting precalculated textures,
*     'areaTex' and 'searchTex'. You'll find them in the 'Textures' folder as
*     C++ headers, and also as regular DDS files. They'll be needed for the
*     'SMAABlendingWeightCalculation' pass.
*
*     If you use the C++ headers, be sure to load them in the format specified
*     inside of them.
*
*     You can also compress 'areaTex' and 'searchTex' using BC5 and BC4
*     respectively, if you have that option in your content processor pipeline.
*     When compressing then, you get a non-perceptible quality decrease, and a
*     marginal performance increase.
*
*  4. All samplers must be set to linear filtering and clamp.
*
*     After you get the technique working, remember that 64-bit inputs have
*     half-rate linear filtering on GCN.
*
*     If SMAA is applied to 64-bit color buffers, switching to point filtering
*     when accesing them will increase the performance. Search for
*     'SMAASamplePoint' to see which textures may benefit from point
*     filtering, and where (which is basically the color input in the edge
*     detection and resolve passes).
*
*  5. All texture reads and buffer writes must be non-sRGB, with the exception
*     of the input read and the output write in
*     'SMAANeighborhoodBlending' (and only in this pass!). If sRGB reads in
*     this last pass are not possible, the technique will work anyway, but
*     will perform antialiasing in gamma space.
*
*     IMPORTANT: for best results the input read for the color/luma edge
*     detection should *NOT* be sRGB.
*
*  6. Before including SMAA.h you'll have to setup the render target metrics,
*     the target and any optional configuration defines. Optionally you can
*     use a preset.
*
*     You have the following targets available:
*         SMAA_HLSL_3
*         SMAA_HLSL_4
*         SMAA_HLSL_4_1
*         SMAA_GLSL_3 *
*         SMAA_GLSL_4 *
*
*         * (See SMAA_INCLUDE_VS and SMAA_INCLUDE_PS below).
*
*     And four presets:
*         SMAA_PRESET_LOW          (%60 of the quality)
*         SMAA_PRESET_MEDIUM       (%80 of the quality)
*         SMAA_PRESET_HIGH         (%95 of the quality)
*         SMAA_PRESET_ULTRA        (%99 of the quality)
*
*     For example:
*         #define SMAA_RT_METRICS float4(1.0 / 1280.0, 1.0 / 720.0, 1280.0, 720.0)
*         #define SMAA_HLSL_4
*         #define SMAA_PRESET_HIGH
*         #include "SMAA.h
*
*     Note that SMAA_RT_METRICS doesn't need to be a macro, it can be a
*     uniform variable. The code is designed to minimize the impact of not
*     using a constant value, but it is still better to hardcode it.
*
*     Depending on how you encoded 'areaTex' and 'searchTex', you may have to
*     add (and customize) the following defines before including SMAA.h:
*          #define SMAA_AREATEX_SELECT(sample) sample.rg
*          #define SMAA_SEARCHTEX_SELECT(sample) sample.r
*
*     If your engine is already using porting macros, you can define
*     SMAA_CUSTOM_SL, and define the porting functions by yourself.
*
*  7. Then, you'll have to setup the passes as indicated in the scheme above.
*     You can take a look into SMAA.fx, to see how we did it for our demo.
*     Checkout the function wrappers, you may want to copy-paste them!
*
*  8. It's recommended to validate the produced |edgesTex| and |blendTex|.
*     You can use a screenshot from your engine to compare the |edgesTex|
*     and |blendTex| produced inside of the engine with the results obtained
*     with the reference demo.
*
*  9. After you get the last pass to work, it's time to optimize. You'll have
*     to initialize a stencil buffer in the first pass (discard is already in
*     the code), then mask execution by using it the second pass. The last
*     pass should be executed in all pixels.
*
*
* After this point you can choose to enable predicated thresholding,
* temporal supersampling and motion blur integration:
*
* a) If you want to use predicated thresholding, take a look into
*    SMAA_PREDICATION; you'll need to pass an extra texture in the edge
*    detection pass.
*
* b) If you want to enable temporal supersampling (SMAA T2x):
*
* 1. The first step is to render using subpixel jitters. I won't go into
*    detail, but it's as simple as moving each vertex position in the
*    vertex shader, you can check how we do it in our DX10 demo.
*
* 2. Then, you must setup the temporal resolve. You may want to take a look
*    into SMAAResolve for resolving 2x modes. After you get it working, you'll
*    probably see ghosting everywhere. But fear not, you can enable the
*    CryENGINE temporal reprojection by setting the SMAA_REPROJECTION macro.
*    Check out SMAA_DECODE_VELOCITY if your velocity buffer is encoded.
*
* 3. The next step is to apply SMAA to each subpixel jittered frame, just as
*    done for 1x.
*
* 4. At this point you should already have something usable, but for best
*    results the proper area textures must be set depending on current jitter.
*    For this, the parameter 'subsampleIndices' of
*    'SMAABlendingWeightCalculationPS' must be set as follows, for our T2x
*    mode:
*
*    @SUBSAMPLE_INDICES
*
*    | S# |  Camera Jitter   |  subsampleIndices    |
*    +----+------------------+---------------------+
*    |  0 |  ( 0.25, -0.25)  |  float4(1, 1, 1, 0)  |
*    |  1 |  (-0.25,  0.25)  |  float4(2, 2, 2, 0)  |
*
*    These jitter positions assume a bottom-to-top y axis. S# stands for the
*    sample number.
*
* More information about temporal supersampling here:
*    http://iryoku.com/aacourse/downloads/13-Anti-Aliasing-Methods-in-CryENGINE-3.pdf
*
* c) If you want to enable spatial multisampling (SMAA S2x):
*
* 1. The scene must be rendered using MSAA 2x. The MSAA 2x buffer must be
*    created with:
*      - DX10:     see below (*)
*      - DX10.1:   D3D10_STANDARD_MULTISAMPLE_PATTERN or
*      - DX11:     D3D11_STANDARD_MULTISAMPLE_PATTERN
*
*    This allows to ensure that the subsample order matches the table in
*    @SUBSAMPLE_INDICES.
*
*    (*) In the case of DX10, we refer the reader to:
*      - SMAA::detectMSAAOrder and
*      - SMAA::msaaReorder
*
*    These functions allow to match the standard multisample patterns by
*    detecting the subsample order for a specific GPU, and reordering
*    them appropriately.
*
* 2. A shader must be run to output each subsample into a separate buffer
*    (DX10 is required). You can use SMAASeparate for this purpose, or just do
*    it in an existing pass (for example, in the tone mapping pass, which has
*    the advantage of feeding tone mapped subsamples to SMAA, which will yield
*    better results).
*
* 3. The full SMAA 1x pipeline must be run for each separated buffer, storing
*    the results in the final buffer. The second run should alpha blend with
*    the existing final buffer using a blending factor of 0.5.
*    'subsampleIndices' must be adjusted as in the SMAA T2x case (see point
*    b).
*
* d) If you want to enable temporal supersampling on top of SMAA S2x
*    (which actually is SMAA 4x):
*
* 1. SMAA 4x consists on temporally jittering SMAA S2x, so the first step is
*    to calculate SMAA S2x for current frame. In this case, 'subsampleIndices'
*    must be set as follows:
*
*    | F# | S# |   Camera Jitter    |    Net Jitter     |   subsampleIndices   |
*    +----+----+--------------------+-------------------+----------------------+
*    |  0 |  0 |  ( 0.125,  0.125)  |  ( 0.375, -0.125) |  float4(5, 3, 1, 3)  |
*    |  0 |  1 |  ( 0.125,  0.125)  |  (-0.125,  0.375) |  float4(4, 6, 2, 3)  |
*    +----+----+--------------------+-------------------+----------------------+
*    |  1 |  2 |  (-0.125, -0.125)  |  ( 0.125, -0.375) |  float4(3, 5, 1, 4)  |
*    |  1 |  3 |  (-0.125, -0.125)  |  (-0.375,  0.125) |  float4(6, 4, 2, 4)  |
*
*    These jitter positions assume a bottom-to-top y axis. F# stands for the
*    frame number. S# stands for the sample number.
*
* 2. After calculating SMAA S2x for current frame (with the new subsample
*    indices), previous frame must be reprojected as in SMAA T2x mode (see
*    point b).
*
* e) If motion blur is used, you may want to do the edge detection pass
*    together with motion blur. This has two advantages:
*
* 1. Pixels under heavy motion can be omitted from the edge detection process.
*    For these pixels we can just store "no edge", as motion blur will take
*    care of them.
* 2. The center pixel tap is reused.
*
* Note that in this case depth testing should be used instead of stenciling,
* as we have to write all the pixels in the motion blur pass.
*
* That's it!

//-----------------------------------------------------------------------------
// SMAA Presets
*
* Note that if you use one of these presets, the following configuration
* macros will be ignored if set in the "Configurable Defines" section.

//-----------------------------------------------------------------------------
// Configurable Defines
*
* SMAA_THRESHOLD specifies the threshold or sensitivity to edges.
* Lowering this value you will be able to detect more edges at the expense of
* performance.
*
* Range: [0, 0.5]
*   0.1 is a reasonable value, and allows to catch most visible edges.
*   0.05 is a rather overkill value, that allows to catch 'em all.
*
*   If temporal supersampling is used, 0.2 could be a reasonable value, as low
*   contrast edges are properly filtered by just 2x.

*
* SMAA_DEPTH_THRESHOLD specifies the threshold for depth edge detection.
*
* Range: depends on the depth range of the scene.

*
* SMAA_MAX_SEARCH_STEPS specifies the maximum steps performed in the
* horizontal/vertical pattern searches, at each side of the pixel.
*
* In number of pixels, it's actually the double. So the maximum line length
* perfectly handled by, for example 16, is 64 (by perfectly, we meant that
* longer lines won't look as good, but still antialiased).
*
* Range: [0, 112]

*
* SMAA_MAX_SEARCH_STEPS_DIAG specifies the maximum steps performed in the
* diagonal pattern searches, at each side of the pixel. In this case we jump
* one pixel at time, instead of two.
*
* Range: [0, 20]
*
* On high-end machines it is cheap (between a 0.8x and 0.9x slower for 16
* steps), but it can have a significant impact on older machines.
*
* Define SMAA_DISABLE_DIAG_DETECTION to disable diagonal processing.

*
* SMAA_CORNER_ROUNDING specifies how much sharp corners will be rounded.
*
* Range: [0, 100]
*
* Define SMAA_DISABLE_CORNER_DETECTION to disable corner processing.

*
* If there is an neighbor edge that has SMAA_LOCAL_CONTRAST_FACTOR times
* bigger contrast than current edge, current edge will be discarded.
*
* This allows to eliminate spurious crossing edges, and is based on the fact
* that, if there is too much contrast in a direction, that will hide
* perceptually contrast in the other neighbors.

*
* Predicated thresholding allows to better preserve texture details and to
* improve performance, by decreasing the number of detected edges using an
* additional buffer like the light accumulation buffer, object ids or even the
* depth buffer (the depth buffer usage may be limited to indoor or short range
* scenes).
*
* It locally decreases the luma or color threshold if an edge is found in an
* additional buffer (so the global threshold can be higher).
*
* This method was developed by Playstation EDGE MLAA team, and used in
* Killzone 3, by using the light accumulation buffer. More information here:
*     http://iryoku.com/aacourse/downloads/06-MLAA-on-PS3.pptx

*
* Threshold to be used in the additional predication buffer.
*
* Range: depends on the input, so you'll have to find the magic number that
* works for you.

*
* How much to scale the global threshold used for luma or color edge
* detection when using predication.
*
* Range: [1, 5]

*
* How much to locally decrease the threshold.
*
* Range: [0, 1]

*
* Temporal reprojection allows to remove ghosting artifacts when using
* temporal supersampling. We use the CryEngine 3 method which also introduces
* velocity weighting. This feature is of extreme importance for totally
* removing ghosting. More information here:
*    http://iryoku.com/aacourse/downloads/13-Anti-Aliasing-Methods-in-CryENGINE-3.pdf
*
* Note that you'll need to setup a velocity buffer for enabling reprojection.
* For static geometry, saving the previous depth buffer is a viable
* alternative.

*
* SMAA_REPROJECTION_WEIGHT_SCALE controls the velocity weighting. It allows to
* remove ghosting trails behind the moving object, which are not removed by
* just using reprojection. Using low values will exhibit ghosting, while using
* high values will disable temporal supersampling under motion.
*
* Behind the scenes, velocity weighting removes temporal supersampling when
* the velocity of the subsamples differs (meaning they are different objects).
*
* Range: [0, 80]

*
* On some compilers, discard cannot be used in vertex shaders. Thus, they need
* to be compiled separately.

//-----------------------------------------------------------------------------
// Texture Access Defines
//-----------------------------------------------------------------------------
// Non-Configurable Defines
//-----------------------------------------------------------------------------
// Porting Functions
//-----------------------------------------------------------------------------
// Misc functions
*
* Gathers current pixel, and the top-left neighbors.

*
* Adjusts the threshold by means of predication.

*
* Conditional move:

//-----------------------------------------------------------------------------
// Vertex Shaders
*
* Edge Detection Vertex Shader

*
* Blend Weight Calculation Vertex Shader

// We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
// And these for the searches, they indicate the ends of the loops:
*
* Neighborhood Blending Vertex Shader

//-----------------------------------------------------------------------------
// Edge Detection Pixel Shaders (First Pass)
*
* Luma Edge Detection
*
* IMPORTANT NOTICE: luma edge detection requires gamma-corrected colors, and
* thus 'colorTex' should be a non-sRGB texture.

// Calculate the threshold:
// Calculate lumas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
*
* Color Edge Detection
*
* IMPORTANT NOTICE: color edge detection requires gamma-corrected colors, and
* thus 'colorTex' should be a non-sRGB texture.

// Calculate the threshold:
// Calculate color deltas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
*
* Depth Edge Detection

//-----------------------------------------------------------------------------
// Diagonal Search Functions
*
* Allows to decode two binary values from a bilinear-filtered access.

// Bilinear access for fetching 'e' have a 0.25 offset, and we are
// interested in the R and G edges:
//
// +---G---+-------+
// |   x o R   x   |
// +-------+-------+
//
// Then, if one of these edge is enabled:
//   Red:   (0.75 * X + 0.25 * 1) => 0.25 or 1.0
//   Green: (0.75 * 1 + 0.25 * X) => 0.75 or 1.0
//
// This function will unpack the values (mad + mul + round):
// wolframalpha.com: round(x * abs(5 * x - 5 * 0.75)) plot 0 to 1
*
* These functions allows to perform diagonal pattern searches.

// @SearchDiag2Optimization
// Fetch both edges at once using bilinear filtering:
// Non-optimized version:
// e.g = SMAASampleLevelZero(edgesTex, coord.xy).g;
// e.r = SMAASampleLevelZeroOffset(edgesTex, coord.xy, int2(1, 0)).r;
*
* Similar to SMAAArea, this calculates the area corresponding to a certain
* diagonal distance and crossing edges 'e'.

// We do a scale and bias for mapping to texel space:
// Diagonal areas are on the second half of the texture:
// Move to proper place, according to the subpixel offset:
// Do it!
*
* This searches for diagonal patterns and returns the corresponding weights.

// Search for the line ends:
// Fetch the crossing edges:
// Non-optimized version:
// float4 coords = mad(float4(-d.x, d.x, d.y, -d.y), SMAA_RT_METRICS.xyxy, texcoord.xyxy);
// float4 c;
// c.x = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2(-1,  0)).g;
// c.y = SMAASampleLevelZeroOffset(edgesTex, coords.xy, int2( 0,  0)).r;
// c.z = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1,  0)).g;
// c.w = SMAASampleLevelZeroOffset(edgesTex, coords.zw, int2( 1, -1)).r;
// Merge crossing edges at each side into a single value:
// Remove the crossing edge if we didn't found the end of the line:
// Fetch the areas for this line:
// Search for the line ends:
// Fetch the crossing edges:
// Remove the crossing edge if we didn't found the end of the line:
// Fetch the areas for this line:
//-----------------------------------------------------------------------------
// Horizontal/Vertical Search Functions
*
* This allows to determine how much length should we add in the last step
* of the searches. It takes the bilinearly interpolated edge (see
* @PSEUDO_GATHER4), and adds 0, 1 or 2, depending on which edges and
* crossing edges are active.

// The texture is flipped vertically, with left and right cases taking half
// of the space horizontally:
// Scale and bias to access texel centers:
// Convert from pixel coordinates to texcoords:
// (We use SMAA_SEARCHTEX_PACKED_SIZE because the texture is cropped)
// Lookup the search texture:
*
* Horizontal/vertical search functions for the 2nd pass.

*
* @PSEUDO_GATHER4
* This texcoord has been offset by (-0.25, -0.125) in the vertex shader to
* sample between edge, thus fetching four edges in a row.
* Sampling with different offsets in each direction allows to disambiguate
* which edges are active from the four fetched ones.

// Non-optimized version:
// We correct the previous (-0.25, -0.125) offset we applied:
// texcoord.x += 0.25 * SMAA_RT_METRICS.x;
// The searches are bias by 1, so adjust the coords accordingly:
// texcoord.x += SMAA_RT_METRICS.x;
// Disambiguate the length added by the last step:
// texcoord.x += 2.0 * SMAA_RT_METRICS.x; // Undo last step
// texcoord.x -= SMAA_RT_METRICS.x * (255.0 / 127.0) * SMAASearchLength(SMAATexturePass2D(searchTex), e, 0.0);
// return mad(SMAA_RT_METRICS.x, offset, texcoord.x);
*
* Ok, we have the distance and both crossing edges. So, what are the areas
* at each side of current edge?

// Rounding prevents precision errors of bilinear filtering:
// We do a scale and bias for mapping to texel space:
// Move to proper place, according to the subpixel offset:
// Do it!
//-----------------------------------------------------------------------------
// Corner Detection Functions
//-----------------------------------------------------------------------------
// Blending Weight Calculation Pixel Shader (Second Pass)
// Diagonals have both north and west edges, so searching for them in
// one of the boundaries is enough.
// We give priority to diagonals, so if we find a diagonal we skip
// horizontal/vertical processing.
// Find the distance to the left:
// Now fetch the left crossing edges, two at a time using bilinear
// filtering. Sampling at -0.25 (see @CROSSING_OFFSET) enables to
// discern what value each edge has:
// Find the distance to the right:
// We want the distances to be in pixel units (doing this here allow to
// better interleave arithmetic and memory accesses):
// SMAAArea below needs a sqrt, as the areas texture is compressed
// quadratically:
// Fetch the right crossing edges:
// Ok, we know how this pattern looks like, now it is time for getting
// the actual area:
// Fix corners:
// Find the distance to the top:
// Fetch the top crossing edges:
// Find the distance to the bottom:
// We want the distances to be in pixel units:
// SMAAArea below needs a sqrt, as the areas texture is compressed
// quadratically:
// Fetch the bottom crossing edges:
// Get the area for this direction:
// Fix corners:
//-----------------------------------------------------------------------------
// Neighborhood Blending Pixel Shader (Third Pass)
// Fetch the blending weights for current pixel:
// Is there any blending weight with a value greater than 0.0?
// Pack velocity into the alpha channel:
// Calculate the blending offsets:
// Calculate the texture coordinates:
// We exploit bilinear filtering to mix current pixel with the chosen
// neighbor:
// Antialias velocity for proper reprojection in a later stage:
// Pack velocity into the alpha channel:
//-----------------------------------------------------------------------------
// Temporal Resolve Pixel Shader (Optional Pass)
// Velocity is assumed to be calculated for motion blur, so we need to
// inverse it for reprojection:
// Fetch current pixel:
// Reproject current coordinates and fetch previous pixel:
// Attenuate the previous pixel if the velocity is different:
// Blend the pixels according to the calculated weight:
// Just blend the pixels:
//-----------------------------------------------------------------------------
// Separate Multisamples Pixel Shader (Optional Pass)
//-----------------------------------------------------------------------------

*/

#pragma once

namespace RetroArchAntiAliasingShadersSmaaSmaaPass2ShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,156,235,
39,248,75,234,60,167,
49,17,191,154,70,55,
34,85,1,0,0,0,
120,5,0,0,5,0,
0,0,52,0,0,0,
192,2,0,0,12,3,
0,0,124,3,0,0,
220,4,0,0,82,68,
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
0,0,0,0,212,1,
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
0,0,1,0,0,0,
15,0,0,0,89,0,
0,0,0,0,0,0,
1,0,0,0,3,0,
0,0,2,0,0,0,
15,0,0,0,84,69,
88,67,79,79,82,68,
0,83,86,95,80,111,
115,105,116,105,111,110,
0,171,171,171,83,72,
69,88,88,1,0,0,
80,0,1,0,86,0,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,0,0,0,0,
4,0,0,0,89,0,
0,4,70,142,32,0,
1,0,0,0,1,0,
0,0,95,0,0,3,
242,16,16,0,0,0,
0,0,95,0,0,3,
50,16,16,0,1,0,
0,0,101,0,0,3,
50,32,16,0,0,0,
0,0,101,0,0,3,
242,32,16,0,1,0,
0,0,103,0,0,4,
242,32,16,0,2,0,
0,0,1,0,0,0,
104,0,0,2,1,0,
0,0,54,0,0,5,
50,32,16,0,0,0,
0,0,70,16,16,0,
1,0,0,0,50,0,
0,13,242,32,16,0,
1,0,0,0,230,142,
32,0,1,0,0,0,
0,0,0,0,2,64,
0,0,0,0,128,63,
0,0,0,0,0,0,
0,0,0,0,128,63,
70,20,16,0,1,0,
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
16,0,2,0,0,0,
246,31,16,0,0,0,
0,0,70,142,32,0,
0,0,0,0,3,0,
0,0,70,14,16,0,
0,0,0,0,62,0,
0,1,83,84,65,84,
148,0,0,0,7,0,
0,0,1,0,0,0,
0,0,0,0,5,0,
0,0,5,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
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


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,86,159,
95,118,53,25,43,137,
145,37,106,15,64,117,
101,162,1,0,0,0,
152,7,0,0,5,0,
0,0,52,0,0,0,
216,2,0,0,36,3,
0,0,88,3,0,0,
252,6,0,0,82,68,
69,70,156,2,0,0,
1,0,0,0,24,1,
0,0,5,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
116,2,0,0,82,68,
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
0,0,236,0,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,1,0,0,0,
1,0,0,0,0,1,
0,0,2,0,0,0,
5,0,0,0,4,0,
0,0,255,255,255,255,
2,0,0,0,1,0,
0,0,13,0,0,0,
7,1,0,0,2,0,
0,0,5,0,0,0,
4,0,0,0,255,255,
255,255,3,0,0,0,
1,0,0,0,13,0,
0,0,18,1,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,1,0,0,0,
0,0,0,0,95,83,
111,117,114,99,101,95,
115,97,109,112,108,101,
114,0,95,83,77,65,
65,95,73,110,112,117,
116,95,115,97,109,112,
108,101,114,0,83,111,
117,114,99,101,0,83,
77,65,65,95,73,110,
112,117,116,0,80,117,
115,104,0,171,18,1,
0,0,4,0,0,0,
48,1,0,0,64,0,
0,0,0,0,0,0,
0,0,0,0,208,1,
0,0,0,0,0,0,
16,0,0,0,2,0,
0,0,236,1,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,16,2,0,0,
16,0,0,0,16,0,
0,0,0,0,0,0,
236,1,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
36,2,0,0,32,0,
0,0,16,0,0,0,
0,0,0,0,236,1,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,54,2,
0,0,48,0,0,0,
4,0,0,0,0,0,
0,0,80,2,0,0,
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
0,0,226,1,0,0,
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
72,2,0,0,77,105,
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
0,0,3,3,0,0,
56,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,15,15,0,0,
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
69,88,156,3,0,0,
80,0,0,0,231,0,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,1,0,0,0,
1,0,0,0,90,0,
0,3,0,96,16,0,
2,0,0,0,90,0,
0,3,0,96,16,0,
3,0,0,0,88,24,
0,4,0,112,16,0,
2,0,0,0,85,85,
0,0,88,24,0,4,
0,112,16,0,3,0,
0,0,85,85,0,0,
98,16,0,3,50,16,
16,0,0,0,0,0,
98,16,0,3,242,16,
16,0,1,0,0,0,
101,0,0,3,242,32,
16,0,0,0,0,0,
104,0,0,2,3,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,18,0,16,0,
0,0,0,0,70,16,
16,0,1,0,0,0,
54,121,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,34,0,
16,0,0,0,0,0,
230,26,16,0,1,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
194,0,16,0,0,0,
0,0,70,16,16,0,
0,0,0,0,214,114,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,17,0,0,10,
18,0,16,0,1,0,
0,0,70,14,16,0,
0,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,128,63,
49,0,0,7,18,0,
16,0,1,0,0,0,
10,0,16,0,1,0,
0,0,1,64,0,0,
172,197,39,55,31,0,
4,3,10,0,16,0,
1,0,0,0,72,0,
0,141,194,0,0,128,
67,85,21,0,242,32,
16,0,0,0,0,0,
70,16,16,0,0,0,
0,0,70,126,16,0,
3,0,0,0,0,96,
16,0,3,0,0,0,
1,64,0,0,0,0,
0,0,18,0,0,1,
52,0,0,7,50,0,
16,0,1,0,0,0,
230,10,16,0,0,0,
0,0,70,0,16,0,
0,0,0,0,49,0,
0,7,18,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
10,0,16,0,1,0,
0,0,1,0,0,7,
82,0,16,0,2,0,
0,0,6,2,16,0,
0,0,0,0,6,0,
16,0,1,0,0,0,
55,0,0,12,162,0,
16,0,2,0,0,0,
6,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,86,13,
16,0,0,0,0,0,
55,0,0,9,18,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,10,0,16,0,
0,0,0,0,26,0,
16,0,0,0,0,0,
55,0,0,9,34,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,42,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
15,0,0,10,66,0,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,0,0,
0,0,0,0,14,0,
0,7,50,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
166,10,16,0,0,0,
0,0,56,0,0,11,
242,0,16,0,1,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,191,
0,0,128,191,230,142,
32,0,1,0,0,0,
0,0,0,0,50,0,
0,9,242,0,16,0,
1,0,0,0,70,14,
16,0,2,0,0,0,
70,14,16,0,1,0,
0,0,70,20,16,0,
0,0,0,0,72,0,
0,141,194,0,0,128,
67,85,21,0,242,0,
16,0,2,0,0,0,
70,0,16,0,1,0,
0,0,70,126,16,0,
3,0,0,0,0,96,
16,0,3,0,0,0,
1,64,0,0,0,0,
0,0,72,0,0,141,
194,0,0,128,67,85,
21,0,242,0,16,0,
1,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,3,0,
0,0,0,96,16,0,
3,0,0,0,1,64,
0,0,0,0,0,0,
56,0,0,7,242,0,
16,0,1,0,0,0,
86,5,16,0,0,0,
0,0,70,14,16,0,
1,0,0,0,50,0,
0,9,242,32,16,0,
0,0,0,0,70,14,
16,0,2,0,0,0,
6,0,16,0,0,0,
0,0,70,14,16,0,
1,0,0,0,21,0,
0,1,62,0,0,1,
83,84,65,84,148,0,
0,0,24,0,0,0,
3,0,0,0,0,0,
0,0,3,0,0,0,
10,0,0,0,0,0,
0,0,1,0,0,0,
2,0,0,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,3,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
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
class AntiAliasingShadersSmaaSmaaPass2ShaderDef : public ShaderDef
{
public:
	AntiAliasingShadersSmaaSmaaPass2ShaderDef() : ShaderDef{}
	{
		Name = "smaa-pass2";
		VertexByteCode = RetroArchAntiAliasingShadersSmaaSmaaPass2ShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchAntiAliasingShadersSmaaSmaaPass2ShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchAntiAliasingShadersSmaaSmaaPass2ShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchAntiAliasingShadersSmaaSmaaPass2ShaderDefs::sFragmentByteCode);
		Params.push_back(ShaderParam("MVP", 0, 0, 64, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("SourceSize", -1, 0, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OriginalSize", -1, 16, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OutputSize", -1, 32, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("FrameCount", -1, 48, 4, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Samplers.push_back(ShaderSampler("SMAA_Input", 3));
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
