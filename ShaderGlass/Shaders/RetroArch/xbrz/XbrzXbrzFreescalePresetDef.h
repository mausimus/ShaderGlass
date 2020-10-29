/*
ShaderGlass preset xbrz / xbrz-freescale imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbrz/xbrz-freescale.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrzXbrzFreescalePresetDef : public PresetDef
{
public:
	XbrzXbrzFreescalePresetDef() : PresetDef{}
	{
		Name = "xbrz-freescale";
		Category = "xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrzShadersXbrzFreescaleShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
