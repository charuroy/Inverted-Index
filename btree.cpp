#include "btree.h"
#include <iostream>

BTree::BTree()
{
	root = new BTNode();	//����B-���ĸ����
}

bool BTree::insert(string *a_word, int doc_id, int pos_num)
{
	PBTNode current_pnode = root;
	
	//�Ƚ�����������Ҷ�ӽ����,current_pnodeΪӦ�������¼�¼��Ҷ�ӽ��
	while(1)
	{
		if(current_pnode->child[0] == NULL)
		{
			//����Ҷ�ӽ�㣬�ж�û����ͬ��key���˳�ѭ��
			for(int i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *a_word)
				{
					//�ҵ��õ����Ѿ��ڴʵ��У��Ǿ͸��µ����б�,ͬʱ�ͷ�ԭ�����ʵĿռ�
					//����һ����ǰ��������ڱ������������б����ҵ����ʵ�λ�ò�������Ϣ
					Posting *current_posting = current_pnode->data[i].posting_list; 
					while(1)
					{
						if(current_posting->doc_id == doc_id)
						{
							current_posting->term_frequency++;
							Position *tmp_position = new Position();
							tmp_position->pos_num = pos_num;
							
							//���������һ��λ����Ϣ���
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

					delete a_word;	//�ͷŴ��ݵ���ʱռ�õĿռ�
					return true;  //���ʵ�����Ϣ���³ɹ�
				}
			}
			break;	//û����ͬԪ�أ������Ҷ�ӽ��ɹ�
		}
		else {
			//��ǰ��㲻��Ҷ�ӽ�㣬ͨ����С�ȽϽ�����һ������
			int i;
			for(i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *a_word)
				{
					//�ҵ��õ����Ѿ��ڴʵ��У��Ǿ͸��µ����б�,ͬʱ�ͷ�ԭ�����ʵĿռ�
					//����һ����ǰ��������ڱ������������б����ҵ����ʵ�λ�ò�������Ϣ
					Posting *current_posting = current_pnode->data[i].posting_list; 
					while(1)
					{
						if(current_posting->doc_id == doc_id)
						{
							current_posting->term_frequency++;
							Position *tmp_position = new Position();
							tmp_position->pos_num = pos_num;
							
							//���������һ��λ����Ϣ���
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

					delete a_word;	//�ͷŴ��ݵ���ʱռ�õĿռ�
					return true;  //���ʵ�����Ϣ���³ɹ�
				}
				if(*(current_pnode->data[i].word) > *a_word) 
				{break;}
			}			
			current_pnode = current_pnode->child[i];
		}
	}

	//���������B-����ͨ��ѭ�����ѽ�㣬current_pnodeΪ�������¼�Ĳ���λ�����ڽ��
	Record tmp_record;		//������ļ�¼
	tmp_record.word = a_word;
	tmp_record.posting_list = new Posting();
	tmp_record.posting_list->doc_id = doc_id;
	tmp_record.posting_list->term_frequency = 1;
	tmp_record.posting_list->pos_info = new Position();
	tmp_record.posting_list->pos_info->pos_num = pos_num;
	PBTNode tmp_right_pointer = NULL;	//������ļ�¼����ָ�룬��ָ��ָ��һ�����Ѻ���½��
	while(1)
	{
		if(current_pnode->data_num != M-1){
			//current_pnodeδ��������ֱ�Ӳ���
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
			current_pnode->data[i+1] = tmp_record;	//��¼�Ĳ���
			current_pnode->data_num++;
			current_pnode->child[i+2] = tmp_right_pointer;	//��ָ��Ĳ���
			if(tmp_right_pointer!=NULL) {
				tmp_right_pointer->parent = current_pnode;	//����ָ��������ҵ����ڵ�
			}
			return true;	//����ɹ�
		} else {
			//current_pnode�������������current_pnode���						


			PBTNode new_node = new BTNode();

			//����new_node�ĵ�һ�����ݵ�Ϊ����������tmp_record���뵽����������
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
						break;		//�������¼һ��������Ϳ�������ѭ��
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
						break;		//�������¼һ��������Ϳ�������ѭ��
					}
				}
				i--;
				if(i == -1)
				{
					current_pnode->data[i+1] = tmp_record;
						current_pnode->child[i+2] = tmp_right_pointer;
						if(tmp_right_pointer != NULL)
							tmp_right_pointer->parent = current_pnode;
						break;		//�������¼һ��������Ϳ�������ѭ��
				}
			}

			//�������ź�������ݺ�һ��ת�Ƶ��½ڵ���ȥ����ȡ���м���
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

			tmp_record = current_pnode->data[i];		//���������ļ�¼
			new_node->child[0] = current_pnode->child[i+1];
			if(new_node->child[0] != NULL)
				new_node->child[0]->parent = new_node;
			
			current_pnode->data_num = M/2;			//�޸ı����ѿ���ԭ���������ݸ���
			new_node->data_num = M/2;				//�޸ı����ѿ����½������ݸ���
			tmp_right_pointer = new_node;		//����������ָ��


			//�����ѳɹ�֮��������
			//���current_pnodeû�и���㣬�Ǿ͸���root��㣻�����Ը����Ϊcurrent_pnode,������һ��ѭ��
			if(current_pnode->parent == NULL)
			{
				//�����µĸ���㣬������ȡ�ļ�¼װ���µĸ��ڵ���
				PBTNode new_root_node = new BTNode();
				new_root_node->data_num = 1;
				new_root_node->data[0] = tmp_record;

				current_pnode->parent = new_root_node;
				tmp_right_pointer->parent = new_root_node;

				new_root_node->child[0] = current_pnode;
				new_root_node->child[1] = tmp_right_pointer;
				
				root = new_root_node;//���¸����
				return true;	//����ɹ�                               
			} else {
				current_pnode = current_pnode->parent;
			}
		}
	}
}

bool BTree::search(Record &a_record)
{
	PBTNode current_pnode = root;
	
	//�Ƚ�����������Ҷ�ӽ����,current_pnodeΪӦ�������¼�¼��Ҷ�ӽ��
	while(1)
	{
		if(current_pnode->child[0] == NULL)
		{
			//����Ҷ�ӽ�㣬��Ҷ�ӽ�����ҵ�Ԫ��
			for(int i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *(a_record.word))
				{
					//�ҵ����ʣ�ȡ��������Ϣ����
					a_record.posting_list = current_pnode->data[i].posting_list;
					return true;
				}
			}

			return false; //������Ҷ�ӽ��Ҳû���ҵ�
		}
		else {
			//��ǰ��㲻��Ҷ�ӽ�㣬ͨ����С�ȽϽ�����һ������
			int i;
			for(i=0; i<current_pnode->data_num; i++)
			{
				if(*(current_pnode->data[i].word) == *(a_record.word))
				{
					//�ڷ�Ҷ�ӽ�����ҵ�Ԫ��
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