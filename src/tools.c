/*************************************************************************
	> File Name: tools.c
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月04日 星期三 17时09分39秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <tools.h>

/* 返回值为正，表示ff中的第几个，若<0 则绝对值表示字符的ASCII */
int get_first (const struct nes_table ff, const char *str)
{
    int     rs = 0;
    int     i, max, len;
    
    for (max = i = 0; i < ff.cnt; i++) {

        len = strlen (ff.table[i].ne_sign);
        if (max < len && 0 == strncmp (str, ff.table[i].ne_sign, len)) {
        
            max = len;
            rs = i;
        }
    }
    if (0 == max) 
        rs = -str[0];

    return rs;
}

/* 判断两个字符串是否相交，相交返回1,否则返回0 */
int is_strs_intersect (const char *str1, const char *str2)
{
    unsigned char buf[256] = {0};
    while ( *str1 ) buf[*str1++] = 1;

    while ( *str2 ) {

        if ( buf[*str2++] ) 
            return 1;
    }

    return 0;
}

/* 打印指定长度的横线 */
void print_line (int len, int cnt)
{
    int size = (len + 2) * cnt;
    char buf[size + 1];

    memset (buf, '-', size);
    buf[size] = 0;
    printf ("%s\n", buf);
}

/* 判断字符串是终结符还是非终结符, 用index标记序号 */
int is_e_ne_sign (const struct ana_table an, const char *str, int *index) 
{
    *index = ERROR;

    int     i = 1, len = strlen (str);
    /* 记录字符在终结符字符串的位置 */
    static char buf[256] = {1,};
    if ( 1 == buf[0] ) {
    
        buf[0] = 0;
        char    *t = an.signs->e_sign;
        while ( *t )
            buf[*t++] = i++;
    }
 
    /* 为终结符 */
    if ( 1 == len && buf[*str] ) {
        
        *index = buf[*str]-1;
        return E_SIGN;
    } else {
        
        /* 为非终结符 */
        for (i = 0; i < an.signs->rows; i++) {
            
            char *temp = an.signs->ne_sign[i].val;
            if (strlen (temp) == len && !strncmp (temp, str, len+1)) {
                
                *index = i;
                return NE_SIGN;
            }
        }
    }

    return ERROR;
}
