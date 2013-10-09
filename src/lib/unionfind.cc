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

#include "unionfind.hh"

using namespace lenactions;

Unionfind::Unionfind(int f) :
	p(this),
	d(0),
	f(f)
{
}

Unionfind& Unionfind::root()
{
	if (p != this)
	{
		p = p->root().p;
		d = p->d+1;
	}
	return *p;
}

int& Unionfind::flag()
{
	return f;
}

void Unionfind::join(Unionfind& u)
{
	Unionfind	*r	= root().p;
	Unionfind	*ur	= u.root().p;
	
	if (r->f)
		ur->p = r;
	else if (ur->f)
		r->p = ur;
	else if (d < u.d)
		r->p = ur;
	else
		ur->p = r;
}