#include "KeyLayout.hpp"
#include <string>
#include <iostream>
#include "../MyGraphicsLibrary/MGL/Events/EventEnums.hpp"

std::ostream& operator<<(std::ostream &s, mgl::Key *k) {
	s << size_t(*k);
	return s;
}

std::istream& operator>>(std::istream &s, mgl::Key *k) {
	size_t t;
	s >> t;
	*k = mgl::Key(t);
	return s;
}

std::ostream& operator<<(std::ostream &s, KeyLayout &k) {
	s << k.move_up << " " << k.move_down << " " << k.move_left << " " << k.move_right << " " 
		<< k.anti_gravity << " " << k.shield << " " << k.pause << " " 
		<< k.hold_anti_gravity_key << " " << k.hold_shield_key;
	return s;
}

std::istream& operator>>(std::istream &s, KeyLayout &k) {
	s >> k.move_up >> k.move_down >> k.move_left >> k.move_right 
		>> k.anti_gravity >> k.shield >> k.pause 
		>> k.hold_anti_gravity_key >> k.hold_shield_key;
	return s;
}

KeyLayout::KeyLayout(const KeyLayout &other)
	: move_up(new mgl::Key(mgl::Key(*other.move_up))), move_down(new mgl::Key(mgl::Key(*other.move_down))),
	move_left(new mgl::Key(mgl::Key(*other.move_left))), move_right(new mgl::Key(mgl::Key(*other.move_right))),
	anti_gravity(new mgl::Key(mgl::Key(*other.anti_gravity))), shield(new mgl::Key(mgl::Key(*other.shield))),
	pause(new mgl::Key(mgl::Key(*other.pause))),
	hold_anti_gravity_key(other.hold_anti_gravity_key), hold_shield_key(other.hold_shield_key) {}

KeyLayout::KeyLayout(const size_t _move_up, const size_t _move_down, const size_t _move_left, const size_t _move_right,
					 const size_t _anti_gravity, const size_t _shield, const size_t _pause,
					 const bool _hold_anti_gravity_key, const bool _hold_shield_key)
	: move_up(new mgl::Key(mgl::Key(_move_up))), move_down(new mgl::Key(mgl::Key(_move_down))), 
	move_left(new mgl::Key(mgl::Key(_move_left))), move_right(new mgl::Key(mgl::Key(_move_right))),
	anti_gravity(new mgl::Key(mgl::Key(_anti_gravity))), shield(new mgl::Key(mgl::Key(_shield))), 
	pause(new mgl::Key(mgl::Key(_pause))), 
	hold_anti_gravity_key(_hold_anti_gravity_key), hold_shield_key(_hold_shield_key) {}

KeyLayout::KeyLayout()
	: move_up(new mgl::Key()), move_down(new mgl::Key()),
	move_left(new mgl::Key()), move_right(new mgl::Key()),
	anti_gravity(new mgl::Key()), shield(new mgl::Key()),
	pause(new mgl::Key()) {}

KeyLayout::~KeyLayout() {
	delete move_up;
	delete move_down;
	delete move_left;
	delete move_right;
	delete anti_gravity;
	delete shield;
	delete pause;
}
