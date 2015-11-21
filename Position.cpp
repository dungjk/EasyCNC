/*!
 \file      Position.cpp
 \brief     Class to manage Cartesian position
 \details   The class manages the position on a Cartesian plane.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */
#include "Position.h"

Position::Position() {
	for (int i = 0; i < N_AXES; i++) {
		axis[i] = 0;
	}
}

Position::Position(const Position &p) {
	for (int i = 0; i < N_AXES; i++) {
		axis[i] = p.axis[i];
	}
}

float& Position::operator[](const int i) {
	return axis[i];
}

float Position::module() const {
	float val = 0;
	for (int i = 0; i < N_AXES; i++) {
		val += sq(axis[i]);
	}
	return sqrt(val);
}

float Position::module2d(int d1, int d2) const {
	return sqrt(sq(axis[d1]) + sq(axis[d2]));
}

float Position::module3d(int d1, int d2, int d3) const {
	return sqrt(sq(axis[d1]) + sq(axis[d2]) + sq(axis[d3]));
}

float Position::offset(int d, const Position &p) const {
	return p.axis[d] - axis[d];
}

float Position::angle(int d1, int d2) const {
	return atan2(axis[d1], axis[d2]);
}

float Position::angle(int d1, int d2, const Position &p) const {
	return atan2(p.axis[d2] - axis[d2], p.axis[d1] - axis[d1]);
}

Position& Position::polar(int d1, int d2, float m, float a) {
	axis[d1] = m * cos(a);
	axis[d2] = m * sin(a);
	return *this;
}

Position Position::operator+(const Position &p) const {
	Position r;
	for (int i = 0; i < N_AXES; i++) {
		r.axis[i] = p.axis[i] + axis[i];
	}
	return r;
}

Position Position::sum2d(int d1, int d2, const Position &p) const {
	Position r = *this;
	r.axis[d1] += p.axis[d1];
	r.axis[d2] += p.axis[d2];
	return r;
}

Position Position::sum3d(int d1, int d2, int d3, const Position &p) const {
	Position r = *this;
	r.axis[d1] += p.axis[d1];
	r.axis[d2] += p.axis[d2];
	r.axis[d3] += p.axis[d3];
	return r;
}

Position& Position::operator+=(const Position &p) {
	for (int i = 0; i < N_AXES; i++)
		axis[i] += p.axis[i];
	return *this;
}

Position Position::operator-(const Position &p) const {
	Position r;
	for (int i = 0; i < N_AXES; i++) {
		r.axis[i] = axis[i] - p.axis[i];
	}
	return r;
}

Position Position::sub2d(int d1, int d2, const Position &p) const {
	Position r = *this;
	r.axis[d1] -= p.axis[d1];
	r.axis[d2] -= p.axis[d2];
	return r;
}

Position Position::sub3d(int d1, int d2, int d3, const Position &p) const {
	Position r = *this;
	r.axis[d1] -= p.axis[d1];
	r.axis[d2] -= p.axis[d2];
	r.axis[d3] -= p.axis[d3];
	return r;
}

Position& Position::operator-=(const Position &p) {
	for (int i = 0; i < N_AXES; i++)
		axis[i] -= p.axis[i];
	return *this;
}

Position Position::operator*(const float p) {
	Position r;
	for (int i = 0; i < N_AXES; i++)
		r.axis[i] *= p;
	return r;
}

Position& Position::operator*=(const float p) {
	for (int i = 0; i < N_AXES; i++)
		axis[i] *= p;
	return *this;
}

Position Position::operator /(const float p) {
	Position r;
	for (int i = 0; i < N_AXES; i++)
		r.axis[i] /= p;
	return r;
}

Position& Position::operator/=(const float p) {
	for (int i = 0; i < N_AXES; i++)
		axis[i] /= p;
	return *this;
}

//************************************

PositionXY::PositionXY() :
		x(0.0), y(0.0) {
}
;
PositionXY::PositionXY(float px, float py) :
		x(px), y(py) {
}
;
PositionXY::PositionXY(const PositionXY &p) :
		x(p.x), y(p.y) {
}
;

float PositionXY::X() const {
	return x;
}
PositionXY& PositionXY::X(float px) {
	x = px;
	return *this;
}
float PositionXY::Y() const {
	return y;
}
PositionXY& PositionXY::Y(float py) {
	y = py;
	return *this;
}

float PositionXY::module() {
	return sqrt(sq(x) + sq(y));
}

float PositionXY::module(const PositionXY &p) {
	return sqrt(sq(p.x - x) + sq(p.y - y));
}

float PositionXY::offsetX(const PositionXY &p) {
	return p.x - x;
}
float PositionXY::offsetY(const PositionXY &p) {
	return p.y - y;
}

float PositionXY::angle() {
	return atan2(y, x);
}

float PositionXY::angle(const PositionXY &p) {
	return atan2(p.y - y, p.x - x);
}

PositionXY& PositionXY::polar(float m, float a) {
	x = m * cos(a);
	y = m * sin(a);
	return *this;
}

PositionXY PositionXY::operator +(const PositionXY &p) const {
	return PositionXY(x + p.x, y + p.y);
}

PositionXY& PositionXY::operator +=(const PositionXY &p) {
	x += p.x;
	y += p.y;
	return *this;
}

//**************************
//-----PositionXYZ---------
//**************************

PositionXYZ::PositionXYZ() :
		x(0.0), y(0.0), z(0.0) {
}

PositionXYZ::PositionXYZ(float px, float py, float pz) :
		x(px), y(py), z(pz) {
}

PositionXYZ::PositionXYZ(const PositionXYZ &p) :
		x(p.x), y(p.y), z(p.z) {
}

float PositionXYZ::X() const {
	return x;
}

float PositionXYZ::Y() const {
	return y;
}

float PositionXYZ::Z() const {
	return z;
}

PositionXYZ& PositionXYZ::X(float px) {
	x = px;
	return *this;
}

PositionXYZ& PositionXYZ::Y(float py) {
	y = py;
	return *this;
}

PositionXYZ& PositionXYZ::Z(float pz) {
	z = pz;
	return *this;
}

float PositionXYZ::module() const {
	return sqrt(sq(x) + sq(y) + sq(z));
}

float PositionXYZ::moduleXY() const {
	return sqrt(sq(x) + sq(y));
}

float PositionXYZ::module(const PositionXYZ &p) const {
	return sqrt(sq(p.x - x) + sq(p.y - y) + sq(p.z - z));
}

float PositionXYZ::moduleXY(const PositionXYZ &p) const {
	return sqrt(sq(p.x - x) + sq(p.y - y));
}

float PositionXYZ::offsetX(const PositionXYZ &p) {
	return p.x - x;
}

float PositionXYZ::offsetY(const PositionXYZ &p) {
	return p.y - y;
}

float PositionXYZ::offsetZ(const PositionXYZ &p) {
	return p.z - z;
}

float PositionXYZ::angleXY() {
	return atan2(y, x);
}

float PositionXYZ::angleXY(const PositionXYZ &p) {
	return atan2(p.y - y, p.x - x);
}

PositionXYZ& PositionXYZ::polarXY(float m, float a) {
	x = m * cos(a);
	y = m * sin(a);
	return *this;
}

PositionXYZ PositionXYZ::operator +(const PositionXYZ &p) const {
	return PositionXYZ(p.x + x, p.y + y, p.z + z);
}

PositionXYZ& PositionXYZ::operator +=(const PositionXYZ &p) {
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

PositionXYZ PositionXYZ::operator -(const PositionXYZ &p) const {
	return PositionXYZ(x - p.x, y - p.y, z - p.z);
}

PositionXYZ& PositionXYZ::operator -=(const PositionXYZ &p) {
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

PositionXYZ PositionXYZ::operator *(float p) {

	return PositionXYZ(x * p, y * p, z * p);
}

PositionXYZ& PositionXYZ::operator *=(float p) {
	x *= p;
	y *= p;
	z *= p;
	return *this;
}

PositionXYZ PositionXYZ::operator /(float p) {
	return PositionXYZ(x / p, y / p, z / p);
}

PositionXYZ& PositionXYZ::operator /=(float p) {
	x /= p;
	y /= p;
	z /= p;
	return *this;
}
