/*
ShaderGlass preset crt / vt220 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/vt220.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtVt220PresetDef : public PresetDef
{
public:
	CrtVt220PresetDef() : PresetDef{}
	{
		Name = "vt220";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "vt220_refpass"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "PrePass0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedNtscNtscPass1ShaderDef()
.Param("alias", "NPass1")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "4.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedNtscNtscPass2ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedNtscNtscPass3ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersVt220Vt220ShaderDef()
.Param("mipmap_input", "true")
.Param("scale_type", "viewport"));
	}
};
}
