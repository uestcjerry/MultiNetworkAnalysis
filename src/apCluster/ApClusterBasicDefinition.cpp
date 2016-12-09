#include "../../include/apCluster/ApClusterBasicDefinition.h"

ApElement::ApElement() 
{
	s = 0.0; r = 0.0; a = 0.0;
}

ApElement::ApElement(const ELEM_T x, const ELEM_T y, const ELEM_T z)
{
	s = x; r = y; a = z;
}
ApElement::ApElement(const ApElement &elem)
{
	s = elem.s; r = elem.r; a = elem.a;
}
ApElement &ApElement::operator=(const ApElement &elem)
{
	if (this == &elem) return *this;
	s = elem.s; r = elem.r; a = elem.a;
	return *this;
}

//ApElement::~ApElement()
//{ }

std::ostream & operator<<(std::ostream &os, const ApElement &obj)
{
	os << "s = " << obj.s << "\tr = " << obj.r << "\ta = " << obj.a;
	return os;
}


ELEM_T ApElement::getS() const
{
	return s;
}
void ApElement::setS(const ELEM_T elem)
{
	s = elem;
}
ELEM_T ApElement::getR() const
{
	return r;
}
void ApElement::setR(const ELEM_T elem)
{
	r = elem;
}
ELEM_T ApElement::getA() const
{
	return a;
}
void ApElement::setA(const ELEM_T elem)
{
	a = elem;
}