#include "../include/basicDef/BasicStruct.h"
#include "../include/crossLink/CrossLink.h"
#include "../include/userManage/UserManage.h"
#include "../include/basicDef/BasicData.h"
#include "../include/eventAnalysis/EventAnalysis.h"
#include "../include/preMulti/PreMultiAna.h"
#include "../include/apCluster/ApClusteringProcedure.h"

#include <iostream>

int main()
{
	// event analysis
	
	//if (eventAnalysisRevokeThis() == false)
	//	return false;

	// ap clustering
	ApClusteringProcedure obj;
	if (!obj.apClusteringProcedure()) {
		std::cerr << "== check error. ==" << std::endl;
		getchar();
	}


	/*
	Time oldday(2016, 5,1, 0,0,0), newday(2016, 5, 31,0,0,0);
	std::cout << distanceDayBetTime(newday, oldday) << std::endl;
	getchar();
	*/

	// pre multilayer analysis
	/*
	if (preMultiAnalysisRevokeThis() == false) {
		getchar();
		return false;
	}
	*/
		

	/*
	std::vector<int> table = {1, 4, 66, 12, 12, 2, 3};

	ListNode *Head = new ListNode();
	Head->next = NULL;

	for (const auto &e : table) {
		// Í·²å·¨
		ListNode *p = new ListNode();
		
		p->data = e;
		p->next = Head->next;
		Head->next = p;
	}

	´«µÝ *Head 
	*/

	return 0;
}