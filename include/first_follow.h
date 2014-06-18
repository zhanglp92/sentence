/*************************************************************************
	> File Name: first_follow.h
	> Author: zhanglp
	> Mail: zhanglp92.gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Git: http://github.com/zhanglp92 
	> Created Time: 2014年05月15日 星期四 18时49分41秒
 ************************************************************************/

#ifndef _FIRST_FOLLOW_H_
#define _FIRST_FOLLOW_H_

#include <my_types.h>


/**
 * @brief first_follow : get first and follow set from sentence
 *
 * @param s : sentence
 * @param ff : first and follow set 
 *
 * @return 
 */
int first_follow (const struct sentence *s, struct nes_table *ff);

/**
 * @brief remove_replace : remove replace alpha from a string 
 *
 * @param str : string 
 */
void remove_replace (char *str);

/**
 * @brief not_end_sign : get a sign that is not end sigin from sentence
 *
 * @param s : sentence
 * @param ff : first and follow set 
 *
 * @return 
 */
int not_end_sign (const struct sentence *s, struct nes_table *ff);

/**
 * @brief first_set : get first set
 *
 * @param s : sentence
 * @param ff : first and follow set 
 *
 * @return 
 */
int first_set (const struct sentence *s, struct nes_table *ff);

/**
 * @brief get_index : get inde of a sign that is not end sign in first and follow set
 *
 * @param ff : 
 * @param ne_s : string 
 *
 * @return 
 */
int get_index (const struct nes_table *ff, char *ne_s);

/**
 * @brief first : get a sign that is not end sign
 *
 * @param s : sentence
 * @param ff : 
 * @param i : index in the sentence 
 */
void first (const struct sentence *s, struct nes_table *ff, size_t i);

/**
 * @brief follow_set : 
 *
 * @param s : 
 * @param ff : 
 *
 * @return 
 */
int follow_set (const struct sentence *s, struct nes_table *ff);

/**
 * @brief is_get_null : decide A => ... | @ 
 *
 * @param s : sentence
 * @param ff
 * @param index
 *
 * @return 
 */
int is_get_null (const struct sentence *s, const struct nes_table *ff, size_t index);

/**
 * @brief follow : 
 *
 * @param s
 * @param ff
 * @param i
 */
void follow (const struct sentence *s, struct nes_table *ff, size_t i);

void show_ff (const struct nes_table *ff);
void destroy_ff (struct nes_table *ff);

#define destroy_F(ff)   destroy_ff (ff)

#define set_first_1(ff_node) \
    (ff_node.f |= 0x02)
#define set_first_0(ff_node) \
    (ff_node.f &= 0xfd) 

#define set_follow_1(ff_node) \
    (ff_node.f |= 0x02)
#define set_follow_0(ff_node) \
    (ff_node.f &= 0xfd) 

#define get_first(ff_node) \
    ((ff_node.f & 0x02) >> 1)
#define get_follow(ff_node) \
    ((ff_node.f & 0x02) >> 1)

#endif
