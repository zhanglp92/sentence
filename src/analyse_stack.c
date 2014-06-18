/*************************************************************************
	> File Name: analyse_stack.c
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月04日 星期三 22时55分33秒
 ************************************************************************/

#include <stdio.h>
#include <analyse_stack.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <tools.h>

static int push_stack (struct ana_stack *s, const char *str)
{
    /* 扩展表的长度 */
    if (s->cnt >= s->size) {
        
        s->size = (s->size + 1) << 1;
        if ( NULL == (s->stack = realloc \
            (s->stack, sizeof (struct node) * s->size + 1)) )
                return ALLOC_F;
    }
    memset (s->stack+s->cnt, 0, sizeof (struct ne_sign) * (s->size - s->cnt));

    /* 入栈 */
    strncpy (s->stack[s->cnt].val, str, strlen (str));
    s->top = s->cnt++;

    return SUCCESS;
}

static void pop_stack (struct ana_stack *s)
{
    if (0 >= s->cnt) 
        return ;

    memset (s->stack[s->top].val, 0, sizeof (struct ne_sign));
    s->top -= 1;
    s->cnt -= 1;
}

static void display (struct ana_stack s, const char *str, const char *note)
{
    #define MAX_LEN     30
    int     i, t, len = MAX_LEN;
    char    buf[NODE_MAX+1] = {0};

    for (i = 0; i < s.cnt; i++) {

        t = strlen (s.stack[i].val);
        snprintf (buf+MAX_LEN-len, t+1, "%s", s.stack[i].val);
        len -= t;
    }
    memset (buf+(MAX_LEN - len), ' ', len);
    printf ("| %s ", buf);

    memset (buf, 0, MAX_LEN+1);
    t = strlen (str);
    memset (buf, ' ', MAX_LEN - t);
    strncat (buf, str, t);
    printf ("|%s ", buf);

    printf ("| %s \n", note);

    memset (buf, '-', (MAX_LEN+2) * 2);
    printf (" %s\n", buf);
    #undef MAX_LEN
}

static const char* get_top (const struct ana_stack s)
{
    return 0 >= s.cnt ? NULL : s.stack[s.top].val;
}

static int add_node (struct tree_node **tree, TYPE who, const char *str);

/* 返回附注信息 */
static const char* analyse_str (struct ana_stack *s, struct ana_table an, char e, int *pos, struct tree_node **tree)
{
    /* 返回附注信息 */
    static char str[NODE_MAX];

    int index_t, rs_t, index_i, rs_i;
    const char *top = get_top (*s);

    /* 栈为空的清况 */
    if (NULL == top) 
        return STACK_EMPTY;

    /* 栈顶和输入字符在分析表的位置 */
    rs_t = is_e_ne_sign (an, top, &index_t);
    char    ch[2] = {e, 0}; /* 将字符变成字符串 */
    rs_i = is_e_ne_sign (an, ch, &index_i);

    /* 输入字符串有不能识别的字符, 则跳过该字符 */ 
    if (ERROR == rs_i || NE_SIGN == rs_i) { 

        *pos += 1;
        return STR_ERROR;
    }

    /* 终结符匹配，成功则出栈 */ 
    else if (E_SIGN == rs_t) {

        if (index_t == index_i) { 

            *pos += 1; 
            pop_stack (s); 
#if 1
            /* 找到对应的栈顶元素 */
            while ( (*tree)->pre ) {

                if( (*tree)->bother ) {

                    *tree = (*tree)->bother;
                    break;
                } else *tree = (*tree)->pre;
            }
            
#endif
        //    return E_MATCH_S;
            return "";
        } else return E_MATCH_F;
    }
    /* 根据分析表中的内容判断 */
    else {
    
        struct anat_node node = an.table[index_t * an.signs->cols + index_i]; 

        /* 分析表中有此项, 出栈，产生式的左部倒序入栈 */
        if ( node.row_add && node.col_add) { 
        
            /* 得到产生式的右部,备份到 str 字符串中*/
            char str[NODE_MAX] = {0};
            int  len = strlen (node.col_add->val), pos = len - 1, index;
            strncpy (str, node.col_add->val, len);
            pop_stack (s);

            /* 产生式的右部为空串 */
            if ( !strncmp (str, EMPTY_SET, len) ) {
#if 1
                /* 添加空串到叶子节点 */
                if (0 > add_node (tree, LCHILD, EMPTY_SET))
                    printf ("tree error ! \n");

                /* 找到对应的栈顶元素 */
                while ( (*tree)->pre ) {

                    if( (*tree)->bother ) {
                        
                        *tree = (*tree)->bother;
                        break;
                    }else *tree = (*tree)->pre;
                }
#endif
                return "";
            }

            /* 子树 */
            while ( 0 <= pos && 0 < len ) {
                
                str[len] = 0;
                if (ERROR != is_e_ne_sign (an, str+pos, &index)) {
#if 1
                    /* 添加一层叶子节点 */
                    if (0 > add_node (tree, LCHILD, str+pos))
                        printf ("tree error ! \n");
#endif
                    push_stack (s, str+pos);
                    len = pos;
                }
                pos--;
            }
            /* 让树根指向栈顶 */
            *tree = (*tree)->LChild; 
            return "";
        }/* 有同步标志，处理后出栈 */
        else if (SYNCH == node.err) {
            
            /* 当栈里只有一个非终结符时，则跳过输入字符 */
            if ( 2 == s->cnt ) {

                sprintf (str, "error, jump %c", e);
                *pos += 1;
                return str;
            }

            sprintf (str, "error, M[%s, %c]=synch", get_top (*s), e);
            /* 出错不需要加入语法树中 */
            pop_stack (s);
            return str;
        }/* 错误则出栈 */
        else {
            
            *pos += 1;
            sprintf (str, "error, M[%s, %c]=error", get_top (*s), e);
            return str;
        }
    }

    return NULL;
}

static int add_node (struct tree_node **tree, TYPE who, const char *str)
{
    /* 申请节点 */
    struct tree_node *node, *pos;
    if (NULL == (node = malloc (sizeof (struct tree_node))))
        return -1;
    memset (node, 0, sizeof (struct tree_node));

    node->len = strlen (str);
    strncpy (node->tval, str, node->len);
 
    /* 树根 */
    if (NULL == *tree) {

        *tree = node;
    }
    /* 判断是兄弟还是孩子 */
    else if (LCHILD == who) {

        /* 将孩子添加到兄弟的最左边，每个孩子都指向下一个兄弟和父亲 */
        node->pre = *tree;
        node->bother = (*tree)->LChild;
        (*tree)->LChild = node;
    }else if (BOTHER == who) {
        
        /* 添加一个兄弟节点 */
        node->pre = (*tree)->pre;
        node->bother = *tree;
        (*tree)->pre->LChild = node;
    }else {
        
        free (node);
        return -1;
    }

    return 0;
}

static void destroy_tree (struct tree_node *tree);
static void display_tree (struct tree_node *tree);
/* 分析表和输入串 */
void analyse_stack (const struct ana_table an, const char *str)
{
    struct ana_stack ana_stack = {0};
    /* 将#,和文法的开始符号加入栈低 */
    push_stack (&ana_stack, "#");
    push_stack (&ana_stack, an.signs->ne_sign[0].val);

    /* 语法树根 */
    struct tree_node *tree = NULL, *tree_pos;
    if (0 > add_node (&tree, 0, an.signs->ne_sign[0].val))
        printf ("tree error ! \n");
    tree_pos = tree;

    /* 表示扫描串的位置 */
    int pos = 0, len = strlen (str);
    /* 栈为空或字符串匹配完则结束 */
    display (ana_stack, str, "");
    while (0 < ana_stack.cnt && pos < len && strncmp (get_top (ana_stack), "#", 2)) {

        const char *buf = analyse_str (&ana_stack, an, str[pos], &pos, &tree_pos);
        display (ana_stack, str+pos, buf);
    }

    display_tree (tree);
    destroy_tree (tree);
}

/* w 为父亲的宽度，求中间节点前边的节点个数 */
static int cal_distance (struct tree_node *tree, int w) 
{
    struct tree_node *pos = tree;
    while ( pos ) {

        pos->c_width = w;
        w = cal_distance (pos->LChild, pos->c_width);
        pos = pos->bother;
        if ( pos ) w += 1;
    }

    return w;
}

/* 打印语法树，借助队列 */
static void display_tree (struct tree_node *tree) 
{
    if ( NULL == tree ) 
        return ;
    
    cal_distance (tree, 0);

    /* 队列结构体 */
    struct my_queue {

        struct tree_node *addr;
        struct my_queue *next;
    };

    /* 空间可以重复利用，等使用完后在总体释放 */
    #define add_ele(head, tail, tnode) \
        do { \
            if ( NULL == tail ) { \
                head = tail = \
                    malloc (sizeof (struct my_queue)); \
                tail->next = head; \
            }else if ( head == tail->next ) { \
                struct my_queue *node; \
                node = malloc (sizeof (struct my_queue)); \
                node->next = head; \
                tail = tail->next = node; \
            }else \
                tail = tail->next; \
            tail->addr = tnode; \
        }while (0)

    /* 将树根加入到队列中 */
    struct my_queue *head = NULL, *mid = NULL, *tail = NULL, *t;
    add_ele (head, tail, tree);
    mid = head;

    char    temp[100] = {0};
    int cnt = 0, i;
    while ( 1 ) {
#if 1
        char *val = head->addr->tval;
        int len = strlen (val);

        memset (temp, 0, 100);
        memset (temp, ' ', (head->addr->c_width - cnt)*BYTE_ALIGN);
        strncat (temp, val, len);
        memset (temp+strlen(temp), ' ', BYTE_ALIGN > len ? BYTE_ALIGN - len : 0);
        printf ("%s", temp);
        cnt = head->addr->c_width + 1;
#endif
        
        struct tree_node *pos = head->addr->LChild;
        while ( pos ) {

            /* 所有兄弟都进栈 */
            add_ele (head, tail, pos);
            pos = pos->bother;
        }

        /* 一层结束 */
        if (head == mid) {
            
            printf ("\n\n");
            cnt = 0;
            mid = tail;
        }

        /* 头追上尾则结束 */
        if (head == tail)
            break;
        head = head->next;
    }

    /* 销毁链表 */
    do {
        
        t = head;
        head = head->next;
        free (t);
    }while ( head != tail );

    #undef add_ele
}

/* 销毁语法树 */
static void destroy_tree (struct tree_node *tree)
{
    if (NULL == tree) 
        return ;

    destroy_tree (tree->LChild);
    destroy_tree (tree->bother);

//    printf ("val = %s, len = %ld \n", tree->tval, tree->len);
    free (tree);
    tree = NULL;
}
