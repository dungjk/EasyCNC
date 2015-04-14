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

float PositionXYZ::module() {
	return sqrt(sq(x) + sq(y) + sq(z));
}

float PositionXYZ::moduleXY() {
	return sqrt(sq(x) + sq(y));
}

float PositionXYZ::module(const PositionXYZ &p) {
	return sqrt(sq(p.x - x) + sq(p.y - y) + sq(p.z - z));
}

float PositionXYZ::moduleXY(const PositionXYZ &p) {
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

PositionXYZ PositionXYZ::operator +(const PositionXYZ &p) const { return PositionXYZ(p.x +x, p.y + y, p.z + z); }

PositionXYZ& PositionXYZ::operator +=(const PositionXYZ &p) {
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

PositionXYZ PositionXYZ::operator -(const PositionXYZ &p) const { return PositionXYZ(x - p.x, y - p.y, z - p.z); }

PositionXYZ& PositionXYZ::operator -=(const PositionXYZ &p) {
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

PositionXYZ PositionXYZ::operator *(float p){

	return PositionXYZ(x * p, y * p, z * p);
}

PositionXYZ& PositionXYZ::operator *=(float p){
	x *= p;
	y *= p;
	z *= p;
	return *this;
}

PositionXYZ PositionXYZ::operator /(float p){
	return PositionXYZ(x / p, y / p, z / p);
}

PositionXYZ& PositionXYZ::operator /=(float p){
	x /= p;
	y /= p;
	z /= p;
	return *this;
}
