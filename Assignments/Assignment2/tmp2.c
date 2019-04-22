#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<math.h>

typedef struct stS{
    int val;
    int key;
}st;

unsigned int next_multiset_combination(const st *multiset, st *ar, int n, unsigned int k)
{
    unsigned int finished = 0;
    unsigned int changed = 0;
    unsigned int i;
 
    for (i = k - 1; !finished && !changed; i--) {
        if (ar[i].val < multiset[i + (n - k)].val) {
            /* Find the successor */
            unsigned int j;
            for (j = 0; multiset[j].val <= ar[i].val; j++);
            /* Replace this element with it */
            ar[i] = multiset[j];
            if (i < k - 1) {
                /* Make the elements after it the same as this part of the multiset */
                unsigned int l;
                for (l = i + 1, j = j + 1; l < k; l++, j++) {
                    ar[l] = multiset[j];
                }
            }
            changed = 1;
        }
        finished = i == 0;
    }
    if (!changed) {
        /* Reset to first combination */
        for (i = 0; i < k; i++) {
            ar[i] = multiset[i];
        }
    }
    return changed;
}

static void print_array(const st *ar, int len)
{
    unsigned int i;
    printf("[ ");
    for (i = 0; i < len; i++) {
        printf("%d", ar[i].val);
        if (i < len - 1) {
            printf(",");
        }
    }
    printf(" ]");
}

int main(){

    int size = 6;
    st *s = (st*) calloc(size,sizeof(st));
    s[0].key = 4;
    s[0].val = 1;
    s[1].key = 4;
    s[1].val = 1;
    s[2].key = 4;
    s[2].val = 1;
    s[3].key = 4;
    s[3].val = 1;

    s[4].key = 2;
    s[4].val = 2;
    s[5].key = 2;
    s[5].val = 2;

    int r = 4;
    st *sd = (st*) calloc(r,sizeof(st));
 
    do {
        print_array(sd, r);
        printf(" \n");
    } while (next_multiset_combination(s, sd, size, r));
 
    return 0;
}