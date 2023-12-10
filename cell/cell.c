
#include <stdio.h>
#include <stdlib.h>
#include "cell.h"

t_d_cell* createCell(int val,int lev){
    t_d_cell* new = malloc(sizeof(t_d_cell));
    new->value = val;
    if (lev<=0){
        new->level = 0;
        new->tab_next = NULL;
    }
    else{
        new->level = lev;
        new->tab_next=malloc(lev * sizeof(t_d_cell *));
        for (int i = 0 ; i < lev ; i++){
            new->tab_next[i]=NULL;
        }
    }
    return new;
}