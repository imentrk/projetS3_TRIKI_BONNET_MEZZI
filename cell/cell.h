
#ifndef PROJETSM3ALGO_CELL_H
#define PROJETSM3ALGO_CELL_H

typedef struct s_d_cell
{
    int value;
    struct s_d_cell **tab_next;
    int level;
} t_d_cell;

t_d_cell* createCell(int val,int level);

#endif //PROJETSM3ALGO_CELL_H
