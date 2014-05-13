
#include <raw_api.h>
#include <raw_api.h>
#include <rb_tree.h>
#include <assert.h>

#define  TEST_TASK_STK_SIZE (10 * 1024)

extern RAW_U8 test_started_raw;
static RAW_TASK_OBJ test_rb_obj;
static PORT_STACK test_rb_stack[TEST_TASK_STK_SIZE];


struct data
{
	struct rb_node node;
	RAW_S32 data;
};

static struct rb_root root = {0};

static struct data* find_data(struct rb_node* p_top, RAW_S32 val)
{
	struct data* p_data;
	struct rb_node* p_node;

	p_node = p_top;
	
	while(p_node){
		p_data = rb_entry(p_node, struct data, node);
		
		if(val < p_data->data){
			p_node = p_node->rb_left;
		}else if(val > p_data->data){
			p_node = p_node->rb_right;
		}else{
			return p_data;
		}
	}
	
	return NULL;
}

struct data* _insert_data(struct data* p_node_data, RAW_S32 val, struct rb_root* p_root)
{
	struct rb_node** p = &p_root->rb_node;
	struct rb_node* parent = NULL;
	struct data* p_data;

	while (*p)
	{
		parent = *p;
		p_data = rb_entry(parent, struct data, node);

		if (val < p_data->data){
			p = &(*p)->rb_left;
		}else if (val > p_data->data){
			p = &(*p)->rb_right;
		}else{
			return p_data;
		}
	}

	rb_link_node(&p_node_data->node, parent, p);
	return NULL;
}

static struct data* insert_data(struct data* p_node_data, RAW_S32 val, struct rb_root* p_root)
{
	struct data* ret;
	
	ret = _insert_data(p_node_data, val, p_root);
	if(ret){
		goto final;
	}
	
	rb_insert_color(&p_node_data->node, p_root);
	
final:
	return ret;
}

void test_rb_task(void * pParam)
{

	struct data data_1 = {{0}, 1};
	struct data data_2 = {{0}, 2};
	struct data data_3 = {{0}, 3};
	struct data data_4 = {{0}, 4};
	struct data data_5 = {{0}, 5};

	insert_data(&data_1, data_1.data, &root);
	insert_data(&data_2, data_2.data, &root);
	insert_data(&data_3, data_3.data, &root);
	insert_data(&data_4, data_4.data, &root);
	insert_data(&data_5, data_5.data, &root);
	
	RAW_ASSERT(&data_1 == find_data(root.rb_node, 1));
	RAW_ASSERT(&data_2 == find_data(root.rb_node, 2));
	RAW_ASSERT(&data_3 == find_data(root.rb_node, 3));
	RAW_ASSERT(&data_4 == find_data(root.rb_node, 4));
	RAW_ASSERT(&data_5 == find_data(root.rb_node, 5));
	
	rb_erase(&data_1.node, &root);
	rb_erase(&data_2.node, &root);
	rb_erase(&data_3.node, &root);
	rb_erase(&data_4.node, &root);
	rb_erase(&data_5.node, &root);
	
	RAW_ASSERT(NULL == find_data(root.rb_node, 1));
	RAW_ASSERT(NULL == find_data(root.rb_node, 2));
	RAW_ASSERT(NULL == find_data(root.rb_node, 3));
	RAW_ASSERT(NULL == find_data(root.rb_node, 4));
	RAW_ASSERT(NULL == find_data(root.rb_node, 5));
	RAW_ASSERT(NULL == root.rb_node);


	while (1);

}



RAW_VOID rbtree_test()
{


	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	raw_task_create(&test_rb_obj, (RAW_U8	*)"task2", 0,
						 10, 0,  test_rb_stack, 
						 TEST_TASK_STK_SIZE , test_rb_task, 1); 

	
}
