#include "NetworkController.hpp"
#include "Networking.hpp"
#include "Shared/GameStateController.hpp"

void NetworkController::send_control_event(ControlEvent ev, bool direction) {
	if (GameStateController::mode() != ProgramMode::Client)
		throw Exceptions::GameStateException("Only client-mode applications can send control events.");
	std::string data("C x");
	data[2] = char(ev);
	if (direction) data[2] |= (1 << 7);
	Networking::send_to_server(data, ControlsData, true);
}
#include <sstream>
void NetworkController::send_aim_event(float x, float y) {
	if (GameStateController::mode() != ProgramMode::Client)
		throw Exceptions::GameStateException("Only client-mode applications can send control events.");
	std::ostringstream s;
	s << "A " << x << ' ' << y;
	Networking::send_to_server(s.str(), ControlsData, false);
}

std::pair<NetworkController::ControlEvent, bool> NetworkController::parse_control_event(std::string const& data) {
	if (data.size() != 3 || data[0] != 'C') throw Exceptions::NetworkingException("Control data seems to be corrupted.");
	return std::make_pair(ControlEvent(data[2] & ((1 << 7) - 1)), data[2] & (1 << 7));
}
std::pair<float, float> NetworkController::parse_aim_event(std::string const& data) {
	if (data.size() < 3 || data[0] != 'A') throw Exceptions::NetworkingException("Aim data seems to be corrupted.");
	std::istringstream s(data);
	char a;
	float x, y;
	s >> a >> x >> y;
	return std::make_pair(x, y);
}

#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
void NetworkController::accept_control_event(std::shared_ptr<MainActor> actor, ControlEvent ev, bool direction) {
	switch (ev) {
		case NetworkController::ControlEvent::move_up:
			if (direction)
				actor->fly_engine()->accelerate_up(false);
			else
				actor->fly_engine()->accelerate_up(true);
			break;
		case NetworkController::ControlEvent::move_down:
			if (direction)
				actor->fly_engine()->accelerate_down(false);
			else
				actor->fly_engine()->accelerate_down(true);
			break;
		case NetworkController::ControlEvent::move_left:
			if (direction)
				actor->fly_engine()->accelerate_left(false);
			else
				actor->fly_engine()->accelerate_left(true);
			break;
		case NetworkController::ControlEvent::move_right:
			if (direction)
				actor->fly_engine()->accelerate_right(false);
			else
				actor->fly_engine()->accelerate_right(true);
			break;
		case NetworkController::ControlEvent::anti_gravity:
			actor->fly_engine()->activate_anti_gravity(actor->mass());
			break;
		case NetworkController::ControlEvent::left_weapon:
			if (direction)
				actor->activateLeftWeapon();
			else
				actor->deactivateLeftWeapon();
			break;
		case NetworkController::ControlEvent::right_weapon:
			if (direction)
				actor->activateRightWeapon();
			else
				actor->deactivateRightWeapon();
			break;
	}
}
void NetworkController::accept_aim_event(std::shared_ptr<MainActor> actor, float x, float y) {
	actor->aim(x, y);
}

#include "Objects/ObjectState/ObjectQueue.hpp"
std::string generate_actor_update(MainActorQueue *queue) {
	std::ostringstream s;
	s << queue->size() << '\n';
	queue->for_each([&s](std::shared_ptr<MainActor> object) {
		auto acceleration = object->get_acceleration();
		auto speed = object->speed();
		auto position = object->position();
		s << acceleration.at(0) << ' ' << acceleration.at(1) << ' '
			<< speed.at(0) << ' ' << speed.at(1) << ' '
			<< position.at(0) << ' ' << position.at(1) << '\n';
	});
	return "QueueUpdate\nActor\n" + s.str();
}
#include "Objects/AbstractObjects/ShootableObject.hpp"
#include "Engines/ObjectStorage/RenderInfoStorage.hpp"
std::string generate_projectile_update(ProjectileQueue *queue) {
	std::ostringstream s;
	s << queue->size() << '\n';
	queue->for_each([&s](std::shared_ptr<ShootableObject> object) {
		auto acceleration = object->acceleration();
		auto speed = object->speed();
		auto position = object->position();
		auto size = object->size();
		s << RenderInfoStorage::getRenderInfo(object->getRenderInto()) << ' '
			<< object->mass() << ' ' << acceleration.at(0) << ' ' << acceleration.at(1) << ' '
			<< speed.at(0) << ' ' << speed.at(1) << ' ' << position.at(0) << ' ' << position.at(1) << ' '
			<< size.at(0) << ' ' << size.at(1) << ' ' << object->damage() << '\n';
	});
	return "QueueUpdate\nProjectile\n" + s.str();
}

void NetworkController::update_state(MainActorQueue *actors, ProjectileQueue *projectiles, ObjectQueue *miscellaneous) {
	if (GameStateController::mode() != ProgramMode::Server)
		throw Exceptions::GameStateException("Only server-mode applications can send queue state.");

	Networking::bcast_from_server(generate_actor_update(actors), ActorData, false);
	Networking::bcast_from_server(generate_projectile_update(projectiles), ProjectileData, false);
	//Networking::bcast_from_server(generate_miscellaneous_update(miscellaneous), OtherData, false);
}
void NetworkController::update_state(std::string data, MainActorQueue *actors, ProjectileQueue *projectiles, ObjectQueue *miscellaneous) {
	if (GameStateController::mode() != ProgramMode::Client)
		throw Exceptions::GameStateException("Only client-mode applications can receive queue state.");

	std::string string;
	std::istringstream input(data);
	std::getline(input, string);
	if (string != "QueueUpdate")
		throw Exceptions::ConnectionException("Received MainActorQueue state seems to be corrupted.");

	std::getline(input, string);
	if (string == "Actor") {
		size_t size;
		input >> size;
		if (size != actors->size()) {
			//throw Exceptions::ConnectionException("Received MainActorQueue state seems to be corrupted.");
		}
		actors->for_each([&input](std::shared_ptr<MainActor> actor) {
			scalar ax, ay, vx, vy, px, py;
			input >> ax >> ay >> vx >> vy >> px >> py;
			actor->update_state(vector(ax, ay), vector(vx, vy), vector(px, py));
		});
	} else if (string == "Projectile") {
		size_t size;
		input >> size;
		if (size != 0) {
			projectiles->clear();
			while (input) {
				scalar ax, ay, vx, vy, px, py, sx, sy, d, m;
				input >> string >> m >> ax >> ay >> vx >> vy >> px >> py >> sx >> sy >> d;
				projectiles->add(std::make_shared<ShootableObject>(RenderInfoStorage::getRenderInfo(string), m, vector(ax, ay), vector(vx, vy), vector(px, py), vector(sx, sy), d));
			}
		}
	}
}