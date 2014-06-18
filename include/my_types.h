/*************************************************************************
	> File Name: my_types.h
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2014年05月13日 星期二 10时52分50秒
 ************************************************************************/

#ifndef _MY_TYPES_H_
#define _MY_TYPES_H_

#include <stdio.h>

/* ＠表示空集 */
#define EMPTY_ALP   '@'
#define EMPTY_SET   "@"

#define TRUE    1
#define FALSE   0

#define SUCCESS      1
#define ALLOC_F     -1
#define PROD_F      -2

/* 一个节点上字符串的最大长度 */
#define NODE_MAX    256

struct ne_sign {

    char val[NODE_MAX>>2];
};
/* 存储所有的非终结符和终结符 */
struct e_ne_signs {
   
    /* 非终结符结构体及它的个数 */
    struct ne_sign  *ne_sign;
    size_t  rows;

    /* 终结符结构体及它的个数 */
    char    e_sign[NODE_MAX];
    size_t  cols;
};


/* ================================================================== 
 * 如下是文法的结构体
 * ================================================================== */
/*
 * 产生式上的一个单位的结构体
 * 如：P->Qa|Sc|d
 * 此结构体用来存储 P, Qa, Sc, d
* */
struct node {

    char        val[NODE_MAX];
    struct node *next;
};

/* 表示一条产生式 */
struct productive {
  
    struct node *head;
    /* 表示产生式的长度,如上个产生式的长度是４ */
    unsigned    length;
};

/* 文法 */
struct sentence {
    
    struct productive   *prod;
    /* 产生式的实际长度 */
    unsigned    cnt;
    /* 产生式表的长度 */
    unsigned    size;
};
/* ================================================================== */

/* ==================================================================
 * 下面是非终结符的First follow 集合的数据结构
 * ================================================================== */
struct ff_node {
    
    /* 非终结符少的多，所以右移两位 */
    char    ne_sign[NODE_MAX>>2];
    char    first[NODE_MAX];
    char    follow[NODE_MAX];
    /* 分别用最低位，次低位表示first, follow 集合有没有求完（1完） */
    unsigned char   f;
};

struct nes_table {
    
    struct ff_node  *table;
    size_t  cnt;
    size_t  size;
};
/* ================================================================== */


/* ==================================================================
 * 下面是分析表的结构
 * ================================================================== */
 /* 分析表中的一个节点类型 */
 #define SYNCH  1
 struct anat_node {
    
    struct node *row_add;   /* 产生式表中的第几条 */
    struct node *col_add;   /* 对应产生式中那的第几个 */
    unsigned char   err;    //
 };
 
 /* 分析表结构体 */
 struct ana_table {
    
    struct anat_node    *table;
    struct e_ne_signs   *signs;
 };
/* ================================================================== */

#endif
