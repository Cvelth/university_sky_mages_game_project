#include "KeyLayout.hpp"
#include <iostream>
#include "..\MyGraphicsLibrary\Events\EventEnums.hpp"

std::ostream & operator<<(std::ostream &s, mgl::Key *k) {
	s << size_t(*k);
	return s;
}

std::istream & operator>>(std::istream &s, mgl::Key *k) {
	size_t t;
	s >> t;
	*k = mgl::Key(t);
	return s;
}

std::ostream& operator<<(std::ostream &s, KeyLayout &k) {
	s << k.move_up << " " << k.move_down << " " << k.move_left << " " << k.move_right << " " << k.anti_gravity;
	return s;
}

std::istream & operator>>(std::istream & s, KeyLayout & k) {
	s >> k.move_up >> k.move_down >> k.move_left >>  k.move_right >> k.anti_gravity;
	return s;
}

KeyLayout::KeyLayout(const KeyLayout & other) {
	move_up = new mgl::Key(*other.move_up);
	move_down = new mgl::Key(*other.move_down);
	move_left = new mgl::Key(*other.move_left);
	move_right = new mgl::Key(*other.move_right);
	anti_gravity = new mgl::Key(*other.anti_gravity);
 }

KeyLayout::KeyLayout(const size_t up, const size_t down, const size_t left, const size_t right, const size_t antiGravity) {
	move_up = new mgl::Key(mgl::Key(up));
	move_down = new mgl::Key(mgl::Key(down));
	move_left = new mgl::Key(mgl::Key(left));
	move_right = new mgl::Key(mgl::Key(right));
	anti_gravity = new mgl::Key(mgl::Key(antiGravity));
}

KeyLayout::KeyLayout() {
	move_up = new mgl::Key;
	move_down = new mgl::Key;
	move_left = new mgl::Key;
	move_right = new mgl::Key;
	anti_gravity = new mgl::Key;
}

KeyLayout::~KeyLayout() {
	delete move_up;
	delete move_down;
	delete move_left;
	delete move_right;
	delete anti_gravity;
}
