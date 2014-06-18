/*************************************************************************
	> File Name: tools.h
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月04日 星期三 17时10分50秒
 ************************************************************************/

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <my_types.h>

#define ERROR       -1
#define E_SIGN      1
#define NE_SIGN     2


/* 返回值为正，表示ff中的第几个，若<0 则绝对值表示字符的ASCII */
int get_first (const struct nes_table ff, const char *str);

/* 判断两个字符串是否相交，相交返回1,否则返回0 */
int is_strs_intersect (const char *str1, const char *str2);

/* 打印指定长度的横线 */
void print_line (int len, int cnt);

/* 判断字符串是终结符还是非终结符, 用index标记序号 */
int is_e_ne_sign (const struct ana_table an, const char *str, int *index);

#endif
