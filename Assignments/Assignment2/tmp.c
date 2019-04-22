#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<math.h>


typedef struct stS{
    int val;
    int key;
}st;


void comb(st *s, st *sd, int index, int r,int start, int end, int size,int c, st *cpyc){

        printf("@@@@@@@@@@22\n");

        for(int i = 0; i < size; i++){
            printf("%d:%d\n",cpyc[i].val,cpyc[i].key);
        }
        printf("@@@@@@@@@@22\n");

    if (index == r)
    {
        //Select the allowed students
        // printf("\n==TA==\n\t");
        for(int i = 0; i < r; i++){
            printf("%d:%d\n",sd[i].val,sd[i].key);
        }

        printf("------------\n");
        // for(int i = 0; i < size; i++){
        //     printf("%d:%d\n",cpyc[i].val,cpyc[i].key);
        // }
        // printf("3####\n");
        
        // memcpy(s,cpyc,size*sizeof(st*));

        for(int i = 0; i < size; i++){
            printf("%d:%d\n",s[i].val,s[i].key);
        }
        printf("3####\n");
        
        // memcpy(s,cpyc,size*sizeof(st*));
        return;
    }
    // printf("%d %d %d %d", index, r, start, end);
    for(int i = start; i <= end; i++){
        printf("$$$%d\n",i);
        // printf("888\n");
        st *cpy = (st*) calloc(size,sizeof(st));
        memcpy(cpy,s,size*sizeof(st*));
        // printf("//%d\n",c);
        // for(int i = 0; i < size; i++){
        //     printf("%d:%d\n",s[i].val,s[i].key);
        //     // printf("^^^^^\n");
        //     // printf("%d:%d\n",cpy[i].val,cpy[i].key);
            
        // }
        // printf("######\n");
        if(s[i].key > 0){
            // printf("1stIF\n");
            s[i].key--;
            cpy[i].key--;
            sd[index] = s[i];
            comb(s,sd,index+1,r,i,end,size,c+1,cpy);
            // memcpy(s,cpy,size*sizeof(st*));

        }
        else{
            // printf("2ndIF\n");
            comb(s, sd, index,r,i+1,end,size,c+1,cpy);
            // memcpy(s,cpy,size*sizeof(st*));

        }
        // printf("/clear%d\n",c);
        // for(int i = 0; i < size; i++){
        //     printf("%d:%d\n",s[i].val,s[i].key);
        // }
        // printf("######\n");
    }
    // memcpy(s,cpyc,size*sizeof(st*));


}

int main(){
    int size = 3;
    st *s = (st*) calloc(size,sizeof(st));
    s[0].key = 1;
    s[0].val = 1;
    s[1].key = 3;
    s[1].val = 2;
    s[2].key = 2;
    s[2].val = 3;

    st *cpy = (st*) calloc(size,sizeof(st));
    memcpy(cpy,s,size*sizeof(st*));

    st *sd = (st*) calloc(4,sizeof(st));
    comb(s,sd,0,4,0,size-1,size,0,s);

    // memcpy(s,cpy,size*sizeof(st*));

    printf(")))))))))FINAL\n");
    for(int i = 0; i < size; i++){
        printf("%d:%d\n",s[i].val,s[i].key);
    }
    return 0;
}