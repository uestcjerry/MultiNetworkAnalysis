#ifndef APCLUSTERBASICDEFINITION_H
#define APCLUSTERBASICDEFINITION_H

#define ELEM_T double

#include <iostream>
/*
 *	ap cluster, both edge and node need three element: 
 *	similarity (s), responsibility (r) and availability (a)
 */
class ApElement {
public:
	ApElement();
	ApElement(const ELEM_T, const ELEM_T, const ELEM_T);		// s r a
	ApElement(const ApElement &);
	ApElement &operator=(const ApElement &);
	~ApElement() = default;
public:
	friend std::ostream& operator<<(std::ostream &, const ApElement &);

public:
	ELEM_T getS() const;
	void setS(const ELEM_T);

	ELEM_T getR() const;
	void setR(const ELEM_T);

	ELEM_T getA() const;
	void setA(const ELEM_T);

private:
	ELEM_T s;
	ELEM_T r;
	ELEM_T a;
};


#endif