/*************************************************************************
	> File Name: first_follow.c
	> Author: zhanglp
	> Mail: zhanglp92.gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Git: http://github.com/zhanglp92 
	> Created Time: 2014年05月15日 星期四 18时48分44秒
 ************************************************************************/

#include <stdio.h>
#include <first_follow.h>
#include <string.h>
#include <stdlib.h>


int first_follow (const struct sentence *s, struct nes_table *ff)
{
    not_end_sign (s, ff);

    first_set (s, ff);

    follow_set (s, ff);
}

void remove_replace (char *str) 
{
    int     i, pos = 0;
    unsigned char    buf[256] = {0};

    for (i = 0; str[i]; i++) {

        if (1 == (buf[str[i]] += 1))
            str[pos++] = str[i];
    }
    memset (str+pos, 0, strlen (str));

/*    for (pos = i = 0; i < 256; i++) 
        if ( buf[i] ) 
            str[pos++] = i;
            */
}

int not_end_sign (const struct sentence *s, struct nes_table *ff)
{
    ff->table = malloc (sizeof (struct ff_node) * s->cnt);
    memset (ff->table, 0, sizeof (struct ff_node) * s->cnt);
    ff->cnt = ff->size = s->cnt;

    int     i;
    for (i = 0; i < s->cnt; i++) {

        strncpy (ff->table[i].ne_sign, s->prod[i].head->val, \
                 strlen (s->prod[i].head->val));
    }
}

int first_set (const struct sentence *s, struct nes_table *ff)
{
    int     i;

    for (i = 0; i < ff->cnt; i++) {
        
        first (s, ff, i);
    }
}

int get_index (const struct nes_table *ff, char *ne_s) 
{
    int     i, len = 0, l, index = -1;


    for (i = 0; i < ff->cnt; i++) {

        l = strlen (ff->table[i].ne_sign);
        if (len < l && !strncmp (ff->table[i].ne_sign, ne_s, l)) {

            index = i;
        }
    }

    return index;
}

void first (const struct sentence *s, struct nes_table *ff, size_t i)
{
    if (get_first (ff->table[i])) 
        return ;

    struct node     *p = s->prod[i].head->next;
    int     index, k;

    for (p; p; p = p->next) {
    
        index = get_index (ff, p->val);
        if (-1 == index) {

            /* 不加如重复的终结符 */
            for (k = 0; ff->table[i].first[k] && ff->table[i].first[k] != p->val[0]; k++) ;
            if (0 == ff->table[i].first[k]) 
                ff->table[i].first[k] = p->val[0];
        }else {
            
            first (s, ff, index);
            strncpy (ff->table[i].first, ff->table[index].first, \
                     strlen (ff->table[index].first));
        }
    }
    set_first_1 (ff->table[i]);
}

int follow_set (const struct sentence *s, struct nes_table *ff)
{
    int     i, index;
    struct node *p;

    /* 文法的开始符号加入# */
    ff->table[0].follow[0] = '#';

    for (i = 0; i < s->cnt; i++)

        follow (s, ff, i);

    /* 填写follow 中未填写完的 */
    for (i = 0; i < s->cnt; i++)

        follow (s, ff, i);

    /* 去除follow 集中的重复 */
    for (i = 0; i < ff->cnt; i++) 

        remove_replace (ff->table[i].follow);

    return SUCCESS;
}

/* yes 1, no 0  A=>@ */ 
int is_get_null (const struct sentence *s, const struct nes_table *ff, size_t index)
{
    int     i = index;
    /* 判断由此非终结符，能不得到空串 */
    char    *buf = ff->table[index].ne_sign;
    int     len = strlen (buf);

    if (0 != strncmp (s->prod[index].head->val, buf, len)) 

        for (i = 0; i < s->size; i++) {
        
            if (0 == strncmp (s->prod[i].head->val, buf, len)) 
                break;
        }

    if (i != s->size) {
        
        struct node *p = s->prod[i].head->next;
        for (p; p; p = p->next) {
            
            if (0 == strncmp (p->val, EMPTY_SET, strlen (p->val))) 
                return 1;
        }
    }

    return 0;
}

/* 判断的是一条产生式 */
void follow (const struct sentence *s, struct nes_table *ff, size_t i)
{
    struct node *head = s->prod[i].head;
    struct node *p = head->next;
    int     pos, index, cur, len;

    for (p; p; p = p->next) {
        
        for (pos = 0; p->val[pos]; ) {

            /* 当前判断的非终结符，B */
            cur = index = get_index (ff, p->val+pos); 
            /* 是非终结符 */
            if (-1 != index) {

                /* 非终结符后的下一个字符的位置 */
                pos += strlen (ff->table[index].ne_sign);
                /* 如B的后边没有字符，则加入follow A 到 follow B */
                if (0 == p->val[pos]) {

                    int i = get_index (ff, head->val);
                    strncat (ff->table[cur].follow, ff->table[i].follow, strlen (ff->table[i].follow));
                }else {
                    
                    /* 当前非终结符后的符号 */
                    index = get_index (ff, p->val + pos);
                    /* 是终结符 */
                    if (-1 == index) {

                        strncat (ff->table[cur].follow, p->val+pos, 1);
                        pos++;
                    }else {
                        
                        /* 添加出去空元素的 first 集 */
                        char    *buf = ff->table[index].first;
                        int     pos = 0, k = strlen (ff->table[cur].first);

                        for (pos = 0; buf[pos]; pos++) {
                            
                            if (EMPTY_ALP != ff->table[index].first[pos])

                                ff->table[cur].follow[k++] = ff->table[index].first[pos];
                        }
                        
                        /* 添加 follow */
                        if (1 == is_get_null (s, ff, index)) {

                            int i = get_index (ff, head->val);
                            strncat (ff->table[cur].follow, ff->table[i].follow, strlen (ff->table[i].follow));
                        }
                    }
                }
            }else pos++;
        }
    }
}

void show_ff (const struct nes_table *ff) 
{
    int     i;

    printf ("first 和 follow 集 \n");
    printf ("--------------------------------------------------\n");
    printf ("| %-8s  | %-16s | %-16s | \n", "", "first set", "follow set");
    printf ("--------------------------------------------------\n");
    for (i = 0; i < ff->cnt; i++) {

        printf ("| %-8s  | %-16s | %-16s | \n", ff->table[i].ne_sign,  \
                ff->table[i].first, ff->table[i].follow);
        printf ("--------------------------------------------------\n");
    }
}

void destroy_ff (struct nes_table *ff) 
{
    free (ff->table);
    ff->table = NULL;
}
