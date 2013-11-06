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

#ifndef PRIORITY_HH
#define PRIORITY_HH

#include <cassert>
#include <vector>

template<typename Key, typename Value>
class priority : private std::vector<std::pair<Key, Value>>
{
	public:
	
		typedef Key																						key_type;
		typedef Value																					value_type;
		typedef std::vector<std::pair<key_type, value_type>>	container_type;
		typedef typename container_type::size_type						size_type;
		typedef typename container_type::iterator							iterator;
		typedef typename container_type::const_iterator				const_iterator;
	
	public:
			
		iterator					begin			()							{ return container_type::begin(); 				}
		const_iterator		begin			()		const			{ return container_type::begin(); 				}
		iterator					end				()							{ return container_type::end();						}
		const_iterator		end				()		const			{ return container_type::end();						}
		
		void							clear			()							{ container_type::clear();								}
		bool							empty			()	 	const			{ return container_type::empty();					}
		size_type					size			()		const			{ return container_type::size();					}
		
		value_type&				top				()							{ return container_type::front().second;	}
		const value_type&	top				()		const			{ return container_type::front().second;	}
		key_type&					top_key		()							{ return container_type::front().first;		}
		const key_type&		top_key		()		const			{ return container_type::front().first;		}
		
		void							push			(Key key, Value value)
		{
			int s = size();
			container_type::push_back(std::make_pair(key, value));
			heapUp(s);
		}

		value_type				pop				()
		{
			value_type result = container_type::front().second;
			swap(0, size()-1);
			container_type::pop_back();
			heapDown(0);
			return result;
		}
		
		
	private:
		void							swap			(int i, int j)
		{
			std::swap(container_type::at(i), container_type::at(j));
		}
		void							heapUp		(int idx)
		{
			int i = idx;
			while ( i>0 && container_type::at(i).first < container_type::at((i-1)/2).first )
			{	
				swap(i, (i-1)/2);
				i = (i-1)/2;
			}
		}
		void							heapDown	(int idx)
		{
			int i = idx, l = size()-1;
			while (2*i+1 <= l)
			{
				int j = 2*i+1;
				if (j < l && (container_type::at(j+1).first < container_type::at(j).first))
					j++;
				if (container_type::at(i).first < container_type::at(j).first)
					return;		
				swap(i, j);
				i = j;
			}
		}
	
};

#endif