#include <iostream>
#include <string>
using namespace std;

#define M 15		//定义B-树的阶数,奇数

struct Position {	//定义一个单词在文档的位置信息结点
	int pos_num;			//位置数，即在文档中第几个
	Position *next_position;	//可能出现的下一个位置的信息
	Position()
	{
		next_position = NULL;	//构造函数初始化next为NULL
	}
};

struct Posting {		//定义一个倒排索引项，若干个posting组成一个倒排列表
	int doc_id;				//文档编号
	Posting *next_posting;  //下一个倒排索引项
	int term_frequency;		//词项频率，简单地用次数代替频率
	Position *pos_info;		//位置信息链表，链表存储了出现的所有位置信息
	Posting()
	{
		next_posting = NULL;
		term_frequency = 0;
		pos_info = NULL;
	}
};


//一个单词所在的逻辑记录，包括单词内容和倒排列表
struct Record{
	string *word;				//指向真实的单词内容
	Posting *posting_list;		//指向词项自己的倒排列表
	Record()
	{
		word = NULL;
		posting_list = NULL;
	}
};


struct BTNode{
	int data_num;			//结点中记录的个数，keynum<=M-1
	BTNode *parent;			//指向父结点的指针
	Record data[M-1];		//记录向量
	BTNode *child[M];		//子树指针向量
	BTNode()	//定义一个结点类的构造函数，用于将新建结点初始化
	{
		data_num = 0;		//记录数为0
		parent = NULL;
		for(int i=0; i<M; i++)
			child[i] = NULL;	//新结点没有记录，没有子树
	}
};
typedef BTNode * PBTNode;


//定义B-树类
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