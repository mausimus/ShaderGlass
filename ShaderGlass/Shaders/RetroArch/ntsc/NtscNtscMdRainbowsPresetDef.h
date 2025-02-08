/*
ShaderGlass preset ntsc / ntsc-md-rainbows imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/ntsc-md-rainbows.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscMdRainbowsPresetDef : public PresetDef
{
public:
	NtscNtscMdRainbowsPresetDef() : PresetDef{}
	{
		Name = "ntsc-md-rainbows";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_ntsc_encodeShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_ntsc_decodeShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source"));
            OverrideParam("avalue", (float)0.000000);
            OverrideParam("bvalue", (float)0.000000);
            OverrideParam("ifreqresponse", (float)1.750000);
            OverrideParam("notch_width", (float)3.450001);
            OverrideParam("ntscsignal", (float)1.000000);
            OverrideParam("qfreqresponse", (float)1.450000);
            OverrideParam("scale_1", (float)1.000000);
            OverrideParam("scantime", (float)47.900070);
            OverrideParam("shadowalpha", (float)0.100000);
	}
};
}
