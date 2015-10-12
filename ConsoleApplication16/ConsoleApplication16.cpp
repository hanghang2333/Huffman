// ConsoleApplication16.cpp : 定义控制台应用程序的入口点。
//
//格式：txt----英文压缩效果明显，中文无法解压，原因：压缩时正常，解压时由于中英文编码方式不同，还按照1byte输出一次出错。
//      docx---中英文都可以，但是基本上没有压缩。原因：docx主要空间并不在于其中的文字，可能本身已经是压缩了。所以中文也可以正常解压。
//      jpg----和docx一样，基本上没有压缩。图像不同于文字，基本上没有相同的像素点
//很多文件本身已经有了进行压缩，再按照huffman压缩并没有效果。但是由于编码方式规范，可以正常进行压缩解压。

#include "stdafx.h"
//#include "HFTree.h"  
#include <iostream>
using namespace std;
void main() {
	int choice = 1;
	char input[255], output[255];
	HFTree h;  while (choice)  {
		cout << "____________________________________________________________"<< endl;
		cout << " *      **    **   哈夫曼编码压缩解压程序   **     **      *" << endl;
		cout << " *    **  *   *  *                        **  *   *  *     *" << endl;
		cout << " *    *     *   *         1)压缩          *     *     *    *" << endl;
		cout << " *      *      *          2)解压            *       *      *" << endl;
		cout << " *        *  *            3)刷新              *   *        *" << endl;
		cout << " *          *                                   *          *" << endl;
		cout << " *                   ╮(╯▽╰)╭      李杭                *" << endl;
		cout << " *                    (=￣ω￣=)　    张世杰               *" << endl;
		cout << " *                   m( _　_ )m       余有为               *" << endl;
		cout << " *                                                TJU      *" << endl;
		cout << "_______________________________________________2015.09.25___" << endl;
		cout << ">";
		cin >> choice;


		switch (choice)   {
		case 1:    {
					   cout << "请输入要压缩的文件名：";
					   cin >> input;
					   cout << "请输入压缩后的文件名：";
					   cin >> output;
					   if (h.compress(input, output))
					   {
						   //h.printcode();
						   cout << endl << "文件压缩成功!" << endl;
					   }
					   else
					   {
						   cout << endl << "文件压缩失败!" << endl;
					   }    }
			break;
		case 2:    {     cout << "请输入待解压的文件名：";
			             cin >> input;
			             cout << "请输入解压后的文件名：";     
						 cin >> output;
			             if (h.icompress(input, output))      
					     cout << endl << "文件解压成功!" << endl;    
			             else    
						 cout << endl << "文件解压失败!" << endl;  
		          }
			break;
		case 3:    {     //执行清屏命令    
					   system("cls");       }
			break;
		default:    cout << "参数错误！请重新输入" << endl;
		}
		cout << endl;
	}
}
