/*************************************************************************
	> File Name: analyse_stack.h
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月04日 星期三 22时55分57秒
 ************************************************************************/

#ifndef _ANALYSE_STACK_H_
#define _ANALYSE_STACK_H_

#include <my_types.h>

/* 分析栈 */
struct ana_stack {

    struct ne_sign *stack;
    int     top;
    int     cnt;
    int     size;
};

#define STACK_EMPTY     "the stack is empty!"
#define STR_ERROR       "input string have error!"
#define E_MATCH_S       "end sign matching success!"
#define E_MATCH_F       "end sign matching fail!"


/* 分析表和输入串 */
void analyse_stack (const struct ana_table an, const char *str);


typedef unsigned char   TYPE;
#define BOTHER          1
#define LCHILD          2

/* 打印时字节对齐 */
#define BYTE_ALIGN      6

/* 语法树的节点 */
struct tree_node {
    
    /* 存储语法树节点的字符串（终结符/非终结符） */
    struct ne_sign tree_val;
#define tval tree_val.val
    int len;
    /* 距离下一个兄弟的距离 */
#define dis len
    int c_width;
    /* 直接前驱 */
    struct tree_node *pre;
    /* 左孩子 */
    struct tree_node *LChild;
    /* 兄弟 */
    struct tree_node *bother;
};

#endif
