/*
ShaderGlass preset xbrz / xbrz-freescale-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/xbrz/xbrz-freescale-multipass.slangp
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
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "xbrz_fs_refpass"));
         	ShaderDefs.push_back(XbrzShadersXbrzFreescaleMultipassXbrzFreescalePass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrzShadersXbrzFreescaleMultipassXbrzFreescalePass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
