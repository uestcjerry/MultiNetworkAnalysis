#include "../../include/preMulti/PreMultiAna.h"
#include <iostream>

/* ================================ PreMultiLayer ==================================== */
PreMultiLayer::PreMultiLayer() { layerId = 0; }
PreMultiLayer::~PreMultiLayer() {}

bool PreMultiLayer::initLayerObj(const std::string &file)
{
	if (layerObj.readFileConsCrosLink(file) == false) {
		std::cerr << "cons crosslink obj error." << std::endl;
		return false;
	}
	unsigned userNum = layerObj.getCapacity();
	nodeActiVec = PreMultiLayer::nodeActiVec_t(userNum + 1);

	return true;
}
void PreMultiLayer::clearLayerObj()
{
	layerObj.deconsCrossLinkObj();
}
void PreMultiLayer::showLayerObj()
{
	layerObj.showCroLink();
}

void PreMultiLayer::setLayerId(const unsigned u) { layerId = u; }

unsigned PreMultiLayer::getLayerId() const { return layerId; }

unsigned PreMultiLayer::getLayerCapacity() const { return layerObj.getCapacity(); }

/*
 *	node activity vector init
 */
bool PreMultiLayer::initNodeActiVec()
{
	unsigned userNum = layerObj.getCapacity();
	if (userNum == 0)
		return false; 

	// node activity vector数组的长度根据这一层的最大节点号为准
	nodeActiVec = PreMultiLayer::nodeActiVec_t(userNum + 1);	

	// 更新node activity数组，即向量 bi
	for (unsigned i = 1; i <= userNum; ++i) {
		unsigned ajaHoriSize = 0, ajaVertSize = 0;
		if (layerObj.getAjaSizeFromHori(i, ajaHoriSize) == false) {
			std::cerr << "get aja size from hori error." << std::endl;
			return false;
		}
		if (ajaHoriSize > 0) {
			nodeActiVec.at(i) = 1;
			continue;
		}
		if (layerObj.getAjaSizeFromVert(i, ajaVertSize) == false) {
			std::cerr << "get aja size from vert error." << std::endl;
			return false;
		}
		if (ajaVertSize > 0)
			nodeActiVec.at(i) = 1;
		//nodeActiVec.at(i) = (ajaHoriSize + ajaVertSize > 0) ? 1 : 0;
	}
	return true;
}
bool PreMultiLayer::getNodeActiVecAt(const unsigned i, unsigned &u)
{
	if (i > layerObj.getCapacity() || i == 0)
		return false;
	u = nodeActiVec.at(i);
	return true;
}

/* ================================= PreMultiLayerManage ============================= */

PreMultiLayerManage::PreMultiLayerManage() { multiLayerNum = 0; }

PreMultiLayerManage::~PreMultiLayerManage() {}

void PreMultiLayerManage::setMultiNum(const unsigned u) { multiLayerNum = u; }

const unsigned PreMultiLayerManage::getMultiNum() const { return multiLayerNum; }

bool PreMultiLayerManage::initMultiLayerObj(const std::string &filePre, const std::vector<std::string> &allFiles)
{
	multiLayerNum = allFiles.size();
	multiLayerObj = PreMultiLayerManage::PreMultiLayerManage_t(multiLayerNum);

	for (unsigned i = 0; i < multiLayerNum; ++i) {
		//std::cout << "init multi layer obj at : " << i << std::endl;

		if (i == multiLayerNum - 1) {
			std::cout << "i = " << i << std::endl;
			//getchar();
		}
		
		if (multiLayerObj.at(i).initLayerObj(filePre + allFiles.at(i)) == false) {
			std::cerr << "init layer obj at : " << i << " error." << std::endl;
			return false;
		}
		multiLayerObj.at(i).setLayerId(i);
		//multiLayerObj.at(i).showLayerObj();
	}
	return true;
}
void PreMultiLayerManage::clearMultiLayerObj()
{
	for (unsigned i = 0; i < multiLayerNum; ++i)
		multiLayerObj.at(i).clearLayerObj();
}
void PreMultiLayerManage::showMultiLayerObj()
{
	for (unsigned i = 0; i < multiLayerNum; ++i) {
		std::cerr << "Layer : " << i << std::endl;
		multiLayerObj.at(i).showLayerObj();
	}
}



/*	=================  pre multi analysis revoke function ================= */
bool preMultiAnalysisRevokeThis()
{
	
	PreMultiLayerManage preMultiObj;

	/*
	//part one: 计算所有点的节点活跃度 <nodeid, Bi> ，存文件 nodeActi
	//init multiLayerNum && multiLayerObj 
	if (preMultiObj.initMultiLayerObj(BasicData::SrcEventWithTimePrefix, BasicData::VecSrcEventFiles) == false)
		return false;

	//preMultiObj.showMultiLayerObj();

	if (preMultiObj.calNodeActiDisOfBi(BasicData::TargetAnaResPrefix) == false) {
		getchar();
		std::cerr << "cal node acti dis of Bi error." << std::endl;
		return false;
	}

	// clearMulti()
	preMultiObj.clearMultiLayerObj();
	*/

	/*
	//part two: analysis the result
	std::string srcFile = "e:\\data_of_weibo\\data_washed\\analysis_result\\nodeActi";		//src file: <nodeId, Bi>
	std::string tarPref = "e:\\data_of_weibo\\data_washed\\analysis_result\\";				//tar file: <Bi, log(Bi)>

	for (double exp = 1.8; exp < 2.5; exp += 0.1) {
		//一个exp指数，画一张图 等下拿它的数据验证下程序 修改下读取文件的src readdir()

		std::cout << "handle the result, exp = " << exp << std::endl;
		if (preMultiObj.handleTheResultFromNodeActi(srcFile, tarPref, exp) == false) {
			std::cerr << "handle the result from node acti error." << std::endl;
			getchar();
			return false;
		}
	}
	*/
	return true;
}