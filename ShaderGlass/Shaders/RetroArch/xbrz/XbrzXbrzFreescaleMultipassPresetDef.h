/*
ShaderGlass preset xbrz / xbrz-freescale-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbrz/xbrz-freescale-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrzXbrzFreescaleMultipassPresetDef : public PresetDef
{
public:
	XbrzXbrzFreescaleMultipassPresetDef() : PresetDef{}
	{
		Name = "xbrz-freescale-multipass";
		Category = "xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrzShadersXbrzFreescalePass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrzShadersXbrzFreescalePass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
