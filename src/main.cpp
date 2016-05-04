#include "../include/basicDef/BasicStruct.h"
#include "../include/crossLink/CrossLink.h"
#include "../include/userManage/UserManage.h"
#include "../include/basicDef/BasicData.h"
#include "../include/eventAnalysis/EventAnalysis.h"
#include "../include/preMulti/PreMultiAna.h"


int main()
{
	// event analysis
	//if (eventAnalysisRevokeThis() == false)
	//	return false;


	// pre multilayer analysis
	if (preMultiAnalysisRevokeThis() == false) {
		getchar();
		return false;
	}
		

	return 0;
}