//#include "HFTree.h"
#include "stdafx.h"
#include <iostream>
using namespace std;

HFTree::HFTree()//--------���캯��
{
	root = 0;
	leafnum = 0;
	byte = 0;
	bitsum = 0;
	lacknum = 0;
}
HFTree::~HFTree()//--------������������Ҫ����ʵ������ָ�룬��Ҫ�ر�delete��delete����ɾ�������������ڽ�֮��Ϊnull
{
	for (int i = 0; i < leaf; i++)
	    {   if (HF[i].codelength != 0){
			delete HF[i].code;    }
	    }
}

bool HFTree::count(char *input)//-----------ͳ�ƺ���     //���룺�ļ� ---->�������Ƶ��ֵ���δ洢����HF����������
{   ifstream inputfile;                                  //����������ļ�����������Ƿ���ڡ�
	char c;
	inputfile.open(input, ios::binary);
	if (!inputfile){
		cout << "��ʧ�ܣ��ļ������ڣ���������" << endl;
		return false;
	    }	
	while (inputfile.get(c)){                           //���ֽڴ��ļ���ȡ���ַ���ÿ���ַ�����ascii��ֵ����128�����������㣺��ӡ�ַ�ֵΪ��32��128��˳��128��Ϊ160-256��
		if (HF[c + 128].weight == MAX){                 //�����ַ��ǵ�һ�γ��֣��ȳ�ʼ��Ȩֵ  ������Ҷ�ڵ���Ŀ��1��leafnum+1;  
			HF[c + 128].weight = 0;   
			leafnum++;  }
		HF[c + 128].weight++;  }
	inputfile.close();
	return true;
}


void HFTree::creat() //---------����huffman����---���룺��---->�������ɵ�HF�������飬����parent��child�����ú��ˡ�
{
	for (int i = leaf; i<2 * leaf - 1; i++)  {         //�ɴ�ӡ�ַ���󵽴�256������Ŀ��������洢���Ľṹ�������ӹ�ϵ��
		int loc1 = -1, loc2 = -1;                     //ÿ�ν�����һ�����ӹ�ϵ�󣬾���256+��λ�ô洢�ϸ��ڵ㣬ͬʱ�ӽڵ��parentֵ����Ϊ-1����������Ѱ���޸����ʱ�Ͳ�
		for (int j = 0; j<i; j++){                    //�ٲ��룬���¸��ڵ�����Ҫ���롣
			if (HF[j].parent != -1)                   //parent��=-1��ζ�Ÿý���Ѿ����˸��ڵ�
				continue;
			if (loc1 == -1 || HF[j].weight < HF[loc1].weight)//�����ϵʽ�loc1=-1ֻ��Сѭ���г��α����г��֡�
			{                                                //����������ʽ�ӣ����Ա�֤loc1��loc2�д洢����Ψ��С�Ľ��
				loc2 = loc1;     loc1 = j;                   //loc1�洢�µ�һ���ҵ����޸����
			}
			else if (loc2 == -1 || HF[j].weight < HF[loc2].weight)     loc2 = j;
		    }
		if (HF[loc1].weight == MAX || HF[loc2].weight == MAX || loc2 == -1) //MAX��ζ���������������û��С���ӽڵ��ˣ�loc2=-1Ҳ�ǣ�������511����ֻʣһ����������  
			break;
		HF[i].weight = HF[loc1].weight + HF[loc2].weight;                   //Ϊ�˼���ѹ���ļ�����Ҫд���huffman������Ϣ��Լ��С��С�Ľ����Ϊ˫�׽�������  
		HF[i].lchild = loc1>loc2 ? loc2 : loc1;                             //���ӽڵ�ȡ�Ͻű�С�ġ���
		HF[i].rchild = loc1>loc2 ? loc1 : loc2;
		HF[loc1].parent = i;
		HF[loc2].parent = i;
		root = i;//��������������
	}
}


void HFTree::printcode() {//-------������뺯��----���룺��----->��������ǰѱ���ֵ�����ų���
	for (int i = 0; i<leaf; i++)
	{
		if (HF[i].codelength != 0)//codelength!=0��ζ��û������ַ�����
		{
			cout << "ֵΪ" << i - 128 << "���ַ���huffman���룺";
			for (int j = 0; j<HF[i].codelength; j++)
			{
				cout << HF[i].code[j];
			}
			cout << endl;
		}
	}
}

void HFTree::code() //--------���뺯��-----���룺��---->�������HF���������ÿ��Ԫ�ص�code�ϴ洢�˱���ֵ��codelengthΪ���볤��
{
	for (int i = 0; i<leaf; i++)                 //����ÿһ��Ԫ��
	{
		int len = 0;   int loc = i; 
		while (HF[loc].parent != -1)            //����������֮�����е�Ԫ�ض��и��ڵ㣬�����и��ڵ�ģ�����������ҳ�����
		   {     
			len++;
			loc = HF[loc].parent;
		   }
		HF[i].codelength = len;
		HF[i].code = new int[len];
		loc = i;
		for (int j = len - 1; j >= 0; j--)      //��������Ǵ�len-1��0�����δӵ�ǰ�ڵ�����ѯ�ʸ��������жϸýڵ�������������Ů
		   {                                    //�����ʹ洢����code�У�codeΪָ��
			if (loc == HF[HF[loc].parent].lchild)
				HF[i].code[j] = 0;
			else     HF[i].code[j] = 1;
			loc = HF[loc].parent;
		   }
	}
}

void HFTree::addbit(int bit) {//���byte����----���룺���ֵ------>������ڽ�β���ܻ᲻��һbyte�������
	if (bit == 0)   byte = byte << 1;
	else   byte = ((byte << 1) | 1);
	bitsum++;
}

void HFTree::resetbyte() {    //���byte���壻
	byte = 0;  bitsum = 0;
}
bool HFTree::compress(char *input, char *output) //----ѹ������ ----���룺��������ļ�----->�����ѹ���õ��ļ�������bool
{
	char c;
	if (!count(input))    return false;//ͳ��Ƶ��
	creat();                           //������������
	code();                            //����
	ifstream ifs;                      //���ļ������
	ofstream ofs;
	ifs.open(input, ios::binary);
	ofs.open(output, ios::binary);
	if (!ifs)  {
		cout << "�޷����ļ� " << input << '!' << endl;
		return false;
	}
	if (!ofs)  {
		cout << "�޷����ļ� " << output << '!' << endl;
		return false;
	}
	ofs.put(0);                            //���ļ��ײ���0��
	ofs.put(root - 384);                   //��ȥ384������Ϊ�˱�֤��������ֻռһ���ֽڣ���511-127����ֹԽ�硣����������Ӱ�����Ľ�ѹ��ԭ�򣺱��ѹ����һ�¼��ɡ�
	for (int i = 0; i<leaf * 2 - 1; i++)  
	    {                                  //����û�г��ֵ�Ԫ�أ�����Ϊ127.�����и��Ľ���λ�ü�384���ã�ͬ����Ϊ�˷�ֹԽ�硣���丸Ϊ511��Ϊ127.ͬʱ511Ҳ��Ϊ127
		if (HF[i].parent == -1)               //����Ӱ���𣺲��ᡣ���۸��ǲ���511��������������root�з����ļ��ס�����511���Ǹ�����root=511�����ǣ����迼��
			ofs.put(127);   else
			ofs.put(HF[i].parent - 384);
	    }
	while (ifs.get(c))  {
		int tmp = c + 128;                //tmp�д洢���Ǹ��ַ���HF������λ��
		for (int i = 0; i < HF[tmp].codelength; i++)   {
			addbit(HF[tmp].code[i]);      //����HF��Ӧָ��code��ֵ��byte�����0����1��ÿ�幻8Ϊ�γ�byte��д�뵽�ļ���
			if (bitsum == 8){
				ofs.put(byte);
				resetbyte();
			}
		}
	}
    if (bitsum != 0){//����������в���8���ģ���������������0.��Ҫ��¼���˶��٣���lacknum����¼
		for (int i = bitsum; i < 8; i++)
		{
			addbit(0);    lacknum++;
		}
			ofs.put(byte);   resetbyte();
		}
	
	ofs.seekp(0, ios::beg);
	ofs.put(lacknum);//��lacknum�洢���ļ���
	ifs.close();
	ofs.close();
	return true;
}


bool HFTree::icompress(char *input, char *output)//---��ѹ����---���룺��������ļ�---->�������ѹ�õ��ļ� 
{
	queue<char> q;                       //����q��
	char c;
	ifstream ifs;                        //��������ļ������
	ofstream ofs;
	ifs.open(input, ios::binary);
	ofs.open(output, ios::binary);
	if (!ifs)  { cout << "�޷����ļ� " << input << '!' << endl;   return true; }
	if (!ofs)  { cout << "�޷����ļ� " << output << '!' << endl;   return false; }
	
	ifs.get(c);
	lacknum = c;                        //��һ���ַ�Ϊlacknum
	ifs.get(c);
	root = c + 384;                     //�ڶ����ַ�����384Ϊ����λ��
	if (root != 511){
		for (int i = 0; i < leaf * 2 - 1; i++)      //���ļ�����ȡ��������������iΪHF�ű�
		{                                      //�������̶��ǻָ������ǵ�ʱ�洢���������
			ifs.get(c);                           //һ����ʶ�������������Ԫ�ض����и���ֻ�и�û�У����ѵ���ȡ�����������
			if (c == 127)    continue;            //������Ҫ���ݾ���ÿ��Ԫ�صĸ�������������Ϳ������׻ָ���
			else   {                              //��Ȼ����¼��ÿ���ڵ�ĸ���Ҫ֪������ڵ������������/����Ů
				HF[i].parent = c + 384;           //����ǰ���ǽű�С��Ϊ���������������ģ����ԸĽ�
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

	while (ifs.get(c))   q.push(c);           //�������ַ��洢������q��,���У�����front��end�ǰ���˳��ġ�

	while (q.size()>1)
	{
		c = q.front();                       //ȡ������
		for (int i = 0; i<8; i++)            //ƥ�䣬��Ϊÿһ��byteȡ����������Ĳ�һ����һ���ַ�
		{
			if (int(c & 128) == 0)          //c��λΪ0�������֪������ַ����������ϵ�
			{                               //�����ν�C���ƣ�ͨ�������ɸ��ڵ���Ҷ�ڵ㲻�ϵ���
				point = HF[point].lchild;  //���յ���Ҷ��õ�ֵ���������ַ�����Ϊ֮ǰ����128������Ҫ��ȥ128.
				if (HF[point].lchild == -1 && HF[point].rchild == -1)//������Ҷ�ڵ�       //����Ϊʲô��ʱҪ���أ���Ϊ��Щ�Ǹ�ֵ��char�ﳬ��127��Ϊ��ֵ
				{                                                     //��511��Ԫ���У�����256��Ϊ������ɵĸ��ڵ㣬ǰ256Ϊ�ַ�  //������
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
		}   q.pop();                           //�Ƴ���ע�������ǰ�һbyte��������ˣ�pop����δƥ����Ĺ������ɴ洢��point��õ��µ�c������ƥ��
	}

	c = q.front();                           //���һ���ַ�
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