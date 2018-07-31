#include "client.hpp"
int main() { return client_main(); }


#include "Shared/GameMode.hpp"
#include "Client/Window/Window.hpp"
#include "Client/Controller/ControllerInterface.hpp"
#include "Engines/Physics/PhysicsEngine.hpp"
#include "Engines/RenderTools/RenderInfoStorage.hpp"
#include "Engines/Camera/Camera.hpp"
#include "Objects/Map/MapGenerator.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
#include "Client/Settings/Settings.hpp"
#include "Shared/AbstractException.hpp"
#include "Engines/RenderTools/HUD_RenderInfo.hpp"
#include "Objects/ObjectQueue/ObjectQueue.hpp"

#include <thread>

FlyEngine* initializeFlyEngine(EnergyStorage* energy_source) {
	FlyEngineControls fe_settings;
	fe_settings.anti_gravity_mode_off_up_acceleration_percent = 1.f;
	fe_settings.anti_gravity_mode_off_down_acceleration_percent = 0.05f;
	fe_settings.anti_gravity_mode_off_left_acceleration_percent = 0.55f;
	fe_settings.anti_gravity_mode_off_right_acceleration_percent = 0.55f;
	fe_settings.anti_gravity_mode_on_up_acceleration_percent = 0.55f;
	fe_settings.anti_gravity_mode_on_down_acceleration_percent = 0.55f;
	fe_settings.anti_gravity_mode_on_left_acceleration_percent = 0.55f;
	fe_settings.anti_gravity_mode_on_right_acceleration_percent = 0.55f;
	return new FlyEngine(energy_source, 1.f, 1300.f, 3.f, fe_settings);
}

class TestWeapon : public Weapon {
public:
	TestWeapon() : Weapon(AmmoProjectileType::Bullet, WeaponSize::One_Arm, 8.f) {
		m_damage = 1.f;
		m_firerate = 60.f;
		m_autofire_supported = true;

		m_initial_ammo_speed = 200.f;
		m_initial_ammo_mass = 0.1f;
		m_initial_ammo_size_h = 0.005f;
		m_initial_ammo_size_v = 0.005f;

		m_ammo_capacity = 5;
		m_reload_cooldown = 0.5f;
		m_energy_efficency_multiplier = 0.25f;

		m_current_ammo = m_ammo_capacity;
	}
};

void game_process(Settings& s) {
	GameModeController::startInitialization();
	ControllerInterface* controller = new ControllerInterface();
	auto keys = s.getKeysValue("Keys_Layout");
	controller->startKeyControl(&keys);

	auto main_object_queue = new MainActorQueue();
	auto projectile_queue = new ProjectileQueue();
	auto miscellaneous_object_queue = new ObjectQueue();

	Window* window = new Window(s.getProgramVersionInfo().c_str(),
		s.getUintValue("Screen_Width"),
		s.getUintValue("Screen_Height"),
		s.getBoolValue("Fullscreen_Window"),
		main_object_queue, projectile_queue, miscellaneous_object_queue);
	window->insertController(controller);
	window->changeUpdateInterval(1'000'000 / s.getUintValue("Graphical_Updates_Per_Second"));


	PhysicsEngine* physics_engine = new PhysicsEngine([&window](void) {
		return window->isWindowClosed();
	}, main_object_queue, projectile_queue, miscellaneous_object_queue);
	physics_engine->changeUpdateInterval(1'000'000 / s.getUintValue("Physical_Updates_Per_Second"));

	RenderInfoStorage::generateRenderInfo();

	MainActor* main_actor = new MainActor(RenderInfoStorage::getMainActorRenderInfo(),
		60.f, 2.f, 4.f, 40.f, 30.f);
	auto energy_storage = new EnergyStorage(1.e6f, 5.f);
	main_actor->giveEnergyStorage(energy_storage);
	main_actor->giveFlyEngine(initializeFlyEngine(energy_storage));
	main_actor->giveRightWeapon(new TestWeapon());

	controller->setMainActor(main_actor);
	main_object_queue->add(main_actor);

	Map *map = MapGenerator::generate_continious_map(120, 80);
	Camera *camera = new Camera(map, main_actor, window->currentAspectRatio(), 100.f);
	window->insertCamera(camera);
	physics_engine->initializeCollisionSystem(map);

	auto hud = new HUD_RenderInfo(energy_storage);
	window->insertHUDRenderInfo(hud);

	std::thread physics_thread(&PhysicsEngine::loop, physics_engine, false);
	window->loop(false);
	physics_thread.join();

	delete hud;
	delete camera;
	delete map;
	delete main_actor;
	delete physics_engine;
	delete window;
	delete main_object_queue;
	delete projectile_queue;
	delete miscellaneous_object_queue;
	delete controller;
}

int client_main() {
	Settings s;
	try {
		s.load();
	}
	catch (Exceptions::SettingsVersionException &e) {
		s.backup();
		s.nullify();
		s.save();
		e.print();
	}
	catch (Exceptions::SettingsAccessException &e) {
		s.backup();
		s.nullify();
		s.save();
		e.print();
	}
	catch (Exceptions::NoSettingsFileException &e) {
		s.nullify();
		s.save();
		e.print();
	}

	try {
		game_process(s);
	}
	catch (Exceptions::AbstractException& e) {
		e.print();
		getchar(); // Prevents Program from closing.
	}
	return 0;
}