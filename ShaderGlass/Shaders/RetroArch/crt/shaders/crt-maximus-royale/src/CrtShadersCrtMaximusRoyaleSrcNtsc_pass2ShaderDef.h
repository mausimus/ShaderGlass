/*
ShaderGlass shader crt-shaders-crt-maximus-royale-src\ntsc_pass2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/shaders/crt-maximus-royale/src/ntsc_pass2.slang
See original file for full credits and usage license with excerpts below. 
This file is auto-generated, do not modify directly.

/////////////////////////////  GPL LICENSE NOTICE  /////////////////////////////
//  crt-maximus-royale: A fully customizable extension for crt-royale shader,
//  inside a TV / MONITOR BOX with backlight and some other cool stuff.
//  Copyright (C) 2022 DigiDwrf
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

*/

#pragma once

namespace RetroArchCrtShadersCrtMaximusRoyaleSrcNtsc_pass2ShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,47,195,
181,212,139,17,18,201,
205,206,52,95,195,69,
206,111,1,0,0,0,
212,6,0,0,5,0,
0,0,52,0,0,0,
192,2,0,0,12,3,
0,0,124,3,0,0,
56,6,0,0,82,68,
69,70,132,2,0,0,
2,0,0,0,136,0,
0,0,2,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
91,2,0,0,82,68,
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
0,0,3,0,0,0,
184,0,0,0,96,0,
0,0,0,0,0,0,
0,0,0,0,128,0,
0,0,2,0,0,0,
188,1,0,0,16,0,
0,0,0,0,0,0,
0,0,0,0,48,1,
0,0,0,0,0,0,
64,0,0,0,2,0,
0,0,68,1,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,104,1,0,0,
64,0,0,0,16,0,
0,0,2,0,0,0,
132,1,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
168,1,0,0,80,0,
0,0,16,0,0,0,
2,0,0,0,132,1,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,103,108,
111,98,97,108,95,77,
86,80,0,102,108,111,
97,116,52,120,52,0,
2,0,3,0,4,0,
4,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,59,1,0,0,
103,108,111,98,97,108,
95,79,114,105,103,105,
110,97,108,83,105,122,
101,0,102,108,111,97,
116,52,0,171,1,0,
3,0,1,0,4,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
124,1,0,0,103,108,
111,98,97,108,95,83,
111,117,114,99,101,83,
105,122,101,0,171,171,
12,2,0,0,0,0,
0,0,4,0,0,0,
2,0,0,0,44,2,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,80,2,
0,0,4,0,0,0,
4,0,0,0,2,0,
0,0,44,2,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,112,97,114,97,
109,115,95,67,86,66,
83,95,83,86,73,68,
69,79,95,82,71,66,
0,102,108,111,97,116,
0,171,171,171,0,0,
3,0,1,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
35,2,0,0,112,97,
114,97,109,115,95,104,
50,120,0,77,105,99,
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
4,11,0,0,89,0,
0,0,0,0,0,0,
1,0,0,0,3,0,
0,0,1,0,0,0,
15,0,0,0,84,69,
88,67,79,79,82,68,
0,83,86,95,80,111,
115,105,116,105,111,110,
0,171,171,171,83,72,
69,88,180,2,0,0,
80,0,1,0,173,0,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,0,0,0,0,
6,0,0,0,89,0,
0,4,70,142,32,0,
1,0,0,0,1,0,
0,0,95,0,0,3,
242,16,16,0,0,0,
0,0,95,0,0,3,
50,16,16,0,1,0,
0,0,101,0,0,3,
50,32,16,0,0,0,
0,0,101,0,0,3,
66,32,16,0,0,0,
0,0,103,0,0,4,
242,32,16,0,1,0,
0,0,1,0,0,0,
104,0,0,2,2,0,
0,0,49,0,0,8,
18,0,16,0,0,0,
0,0,1,64,0,0,
0,0,250,67,10,128,
32,0,0,0,0,0,
4,0,0,0,24,0,
0,11,98,0,16,0,
0,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
128,63,0,0,0,0,
86,133,32,0,1,0,
0,0,0,0,0,0,
60,0,0,7,18,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,56,0,
0,11,114,0,16,0,
1,0,0,0,2,64,
0,0,0,0,0,63,
0,0,0,62,0,0,
128,62,0,0,0,0,
6,128,32,0,0,0,
0,0,5,0,0,0,
55,0,0,9,18,0,
16,0,0,0,0,0,
10,0,16,0,0,0,
0,0,42,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
55,0,0,9,66,0,
16,0,0,0,0,0,
26,0,16,0,0,0,
0,0,26,0,16,0,
1,0,0,0,10,0,
16,0,0,0,0,0,
14,0,0,7,18,0,
16,0,1,0,0,0,
1,64,0,0,0,0,
0,63,42,0,16,0,
0,0,0,0,54,0,
0,5,34,0,16,0,
1,0,0,0,1,64,
0,0,0,0,0,0,
0,0,0,8,50,0,
16,0,0,0,0,0,
70,0,16,128,65,0,
0,0,1,0,0,0,
70,16,16,0,1,0,
0,0,49,0,0,8,
130,0,16,0,0,0,
0,0,10,128,32,0,
1,0,0,0,0,0,
0,0,1,64,0,0,
0,0,0,64,54,0,
0,5,50,0,16,0,
1,0,0,0,70,16,
16,0,1,0,0,0,
56,0,0,8,66,0,
16,0,1,0,0,0,
1,64,0,0,0,0,
0,63,10,128,32,0,
0,0,0,0,5,0,
0,0,55,0,0,9,
114,32,16,0,0,0,
0,0,246,15,16,0,
0,0,0,0,70,2,
16,0,0,0,0,0,
70,2,16,0,1,0,
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
148,0,0,0,18,0,
0,0,2,0,0,0,
0,0,0,0,5,0,
0,0,11,0,0,0,
0,0,0,0,1,0,
0,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
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
0,0
};


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,43,240,
58,187,26,71,183,252,
228,168,205,173,51,134,
227,205,1,0,0,0,
140,8,0,0,5,0,
0,0,52,0,0,0,
212,1,0,0,32,2,
0,0,84,2,0,0,
240,7,0,0,82,68,
69,70,152,1,0,0,
1,0,0,0,184,0,
0,0,3,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
111,1,0,0,82,68,
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
0,0,2,0,0,0,
208,0,0,0,16,0,
0,0,0,0,0,0,
0,0,0,0,32,1,
0,0,0,0,0,0,
4,0,0,0,2,0,
0,0,64,1,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,100,1,0,0,
4,0,0,0,4,0,
0,0,0,0,0,0,
64,1,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
112,97,114,97,109,115,
95,67,86,66,83,95,
83,86,73,68,69,79,
95,82,71,66,0,102,
108,111,97,116,0,171,
171,171,0,0,3,0,
1,0,1,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,55,1,
0,0,112,97,114,97,
109,115,95,104,50,120,
0,77,105,99,114,111,
115,111,102,116,32,40,
82,41,32,72,76,83,
76,32,83,104,97,100,
101,114,32,67,111,109,
112,105,108,101,114,32,
49,48,46,49,0,171,
73,83,71,78,68,0,
0,0,2,0,0,0,
8,0,0,0,56,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
3,3,0,0,56,0,
0,0,1,0,0,0,
0,0,0,0,3,0,
0,0,0,0,0,0,
4,4,0,0,84,69,
88,67,79,79,82,68,
0,171,171,171,79,83,
71,78,44,0,0,0,
1,0,0,0,8,0,
0,0,32,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,15,0,
0,0,83,86,95,84,
97,114,103,101,116,0,
171,171,83,72,69,88,
148,5,0,0,80,0,
0,0,101,1,0,0,
106,8,0,1,53,24,
0,0,102,0,0,0,
130,169,73,183,127,61,
249,184,0,0,0,0,
0,0,0,0,44,198,
185,183,19,62,142,185,
0,0,0,0,0,0,
0,0,75,180,92,183,
188,195,3,186,0,0,
0,0,0,0,0,0,
130,169,73,183,37,3,
116,186,0,0,0,0,
0,0,0,0,139,160,
81,184,235,47,190,186,
0,0,0,0,0,0,
0,0,18,243,238,184,
168,80,7,187,0,0,
0,0,0,0,0,0,
100,21,0,185,187,249,
48,187,0,0,0,0,
0,0,0,0,183,78,
188,182,150,107,84,187,
0,0,0,0,0,0,
0,0,138,204,50,57,
208,2,103,187,0,0,
0,0,0,0,0,0,
159,184,120,57,112,144,
91,187,0,0,0,0,
0,0,0,0,100,225,
49,57,71,76,35,187,
0,0,0,0,0,0,
0,0,101,200,149,57,
125,11,61,186,0,0,
0,0,0,0,0,0,
210,12,129,58,244,220,
13,59,0,0,0,0,
0,0,0,0,225,75,
4,59,156,226,206,59,
0,0,0,0,0,0,
0,0,154,56,3,59,
21,39,65,60,0,0,
0,0,0,0,0,0,
38,133,110,186,17,237,
151,60,0,0,0,0,
0,0,0,0,250,251,
230,187,1,99,216,60,
0,0,0,0,0,0,
0,0,178,164,88,188,
195,197,15,61,0,0,
0,0,0,0,0,0,
77,141,78,188,118,7,
53,61,0,0,0,0,
0,0,0,0,97,70,
33,59,204,239,89,61,
0,0,0,0,0,0,
0,0,143,234,18,61,
22,70,124,61,0,0,
0,0,0,0,0,0,
209,16,172,61,231,232,
140,61,0,0,0,0,
0,0,0,0,40,209,
10,62,15,72,152,61,
0,0,0,0,0,0,
0,0,177,119,51,62,
67,115,159,61,0,0,
0,0,0,0,0,0,
164,189,66,62,57,230,
161,61,0,0,0,0,
0,0,0,0,89,0,
0,4,70,142,32,0,
1,0,0,0,1,0,
0,0,90,0,0,3,
0,96,16,0,2,0,
0,0,88,24,0,4,
0,112,16,0,2,0,
0,0,85,85,0,0,
98,16,0,3,50,16,
16,0,0,0,0,0,
98,16,0,3,66,16,
16,0,0,0,0,0,
101,0,0,3,242,32,
16,0,0,0,0,0,
104,0,0,2,4,0,
0,0,49,0,0,8,
18,0,16,0,0,0,
0,0,10,128,32,0,
1,0,0,0,0,0,
0,0,1,64,0,0,
0,0,0,64,31,0,
4,3,10,0,16,0,
0,0,0,0,14,0,
0,10,18,0,16,0,
0,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
128,63,0,0,128,63,
42,16,16,0,0,0,
0,0,54,0,0,8,
162,0,16,0,1,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,54,0,
0,8,226,0,16,0,
0,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
54,0,0,5,18,0,
16,0,2,0,0,0,
1,64,0,0,0,0,
0,0,48,0,0,1,
33,0,0,7,34,0,
16,0,2,0,0,0,
10,0,16,0,2,0,
0,0,1,64,0,0,
24,0,0,0,3,0,
4,3,26,0,16,0,
2,0,0,0,43,0,
0,5,34,0,16,0,
2,0,0,0,10,0,
16,0,2,0,0,0,
0,0,0,7,66,0,
16,0,2,0,0,0,
26,0,16,0,2,0,
0,0,1,64,0,0,
0,0,192,193,56,0,
0,7,18,0,16,0,
1,0,0,0,10,0,
16,0,0,0,0,0,
42,0,16,0,2,0,
0,0,0,0,0,7,
194,0,16,0,2,0,
0,0,6,4,16,0,
1,0,0,0,6,20,
16,0,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,3,0,
0,0,230,10,16,0,
2,0,0,0,70,126,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,0,0,0,8,
18,0,16,0,1,0,
0,0,26,0,16,128,
65,0,0,0,2,0,
0,0,1,64,0,0,
0,0,192,65,56,0,
0,7,66,0,16,0,
1,0,0,0,10,0,
16,0,0,0,0,0,
10,0,16,0,1,0,
0,0,0,0,0,7,
82,0,16,0,1,0,
0,0,166,11,16,0,
1,0,0,0,6,17,
16,0,0,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
226,0,16,0,2,0,
0,0,134,0,16,0,
1,0,0,0,54,121,
16,0,2,0,0,0,
0,96,16,0,2,0,
0,0,0,0,0,7,
226,0,16,0,2,0,
0,0,86,14,16,0,
2,0,0,0,6,9,
16,0,3,0,0,0,
54,0,0,6,114,0,
16,0,3,0,0,0,
70,145,144,0,10,0,
16,0,2,0,0,0,
50,0,0,9,226,0,
16,0,0,0,0,0,
86,14,16,0,2,0,
0,0,6,9,16,0,
3,0,0,0,86,14,
16,0,0,0,0,0,
30,0,0,7,18,0,
16,0,2,0,0,0,
10,0,16,0,2,0,
0,0,1,64,0,0,
1,0,0,0,22,0,
0,1,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
1,0,0,0,70,16,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,12,114,0,16,0,
0,0,0,0,70,2,
16,0,1,0,0,0,
2,64,0,0,164,189,
66,62,57,230,161,61,
57,230,161,61,0,0,
0,0,150,7,16,0,
0,0,0,0,16,0,
0,10,18,0,16,0,
1,0,0,0,2,64,
0,0,0,0,128,63,
106,188,116,63,219,249,
30,63,0,0,0,0,
70,2,16,0,0,0,
0,0,16,0,0,10,
34,0,16,0,1,0,
0,0,2,64,0,0,
0,0,128,63,150,67,
139,190,2,188,37,191,
0,0,0,0,70,2,
16,0,0,0,0,0,
16,0,0,10,66,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
128,63,104,145,141,191,
85,48,218,63,0,0,
0,0,70,2,16,0,
0,0,0,0,18,0,
0,1,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
1,0,0,0,70,16,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,21,0,
0,1,54,0,0,5,
114,32,16,0,0,0,
0,0,70,2,16,0,
1,0,0,0,54,0,
0,5,130,32,16,0,
0,0,0,0,1,64,
0,0,0,0,128,63,
62,0,0,1,83,84,
65,84,148,0,0,0,
34,0,0,0,4,0,
0,0,25,0,0,0,
3,0,0,0,14,0,
0,0,2,0,0,0,
0,0,0,0,2,0,
0,0,2,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,4,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,6,0,
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
0,0,0,0
};

}

namespace RetroArch
{
class CrtShadersCrtMaximusRoyaleSrcNtsc_pass2ShaderDef : public ShaderDef
{
public:
	CrtShadersCrtMaximusRoyaleSrcNtsc_pass2ShaderDef() : ShaderDef{}
	{
		Name = "ntsc_pass2";
		VertexByteCode = RetroArchCrtShadersCrtMaximusRoyaleSrcNtsc_pass2ShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchCrtShadersCrtMaximusRoyaleSrcNtsc_pass2ShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchCrtShadersCrtMaximusRoyaleSrcNtsc_pass2ShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchCrtShadersCrtMaximusRoyaleSrcNtsc_pass2ShaderDefs::sFragmentByteCode);
		Params.push_back(ShaderParam("h2x", -1, 4, 4, 0.000000f, 2.000000f, 2.000000f, 1.000000f, "Horizontal Scale"));
		Params.push_back(ShaderParam("CVBS_SVIDEO_RGB", -1, 0, 4, 0.000000f, 2.000000f, 0.000000f, 1.000000f, "CVBS - SVIDEO - RGB"));
		Params.push_back(ShaderParam("MVP", 0, 0, 64, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("SourceSize", 0, 80, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OriginalSize", 0, 64, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
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