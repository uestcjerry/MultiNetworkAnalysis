#ifndef BASICSTRUCT_H
#define BASICSTRUCT_H

#define _TimeData_t unsigned
#define _NodeTag_t unsigned
#define _NodeWei_t unsigned


#include "BasicData.h"

// ===================================================================== //
class Time {
public:
	friend bool operator<(const Time &, const Time &);
	friend bool operator>(const Time &, const Time &);
	friend bool operator==(const Time &, const Time &);
	friend std::ostream& operator<<(std::ostream &, const Time &);

	friend long long distanceDayBetTime(const Time &, const Time &);		//����ʱ��Ĳ�ֵ(��)

	Time();
	Time(const _TimeData_t, const _TimeData_t, const _TimeData_t, const _TimeData_t, const _TimeData_t, const _TimeData_t);
	Time(const Time &);
	Time& operator=(const Time &);

	bool setAttribute(std::vector<_TimeData_t> &);
	
private:
	_TimeData_t year;
	_TimeData_t month;
	_TimeData_t day;

	_TimeData_t hour;				// string may be "00"
	_TimeData_t minute;
	_TimeData_t second;
};


//==================================================================================//

class BasicEdge {
public:
	friend bool operator<(const BasicEdge &, const BasicEdge &);					// ������from �� to �����бߵıȽ�
	friend std::ostream& operator<<(std::ostream &, const BasicEdge &);

	// ʱ��Ƚϵ���
	friend bool compareTransTimeLess(const BasicEdge &, const BasicEdge &);
	friend bool compareTransTimeMore(const BasicEdge &, const BasicEdge &);
	friend bool compareTransTimeEqua(const BasicEdge &, const BasicEdge &);

	BasicEdge();
	BasicEdge(const _NodeTag_t, const _NodeTag_t, const _NodeWei_t, Time &);
	BasicEdge(const BasicEdge &);
	BasicEdge& operator=(const BasicEdge &);
	~BasicEdge();

	_NodeTag_t getFrom();
	_NodeTag_t getTo();
	_NodeWei_t getWeight();
	const _NodeTag_t getFrom() const;
	const _NodeTag_t getTo() const;
	const _NodeWei_t getWeight() const;
	
	Time getTime() const;
	
private:
	_NodeTag_t from;
	_NodeTag_t to;
	_NodeWei_t weight;
	Time transTime;
};


// =================================================================================//

class BfsNode {
public:
	enum COLOR { WHITE = 1, GRAY = 2, BLACK = 3 };
	typedef COLOR BfsColor;

	BfsNode();
	BfsNode(const BfsNode &);
	BfsNode & operator=(const BfsNode &);
	~BfsNode();

	void useNode();
	bool useJudge() const;

	void setColor(const BfsNode::BfsColor c);
	BfsNode::BfsColor getColor() const;

	void setBranch(const unsigned b);
	unsigned getBranch() const;

private:
	bool used;							//��ʼ��������Ϊfalse��useNode()֮�� true
	BfsColor color;		
	unsigned branch;
};

#endif