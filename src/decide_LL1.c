/*************************************************************************
	> File Name: decide_LL1.c
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月04日 星期三 17时06分26秒
 ************************************************************************/

#include <stdio.h>
#include <decide_LL1.h>
#include <tools.h>
#include <string.h>

/* 判断是否为LL1文法, 是LL1文法返回1,否则返回0 */
int decide_LL1 (const struct sentence s, const struct nes_table ff)
{
    int     i;
    struct node *p, *q;

    /* 判断每个产生式中的候选首集是否有交集 */
    for (i = 0; i < s.cnt; i++) {

        for (p = s.prod[i].head->next; p; p = p->next) {
            
            /* 判断包含空集的非终结符的first follow 集是否有交集 */
            if ( !strncmp (p->val, EMPTY_SET, strlen (EMPTY_SET)) ) 
                if ( is_strs_intersect (ff.table[i].first, ff.table[i].follow) )
                    return 0;

            for (q = p->next; q; q = q->next) {

                /* 判断包含空集的非终结符的first follow 集是否有交集 */
                if ( !strncmp (q->val, EMPTY_SET, strlen (EMPTY_SET)) ) 
                    if ( is_strs_intersect (ff.table[i].first, ff.table[i].follow) )
                        return 0;

                int index1 = get_first (ff, p->val);
                int index2 = get_first (ff, q->val);

                if (index1 == index2) 
                    return 0;

                else {
                    
                    char buf1[NODE_MAX] = {0}, *f1 = ff.table[index1].first;
                    char buf2[NODE_MAX] = {0}, *f2 = ff.table[index2].first;

                    0 > index1 ? snprintf (buf1, 1, "%c", (char)(-index1)) :
                                 snprintf (buf1, strlen (f1), "%s", f1);
                    0 > index2 ? snprintf (buf2, 1, "%c", (char)(-index2)) :
                                 snprintf (buf2, strlen (f2), "%s", f2);

                    /* 判断候选首符集是否相交 */
                    if ( is_strs_intersect (buf1, buf2) )
                        return 0;
                }
            }
        }
    }

    return 1;
}
