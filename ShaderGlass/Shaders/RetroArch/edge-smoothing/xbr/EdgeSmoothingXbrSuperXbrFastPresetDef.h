/*
ShaderGlass preset edge-smoothing-xbr / super-xbr-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/xbr/super-xbr-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingXbrSuperXbrFastPresetDef : public PresetDef
{
public:
	EdgeSmoothingXbrSuperXbrFastPresetDef() : PresetDef{}
	{
		Name = "super-xbr-fast";
		Category = "edge-smoothing-xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSupportLinearizeShaderDef()
.Param("alias", "XbrSource")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrFastPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrFastPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrFastPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrCustomJinc2SharperShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSupportDelinearizeShaderDef()
.Param("filter_linear", "false"));
	}
};
}
