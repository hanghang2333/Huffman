//#include "HFTree.h"
#include "stdafx.h"
#include <iostream>
using namespace std;

HFTree::HFTree()//--------构造函数
{
	root = 0;
	leafnum = 0;
	byte = 0;
	bitsum = 0;
	lacknum = 0;
}
HFTree::~HFTree()//--------析构函数，主要是类实例中有指针，需要特别delete，delete并非删除，而是类似于将之置为null
{
	for (int i = 0; i < leaf; i++)
	    {   if (HF[i].codelength != 0){
			delete HF[i].code;    }
	    }
}

bool HFTree::count(char *input)//-----------统计函数     //输入：文件 ---->输出：将频率值依次存储到了HF【】数组中
{   ifstream inputfile;                                  //定义出输入文件流，并检测是否存在。
	char c;
	inputfile.open(input, ios::binary);
	if (!inputfile){
		cout << "打开失败，文件不存在，请认真检查" << endl;
		return false;
	    }	
	while (inputfile.get(c)){                           //按字节从文件中取出字符，每个字符按照ascii码值加上128，后续处理方便：打印字符值为从32到128，顺移128，为160-256；
		if (HF[c + 128].weight == MAX){                 //若该字符是第一次出现，先初始化权值  ，并且叶节点数目加1即leafnum+1;  
			HF[c + 128].weight = 0;   
			leafnum++;  }
		HF[c + 128].weight++;  }
	inputfile.close();
	return true;
}


void HFTree::creat() //---------创建huffman树，---输入：无---->输出：完成的HF【】数组，即将parent，child都设置好了。
{
	for (int i = leaf; i<2 * leaf - 1; i++)  {         //可打印字符最大到达256，后面的可以用来存储树的结构，即父子关系。
		int loc1 = -1, loc2 = -1;                     //每次建立起一个父子关系后，就在256+的位置存储上父节点，同时子节点的parent值不再为-1，再在里面寻找无父结点时就不
		for (int j = 0; j<i; j++){                    //再参与，而新父节点则需要参与。
			if (HF[j].parent != -1)                   //parent！=-1意味着该结点已经有了父节点
				continue;
			if (loc1 == -1 || HF[j].weight < HF[loc1].weight)//这个关系式里，loc1=-1只在小循环中初次遍历中出现。
			{                                                //经过这两个式子，可以保证loc1和loc2中存储的是唯二小的结点
				loc2 = loc1;     loc1 = j;                   //loc1存储下第一个找到的无父结点
			}
			else if (loc2 == -1 || HF[j].weight < HF[loc2].weight)     loc2 = j;
		    }
		if (HF[loc1].weight == MAX || HF[loc2].weight == MAX || loc2 == -1) //MAX意味着搜索到了最后，再没有小的子节点了，loc2=-1也是（根到了511），只剩一棵树，结束  
			break;
		HF[i].weight = HF[loc1].weight + HF[loc2].weight;                   //为了减少压缩文件中需要写入的huffman树的信息，约定小标小的结点做为双亲结点的左孩子  
		HF[i].lchild = loc1>loc2 ? loc2 : loc1;                             //左子节点取较脚标小的。。
		HF[i].rchild = loc1>loc2 ? loc1 : loc2;
		HF[loc1].parent = i;
		HF[loc2].parent = i;
		root = i;//建立好树后根结点
	}
}


void HFTree::printcode() {//-------输出编码函数----输入：无----->输出：就是把编码值挨个排出来
	for (int i = 0; i<leaf; i++)
	{
		if (HF[i].codelength != 0)//codelength!=0意味着没有这个字符出现
		{
			cout << "值为" << i - 128 << "的字符的huffman编码：";
			for (int j = 0; j<HF[i].codelength; j++)
			{
				cout << HF[i].code[j];
			}
			cout << endl;
		}
	}
}

void HFTree::code() //--------编码函数-----输入：无---->输出：在HF【】数组的每个元素的code上存储了编码值，codelength为编码长度
{
	for (int i = 0; i<leaf; i++)                 //对于每一个元素
	{
		int len = 0;   int loc = i; 
		while (HF[loc].parent != -1)            //经过建立树之后，所有的元素都有父节点，对于有父节点的，向根迭代，找出层数
		   {     
			len++;
			loc = HF[loc].parent;
		   }
		HF[i].codelength = len;
		HF[i].code = new int[len];
		loc = i;
		for (int j = len - 1; j >= 0; j--)      //编码过程是从len-1到0，依次从当前节点向上询问父，父来判断该节点是他左还是右子女
		   {                                    //编完后就存储在了code中，code为指针
			if (loc == HF[HF[loc].parent].lchild)
				HF[i].code[j] = 0;
			else     HF[i].code[j] = 1;
			loc = HF[loc].parent;
		   }
	}
}

void HFTree::addbit(int bit) {//填充byte函数----输入：填充值------>输出：在结尾可能会不足一byte，添加满
	if (bit == 0)   byte = byte << 1;
	else   byte = ((byte << 1) | 1);
	bitsum++;
}

void HFTree::resetbyte() {    //清除byte缓冲；
	byte = 0;  bitsum = 0;
}
bool HFTree::compress(char *input, char *output) //----压缩函数 ----输入：输入输出文件----->输出：压缩好的文件，返回bool
{
	char c;
	if (!count(input))    return false;//统计频率
	creat();                           //建立哈夫曼树
	code();                            //编码
	ifstream ifs;                      //打开文件并检测
	ofstream ofs;
	ifs.open(input, ios::binary);
	ofs.open(output, ios::binary);
	if (!ifs)  {
		cout << "无法打开文件 " << input << '!' << endl;
		return false;
	}
	if (!ofs)  {
		cout << "无法打开文件 " << output << '!' << endl;
		return false;
	}
	ofs.put(0);                            //在文件首插入0；
	ofs.put(root - 384);                   //减去384纯粹是为了保证根的描述只占一个字节，（511-127）防止越界。这样并不会影响后面的解压。原因：编解压规则一致即可。
	for (int i = 0; i<leaf * 2 - 1; i++)  
	    {                                  //对于没有出现的元素，放置为127.对于有父的将父位置减384放置，同样是为了防止越界。若其父为511会为127.同时511也会为127
		if (HF[i].parent == -1)               //会有影响吗：不会。无论根是不是511，都会刻意存在了root中放在文件首。，即511若是根，则root=511，不是，则不需考虑
			ofs.put(127);   else
			ofs.put(HF[i].parent - 384);
	    }
	while (ifs.get(c))  {
		int tmp = c + 128;                //tmp中存储的是该字符在HF数组中位置
		for (int i = 0; i < HF[tmp].codelength; i++)   {
			addbit(HF[tmp].code[i]);      //按照HF对应指针code的值向byte里插入0或者1，每插够8为形成byte后写入到文件中
			if (bitsum == 8){
				ofs.put(byte);
				resetbyte();
			}
		}
	}
    if (bitsum != 0){//到最后若还有不满8个的，就填满，不妨填0.但要记录填了多少，用lacknum来记录
		for (int i = bitsum; i < 8; i++)
		{
			addbit(0);    lacknum++;
		}
			ofs.put(byte);   resetbyte();
		}
	
	ofs.seekp(0, ios::beg);
	ofs.put(lacknum);//把lacknum存储在文件首
	ifs.close();
	ofs.close();
	return true;
}


bool HFTree::icompress(char *input, char *output)//---解压函数---输入：输入输出文件---->输出：解压好的文件 
{
	queue<char> q;                       //队列q；
	char c;
	ifstream ifs;                        //输入输出文件并检测
	ofstream ofs;
	ifs.open(input, ios::binary);
	ofs.open(output, ios::binary);
	if (!ifs)  { cout << "无法打开文件 " << input << '!' << endl;   return true; }
	if (!ofs)  { cout << "无法打开文件 " << output << '!' << endl;   return false; }
	
	ifs.get(c);
	lacknum = c;                        //第一个字符为lacknum
	ifs.get(c);
	root = c + 384;                     //第二个字符加上384为根的位置
	if (root != 511){
		for (int i = 0; i < leaf * 2 - 1; i++)      //从文件中提取出来哈夫曼树，i为HF脚标
		{                                      //整个过程都是恢复树，是当时存储树的逆操作
			ifs.get(c);                           //一个共识是所有有意义的元素都会有父，只有根没有，根已单独取出，所以这个
			if (c == 127)    continue;            //树的主要内容就是每个元素的父，有了这个，就可以轻易恢复树
			else   {                              //当然，记录了每个节点的父后还要知道这个节点是这个父的左/右子女
				HF[i].parent = c + 384;           //由于前面是脚标小的为左，这里是这样检测的，可以改进
				if (HF[c + 384].lchild == -1)
					HF[c + 384].lchild = i;
				else     HF[c + 384].rchild = i;
			}
		}
	}
	else{
		for (int i = 0; i < leaf * 2 - 1; i++)      
		{                                     
			ifs.get(c);                           
			if (c == 127)    {
				HF[i].parent = 511;
				if (HF[511].lchild == -1)
					HF[511].lchild = i;
				else     HF[511].rchild = i;
			             }
			else   {                              
				HF[i].parent = c + 384;           
				if (HF[c + 384].lchild == -1)
					HF[c + 384].lchild = i;
				else     HF[c + 384].rchild = i;
			}
		}

	}
	int point = root;

	while (ifs.get(c))   q.push(c);           //把所有字符存储到队列q中,队列，即从front到end是按照顺序的。

	while (q.size()>1)
	{
		c = q.front();                       //取队列首
		for (int i = 0; i<8; i++)            //匹配，因为每一个byte取出后所代表的不一定是一个字符
		{
			if (int(c & 128) == 0)          //c首位为0，则可以知道这个字符是左子树上的
			{                               //是依次将C左移，通过这样由根节点向叶节点不断迭代
				point = HF[point].lchild;  //最终到达叶后得到值，输出这个字符，因为之前加了128，这里要减去128.
				if (HF[point].lchild == -1 && HF[point].rchild == -1)//到达了叶节点       //至于为什么当时要加呢，因为有些是负值，char里超过127的为负值
				{                                                     //在511个元素中，大于256的为后面组成的父节点，前256为字符  //测试下
					ofs.put(char(point - 128));
					point = root;
				}
				c = c << 1;
			}
			else    {          
				point = HF[point].rchild;
				if (HF[point].lchild == -1 && HF[point].rchild == -1)
				{
					ofs.put(char(point - 128));      point = root;
				}     c = c << 1;
			}
		}   q.pop();                           //移除，注意这里是把一byte遍历完成了，pop出，未匹配完的过程依旧存储在point里，得到新的c后会继续匹配
	}

	c = q.front();                           //最后一个字符
	for (int i = 0; i<8 - lacknum; i++)
	{
		if (int(c & 128) == 0)
		{
			point = HF[point].lchild;
			if (HF[point].lchild == -1 && HF[point].rchild == -1)
			{
				ofs.put(char(point - 128));
				point = root;
			}    c = c << 1;
		}
		else{
			point = HF[point].rchild;
			if (HF[point].lchild == -1 && HF[point].rchild == -1)
			{
				ofs.put(char(point - 128));     point = root;
			}
			c = c << 1;
		}
	}
	q.pop();  ifs.close();  ofs.close();  return true;
}