/*************************************************************************
	> File Name: decide_LL1.h
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Github: http://github.com/zhanglp92 
	> Created Time: 2014年06月04日 星期三 17时28分37秒
 ************************************************************************/

#ifndef _DECIDE_LL1_H_
#define _DECIDE_LL1_H_

#include <my_types.h>

/* 判断是否为LL1文法, 是LL1文法返回1,否则返回0 */
int decide_LL1 (const struct sentence s, const struct nes_table ff);

#endif

