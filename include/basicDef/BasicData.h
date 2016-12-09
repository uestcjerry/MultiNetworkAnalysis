#ifndef BASICDATA_H
#define BASICDATA_H

#include <string>
#include <vector>

namespace BasicData {
	const unsigned MaxHashLength = 2000;

	const unsigned MaxCrossLinkLength = 1767200;

	const unsigned MaxUserNumber = 1767200;

	const int INFI = 999999;
	const int NIL = 0;

	const std::string TargetAnaResPrefix = "e:\\data_of_weibo\\data_washed\\analysis_result\\";

	const std::string SrcUserRehashedDelRepPrefix = "e:\\data_of_weibo\\data_washed\\user_occurred_rehashed_del_rep\\";

	const std::string SrcEventWithTimePrefix = "e:\\data_of_weibo\\data_washed\\event_max_time\\";		// for analysis

	const std::string SrcEventFilePrefix = "e:\\data_of_weibo\\data_washed\\event_max\\";

	const std::vector<std::string> VecSrcEventFiles = {
		"Nature_Ap_Network_wash",				// 这个是nature的AP聚类数据集，结果是划分为三个社团3 7 20 

		"Foxconn worker falls to death",
		"incident of self-burning at Yancheng, Jangsu",
		"individual income tax threshold rise up to 3500",
		"Chongqing gang trials",
		"Windows Phone release",
		"earthquake of Yunnan Yingjiang",
		"Yao Ming retire",
		"Yushu earthquake",
		"Death of Wang Yue",
		"China Petro chemical Co. Ltd",
		"Qian Yunhui",
		"family violence of Li Yang",
		"the death of Kim Jongil",
		"Xiaomi release",
		"Tang Jun educatioin qualification fake",
		"Wenzhou train collision",
		"case of running fast car in Heibei University",
		"the death of Osama Bin Laden",
		"death of Muammar Gaddafi",
		"Guo Meimei",
		"House prices"
	};

	const std::vector<std::string> VecUserFiles = {
		"user_k_core_src_network"
	};
	const std::vector<std::string> VecEventAggreFiles = {
		"aggregated_event_network"
	};


	const std::vector<std::string> VecSrcUserFiles = {
		"1000",
		"1001",
		"1002",
		"1003",
		"1004",
		"1005",
		"1006",
		"1007",
		"1008"
	};
};


#endif