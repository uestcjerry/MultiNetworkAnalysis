#ifndef RANKBI_H
#define RANKBI_H

#include <vector>

/*
 *	����ÿ���ڵ㣬��Ҫ���� bi �����Ϣ
 *	Rankbi ����ĸ������ڽڵ����
 */
class Rankbi {
public:
	Rankbi();
	Rankbi(const unsigned);
	Rankbi(const Rankbi &);
	Rankbi & operator=(const Rankbi &);
	~Rankbi();

	void setNodeId(const unsigned);
	const unsigned getNodeId() const;
	
	void setbi(const std::vector<unsigned> &);
	void setMultilayerNum(const unsigned);
	const unsigned getMultilayerNum() const;

	void setRankbi(const unsigned);
	const unsigned getRankbi() const;

	void setBi(const unsigned);
	const unsigned getBi() const;
public:
	bool calRankbiAndBiOfThisNode();	//ӵ��ǰ��������֮�󣬼������������

private:
	unsigned nodeId;

	std::vector<unsigned> bi;			//�������ĳ���Ӧ�úͶ������Ĳ���һ��  capacity
	unsigned multilayNum;				//���ȵ����ܶ������Ĳ�����ע����ȷ��ʼ��

	unsigned rankbi;					//���ݲ�����һ���� 2^M - 1 ��������ϣ�ÿ����� bi �����ӵ�λ����λ��Ӧһ��������ϣ����ƽ���������Ϊrankֵ
	unsigned Bi;						//bi�����з���ĸ�������ʾ�ڵ��Ծ��
		
};


#endif