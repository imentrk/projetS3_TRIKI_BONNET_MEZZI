
#include "agenda.h"
#include "../timer/timer.h"
#define MAX_LINES_FIRSTNAMES 3600
#define MAX_LINES_NAMES 10000
#define MAX_LINE_LENGTH 100

char * scanString(void){
    char tab[MAX]; //saisie temporaire
    scanf("%100s", tab); //on sécurise la saisie à MAX=100 car.
    char * res = malloc(strlen(tab) + 1); //inclure l'espace mémoire pour '\0'
    strcpy(res, tab);
    return res;
}

contact scanContact(void){
    contact ctct;
    printf("Contact's name : ");
    ctct.lastName = scanString();
    printf("Contact's firstname : ");
    ctct.firstName = scanString();
    return ctct;
}

appointment scanAppointment(void){
    appointment rdv;
    printf("\ndate appointment (day month year) : ");
    scanf("%d %d %d", &rdv.date.day, &rdv.date.month, &rdv.date.year);
    printf("hour appointment (hour minute) : ");
    scanf("%d %d", &rdv.time.hour, &rdv.time.minute);
    printf("duration appointment (hour minute) : ");
    scanf("%d %d", &rdv.duration.hour, &rdv.duration.minute);
    printf("appointement's object : ");
    rdv.object = scanString();
    return rdv;
}

//fonction permettant de lire des fichiers de noms et prenoms
//et de les mettre dans des tableaux
void readFileFirstnames(char chart[MAX_LINES_FIRSTNAMES][MAX_LINE_LENGTH], const char * filename)
{   //on cree un pointeur sur fichier pour lire le fichier de prenoms
    FILE * fptr = fopen(filename,"r");
    //on verifie que le fichier s'est ouvert correctement
    if (fptr == NULL) {
        printf("error opening file");
        exit(-1); // Exit with an error code
    }
    //printf("Readfiling\n");
    for (int i=0;i<MAX_LINES_FIRSTNAMES;i++) {
        //on stocke les prenoms dans un tableau grace a la fonction fgets et on enleve le retour a la ligne
        fgets(chart[i],MAX_LINE_LENGTH,fptr);
        chart[i][strcspn(chart[i], "\n")] = '\0';
    }
    //on ferme le fichier
    fclose(fptr);
    return;
}

//cette fonction est la meme que la fonction readFileFirstnames
void readFileNames(char chart[MAX_LINES_NAMES][MAX_LINE_LENGTH], const char * filename)
{
    FILE * fptr = fopen(filename,"r");
    //on verifie que le fichier s'est ouvert correctement
    if (fptr == NULL) {
        printf("error opening file");
        exit(-1); // Exit with an error code
    }
    //printf("Readfiling\n");

    //je choisis des indices random dans les tableaux de noms et de prenoms et je cree un contact aleatoire
    for (int i=0;i<MAX_LINES_NAMES;i++) {
        fgets(chart[i],MAX_LINE_LENGTH,fptr);
        chart[i][strcspn(chart[i], "\n")] = '\0';
    }
    fclose(fptr);
    return;
}

contact create_random_Contact(char names_list[MAX_LINES_NAMES][MAX_LINE_LENGTH],char firstnames_list[MAX_LINES_FIRSTNAMES][MAX_LINE_LENGTH]){
    contact new_contact;
    new_contact.firstName = firstnames_list[rand()%3601];
    new_contact.lastName = names_list[rand()%10000];
    return new_contact;
}

t_rdv_cell* createRdvCell(appointment rdv){
    t_rdv_cell* new_cell = malloc(sizeof(t_rdv_cell));
    new_cell->value = rdv;
    new_cell->next = NULL;
    return new_cell;
}

appointment_list createAppointmentList(t_rdv_cell* rdv_cell){
    appointment_list new_list;
    new_list.head=rdv_cell;
    return new_list;
}

t_entree createEntree(contact Contact,appointment_list ContactList){
    t_entree new_entree;
    new_entree.person = Contact;
    new_entree.person_appointments = ContactList;
    return new_entree;
}

t_agenda_cell* createAgendaCell(t_entree entree){
    t_agenda_cell* new_agenda_cell = malloc(sizeof(t_agenda_cell));
    new_agenda_cell->value=entree;
    new_agenda_cell->level=4;
    new_agenda_cell->tab_next=malloc(4*sizeof(t_agenda_cell*));
    for (int i = 0 ; i < 4 ; i++){
        new_agenda_cell->tab_next[i]=NULL;
    }
    return new_agenda_cell;
}

t_agenda createEmptyAgenda(){
    t_agenda agenda;
    agenda.max_level=4;
    agenda.heads=malloc(4*sizeof(t_agenda_cell*));
    for (int i = 0 ; i < 4; i++){
        agenda.heads[i]=NULL;
    }
    return agenda;
}

void addHeadAgenda(t_agenda* agenda, t_agenda_cell* newCell, int insert_level){

    //si le niveau auquel je veux inserer est vide alors j'ajoute simplement la nouvelle cellule en tete
    if (agenda->heads[insert_level]==NULL){
        agenda->heads[insert_level]=newCell;

    }
    else {
        //sinon je remplace l'ancienne tete par la nouvelle
        newCell->tab_next[insert_level]=agenda->heads[insert_level];
        agenda->heads[insert_level]=newCell;
    }
}

void upToDown(char* mot){
    //la fonction qui convertie les chaines de caracteres en majuscules en minuscules
    //la fonction tolower convertie un caractere majuscule en un caractere minuscule
    for(int i = 0 ; i < strlen(mot) ; i++){
        mot[i]=tolower(mot[i]);
    }
}

void addContactToAgenda(t_agenda* agenda,t_entree nouvelle_entree){

    //on cree une nouvelle cellule agenda a partir de l'entree mise en argument

    t_agenda_cell *new_cell = createAgendaCell(nouvelle_entree);

    //si l'agenda est vide, on ajoute cette cellule en tete de liste de tous les niveaux

    if (agenda->heads[0] == NULL) {
        for (int i = 0; i < 4; i++) {
            agenda->heads[i] = new_cell;
        }
    }

    //on introduit par ordre alphabetique

    else {

        // on fait une boucle
        //pour chaque niveau, on insere au bon endroit dans l'ordre alphabetique et selon les contraintes d'insertion qui permettent une recherche efficace

        for (int i = 0; i < 4; i++) {

            //si le nom est avant dans l'ordre alphabetique on l'introduit directemet dans l'agenda, en premier

            if ((agenda->heads[i] == NULL) ||
                strcmp(nouvelle_entree.person.lastName, agenda->heads[i]->value.person.lastName) < 0) {
                addHeadAgenda(agenda, new_cell, i);
            }

            //s'il est apres dans l'ordre alphabetique on parcourt l'agenda jusqu'a arriver au bon endroit dans la liste

            else {
                t_agenda_cell *tmp = agenda->heads[i];
                t_agenda_cell *ptmp = NULL;
                while ((tmp != NULL) && (strcasecmp(tmp->value.person.lastName, new_cell->value.person.lastName) < 0)) {
                    ptmp = tmp;
                    tmp = tmp->tab_next[i];
                }
                //une fois qu'on est au bon endroit, en fonction du niveau, on insere ou pas selon les lettres de ptmp
                //au niveau 0 on insere tout le temps
                if (i == 0) {
                    ptmp->tab_next[i] = new_cell;
                    new_cell->tab_next[i] = tmp;
                }
                //au niveau 1 on insere quand une des 3 premieres lettres differe de ptmp

                if (i == 1) {
                    if (!(ptmp->value.person.lastName[0] == new_cell->value.person.lastName[0] &&
                        ptmp->value.person.lastName[1] == new_cell->value.person.lastName[1] &&
                            ptmp->value.person.lastName[2] == new_cell->value.person.lastName[2])) {
                            ptmp->tab_next[i] = new_cell;
                            new_cell->tab_next[i] = tmp;
                        }
                }
                //au niveau 2 on insere quand une des deux premieres lettre differe de celles de ptmp
                if (i == 2) {
                        if (!(ptmp->value.person.lastName[0] == new_cell->value.person.lastName[0] &&
                            ptmp->value.person.lastName[1] == new_cell->value.person.lastName[1])) {
                            ptmp->tab_next[i] = new_cell;
                            new_cell->tab_next[i] = tmp;
                        }
                }
                //au niveau 3 on insere quand la premiere lettre de newcell est differente de la premiere lettre de ptmp
                if (i == 3) {
                        if (ptmp->value.person.lastName[0] != new_cell->value.person.lastName[0]){
                            ptmp->tab_next[i] = new_cell;
                            new_cell->tab_next[i] = tmp;
                        }
                }
            }
        }

    }
    //une fois que la cellule newcell est inseree au bon endroit on doit s'assurer que la suivante est bien inseree aussi
    for (int i = 0 ; i < 4 ; i++){
        //au niveau 1, on retire la suivante de newcell si ses 3 premieres lettres sont les memes que celles de newcell
        if (i == 1) {

            if(new_cell->tab_next[i] != NULL && new_cell->value.person.lastName[0] == new_cell->tab_next[i]->value.person.lastName[0]
               && new_cell->value.person.lastName[1] == new_cell->tab_next[i]->value.person.lastName[1]
               && new_cell->value.person.lastName[2] == new_cell->tab_next[i]->value.person.lastName[2]){
                new_cell->tab_next[i]=new_cell->tab_next[i]->tab_next[i];
            }
        }
        //au niveau 2, on retire la suivante de newcell si ses 2 premieres lettres sont les memes que celles de newcell
        if (i == 2) {

            if(new_cell->tab_next[i] != NULL &&
               new_cell->value.person.lastName[0] == new_cell->tab_next[i]->value.person.lastName[0]
               && new_cell->value.person.lastName[1] == new_cell->tab_next[i]->value.person.lastName[1]){
                new_cell->tab_next[i]=new_cell->tab_next[i]->tab_next[i];
            }
        }
        //au niveau 3, on retire la suivante de newcell si sa premiere lettre est celle de newcell
        if (i == 3) {

            if(new_cell->tab_next[i] != NULL && new_cell->value.person.lastName[0] == new_cell->tab_next[i]->value.person.lastName[0]){
                new_cell->tab_next[i]=new_cell->tab_next[i]->tab_next[i];
            }
        }
    }
}

void fill_random_agenda(t_agenda* agenda,int nb_contacts){
    //on remplit l'agenda a partir des fichiers de noms et de prenoms de façon aleatoire
    if (agenda==NULL){
        return;
    }

    const char * firstnamefile = "prenoms.txt";
    const char * namefile = "noms.txt";

    char firstname[MAX_LINES_FIRSTNAMES][MAX_LINE_LENGTH];
    char name[MAX_LINES_NAMES][MAX_LINE_LENGTH];

    //on stocke les noms et les prenoms des fichiers dans les tableaux
    readFileFirstnames(firstname,firstnamefile);
    readFileNames(name,namefile);

    //pour chaque contact, on cree un contact a partir de ces tableau, on cree une liste vide de rdv
    //on cree une entree et on l'ajoute a l'agenda
    for (int i = 0 ; i < nb_contacts ; i++){
        contact nouveau_contact = create_random_Contact(&name[i],&firstname[i]);
        appointment_list nouvelle_liste = createAppointmentList(NULL);
        t_entree nouvelle_entree = createEntree(nouveau_contact,nouvelle_liste);
        addContactToAgenda(agenda,nouvelle_entree);
    }
}

void seekContactInAgenda(t_agenda agenda){

    //on cherche un contact dans un agenda de maniere aleatoire
    if(agenda.heads==NULL){
        return;
    }

    printf("Looking for a contact ? \n");

    //on demande de saisir les 3 premieres lettres du contact que l'on stocke dans des variables de type caractere
        char lettre1;
    printf("First letter : ");
    scanf( " %c",&lettre1);

    char lettre2;
    printf("Second letter : ");
    scanf(" %c",&lettre2);

    char lettre3;
    printf("Third letter : ");
    scanf(" %c",&lettre3);

    //la variable test permet de savoir si on a trouve le contact cherche
    int test;

    //on part du plus haut niveau pour une recherche dichotomique
    for(int i = 3 ; i >= 0 ; i--){
        test=0;
        t_agenda_cell* tmp = agenda.heads[i];
        while(tmp!=NULL){
            //pour chaque niveau, on parcourt la liste en comparant les 3 lettres saisies avec les 3 premieres lettres des contacts
            if (tmp->value.person.lastName[0]==toupper(lettre1) && tmp->value.person.lastName[1]==toupper(lettre2) && tmp->value.person.lastName[2]==toupper(lettre3)){
                //on convertit le nom et le prenom du contact en minuscule
                upToDown(tmp->value.person.lastName);
                upToDown(tmp->value.person.firstName);
                //si c'est égal on affiche le contact
                printf("%s_%s\n",tmp->value.person.lastName,tmp->value.person.firstName);
                //on incremente la variable test pour dire qu'on a trouvé
                test++;
            }
            tmp=tmp->tab_next[i];
        }
        //si on a trouve le contact on sort de la boucle
        if (test!=0){
            return;
        }
    }
}

void displayAppointmentContact(const char* lastName, t_agenda agenda) {
    char minlastName[MAX_LINE_LENGTH];
    strcpy(minlastName, lastName);
    upToDown(minlastName);
    int test;
    for (int i = 3; i >= 0; i--) {
        test = 0;
        t_agenda_cell* tmp = agenda.heads[i];
        while (tmp != NULL) {
            //comparaison du nom de famille
            if (strcasecmp(tmp->value.person.lastName, minlastName) == 0) {
                printf("Appointments for %s %s :\n", tmp->value.person.firstName, tmp->value.person.lastName, i);

                t_rdv_cell* appointmentCell = tmp->value.person_appointments.head;

                while (appointmentCell != NULL) {
                    printf("Date: %d-%d-%d\n", appointmentCell->value.date.year,
                           appointmentCell->value.date.month, appointmentCell->value.date.day);
                    printf("Time: %d:%d\n", appointmentCell->value.time.hour,
                           appointmentCell->value.time.minute);
                    printf("Duration: %d hours %d minutes\n", appointmentCell->value.duration.hour,
                           appointmentCell->value.duration.minute);
                    printf("Purpose: %s\n", appointmentCell->value.object);
                    printf("\n");

                    appointmentCell = appointmentCell->next;
                }
                test++;
            }
            tmp = tmp->tab_next[i];
        }
        //on a trouvé le contact et on a deja affiché son rdv
        if (test != 0) {
            return;
        }
    }
    printf("The contact was not found in the agenda.\n");
}


void addContact(t_agenda* agenda, const char* lastName, const char* firstName, appointment_list appointments){

    char minlastName[MAX_LINE_LENGTH];
    strcpy(minlastName, lastName);
    upToDown(minlastName);

    char minfirstName[MAX_LINE_LENGTH];
    strcpy(minfirstName, firstName);
    upToDown(minfirstName);

    contact newContact;
    newContact.lastName = minlastName;
    newContact.firstName = minfirstName;


    t_entree entry = createEntree(newContact, appointments);

    t_agenda_cell * agendaCell = createAgendaCell(entry);

    //si agenda vide : on ajoute à tous les niv
    if (agenda->heads[0] == NULL) {
        for (int i = 0; i < 4; i++) {
            addHeadAgenda(agenda, agendaCell, i);
        }
        return;
    }

    //on ajoute au bon niv
    for (int i = 0; i < 4; i++) {

        if (agenda->heads[i] == NULL) {
            addHeadAgenda(agenda, agendaCell, i);
            break;
        }

        t_agenda_cell * prev = NULL;
        t_agenda_cell * tmp = agenda->heads[i];

        while (tmp != NULL && strcasecmp(tmp->value.person.lastName, agendaCell->value.person.lastName) < 0) {
            prev = tmp;
            tmp = tmp->tab_next[i];
        }

        if (prev == NULL) {
            addHeadAgenda(agenda, agendaCell, i);
            break;
        }

        prev->tab_next[i] = agendaCell;
        agendaCell->tab_next[i] = tmp;
    }
}


void addAppointmentForContact(t_agenda* agenda, appointment newAppointment, const char* lastName) {

    char minlastName[MAX_LINE_LENGTH];
    strcpy(minlastName, lastName);
    upToDown(minlastName);

    t_agenda_cell * prev = NULL;
    t_agenda_cell * tmp = agenda->heads[0];


    while (tmp != NULL && strcasecmp(tmp->value.person.lastName, minlastName) < 0) {
        prev = tmp;
        tmp = tmp->tab_next[0];
    }

    //on trouve pas le contact donc on ajoute nouvelle entrée
    if (tmp == NULL || strcasecmp(tmp->value.person.lastName, minlastName) != 0) {
        t_rdv_cell * appointmentCell = createRdvCell(newAppointment);

        appointment_list appointments = createAppointmentList(appointmentCell);

        t_entree entry = createEntree((contact){.lastName = minlastName, .firstName = ""}, appointments);

        t_agenda_cell * agendaCell = createAgendaCell(entry);

        //on ajoute le nouveau contact au niv = 0
        int level = 0;
        if (prev == NULL) {
            addHeadAgenda(agenda, agendaCell, level);
        } else {
            prev->tab_next[level] = agendaCell;
            agendaCell->tab_next[level] = tmp;
        }
    } else {
        //on trouve le contact et on ajoute le nouveau rdv
        t_rdv_cell * appointmentCell = createRdvCell(newAppointment);
        appointmentCell->next = tmp->value.person_appointments.head;
        tmp->value.person_appointments.head = appointmentCell;
    }
}

void addContactToAgendaInLevel0(t_agenda* agenda,t_entree nouvelle_entree) {
    //nouvelle cellule pour l'agenda a partir d'une entree

    t_agenda_cell *new_cell = createAgendaCell(nouvelle_entree);

    //si l'agenda est vide

    if (agenda->heads[0] == NULL) {
        agenda->heads[0] = new_cell;
    }

        //on introduit par ordre alphabetique

    else {

        //si le nom est avant dans l'ordre alphabetique on l'introduit directement dans l'agenda au niveau 0, en premier
        if ((agenda->heads[0] == NULL) ||
            strcmp(nouvelle_entree.person.lastName, agenda->heads[0]->value.person.lastName) < 0) {
            addHeadAgenda(agenda, new_cell, 0);
        }

            //s'il est après dans l'ordre alphabétique on parcourt l'agenda au niveau 0 jusqu'a arriver au bon endroit dans l'alphabet

        else {
            t_agenda_cell *tmp = agenda->heads[0];
            t_agenda_cell *ptmp = NULL;
            while ((tmp != NULL) && (strcasecmp(tmp->value.person.lastName, new_cell->value.person.lastName) < 0)) {
                ptmp = tmp;
                tmp = tmp->tab_next[0];
            }


            ptmp->tab_next[0] = new_cell;
            new_cell->tab_next[0] = tmp;


        }
    }
}

void seekContactInAgendaInLevel0(t_agenda agenda) {
    if (agenda.heads == NULL) {
        return;
    }

    printf("Looking for a contact?\n");

    char lettre1;
    printf("First letter: ");
    scanf(" %c", &lettre1);

    char lettre2;
    printf("Second letter: ");
    scanf(" %c", &lettre2);

    char lettre3;
    printf("Third letter: ");
    scanf(" %c", &lettre3);

    t_agenda_cell *tmp = agenda.heads[0];

    while (tmp != NULL) {

        if (toupper(tmp->value.person.lastName[0]) == toupper(lettre1) && toupper(tmp->value.person.lastName[1]) == toupper(lettre2) && toupper(tmp->value.person.lastName[2]) == toupper(lettre3)) {
            upToDown(tmp->value.person.lastName);
            upToDown(tmp->value.person.firstName);
            printf("%s_%s\n", tmp->value.person.lastName, tmp->value.person.firstName);
            break;
        }
        tmp = tmp->tab_next[0];
    }
}

void DeleteAppointmentForContact(t_agenda* agenda, const char* lastName, const char* appointmentObject) {
    char minLastName[MAX_LINE_LENGTH];
    strcpy(minLastName, lastName);
    upToDown(minLastName);

    t_agenda_cell *prev_cell = NULL;
    t_agenda_cell *tmp_cell = agenda->heads[0];

    while (tmp_cell != NULL && strcasecmp(tmp_cell->value.person.lastName, minLastName) < 0) {
        prev_cell = tmp_cell;
        tmp_cell = tmp_cell->tab_next[0];
    }

    // recherche du contact
    if (tmp_cell != NULL && strcasecmp(tmp_cell->value.person.lastName, minLastName) == 0) {
        // recherche du rendez-vous
        t_rdv_cell *prev_appointment = NULL;
        t_rdv_cell *tmp_appointment = tmp_cell->value.person_appointments.head;

        while (tmp_appointment != NULL &&
               strcasecmp(tmp_appointment->value.object, appointmentObject) != 0) {
            prev_appointment = tmp_appointment;
            tmp_appointment = tmp_appointment->next;
        }

        // effacer le rendez-vous si trouvé
        if (tmp_appointment != NULL) {
            if (prev_appointment == NULL) {
                // si le rendez-vous est en tête de liste
                tmp_cell->value.person_appointments.head = tmp_appointment->next;
            } else {
                // sinon
                prev_appointment->next = tmp_appointment->next;
            }

            free(tmp_appointment);
        }
    }
}

void testingTheComplexityForTheSeekFunctions(t_agenda agenda, int numSearches,char ** time_lvl0, char ** time_all_levels) {
    startTimer();//le timer commence

    //on  effectue les recherches avec searchInLevel0
    for (int i = 0; i < numSearches; i++) {
        seekContactInAgendaInLevel0(agenda);
    }

    stopTimer();//le timer s'arrête

    *time_lvl0 = getTimeAsString(); // fonction du module timer



    startTimer();//le timer commence

    // on effectue les recherches avec searchFromTopLevel
    for (int i = 0; i < numSearches; i++) {
        seekContactInAgenda(agenda);
    }

    stopTimer();//le timer s'arrête
    *time_all_levels  = getTimeAsString();
}
void ComplexityOfTheSeekFunctions(t_agenda agenda, int numSearches, int max) {
    FILE *log_file = fopen("log_1.txt", "w");
    char format[] = "%d\t%s\t%s\n";
    char *time_lvl0;
    char *time_all_levels;

    for (int level = 0; level <= 3; level++) {
        testingTheComplexityForTheSeekFunctions(agenda, numSearches, &time_lvl0, &time_all_levels);
        fprintf(log_file, format, level, time_lvl0, time_all_levels);
    }

    fclose(log_file);
}

void saveAppointmentsToFile(t_agenda *agenda, const char *filename) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    fprintf(file, "Liste des rendez-vous :\n\n");

    t_agenda_cell *tmp_cell = agenda->heads[0];

    while (tmp_cell != NULL) {
        fprintf(file, "Nom : %s\n", tmp_cell->value.person.lastName);

        t_rdv_cell *tmp_appointment = tmp_cell->value.person_appointments.head;

        while (tmp_appointment != NULL) {
            fprintf(file, "  - %s\n", tmp_appointment->value.object);
            fprintf(file, "    Date et heure : %d/%d/%d à %d:%d\n", tmp_appointment->value.date.day,tmp_appointment->value.date.month,tmp_appointment->value.date.year, tmp_appointment->value.time.hour, tmp_appointment->value.time.minute);
            fprintf(file, "    Durée : %dh%dmin\n", tmp_appointment->value.duration.hour, tmp_appointment->value.duration.minute);
            fprintf(file, "\n");

            tmp_appointment = tmp_appointment->next;
        }

        tmp_cell = tmp_cell->tab_next[0];
    }

    fclose(file);
}

void menu(t_agenda agenda) {
    contact *listeContacts = NULL;


    printf("\nMenu principal:\n");
    printf("1. Rechercher un contact\n");
    printf("2. Afficher les rendez-vous d'un contact\n");
    printf("3. Creer un contact\n");
    printf("4. Creer un rendez-vous pour un contact\n");
    printf("5. Supprimer un rendez-vous\n");
    printf("6. Sauvegarder le fichier de tous les rendez-vous\n");
    printf("7. Fournir les temps de calcul pour une insertion de nouveau contact\n");
    printf("0. Quitter\n");

    int choix;
    printf("Choisissez une option : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1: {
            seekContactInAgenda(agenda);
            menu(agenda);
        }
        case 2: {
            char lastName[50];
            printf("Entrez le nom du contact : ");
            scanf("%s", lastName);
            displayAppointmentContact(lastName, agenda);
            menu(agenda);
        }
        case 3:{
            contact nouveauContact = scanContact();
            addContactToAgenda(&agenda,createEntree(nouveauContact,createAppointmentList(NULL)));
            menu(agenda);
        }
        case 4: {
            char nomContact[50];
            printf("Entrez le nom du contact : ");
            scanf("%s", nomContact);
            appointment newAppointment = scanAppointment();
            addAppointmentForContact(&agenda, newAppointment, nomContact);
            menu(agenda);
        }
        case 5: {
            char nomContact[50];
            printf("Entrez le nom du contact : ");
            scanf("%s", nomContact);
            printf("Entrez l'object du rendez-vous à supprimer:");
            char * appointmentObject = scanString();
            DeleteAppointmentForContact(&agenda, nomContact,appointmentObject);
            menu(agenda);
        }
        case 6:{
            char nomFichier[50];
            printf("Entrez le nom du fichier : ");
            scanf("%s", nomFichier);
            saveAppointmentsToFile(&agenda, nomFichier);
            menu(agenda);
        }
        case 7:{
            int numSearches= 0;
            printf("Entrez le nombre d'itérations :");
            scanf("%d",&numSearches);
            ComplexityOfTheAddFunctions (&agenda,100,numSearches,4);
            menu(agenda);
        }
        case 0:{
            printf("Au revoir !\n");
            return;
        }
        default:{
            printf("L'option saisie est invalide, veuillez saisir un chiffre entre 0 et 7. Merci\n");
            menu(agenda);
        }
    }
}

void testingTheComplexityForTheAddFunctions(t_agenda *agenda, int numSearches, char **time_lvl0, char **time_all_levels, t_entree entry) {
    startTimer(); // Le timer commence

    // On effectue les ajouts avec addContactToAgendaInLevel0
    for (int i = 0; i < numSearches; i++) {
        addContactToAgendaInLevel0(agenda, entry);
    }

    stopTimer(); // Le timer s'arrête
    *time_lvl0 = getTimeAsString(); // Fonction du module timer

    startTimer(); // Le timer commence

    // On effectue les ajouts avec addContactToAgenda
    for (int i = 0; i < numSearches; i++) {
        addContactToAgenda(agenda, entry);
    }

    stopTimer(); // Le timer s'arrête
    *time_all_levels = getTimeAsString(); // Fonction du module timer
}

void ComplexityOfTheAddFunctions (t_agenda * agenda, int sizeRand, int numSearches, int max){
    FILE *log_file = fopen("log.txt", "w");
    char format[] = "%d\t%s\t%s\n";
    char *time_lvl0;
    char *time_all_levels;
    const char * firstnamefile = "prenoms.txt";
    const char * namefile = "noms.txt";

    char firstname[MAX_LINES_FIRSTNAMES][MAX_LINE_LENGTH];
    char name[MAX_LINES_NAMES][MAX_LINE_LENGTH];

    readFileFirstnames(firstname,firstnamefile);
    readFileNames(name,namefile);
    t_entree entry=createEntree(create_random_Contact(&name[rand() % sizeRand], &firstname[rand() % sizeRand]),createAppointmentList(NULL));
    for (int level = 0; level < max; level++) {
        testingTheComplexityForTheAddFunctions(agenda,numSearches, &time_lvl0, &time_all_levels, entry);
        fprintf(log_file, format, level, time_lvl0, time_all_levels);
    }

    fclose(log_file);
}


