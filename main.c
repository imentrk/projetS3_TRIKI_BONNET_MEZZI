#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list/list.h"
#include "agenda/agenda.h"
#include <time.h>

#define MAX_LINES_FIRSTNAMES 3600
#define MAX_LINES_NAMES 10000
#define MAX_LINE_LENGTH 100

int main()
{
    //CHOISIR LA PARTIE A EXECUTER ET DECOMMENTER

    //PARTIE 1
    
    /*t_d_list list = createEmptyList(4);
    insertCell(&list, 3, 1);
    displayListSimple(list);
    printf("\n");
    insertCell(&list, 13, 2);
    displayListSimple(list);
    printf("\n");
    insertCell(&list, 5, 3);
    displayListSimple(list);
    printf("\n");
    insertCell(&list, 9, 4);
    displayListSimple(list);
    printf("\n");
    displayListAligned(list);*/


    //PARTIE 2

    /*t_d_list list = createListFromLevel(4);
    displayListSimple(list);*/


    /*int numSearches=0;
    int max_level=0;
    printf("Entrez le nombre d'itérations de la recherche maximum : ");
    scanf("%d", &numSearches);
    printf("\nEntrez le nombre de niveau maximum de la liste : ");
    scanf("%d",&max_level);
    srand(time(NULL));
    Complexity (100, numSearches, max_level);*/


    //PARTIE 3

    /*t_agenda agenda = createEmptyAgenda();
    fill_random_agenda(&agenda,10);
    menu(agenda);*/

    return 0;
}
