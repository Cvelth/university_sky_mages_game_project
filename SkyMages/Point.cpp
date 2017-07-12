#include "Point.hpp"
const float POINT_SIZE = 0.05f;

Point Point::operator+(const Point & p) const {
	return Point(m_x + p.m_x, m_y + p.m_y);
}

Point Point::operator+=(const Point & p) {
	m_x += p.m_x;
	m_y += p.m_y;
	return *this;
}

Point Point::operator-=(const Point & p) {
	m_x -= p.m_x;
	m_y -= p.m_y;
	return *this;
}

Point Point::operator-(const Point & p) const {
	return Point(m_x - p.m_x, m_y - p.m_y);
}

Point Point::operator=(const Point & p) {
	return Point(m_x = p.m_x, m_y = p.m_y);
}

Point Point::operator*(float f) const {
	return Point(m_x * f, m_y * f);
}

Point Point::operator/(float f) const {
	return Point(m_x / f, m_y / f);
}

float Point::operator^(Point & p) const {
	return m_x * p.m_x + m_y * p.m_y;
}

Point Point::operator-() const {
	return Point(-m_x, -m_y);
}

Point Point::center(Point a, Point b) {
	return Point((a.m_x + b.m_x) / 2, (a.m_y + b.m_y) / 2);
}

Point Point::normalize() const {
	auto l = length();
	return Point(m_x / l, m_y / l);
}

size_t Point::dimentions() {
	return 3;
}

#include <ctgmath>
bool Point::operator==(const Point& p) const {
	return fabs(m_x - p.m_x) < POINT_SIZE && fabs(m_y - p.m_y) < POINT_SIZE;
}

float Point::length() const {
	return sqrtf(m_x*m_x + m_y*m_y);
}