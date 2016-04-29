#ifndef USER_H
#define USER_H

#include <set>
#include <vector>

class User {
public:
	typedef unsigned _UserTag_t;

	friend std::ostream& operator<<(std::ostream &, const User &);

	User();
	User(const _UserTag_t);
	User(const User &);
	User& operator=(const User &);
	~User();

	_UserTag_t getId();
	const _UserTag_t getId() const;
	void setId(const _UserTag_t);
	
	void addFocusUser(const _UserTag_t);
	void addBeFocusUser(const _UserTag_t);

	size_t getFocusUserVecSize();
	const size_t getFocusUserVecSize() const;
	size_t getBeFocusUserVecSize();
	const size_t getBeFocusUserVecSize() const;

	bool searchFocusUser(const _UserTag_t);

	std::vector<_UserTag_t> getFocusUserVec();
	std::vector<_UserTag_t> getFocusUserVec() const;
	std::vector<_UserTag_t> getBeFocusUserVec();
	std::vector<_UserTag_t> getBeFocusUserVec() const;

private:
	_UserTag_t id;
	std::set<_UserTag_t> focusUserSet;				// A->B for find
	std::vector<_UserTag_t> focusUserVec;			// for loop
	std::vector<_UserTag_t> beFocusUserVec;
};


#endif