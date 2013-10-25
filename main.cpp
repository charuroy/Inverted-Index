#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "btree.h"
using namespace std;

#define MAX_DOC_NUM 10	 //�ĵ���

//����һ��B-���Ľ��
void access_node(PBTNode pnode, ofstream &outfile)
{	
	int i;
	for(i=0; i<pnode->data_num; i++)
	{
		ostringstream posting_list_str;
		if(pnode->child[i] != NULL)
			access_node(pnode->child[i], outfile);		//���ʵ�ǰ���ݵ�ǰһ��ָ��ָ�������

		Posting *current_posting = pnode->data[i].posting_list;
		Position *current_position;
		while(current_posting != NULL)
		{
			//��ӡ��ǰ�ĵ��г��ָõ��ʵ�����λ��
			posting_list_str << "<" << current_posting->doc_id << ", " 
					<< current_posting->term_frequency << ", ";

			current_position = current_posting->pos_info;
			while(current_position != NULL)
			{
				posting_list_str << current_position->pos_num << " ";
				current_position = current_position->next_position;
			}
			posting_list_str << "> ";  //һ���ĵ���λ��������
			current_posting = current_posting->next_posting;
		}
		
		outfile << setiosflags(ios::left) << setw(20) << *pnode->data[i].word << posting_list_str.str() << endl;
	}
	if(pnode->child[i] != NULL)
		access_node(pnode->child[i], outfile);  //�������һ������
}

int main(int argc, char *argv[])
{
	//����B-��
	BTree bt;

	//ɨ���ĵ�ģ��
	cout << "�ĵ�ɨ����..." << endl;
	for(int doc_id=1; doc_id<=MAX_DOC_NUM; doc_id++)
	{
		ostringstream  file_name("documents/", ios::ate);
		file_name << doc_id << ".txt";
		ifstream infile(file_name.str(), ios::in);		//�����ĵ�������
		if(!infile)
		{
			cout<<"open file error!"<<endl;
			exit(1);
		}

		//����ѭ������ͣ�ض�ȡ���ʣ����뵽B-��
		int word_counter = 1;  //���ʼ�����������ͳ�Ƶ��ʵ�λ����Ϣ
		while(1)
		{
			string *word = new string();
			if(!(infile >> *word))	
			{
				delete word;
				break;
			}
			//����дͳһת��Сд
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
				//���˵���Ч���ʺ󣬽������Լ����ʵ��ĵ���Ϣ��λ����Ϣ����B-��
				bt.insert(word, doc_id, word_counter);
			}

			word_counter++;
		}

		infile.close();
	}
	cout << "���������������!" << endl;

	//����B-���ʵ䣬����������������ļ�
	ofstream outfile("documents/index.txt", ios::out);
	cout << "�������ɵ����ļ�..." << endl;
	access_node(bt.get_root(), outfile);
	outfile.close();
	cout << "�����ļ����ɳɹ�!" << endl;


	//B-��������ɺ��ṩ����ѯ�Ľӿ�
	string word;
	Record tmp_record;
	tmp_record.word = &word;
	Posting *current_posting;
	Position *current_position;
	while(1)
	{
		cout << "�������ѯ�ؼ��� : " << endl;
		cin >> word;
		if(bt.search(tmp_record))
		{
			//�ѻ�õ����б��ָ�룬ֱ�Ӷ�ȡ����
			current_posting = tmp_record.posting_list;
			while(current_posting != NULL)
			{
				//��ӡ��ǰ�ĵ��г��ָõ��ʵ�����λ��
				cout << "�ĵ� : " << current_posting->doc_id << "  ���ִ��� : " 
					 << current_posting->term_frequency << endl;
				cout << "λ�� : ";
				current_position = current_posting->pos_info;
				while(current_position != NULL)
				{
					cout << current_position->pos_num << "  ";
					current_position = current_position->next_position;
				}
				cout << endl;  //һ���ĵ���λ��������
				current_posting = current_posting->next_posting;
			}
			cout << endl;
		} else {
			cout << "����ʧ�ܣ�" << endl << endl;
		}
	}

	return 0;
}
