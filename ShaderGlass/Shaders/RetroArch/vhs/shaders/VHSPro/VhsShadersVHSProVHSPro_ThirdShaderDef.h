/*
ShaderGlass shader vhs-shaders-VHSPro\VHSPro_Third imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/vhs/shaders/VHSPro/VHSPro_Third.slang
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArchVhsShadersVHSProVHSPro_ThirdShaderDefs
{
static const BYTE sVertexByteCode[] =
{
68,88,66,67,244,103,
242,1,194,22,72,189,
189,228,247,209,36,167,
223,163,1,0,0,0,
140,18,0,0,5,0,
0,0,52,0,0,0,
60,16,0,0,136,16,
0,0,224,16,0,0,
240,17,0,0,82,68,
69,70,0,16,0,0,
1,0,0,0,96,0,
0,0,1,0,0,0,
60,0,0,0,0,5,
254,255,0,129,0,0,
214,15,0,0,82,68,
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
92,0,0,0,61,0,
0,0,120,0,0,0,
48,1,0,0,0,0,
0,0,0,0,0,0,
0,10,0,0,0,0,
0,0,64,0,0,0,
2,0,0,0,20,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,56,10,
0,0,64,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,120,10,0,0,
68,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
144,10,0,0,72,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,172,10,
0,0,76,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,191,10,0,0,
80,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
217,10,0,0,84,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,254,10,
0,0,88,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,17,11,0,0,
92,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
32,11,0,0,96,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,47,11,
0,0,100,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,66,11,0,0,
104,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
85,11,0,0,108,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,112,11,
0,0,112,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,134,11,0,0,
116,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
155,11,0,0,120,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,176,11,
0,0,124,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,198,11,0,0,
128,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
226,11,0,0,132,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,249,11,
0,0,136,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,20,12,0,0,
140,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
50,12,0,0,144,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,75,12,
0,0,148,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,99,12,0,0,
152,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
127,12,0,0,156,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,150,12,
0,0,160,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,172,12,0,0,
164,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
200,12,0,0,168,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,219,12,
0,0,172,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,242,12,0,0,
176,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
8,13,0,0,180,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,36,13,
0,0,184,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,57,13,0,0,
188,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
86,13,0,0,192,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,109,13,
0,0,196,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,135,13,0,0,
200,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
162,13,0,0,204,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,183,13,
0,0,208,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,209,13,0,0,
212,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
230,13,0,0,216,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,250,13,
0,0,220,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,21,14,0,0,
224,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
40,14,0,0,228,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,67,14,
0,0,232,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,86,14,0,0,
236,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
116,14,0,0,240,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,137,14,
0,0,244,0,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,158,14,0,0,
248,0,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
179,14,0,0,252,0,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,199,14,
0,0,0,1,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,219,14,0,0,
4,1,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
239,14,0,0,8,1,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,5,15,
0,0,12,1,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,32,15,0,0,
16,1,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
54,15,0,0,20,1,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,74,15,
0,0,24,1,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,96,15,0,0,
28,1,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
119,15,0,0,32,1,
0,0,4,0,0,0,
0,0,0,0,84,10,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,142,15,
0,0,36,1,0,0,
4,0,0,0,0,0,
0,0,84,10,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,165,15,0,0,
40,1,0,0,4,0,
0,0,0,0,0,0,
84,10,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
193,15,0,0,44,1,
0,0,4,0,0,0,
0,0,0,0,84,10,
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
0,0,11,10,0,0,
103,108,111,98,97,108,
95,115,99,114,101,101,
110,76,105,110,101,115,
78,117,109,0,102,108,
111,97,116,0,0,0,
3,0,1,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
78,10,0,0,103,108,
111,98,97,108,95,86,
72,83,95,66,108,101,
101,100,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,86,
72,83,95,66,108,101,
101,100,77,111,100,101,
95,115,101,108,101,99,
116,0,103,108,111,98,
97,108,95,98,108,101,
101,100,65,109,111,117,
110,116,0,103,108,111,
98,97,108,95,86,72,
83,95,70,105,115,104,
69,121,101,95,116,111,
103,103,108,101,0,103,
108,111,98,97,108,95,
86,72,83,95,70,105,
115,104,69,121,101,95,
72,121,112,101,114,115,
112,97,99,101,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,102,105,115,104,101,
121,101,66,101,110,100,
0,103,108,111,98,97,
108,95,99,117,116,111,
102,102,88,0,103,108,
111,98,97,108,95,99,
117,116,111,102,102,89,
0,103,108,111,98,97,
108,95,99,117,116,111,
102,102,70,97,100,101,
88,0,103,108,111,98,
97,108,95,99,117,116,
111,102,102,70,97,100,
101,89,0,103,108,111,
98,97,108,95,86,72,
83,95,86,105,103,110,
101,116,116,101,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,118,105,103,110,101,
116,116,101,65,109,111,
117,110,116,0,103,108,
111,98,97,108,95,118,
105,103,110,101,116,116,
101,83,112,101,101,100,
0,103,108,111,98,97,
108,95,110,111,105,115,
101,76,105,110,101,115,
76,117,109,0,103,108,
111,98,97,108,95,110,
111,105,115,101,81,117,
97,110,116,105,122,101,
88,0,103,108,111,98,
97,108,95,86,72,83,
95,70,105,108,109,71,
114,97,105,110,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,102,105,108,109,71,
114,97,105,110,65,109,
111,117,110,116,0,103,
108,111,98,97,108,95,
86,72,83,95,89,73,
81,78,111,105,115,101,
95,116,111,103,103,108,
101,0,103,108,111,98,
97,108,95,115,105,103,
110,97,108,78,111,105,
115,101,84,121,112,101,
95,115,101,108,101,99,
116,0,103,108,111,98,
97,108,95,115,105,103,
110,97,108,78,111,105,
115,101,65,109,111,117,
110,116,0,103,108,111,
98,97,108,95,115,105,
103,110,97,108,78,111,
105,115,101,80,111,119,
101,114,0,103,108,111,
98,97,108,95,86,72,
83,95,76,105,110,101,
78,111,105,115,101,95,
116,111,103,103,108,101,
0,103,108,111,98,97,
108,95,108,105,110,101,
78,111,105,115,101,65,
109,111,117,110,116,0,
103,108,111,98,97,108,
95,108,105,110,101,78,
111,105,115,101,83,112,
101,101,100,0,103,108,
111,98,97,108,95,86,
72,83,95,84,97,112,
101,78,111,105,115,101,
95,116,111,103,103,108,
101,0,103,108,111,98,
97,108,95,116,97,112,
101,78,111,105,115,101,
84,72,0,103,108,111,
98,97,108,95,116,97,
112,101,78,111,105,115,
101,65,109,111,117,110,
116,0,103,108,111,98,
97,108,95,116,97,112,
101,78,111,105,115,101,
83,112,101,101,100,0,
103,108,111,98,97,108,
95,86,72,83,95,83,
99,97,110,76,105,110,
101,115,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,115,
99,97,110,76,105,110,
101,87,105,100,116,104,
0,103,108,111,98,97,
108,95,86,72,83,95,
76,105,110,101,115,70,
108,111,97,116,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,108,105,110,101,115,
70,108,111,97,116,83,
112,101,101,100,0,103,
108,111,98,97,108,95,
86,72,83,95,83,116,
114,101,116,99,104,95,
116,111,103,103,108,101,
0,103,108,111,98,97,
108,95,86,72,83,95,
74,105,116,116,101,114,
95,72,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,106,
105,116,116,101,114,72,
65,109,111,117,110,116,
0,103,108,111,98,97,
108,95,86,72,83,74,
105,116,116,101,114,95,
86,95,116,111,103,103,
108,101,0,103,108,111,
98,97,108,95,106,105,
116,116,101,114,86,65,
109,111,117,110,116,0,
103,108,111,98,97,108,
95,106,105,116,116,101,
114,86,83,112,101,101,
100,0,103,108,111,98,
97,108,95,86,72,83,
95,84,119,105,116,99,
104,95,72,95,116,111,
103,103,108,101,0,103,
108,111,98,97,108,95,
116,119,105,116,99,104,
72,70,114,101,113,0,
103,108,111,98,97,108,
95,86,72,83,95,84,
119,105,116,99,104,95,
86,95,116,111,103,103,
108,101,0,103,108,111,
98,97,108,95,116,119,
105,116,99,104,86,70,
114,101,113,0,103,108,
111,98,97,108,95,86,
72,83,95,83,105,103,
110,97,108,84,119,101,
97,107,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,115,
105,103,110,97,108,65,
100,106,117,115,116,89,
0,103,108,111,98,97,
108,95,115,105,103,110,
97,108,65,100,106,117,
115,116,73,0,103,108,
111,98,97,108,95,115,
105,103,110,97,108,65,
100,106,117,115,116,81,
0,103,108,111,98,97,
108,95,115,105,103,110,
97,108,83,104,105,102,
116,89,0,103,108,111,
98,97,108,95,115,105,
103,110,97,108,83,104,
105,102,116,73,0,103,
108,111,98,97,108,95,
115,105,103,110,97,108,
83,104,105,102,116,81,
0,103,108,111,98,97,
108,95,103,97,109,109,
97,67,111,114,101,99,
116,105,111,110,0,103,
108,111,98,97,108,95,
86,72,83,95,70,101,
101,100,98,97,99,107,
95,116,111,103,103,108,
101,0,103,108,111,98,
97,108,95,102,101,101,
100,98,97,99,107,65,
109,111,117,110,116,0,
103,108,111,98,97,108,
95,102,101,101,100,98,
97,99,107,70,97,100,
101,0,103,108,111,98,
97,108,95,102,101,101,
100,98,97,99,107,84,
104,114,101,115,104,0,
103,108,111,98,97,108,
95,102,101,101,100,98,
97,99,107,67,111,108,
111,114,95,82,0,103,
108,111,98,97,108,95,
102,101,101,100,98,97,
99,107,67,111,108,111,
114,95,71,0,103,108,
111,98,97,108,95,102,
101,101,100,98,97,99,
107,67,111,108,111,114,
95,66,0,103,108,111,
98,97,108,95,102,101,
101,100,98,97,99,107,
68,101,98,117,103,95,
116,111,103,103,108,101,
0,103,108,111,98,97,
108,95,110,111,105,115,
101,76,105,110,101,115,
78,117,109,0,77,105,
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
80,0,0,0,2,0,
0,0,8,0,0,0,
56,0,0,0,0,0,
0,0,0,0,0,0,
3,0,0,0,0,0,
0,0,3,12,0,0,
65,0,0,0,0,0,
0,0,1,0,0,0,
3,0,0,0,1,0,
0,0,15,0,0,0,
84,69,88,67,79,79,
82,68,0,83,86,95,
80,111,115,105,116,105,
111,110,0,171,171,171,
83,72,69,88,8,1,
0,0,80,0,1,0,
66,0,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,4,0,0,0,
95,0,0,3,242,16,
16,0,0,0,0,0,
95,0,0,3,50,16,
16,0,1,0,0,0,
101,0,0,3,50,32,
16,0,0,0,0,0,
103,0,0,4,242,32,
16,0,1,0,0,0,
1,0,0,0,104,0,
0,2,1,0,0,0,
54,0,0,5,50,32,
16,0,0,0,0,0,
70,16,16,0,1,0,
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
148,0,0,0,6,0,
0,0,1,0,0,0,
0,0,0,0,4,0,
0,0,4,0,0,0,
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
68,88,66,67,109,87,
201,217,250,99,19,15,
133,119,228,217,27,196,
13,240,1,0,0,0,
96,24,0,0,5,0,
0,0,52,0,0,0,
232,18,0,0,28,19,
0,0,80,19,0,0,
196,23,0,0,82,68,
69,70,172,18,0,0,
2,0,0,0,176,1,
0,0,8,0,0,0,
60,0,0,0,0,5,
255,255,0,129,0,0,
132,18,0,0,82,68,
49,49,60,0,0,0,
24,0,0,0,32,0,
0,0,40,0,0,0,
36,0,0,0,12,0,
0,0,0,0,0,0,
60,1,0,0,3,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,2,0,0,0,
1,0,0,0,1,0,
0,0,78,1,0,0,
3,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,3,0,
0,0,1,0,0,0,
1,0,0,0,104,1,
0,0,3,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
4,0,0,0,1,0,
0,0,1,0,0,0,
127,1,0,0,2,0,
0,0,5,0,0,0,
4,0,0,0,255,255,
255,255,2,0,0,0,
1,0,0,0,13,0,
0,0,136,1,0,0,
2,0,0,0,5,0,
0,0,4,0,0,0,
255,255,255,255,3,0,
0,0,1,0,0,0,
13,0,0,0,153,1,
0,0,2,0,0,0,
5,0,0,0,4,0,
0,0,255,255,255,255,
4,0,0,0,1,0,
0,0,13,0,0,0,
167,1,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,1,0,
0,0,171,1,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,1,0,
0,0,1,0,0,0,
0,0,0,0,95,79,
114,105,103,105,110,97,
108,95,115,97,109,112,
108,101,114,0,95,79,
114,105,103,105,110,97,
108,72,105,115,116,111,
114,121,49,95,115,97,
109,112,108,101,114,0,
95,80,97,115,115,70,
101,101,100,98,97,99,
107,48,95,115,97,109,
112,108,101,114,0,79,
114,105,103,105,110,97,
108,0,79,114,105,103,
105,110,97,108,72,105,
115,116,111,114,121,49,
0,80,97,115,115,70,
101,101,100,98,97,99,
107,48,0,85,66,79,
0,80,117,115,104,0,
167,1,0,0,61,0,
0,0,224,1,0,0,
48,1,0,0,0,0,
0,0,0,0,0,0,
171,1,0,0,4,0,
0,0,64,17,0,0,
64,0,0,0,0,0,
0,0,0,0,0,0,
104,11,0,0,0,0,
0,0,64,0,0,0,
0,0,0,0,124,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,160,11,
0,0,64,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,224,11,0,0,
68,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
248,11,0,0,72,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,20,12,
0,0,76,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,39,12,0,0,
80,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
65,12,0,0,84,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,102,12,
0,0,88,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,121,12,0,0,
92,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
136,12,0,0,96,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,151,12,
0,0,100,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,170,12,0,0,
104,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
189,12,0,0,108,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,216,12,
0,0,112,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,238,12,0,0,
116,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
3,13,0,0,120,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,24,13,
0,0,124,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,46,13,0,0,
128,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
74,13,0,0,132,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,97,13,
0,0,136,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,124,13,0,0,
140,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
154,13,0,0,144,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,179,13,
0,0,148,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,203,13,0,0,
152,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
231,13,0,0,156,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,254,13,
0,0,160,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,20,14,0,0,
164,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
48,14,0,0,168,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,67,14,
0,0,172,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,90,14,0,0,
176,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
112,14,0,0,180,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,140,14,
0,0,184,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,161,14,0,0,
188,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
190,14,0,0,192,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,213,14,
0,0,196,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,239,14,0,0,
200,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
10,15,0,0,204,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,31,15,
0,0,208,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,57,15,0,0,
212,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
78,15,0,0,216,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,98,15,
0,0,220,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,125,15,0,0,
224,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
144,15,0,0,228,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,171,15,
0,0,232,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,190,15,0,0,
236,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
220,15,0,0,240,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,241,15,
0,0,244,0,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,6,16,0,0,
248,0,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
27,16,0,0,252,0,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,47,16,
0,0,0,1,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,67,16,0,0,
4,1,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
87,16,0,0,8,1,
0,0,4,0,0,0,
0,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,109,16,
0,0,12,1,0,0,
4,0,0,0,0,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,136,16,0,0,
16,1,0,0,4,0,
0,0,2,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
158,16,0,0,20,1,
0,0,4,0,0,0,
2,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,178,16,
0,0,24,1,0,0,
4,0,0,0,2,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,200,16,0,0,
28,1,0,0,4,0,
0,0,2,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
223,16,0,0,32,1,
0,0,4,0,0,0,
2,0,0,0,188,11,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,246,16,
0,0,36,1,0,0,
4,0,0,0,2,0,
0,0,188,11,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,13,17,0,0,
40,1,0,0,4,0,
0,0,0,0,0,0,
188,11,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
41,17,0,0,44,1,
0,0,4,0,0,0,
0,0,0,0,188,11,
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
0,0,115,11,0,0,
103,108,111,98,97,108,
95,115,99,114,101,101,
110,76,105,110,101,115,
78,117,109,0,102,108,
111,97,116,0,0,0,
3,0,1,0,1,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
182,11,0,0,103,108,
111,98,97,108,95,86,
72,83,95,66,108,101,
101,100,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,86,
72,83,95,66,108,101,
101,100,77,111,100,101,
95,115,101,108,101,99,
116,0,103,108,111,98,
97,108,95,98,108,101,
101,100,65,109,111,117,
110,116,0,103,108,111,
98,97,108,95,86,72,
83,95,70,105,115,104,
69,121,101,95,116,111,
103,103,108,101,0,103,
108,111,98,97,108,95,
86,72,83,95,70,105,
115,104,69,121,101,95,
72,121,112,101,114,115,
112,97,99,101,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,102,105,115,104,101,
121,101,66,101,110,100,
0,103,108,111,98,97,
108,95,99,117,116,111,
102,102,88,0,103,108,
111,98,97,108,95,99,
117,116,111,102,102,89,
0,103,108,111,98,97,
108,95,99,117,116,111,
102,102,70,97,100,101,
88,0,103,108,111,98,
97,108,95,99,117,116,
111,102,102,70,97,100,
101,89,0,103,108,111,
98,97,108,95,86,72,
83,95,86,105,103,110,
101,116,116,101,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,118,105,103,110,101,
116,116,101,65,109,111,
117,110,116,0,103,108,
111,98,97,108,95,118,
105,103,110,101,116,116,
101,83,112,101,101,100,
0,103,108,111,98,97,
108,95,110,111,105,115,
101,76,105,110,101,115,
76,117,109,0,103,108,
111,98,97,108,95,110,
111,105,115,101,81,117,
97,110,116,105,122,101,
88,0,103,108,111,98,
97,108,95,86,72,83,
95,70,105,108,109,71,
114,97,105,110,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,102,105,108,109,71,
114,97,105,110,65,109,
111,117,110,116,0,103,
108,111,98,97,108,95,
86,72,83,95,89,73,
81,78,111,105,115,101,
95,116,111,103,103,108,
101,0,103,108,111,98,
97,108,95,115,105,103,
110,97,108,78,111,105,
115,101,84,121,112,101,
95,115,101,108,101,99,
116,0,103,108,111,98,
97,108,95,115,105,103,
110,97,108,78,111,105,
115,101,65,109,111,117,
110,116,0,103,108,111,
98,97,108,95,115,105,
103,110,97,108,78,111,
105,115,101,80,111,119,
101,114,0,103,108,111,
98,97,108,95,86,72,
83,95,76,105,110,101,
78,111,105,115,101,95,
116,111,103,103,108,101,
0,103,108,111,98,97,
108,95,108,105,110,101,
78,111,105,115,101,65,
109,111,117,110,116,0,
103,108,111,98,97,108,
95,108,105,110,101,78,
111,105,115,101,83,112,
101,101,100,0,103,108,
111,98,97,108,95,86,
72,83,95,84,97,112,
101,78,111,105,115,101,
95,116,111,103,103,108,
101,0,103,108,111,98,
97,108,95,116,97,112,
101,78,111,105,115,101,
84,72,0,103,108,111,
98,97,108,95,116,97,
112,101,78,111,105,115,
101,65,109,111,117,110,
116,0,103,108,111,98,
97,108,95,116,97,112,
101,78,111,105,115,101,
83,112,101,101,100,0,
103,108,111,98,97,108,
95,86,72,83,95,83,
99,97,110,76,105,110,
101,115,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,115,
99,97,110,76,105,110,
101,87,105,100,116,104,
0,103,108,111,98,97,
108,95,86,72,83,95,
76,105,110,101,115,70,
108,111,97,116,95,116,
111,103,103,108,101,0,
103,108,111,98,97,108,
95,108,105,110,101,115,
70,108,111,97,116,83,
112,101,101,100,0,103,
108,111,98,97,108,95,
86,72,83,95,83,116,
114,101,116,99,104,95,
116,111,103,103,108,101,
0,103,108,111,98,97,
108,95,86,72,83,95,
74,105,116,116,101,114,
95,72,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,106,
105,116,116,101,114,72,
65,109,111,117,110,116,
0,103,108,111,98,97,
108,95,86,72,83,74,
105,116,116,101,114,95,
86,95,116,111,103,103,
108,101,0,103,108,111,
98,97,108,95,106,105,
116,116,101,114,86,65,
109,111,117,110,116,0,
103,108,111,98,97,108,
95,106,105,116,116,101,
114,86,83,112,101,101,
100,0,103,108,111,98,
97,108,95,86,72,83,
95,84,119,105,116,99,
104,95,72,95,116,111,
103,103,108,101,0,103,
108,111,98,97,108,95,
116,119,105,116,99,104,
72,70,114,101,113,0,
103,108,111,98,97,108,
95,86,72,83,95,84,
119,105,116,99,104,95,
86,95,116,111,103,103,
108,101,0,103,108,111,
98,97,108,95,116,119,
105,116,99,104,86,70,
114,101,113,0,103,108,
111,98,97,108,95,86,
72,83,95,83,105,103,
110,97,108,84,119,101,
97,107,95,116,111,103,
103,108,101,0,103,108,
111,98,97,108,95,115,
105,103,110,97,108,65,
100,106,117,115,116,89,
0,103,108,111,98,97,
108,95,115,105,103,110,
97,108,65,100,106,117,
115,116,73,0,103,108,
111,98,97,108,95,115,
105,103,110,97,108,65,
100,106,117,115,116,81,
0,103,108,111,98,97,
108,95,115,105,103,110,
97,108,83,104,105,102,
116,89,0,103,108,111,
98,97,108,95,115,105,
103,110,97,108,83,104,
105,102,116,73,0,103,
108,111,98,97,108,95,
115,105,103,110,97,108,
83,104,105,102,116,81,
0,103,108,111,98,97,
108,95,103,97,109,109,
97,67,111,114,101,99,
116,105,111,110,0,103,
108,111,98,97,108,95,
86,72,83,95,70,101,
101,100,98,97,99,107,
95,116,111,103,103,108,
101,0,103,108,111,98,
97,108,95,102,101,101,
100,98,97,99,107,65,
109,111,117,110,116,0,
103,108,111,98,97,108,
95,102,101,101,100,98,
97,99,107,70,97,100,
101,0,103,108,111,98,
97,108,95,102,101,101,
100,98,97,99,107,84,
104,114,101,115,104,0,
103,108,111,98,97,108,
95,102,101,101,100,98,
97,99,107,67,111,108,
111,114,95,82,0,103,
108,111,98,97,108,95,
102,101,101,100,98,97,
99,107,67,111,108,111,
114,95,71,0,103,108,
111,98,97,108,95,102,
101,101,100,98,97,99,
107,67,111,108,111,114,
95,66,0,103,108,111,
98,97,108,95,102,101,
101,100,98,97,99,107,
68,101,98,117,103,95,
116,111,103,103,108,101,
0,103,108,111,98,97,
108,95,110,111,105,115,
101,76,105,110,101,115,
78,117,109,0,171,171,
224,17,0,0,0,0,
0,0,16,0,0,0,
0,0,0,0,252,17,
0,0,0,0,0,0,
255,255,255,255,0,0,
0,0,255,255,255,255,
0,0,0,0,32,18,
0,0,16,0,0,0,
16,0,0,0,0,0,
0,0,252,17,0,0,
0,0,0,0,255,255,
255,255,0,0,0,0,
255,255,255,255,0,0,
0,0,52,18,0,0,
32,0,0,0,16,0,
0,0,2,0,0,0,
252,17,0,0,0,0,
0,0,255,255,255,255,
0,0,0,0,255,255,
255,255,0,0,0,0,
70,18,0,0,48,0,
0,0,4,0,0,0,
0,0,0,0,96,18,
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
0,0,0,0,242,17,
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
0,0,88,18,0,0,
77,105,99,114,111,115,
111,102,116,32,40,82,
41,32,72,76,83,76,
32,83,104,97,100,101,
114,32,67,111,109,112,
105,108,101,114,32,49,
48,46,49,0,73,83,
71,78,44,0,0,0,
1,0,0,0,8,0,
0,0,32,0,0,0,
0,0,0,0,0,0,
0,0,3,0,0,0,
0,0,0,0,3,3,
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
83,72,69,88,108,4,
0,0,80,0,0,0,
27,1,0,0,106,8,
0,1,89,0,0,4,
70,142,32,0,0,0,
0,0,19,0,0,0,
89,0,0,4,70,142,
32,0,1,0,0,0,
3,0,0,0,90,0,
0,3,0,96,16,0,
2,0,0,0,90,0,
0,3,0,96,16,0,
3,0,0,0,90,0,
0,3,0,96,16,0,
4,0,0,0,88,24,
0,4,0,112,16,0,
2,0,0,0,85,85,
0,0,88,24,0,4,
0,112,16,0,3,0,
0,0,85,85,0,0,
88,24,0,4,0,112,
16,0,4,0,0,0,
85,85,0,0,98,16,
0,3,50,16,16,0,
0,0,0,0,101,0,
0,3,242,32,16,0,
0,0,0,0,104,0,
0,2,4,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,0,0,
0,0,70,16,16,0,
0,0,0,0,70,126,
16,0,3,0,0,0,
0,96,16,0,3,0,
0,0,69,0,0,139,
194,0,0,128,67,85,
21,0,114,0,16,0,
1,0,0,0,70,16,
16,0,0,0,0,0,
70,126,16,0,2,0,
0,0,0,96,16,0,
2,0,0,0,0,0,
0,8,18,0,16,0,
0,0,0,0,10,0,
16,0,0,0,0,0,
10,0,16,128,65,0,
0,0,1,0,0,0,
0,0,0,7,18,0,
16,0,0,0,0,0,
26,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,0,0,
0,8,18,0,16,0,
0,0,0,0,26,0,
16,128,65,0,0,0,
1,0,0,0,10,0,
16,0,0,0,0,0,
0,0,0,7,18,0,
16,0,0,0,0,0,
42,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,0,0,
0,8,18,0,16,0,
0,0,0,0,42,0,
16,128,65,0,0,0,
1,0,0,0,10,0,
16,0,0,0,0,0,
56,0,0,8,18,0,
16,0,0,0,0,0,
10,0,16,128,129,0,
0,0,0,0,0,0,
1,64,0,0,171,170,
170,62,49,0,0,8,
34,0,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,42,128,
32,0,0,0,0,0,
17,0,0,0,55,0,
0,9,18,0,16,0,
0,0,0,0,26,0,
16,0,0,0,0,0,
1,64,0,0,0,0,
0,0,10,0,16,0,
0,0,0,0,56,0,
0,7,114,0,16,0,
0,0,0,0,6,0,
16,0,0,0,0,0,
70,2,16,0,1,0,
0,0,50,0,0,14,
114,0,16,0,0,0,
0,0,70,2,16,128,
65,0,0,0,0,0,
0,0,6,128,32,0,
0,0,0,0,17,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,1,0,0,0,
70,16,16,0,0,0,
0,0,70,126,16,0,
4,0,0,0,0,96,
16,0,4,0,0,0,
14,0,0,11,18,0,
16,0,2,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
128,63,10,128,32,0,
1,0,0,0,2,0,
0,0,54,0,0,5,
34,0,16,0,2,0,
0,0,1,64,0,0,
0,0,0,0,0,0,
0,7,194,0,16,0,
2,0,0,0,6,4,
16,0,2,0,0,0,
6,20,16,0,0,0,
0,0,0,0,0,8,
50,0,16,0,2,0,
0,0,70,0,16,128,
65,0,0,0,2,0,
0,0,70,16,16,0,
0,0,0,0,69,0,
0,139,194,0,0,128,
67,85,21,0,114,0,
16,0,3,0,0,0,
70,0,16,0,2,0,
0,0,70,126,16,0,
4,0,0,0,0,96,
16,0,4,0,0,0,
69,0,0,139,194,0,
0,128,67,85,21,0,
114,0,16,0,2,0,
0,0,230,10,16,0,
2,0,0,0,70,126,
16,0,4,0,0,0,
0,96,16,0,4,0,
0,0,0,0,0,7,
114,0,16,0,1,0,
0,0,70,2,16,0,
1,0,0,0,70,2,
16,0,2,0,0,0,
0,0,0,7,114,0,
16,0,1,0,0,0,
70,2,16,0,3,0,
0,0,70,2,16,0,
1,0,0,0,56,0,
0,8,114,0,16,0,
1,0,0,0,70,2,
16,0,1,0,0,0,
86,133,32,0,0,0,
0,0,17,0,0,0,
50,0,0,16,114,0,
16,0,1,0,0,0,
70,2,16,128,65,0,
0,0,1,0,0,0,
2,64,0,0,171,170,
170,62,171,170,170,62,
171,170,170,62,0,0,
0,0,2,64,0,0,
0,0,128,63,0,0,
128,63,0,0,128,63,
0,0,0,0,50,0,
0,13,114,0,16,0,
0,0,0,0,70,2,
16,128,65,0,0,0,
0,0,0,0,70,2,
16,0,1,0,0,0,
2,64,0,0,0,0,
128,63,0,0,128,63,
0,0,128,63,0,0,
0,0,56,0,0,8,
18,32,16,0,0,0,
0,0,10,0,16,0,
0,0,0,0,58,128,
32,0,0,0,0,0,
17,0,0,0,56,0,
0,8,98,32,16,0,
0,0,0,0,86,6,
16,0,0,0,0,0,
6,129,32,0,0,0,
0,0,18,0,0,0,
54,0,0,5,130,32,
16,0,0,0,0,0,
1,64,0,0,0,0,
128,63,62,0,0,1,
83,84,65,84,148,0,
0,0,28,0,0,0,
4,0,0,0,0,0,
0,0,2,0,0,0,
19,0,0,0,0,0,
0,0,0,0,0,0,
1,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,5,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
2,0,0,0,1,0,
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
class VhsShadersVHSProVHSPro_ThirdShaderDef : public ShaderDef
{
public:
	VhsShadersVHSProVHSPro_ThirdShaderDef() : ShaderDef{}
	{
		Name = "VHSPro_Third";
		VertexByteCode = RetroArchVhsShadersVHSProVHSPro_ThirdShaderDefs::sVertexByteCode;
		VertexLength = sizeof(RetroArchVhsShadersVHSProVHSPro_ThirdShaderDefs::sVertexByteCode);
		FragmentByteCode = RetroArchVhsShadersVHSProVHSPro_ThirdShaderDefs::sFragmentByteCode;
		FragmentLength = sizeof(RetroArchVhsShadersVHSProVHSPro_ThirdShaderDefs::sFragmentByteCode);
		Params.push_back(ShaderParam("screenLinesNum", 0, 64, 4, 1.000000f, 2160.000000f, 1080.000000f));
		Params.push_back(ShaderParam("VHS_Bleed_toggle", 0, 68, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_BleedMode_select", 0, 72, 4, 1.000000f, 5.000000f, 1.000000f));
		Params.push_back(ShaderParam("bleedAmount", 0, 76, 4, 0.000000f, 15.000000f, 0.500000f));
		Params.push_back(ShaderParam("VHS_FishEye_toggle", 0, 80, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("VHS_FishEye_Hyperspace_toggle", 0, 84, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("fisheyeBend", 0, 88, 4, 0.000000f, 3.000000f, 0.500000f));
		Params.push_back(ShaderParam("cutoffX", 0, 92, 4, 0.000000f, 50.000000f, 2.000000f));
		Params.push_back(ShaderParam("cutoffY", 0, 96, 4, 0.000000f, 50.000000f, 3.000000f));
		Params.push_back(ShaderParam("cutoffFadeX", 0, 100, 4, 0.000000f, 50.000000f, 25.000000f));
		Params.push_back(ShaderParam("cutoffFadeY", 0, 104, 4, 0.000000f, 50.000000f, 25.000000f));
		Params.push_back(ShaderParam("VHS_Vignette_toggle", 0, 108, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("vignetteAmount", 0, 112, 4, 0.000000f, 5.000000f, 0.300000f));
		Params.push_back(ShaderParam("vignetteSpeed", 0, 116, 4, 0.000000f, 5.000000f, 1.000000f));
		Params.push_back(ShaderParam("noiseLinesNum", 0, 300, 4, 1.000000f, 1080.000000f, 240.000000f));
		Params.push_back(ShaderParam("noiseQuantizeX", 0, 124, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("VHS_FilmGrain_toggle", 0, 128, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("filmGrainAmount", 0, 132, 4, 0.000000f, 1.000000f, 0.500000f));
		Params.push_back(ShaderParam("VHS_YIQNoise_toggle", 0, 136, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("signalNoiseType_select", 0, 140, 4, 1.000000f, 3.000000f, 1.000000f));
		Params.push_back(ShaderParam("signalNoiseAmount", 0, 144, 4, 0.000000f, 1.000000f, 0.100000f));
		Params.push_back(ShaderParam("signalNoisePower", 0, 148, 4, 0.000000f, 1.000000f, 0.750000f));
		Params.push_back(ShaderParam("VHS_LineNoise_toggle", 0, 152, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("lineNoiseAmount", 0, 156, 4, 0.000000f, 10.000000f, 1.000000f));
		Params.push_back(ShaderParam("lineNoiseSpeed", 0, 160, 4, 0.000000f, 10.000000f, 5.000000f));
		Params.push_back(ShaderParam("VHS_TapeNoise_toggle", 0, 164, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("tapeNoiseTH", 0, 168, 4, 0.000000f, 1.500000f, 0.630000f));
		Params.push_back(ShaderParam("tapeNoiseAmount", 0, 172, 4, 0.000000f, 1.500000f, 0.050000f));
		Params.push_back(ShaderParam("tapeNoiseSpeed", 0, 176, 4, -1.500000f, 1.500000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_ScanLines_toggle", 0, 180, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("scanLineWidth", 0, 184, 4, 0.000000f, 20.000000f, 10.000000f));
		Params.push_back(ShaderParam("VHS_LinesFloat_toggle", 0, 188, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("linesFloatSpeed", 0, 192, 4, -3.000000f, 3.000000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_Stretch_toggle", 0, 196, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_Jitter_H_toggle", 0, 200, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("jitterHAmount", 0, 204, 4, 0.000000f, 5.000000f, 0.150000f));
		Params.push_back(ShaderParam("VHSJitter_V_toggle", 0, 208, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("jitterVAmount", 0, 212, 4, 0.000000f, 15.000000f, 7.500000f));
		Params.push_back(ShaderParam("jitterVSpeed", 0, 216, 4, 0.000000f, 5.000000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_Twitch_H_toggle", 0, 220, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("twitchHFreq", 0, 224, 4, 0.000000f, 5.000000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_Twitch_V_toggle", 0, 228, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("twitchVFreq", 0, 232, 4, 0.000000f, 5.000000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_SignalTweak_toggle", 0, 236, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("signalAdjustY", 0, 240, 4, -0.250000f, 0.250000f, 0.000000f));
		Params.push_back(ShaderParam("signalAdjustI", 0, 244, 4, -0.250000f, 0.250000f, 0.000000f));
		Params.push_back(ShaderParam("signalAdjustQ", 0, 248, 4, -0.250000f, 0.250000f, 0.000000f));
		Params.push_back(ShaderParam("signalShiftY", 0, 252, 4, -2.000000f, 2.000000f, 1.000000f));
		Params.push_back(ShaderParam("signalShiftI", 0, 256, 4, -2.000000f, 2.000000f, 1.000000f));
		Params.push_back(ShaderParam("signalShiftQ", 0, 260, 4, -2.000000f, 2.000000f, 1.000000f));
		Params.push_back(ShaderParam("gammaCorection", 0, 264, 4, 0.000000f, 2.000000f, 1.000000f));
		Params.push_back(ShaderParam("VHS_Feedback_toggle", 0, 268, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("feedbackAmount", 0, 272, 4, 0.000000f, 3.000000f, 2.000000f));
		Params.push_back(ShaderParam("feedbackFade", 0, 276, 4, 0.000000f, 1.000000f, 0.820000f));
		Params.push_back(ShaderParam("feedbackThresh", 0, 280, 4, 0.000000f, 1.000000f, 0.100000f));
		Params.push_back(ShaderParam("feedbackColor_R", 0, 284, 4, 0.000000f, 1.000000f, 1.000000f));
		Params.push_back(ShaderParam("feedbackColor_G", 0, 288, 4, 0.000000f, 1.000000f, 0.500000f));
		Params.push_back(ShaderParam("feedbackColor_B", 0, 292, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("feedbackDebug_toggle", 0, 296, 4, 0.000000f, 1.000000f, 0.000000f));
		Params.push_back(ShaderParam("MVP", 0, 0, 64, 0.000000f, 0.000000f, 0.000000f));
		Params.push_back(ShaderParam("SourceSize", -1, 0, 16, 0.000000f, 0.000000f, 0.000000f));
		Params.push_back(ShaderParam("OriginalSize", -1, 16, 16, 0.000000f, 0.000000f, 0.000000f));
		Params.push_back(ShaderParam("OutputSize", -1, 32, 16, 0.000000f, 0.000000f, 0.000000f));
		Params.push_back(ShaderParam("FrameCount", -1, 48, 4, 0.000000f, 0.000000f, 0.000000f));
		Params.push_back(ShaderParam("noiseLinesLum", 0, 120, 4, 0.000000f, 0.000000f, 0.000000f));
		Samplers.push_back(ShaderSampler("Original", 2));
		Samplers.push_back(ShaderSampler("OriginalHistory1", 3));
		Samplers.push_back(ShaderSampler("PassFeedback0", 4));
/*
VertexSource = %*VERTEX_SOURCE*%;
*/
/*
FragmentSource = %*FRAGMENT_SOURCE*%;
*/
	}
};
}