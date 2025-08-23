/*
ShaderGlass preset subframe-bfi / 120hz-smart-BFI imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/subframe-bfi/120hz-smart-BFI.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SubframeBfi120hzSmartBFIPresetDef : public PresetDef
{
public:
	SubframeBfi120hzSmartBFIPresetDef() : PresetDef{}
	{
		Name = "120hz-smart-BFI";
		Category = "subframe-bfi";
	}

	void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "bfiRefPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(SubframeBfiShaders120hzSmartBFICalculationsShaderDef()
.Param("alias", "calcPass")
.Param("filter_linear", "false")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(SubframeBfiShaders120hzSmartBFIBfi_flickerShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
