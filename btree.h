#include <iostream>
#include <string>
using namespace std;

#define M 15		//����B-���Ľ���,����

struct Position {	//����һ���������ĵ���λ����Ϣ���
	int pos_num;			//λ�����������ĵ��еڼ���
	Position *next_position;	//���ܳ��ֵ���һ��λ�õ���Ϣ
	Position()
	{
		next_position = NULL;	//���캯����ʼ��nextΪNULL
	}
};

struct Posting {		//����һ��������������ɸ�posting���һ�������б�
	int doc_id;				//�ĵ����
	Posting *next_posting;  //��һ������������
	int term_frequency;		//����Ƶ�ʣ��򵥵��ô�������Ƶ��
	Position *pos_info;		//λ����Ϣ��������洢�˳��ֵ�����λ����Ϣ
	Posting()
	{
		next_posting = NULL;
		term_frequency = 0;
		pos_info = NULL;
	}
};


//һ���������ڵ��߼���¼�������������ݺ͵����б�
struct Record{
	string *word;				//ָ����ʵ�ĵ�������
	Posting *posting_list;		//ָ������Լ��ĵ����б�
	Record()
	{
		word = NULL;
		posting_list = NULL;
	}
};


struct BTNode{
	int data_num;			//����м�¼�ĸ�����keynum<=M-1
	BTNode *parent;			//ָ�򸸽���ָ��
	Record data[M-1];		//��¼����
	BTNode *child[M];		//����ָ������
	BTNode()	//����һ�������Ĺ��캯�������ڽ��½�����ʼ��
	{
		data_num = 0;		//��¼��Ϊ0
		parent = NULL;
		for(int i=0; i<M; i++)
			child[i] = NULL;	//�½��û�м�¼��û������
	}
};
typedef BTNode * PBTNode;


//����B-����
class BTree
{
private:
	PBTNode root;
public:
	BTree();
	bool insert(string *a_word, int doc_id, int pos_num);
	bool search(Record &a_record);
	PBTNode get_root();
};