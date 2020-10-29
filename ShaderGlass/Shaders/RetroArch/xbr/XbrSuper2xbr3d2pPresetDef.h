/*
ShaderGlass preset xbr / super-2xbr-3d-2p imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/super-2xbr-3d-2p.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrSuper2xbr3d2pPresetDef : public PresetDef
{
public:
	XbrSuper2xbr3d2pPresetDef() : PresetDef{}
	{
		Name = "super-2xbr-3d-2p";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper2xbr3dPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper2xbr3dPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrCustomJinc2SharperShaderDef()
.Param("filter_linear", "false"));
	}
};
}
