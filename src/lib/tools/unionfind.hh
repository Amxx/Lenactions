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

#ifndef UNIONFIND_HH
#define UNIONFIND_HH

namespace lenactions {

	class Unionfind
	{
		public:
			Unionfind(int f = 0);
			Unionfind&	root();
			int&				flag();
			void				join(Unionfind& u);
		private:
			Unionfind*	p;
			int 				d;
			int 				f;
	};
}


#endif