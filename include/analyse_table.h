/*************************************************************************
	> File Name: analyse_table.h
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月01日 星期日 21时38分50秒
 ************************************************************************/

#ifndef _ANALYSE_TABLE_H_
#define _ANALYSE_TABLE_H_

#include <my_types.h>

int analyse_table (struct sentence s, struct nes_table ff, struct ana_table *an);
void show_ana_table (const struct ana_table an);
void destroy_T (struct ana_table *an);
void destroy_E_NE (struct e_ne_signs *e_ne);

#endif 
