/*************************************************************************
	> File Name: analyse_table.c
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月01日 星期日 21时38分14秒
 ************************************************************************/

#include <analyse_table.h>
#include <string.h>
#include <stdlib.h>
#include <tools.h>

/* 添加一条记录到分析表中 */ 
static void add_ana_table (const struct ana_table *an,int row,char e,const void *row_add,const void *col_add,struct nes_table ff)
{
    /* 若e为空字符，则吧e属于follow(A)把产生式加入至M[A, e]中*/
    if (EMPTY_ALP == e) {

        char *buf = ff.table[row].follow;
        while ( *buf ) 
            add_ana_table (an, row, *buf++, row_add, col_add, ff);
        return ;
    }

    /* 若e终结符，则把产生式加入至M[A, e]中*/
    int     col;
    for (col = 0; col < an->signs->cols; col++) {

        if (e == an->signs->e_sign[col])
            break ;
    }

    /* 添加 sentence 表中的地址 */
    an->table[row * an->signs->cols + col].row_add = (struct node*)row_add;
    an->table[row * an->signs->cols + col].col_add = (struct node*)col_add;
}

/* 添加同步位 */
void add_synch (struct ana_table *an, int row, const char *str)
{
    char *buf = an->signs->e_sign;
    char temp[255] = {0};
    int     col = 0;

    while ( *str ) 
        temp[*str++] = 1;

    while ( *buf ) {

        /* 给所有的follow 集加入synch */
        if ( temp[*buf++] ) 
            an->table[row * an->signs->cols + col].err = SYNCH;
        col++;

    }
}

int analyse_table (struct sentence s, struct nes_table ff, struct ana_table *an)
{
    int     i, j, k;
    an->table = malloc (sizeof (struct anat_node)*an->signs->rows * an->signs->cols);
    memset (an->table, 0, sizeof (struct anat_node)*an->signs->rows * an->signs->cols);


    struct node *head, *p;

    for (i = 0; i < s.cnt; i++) {

        p = head = s.prod[i].head;
        add_synch (an, i, ff.table[i].follow);
        while ( (p = p->next) ) {

            /* get first 集 */
            int index = get_first (ff, p->val);
            if (0 <= index) {
                
                char    *buf = ff.table[index].first;
                while ( *buf )
                    add_ana_table (an, i, *buf++, head, p, ff);

            }else {
    
                add_ana_table (an, i, (char)(-index), head, p, ff);
            }
        }
    }
}

void show_ana_table (const struct ana_table an) 
{
    int     i, j;
    char    *buf;

    printf ("分析表 \n");
    print_line (10, an.signs->cols + 1);
    buf = an.signs->e_sign;
    printf ("| %-10s", "");
    while ( *buf ) printf ("| %-10c", *buf++);
    printf ("|\n");
    print_line (10, an.signs->cols + 1);

    for (i = 0; i < an.signs->rows; i++) {

        printf ("| %-10s", an.signs->ne_sign[i].val);
        for (j = 0; j < an.signs->cols; j++) {
    
            char temp[NODE_MAX] = "";
            char *buf1 = an.table[i * an.signs->cols + j].row_add->val;
            char *buf2 = an.table[i * an.signs->cols + j].col_add->val;

            if (buf1 && buf2)
                sprintf (temp, "%s->%s", buf1, buf2);
            else if (SYNCH == an.table[i * an.signs->cols + j].err)
                sprintf (temp, "%s", "synch");
            else 
                sprintf (temp, "%s", "");

            printf ("| %-10s", temp);
        }
        printf ("|\n");
        print_line (10, an.signs->cols + 1);
    }
}

void destroy_T (struct ana_table *an)
{
    /* 释放分析表空间 */
    free (an->table);
}

void destroy_E_NE (struct e_ne_signs *e_ne)
{
    /* 释放存储非终结符的空间 */
    free (e_ne->ne_sign);
}
