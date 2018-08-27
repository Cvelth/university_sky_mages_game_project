#include "server.hpp"
#include <iostream>
#include "Engines/Networking/Networking.hpp"
#include "Engines/Networking/Message.hpp"
void actors_broadcast(MainActorQueue &actors) {
	std::cout << "Broadcasting actor queue...";
	Networking::bcast_from_server(make_actor_queue_message(actors));
	std::cout << "\rActor queue was broadcasted.\n";
}
void actors_help() {
	std::cout << "Supported commands:\n"
		<< " - actors broadcast - sends actor data to all the clients.\n";
}
void actors_(MainActorQueue &actors, std::istream &input) {
	std::string string;
	input >> string; if (string == "broadcast")
		actors_broadcast(actors);
	else if (string == "help")
		actors_help();
	else
		std::cout << "Unsupported actors-related server command.\nCall \"actors help\" for list of supported ones.\n";
}

#include "Shared/SharedFunctions.hpp"
#include "Engines/ObjectStorage/Objects.hpp"
#include "Engines/ObjectStorage/Settings.hpp"

#include "Objects/Objects/ObjectQueue.hpp"
#include "Objects/Actors/MainActor.hpp"

#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Shield.hpp"
#include "Objects/EquipableItems/Trinket.hpp"
#include "Objects/EquipableItems/Upgrade.hpp"
#include "Objects/EquipableItems/Weapon.hpp"

void actors_add(std::shared_ptr<Objects> objects, MainActorQueue &actors) {
	std::cout << "\rAdding new actor...";
	auto actor = std::make_shared<MainActor>(60.f, mgl::math::vector{0.f,0.f}, mgl::math::vector{0.f,0.f}, mgl::math::vector{30.f,50.f}, mgl::math::vector{2.f, 4.f}, Functions::string_indexation("MainActor", actors.size()));

	auto energy_storage = objects->get_energy_storage("");
	actor->giveEnergyStorage(energy_storage);

	auto fly_engine = objects->get_fly_engine("");
	actor->giveFlyEngine(fly_engine);

	auto left_weapon = objects->get_weapon("");
	actor->giveLeftWeapon(left_weapon);

	auto right_weapon = objects->get_weapon("");
	actor->giveRightWeapon(right_weapon);

	auto shield = objects->get_shield_generator("");
	actor->giveShieldGenerator(shield);

	auto trinket = objects->get_trinket("");
	actor->giveTrinket(trinket);

	auto upgrade = objects->get_upgrade("");
	actor->giveUpgrade(upgrade);

	std::cout << "\rActor was given:   \n   "
		<< energy_storage->name() << ",\n   " << fly_engine->name() << ",\n   " << left_weapon->name() << ",\n   "
		<< right_weapon->name() << ",\n   " << shield->name() << ",\n   ";
	if (trinket)
		std::cout << trinket->name() << ",\n   ";
	else
		std::cout << "no trinket,\n   ";
	if (upgrade)
		std::cout << upgrade->name() << ".\n";
	else
		std::cout << "no upgrade.\n";

	actors.add(actor);
	std::cout << "\rActor was generated.\n";
	actors_broadcast(actors);
}