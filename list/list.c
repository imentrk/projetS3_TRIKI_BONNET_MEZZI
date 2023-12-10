
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"
#include "../timer/timer.h"

t_d_list createEmptyList(int max){
    //on cree une liste a partir d'un entier donne en argument indiquant le level max de la liste
    t_d_list list;
    //si le level demande est inferieur a zero on met le level max a zero et le tableau de heads a NULL
    if (max<=0){
        list.max_level=0;
        list.heads=NULL;
    }
    else{
        //sinon on affecte au niveau max de la liste l'entier max
        list.max_level = max;
        //on alloue dynamiquement l'espace pour le tableau de heads on initialise les case a NULL
        list.heads = malloc(max * sizeof(t_d_cell *));
        for (int i = 0 ; i < max ; i++){
            list.heads[i]=NULL;
        }
    }
    return list;
}

void addHeadList (t_d_list* l, t_d_cell* newCell, int insert_level){
    if(insert_level > l->max_level){
        return;
    }

    //si le niveau auquel je veux inserer est une liste vide je mets la nouvelle cellule en tete
    if (l->heads[insert_level]==NULL){
        l->heads[insert_level]=newCell;

        //printf("ok1");
    }
    else {
        //sinon je remplace l'ancienne tête par la nouvelle cellule
        newCell->tab_next[insert_level]=l->heads[insert_level];
        l->heads[insert_level]=newCell;
    }
}

void insertCell (t_d_list * l, int val, int level){ //le premier level = 1
    if(level > l->max_level){
        return;
    }
    t_d_cell * newCell = createCell(val,level);
    //si toute la liste est vide je mets la nouvelle cellule en tete de liste a tous les niveaux
    if (l->heads[0]==NULL){
        for (int i = 0 ; i < level ; i++){
            l->heads[i]=newCell;
        }
    }
    else {
        //sinon je fais une boucle allant de 0 jusqu'au nombre de niveaux
        for (int i = 0 ; i < level ; i++){
            //si la tete de liste du niveau en question est NULL ou alors que la valeur qu'on veut inserer
            //est inferieure a celle de la tete de liste du niveau j'ajoute une nouvelle tete
            if ( (l->heads[i]==NULL) || (val < l->heads[i]->value) ){
                addHeadList(l,newCell,i);
            }
            else {
                //sinon je parcours la liste du niveau i

                t_d_cell* tmp=l->heads[i];
                t_d_cell* ptmp=NULL;
                while( (tmp!=NULL) && (tmp->value < val) ){
                    ptmp=tmp;
                    tmp=tmp->tab_next[i];
                }
                //quand je suis au bon endroit j'insere la cellule
                ptmp->tab_next[i]=newCell;
                newCell->tab_next[i]=tmp;
            }
        }
    }
    /* if(level > l->max_level){
        return;
    }
    t_d_cell * newCell = createCell(val,level);

    t_d_cell * cur = l->heads[level];
    t_d_cell * prev = NULL;
    while(cur!=NULL && cur->value < val){
        prev = cur;
        cur = cur->tab_next[level];
    }
    if(prev == NULL){
        addHeadList(l,val,level);
        return;
    }
    for(int i=0;i<=level; i++){
        newCell->tab_next[i]=prev->tab_next[i];
        prev->tab_next[i]=newCell;
    } */
}

void displayLevel(t_d_list l,int level){
    if (level >= l.max_level){
        printf("level bigger than max_level : cannot display list");
        return;
    }
    printf("[list head_%d @-]-->",level);
    if (l.heads[level]==NULL){
       printf("NULL");
       return;
    }
    t_d_cell* tmp = l.heads[level];
    while (tmp!=NULL){
        printf("[%d|@-]-->",tmp->value);
        tmp = tmp->tab_next[level];
    }
    printf(" NULL\n");
}

void displayListSimple(t_d_list l){
    for (int level = 0 ; level <l.max_level; level++) {
        displayLevel(l, level);
        printf("\n");
    }
}

void displayListAligned(t_d_list list){

    displayLevel(list,0); //affichage du niveau 0

    //création d'une liste avec les valeurs de toutes les cellules au niveau 0
    int nb[1000];
    int i=0;
    t_d_cell* temp = list.heads[0];
    while(temp->tab_next[0]!= NULL)
    {
        nb[i++] = temp->value;
        temp = temp->tab_next[0];
    }
    nb[i]=temp->value;
    int last = i;

    //affichage dans l'ordre
    for (int j=1;j<list.max_level; j++)
    {
        temp = list.heads[j];
        i=0;
        int pos = 0;
        int sup ;
        printf("[list head_%d @-]--",j);

        while( temp != NULL)// tant que pas en bout de ligne
        {
            while( nb[i] != temp->value ) // tant que les valeurs ne sont pas égales je mets des traits
            {
                printf("---------");
                i++;
            }
            printf(">[%d|@-]--",temp->value);// quand c'est égal je l'écris
            temp = temp->tab_next[j];
            i++;
        }
        while(i<=last)
        {

            printf("----------");
            i++;


        }
        printf("> NULL\n");// quand plus de valeurs


    }
}

t_d_list createListFromLevel(int n){

    t_d_list list;
    //cas où l'entier donné en argument est nul ou négatif : la liste ne contient pas de cellules

    if (n<=0){
        list.max_level=0;
        list.heads=NULL;
    }

    else{
        //creation d'un tableau d'entiers pour contenir les niveaux des cellules
        int levels[(int)pow(2,n)-1];
        //incrementation est l'entier qui va nous permettre d'augmenter le niveau des cellules en fonction de leur indice
        //incrementation va de 0 a n-1
        int incrementation=0;
        //on initialise et remplit le tableau avec la boucle while suivante
        while(incrementation<n){
            for (int i = 0 ; i < (pow(2,n)-1) ; i++){
                //on ajoute incrementation aux entiers suivants les entiers congrus a 0 modulo 2^incrementation
                if(i%(int)pow(2,incrementation)==0){
                    levels[i+(int)pow(2,incrementation)-1]=incrementation;
                }
            }
            incrementation++;
        }

        /*for (int i = 0 ; i < (pow(2,n)-1) ; i++){
            printf("%d ",levels[i]);
        }*/

        //on affecte à la liste son nombre de niveaux et on cree les cellules pour chaque niveau
        list = createEmptyList(n);
        for (int i=0 ; i < (pow(2,n)-1) ; i++){
            insertCell(&list, (i+1), levels[i]+1);
        }
    }
    return list;
}

t_d_cell * searchInLevel0(t_d_list list, int val) {
    t_d_cell  * curr = list.heads[0]; //on cherche seulement au niveau 0

    //parcours de la liste au niveau 0
    while (curr != NULL) {
        //cas 1 : val trouvée
        if (curr->value == val) {
            return curr;
        }
        //cas 2 : val non trouvée, on passe à la cellule suivante au niveau 0
        curr = curr->tab_next[0];
    }

    return NULL;//val n'existe pas au niveau 0
}

t_d_cell* searchFromTopLevel(t_d_list list, int val) {
    int level = list.max_level - 1; //pour commencer par le niveau le plus haut
    t_d_cell  * curr = list.heads[level];

    //parcours du haut vers le bas
    while (level >= 0) {
        //parcours de la liste au niveau actuel
        while (curr != NULL && curr->value <= val) {
            //cas 1 : val trouvée
            if (curr->value == val) {
                return curr;
            }
            //cas 2 : val non trouvée, on passe à la cellule suivante au niveau actuel
            curr = curr->tab_next[level];
        }
        level--; //on passe au niveau inférieur si val non trouvée au niveau actuel
        if (level >= 0) {
            // mettre à jour curr pour le nouveau niveau,
            // si curr != NULL, curr va pointer vers la cellule suivante au nouveau niveau
            // sinon curr va pointervers la tête de la liste au nouveau niveau
            curr = (curr != NULL) ? curr->tab_next[level] : list.heads[level];
        }
    }

    return NULL;//val n'existe pas dans la liste
}

void testingTheComplexity(t_d_list list, int sizeRand, int numSearches,char ** time_lvl0, char ** time_all_levels){

    //printf("on teste la compléxité d'une recherche qui commence au niveau 0");
    startTimer();//le timer commence
    //on  effectue les recherches avec searchInLevel0
    for (int i = 0; i < numSearches; i++) {
        int val = rand() % sizeRand;  // recherche aléatoire
        t_d_cell * answer = searchInLevel0(list, val);
    }

    stopTimer();//le timer s'arrête

    *time_lvl0 = getTimeAsString(); // fonction du module timer

    startTimer();//le timer commence

    // on effectue les recherches avec searchFromTopLevel
    for (int i = 0; i < numSearches; i++) {
        int val = rand() % sizeRand;  // recherche aléatoire
        t_d_cell * answer = searchFromTopLevel(list, val);
    }

    stopTimer();//le timer s'arrête

    *time_all_levels  = getTimeAsString();


}

void Complexity (int sizeRand, int numSearches, int max){
    FILE *log_file = fopen("log.txt","w"); //on crée un fichier
    char format[] = "%d\t%s\t%s\n" ;
    char *time_lvl0;
    char *time_all_levels;
    for(int level=7; level< max; level++){

        testingTheComplexity(createListFromLevel(level), sizeRand, numSearches,&time_lvl0,&time_all_levels);
        fprintf(log_file,format,level,time_lvl0, time_all_levels);  //on remplit le fichier à chaque itération
    }
    fclose(log_file);  //on ferme le fichier
}
