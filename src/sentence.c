/*************************************************************************
	> File Name: sentence.c
	> Author: zhanglp
	> Mail: zhanglp92@gmail.com 
	> Created Time: 2014年05月14日 星期三 15时58分19秒
	> blog: http://blog.csdn.net/zhanglpql
	> github: https://github.com/zhanglp92
 ************************************************************************/

#include <stdio.h>
#include <sentence.h>
#include <string.h>

/* argv[1]表示文法，argv[2]表示句子 */
int main (int argc, const char **argv)
{
    if ( 2 != argc ) {

        fprintf (stderr, "argument error ! \n");
        return 0;
    }

    const char *SEN_name = argv[1];
//    const char *JU_name = argv[2];
    struct sentence     sentence;
    struct nes_table    ff;
    struct e_ne_signs   e_ne_signs;
    struct ana_table    ana_table;

    /* 消除左递归 */
    if (FALSE == clean_RCS_S (SEN_name, &sentence))
        return 0;
    get_e_ne_signs (sentence, &e_ne_signs);
    show_prod (&sentence);

    /* 构造first follow 集 */
    first_follow (&sentence, &ff);
    show_ff (&ff);

    if (0 == decide_LL1 (sentence, ff)) {

        printf ("此文不是LL1文法 \n");
        goto end2;
    }

    /* 构造分析表 */
    ana_table.signs = &e_ne_signs;
    analyse_table (sentence, ff, &ana_table);
    show_ana_table (ana_table);


    /* 分析输入串 */
    char JU_name[NODE_MAX];
    printf ("input words (\"#\" is end) : \n");
    while ( scanf ("%s", JU_name) && strncmp (JU_name, "#", 2) ) {

        analyse_stack (ana_table, JU_name);
        printf ("input words (\"#\" is end) : \n");
    }

end1:
    destroy_T (&ana_table);
    destroy_E_NE (&e_ne_signs);

end2:
    destroy_F (&ff);
    destroy_S (&sentence);
    return 0;
}

