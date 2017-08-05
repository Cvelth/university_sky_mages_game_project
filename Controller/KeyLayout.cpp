#include "KeyLayout.hpp"
#include <iostream>

std::ostream & operator<<(std::ostream &s, mgl::Key& k) {
	s << size_t(k);
	return s;
}

std::istream & operator>>(std::istream &s, mgl::Key &k) {
	size_t t;
	s >> t;
	k = mgl::Key(t);
	return s;
}

std::ostream& operator<<(std::ostream &s, KeyLayout &k) {
	s << k.move_up << " " << k.move_down << " " << k.move_left << " " << k.move_right;
	return s;
}

std::istream & operator>>(std::istream & s, KeyLayout & k) {
	s >> k.move_up >> k.move_down >> k.move_left >>  k.move_right;
	return s;
}
