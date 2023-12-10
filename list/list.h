

#ifndef PROJETSM3ALGO_LIST_H
#define PROJETSM3ALGO_LIST_H
#include "../cell/cell.h"

typedef struct s_d_list{
    t_d_cell **heads;
    int max_level;
}t_d_list;

t_d_list createEmptyList(int max);
void addHeadList (t_d_list * l, t_d_cell* newCell, int insert_level);
void insertCell (t_d_list * l, int val, int level);
void displayLevel(t_d_list l,int level);
void displayListSimple(t_d_list l);
void displayListAligned(t_d_list list);
t_d_list createListFromLevel(int n);
t_d_cell* searchInLevel0(t_d_list list, int val);
t_d_cell* searchFromTopLevel(t_d_list list, int val);
void testingTheComplexity(t_d_list list, int sizeRand, int numSearches,char ** time_lvl0, char ** time_all_levels);
void Complexity (int sizeRand, int numSearches, int max);

#endif //PROJETSM3ALGO_LIST_H
