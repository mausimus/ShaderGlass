/*
ShaderGlass preset subframe-bfi / motionblur_test imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/subframe-bfi/motionblur_test.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SubframeBfiMotionblur_testPresetDef : public PresetDef
{
public:
	SubframeBfiMotionblur_testPresetDef() : PresetDef{}
	{
		Name = "motionblur_test";
		Category = "subframe-bfi";
	}

	void Build() {
         	ShaderDefs.push_back(SubframeBfiShadersMotionblur_testShadersMotionblur_testShaderDef());
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources60TextureDef()
.Param("name", "base"));
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources120TextureDef()
.Param("name", "twoSub"));
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources180TextureDef()
.Param("name", "threeSub"));
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources240TextureDef()
.Param("name", "fourSub"));
	}
};
}
