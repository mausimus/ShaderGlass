#pragma once

class PassthroughPresetDef : public PresetDef
{
public:
	PassthroughPresetDef() : PresetDef{}
	{
		Name = "none";
		Category = "general";
	ShaderDefs.push_back(PassthroughShaderDef());
	}
};

