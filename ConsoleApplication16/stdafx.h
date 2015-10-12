// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//
#include <iostream>
#include <queue>
#include <fstream>
#include <ostream>
using namespace std;
const int MAX = 9999999;//频率最大值，之所以不设置为0原因：避免混淆。不好判定分辨是确实为0频还是没有这个元素，其实等价。
const int leaf = 256;   //这里没有完全按完全二叉树去存储而是简化为数组，如为前者，则0频更合适
                        //leaf=256意为ascii码一共也就256种。。
typedef struct HFMAN{   
	long weight;        //HFMAN结点。频率，父，左子，右子，编码，码长
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



class HFTree{    //Huffman树类。函数：构造，析构，统计，建立树，编码，输出，补全，重置，压缩，解压
public:            //           变量：根，叶子类数，HF数组，byte缓冲，位数bitsum，缺位lacknum
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