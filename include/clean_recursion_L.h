/*************************************************************************
	> File Name: clean_recursion_L.h
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Created Time: 2014年05月14日 星期三 15时53分32秒
	> blog: http://blog.csdn.net/zhanglpql
	> github: https://github.com/zhanglp92
 ************************************************************************/

#ifndef _CLEAN_RECURSION_L_H_
#define _CLEAN_RECURSION_L_H_

#include "my_types.h"



/* malloc 一段空间并赋值 */
#define MALLOC_AND_fill(node, type, src, dest, len) \
    do{    \
        node = malloc (sizeof ( type)); \
        memset (node, 0, sizeof ( type)); \
        if (0 < len)     \
            memcpy (src, dest, len); \
      }while (0)

/**
* @brief read_sentence : read sentence from file 
*
* @param FILE_name : sentence of file name 
* @param sentence : argument of sentence
*
* @return 
*/
int read_sentence (const char *FILE_name, struct sentence *sentence);

/**
* @brief destroy_sentence : remove a sentence
*
* @param s : sentence
*/
void destroy_sentence (struct sentence *s);

/**
* @brief show_prod : show sentence
*
* @param s : sentence
*/
void show_prod (struct sentence *s);

/**
* @brief clean_recursion_L : remove recursion  of left 
*
* @param sentence : sentence 
*
* @return 
*/
int clean_recursion_L (struct sentence *sentence);

/**
* @brief clean_direct_a_prod : remove direct 
*
* @param s : sentence
* @param i : index of sentence means productive
*/
void clean_direct_a_prod (struct sentence *s, size_t i);

/**
* @brief string_to_productive : string to productive
*
* @param s : sentence use deposit productive
* @param prod : productive
*
* @return 
*/
int string_to_productive (const char *s, struct productive *prod);

/**
* @brief add_productive_to_sentence : add a productive to sentence 
*
* @param s : sentence
* @param p : productive
*
* @return 
*/
int add_productive_to_sentence (struct sentence *s, const struct productive *p);

/**
* @brief clean_indirect : remove index
*
* @param s : sentence
*/
void clean_indirect (struct sentence *s); 

/**
* @brief combine_productive : comine tow productives
*
* @param s : sentence 
* @param i : index
* @param j : index
*/
void combine_productive (struct sentence *s, size_t i, size_t j);

/**
* @brief clean_recursion_sentence 
*
* @param FILE_name
* @param s
*
* @return 
*/
int clean_recursion_sentence (const char *FILE_name, struct sentence *s);

int get_e_ne_signs (struct sentence s, struct e_ne_signs *e_ne);

#define destroy_S(s)                destroy_sentence (s) 
#define str_2_PROD(s, prod)         string_to_productive (s, prod)
#define add_P_2_S(s, prod)          add_productive_to_sentence (s, prod)
#define clean_RCS_S(FILE_name, s)   clean_recursion_sentence (FILE_name, s) 


#endif

