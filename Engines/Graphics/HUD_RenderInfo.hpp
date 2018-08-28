#pragma once
#include <memory>
#include "RenderInfo.hpp"
namespace mgl {
	class ShaderProgram;
	class ShaderVariable;
	class Primitive;
}
class MainActor;
class HUD_RenderInfo : public RenderInfo {
	mgl::Primitive *m_energy_bar;
	mgl::Primitive *m_weapon_bars[2];
	std::shared_ptr<MainActor> m_actor;
	float m_current_energy_percent;
	float m_current_ammo_values[2];
protected:
	void initialize_energy_bar();
	void initialize_weapon_bars();

	void recalculate_energy_percent();
	void recalculate_ammo_values();
public:
	HUD_RenderInfo(std::shared_ptr<MainActor> actor) : m_actor(actor) {
		initialize_energy_bar();
		initialize_weapon_bars();
	}
	virtual ~HUD_RenderInfo() {}

	void changeCenterFigure(std::shared_ptr<MainActor> actor) { m_actor = actor; }
	virtual void recalculate() {
		recalculate_energy_percent();
		recalculate_ammo_values();
	}
};
