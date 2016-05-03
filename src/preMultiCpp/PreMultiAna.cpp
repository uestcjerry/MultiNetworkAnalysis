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
		std::cout << "init multi layer obj at : " << i << std::endl;
		if (multiLayerObj.at(i).initLayerObj(filePre + allFiles.at(i)) == false) {
			std::cerr << "init layer obj at : " << i << " error." << std::endl;
			return false;
		}
		multiLayerObj.at(i).showLayerObj();
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
	
	std::string filePrefix = "E:\\data_of_weibo\\multi_test\\";
	std::vector<std::string> multiFiles = { 
		"Foxconn worker falls to death",
		"individual income tax threshold rise up to 3500",
		"incident of self-burning at Yancheng, Jangsu",
		"Chongqing gang trials",
		"Windows Phone release"
	};

	if (preMultiObj.initMultiLayerObj(filePrefix, multiFiles) == false)
		return false;

	preMultiObj.showMultiLayerObj();


	// analysis..

	// clearMulti()

	return true;
}