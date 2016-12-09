#ifndef APCLUSTERINGPROCEDURE_H
#define APCLUSTERINGPROCEDURE_H

#include "../crossLink/CrossLink.h"
#include "../apCluster/ApClusterStorageDefinition.h"

/*
*	Ap-Clustering
*/
class ApClusteringProcedure {
public:
	typedef std::vector<bool>					NODETAG_T;

	ApClusteringProcedure() = default;
	~ApClusteringProcedure() = default;

public:
	bool buildNetwork(const std::string &);		//�������ļ�����·����
	void clearNetwork();

	
public:
	// ap ������ú���
	bool apClusteringProcedure();				// total function

public:
	// ָ�����������ļ�������ʼap���࣬�����Ǵ�ʱ��������ݣ�����ֻ����ϴ���ͨ����
	bool apClusteringFromFile(const std::string &, const double, const double);
	// ��ǵ�ǰnetwork�е����нڵ�
	bool tagAllNetworkNodes(NODETAG_T &, unsigned &);
	// ѡȡ��start��ʼ����ͨ����
	bool getComponentNodesFromNetwork(NODETAG_T &, std::vector<unsigned> &, const unsigned);

	// ���ھֲ���Ϣ��������ָ�깹�죬������µ��ǹ̶����ƶ�s����
	// ע�⣺���ڹ̶����ƶȶ��ԣ�s_x_y �� s_y_x ��һ���ġ���r_x_y �Լ� a_x_y �������ƣ���Ϊ�߼���ͬ��
	bool getCnIndex(std::vector<unsigned> &, ApEdgeStorage &, const double);
	bool getAaIndex(std::vector<unsigned> &, ApEdgeStorage &, const double);
	bool getRaIndex(std::vector<unsigned> &, ApEdgeStorage &, const double);

	// ���� r
	bool updateResponsibility(std::vector<unsigned> &, ApEdgeStorage &, const double);

	// ���� a
	bool updateAvailability(std::vector<unsigned> &, ApEdgeStorage &, const double);
	
	// �ҳ����������Ƽ����ĵ�
	bool getRecommendNode(ApEdgeStorage &, std::vector<unsigned> &, std::vector<unsigned> &);


public:
	// ���Ժ���
	void showNetwork();
	bool readSimilarityFromFile(const std::string &, ApEdgeStorage &, const double);


private:
	CrossLink network;							// ����ȡ�ϴ����ͨ�������з���
};

#endif