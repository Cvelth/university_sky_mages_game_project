#pragma once
class Point {
private:
	float m_x, m_y;

public:
	Point(float x = 0.f, float y = 0.f) : m_x(x), m_y(y) {};
	float x() const { return m_x; }
	float y() const { return m_y; }
	void x(float x) { m_x = x; }
	void y(float y) { m_y = y; }

	bool operator==(const Point& p) const;
	inline bool operator!=(const Point& p) const {
		return !operator==(p);
	}

	Point operator+(const Point& p) const;
	Point operator+=(const Point& p);
	Point operator-(const Point& p) const;
	Point operator-=(const Point& p);
	Point operator=(const Point& p);

	Point operator*(float f) const;
	Point operator/(float f) const;

	float operator^(Point& p) const;

	Point operator-() const;

	static Point center(Point, Point);
	float length() const;
	Point normalize() const;

	static size_t dimentions();
};