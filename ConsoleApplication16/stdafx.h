// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//
#include <iostream>
#include <queue>
#include <fstream>
#include <ostream>
using namespace std;
const int MAX = 9999999;//Ƶ�����ֵ��֮���Բ�����Ϊ0ԭ�򣺱�������������ж��ֱ���ȷʵΪ0Ƶ����û�����Ԫ�أ���ʵ�ȼۡ�
const int leaf = 256;   //����û����ȫ����ȫ������ȥ�洢���Ǽ�Ϊ���飬��Ϊǰ�ߣ���0Ƶ������
                        //leaf=256��Ϊascii��һ��Ҳ��256�֡���
typedef struct HFMAN{   
	long weight;        //HFMAN��㡣Ƶ�ʣ��������ӣ����ӣ����룬�볤
	int parent;
	int lchild;
	int rchild;
	int *code;
	int codelength;

	HFMAN(){
		weight = MAX;
		parent = -1;
		lchild = -1;
		rchild = -1;
		codelength = 0;
	}
}HFNODE;



class HFTree{    //Huffman���ࡣ���������죬������ͳ�ƣ������������룬�������ȫ�����ã�ѹ������ѹ
public:            //           ����������Ҷ��������HF���飬byte���壬λ��bitsum��ȱλlacknum
	HFTree();
	virtual ~HFTree();

	bool count(char *input);
	void creat();
	void code();
	void printcode();

	void  addbit(int bit);
	void resetbyte();
	bool compress(char *input, char *output);
	bool icompress(char *input, char *output);


private:
	int root;
	int leafnum;
	HFMAN HF[2 * leaf - 1];
	char byte;
	int bitsum;
	int lacknum;

};