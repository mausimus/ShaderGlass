/*
ShaderGlass shader ddt-shaders\ddt-xbr-lv1 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ddt/shaders/ddt-xbr-lv1.slang
See original file for full credits and usage license with excerpts below. 
This file is auto-generated, do not modify directly.


Hyllian's DDT-xBR-lv1 Shader

Copyright (C) 2011-2022 Hyllian/Jararaca - sergiogdb@gmail.com

Sharpness control - Copyright (c) 2022 Filippo Scognamiglio

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.



// set to 1.0 to use dynamic sharpening
// Set to 1.0 to bias the interpolation towards sharpening
// Minimum amount of sharpening in range [0.0, 1.0]
// Maximum amount of sharpening in range [0.0, 1.0]
// If USE_DYNAMIC_SHARPNESS is 0 apply this static sharpness
// Fast computation of barycentric coordinates only in the sub-triangle 1 2 4
// Alter colors and coordinates to compute the other triangle.
//    A1 B1
// A0 A  B  B2
// C0 C  D  D2
//    C3 D3

*/

#pragma once

namespace RetroArchDdtShadersDdtXbrLv1ShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,15,146,
48,159,179,2,1,128,
156,89,211,81,249,2,
133,154,1,0,0,0,
184,7,0,0,5,0,
0,0,52,0,0,0,
124,4,0,0,200,4,
0,0,80,5,0,0,
28,7,0,0,82,68,
69,70,64,4,0,0,
2,0,0,0,136,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
22,4,0,0,82,68,
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
0,0,10,0,0,0,
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
235,0,0,0,168,2,
0,0,0,0,0,0,
16,0,0,0,2,0,
0,0,196,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,232,2,0,0,
16,0,0,0,16,0,
0,0,0,0,0,0,
196,2,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
252,2,0,0,32,0,
0,0,16,0,0,0,
0,0,0,0,196,2,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,14,3,
0,0,48,0,0,0,
4,0,0,0,0,0,
0,0,40,3,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,76,3,0,0,
52,0,0,0,4,0,
0,0,0,0,0,0,
112,3,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
148,3,0,0,56,0,
0,0,4,0,0,0,
0,0,0,0,112,3,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,175,3,
0,0,60,0,0,0,
4,0,0,0,0,0,
0,0,112,3,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,204,3,0,0,
64,0,0,0,4,0,
0,0,0,0,0,0,
112,3,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
233,3,0,0,68,0,
0,0,4,0,0,0,
0,0,0,0,112,3,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,1,4,
0,0,72,0,0,0,
4,0,0,0,0,0,
0,0,112,3,0,0,
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
0,0,186,2,0,0,
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
32,3,0,0,112,97,
114,97,109,115,95,85,
83,69,95,68,89,78,
65,77,73,67,95,83,
72,65,82,80,78,69,
83,83,0,102,108,111,
97,116,0,171,0,0,
3,0,1,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
105,3,0,0,112,97,
114,97,109,115,95,85,
83,69,95,83,72,65,
82,80,69,78,73,78,
71,95,66,73,65,83,
0,112,97,114,97,109,
115,95,68,89,78,65,
77,73,67,95,83,72,
65,82,80,78,69,83,
83,95,77,73,78,0,
112,97,114,97,109,115,
95,68,89,78,65,77,
73,67,95,83,72,65,
82,80,78,69,83,83,
95,77,65,88,0,112,
97,114,97,109,115,95,
83,84,65,84,73,67,
95,83,72,65,82,80,
78,69,83,83,0,112,
97,114,97,109,115,95,
68,68,84,95,84,72,
82,69,83,72,79,76,
68,0,77,105,99,114,
111,115,111,102,116,32,
40,82,41,32,72,76,
83,76,32,83,104,97,
100,101,114,32,67,111,
109,112,105,108,101,114,
32,49,48,46,49,0,
171,171,73,83,71,78,
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
79,83,71,78,128,0,
0,0,4,0,0,0,
8,0,0,0,104,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
3,12,0,0,104,0,
0,0,2,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
12,3,0,0,104,0,
0,0,1,0,0,0,
0,0,0,0,3,0,
0,0,1,0,0,0,
15,0,0,0,113,0,
0,0,0,0,0,0,
1,0,0,0,3,0,
0,0,2,0,0,0,
15,0,0,0,84,69,
88,67,79,79,82,68,
0,83,86,95,80,111,
115,105,116,105,111,110,
0,171,171,171,83,72,
69,88,196,1,0,0,
80,0,1,0,113,0,
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
194,32,16,0,0,0,
0,0,101,0,0,3,
242,32,16,0,1,0,
0,0,103,0,0,4,
242,32,16,0,2,0,
0,0,1,0,0,0,
104,0,0,2,1,0,
0,0,56,0,0,10,
50,0,16,0,0,0,
0,0,70,16,16,0,
1,0,0,0,2,64,
0,0,71,3,128,63,
71,3,128,63,0,0,
0,0,0,0,0,0,
56,0,0,8,194,32,
16,0,0,0,0,0,
6,4,16,0,0,0,
0,0,6,132,32,0,
1,0,0,0,0,0,
0,0,56,0,0,10,
50,32,16,0,0,0,
0,0,70,16,16,0,
1,0,0,0,2,64,
0,0,71,3,128,63,
71,3,128,63,0,0,
0,0,0,0,0,0,
54,0,0,6,146,32,
16,0,1,0,0,0,
166,142,32,0,1,0,
0,0,0,0,0,0,
54,0,0,8,98,32,
16,0,1,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
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
148,0,0,0,10,0,
0,0,1,0,0,0,
0,0,0,0,6,0,
0,0,7,0,0,0,
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
68,88,66,67,243,95,
132,162,147,212,112,150,
214,157,173,108,91,182,
50,10,1,0,0,0,
240,23,0,0,5,0,
0,0,52,0,0,0,
52,4,0,0,152,4,
0,0,204,4,0,0,
84,23,0,0,82,68,
69,70,248,3,0,0,
1,0,0,0,184,0,
0,0,3,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
206,3,0,0,82,68,
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
0,0,10,0,0,0,
208,0,0,0,80,0,
0,0,0,0,0,0,
0,0,0,0,96,2,
0,0,0,0,0,0,
16,0,0,0,0,0,
0,0,124,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,160,2,0,0,
16,0,0,0,16,0,
0,0,0,0,0,0,
124,2,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
180,2,0,0,32,0,
0,0,16,0,0,0,
0,0,0,0,124,2,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,198,2,
0,0,48,0,0,0,
4,0,0,0,0,0,
0,0,224,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,4,3,0,0,
52,0,0,0,4,0,
0,0,2,0,0,0,
40,3,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
76,3,0,0,56,0,
0,0,4,0,0,0,
2,0,0,0,40,3,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,103,3,
0,0,60,0,0,0,
4,0,0,0,2,0,
0,0,40,3,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,132,3,0,0,
64,0,0,0,4,0,
0,0,2,0,0,0,
40,3,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
161,3,0,0,68,0,
0,0,4,0,0,0,
2,0,0,0,40,3,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,185,3,
0,0,72,0,0,0,
4,0,0,0,2,0,
0,0,40,3,0,0,
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
0,0,114,2,0,0,
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
216,2,0,0,112,97,
114,97,109,115,95,85,
83,69,95,68,89,78,
65,77,73,67,95,83,
72,65,82,80,78,69,
83,83,0,102,108,111,
97,116,0,171,0,0,
3,0,1,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
33,3,0,0,112,97,
114,97,109,115,95,85,
83,69,95,83,72,65,
82,80,69,78,73,78,
71,95,66,73,65,83,
0,112,97,114,97,109,
115,95,68,89,78,65,
77,73,67,95,83,72,
65,82,80,78,69,83,
83,95,77,73,78,0,
112,97,114,97,109,115,
95,68,89,78,65,77,
73,67,95,83,72,65,
82,80,78,69,83,83,
95,77,65,88,0,112,
97,114,97,109,115,95,
83,84,65,84,73,67,
95,83,72,65,82,80,
78,69,83,83,0,112,
97,114,97,109,115,95,
68,68,84,95,84,72,
82,69,83,72,79,76,
68,0,77,105,99,114,
111,115,111,102,116,32,
40,82,41,32,72,76,
83,76,32,83,104,97,
100,101,114,32,67,111,
109,112,105,108,101,114,
32,49,48,46,49,0,
171,171,73,83,71,78,
92,0,0,0,3,0,
0,0,8,0,0,0,
80,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,3,0,0,
80,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,12,0,0,
80,0,0,0,1,0,
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
69,88,128,18,0,0,
80,0,0,0,160,4,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,1,0,0,0,
5,0,0,0,90,0,
0,3,0,96,16,0,
2,0,0,0,88,24,
0,4,0,112,16,0,
2,0,0,0,85,85,
0,0,98,16,0,3,
50,16,16,0,0,0,
0,0,98,16,0,3,
194,16,16,0,0,0,
0,0,98,16,0,3,
242,16,16,0,1,0,
0,0,101,0,0,3,
242,32,16,0,0,0,
0,0,104,0,0,2,
11,0,0,0,26,0,
0,5,50,0,16,0,
0,0,0,0,230,26,
16,0,0,0,0,0,
0,0,0,10,50,0,
16,0,0,0,0,0,
70,0,16,0,0,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,191,0,0,0,0,
0,0,0,0,49,0,
0,10,242,0,16,0,
1,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
70,4,16,0,0,0,
0,0,49,0,0,10,
242,0,16,0,2,0,
0,0,70,4,16,0,
0,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
54,0,0,6,50,0,
16,0,0,0,0,0,
70,0,16,128,129,0,
0,0,0,0,0,0,
30,0,0,8,242,0,
16,0,1,0,0,0,
70,14,16,128,65,0,
0,0,1,0,0,0,
70,14,16,0,2,0,
0,0,43,0,0,5,
242,0,16,0,1,0,
0,0,70,14,16,0,
1,0,0,0,50,0,
0,10,242,0,16,0,
2,0,0,0,230,4,
16,128,65,0,0,0,
1,0,0,0,230,20,
16,0,1,0,0,0,
70,20,16,0,0,0,
0,0,50,0,0,9,
50,0,16,0,3,0,
0,0,230,10,16,0,
1,0,0,0,230,26,
16,0,1,0,0,0,
230,10,16,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
3,0,0,0,70,0,
16,0,3,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,16,0,
0,10,130,0,16,0,
0,0,0,0,70,2,
16,0,3,0,0,0,
2,64,0,0,135,22,
153,62,162,69,22,63,
213,120,233,61,0,0,
0,0,50,0,0,9,
242,0,16,0,3,0,
0,0,70,14,16,0,
1,0,0,0,70,30,
16,0,1,0,0,0,
70,20,16,0,0,0,
0,0,50,0,0,10,
50,0,16,0,4,0,
0,0,230,10,16,128,
65,0,0,0,1,0,
0,0,230,26,16,0,
1,0,0,0,70,0,
16,0,3,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,4,0,
0,0,70,0,16,0,
4,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,16,0,0,10,
18,0,16,0,4,0,
0,0,70,2,16,0,
4,0,0,0,2,64,
0,0,135,22,153,62,
162,69,22,63,213,120,
233,61,0,0,0,0,
0,0,0,8,34,0,
16,0,4,0,0,0,
58,0,16,128,65,0,
0,0,0,0,0,0,
10,0,16,0,4,0,
0,0,56,0,0,7,
242,0,16,0,5,0,
0,0,70,14,16,0,
1,0,0,0,70,30,
16,0,1,0,0,0,
50,0,0,12,242,0,
16,0,6,0,0,0,
70,14,16,0,5,0,
0,0,2,64,0,0,
0,0,0,64,0,0,
0,64,0,0,0,64,
0,0,0,64,70,20,
16,0,0,0,0,0,
50,0,0,12,50,0,
16,0,1,0,0,0,
230,10,16,0,5,0,
0,0,2,64,0,0,
0,0,0,64,0,0,
0,64,0,0,0,0,
0,0,0,0,70,0,
16,0,3,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,5,0,
0,0,70,0,16,0,
1,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,16,0,0,10,
18,0,16,0,1,0,
0,0,70,2,16,0,
5,0,0,0,2,64,
0,0,135,22,153,62,
162,69,22,63,213,120,
233,61,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,5,0,
0,0,70,0,16,0,
6,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,16,0,0,10,
34,0,16,0,1,0,
0,0,70,2,16,0,
5,0,0,0,2,64,
0,0,135,22,153,62,
162,69,22,63,213,120,
233,61,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,5,0,
0,0,230,10,16,0,
6,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,50,0,0,9,
194,0,16,0,4,0,
0,0,166,14,16,0,
1,0,0,0,166,30,
16,0,1,0,0,0,
6,4,16,0,6,0,
0,0,50,0,0,9,
194,0,16,0,1,0,
0,0,166,14,16,0,
1,0,0,0,166,30,
16,0,1,0,0,0,
6,4,16,0,3,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
6,0,0,0,230,10,
16,0,1,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,7,0,0,0,
230,10,16,0,4,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
16,0,0,10,66,0,
16,0,1,0,0,0,
70,2,16,0,7,0,
0,0,2,64,0,0,
135,22,153,62,162,69,
22,63,213,120,233,61,
0,0,0,0,16,0,
0,10,130,0,16,0,
1,0,0,0,70,2,
16,0,5,0,0,0,
2,64,0,0,135,22,
153,62,162,69,22,63,
213,120,233,61,0,0,
0,0,0,0,0,8,
66,0,16,0,4,0,
0,0,58,0,16,128,
65,0,0,0,1,0,
0,0,26,0,16,0,
1,0,0,0,0,0,
0,9,34,0,16,0,
4,0,0,0,42,0,
16,128,129,0,0,0,
4,0,0,0,26,0,
16,128,129,0,0,0,
4,0,0,0,16,0,
0,10,66,0,16,0,
4,0,0,0,70,2,
16,0,6,0,0,0,
2,64,0,0,135,22,
153,62,162,69,22,63,
213,120,233,61,0,0,
0,0,0,0,0,8,
162,0,16,0,1,0,
0,0,86,13,16,128,
65,0,0,0,1,0,
0,0,166,10,16,0,
4,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,5,0,0,0,
70,16,16,0,0,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
16,0,0,10,130,0,
16,0,4,0,0,0,
70,2,16,0,5,0,
0,0,2,64,0,0,
135,22,153,62,162,69,
22,63,213,120,233,61,
0,0,0,0,0,0,
0,8,18,0,16,0,
4,0,0,0,10,0,
16,128,65,0,0,0,
4,0,0,0,58,0,
16,0,4,0,0,0,
0,0,0,8,130,0,
16,0,0,0,0,0,
58,0,16,128,65,0,
0,0,0,0,0,0,
58,0,16,0,4,0,
0,0,0,0,0,9,
130,0,16,0,0,0,
0,0,58,0,16,128,
129,0,0,0,0,0,
0,0,10,0,16,128,
129,0,0,0,4,0,
0,0,0,0,0,8,
130,0,16,0,0,0,
0,0,26,0,16,128,
129,0,0,0,1,0,
0,0,58,0,16,0,
0,0,0,0,0,0,
0,8,130,0,16,0,
0,0,0,0,58,0,
16,128,129,0,0,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,7,0,
0,0,70,0,16,0,
3,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
3,0,0,0,230,10,
16,0,3,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,16,0,
0,10,34,0,16,0,
1,0,0,0,70,2,
16,0,7,0,0,0,
2,64,0,0,135,22,
153,62,162,69,22,63,
213,120,233,61,0,0,
0,0,16,0,0,10,
130,0,16,0,1,0,
0,0,70,2,16,0,
3,0,0,0,2,64,
0,0,135,22,153,62,
162,69,22,63,213,120,
233,61,0,0,0,0,
0,0,0,8,130,0,
16,0,3,0,0,0,
58,0,16,128,65,0,
0,0,1,0,0,0,
26,0,16,0,1,0,
0,0,50,0,0,10,
130,0,16,0,0,0,
0,0,58,0,16,128,
129,0,0,0,3,0,
0,0,1,64,0,0,
0,0,128,64,58,0,
16,0,0,0,0,0,
0,0,0,8,130,0,
16,0,0,0,0,0,
26,0,16,128,65,0,
0,0,4,0,0,0,
58,0,16,0,0,0,
0,0,50,0,0,10,
130,0,16,0,3,0,
0,0,42,128,32,0,
1,0,0,0,4,0,
0,0,1,64,0,0,
205,204,204,61,58,0,
16,0,0,0,0,0,
56,0,0,8,130,0,
16,0,3,0,0,0,
58,0,16,0,3,0,
0,0,42,128,32,0,
1,0,0,0,4,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
8,0,0,0,70,0,
16,0,2,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,2,0,0,0,
230,10,16,0,2,0,
0,0,70,126,16,0,
2,0,0,0,0,96,
16,0,2,0,0,0,
16,0,0,10,18,0,
16,0,2,0,0,0,
70,2,16,0,2,0,
0,0,2,64,0,0,
135,22,153,62,162,69,
22,63,213,120,233,61,
0,0,0,0,16,0,
0,10,34,0,16,0,
2,0,0,0,70,2,
16,0,8,0,0,0,
2,64,0,0,135,22,
153,62,162,69,22,63,
213,120,233,61,0,0,
0,0,0,0,0,8,
66,0,16,0,2,0,
0,0,42,0,16,128,
65,0,0,0,1,0,
0,0,26,0,16,0,
2,0,0,0,0,0,
0,8,66,0,16,0,
1,0,0,0,42,0,
16,128,65,0,0,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
0,0,0,8,34,0,
16,0,2,0,0,0,
26,0,16,0,1,0,
0,0,26,0,16,128,
65,0,0,0,2,0,
0,0,0,0,0,9,
66,0,16,0,1,0,
0,0,42,0,16,128,
129,0,0,0,1,0,
0,0,26,0,16,128,
129,0,0,0,2,0,
0,0,0,0,0,8,
34,0,16,0,2,0,
0,0,10,0,16,128,
65,0,0,0,1,0,
0,0,10,0,16,0,
2,0,0,0,0,0,
0,8,18,0,16,0,
1,0,0,0,10,0,
16,128,65,0,0,0,
1,0,0,0,58,0,
16,0,1,0,0,0,
0,0,0,8,18,0,
16,0,2,0,0,0,
58,0,16,0,1,0,
0,0,10,0,16,128,
65,0,0,0,2,0,
0,0,0,0,0,8,
66,0,16,0,1,0,
0,0,42,0,16,0,
1,0,0,0,10,0,
16,128,129,0,0,0,
2,0,0,0,0,0,
0,8,18,0,16,0,
1,0,0,0,10,0,
16,128,129,0,0,0,
1,0,0,0,42,0,
16,0,1,0,0,0,
0,0,0,9,66,0,
16,0,1,0,0,0,
26,0,16,128,129,0,
0,0,2,0,0,0,
42,0,16,128,129,0,
0,0,2,0,0,0,
0,0,0,8,18,0,
16,0,2,0,0,0,
42,0,16,128,65,0,
0,0,4,0,0,0,
58,0,16,0,4,0,
0,0,50,0,0,10,
18,0,16,0,1,0,
0,0,10,0,16,128,
129,0,0,0,2,0,
0,0,1,64,0,0,
0,0,128,64,10,0,
16,0,1,0,0,0,
0,0,0,8,18,0,
16,0,1,0,0,0,
42,0,16,128,65,0,
0,0,1,0,0,0,
10,0,16,0,1,0,
0,0,49,0,0,7,
66,0,16,0,1,0,
0,0,58,0,16,0,
3,0,0,0,10,0,
16,0,1,0,0,0,
50,0,0,10,18,0,
16,0,1,0,0,0,
42,128,32,0,1,0,
0,0,4,0,0,0,
1,64,0,0,205,204,
204,61,10,0,16,0,
1,0,0,0,56,0,
0,8,18,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
42,128,32,0,1,0,
0,0,4,0,0,0,
49,0,0,7,130,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,0,0,
0,8,18,0,16,0,
1,0,0,0,26,0,
16,128,65,0,0,0,
1,0,0,0,42,0,
16,0,4,0,0,0,
0,0,0,8,114,0,
16,0,2,0,0,0,
118,15,16,128,65,0,
0,0,1,0,0,0,
230,11,16,0,4,0,
0,0,0,0,0,9,
34,0,16,0,2,0,
0,0,42,0,16,128,
129,0,0,0,2,0,
0,0,26,0,16,128,
129,0,0,0,2,0,
0,0,0,0,0,8,
18,0,16,0,2,0,
0,0,10,0,16,128,
129,0,0,0,2,0,
0,0,26,0,16,0,
2,0,0,0,0,0,
0,8,18,0,16,0,
1,0,0,0,10,0,
16,128,129,0,0,0,
1,0,0,0,10,0,
16,0,2,0,0,0,
49,0,0,7,18,0,
16,0,1,0,0,0,
1,64,0,0,0,0,
0,0,10,0,16,0,
1,0,0,0,1,0,
0,7,66,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
42,0,16,0,1,0,
0,0,1,0,0,7,
130,0,16,0,0,0,
0,0,58,0,16,0,
0,0,0,0,10,0,
16,0,1,0,0,0,
52,0,0,7,18,0,
16,0,1,0,0,0,
42,0,16,0,4,0,
0,0,58,0,16,0,
1,0,0,0,51,0,
0,7,130,0,16,0,
1,0,0,0,42,0,
16,0,4,0,0,0,
58,0,16,0,1,0,
0,0,52,0,0,7,
18,0,16,0,2,0,
0,0,26,0,16,0,
1,0,0,0,58,0,
16,0,4,0,0,0,
51,0,0,7,34,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,58,0,16,0,
4,0,0,0,51,0,
0,7,34,0,16,0,
1,0,0,0,58,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,52,0,0,7,
18,0,16,0,1,0,
0,0,10,0,16,0,
1,0,0,0,10,0,
16,0,2,0,0,0,
0,0,0,7,130,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,10,0,16,0,
1,0,0,0,0,0,
0,8,18,0,16,0,
1,0,0,0,26,0,
16,128,65,0,0,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
0,0,0,7,34,0,
16,0,1,0,0,0,
58,0,16,0,1,0,
0,0,1,64,0,0,
205,204,76,61,14,0,
0,7,18,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,75,0,0,5,
34,0,16,0,1,0,
0,0,10,0,16,0,
1,0,0,0,24,0,
0,11,50,0,16,0,
2,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
0,0,0,0,0,0,
150,133,32,0,1,0,
0,0,3,0,0,0,
55,0,0,9,18,0,
16,0,1,0,0,0,
26,0,16,0,2,0,
0,0,26,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
56,0,0,8,34,0,
16,0,1,0,0,0,
1,64,0,0,0,0,
0,63,58,128,32,0,
1,0,0,0,3,0,
0,0,50,0,0,11,
130,0,16,0,1,0,
0,0,10,128,32,0,
1,0,0,0,4,0,
0,0,1,64,0,0,
0,0,0,63,26,0,
16,128,65,0,0,0,
1,0,0,0,50,0,
0,9,18,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
58,0,16,0,1,0,
0,0,26,0,16,0,
1,0,0,0,56,0,
0,8,34,0,16,0,
1,0,0,0,1,64,
0,0,0,0,0,63,
26,128,32,0,1,0,
0,0,4,0,0,0,
55,0,0,9,18,0,
16,0,1,0,0,0,
10,0,16,0,2,0,
0,0,10,0,16,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
0,0,0,11,50,0,
16,0,2,0,0,0,
70,0,16,128,65,0,
0,0,0,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,0,0,0,0,
0,0,0,0,0,8,
34,0,16,0,1,0,
0,0,26,0,16,128,
65,0,0,0,0,0,
0,0,10,0,16,0,
2,0,0,0,0,0,
0,8,34,0,16,0,
1,0,0,0,10,0,
16,128,65,0,0,0,
1,0,0,0,26,0,
16,0,1,0,0,0,
50,0,0,9,130,0,
16,0,1,0,0,0,
10,0,16,0,1,0,
0,0,1,64,0,0,
0,0,0,192,1,64,
0,0,0,0,128,63,
14,32,0,7,34,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,58,0,16,0,
1,0,0,0,0,0,
0,8,50,0,16,0,
4,0,0,0,70,0,
16,0,0,0,0,0,
6,0,16,128,65,0,
0,0,1,0,0,0,
14,32,0,7,50,0,
16,0,4,0,0,0,
70,0,16,0,4,0,
0,0,246,15,16,0,
1,0,0,0,56,0,
0,7,114,0,16,0,
8,0,0,0,6,0,
16,0,4,0,0,0,
70,2,16,0,7,0,
0,0,50,0,0,9,
114,0,16,0,8,0,
0,0,70,2,16,0,
5,0,0,0,86,5,
16,0,1,0,0,0,
70,2,16,0,8,0,
0,0,0,0,0,8,
34,0,16,0,1,0,
0,0,26,0,16,128,
65,0,0,0,1,0,
0,0,1,64,0,0,
0,0,128,63,0,0,
0,8,34,0,16,0,
1,0,0,0,10,0,
16,128,65,0,0,0,
4,0,0,0,26,0,
16,0,1,0,0,0,
50,0,0,9,114,0,
16,0,8,0,0,0,
70,2,16,0,3,0,
0,0,86,5,16,0,
1,0,0,0,70,2,
16,0,8,0,0,0,
0,0,0,8,114,0,
16,0,9,0,0,0,
70,2,16,128,65,0,
0,0,3,0,0,0,
70,2,16,0,6,0,
0,0,50,0,0,9,
114,0,16,0,9,0,
0,0,6,0,16,0,
4,0,0,0,70,2,
16,0,9,0,0,0,
70,2,16,0,3,0,
0,0,0,0,0,8,
114,0,16,0,10,0,
0,0,70,2,16,128,
65,0,0,0,5,0,
0,0,70,2,16,0,
7,0,0,0,50,0,
0,9,210,0,16,0,
4,0,0,0,6,0,
16,0,4,0,0,0,
6,9,16,0,10,0,
0,0,6,9,16,0,
5,0,0,0,0,0,
0,8,114,0,16,0,
9,0,0,0,134,3,
16,128,65,0,0,0,
4,0,0,0,70,2,
16,0,9,0,0,0,
50,0,0,9,114,0,
16,0,4,0,0,0,
86,5,16,0,4,0,
0,0,70,2,16,0,
9,0,0,0,134,3,
16,0,4,0,0,0,
55,0,0,9,114,0,
16,0,4,0,0,0,
166,10,16,0,1,0,
0,0,70,2,16,0,
8,0,0,0,70,2,
16,0,4,0,0,0,
0,0,0,8,66,0,
16,0,0,0,0,0,
10,0,16,128,65,0,
0,0,0,0,0,0,
1,64,0,0,0,0,
128,63,0,0,0,8,
66,0,16,0,2,0,
0,0,10,0,16,128,
65,0,0,0,2,0,
0,0,1,64,0,0,
0,0,128,63,49,0,
0,7,18,0,16,0,
0,0,0,0,26,0,
16,0,2,0,0,0,
10,0,16,0,2,0,
0,0,55,0,0,9,
98,0,16,0,0,0,
0,0,6,0,16,0,
0,0,0,0,166,9,
16,0,2,0,0,0,
86,6,16,0,0,0,
0,0,55,0,0,9,
114,0,16,0,2,0,
0,0,6,0,16,0,
0,0,0,0,70,2,
16,0,3,0,0,0,
70,2,16,0,7,0,
0,0,0,0,0,8,
18,0,16,0,0,0,
0,0,10,0,16,128,
65,0,0,0,1,0,
0,0,26,0,16,0,
0,0,0,0,14,32,
0,7,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
58,0,16,0,1,0,
0,0,56,0,0,7,
114,0,16,0,3,0,
0,0,6,0,16,0,
0,0,0,0,70,2,
16,0,6,0,0,0,
0,0,0,8,34,0,
16,0,0,0,0,0,
26,0,16,128,65,0,
0,0,0,0,0,0,
1,64,0,0,0,0,
128,63,0,0,0,8,
34,0,16,0,0,0,
0,0,42,0,16,128,
65,0,0,0,0,0,
0,0,26,0,16,0,
0,0,0,0,0,0,
0,8,34,0,16,0,
0,0,0,0,10,0,
16,128,65,0,0,0,
1,0,0,0,26,0,
16,0,0,0,0,0,
14,32,0,7,34,0,
16,0,0,0,0,0,
26,0,16,0,0,0,
0,0,58,0,16,0,
1,0,0,0,50,0,
0,9,114,0,16,0,
1,0,0,0,70,2,
16,0,2,0,0,0,
86,5,16,0,0,0,
0,0,70,2,16,0,
3,0,0,0,0,0,
0,8,34,0,16,0,
0,0,0,0,26,0,
16,128,65,0,0,0,
0,0,0,0,1,64,
0,0,0,0,128,63,
0,0,0,8,18,0,
16,0,0,0,0,0,
10,0,16,128,65,0,
0,0,0,0,0,0,
26,0,16,0,0,0,
0,0,50,0,0,9,
114,0,16,0,0,0,
0,0,70,2,16,0,
5,0,0,0,6,0,
16,0,0,0,0,0,
70,2,16,0,1,0,
0,0,55,0,0,9,
114,32,16,0,0,0,
0,0,246,15,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
70,2,16,0,4,0,
0,0,54,0,0,5,
130,32,16,0,0,0,
0,0,1,64,0,0,
0,0,128,63,62,0,
0,1,83,84,65,84,
148,0,0,0,135,0,
0,0,11,0,0,0,
0,0,0,0,4,0,
0,0,110,0,0,0,
1,0,0,0,2,0,
0,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
12,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,1,0,0,0,
6,0,0,0,2,0,
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
class DdtShadersDdtXbrLv1ShaderDef : public ShaderDef
{
public:
	DdtShadersDdtXbrLv1ShaderDef() : ShaderDef{}
	{
		Name = "ddt-xbr-lv1";
		VertexByteCode = RetroArchDdtShadersDdtXbrLv1ShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchDdtShadersDdtXbrLv1ShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchDdtShadersDdtXbrLv1ShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchDdtShadersDdtXbrLv1ShaderDefs::sFragmentByteCode);
		Params.push_back(ShaderParam("USE_DYNAMIC_SHARPNESS", -1, 52, 4, 0.000000f, 1.000000f, 1.000000f, 1.000000f, "Dynamic Sharpness [ 0FF | ON ]"));
		Params.push_back(ShaderParam("USE_SHARPENING_BIAS", -1, 56, 4, 0.000000f, 1.000000f, 1.000000f, 1.000000f, "Sharpness Bias [ 0FF | ON ]"));
		Params.push_back(ShaderParam("DYNAMIC_SHARPNESS_MIN", -1, 60, 4, 0.000000f, 1.000000f, 0.000000f, 0.100000f, "Dynamic Sharpness Min"));
		Params.push_back(ShaderParam("DYNAMIC_SHARPNESS_MAX", -1, 64, 4, 0.000000f, 1.000000f, 0.300000f, 0.100000f, "Dynamic Sharpness Max"));
		Params.push_back(ShaderParam("STATIC_SHARPNESS", -1, 68, 4, 0.000000f, 1.000000f, 0.500000f, 0.100000f, "Static Sharpness"));
		Params.push_back(ShaderParam("DDT_THRESHOLD", -1, 72, 4, 1.000000f, 6.000000f, 2.600000f, 0.200000f, "DDT Diagonal Threshold"));
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