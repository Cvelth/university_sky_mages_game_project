#include "ObjectState.hpp"
vector const operator+(vector const& a, vector const& b) {
	return vector(a.h + b.h, a.v + b.v);
}
vector const operator-(vector const& a, vector const& b) {
	return vector(a.h - b.h, a.v - b.v);
}
vector const operator*(vector const& a, scalar b) {
	return vector(a.h * b, a.v * b);
}

vector const operator*(vector const& a, vector const& b) {
	return vector(a.h * b.h, a.v * b.v);
}
