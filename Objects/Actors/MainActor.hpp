#pragma once
#include <vector>
#include "Objects/AbstractObjects/ActorObject.hpp"
#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportableItemWasGivenException)

class EnergyStorage;
class FlyEngine;
class Weapon;
class ShieldGenerator;
class Trinket;

class ShootableObject;
class Controller;
class PhysicsEngine; 
class MessageInputStream;
class MessageOutputStream;
template <typename Type> class Update;

class MainActor : public Actor {
	friend Controller;
	//friend PhysicsEngine;
protected:
	EnergyStorage *m_energy_storage;
	FlyEngine *m_engine;
	Weapon *m_weapon_left_arm;
	Weapon *m_weapon_right_arm;
	ShieldGenerator *m_shield;
	Trinket *m_trinket;

	float m_aim_x;
	float m_aim_y;

protected:
	std::shared_ptr<ShootableObject> shootRightWeapon(size_t id);
	std::shared_ptr<ShootableObject> shootLeftWeapon(size_t id);
public:
	MainActor(float mass, vector const& acceleration, vector const& speed,
			  vector const& position, vector const& size, std::shared_ptr<RenderInfo> render_info);
	~MainActor();

	void giveEnergyStorage(EnergyStorage *es);
	void giveFlyEngine(FlyEngine *fe);
	void giveRightWeapon(Weapon *w);
	void giveLeftWeapon(Weapon *w);
	void giveShieldGenerator(ShieldGenerator *sg);
	void giveTrinket(Trinket *t);
	EnergyStorage* takeEnergyStorage();
	FlyEngine* takeFlyEngine();
	Weapon* takeRightWeapon();
	Weapon* takeLeftWeapon();
	ShieldGenerator* takeShieldGenerator();
	Trinket* takeTrinket();

	void aim(float x, float y);
	void activateRightWeapon();
	void deactivateRightWeapon();
	void activateLeftWeapon();
	void deactivateLeftWeapon();
	void activateShieldGenerator();
	void deactivateShieldGenerator();

	std::vector<std::shared_ptr<ShootableObject>> shootingProcess(size_t id);
	void was_hit(std::shared_ptr<ShootableObject> so);

	inline EnergyStorage* energy_storage() const { return m_energy_storage; }
	inline FlyEngine* fly_engine() const { return m_engine; }
	inline Weapon* left_weapon() const { return m_weapon_left_arm; }
	inline Weapon* right_weapon() const { return m_weapon_right_arm; }
	inline ShieldGenerator* shield() const { return m_shield; }
	inline Trinket* trinket() const { return m_trinket; }
public:
	virtual scalar mass() const override;
	virtual vector acceleration(scalar const& time_correct) const override;
	virtual vector get_acceleration() const;
	virtual void update_state(vector const& acceleration, vector const& speed, 
							  vector const& position, float capacity_percent);

	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<MainActor> &v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<MainActor> const& v);
	friend MessageInputStream& operator>>(MessageInputStream &s, Update<std::shared_ptr<MainActor>> &v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, Update<std::shared_ptr<MainActor> const> const& v);
};