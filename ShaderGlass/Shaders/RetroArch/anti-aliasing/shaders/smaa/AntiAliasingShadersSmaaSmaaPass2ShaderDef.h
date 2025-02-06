/*
ShaderGlass shader anti-aliasing-shaders-smaa\smaa-pass2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/anti-aliasing/shaders/smaa/smaa-pass2.slang
See original file for full credits and usage license with excerpts below. 
This file is auto-generated, do not modify directly.

// SPDX-License-Identifier: Unlicense
//-----------------------------------------------------------------------------
// Neighborhood Blending Shader (Third Pass)
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
