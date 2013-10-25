#include "btree.h"
#include <iostream>

BTree::BTree()
{
	root = new BTNode();	//创建B-树的根结点
}

bool BTree::insert(string *a_word, int doc_id, int pos_num)
{
	PBTNode current_pnode = root;
	
	//先将数据搜索到叶子结点上,current_pnode为应当接受新纪录的叶子结点
	while(1)
	{
		if(current_pnode->child[0] == NULL)
		{
			//进入叶子结点，判断没有相同的key后退出循环
			for(int i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *a_word)
				{
					//找到该单词已经在词典中，那就更新倒排列表,同时释放原来单词的空间
					//设置一个当前索引项，用于遍历整个索引列表，来找到合适的位置插入新信息
					Posting *current_posting = current_pnode->data[i].posting_list; 
					while(1)
					{
						if(current_posting->doc_id == doc_id)
						{
							current_posting->term_frequency++;
							Position *tmp_position = new Position();
							tmp_position->pos_num = pos_num;
							
							//遍历到最后一个位置信息结点
							Position *current_position = current_posting->pos_info;
							while(current_position->next_position != NULL)
							{
								current_position = current_position->next_position;
							}
							current_position->next_position = tmp_position;
							break;
						} else if(current_posting->next_posting == NULL)
						{
							Posting *tmp_new_posting = new Posting();
							tmp_new_posting->doc_id = doc_id;
							tmp_new_posting->term_frequency = 1;
							tmp_new_posting->pos_info = new Position();
							tmp_new_posting->pos_info->pos_num = pos_num;
			
							current_posting->next_posting = tmp_new_posting;
							break;
						} else {
							current_posting  = current_posting->next_posting;
						}
					}

					delete a_word;	//释放传递单词时占用的空间
					return true;  //单词的新信息更新成功
				}
			}
			break;	//没有相同元素，则进入叶子结点成功
		}
		else {
			//当前结点不是叶子结点，通过大小比较进入下一层子树
			int i;
			for(i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *a_word)
				{
					//找到该单词已经在词典中，那就更新倒排列表,同时释放原来单词的空间
					//设置一个当前索引项，用于遍历整个索引列表，来找到合适的位置插入新信息
					Posting *current_posting = current_pnode->data[i].posting_list; 
					while(1)
					{
						if(current_posting->doc_id == doc_id)
						{
							current_posting->term_frequency++;
							Position *tmp_position = new Position();
							tmp_position->pos_num = pos_num;
							
							//遍历到最后一个位置信息结点
							Position *current_position = current_posting->pos_info;
							while(current_position->next_position != NULL)
							{
								current_position = current_position->next_position;
							}
							current_position->next_position = tmp_position;
							break;
						} else if(current_posting->next_posting == NULL)
						{
							Posting *tmp_new_posting = new Posting();
							tmp_new_posting->doc_id = doc_id;
							tmp_new_posting->term_frequency = 1;
							tmp_new_posting->pos_info = new Position();
							tmp_new_posting->pos_info->pos_num = pos_num;
			
							current_posting->next_posting = tmp_new_posting;
							break;
						} else {
							current_posting  = current_posting->next_posting;
						}
					}

					delete a_word;	//释放传递单词时占用的空间
					return true;  //单词的新信息更新成功
				}
				if(*(current_pnode->data[i].word) > *a_word) 
				{break;}
			}			
			current_pnode = current_pnode->child[i];
		}
	}

	//插入结点进入B-树，通过循环分裂结点，current_pnode为待插入记录的插入位置所在结点
	Record tmp_record;		//待插入的记录
	tmp_record.word = a_word;
	tmp_record.posting_list = new Posting();
	tmp_record.posting_list->doc_id = doc_id;
	tmp_record.posting_list->term_frequency = 1;
	tmp_record.posting_list->pos_info = new Position();
	tmp_record.posting_list->pos_info->pos_num = pos_num;
	PBTNode tmp_right_pointer = NULL;	//待插入的记录的右指针，右指针指向一个分裂后的新结点
	while(1)
	{
		if(current_pnode->data_num != M-1){
			//current_pnode未满，可以直接插入
			int i;
			for(i=current_pnode->data_num-1; i>=0; i--)
			{
				if(*(current_pnode->data[i].word) > *(tmp_record.word))
				{
					current_pnode->data[i+1] = current_pnode->data[i];
					current_pnode->child[i+2] = current_pnode->child[i+1];
				} else {
					break;
				}
			}
			current_pnode->data[i+1] = tmp_record;	//记录的插入
			current_pnode->data_num++;
			current_pnode->child[i+2] = tmp_right_pointer;	//右指针的插入
			if(tmp_right_pointer!=NULL) {
				tmp_right_pointer->parent = current_pnode;	//给右指针的子树找到父节点
			}
			return true;	//插入成功
		} else {
			//current_pnode已满，必须分裂current_pnode结点						


			PBTNode new_node = new BTNode();

			//先以new_node的第一个数据点为缓冲区，将tmp_record插入到排序序列中
			int i = M-2;
			while(1)
			{
				if(i==M-2){
					if(*(current_pnode->data[i].word) > *(tmp_record.word))
					{
						new_node->data[0] = current_pnode->data[i];
						new_node->child[1] = current_pnode->child[i+1];
					} else {
						new_node->data[0] = tmp_record;
						new_node->child[1] = tmp_right_pointer;		
						break;		//待插入记录一旦被插入就可以跳出循环
					}
				} else {
					if(*(current_pnode->data[i].word) > *(tmp_record.word))
					{
						current_pnode->data[i+1] = current_pnode->data[i];
						current_pnode->child[i+2] = current_pnode->child[i+1];
					} else {
						current_pnode->data[i+1] = tmp_record;
						current_pnode->child[i+2] = tmp_right_pointer;
						if(tmp_right_pointer != NULL)
							tmp_right_pointer->parent = current_pnode;
						break;		//待插入记录一旦被插入就可以跳出循环
					}
				}
				i--;
				if(i == -1)
				{
					current_pnode->data[i+1] = tmp_record;
						current_pnode->child[i+2] = tmp_right_pointer;
						if(tmp_right_pointer != NULL)
							tmp_right_pointer->parent = current_pnode;
						break;		//待插入记录一旦被插入就可以跳出循环
				}
			}

			//将重新排好序的数据后一半转移到新节点中去，并取出中间项
			int j = M/2-1;
			new_node->data[j] = new_node->data[0];
			new_node->child[j+1] = new_node->child[1];
			if(new_node->child[j+1] != NULL)
				new_node->child[j+1]->parent = new_node;
			
			i = M-2;
			j--;
			while(j>=0)
			{
				new_node->data[j] = current_pnode->data[i];
				new_node->child[j+1] = current_pnode->child[i+1];
				if(new_node->child[j+1] != NULL)
					new_node->child[j+1]->parent = new_node;

				j--; 
				i--;
			}

			tmp_record = current_pnode->data[i];		//更新悬浮的记录
			new_node->child[0] = current_pnode->child[i+1];
			if(new_node->child[0] != NULL)
				new_node->child[0]->parent = new_node;
			
			current_pnode->data_num = M/2;			//修改被分裂开的原来结点的数据个数
			new_node->data_num = M/2;				//修改被分裂开的新结点的数据个数
			tmp_right_pointer = new_node;		//更新右子树指针


			//结点分裂成功之后考虑问题
			//如果current_pnode没有父结点，那就更新root结点；否则以父结点为current_pnode,进入下一轮循环
			if(current_pnode->parent == NULL)
			{
				//创建新的根结点，并将提取的记录装入新的根节点中
				PBTNode new_root_node = new BTNode();
				new_root_node->data_num = 1;
				new_root_node->data[0] = tmp_record;

				current_pnode->parent = new_root_node;
				tmp_right_pointer->parent = new_root_node;

				new_root_node->child[0] = current_pnode;
				new_root_node->child[1] = tmp_right_pointer;
				
				root = new_root_node;//更新根结点
				return true;	//插入成功                               
			} else {
				current_pnode = current_pnode->parent;
			}
		}
	}
}

bool BTree::search(Record &a_record)
{
	PBTNode current_pnode = root;
	
	//先将数据搜索到叶子结点上,current_pnode为应当接受新纪录的叶子结点
	while(1)
	{
		if(current_pnode->child[0] == NULL)
		{
			//进入叶子结点，在叶子结点中找到元素
			for(int i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *(a_record.word))
				{
					//找到单词，取出倒排信息即可
					a_record.posting_list = current_pnode->data[i].posting_list;
					return true;
				}
			}

			return false; //遍历到叶子结点也没能找到
		}
		else {
			//当前结点不是叶子结点，通过大小比较进入下一层子树
			int i;
			for(i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *(a_record.word))
				{
					//在非叶子结点中找到元素
					a_record.posting_list = current_pnode->data[i].posting_list;
					return true;
				}
				if(*(current_pnode->data[i].word) > *(a_record.word)) 
				{break;}
			}			
			current_pnode = current_pnode->child[i];
		}
	}

}


PBTNode BTree::get_root()
{
	return root;
}