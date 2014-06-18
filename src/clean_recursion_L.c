/*************************************************************************
	> File Name: clean_recursion_L.c
	> Author: zhanglp
	> Mail: zhanglp92.gmail.com 
	> Blog: http://blog.csdn.net/zhanglpql 
	> Git: http://github.com/zhanglp92 
	> Created Time: 2014年05月15日 星期四 11时21分03秒
 ************************************************************************/

#include <stdio.h>
#include <clean_recursion_L.h>
#include <stdlib.h>
#include <string.h>

/* 得到所有的终结符 */
void get_e_signs (struct sentence s, char *dst, const char *src)
{
    int     i, max, k, pos = strlen (dst);
    struct node *head;

    while ( *src ) {

        /* 避开非终结符 */
        for (max = 0, i = 0; i < s.cnt; i++) {

            char    *str = s.prod[i].head->val;
            int     len = strlen (str);
            if (max < len && 0 == strncmp (src, str, len)) {
                k = i;
                max = len;
            }
        }
        if ( max ) {

            src += strlen (s.prod[k].head->val);
            continue;
        }

        /* 避开重复的终结符 */
        for (i = 0; i < pos; i++) {
            
            if (dst[i] == *src)
                break;
        }

        if (i == pos && *src != EMPTY_ALP) 
            dst[pos++] = *src;
        src += 1;
    }
    dst[pos] = 0;
}

int get_e_ne_signs (struct sentence s, struct e_ne_signs *e_ne)
{
    memset (e_ne, 0, sizeof (struct e_ne_signs));
    e_ne->e_sign[0] = '#';  /* 非终结符中加入 #  */
    e_ne->ne_sign = realloc (e_ne->ne_sign, sizeof (struct ne_sign) * s.cnt);
    memset (e_ne->ne_sign, 0, sizeof (struct ne_sign) * s.cnt);

    int i;
    struct node   *p;

    for (i = 0; i < s.cnt; i++) {

        /* 得到非终结符 */
        p = s.prod[i].head;
        strncpy (e_ne->ne_sign[i].val, p->val, strlen (p->val));

        while ( (p = p->next) ) {

            get_e_signs (s, e_ne->e_sign, p->val);
        }
    }
    e_ne->rows = s.cnt;
    e_ne->cols = strlen (e_ne->e_sign);

    return SUCCESS;
}

int clean_recursion_sentence (const char *FILE_name, struct sentence *sentence)
{
    memset (sentence, 0, sizeof (struct sentence));

    /* 读取文本里的文法 */
    if (FALSE == read_sentence (FILE_name, sentence))
        return FALSE;

    /* 消除左递归后的文法 */
    clean_recursion_L (sentence);

    return SUCCESS;
}

/* 临时打印产生式的函数 */
void show_prod (struct sentence *s)
{
    int i;
    struct node   *p;

    printf ("消除左递归后的文法 \n");
    printf ("%s\n", "--------------------------------------------------");
    for (i = 0; i < s->cnt; i++) {
    
        p = s->prod[i].head;
        printf ("| %s -> ", p->val);

        while ( (p = p->next) ) {

            if (NULL == p->next)
                break;
            printf ("%s | ", p->val);
        }
        printf ("%s \n", p->val);
    }
    printf ("%s\n", "--------------------------------------------------");
}

int clean_recursion_L (struct sentence *sentence) 
{
    clean_indirect (sentence);

    return SUCCESS;
}

void clean_indirect (struct sentence *s)
{
    int     i, j;
    struct node     *p, *pi, *pj;


    for (i = 0; i < s->cnt; i++) {
        
        for (j = 0; j <= i; j++) {

            /* 如果产生式有修改，pi就会更新 */
            pi = s->prod[i].head->next;
            pj = s->prod[j].head;
            if (0 == strncmp (pi->val, pj->val, strlen (pj->val))) {
                
                combine_productive (s, i, j);
            }
        }
    }
}

#if 0
void clean_direct (struct sentence *s)
{
    int     i;

    for (i = 0; i < s->cnt; i++) 
        clean_direct_a_prod (s, i);
}
#endif

void clean_direct_a_prod (struct sentence *s, size_t i)
{
    int         len, flag = 0;
    struct node *head, *p, *q, *node;
    struct productive new;

    head = s->prod[i].head;
    len = strlen (head->val);
    q = head->next;

        
    /* 判断是否含有直接左递归 */
    if (0 == strncmp (head->val, q->val, len)) {

        /* 添加的产生式的头部 */
        memset (&new, 0, sizeof (struct productive));
        MALLOC_AND_fill (node, struct node, node->val, head->val, len);
        strncat (node->val, "\'", 1);

        new.head = node;
        node->next = head->next;
        s->prod[i].length = new.length = 1;

        for (q = head->next; q; q = q->next) {

            if (0 == strncmp (head->val, q->val, len)) {
            
                memmove (q->val, q->val+len, strlen (q->val)-len+1);
                strncat (q->val, new.head->val, len+1);
                new.length++;
                p = q;
            }else break;
        }
        MALLOC_AND_fill (node, struct node, node->val, EMPTY_SET, strlen (EMPTY_SET));
        p->next = node;     /* 给新的产生式添加空元素 */
        new.length++;

        head->next = q;
        if (NULL == q) {

            MALLOC_AND_fill (node, struct node, node->val, new.head->val, len+1);
            head->next = node;
            s->prod[i].length += 1;
        }

        for (q; q; q = q->next) {
            
            strncat (q->val, new.head->val, len+1);
            s->prod[i].length += 1;
        }

        /* 添加新的产生式到文法中 */
        add_P_2_S (s, &new);
    }
}

void combine_productive (struct sentence *s, size_t i, size_t j)
{
    if (i == j) {
        clean_direct_a_prod (s, i);
        return ;
    }

    struct node     *node, *p, *q, *k, *fre;
    struct node     *head = s->prod[i].head;
    struct productive   new = {0};

    char    *buf = head->next->val + strlen (s->prod[j].head->val);
    p = s->prod[j].head->next;
    k = head;
    fre = head->next;
    q = head->next->next;    /* 记录断开的地方 */
    for (p; p; p = p->next) {

        MALLOC_AND_fill (node, struct node, node->val, p->val, strlen (p->val));
        strncat (node->val, buf, strlen (buf));
        k = k->next = node;
        s->prod[i].length += 1;
    }
    k->next = fre->next;    /* 链接从第一个节点之后的所有节点 */
    if (k != head) {
        
        free (fre);     /* 第i条产生式多余的第一个节点 */
        s->prod[i].length -= 1;
    }

    clean_direct_a_prod (s, i);
}

int read_sentence (const char *FILE_name, struct sentence *sentence)
{
    FILE    *fp = NULL;
    char    *buf = NULL;
    size_t  len = 0;
    struct productive   prod;
    
    /* 初始化 sentence */
    memset (sentence, 0, sizeof (struct sentence));
    if (NULL == (fp = fopen (FILE_name, "r")) ) {

        perror ("recursion fopen");
        return FALSE;
    }

    while ( !feof (fp) ) {

        getline (&buf, &len, fp);
        if (0 == strncmp (buf, EMPTY_SET, 1)) 
            break;

        if (SUCCESS != str_2_PROD (buf, &prod)) {

            printf ("productive error : \"%s\"\n", buf);
        }
        add_P_2_S (sentence, &prod);

        free (buf);
        buf = NULL;
    }


    fclose (fp);

    return SUCCESS;
}

int add_productive_to_sentence (struct sentence *s, const struct productive *p)
{
    /* 扩展产生式表的长度 */
    if (s->cnt >= s->size) {
        
        s->size = (s->size + 1) << 1;
        if ( NULL == (s->prod = realloc \
            (s->prod, sizeof (struct productive) * s->size + 1)) )
                return ALLOC_F;
    }

    /* 加入新的产生式 */
    s->prod[s->cnt] = *p;
    s->cnt++;

    return SUCCESS;
}

int string_to_productive (const char *s, struct productive *prod)
{
    struct node *head = NULL, *node = NULL, *p = head;
    size_t  start = 0, pos = 0;
    size_t  len;

    prod->length = 0;
    for (start = pos = 0; s[pos]; pos++) {
        
        if ( '-' == s[pos] && '>' == s[pos+1] ) {

            if (start >= pos) 
                goto fail1; 
            len = pos - start; 
            pos++;

        }else if ('|' == s[pos] || '\n' == s[pos]) {
        
            if (start >= pos) 
                goto fail1;
            len = pos - start;

        }else continue;

        node = (struct node*)malloc (sizeof (struct node));
        memset (node, 0, sizeof (struct node));

        MALLOC_AND_fill (node, struct node, node, node, 0);

    //    if (1 != len || EMPTY_ALP != s[start])
            strncpy (node->val, s+start, len);
        if (NULL == head)
            head = node;
        else 
            p->next = node;
        p = node;

        (prod->length)++;
        start = pos+1;
    }

    prod->head = head;
    return SUCCESS;

fail1:
    return PROD_F;
}

void destroy_sentence (struct sentence *s)
{
    int     i;
    struct node *p, *q;
    
    for (i = 0; i < s->cnt; i++) {

        /* 销毁一条产生式 */
        while ( s->prod[i].head ) {

            p = s->prod[i].head;
            q = p->next;
            free (p);
            s->prod[i].head = q;
        }
        s->prod[i].head = NULL;
    }

//    free (s);
}
