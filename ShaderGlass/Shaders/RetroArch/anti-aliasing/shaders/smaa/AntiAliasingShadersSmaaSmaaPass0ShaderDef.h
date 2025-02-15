/*
ShaderGlass shader anti-aliasing/shaders/smaa\smaa-pass0 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/anti-aliasing/shaders/smaa/smaa-pass0.slang
See original file for full credits and usage license with excerpts below. 
This file is auto-generated, do not modify directly.

// SPDX-License-Identifier: Unlicense
//-----------------------------------------------------------------------------
// Edge Detection Shaders (First Pass)
//-----------------------------------------------------------------------------
// SMAA Presets
//-----------------------------------------------------------------------------
// Configurable Defines
//-----------------------------------------------------------------------------
// Texture Access Defines
//-----------------------------------------------------------------------------
// Non-Configurable Defines
//-----------------------------------------------------------------------------
// Porting Functions
//-----------------------------------------------------------------------------
// Misc functions
//-----------------------------------------------------------------------------
// Vertex Shaders
// We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
// And these for the searches, they indicate the ends of the loops:
//-----------------------------------------------------------------------------
// Edge Detection Pixel Shaders (First Pass)
// Calculate the threshold:
// Calculate lumas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
// Calculate the threshold:
// Calculate color deltas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
//-----------------------------------------------------------------------------
// Diagonal Search Functions
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
// @SearchDiag2Optimization
// Fetch both edges at once using bilinear filtering:
// Non-optimized version:
// e.g = SMAASampleLevelZero(edgesTex, coord.xy).g;
// e.r = SMAASampleLevelZeroOffset(edgesTex, coord.xy, int2(1, 0)).r;
// We do a scale and bias for mapping to texel space:
// Diagonal areas are on the second half of the texture:
// Move to proper place, according to the subpixel offset:
// Do it!
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
// The texture is flipped vertically, with left and right cases taking half
// of the space horizontally:
// Scale and bias to access texel centers:
// Convert from pixel coordinates to texcoords:
// (We use SMAA_SEARCHTEX_PACKED_SIZE because the texture is cropped)
// Lookup the search texture:
// Non-optimized version:
// We correct the previous (-0.25, -0.125) offset we applied:
// texcoord.x += 0.25 * SMAA_RT_METRICS.x;
// The searches are bias by 1, so adjust the coords accordingly:
// texcoord.x += SMAA_RT_METRICS.x;
// Disambiguate the length added by the last step:
// texcoord.x += 2.0 * SMAA_RT_METRICS.x; // Undo last step
// texcoord.x -= SMAA_RT_METRICS.x * (255.0 / 127.0) * SMAASearchLength(SMAATexturePass2D(searchTex), e, 0.0);
// return mad(SMAA_RT_METRICS.x, offset, texcoord.x);
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
//-----------------------------------------------------------------------------
// SMAA Presets
//-----------------------------------------------------------------------------
// Configurable Defines
//-----------------------------------------------------------------------------
// Texture Access Defines
//-----------------------------------------------------------------------------
// Non-Configurable Defines
//-----------------------------------------------------------------------------
// Porting Functions
//-----------------------------------------------------------------------------
// Misc functions
//-----------------------------------------------------------------------------
// Vertex Shaders
// We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
// And these for the searches, they indicate the ends of the loops:
//-----------------------------------------------------------------------------
// Edge Detection Pixel Shaders (First Pass)
// Calculate the threshold:
// Calculate lumas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
// Calculate the threshold:
// Calculate color deltas:
// We do the usual threshold:
// Then discard if there is no edge:
// Calculate right and bottom deltas:
// Calculate the maximum delta in the direct neighborhood:
// Calculate left-left and top-top deltas:
// Calculate the final maximum delta:
// Local contrast adaptation:
//-----------------------------------------------------------------------------
// Diagonal Search Functions
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
// @SearchDiag2Optimization
// Fetch both edges at once using bilinear filtering:
// Non-optimized version:
// e.g = SMAASampleLevelZero(edgesTex, coord.xy).g;
// e.r = SMAASampleLevelZeroOffset(edgesTex, coord.xy, int2(1, 0)).r;
// We do a scale and bias for mapping to texel space:
// Diagonal areas are on the second half of the texture:
// Move to proper place, according to the subpixel offset:
// Do it!
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
// The texture is flipped vertically, with left and right cases taking half
// of the space horizontally:
// Scale and bias to access texel centers:
// Convert from pixel coordinates to texcoords:
// (We use SMAA_SEARCHTEX_PACKED_SIZE because the texture is cropped)
// Lookup the search texture:
// Non-optimized version:
// We correct the previous (-0.25, -0.125) offset we applied:
// texcoord.x += 0.25 * SMAA_RT_METRICS.x;
// The searches are bias by 1, so adjust the coords accordingly:
// texcoord.x += SMAA_RT_METRICS.x;
// Disambiguate the length added by the last step:
// texcoord.x += 2.0 * SMAA_RT_METRICS.x; // Undo last step
// texcoord.x -= SMAA_RT_METRICS.x * (255.0 / 127.0) * SMAASearchLength(SMAATexturePass2D(searchTex), e, 0.0);
// return mad(SMAA_RT_METRICS.x, offset, texcoord.x);
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
// Unavailable as we don't have access to a depth buffer (yet?)
// } else if (params.SMAA_EDT <= 2.0) {
//     FragColor = vec4(SMAADepthEdgeDetectionPS(vTexCoord, offset, depthTex), 0.0, 0.0);

*/

#pragma once

namespace RetroArchAntiAliasingShadersSmaaSmaaPass0ShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,195,95,
164,73,200,224,62,193,
32,95,223,129,179,14,
80,167,1,0,0,0,
176,7,0,0,5,0,
0,0,52,0,0,0,
72,4,0,0,148,4,
0,0,52,5,0,0,
20,7,0,0,82,68,
69,70,12,4,0,0,
2,0,0,0,136,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
226,3,0,0,82,68,
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
0,0,9,0,0,0,
24,1,0,0,80,0,
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
235,0,0,0,128,2,
0,0,0,0,0,0,
16,0,0,0,2,0,
0,0,156,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,192,2,0,0,
16,0,0,0,16,0,
0,0,0,0,0,0,
156,2,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
212,2,0,0,32,0,
0,0,16,0,0,0,
0,0,0,0,156,2,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,230,2,
0,0,48,0,0,0,
4,0,0,0,0,0,
0,0,0,3,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,36,3,0,0,
52,0,0,0,4,0,
0,0,0,0,0,0,
60,3,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
96,3,0,0,56,0,
0,0,4,0,0,0,
0,0,0,0,60,3,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,118,3,
0,0,60,0,0,0,
4,0,0,0,0,0,
0,0,60,3,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,147,3,0,0,
64,0,0,0,4,0,
0,0,0,0,0,0,
60,3,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
181,3,0,0,68,0,
0,0,4,0,0,0,
0,0,0,0,60,3,
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
0,0,0,0,146,2,
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
0,0,248,2,0,0,
112,97,114,97,109,115,
95,83,77,65,65,95,
69,68,84,0,102,108,
111,97,116,0,171,171,
0,0,3,0,1,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,52,3,0,0,
112,97,114,97,109,115,
95,83,77,65,65,95,
84,72,82,69,83,72,
79,76,68,0,112,97,
114,97,109,115,95,83,
77,65,65,95,77,65,
88,95,83,69,65,82,
67,72,95,83,84,69,
80,83,0,112,97,114,
97,109,115,95,83,77,
65,65,95,77,65,88,
95,83,69,65,82,67,
72,95,83,84,69,80,
83,95,68,73,65,71,
0,112,97,114,97,109,
115,95,83,77,65,65,
95,76,79,67,65,76,
95,67,79,78,84,82,
65,83,84,95,65,68,
65,80,84,65,84,73,
79,78,95,70,65,67,
84,79,82,0,77,105,
99,114,111,115,111,102,
116,32,40,82,41,32,
72,76,83,76,32,83,
104,97,100,101,114,32,
67,111,109,112,105,108,
101,114,32,49,48,46,
49,0,171,171,73,83,
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
3,0,0,0,1,0,
0,0,15,0,0,0,
128,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,2,0,
0,0,15,0,0,0,
128,0,0,0,3,0,
0,0,0,0,0,0,
3,0,0,0,3,0,
0,0,15,0,0,0,
137,0,0,0,0,0,
0,0,1,0,0,0,
3,0,0,0,4,0,
0,0,15,0,0,0,
84,69,88,67,79,79,
82,68,0,83,86,95,
80,111,115,105,116,105,
111,110,0,171,171,171,
83,72,69,88,216,1,
0,0,80,0,1,0,
118,0,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,4,0,0,0,
89,0,0,4,70,142,
32,0,1,0,0,0,
1,0,0,0,95,0,
0,3,242,16,16,0,
0,0,0,0,95,0,
0,3,50,16,16,0,
1,0,0,0,101,0,
0,3,50,32,16,0,
0,0,0,0,101,0,
0,3,242,32,16,0,
1,0,0,0,101,0,
0,3,242,32,16,0,
2,0,0,0,101,0,
0,3,242,32,16,0,
3,0,0,0,103,0,
0,4,242,32,16,0,
4,0,0,0,1,0,
0,0,104,0,0,2,
1,0,0,0,54,0,
0,5,50,32,16,0,
0,0,0,0,70,16,
16,0,1,0,0,0,
50,0,0,13,242,32,
16,0,1,0,0,0,
230,142,32,0,1,0,
0,0,0,0,0,0,
2,64,0,0,0,0,
128,191,0,0,0,0,
0,0,0,0,0,0,
128,191,70,20,16,0,
1,0,0,0,50,0,
0,13,242,32,16,0,
2,0,0,0,230,142,
32,0,1,0,0,0,
0,0,0,0,2,64,
0,0,0,0,128,63,
0,0,0,0,0,0,
0,0,0,0,128,63,
70,20,16,0,1,0,
0,0,50,0,0,13,
242,32,16,0,3,0,
0,0,230,142,32,0,
1,0,0,0,0,0,
0,0,2,64,0,0,
0,0,0,192,0,0,
0,0,0,0,0,0,
0,0,0,192,70,20,
16,0,1,0,0,0,
56,0,0,8,242,0,
16,0,0,0,0,0,
86,21,16,0,0,0,
0,0,70,142,32,0,
0,0,0,0,1,0,
0,0,50,0,0,10,
242,0,16,0,0,0,
0,0,6,16,16,0,
0,0,0,0,70,142,
32,0,0,0,0,0,
0,0,0,0,70,14,
16,0,0,0,0,0,
50,0,0,10,242,0,
16,0,0,0,0,0,
166,26,16,0,0,0,
0,0,70,142,32,0,
0,0,0,0,2,0,
0,0,70,14,16,0,
0,0,0,0,50,0,
0,10,242,32,16,0,
4,0,0,0,246,31,
16,0,0,0,0,0,
70,142,32,0,0,0,
0,0,3,0,0,0,
70,14,16,0,0,0,
0,0,62,0,0,1,
83,84,65,84,148,0,
0,0,9,0,0,0,
1,0,0,0,0,0,
0,0,7,0,0,0,
7,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
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
0,0,0,0,0,0,
0,0,0,0,0,0
};


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,124,255,
102,105,183,235,173,12,
206,42,204,233,32,60,
175,232,1,0,0,0,
180,15,0,0,5,0,
0,0,52,0,0,0,
0,4,0,0,124,4,
0,0,176,4,0,0,
24,15,0,0,82,68,
69,70,196,3,0,0,
1,0,0,0,184,0,
0,0,3,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
154,3,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
156,0,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
1,0,0,0,1,0,
0,0,172,0,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,2,0,
0,0,1,0,0,0,
13,0,0,0,179,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,1,0,
0,0,0,0,0,0,
95,83,111,117,114,99,
101,95,115,97,109,112,
108,101,114,0,83,111,
117,114,99,101,0,80,
117,115,104,0,179,0,
0,0,9,0,0,0,
208,0,0,0,80,0,
0,0,0,0,0,0,
0,0,0,0,56,2,
0,0,0,0,0,0,
16,0,0,0,0,0,
0,0,84,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,120,2,0,0,
16,0,0,0,16,0,
0,0,0,0,0,0,
84,2,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
140,2,0,0,32,0,
0,0,16,0,0,0,
0,0,0,0,84,2,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,158,2,
0,0,48,0,0,0,
4,0,0,0,0,0,
0,0,184,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,220,2,0,0,
52,0,0,0,4,0,
0,0,2,0,0,0,
244,2,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
24,3,0,0,56,0,
0,0,4,0,0,0,
2,0,0,0,244,2,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,46,3,
0,0,60,0,0,0,
4,0,0,0,0,0,
0,0,244,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,75,3,0,0,
64,0,0,0,4,0,
0,0,0,0,0,0,
244,2,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
109,3,0,0,68,0,
0,0,4,0,0,0,
2,0,0,0,244,2,
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
0,0,0,0,74,2,
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
0,0,176,2,0,0,
112,97,114,97,109,115,
95,83,77,65,65,95,
69,68,84,0,102,108,
111,97,116,0,171,171,
0,0,3,0,1,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,236,2,0,0,
112,97,114,97,109,115,
95,83,77,65,65,95,
84,72,82,69,83,72,
79,76,68,0,112,97,
114,97,109,115,95,83,
77,65,65,95,77,65,
88,95,83,69,65,82,
67,72,95,83,84,69,
80,83,0,112,97,114,
97,109,115,95,83,77,
65,65,95,77,65,88,
95,83,69,65,82,67,
72,95,83,84,69,80,
83,95,68,73,65,71,
0,112,97,114,97,109,
115,95,83,77,65,65,
95,76,79,67,65,76,
95,67,79,78,84,82,
65,83,84,95,65,68,
65,80,84,65,84,73,
79,78,95,70,65,67,
84,79,82,0,77,105,
99,114,111,115,111,102,
116,32,40,82,41,32,
72,76,83,76,32,83,
104,97,100,101,114,32,
67,111,109,112,105,108,
101,114,32,49,48,46,
49,0,171,171,73,83,
71,78,116,0,0,0,
4,0,0,0,8,0,
0,0,104,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,3,3,
0,0,104,0,0,0,
1,0,0,0,0,0,
0,0,3,0,0,0,
1,0,0,0,15,15,
0,0,104,0,0,0,
2,0,0,0,0,0,
0,0,3,0,0,0,
2,0,0,0,15,15,
0,0,104,0,0,0,
3,0,0,0,0,0,
0,0,3,0,0,0,
3,0,0,0,15,15,
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
83,72,69,88,96,10,
0,0,80,0,0,0,
152,2,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,1,0,
0,0,5,0,0,0,
90,0,0,3,0,96,
16,0,2,0,0,0,
88,24,0,4,0,112,
16,0,2,0,0,0,
85,85,0,0,98,16,
0,3,50,16,16,0,
0,0,0,0,98,16,
0,3,242,16,16,0,
1,0,0,0,98,16,
0,3,242,16,16,0,
2,0,0,0,98,16,
0,3,242,16,16,0,
3,0,0,0,101,0,
0,3,242,32,16,0,
0,0,0,0,104,0,
0,2,7,0,0,0,
24,0,0,8,18,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
0,0,26,128,32,0,
1,0,0,0,3,0,
0,0,31,0,4,3,
10,0,16,0,0,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
0,0,0,0,70,16,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,16,0,
0,10,18,0,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
2,64,0,0,208,179,
89,62,89,23,55,63,
152,221,147,61,0,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,226,0,16,0,
0,0,0,0,70,16,
16,0,1,0,0,0,
54,121,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,16,0,
0,10,18,0,16,0,
1,0,0,0,150,7,
16,0,0,0,0,0,
2,64,0,0,208,179,
89,62,89,23,55,63,
152,221,147,61,0,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,226,0,16,0,
0,0,0,0,230,26,
16,0,1,0,0,0,
54,121,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,16,0,
0,10,34,0,16,0,
1,0,0,0,150,7,
16,0,0,0,0,0,
2,64,0,0,208,179,
89,62,89,23,55,63,
152,221,147,61,0,0,
0,0,0,0,0,8,
98,0,16,0,0,0,
0,0,6,0,16,0,
0,0,0,0,6,1,
16,128,65,0,0,0,
1,0,0,0,29,0,
0,9,194,0,16,0,
1,0,0,0,86,9,
16,128,129,0,0,0,
0,0,0,0,166,138,
32,0,1,0,0,0,
3,0,0,0,1,0,
0,10,194,0,16,0,
1,0,0,0,166,14,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,128,63,0,0,
128,63,15,0,0,10,
130,0,16,0,0,0,
0,0,230,10,16,0,
1,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
0,0,0,0,0,0,
24,0,0,7,130,0,
16,0,0,0,0,0,
58,0,16,0,0,0,
0,0,1,64,0,0,
0,0,0,0,13,0,
4,3,58,0,16,0,
0,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,2,0,0,0,
70,16,16,0,2,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
16,0,0,10,18,0,
16,0,2,0,0,0,
70,2,16,0,2,0,
0,0,2,64,0,0,
208,179,89,62,89,23,
55,63,152,221,147,61,
0,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,3,0,0,0,
230,26,16,0,2,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
16,0,0,10,34,0,
16,0,2,0,0,0,
70,2,16,0,3,0,
0,0,2,64,0,0,
208,179,89,62,89,23,
55,63,152,221,147,61,
0,0,0,0,0,0,
0,8,146,0,16,0,
0,0,0,0,6,0,
16,0,0,0,0,0,
6,4,16,128,65,0,
0,0,2,0,0,0,
52,0,0,9,146,0,
16,0,0,0,0,0,
6,12,16,128,129,0,
0,0,0,0,0,0,
86,9,16,128,129,0,
0,0,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,2,0,
0,0,70,16,16,0,
3,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,16,0,0,10,
18,0,16,0,2,0,
0,0,70,2,16,0,
2,0,0,0,2,64,
0,0,208,179,89,62,
89,23,55,63,152,221,
147,61,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,3,0,
0,0,230,26,16,0,
3,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,16,0,0,10,
34,0,16,0,2,0,
0,0,70,2,16,0,
3,0,0,0,2,64,
0,0,208,179,89,62,
89,23,55,63,152,221,
147,61,0,0,0,0,
0,0,0,8,50,0,
16,0,1,0,0,0,
70,0,16,0,1,0,
0,0,70,0,16,128,
65,0,0,0,2,0,
0,0,52,0,0,8,
146,0,16,0,0,0,
0,0,6,12,16,0,
0,0,0,0,6,4,
16,128,129,0,0,0,
1,0,0,0,52,0,
0,7,18,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,56,0,0,9,
98,0,16,0,0,0,
0,0,86,6,16,128,
129,0,0,0,0,0,
0,0,86,133,32,0,
1,0,0,0,4,0,
0,0,29,0,0,7,
50,0,16,0,0,0,
0,0,150,5,16,0,
0,0,0,0,6,0,
16,0,0,0,0,0,
1,0,0,10,50,0,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,0,0,
0,0,0,0,56,0,
0,7,50,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
230,10,16,0,1,0,
0,0,18,0,0,1,
29,0,0,8,66,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,26,128,32,0,
1,0,0,0,3,0,
0,0,31,0,4,3,
42,0,16,0,0,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
1,0,0,0,70,16,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,2,0,0,0,
70,16,16,0,1,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
0,0,0,8,114,0,
16,0,3,0,0,0,
70,2,16,0,1,0,
0,0,70,2,16,128,
65,0,0,0,2,0,
0,0,52,0,0,9,
66,0,16,0,0,0,
0,0,26,0,16,128,
129,0,0,0,3,0,
0,0,10,0,16,128,
129,0,0,0,3,0,
0,0,52,0,0,8,
18,0,16,0,3,0,
0,0,42,0,16,128,
129,0,0,0,3,0,
0,0,42,0,16,0,
0,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,4,0,0,0,
230,26,16,0,1,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
0,0,0,8,114,0,
16,0,5,0,0,0,
70,2,16,0,1,0,
0,0,70,2,16,128,
65,0,0,0,4,0,
0,0,52,0,0,9,
66,0,16,0,0,0,
0,0,26,0,16,128,
129,0,0,0,5,0,
0,0,10,0,16,128,
129,0,0,0,5,0,
0,0,52,0,0,8,
34,0,16,0,3,0,
0,0,42,0,16,128,
129,0,0,0,5,0,
0,0,42,0,16,0,
0,0,0,0,29,0,
0,8,194,0,16,0,
0,0,0,0,6,4,
16,0,3,0,0,0,
166,138,32,0,1,0,
0,0,3,0,0,0,
1,0,0,10,194,0,
16,0,0,0,0,0,
166,14,16,0,0,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,128,63,
0,0,128,63,15,0,
0,10,130,0,16,0,
1,0,0,0,230,10,
16,0,0,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,0,0,0,0,
0,0,24,0,0,7,
130,0,16,0,1,0,
0,0,58,0,16,0,
1,0,0,0,1,64,
0,0,0,0,0,0,
13,0,4,3,58,0,
16,0,1,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,5,0,
0,0,70,16,16,0,
2,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,0,0,0,8,
114,0,16,0,5,0,
0,0,70,2,16,0,
1,0,0,0,70,2,
16,128,65,0,0,0,
5,0,0,0,52,0,
0,9,130,0,16,0,
1,0,0,0,26,0,
16,128,129,0,0,0,
5,0,0,0,10,0,
16,128,129,0,0,0,
5,0,0,0,52,0,
0,8,18,0,16,0,
5,0,0,0,42,0,
16,128,129,0,0,0,
5,0,0,0,58,0,
16,0,1,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,6,0,
0,0,230,26,16,0,
2,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,0,0,0,8,
114,0,16,0,1,0,
0,0,70,2,16,0,
1,0,0,0,70,2,
16,128,65,0,0,0,
6,0,0,0,52,0,
0,9,18,0,16,0,
1,0,0,0,26,0,
16,128,129,0,0,0,
1,0,0,0,10,0,
16,128,129,0,0,0,
1,0,0,0,52,0,
0,8,34,0,16,0,
5,0,0,0,42,0,
16,128,129,0,0,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
52,0,0,7,50,0,
16,0,1,0,0,0,
70,0,16,0,3,0,
0,0,70,0,16,0,
5,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,5,0,0,0,
70,16,16,0,3,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
0,0,0,8,114,0,
16,0,2,0,0,0,
70,2,16,0,2,0,
0,0,70,2,16,128,
65,0,0,0,5,0,
0,0,52,0,0,9,
66,0,16,0,1,0,
0,0,26,0,16,128,
129,0,0,0,2,0,
0,0,10,0,16,128,
129,0,0,0,2,0,
0,0,52,0,0,8,
18,0,16,0,2,0,
0,0,42,0,16,128,
129,0,0,0,2,0,
0,0,42,0,16,0,
1,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,5,0,0,0,
230,26,16,0,3,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
0,0,0,8,114,0,
16,0,4,0,0,0,
70,2,16,0,4,0,
0,0,70,2,16,128,
65,0,0,0,5,0,
0,0,52,0,0,9,
66,0,16,0,1,0,
0,0,26,0,16,128,
129,0,0,0,4,0,
0,0,10,0,16,128,
129,0,0,0,4,0,
0,0,52,0,0,8,
34,0,16,0,2,0,
0,0,42,0,16,128,
129,0,0,0,4,0,
0,0,42,0,16,0,
1,0,0,0,52,0,
0,7,50,0,16,0,
1,0,0,0,70,0,
16,0,1,0,0,0,
70,0,16,0,2,0,
0,0,52,0,0,7,
18,0,16,0,1,0,
0,0,26,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
56,0,0,8,98,0,
16,0,1,0,0,0,
6,1,16,0,3,0,
0,0,86,133,32,0,
1,0,0,0,4,0,
0,0,29,0,0,7,
50,0,16,0,1,0,
0,0,150,5,16,0,
1,0,0,0,6,0,
16,0,1,0,0,0,
1,0,0,10,50,0,
16,0,1,0,0,0,
70,0,16,0,1,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,0,0,
0,0,0,0,56,0,
0,7,50,0,16,0,
0,0,0,0,230,10,
16,0,0,0,0,0,
70,0,16,0,1,0,
0,0,18,0,0,1,
54,0,0,8,50,0,
16,0,0,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,21,0,0,1,
21,0,0,1,54,0,
0,5,50,32,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
54,0,0,8,194,32,
16,0,0,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,62,0,0,1,
83,84,65,84,148,0,
0,0,78,0,0,0,
7,0,0,0,0,0,
0,0,5,0,0,0,
48,0,0,0,0,0,
0,0,4,0,0,0,
3,0,0,0,2,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,14,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
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
class AntiAliasingShadersSmaaSmaaPass0ShaderDef : public ShaderDef
{
public:
	AntiAliasingShadersSmaaSmaaPass0ShaderDef() : ShaderDef{}
	{
		Name = "smaa-pass0";
		VertexByteCode = RetroArchAntiAliasingShadersSmaaSmaaPass0ShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchAntiAliasingShadersSmaaSmaaPass0ShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchAntiAliasingShadersSmaaSmaaPass0ShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchAntiAliasingShadersSmaaSmaaPass0ShaderDefs::sFragmentByteCode);
		Format = "";
		Params.push_back(ShaderParam("MVP", 0, 0, 64, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("SourceSize", -1, 0, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OriginalSize", -1, 16, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OutputSize", -1, 32, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("FrameCount", -1, 48, 4, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("SMAA_EDT", -1, 52, 4, 0.000000f, 1.000000f, 1.000000f, 1.000000f, "SMAA Edge Detection: Luma | Color"));
		Params.push_back(ShaderParam("SMAA_THRESHOLD", -1, 56, 4, 0.010000f, 0.500000f, 0.050000f, 0.010000f, "SMAA Threshold"));
		Params.push_back(ShaderParam("SMAA_MAX_SEARCH_STEPS", -1, 60, 4, 4.000000f, 112.000000f, 32.000000f, 1.000000f, "SMAA Max Search Steps"));
		Params.push_back(ShaderParam("SMAA_MAX_SEARCH_STEPS_DIAG", -1, 64, 4, 4.000000f, 20.000000f, 16.000000f, 1.000000f, "SMAA Max Search Steps Diagonal"));
		Params.push_back(ShaderParam("SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR", -1, 68, 4, 1.000000f, 4.000000f, 2.000000f, 0.100000f, "SMAA Local Contrast Adapt. Factor"));
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
