#include "unionfind.hh"


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