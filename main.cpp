#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "btree.h"
using namespace std;

#define MAX_DOC_NUM 10	 //文档数

//访问一个B-树的结点
void access_node(PBTNode pnode, ofstream &outfile)
{	
	int i;
	for(i=0; i<pnode->data_num; i++)
	{
		ostringstream posting_list_str;
		if(pnode->child[i] != NULL)
			access_node(pnode->child[i], outfile);		//访问当前数据的前一个指针指向的子树

		Posting *current_posting = pnode->data[i].posting_list;
		Position *current_position;
		while(current_posting != NULL)
		{
			//打印当前文档中出现该单词的所有位置
			posting_list_str << "<" << current_posting->doc_id << ", " 
					<< current_posting->term_frequency << ", ";

			current_position = current_posting->pos_info;
			while(current_position != NULL)
			{
				posting_list_str << current_position->pos_num << " ";
				current_position = current_position->next_position;
			}
			posting_list_str << "> ";  //一个文档的位置输出完毕
			current_posting = current_posting->next_posting;
		}
		
		outfile << setiosflags(ios::left) << setw(20) << *pnode->data[i].word << posting_list_str.str() << endl;
	}
	if(pnode->child[i] != NULL)
		access_node(pnode->child[i], outfile);  //访问最后一棵子树
}

int main(int argc, char *argv[])
{
	//创建B-树
	BTree bt;

	//扫描文档模块
	cout << "文档扫描中..." << endl;
	for(int doc_id=1; doc_id<=MAX_DOC_NUM; doc_id++)
	{
		ostringstream  file_name("documents/", ios::ate);
		file_name << doc_id << ".txt";
		ifstream infile(file_name.str(), ios::in);		//建立文档输入流
		if(!infile)
		{
			cout<<"open file error!"<<endl;
			exit(1);
		}

		//进入循环，不停地读取单词，插入到B-树
		int word_counter = 1;  //单词计数器，用于统计单词的位置信息
		while(1)
		{
			string *word = new string();
			if(!(infile >> *word))	
			{
				delete word;
				break;
			}
			//将大写统一转成小写
			for(int i=0; i<word->size(); i++)
			{		
				if((*word)[i] >= 'A' && (*word)[i] <= 'Z')
				{
					(*word)[i] += 32;
				}
			}
			if(*word != "a" && *word != "an" && *word != "the" && *word != "by"
				&& *word != "to" && *word != "has" && *word != "on")
			{
				//过滤掉无效单词后，将单词以及单词的文档信息和位置信息插入B-树
				bt.insert(word, doc_id, word_counter);
			}

			word_counter++;
		}

		infile.close();
	}
	cout << "倒排索引建立完成!" << endl;

	//遍历B-树词典，将数据输出到磁盘文件
	ofstream outfile("documents/index.txt", ios::out);
	cout << "正在生成倒排文件..." << endl;
	access_node(bt.get_root(), outfile);
	outfile.close();
	cout << "倒排文件生成成功!" << endl;


	//B-树建立完成后，提供外界查询的接口
	string word;
	Record tmp_record;
	tmp_record.word = &word;
	Posting *current_posting;
	Position *current_position;
	while(1)
	{
		cout << "请输入查询关键词 : " << endl;
		cin >> word;
		if(bt.search(tmp_record))
		{
			//已获得倒排列表的指针，直接读取即可
			current_posting = tmp_record.posting_list;
			while(current_posting != NULL)
			{
				//打印当前文档中出现该单词的所有位置
				cout << "文档 : " << current_posting->doc_id << "  出现次数 : " 
					 << current_posting->term_frequency << endl;
				cout << "位置 : ";
				current_position = current_posting->pos_info;
				while(current_position != NULL)
				{
					cout << current_position->pos_num << "  ";
					current_position = current_position->next_position;
				}
				cout << endl;  //一个文档的位置输出完毕
				current_posting = current_posting->next_posting;
			}
			cout << endl;
		} else {
			cout << "检索失败！" << endl << endl;
		}
	}

	return 0;
}
