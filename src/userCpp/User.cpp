#include "../../include/userManage/User.h"
#include <string>

std::ostream& operator<<(std::ostream &os, const User &u)
{
	os << "user id = " << u.id << "\n";
	os << "focus: ";
	for (const auto &i : u.focusUserVec) os << i << " ";
	os << "\n";
	
	os << "be-focus: ";
	for (const auto &j : u.beFocusUserVec) os << j << " ";
	os << "\n";
	return os;
}

User::User() : id(0) 
{
	/*
	focusUserVec = std::vector<_UserTag_t>();
	focusUserSet = std::set<_UserTag_t>();
	beFocusUserVec = std::vector<_UserTag_t>();
	*/
}

User::User(const _UserTag_t i) : id(i)
{
	/*
	focusUserVec = std::vector<_UserTag_t>();
	focusUserSet = std::set<_UserTag_t>();
	beFocusUserVec = std::vector<_UserTag_t>();
	*/
}

User::User(const User &u)
{
	id = u.id;
	focusUserSet = u.focusUserSet;
	focusUserVec = u.focusUserVec;
	beFocusUserVec = u.beFocusUserVec;
}
User& User::operator=(const User &u)
{
	if (this == &u)
		return *this;
	id = u.id;
	focusUserSet = u.focusUserSet;
	focusUserVec = u.focusUserVec;
	beFocusUserVec = u.beFocusUserVec;
	return *this;
}

User::~User() {}

User::_UserTag_t User::getId() { return id; }

const User::_UserTag_t User::getId() const { return id; }

void User::setId(const User::_UserTag_t u) { id = u; }

void User::addFocusUser(const _UserTag_t u) 
{
	focusUserSet.insert(u);
	focusUserVec.push_back(u);
}

void User::addBeFocusUser(const _UserTag_t u)
{
	beFocusUserVec.push_back(u);
}


size_t User::getFocusUserVecSize()
{
	return focusUserVec.size();
}
const size_t User::getFocusUserVecSize() const
{
	return focusUserVec.size();
}
size_t User::getBeFocusUserVecSize()
{
	return beFocusUserVec.size();
}
const size_t User::getBeFocusUserVecSize() const
{
	return beFocusUserVec.size();
}

bool User::searchFocusUser(const _UserTag_t target)
{
	if (focusUserSet.find(target) == focusUserSet.end())
		return false;
	else
		return true;
}

std::vector<User::_UserTag_t> User::getFocusUserVec()
{
	return focusUserVec;
}
std::vector<User::_UserTag_t> User::getFocusUserVec() const
{
	return focusUserVec;
}
std::vector<User::_UserTag_t> User::getBeFocusUserVec()
{
	return beFocusUserVec;
}
std::vector<User::_UserTag_t> User::getBeFocusUserVec() const
{
	return beFocusUserVec;
}
