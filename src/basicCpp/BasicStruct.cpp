#include "../../include/basicDef/BasicStruct.h"



// ============================================== Time ================================================== //

bool operator<(const Time &l, const Time &r)
{
	if (l.year != r.year)
		return l.year < r.year;
	else if (l.month != r.month)
		return l.month < r.month;
	else if (l.day != r.day)
		return l.day < r.day;
	else
		return false;
	/*
	else if (l.hour != r.hour)
		return l.hour < r.hour;
	else if (l.minute != r.minute)
		return l.minute < r.minute;
	else if (l.second != r.second)
		return l.second < r.second;
	else
		return false;
	*/
}
bool operator>(const Time &l, const Time &r)
{
	if (l.year != r.year)
		return l.year > r.year;
	else if (l.month != r.month)
		return l.month > r.month;
	else if (l.day != r.day)
		return l.day > r.day;
	else
		return false;
}
bool operator==(const Time &l, const Time &r)
{
	//  && l.hour == r.hour && l.minute == r.minute && l.second == r.second
	if (l.year == r.year && l.month == r.month && l.day == r.day)
		return true;
	else
		return false;
}
std::ostream& operator<<(std::ostream &os, const Time &u)
{
	os << u.year << " " << u.month << " " << u.day << " " << u.hour << " " << u.minute << " " << u.second;
	return os;
}
/*
 *	计算时间之间的差值（天）
 */
long long distanceDayBetTime(const Time &l, const Time &r)
{
	if (l > r)
		return distanceDayBetTime(r, l);

	long long nd, nm, ny;
	long long od, om, oy;

	nm = (r.month + 9) % 12;
	ny = r.year - nm / 10;
	nd = 365 * ny + ny / 4 - ny / 100 + ny / 400 + (nm * 306 + 5) / 10 + (r.day - 1);

	om = (l.month + 9) % 12;
	oy = l.year - om / 10;
	od = 365 * oy + oy / 4 - oy / 100 + oy / 400 + (om * 306 + 5) / 10 + (l.day - 1);

	return nd - od;
}


Time::Time() : year(0), month(0), day(0), hour(0), minute(0), second(0)
{}

Time::Time(const _TimeData_t a, const _TimeData_t b, const _TimeData_t c, const _TimeData_t d, const _TimeData_t e, const _TimeData_t f)
	: year(a), month(b), day(c), hour(d), minute(e), second(f)
{}

Time::Time(const Time &u) : year(u.year), month(u.month), day(u.day), hour(u.hour), minute(u.minute), second(u.second)
{}

Time& Time::operator=(const Time &u)
{
	if (this == &u)
		return *this;
	year = u.year, month = u.month, day = u.day, hour = u.hour, minute = u.minute, second = u.second;
	return *this;
}

bool Time::setAttribute(std::vector<_TimeData_t> &vec)
{
	if (vec.size() != 6)
		return false;
	year = vec[0], month = vec[1], day = vec[2], hour = vec[3], minute = vec[4], second = vec[5];
	return true;
}


// ========================================= BasicEdge ========================================= //

bool operator<(const BasicEdge &l, const BasicEdge &r)
{
	if (l.from != r.from) return l.from < r.from;
	else if (l.to != r.to) return l.to < r.to;
	else return false;
}
std::ostream& operator<<(std::ostream &os, const BasicEdge &u)
{
	os << u.from << " " << u.to; 
	return os;
}
bool compareTransTimeLess(const BasicEdge &l , const BasicEdge &r)
{
	return l.transTime < r.transTime;
}
bool compareTransTimeMore(const BasicEdge &l, const BasicEdge &r)
{
	return l.transTime > r.transTime;
}
bool compareTransTimeEqua(const BasicEdge &l, const BasicEdge &r)
{
	return l.transTime == r.transTime;
}

BasicEdge::BasicEdge() {}

BasicEdge::BasicEdge(const _NodeTag_t a, const _NodeTag_t b, const _NodeWei_t c, Time &d) : from(a), to(b), weight(c), transTime(d) {}

BasicEdge::BasicEdge(const BasicEdge &u) { from = u.from, to = u.to, weight = u.weight, transTime = u.transTime; }

BasicEdge& BasicEdge::operator=(const BasicEdge &u) { 
	if (this == &u)
		return *this;
	from = u.from, to = u.to, weight = u.weight, transTime = u.transTime; 
	return *this; 
}

BasicEdge::~BasicEdge() {}

_NodeTag_t BasicEdge::getFrom() { return from; }

_NodeTag_t BasicEdge::getTo() { return to; }

_NodeWei_t BasicEdge::getWeight() { return weight; }

const _NodeTag_t BasicEdge::getFrom() const { return from; }

const _NodeTag_t BasicEdge::getTo() const { return to; }

const _NodeWei_t BasicEdge::getWeight() const { return weight; }

Time BasicEdge::getTime() const
{
	return transTime;
}


// ================================================== BfsNode =============================== //

BfsNode::BfsNode() : used(false), color(BfsNode::BfsColor::WHITE), branch(0) {}

BfsNode::BfsNode(const BfsNode &u) : used(u.used), color(u.color), branch(u.branch) {}

BfsNode & BfsNode::operator = (const BfsNode &u)
{
	if (this == &u) return *this;
	used = u.used, color = u.color, branch = u.branch;
	return *this;
}

BfsNode::~BfsNode() {}

void BfsNode::useNode() { used = true; }

bool BfsNode::useJudge() const { return used; }

void BfsNode::setColor(const BfsNode::BfsColor c) { color = c; }

BfsNode::BfsColor BfsNode::getColor() const { return color; }

void BfsNode::setBranch(const unsigned b) { branch = b; }

unsigned BfsNode::getBranch() const { return branch; }


// ============================================== EdgePairDis =================================== //
/*
bool operator==(const EdgePairDis &l, const EdgePairDis &r)
{
	if (l.from == r.from && l.to == r.to) 
		return true;
	else 
		return false;
}
bool operator<(const EdgePairDis &l, const EdgePairDis &r)
{
	if (l.from != r.from)
		return l.from < r.from;
	else if (l.to != r.to)
		return l.to < r.to;
	else
		return false;
}

EdgePairDis::EdgePairDis() : from(0), to(0), distance(0)
{}
EdgePairDis::EdgePairDis(const unsigned a, const unsigned b, const unsigned dis = 0) : from(a), to(b), distance(dis)
{}
EdgePairDis::EdgePairDis(const EdgePairDis &u) : from(u.from), to(u.to), distance(u.distance)
{}
EdgePairDis& EdgePairDis::operator=(const EdgePairDis &u)
{
	if (this == &u)
		return *this;
	from = u.from, to = u.to, distance = u.distance;

	return *this;
}
EdgePairDis::~EdgePairDis() {}

void EdgePairDis::setFrom(const unsigned u) { from = u; }

const unsigned EdgePairDis::getFrom() const { return from; }

void EdgePairDis::setTo(const unsigned u) { to = u; }

const unsigned EdgePairDis::getTo() const { return to; }

void EdgePairDis::setDis(const unsigned u) { distance = u; }

const unsigned EdgePairDis::getDis() const { return distance; }
*/