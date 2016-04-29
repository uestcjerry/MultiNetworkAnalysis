#ifndef USERMANAGE_H
#define USERMANAGE_H

#include "User.h"

class UserManage {
public:
	typedef unsigned _UserManageLen_t;		// 1767200 max user id

	UserManage();
	~UserManage();
	
	bool testUserManage();		// test finish..

	void deconsUserVecObj();
	bool consUserVecObj();									//renew capacity , after decons, call readFilecons() is ok
	bool readFileConsUserManage(const std::string &);		//revoke cons() & this() for init


	_UserManageLen_t getCapacity();
	const _UserManageLen_t getCapacity() const;

	bool addUser(const _UserManageLen_t);					// set id while cons() , do not need this anymore

	bool addUserFollow(const _UserManageLen_t a, const _UserManageLen_t b);			// a->b : a follow b
	bool addUserBeFollow(const _UserManageLen_t a, const _UserManageLen_t b);		// a->b : b is be-followed by a
	
	bool searchUserFollow(const _UserManageLen_t, const _UserManageLen_t);

	void displayUserManage();
	
private:
	UserManage(const UserManage &) = delete;
	UserManage& operator=(const UserManage &) = delete;
private:
	std::vector<User> usersVecObj;
	_UserManageLen_t capacity;
};


#endif