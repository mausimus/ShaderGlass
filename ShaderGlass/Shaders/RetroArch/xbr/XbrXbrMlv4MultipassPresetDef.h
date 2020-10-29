/*
ShaderGlass preset xbr / xbr-mlv4-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/xbr-mlv4-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrXbrMlv4MultipassPresetDef : public PresetDef
{
public:
	XbrXbrMlv4MultipassPresetDef() : PresetDef{}
	{
		Name = "xbr-mlv4-multipass";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "REF")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersXbrMlv4MultipassXbrMlv4Pass1ShaderDef()
.Param("alias", "PASS1")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersXbrMlv4MultipassXbrMlv4Pass2ShaderDef()
.Param("alias", "PASS2")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersXbrMlv4MultipassXbrMlv4Pass3ShaderDef()
.Param("alias", "PASS3")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000"));
         	ShaderDefs.push_back(XbrShadersXbrMlv4MultipassXbrMlv4Pass4ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false"));
	}
};
}
