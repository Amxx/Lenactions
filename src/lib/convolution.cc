/*\
* lenactions
* A simple image editing library
* Copyrigth 2013 - Hadrien Croubois
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
\*/

#include "convolution.hh"

#include <cstdio>

using namespace lenactions;


convolution::convolution(float v[9])
{
	norm = 0;
  for (int i = 0; i < 9; ++i)
	{
		if (v[i]>0)
			norm += v[i];
    val[i] = v[i];
	}
}

convolution::convolution(float v[0], float n) : norm(n)
{
  for (int i = 0; i < 9; ++i)
    val[i] = v[i];
}



convolution convolution::Smooth() {
	float filtre[9] = { 1.f, 2.f, 1.f,  2.f,  4.f,  2.f,  1.f,  2.f,  1.f};
	return convolution(filtre);
}
convolution convolution::PrewitzH() {
	float filtre[9] = { -1.f, -1.f, -1.f,  0.f,  0.f,  0.f,  1.f,  1.f,  1.f};
	return convolution(filtre);
}
convolution convolution::PrewitzV() {
	float filtre[9] = { -1.f,  0.f,  1.f, -1.f,  0.f,  1.f, -1.f,  0.f,  1.f};
	return convolution(filtre);
}

convolution convolution::SobelH() {
	float filtre[9] = { -1.f, -2.f, -1.f,  0.f,  0.f,  0.f,  1.f,  2.f,  1.f};
	return convolution(filtre);
}
convolution convolution::SobelV() {
	float filtre[9] = { -1.f,  0.f,  1.f, -2.f,  0.f,  2.f, -1.f,  0.f,  1.f};
	return convolution(filtre);
}

convolution convolution::KirschH() {
	float filtre[9] = { -3.f, -3.f,  5.f, -3.f,  0.f,  5.f, -3.f, -3.f,  5.f};
	return convolution(filtre);
}
convolution convolution::KirschV() {
	float filtre[9] = { -3.f, -3.f, -3.f, -3.f,  0.f, -3.f,  5.f,  5.f,  5.f};
	return convolution(filtre);
}




