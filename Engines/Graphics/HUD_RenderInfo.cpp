#include "HUD_RenderInfo.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/AbstractDrawableObject.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/Primitive.hpp"
#include "../MyGraphicsLibrary/MGL/SharedHeaders/Color.hpp"
#include "../MyGraphicsLibrary/MGL/Math/Vector.hpp"
#include "Engines/ObjectStorage/RenderInfoStorage.hpp"

mgl::Primitive* make_rectangle(mgl::VertexConnectionType connection, std::shared_ptr<mgl::Color> color, mgl::math::vector p0, mgl::math::vector p1, mgl::math::vector p2, mgl::math::vector p3) {
	auto ret = new mgl::Primitive(connection, &*color);
	ret->initialize_data_edit();
	ret->insert(p0);
	ret->insert(p1);
	ret->insert(p2);
	ret->insert(p3);
	ret->stop_data_edit();
	return ret;
}

void HUD_RenderInfo::initialize_energy_bar() {
	addPrimitive(make_rectangle(mgl::VertexConnectionType::TriangleStrip,
								RenderInfoStorage::getPalette("EnergyBar").at(0),
								mgl::math::vector(0.975f, 0.01f),
								mgl::math::vector(0.995f, 0.01f),
								mgl::math::vector(0.975f, 0.99f),
								mgl::math::vector(0.995f, 0.99f)));
	addPrimitive(make_rectangle(mgl::VertexConnectionType::TriangleStrip,
								RenderInfoStorage::getPalette("EnergyBar").at(1),
								mgl::math::vector(0.978f, 0.015f),
								mgl::math::vector(0.992f, 0.015f),
								mgl::math::vector(0.978f, 0.985f),
								mgl::math::vector(0.992f, 0.985f)));
	addPrimitive(m_energy_bar = new mgl::Primitive(mgl::VertexConnectionType::TriangleStrip,
												   &*RenderInfoStorage::getPalette("EnergyBar").at(2)));
}
void HUD_RenderInfo::initialize_weapon_bars() {
	for (auto i = 0; i < 2; i++) {
		addPrimitive(make_rectangle(mgl::VertexConnectionType::TriangleStrip,
									RenderInfoStorage::getPalette("EnergyBar").at(0),
									mgl::math::vector(0.80f, 0.970f - 0.025f * i),
									mgl::math::vector(0.80f, 0.990f - 0.025f * i),
									mgl::math::vector(0.97f, 0.970f - 0.025f * i),
									mgl::math::vector(0.97f, 0.990f - 0.025f * i)));
		addPrimitive(make_rectangle(mgl::VertexConnectionType::TriangleStrip,
									RenderInfoStorage::getPalette("EnergyBar").at(1),
									mgl::math::vector(0.803f, 0.973f - 0.025f * i),
									mgl::math::vector(0.803f, 0.987f - 0.025f * i),
									mgl::math::vector(0.967f, 0.973f - 0.025f * i),
									mgl::math::vector(0.967f, 0.987f - 0.025f * i)));
		addPrimitive(m_weapon_bars[i] = new mgl::Primitive(mgl::VertexConnectionType::TriangleStrip,
														   &*RenderInfoStorage::getPalette("EnergyBar").at(2)));
	}
}
void HUD_RenderInfo::initialize_anti_gravity_indicator() {
	addPrimitive(make_rectangle(mgl::VertexConnectionType::TriangleStrip,
								RenderInfoStorage::getPalette("EnergyBar").at(0),
								mgl::math::vector(0.93f, 0.010f),
								mgl::math::vector(0.93f, 0.050f),
								mgl::math::vector(0.97f, 0.010f),
								mgl::math::vector(0.97f, 0.050f)));
	m_anti_gravity_indicator[0] = make_rectangle(mgl::VertexConnectionType::TriangleStrip,
												 RenderInfoStorage::getPalette("EnergyBar").at(1),
												 mgl::math::vector(0.933f, 0.013f),
												 mgl::math::vector(0.933f, 0.047f),
												 mgl::math::vector(0.967f, 0.013f),
												 mgl::math::vector(0.967f, 0.047f));
	m_anti_gravity_indicator[1] = make_rectangle(mgl::VertexConnectionType::TriangleStrip,
												 RenderInfoStorage::getPalette("EnergyBar").at(2),
												 mgl::math::vector(0.933f, 0.013f),
												 mgl::math::vector(0.933f, 0.047f),
												 mgl::math::vector(0.967f, 0.013f),
												 mgl::math::vector(0.967f, 0.047f));
}
void HUD_RenderInfo::initialize_shield_indicator() {
	addPrimitive(make_rectangle(mgl::VertexConnectionType::TriangleStrip,
								RenderInfoStorage::getPalette("EnergyBar").at(0),
								mgl::math::vector(0.885f, 0.010f),
								mgl::math::vector(0.885f, 0.050f),
								mgl::math::vector(0.925f, 0.010f),
								mgl::math::vector(0.925f, 0.050f)));
	m_shield_indicator[0] = make_rectangle(mgl::VertexConnectionType::TriangleStrip,
										   RenderInfoStorage::getPalette("EnergyBar").at(1),
										   mgl::math::vector(0.888f, 0.013f),
										   mgl::math::vector(0.888f, 0.047f),
										   mgl::math::vector(0.923f, 0.013f),
										   mgl::math::vector(0.923f, 0.047f));
	m_shield_indicator[1] = make_rectangle(mgl::VertexConnectionType::TriangleStrip,
										   RenderInfoStorage::getPalette("EnergyBar").at(2),
										   mgl::math::vector(0.888f, 0.013f),
										   mgl::math::vector(0.888f, 0.047f),
										   mgl::math::vector(0.923f, 0.013f),
										   mgl::math::vector(0.923f, 0.047f));
}

#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
void HUD_RenderInfo::recalculate_energy_percent() {
	float current_capacity;
	if (m_actor && m_actor->energy_storage())
		current_capacity = m_actor->energy_storage()->getCapacityPercent();
	else
		current_capacity = 0.f;

	if (m_current_energy_percent != current_capacity) {
		m_current_energy_percent = current_capacity;
		current_capacity *= 0.97f;
		current_capacity += 0.015f;
		current_capacity = 1.f - current_capacity;

		m_energy_bar->deleteAll();
		m_energy_bar->initialize_data_edit();
		m_energy_bar->insert(mgl::math::vector(0.978f, current_capacity));
		m_energy_bar->insert(mgl::math::vector(0.992f, current_capacity));
		m_energy_bar->insert(mgl::math::vector(0.978f, 0.985f));
		m_energy_bar->insert(mgl::math::vector(0.992f, 0.985f));
		m_energy_bar->stop_data_edit();
		m_energy_bar->send(mgl::DataUsage::StreamDraw);
	}
}
#include "Objects/EquipableItems/Weapon.hpp"
void HUD_RenderInfo::recalculate_ammo_values() {
	float current_values[2]{0.f, 0.f};
	if (m_actor) {
		if (m_actor->left_weapon())
			current_values[0] = m_actor->left_weapon()->get_ammo_value();
		if (m_actor->right_weapon())
			current_values[1] = m_actor->right_weapon()->get_ammo_value();
	}
	
	for (auto i = 0; i < 2; i++) {
		if (m_current_ammo_values[i] != current_values[i]) {
			m_current_ammo_values[i] = current_values[i];
			current_values[i] = 1.f - current_values[i];
			current_values[i] *= 0.154f;
			current_values[i] += 0.803f;

			m_weapon_bars[i]->deleteAll();
			m_weapon_bars[i]->initialize_data_edit();
			m_weapon_bars[i]->insert(mgl::math::vector(current_values[i], 0.973f - 0.025f * i));
			m_weapon_bars[i]->insert(mgl::math::vector(current_values[i], 0.987f - 0.025f * i));
			m_weapon_bars[i]->insert(mgl::math::vector(0.967f, 0.973f - 0.025f * i));
			m_weapon_bars[i]->insert(mgl::math::vector(0.967f, 0.987f - 0.025f * i));
			m_weapon_bars[i]->stop_data_edit();
			m_weapon_bars[i]->send(mgl::DataUsage::StreamDraw);

		}
	}
}

HUD_RenderInfo::~HUD_RenderInfo() {
	//delete m_anti_gravity_indicator[0];
	//delete m_anti_gravity_indicator[1];
	//delete m_shield_indicator[0];
	//delete m_shield_indicator[1];
}

#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Shield.hpp"
void HUD_RenderInfo::insertVertexArray(mgl::VertexArray *vao) {
	RenderInfo::insertVertexArray(vao);
	m_anti_gravity_indicator[0]->insertVertexArray(vao);
	m_anti_gravity_indicator[1]->insertVertexArray(vao);
	m_shield_indicator[0]->insertVertexArray(vao);
	m_shield_indicator[1]->insertVertexArray(vao);
}
void HUD_RenderInfo::send(mgl::DataUsage const& u) {
	RenderInfo::send(u);
	m_anti_gravity_indicator[0]->send(mgl::DataUsage::StaticDraw);
	m_anti_gravity_indicator[1]->send(mgl::DataUsage::StaticDraw);
	m_shield_indicator[0]->send(mgl::DataUsage::StaticDraw);
	m_shield_indicator[1]->send(mgl::DataUsage::StaticDraw);
}
void HUD_RenderInfo::draw() {
	RenderInfo::draw();
	if (m_actor && m_actor->fly_engine() && m_actor->fly_engine()->is_anti_gravity_active())
		m_anti_gravity_indicator[1]->draw();
	else
		m_anti_gravity_indicator[0]->draw();

	if (m_actor && m_actor->shield() && m_actor->shield()->is_active())
		m_shield_indicator[1]->draw();
	else
		m_shield_indicator[0]->draw();
}
void HUD_RenderInfo::clean() {
	RenderInfo::clean();
	m_anti_gravity_indicator[0]->clean();
	m_anti_gravity_indicator[1]->clean();
	m_shield_indicator[0]->clean();
	m_shield_indicator[1]->clean();
}
