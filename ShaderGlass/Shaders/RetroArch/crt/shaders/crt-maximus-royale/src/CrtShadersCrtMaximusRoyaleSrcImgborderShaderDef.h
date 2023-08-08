/*
ShaderGlass shader crt-shaders-crt-maximus-royale-src\imgborder imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/shaders/crt-maximus-royale/src/imgborder.slang
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
////DRAWING////
///NOISE///
////OUTPUT////

*/

#pragma once

namespace RetroArchCrtShadersCrtMaximusRoyaleSrcImgborderShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,191,98,
141,72,40,210,189,227,
198,219,223,186,179,67,
113,229,1,0,0,0,
88,6,0,0,5,0,
0,0,52,0,0,0,
8,2,0,0,84,2,
0,0,12,3,0,0,
188,5,0,0,82,68,
69,70,204,1,0,0,
1,0,0,0,96,0,
0,0,1,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
164,1,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
92,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,1,0,
0,0,85,66,79,0,
92,0,0,0,3,0,
0,0,120,0,0,0,
96,0,0,0,0,0,
0,0,0,0,0,0,
240,0,0,0,0,0,
0,0,64,0,0,0,
2,0,0,0,4,1,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,40,1,
0,0,64,0,0,0,
16,0,0,0,2,0,
0,0,68,1,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,104,1,0,0,
80,0,0,0,4,0,
0,0,0,0,0,0,
128,1,0,0,0,0,
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
0,0,0,0,251,0,
0,0,103,108,111,98,
97,108,95,79,117,116,
112,117,116,83,105,122,
101,0,102,108,111,97,
116,52,0,171,171,171,
1,0,3,0,1,0,
4,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,58,1,0,0,
103,108,111,98,97,108,
95,70,114,97,109,101,
67,111,117,110,116,0,
100,119,111,114,100,0,
0,0,19,0,1,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,122,1,0,0,
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
176,0,0,0,6,0,
0,0,8,0,0,0,
152,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,12,0,0,
152,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,3,0,0,
152,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,3,12,0,0,
152,0,0,0,3,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,12,3,0,0,
152,0,0,0,4,0,
0,0,0,0,0,0,
3,0,0,0,2,0,
0,0,1,14,0,0,
161,0,0,0,0,0,
0,0,1,0,0,0,
3,0,0,0,3,0,
0,0,15,0,0,0,
84,69,88,67,79,79,
82,68,0,83,86,95,
80,111,115,105,116,105,
111,110,0,171,171,171,
83,72,69,88,168,2,
0,0,80,0,1,0,
170,0,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,5,0,0,0,
95,0,0,3,242,16,
16,0,0,0,0,0,
95,0,0,3,50,16,
16,0,1,0,0,0,
101,0,0,3,50,32,
16,0,0,0,0,0,
101,0,0,3,194,32,
16,0,0,0,0,0,
101,0,0,3,50,32,
16,0,1,0,0,0,
101,0,0,3,194,32,
16,0,1,0,0,0,
101,0,0,3,18,32,
16,0,2,0,0,0,
103,0,0,4,242,32,
16,0,3,0,0,0,
1,0,0,0,104,0,
0,2,2,0,0,0,
54,0,0,5,50,32,
16,0,0,0,0,0,
70,16,16,0,1,0,
0,0,56,0,0,11,
50,0,16,0,0,0,
0,0,2,64,0,0,
171,170,170,63,57,142,
227,63,0,0,0,0,
0,0,0,0,86,133,
32,0,0,0,0,0,
4,0,0,0,14,0,
0,8,50,0,16,0,
0,0,0,0,70,0,
16,0,0,0,0,0,
6,128,32,0,0,0,
0,0,4,0,0,0,
0,0,0,10,50,0,
16,0,1,0,0,0,
70,16,16,0,1,0,
0,0,2,64,0,0,
0,0,0,191,0,0,
0,191,0,0,0,0,
0,0,0,0,54,0,
0,5,66,0,16,0,
0,0,0,0,1,64,
0,0,0,0,128,63,
14,0,0,7,146,0,
16,0,0,0,0,0,
6,4,16,0,1,0,
0,0,6,8,16,0,
0,0,0,0,14,0,
0,7,98,0,16,0,
0,0,0,0,6,1,
16,0,1,0,0,0,
86,6,16,0,0,0,
0,0,0,0,0,10,
194,32,16,0,1,0,
0,0,86,9,16,0,
0,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,63,0,0,0,63,
0,0,0,10,194,32,
16,0,0,0,0,0,
6,12,16,0,0,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,63,
0,0,0,63,50,0,
0,15,50,32,16,0,
1,0,0,0,198,0,
16,0,0,0,0,0,
2,64,0,0,52,72,
131,63,52,72,131,63,
0,0,0,0,0,0,
0,0,2,64,0,0,
0,0,0,63,0,0,
0,63,0,0,0,0,
0,0,0,0,14,0,
0,9,18,32,16,0,
2,0,0,0,10,128,
32,0,0,0,0,0,
4,0,0,0,26,128,
32,0,0,0,0,0,
4,0,0,0,56,0,
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
16,0,0,0,2,0,
0,0,0,0,0,0,
8,0,0,0,13,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,2,0,
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
0,0,0,0
};


static const BYTE sFragmentByteCode[] =
{
68,88,66,67,115,83,
56,147,64,89,150,81,
83,72,20,43,128,116,
249,187,1,0,0,0,
136,26,0,0,5,0,
0,0,52,0,0,0,
44,9,0,0,192,9,
0,0,244,9,0,0,
236,25,0,0,82,68,
69,70,240,8,0,0,
2,0,0,0,240,4,
0,0,26,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
199,8,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
124,3,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
1,0,0,0,1,0,
0,0,141,3,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,1,0,0,0,
1,0,0,0,160,3,
0,0,3,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
4,0,0,0,1,0,
0,0,1,0,0,0,
179,3,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,5,0,0,0,
1,0,0,0,1,0,
0,0,198,3,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,6,0,
0,0,1,0,0,0,
1,0,0,0,219,3,
0,0,3,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
7,0,0,0,1,0,
0,0,1,0,0,0,
240,3,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,8,0,0,0,
1,0,0,0,1,0,
0,0,6,4,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,9,0,
0,0,1,0,0,0,
1,0,0,0,28,4,
0,0,3,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
10,0,0,0,1,0,
0,0,1,0,0,0,
51,4,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,11,0,0,0,
1,0,0,0,1,0,
0,0,67,4,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,12,0,
0,0,1,0,0,0,
1,0,0,0,83,4,
0,0,3,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
13,0,0,0,1,0,
0,0,1,0,0,0,
102,4,0,0,2,0,
0,0,5,0,0,0,
4,0,0,0,255,255,
255,255,2,0,0,0,
1,0,0,0,13,0,
0,0,110,4,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,3,0,
0,0,1,0,0,0,
13,0,0,0,120,4,
0,0,2,0,0,0,
5,0,0,0,4,0,
0,0,255,255,255,255,
4,0,0,0,1,0,
0,0,13,0,0,0,
130,4,0,0,2,0,
0,0,5,0,0,0,
4,0,0,0,255,255,
255,255,5,0,0,0,
1,0,0,0,13,0,
0,0,140,4,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,6,0,
0,0,1,0,0,0,
13,0,0,0,152,4,
0,0,2,0,0,0,
5,0,0,0,4,0,
0,0,255,255,255,255,
7,0,0,0,1,0,
0,0,13,0,0,0,
164,4,0,0,2,0,
0,0,5,0,0,0,
4,0,0,0,255,255,
255,255,8,0,0,0,
1,0,0,0,13,0,
0,0,177,4,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,9,0,
0,0,1,0,0,0,
13,0,0,0,190,4,
0,0,2,0,0,0,
5,0,0,0,4,0,
0,0,255,255,255,255,
10,0,0,0,1,0,
0,0,13,0,0,0,
204,4,0,0,2,0,
0,0,5,0,0,0,
4,0,0,0,255,255,
255,255,11,0,0,0,
1,0,0,0,13,0,
0,0,211,4,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,12,0,
0,0,1,0,0,0,
13,0,0,0,218,4,
0,0,2,0,0,0,
5,0,0,0,4,0,
0,0,255,255,255,255,
13,0,0,0,1,0,
0,0,13,0,0,0,
228,4,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,1,0,
0,0,232,4,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,1,0,0,0,
0,0,0,0,95,66,
71,80,65,83,83,50,
95,115,97,109,112,108,
101,114,0,95,84,86,
66,79,82,68,69,82,
49,95,115,97,109,112,
108,101,114,0,95,84,
86,66,79,82,68,69,
82,50,95,115,97,109,
112,108,101,114,0,95,
84,86,66,79,82,68,
69,82,51,95,115,97,
109,112,108,101,114,0,
95,84,86,66,79,82,
68,69,82,50,66,71,
95,115,97,109,112,108,
101,114,0,95,84,86,
66,79,82,68,69,82,
51,66,71,95,115,97,
109,112,108,101,114,0,
95,84,86,66,79,82,
68,69,82,50,76,69,
68,95,115,97,109,112,
108,101,114,0,95,84,
86,66,79,82,68,69,
82,51,76,69,68,95,
115,97,109,112,108,101,
114,0,95,77,79,78,
73,84,79,82,66,79,
82,68,69,82,95,115,
97,109,112,108,101,114,
0,95,83,72,65,80,
69,49,95,115,97,109,
112,108,101,114,0,95,
83,72,65,80,69,50,
95,115,97,109,112,108,
101,114,0,95,76,65,
83,84,95,80,65,83,
83,95,115,97,109,112,
108,101,114,0,66,71,
80,65,83,83,50,0,
84,86,66,79,82,68,
69,82,49,0,84,86,
66,79,82,68,69,82,
50,0,84,86,66,79,
82,68,69,82,51,0,
84,86,66,79,82,68,
69,82,50,66,71,0,
84,86,66,79,82,68,
69,82,51,66,71,0,
84,86,66,79,82,68,
69,82,50,76,69,68,
0,84,86,66,79,82,
68,69,82,51,76,69,
68,0,77,79,78,73,
84,79,82,66,79,82,
68,69,82,0,83,72,
65,80,69,49,0,83,
72,65,80,69,50,0,
76,65,83,84,95,80,
65,83,83,0,85,66,
79,0,80,117,115,104,
0,171,171,171,228,4,
0,0,3,0,0,0,
32,5,0,0,96,0,
0,0,0,0,0,0,
0,0,0,0,232,4,
0,0,11,0,0,0,
76,6,0,0,48,0,
0,0,0,0,0,0,
0,0,0,0,152,5,
0,0,0,0,0,0,
64,0,0,0,0,0,
0,0,172,5,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,208,5,0,0,
64,0,0,0,16,0,
0,0,2,0,0,0,
236,5,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
16,6,0,0,80,0,
0,0,4,0,0,0,
2,0,0,0,40,6,
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
0,0,163,5,0,0,
103,108,111,98,97,108,
95,79,117,116,112,117,
116,83,105,122,101,0,
102,108,111,97,116,52,
0,171,171,171,1,0,
3,0,1,0,4,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
226,5,0,0,103,108,
111,98,97,108,95,70,
114,97,109,101,67,111,
117,110,116,0,100,119,
111,114,100,0,0,0,
19,0,1,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
34,6,0,0,4,8,
0,0,0,0,0,0,
4,0,0,0,2,0,
0,0,28,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,64,8,0,0,
4,0,0,0,4,0,
0,0,2,0,0,0,
28,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
78,8,0,0,8,0,
0,0,4,0,0,0,
2,0,0,0,28,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,89,8,
0,0,12,0,0,0,
4,0,0,0,2,0,
0,0,28,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,100,8,0,0,
16,0,0,0,4,0,
0,0,2,0,0,0,
28,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
111,8,0,0,20,0,
0,0,4,0,0,0,
2,0,0,0,28,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,126,8,
0,0,24,0,0,0,
4,0,0,0,2,0,
0,0,28,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,138,8,0,0,
28,0,0,0,4,0,
0,0,2,0,0,0,
28,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
150,8,0,0,32,0,
0,0,4,0,0,0,
2,0,0,0,28,8,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,162,8,
0,0,36,0,0,0,
4,0,0,0,2,0,
0,0,28,8,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,178,8,0,0,
40,0,0,0,4,0,
0,0,2,0,0,0,
28,8,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
112,97,114,97,109,115,
95,99,114,116,70,114,
97,109,101,0,102,108,
111,97,116,0,171,171,
0,0,3,0,1,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,20,8,0,0,
112,97,114,97,109,115,
95,98,103,77,111,100,
101,0,112,97,114,97,
109,115,95,98,103,82,
0,112,97,114,97,109,
115,95,98,103,71,0,
112,97,114,97,109,115,
95,98,103,66,0,112,
97,114,97,109,115,95,
108,101,100,77,111,100,
101,0,112,97,114,97,
109,115,95,108,101,100,
82,0,112,97,114,97,
109,115,95,108,101,100,
71,0,112,97,114,97,
109,115,95,108,101,100,
66,0,112,97,114,97,
109,115,95,108,101,100,
83,112,101,101,100,0,
112,97,114,97,109,115,
95,108,101,100,66,114,
105,103,104,116,110,101,
115,115,0,77,105,99,
114,111,115,111,102,116,
32,40,82,41,32,72,
76,83,76,32,83,104,
97,100,101,114,32,67,
111,109,112,105,108,101,
114,32,49,48,46,49,
0,171,73,83,71,78,
140,0,0,0,5,0,
0,0,8,0,0,0,
128,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,3,0,0,
128,0,0,0,1,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,12,0,0,0,
128,0,0,0,2,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,3,3,0,0,
128,0,0,0,3,0,
0,0,0,0,0,0,
3,0,0,0,1,0,
0,0,12,12,0,0,
128,0,0,0,4,0,
0,0,0,0,0,0,
3,0,0,0,2,0,
0,0,1,1,0,0,
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
69,88,240,15,0,0,
80,0,0,0,252,3,
0,0,106,8,0,1,
89,0,0,4,70,142,
32,0,0,0,0,0,
6,0,0,0,89,0,
0,4,70,142,32,0,
1,0,0,0,3,0,
0,0,90,0,0,3,
0,96,16,0,2,0,
0,0,90,0,0,3,
0,96,16,0,3,0,
0,0,90,0,0,3,
0,96,16,0,4,0,
0,0,90,0,0,3,
0,96,16,0,5,0,
0,0,90,0,0,3,
0,96,16,0,6,0,
0,0,90,0,0,3,
0,96,16,0,7,0,
0,0,90,0,0,3,
0,96,16,0,8,0,
0,0,90,0,0,3,
0,96,16,0,9,0,
0,0,90,0,0,3,
0,96,16,0,10,0,
0,0,90,0,0,3,
0,96,16,0,11,0,
0,0,90,0,0,3,
0,96,16,0,12,0,
0,0,90,0,0,3,
0,96,16,0,13,0,
0,0,88,24,0,4,
0,112,16,0,2,0,
0,0,85,85,0,0,
88,24,0,4,0,112,
16,0,3,0,0,0,
85,85,0,0,88,24,
0,4,0,112,16,0,
4,0,0,0,85,85,
0,0,88,24,0,4,
0,112,16,0,5,0,
0,0,85,85,0,0,
88,24,0,4,0,112,
16,0,6,0,0,0,
85,85,0,0,88,24,
0,4,0,112,16,0,
7,0,0,0,85,85,
0,0,88,24,0,4,
0,112,16,0,8,0,
0,0,85,85,0,0,
88,24,0,4,0,112,
16,0,9,0,0,0,
85,85,0,0,88,24,
0,4,0,112,16,0,
10,0,0,0,85,85,
0,0,88,24,0,4,
0,112,16,0,11,0,
0,0,85,85,0,0,
88,24,0,4,0,112,
16,0,12,0,0,0,
85,85,0,0,88,24,
0,4,0,112,16,0,
13,0,0,0,85,85,
0,0,98,16,0,3,
50,16,16,0,0,0,
0,0,98,16,0,3,
50,16,16,0,1,0,
0,0,98,16,0,3,
194,16,16,0,1,0,
0,0,98,16,0,3,
18,16,16,0,2,0,
0,0,101,0,0,3,
242,32,16,0,0,0,
0,0,104,0,0,2,
9,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,0,0,0,0,
70,16,16,0,1,0,
0,0,70,126,16,0,
13,0,0,0,0,96,
16,0,13,0,0,0,
24,0,0,11,242,0,
16,0,1,0,0,0,
2,64,0,0,0,0,
128,63,0,0,64,64,
0,0,0,64,0,0,
128,64,6,128,32,0,
1,0,0,0,0,0,
0,0,31,0,4,3,
10,0,16,0,1,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,242,0,16,0,
2,0,0,0,230,26,
16,0,1,0,0,0,
70,126,16,0,3,0,
0,0,0,96,16,0,
3,0,0,0,56,0,
0,7,130,0,16,0,
0,0,0,0,10,16,
16,0,2,0,0,0,
1,64,0,0,0,0,
16,63,49,0,0,7,
18,0,16,0,1,0,
0,0,10,16,16,0,
0,0,0,0,1,64,
0,0,154,153,153,62,
31,0,4,3,10,0,
16,0,1,0,0,0,
56,0,0,7,18,0,
16,0,3,0,0,0,
58,0,16,0,0,0,
0,0,10,16,16,0,
0,0,0,0,54,0,
0,5,34,0,16,0,
3,0,0,0,58,16,
16,0,1,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,3,0,
0,0,70,0,16,0,
3,0,0,0,70,126,
16,0,3,0,0,0,
0,96,16,0,3,0,
0,0,18,0,0,1,
54,0,0,8,114,0,
16,0,3,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,21,0,0,1,
49,0,0,7,18,0,
16,0,1,0,0,0,
1,64,0,0,51,51,
51,63,10,16,16,0,
0,0,0,0,31,0,
4,3,10,0,16,0,
1,0,0,0,0,0,
0,7,18,0,16,0,
1,0,0,0,10,16,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,191,50,0,0,9,
18,0,16,0,4,0,
0,0,10,0,16,0,
1,0,0,0,58,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,54,0,0,5,
34,0,16,0,4,0,
0,0,58,16,16,0,
1,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,3,0,0,0,
70,0,16,0,4,0,
0,0,70,126,16,0,
3,0,0,0,0,96,
16,0,3,0,0,0,
21,0,0,1,50,0,
0,10,130,0,16,0,
0,0,0,0,10,16,
16,0,0,0,0,0,
10,128,32,0,0,0,
0,0,4,0,0,0,
1,64,0,0,0,0,
200,193,56,0,0,7,
18,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,1,64,
0,0,205,204,76,62,
0,0,0,8,130,0,
16,0,3,0,0,0,
10,16,16,128,65,0,
0,0,0,0,0,0,
1,64,0,0,0,0,
128,63,50,0,0,10,
130,0,16,0,3,0,
0,0,58,0,16,0,
3,0,0,0,10,128,
32,0,0,0,0,0,
4,0,0,0,1,64,
0,0,0,0,200,193,
49,0,0,7,18,0,
16,0,4,0,0,0,
10,16,16,0,2,0,
0,0,1,64,0,0,
57,142,227,63,49,0,
0,7,34,0,16,0,
4,0,0,0,1,64,
0,0,227,111,179,63,
10,16,16,0,2,0,
0,0,1,0,0,7,
18,0,16,0,4,0,
0,0,26,0,16,0,
4,0,0,0,10,0,
16,0,4,0,0,0,
49,0,0,10,98,0,
16,0,4,0,0,0,
246,15,16,0,0,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,160,64,
0,0,0,0,54,0,
0,6,130,0,16,0,
0,0,0,0,10,0,
16,128,65,0,0,0,
1,0,0,0,1,0,
0,7,130,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
42,0,16,0,4,0,
0,0,55,0,0,9,
130,0,16,0,0,0,
0,0,26,0,16,0,
4,0,0,0,1,64,
0,0,0,0,128,63,
58,0,16,0,0,0,
0,0,49,0,0,10,
98,0,16,0,4,0,
0,0,246,15,16,0,
3,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
160,64,0,0,0,0,
50,0,0,10,18,0,
16,0,1,0,0,0,
58,0,16,128,65,0,
0,0,3,0,0,0,
1,64,0,0,205,204,
76,62,58,0,16,0,
0,0,0,0,55,0,
0,9,130,0,16,0,
0,0,0,0,42,0,
16,0,4,0,0,0,
10,0,16,0,1,0,
0,0,58,0,16,0,
0,0,0,0,55,0,
0,9,130,0,16,0,
0,0,0,0,26,0,
16,0,4,0,0,0,
1,64,0,0,0,0,
128,63,58,0,16,0,
0,0,0,0,1,0,
0,7,130,0,16,0,
0,0,0,0,58,0,
16,0,0,0,0,0,
10,0,16,0,4,0,
0,0,18,0,0,1,
54,0,0,8,114,0,
16,0,3,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,54,0,0,5,
130,0,16,0,0,0,
0,0,1,64,0,0,
0,0,0,0,21,0,
0,1,57,0,0,8,
18,0,16,0,1,0,
0,0,1,64,0,0,
0,0,0,64,10,128,
32,0,1,0,0,0,
0,0,0,0,55,0,
0,9,18,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
26,0,16,0,1,0,
0,0,1,64,0,0,
255,255,255,255,31,0,
4,3,10,0,16,0,
1,0,0,0,56,0,
0,11,50,0,16,0,
4,0,0,0,2,64,
0,0,10,215,35,60,
10,215,35,60,0,0,
0,0,0,0,0,0,
230,138,32,0,1,0,
0,0,0,0,0,0,
56,0,0,11,114,0,
16,0,5,0,0,0,
2,64,0,0,10,215,
35,60,10,215,35,60,
10,215,35,60,0,0,
0,0,134,131,32,0,
1,0,0,0,1,0,
0,0,56,0,0,11,
194,0,16,0,4,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,10,215,35,60,
10,215,35,60,6,136,
32,0,1,0,0,0,
2,0,0,0,49,0,
0,11,50,0,16,0,
6,0,0,0,2,64,
0,0,0,0,0,0,
0,0,128,63,0,0,
0,0,0,0,0,0,
86,133,32,0,1,0,
0,0,0,0,0,0,
49,0,0,8,18,0,
16,0,1,0,0,0,
26,128,32,0,1,0,
0,0,0,0,0,0,
1,64,0,0,0,0,
64,64,1,0,0,7,
18,0,16,0,1,0,
0,0,10,0,16,0,
1,0,0,0,10,0,
16,0,6,0,0,0,
1,0,0,7,50,0,
16,0,7,0,0,0,
70,0,16,0,4,0,
0,0,6,0,16,0,
1,0,0,0,1,0,
0,7,66,0,16,0,
7,0,0,0,10,0,
16,0,5,0,0,0,
10,0,16,0,1,0,
0,0,31,0,4,3,
26,0,16,0,6,0,
0,0,69,0,0,142,
194,0,0,128,67,85,
21,0,114,0,16,0,
6,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,0,0,
0,7,114,0,16,0,
7,0,0,0,70,2,
16,0,6,0,0,0,
70,2,16,0,7,0,
0,0,21,0,0,1,
24,0,0,11,114,0,
16,0,6,0,0,0,
2,64,0,0,0,0,
128,63,0,0,0,64,
0,0,64,64,0,0,
0,0,86,133,32,0,
1,0,0,0,1,0,
0,0,1,0,0,7,
50,0,16,0,5,0,
0,0,150,5,16,0,
5,0,0,0,6,0,
16,0,6,0,0,0,
1,0,0,7,66,0,
16,0,5,0,0,0,
42,0,16,0,4,0,
0,0,10,0,16,0,
6,0,0,0,86,0,
0,6,18,0,16,0,
1,0,0,0,10,128,
32,0,0,0,0,0,
5,0,0,0,56,0,
0,8,18,0,16,0,
1,0,0,0,10,0,
16,0,1,0,0,0,
26,128,32,0,1,0,
0,0,2,0,0,0,
56,0,0,7,130,0,
16,0,3,0,0,0,
10,0,16,0,1,0,
0,0,1,64,0,0,
111,18,131,58,77,0,
0,6,18,0,16,0,
4,0,0,0,0,208,
0,0,58,0,16,0,
3,0,0,0,50,0,
0,15,146,0,16,0,
6,0,0,0,6,0,
16,0,1,0,0,0,
2,64,0,0,111,18,
131,58,0,0,0,0,
0,0,0,0,111,18,
131,58,2,64,0,0,
145,10,6,192,0,0,
0,0,0,0,0,0,
145,10,134,192,77,0,
0,6,98,0,16,0,
4,0,0,0,0,208,
0,0,6,3,16,0,
6,0,0,0,0,0,
0,10,114,0,16,0,
4,0,0,0,70,2,
16,0,4,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
0,0,55,0,0,9,
114,0,16,0,4,0,
0,0,86,5,16,0,
6,0,0,0,70,2,
16,0,4,0,0,0,
70,2,16,0,5,0,
0,0,31,0,4,3,
42,0,16,0,6,0,
0,0,69,0,0,142,
194,0,0,128,67,85,
21,0,114,0,16,0,
5,0,0,0,2,64,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,50,0,
0,12,114,0,16,0,
4,0,0,0,70,2,
16,0,5,0,0,0,
2,64,0,0,0,0,
160,64,0,0,160,64,
0,0,160,64,0,0,
0,0,70,2,16,0,
4,0,0,0,21,0,
0,1,29,0,0,8,
18,0,16,0,1,0,
0,0,26,128,32,0,
1,0,0,0,1,0,
0,0,1,64,0,0,
0,0,0,64,56,0,
0,7,114,0,16,0,
5,0,0,0,246,15,
16,0,4,0,0,0,
70,2,16,0,4,0,
0,0,55,0,0,9,
114,0,16,0,4,0,
0,0,6,0,16,0,
1,0,0,0,70,2,
16,0,5,0,0,0,
70,2,16,0,4,0,
0,0,18,0,0,1,
54,0,0,8,114,0,
16,0,7,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,54,0,0,8,
114,0,16,0,4,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,21,0,
0,1,31,0,4,3,
42,0,16,0,1,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,242,0,16,0,
2,0,0,0,230,26,
16,0,1,0,0,0,
70,126,16,0,4,0,
0,0,0,96,16,0,
4,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,242,0,
16,0,5,0,0,0,
230,26,16,0,1,0,
0,0,70,126,16,0,
6,0,0,0,0,96,
16,0,6,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,6,0,
0,0,230,26,16,0,
1,0,0,0,70,126,
16,0,8,0,0,0,
0,96,16,0,8,0,
0,0,18,0,0,1,
54,0,0,8,114,0,
16,0,6,0,0,0,
2,64,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,54,0,0,8,
242,0,16,0,5,0,
0,0,2,64,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,21,0,
0,1,31,0,4,3,
26,0,16,0,1,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,242,0,16,0,
2,0,0,0,230,26,
16,0,1,0,0,0,
70,126,16,0,5,0,
0,0,0,96,16,0,
5,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,242,0,
16,0,5,0,0,0,
230,26,16,0,1,0,
0,0,70,126,16,0,
7,0,0,0,0,96,
16,0,7,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,6,0,
0,0,230,26,16,0,
1,0,0,0,70,126,
16,0,9,0,0,0,
0,96,16,0,9,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
1,0,0,0,230,26,
16,0,1,0,0,0,
70,126,16,0,12,0,
0,0,0,96,16,0,
12,0,0,0,18,0,
0,1,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
1,0,0,0,230,26,
16,0,1,0,0,0,
70,126,16,0,11,0,
0,0,0,96,16,0,
11,0,0,0,21,0,
0,1,31,0,4,3,
58,0,16,0,1,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,242,0,16,0,
2,0,0,0,230,26,
16,0,1,0,0,0,
70,126,16,0,10,0,
0,0,0,96,16,0,
10,0,0,0,21,0,
0,1,0,0,0,10,
50,0,16,0,8,0,
0,0,70,16,16,0,
0,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
0,0,0,0,0,0,
56,0,0,7,130,0,
16,0,1,0,0,0,
26,0,16,0,8,0,
0,0,10,0,16,0,
8,0,0,0,86,0,
0,6,130,0,16,0,
3,0,0,0,10,128,
32,0,0,0,0,0,
5,0,0,0,56,0,
0,7,130,0,16,0,
4,0,0,0,58,0,
16,0,3,0,0,0,
1,64,0,0,111,18,
131,58,65,0,0,5,
130,0,16,0,4,0,
0,0,58,0,16,0,
4,0,0,0,50,0,
0,10,130,0,16,0,
3,0,0,0,58,0,
16,128,65,0,0,0,
4,0,0,0,1,64,
0,0,0,0,122,68,
58,0,16,0,3,0,
0,0,0,0,0,7,
130,0,16,0,3,0,
0,0,58,0,16,0,
3,0,0,0,1,64,
0,0,0,0,32,65,
56,0,0,7,130,0,
16,0,1,0,0,0,
58,0,16,0,1,0,
0,0,58,0,16,0,
3,0,0,0,56,0,
0,10,114,0,16,0,
8,0,0,0,246,15,
16,0,1,0,0,0,
2,64,0,0,0,0,
32,65,0,0,128,63,
204,204,204,61,0,0,
0,0,65,0,0,5,
98,0,16,0,8,0,
0,0,86,6,16,0,
8,0,0,0,50,0,
0,13,50,0,16,0,
8,0,0,0,150,5,
16,128,65,0,0,0,
8,0,0,0,2,64,
0,0,0,0,32,65,
0,0,200,66,0,0,
0,0,0,0,0,0,
6,0,16,0,8,0,
0,0,0,0,0,10,
50,0,16,0,8,0,
0,0,70,0,16,0,
8,0,0,0,2,64,
0,0,0,0,128,63,
0,0,128,63,0,0,
0,0,0,0,0,0,
56,0,0,7,130,0,
16,0,1,0,0,0,
26,0,16,0,8,0,
0,0,10,0,16,0,
8,0,0,0,56,0,
0,7,130,0,16,0,
3,0,0,0,58,0,
16,0,1,0,0,0,
1,64,0,0,0,0,
200,66,65,0,0,5,
130,0,16,0,3,0,
0,0,58,0,16,0,
3,0,0,0,50,0,
0,10,130,0,16,0,
1,0,0,0,58,0,
16,128,65,0,0,0,
3,0,0,0,1,64,
0,0,10,215,35,60,
58,0,16,0,1,0,
0,0,0,0,0,7,
130,0,16,0,1,0,
0,0,58,0,16,0,
1,0,0,0,1,64,
0,0,10,215,163,187,
0,0,0,7,114,0,
16,0,0,0,0,0,
70,2,16,0,0,0,
0,0,70,2,16,0,
1,0,0,0,0,0,
0,7,114,0,16,0,
0,0,0,0,246,15,
16,0,1,0,0,0,
70,2,16,0,0,0,
0,0,0,0,0,8,
114,0,16,0,1,0,
0,0,70,2,16,128,
65,0,0,0,0,0,
0,0,70,2,16,0,
2,0,0,0,50,0,
0,9,114,0,16,0,
0,0,0,0,246,15,
16,0,2,0,0,0,
70,2,16,0,1,0,
0,0,70,2,16,0,
0,0,0,0,56,0,
0,7,114,0,16,0,
1,0,0,0,70,2,
16,0,4,0,0,0,
70,2,16,0,6,0,
0,0,50,0,0,9,
114,0,16,0,1,0,
0,0,70,2,16,0,
5,0,0,0,70,2,
16,0,7,0,0,0,
70,2,16,0,1,0,
0,0,0,0,0,8,
114,0,16,0,1,0,
0,0,70,2,16,128,
65,0,0,0,0,0,
0,0,70,2,16,0,
1,0,0,0,50,0,
0,9,114,0,16,0,
0,0,0,0,246,15,
16,0,5,0,0,0,
70,2,16,0,1,0,
0,0,70,2,16,0,
0,0,0,0,0,0,
0,8,114,0,16,0,
1,0,0,0,70,2,
16,128,65,0,0,0,
0,0,0,0,70,2,
16,0,3,0,0,0,
50,0,0,9,114,32,
16,0,0,0,0,0,
246,15,16,0,0,0,
0,0,70,2,16,0,
1,0,0,0,70,2,
16,0,0,0,0,0,
54,0,0,5,130,32,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,62,0,0,1,
83,84,65,84,148,0,
0,0,125,0,0,0,
9,0,0,0,0,0,
0,0,5,0,0,0,
60,0,0,0,0,0,
0,0,8,0,0,0,
6,0,0,0,9,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,15,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
10,0,0,0,6,0,
0,0,5,0,0,0,
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
class CrtShadersCrtMaximusRoyaleSrcImgborderShaderDef : public ShaderDef
{
public:
	CrtShadersCrtMaximusRoyaleSrcImgborderShaderDef() : ShaderDef{}
	{
		Name = "imgborder";
		VertexByteCode = RetroArchCrtShadersCrtMaximusRoyaleSrcImgborderShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchCrtShadersCrtMaximusRoyaleSrcImgborderShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchCrtShadersCrtMaximusRoyaleSrcImgborderShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchCrtShadersCrtMaximusRoyaleSrcImgborderShaderDefs::sFragmentByteCode);
		Params.push_back(ShaderParam("crtFrame", -1, 0, 4, 1.000000f, 4.000000f, 1.000000f, 1.000000f, "CRT Frame"));
		Params.push_back(ShaderParam("bgMode", -1, 4, 4, 0.000000f, 3.000000f, 0.000000f, 1.000000f, "BG Mode"));
		Params.push_back(ShaderParam("bgR", -1, 8, 4, 0.000000f, 100.000000f, 0.000000f, 1.000000f, "BG Red"));
		Params.push_back(ShaderParam("bgG", -1, 12, 4, 0.000000f, 100.000000f, 25.000000f, 1.000000f, "BG Green"));
		Params.push_back(ShaderParam("bgB", -1, 16, 4, 0.000000f, 100.000000f, 50.000000f, 1.000000f, "BG Blue"));
		Params.push_back(ShaderParam("ledMode", -1, 20, 4, 0.000000f, 3.000000f, 0.000000f, 1.000000f, "Rear TV LED Mode"));
		Params.push_back(ShaderParam("ledR", -1, 24, 4, 0.000000f, 100.000000f, 50.000000f, 1.000000f, "LED Red"));
		Params.push_back(ShaderParam("ledG", -1, 28, 4, 0.000000f, 100.000000f, 50.000000f, 1.000000f, "LED Green"));
		Params.push_back(ShaderParam("ledB", -1, 32, 4, 0.000000f, 100.000000f, 50.000000f, 1.000000f, "LED Blue"));
		Params.push_back(ShaderParam("ledSpeed", -1, 36, 4, 1.000000f, 20.000000f, 1.000000f, 1.000000f, "LED Speed"));
		Params.push_back(ShaderParam("ledBrightness", -1, 40, 4, 1.000000f, 100.000000f, 50.000000f, 1.000000f, "LED Brightness"));
		Params.push_back(ShaderParam("MVP", 0, 0, 64, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("OutputSize", 0, 64, 16, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Params.push_back(ShaderParam("FrameCount", 0, 80, 4, 0.000000f, 0.000000f, 0.000000f, 0.000000f, ""));
		Samplers.push_back(ShaderSampler("LAST_PASS", 13));
		Samplers.push_back(ShaderSampler("TVBORDER1", 3));
		Samplers.push_back(ShaderSampler("BGPASS2", 2));
		Samplers.push_back(ShaderSampler("TVBORDER2", 4));
		Samplers.push_back(ShaderSampler("TVBORDER2BG", 6));
		Samplers.push_back(ShaderSampler("TVBORDER2LED", 8));
		Samplers.push_back(ShaderSampler("TVBORDER3", 5));
		Samplers.push_back(ShaderSampler("TVBORDER3BG", 7));
		Samplers.push_back(ShaderSampler("TVBORDER3LED", 9));
		Samplers.push_back(ShaderSampler("SHAPE2", 12));
		Samplers.push_back(ShaderSampler("SHAPE1", 11));
		Samplers.push_back(ShaderSampler("MONITORBORDER", 10));
/*
VertexSource = %*VERTEX_SOURCE*%;
*/
/*
FragmentSource = %*FRAGMENT_SOURCE*%;
*/
	}
};
}