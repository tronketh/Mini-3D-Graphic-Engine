#include "mat4x4.h"

mat4x4 mat4x4::operator*(const mat4x4 rhs) {
	mat4x4 temp;
	for (int i = 0; i < 4; i++) { // row
		for (int j = 0; j < 4; j++) { //column
			temp.m[i][j] = 0;
			for (int k = 0; k < 4; k++) { // number of additions
				temp.m[i][j] += m[i][k] * rhs.m[k][j];
			}
		}
	}
	return temp;
}

mat4x4& mat4x4::operator*=(const mat4x4 rhs) {
	mat4x4 temp;
	for (int i = 0; i < 4; i++) { // row
		for (int j = 0; j < 4; j++) { //column
			temp.m[i][j] = 0;
			for (int k = 0; k < 4; k++) { // number of additions
				temp.m[i][j] += m[i][k] * rhs.m[k][j];
			}
		}
	}
	for (int i = 0; i < 4; i++) { // row
		for (int j = 0; j < 4; j++) { //column
			this->m[i][j] = temp.m[i][j];
		}
	}
	return *this;
}