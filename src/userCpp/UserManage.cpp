#include "../../include/userManage/UserManage.h"
#include "../../include/basicDef/BasicData.h"

#include <fstream>
#include <iostream>

UserManage::UserManage() { }

UserManage::~UserManage() 
{ 
	capacity = 0; 
}

/*
*	for test
*/
bool UserManage::testUserManage()
{
	std::string srcFile = "E:\\data_of_weibo\\data_washed\\user_occurred_rehashed_del_rep\\test.txt";
	if (false == readFileConsUserManage(srcFile)) {
		std::cerr << "read file cons user manage error." << std::endl;
		return false;
	}

	if (searchUserFollow(1, 1767100) == false)
		std::cout << "could not find.." << std::endl;
	else
		std::cout << "find.." << std::endl;
	getchar();

	displayUserManage();
	
	return true;
}


void UserManage::deconsUserVecObj()
{
	usersVecObj.clear(); capacity = 0;
}
bool UserManage::consUserVecObj()
{
	std::cout << "cons user vector obj begin.." << std::endl;

	usersVecObj = std::vector<User>(BasicData::MaxUserNumber + 1);

	for (unsigned i = 1; i <= BasicData::MaxUserNumber; ++i)
		usersVecObj.at(i).setId(i);
	capacity = BasicData::MaxUserNumber;

	std::cout << "cons user vector obj finish.." << std::endl;
	return true;
}
bool UserManage::readFileConsUserManage(const std::string &file)
{
	std::cout << "read file: \n" << file << std::endl;

	std::fstream inputFile(file, std::ios_base::in);
	if (!inputFile.is_open()) {
		std::cerr << "open file error." << std::endl;
		return false;
	}

	unsigned userOne, userTwo;
	long long line = 0;
	while (inputFile >> userOne >> userTwo) {
		if (++line % 1000000 == 0)
			std::cout << line << std::endl;

		if (addUserFollow(userOne, userTwo) == false)
			std::cerr << "add user follow error." << std::endl;
		if (addUserBeFollow(userOne, userTwo) == false)
			std::cerr << "add user be follow error." << std::endl;
	}
	inputFile.close();

	std::cout << "read file cons finish.. line = " << line << std::endl;
	return true;
}

UserManage::_UserManageLen_t UserManage::getCapacity()
{
	return usersVecObj.size() - 1;
}
const UserManage::_UserManageLen_t UserManage::getCapacity() const
{
	return usersVecObj.size() - 1;
}

bool UserManage::addUser(const _UserManageLen_t u)
{
	try {
		usersVecObj.at(u).setId(u);
	}
	catch (std::out_of_range e) {
		e.what();
		return false;
	}
	return true;
}
bool UserManage::addUserFollow(const _UserManageLen_t a, const _UserManageLen_t b)
{
	try {
		//addUser(a);
		usersVecObj.at(a).addFocusUser(b);
	}
	catch (std::out_of_range e) {
		e.what();
		return false;
	}
	return true;
}
bool UserManage::addUserBeFollow(const _UserManageLen_t a, const _UserManageLen_t b)
{
	try {
		//addUser(b);
		usersVecObj.at(b).addBeFocusUser(a);
	}
	catch (std::out_of_range e){
		e.what();
		return false;
	}
	return true;
}
bool UserManage::searchUserFollow(const _UserManageLen_t a, const _UserManageLen_t b)
{
	if (a > capacity || b > capacity || a <= 0 || b <= 0) {
		std::cerr << "search out of range." << std::endl;
		return false;
	}
		
	if (usersVecObj.at(a).searchFocusUser(b) == false)
		return false;
	else
	    return true;
}

void UserManage::displayUserManage()
{
	std::cout << "display user manage begin.." << std::endl;

	for (unsigned i = 1; i <= capacity; ++i) {
		if (usersVecObj.at(i).getFocusUserVecSize() == 0 && usersVecObj.at(i).getBeFocusUserVecSize() == 0)
			continue;

		std::cout << "user id = " << usersVecObj.at(i).getId() << std::endl;
		
		std::vector<unsigned> focusVec = usersVecObj.at(i).getFocusUserVec();
		if (focusVec.size() == 0)
			std::cout << "no focus user." << std::endl;
		else {
			std::cout << "focus user : ";
			for (const auto &u : focusVec)
				std::cout << u << ", ";
			std::cout << std::endl;
		}

		std::vector<unsigned> befocusVec = usersVecObj.at(i).getBeFocusUserVec();
		if (befocusVec.size() == 0)
			std::cout << "no be-focus user." << std::endl;
		else {
			std::cout << "be-focus user : ";
			for (const auto &u : befocusVec)
				std::cout << u << ", ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << "press any key.." << std::endl;
	getchar();
}