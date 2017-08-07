#pragma once
using scalar = float;
struct vector {
	scalar h;
	scalar v;

	vector(scalar h, scalar v) : h(h), v(v) {}
	inline vector& operator+=(vector const& p) {
		h += p.h;
		v += p.v;
		return *this;
	}
	inline vector& operator*=(scalar const m) {
		h *= m;
		v *= m;
		return *this;
	}
	inline vector& operator*=(vector const& m) {
		h *= m.h;
		v *= m.v;
		return *this;
	}
	inline vector operator-() const {
		return vector(-h, -v);
	}
	scalar const abs() const;
	friend vector const operator+(vector const& a, vector const& b);
	friend vector const operator-(vector const& a, vector const& b);
	friend vector const operator*(vector const& a, scalar b);
	friend vector const operator*(vector const& a, vector const& b);
	friend vector const operator/(vector const& a, scalar b);
};