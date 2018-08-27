#pragma once
#include <memory>
#include "RenderInfo.hpp"
namespace mgl {
	class ShaderProgram;
	class ShaderVariable;
	class Primitive;
}
class EnergyStorage;
class HUD_RenderInfo : public RenderInfo {
	mgl::Primitive *m_energy_bar;
	std::shared_ptr<EnergyStorage> m_energy_source;
	float m_current_energy_percent;
public:
	HUD_RenderInfo(std::shared_ptr<EnergyStorage> energy_source);
	~HUD_RenderInfo();

	void change_energy_source(std::shared_ptr<EnergyStorage> es) { m_energy_source = es; }
	virtual void recalculate();
};
