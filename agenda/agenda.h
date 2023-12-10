
#ifndef PROJETSM3ALGO_AGENDA_H
#define PROJETSM3ALGO_AGENDA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100
#define MAX_LINES_FIRSTNAMES 3600
#define MAX_LINES_NAMES 10000
#define MAX_LINE_LENGTH 100

//pour un contact
typedef struct contact{
    char* lastName;
    char* firstName;
}contact;

//pour un rendez-vous
typedef struct Date{
    int day;
    int month;
    int year;
}Date;
typedef struct Time{
    int hour;
    int minute;
}Time;

typedef struct Duration{
    int hour;
    int minute;
}Duration;

typedef struct Appointment{
    char * object;
    Date date;
    Time time;
    Duration duration;
}appointment;

char * scanString(void);

contact scanContact(void);

appointment scanAppointment(void);

//cellule stockant un rdv
typedef struct rdv_cell{
    appointment value;
    struct rdv_cell* next;
}t_rdv_cell;

//liste de rdv
typedef struct appointment_list{
    t_rdv_cell* head;
}appointment_list;

//entree = correspondance entre un contact et une liste de rdv
typedef struct entree{
    contact person;
    appointment_list person_appointments;
}t_entree;

//cellule de la liste agenda contenant une entree
typedef struct agenda_cell{
    t_entree value;
    struct agenda_cell **tab_next;
    int level;
}t_agenda_cell;

//un agenda est une skip liste
//les cellules contiennent des entrees
typedef struct agenda_list{
    t_agenda_cell **heads;
    int max_level;
}t_agenda;


void readFileNames(char chart[MAX_LINES_NAMES][MAX_LINE_LENGTH], const char * filename);

void readFileFirstnames(char chart[MAX_LINES_FIRSTNAMES][MAX_LINE_LENGTH], const char * filename);

contact create_random_Contact(char names_list[MAX_LINES_NAMES][MAX_LINE_LENGTH],char firstnames_list[MAX_LINES_FIRSTNAMES][MAX_LINE_LENGTH]);

t_rdv_cell* createRdvCell(appointment rdv);

appointment_list createAppointmentList(t_rdv_cell*);

t_entree createEntree(contact person,appointment_list);

t_agenda_cell* createAgendaCell(t_entree);

t_agenda createEmptyAgenda();

void addHeadAgenda(t_agenda* l, t_agenda_cell* newCell, int insert_level);

void addContactToAgenda(t_agenda* agenda,t_entree nouvelle_entree);

void fill_random_agenda (t_agenda*,int);

void upToDown(char* mot);

void seekContactInAgenda(t_agenda agenda);

void displayAppointmentContact(const char* lastName, t_agenda agenda);

void addContact(t_agenda* agenda, const char* lastName, const char* firstName, appointment_list appointments);

void addAppointmentForContact(t_agenda* agenda, appointment newAppointment, const char* lastName);

void addContactToAgendaInLevel0(t_agenda* agenda,t_entree nouvelle_entree);

void seekContactInAgendaInLevel0(t_agenda agenda);

void DeleteAppointmentForContact(t_agenda* agenda, const char* lastName, const char* appointmentObject);

void testingTheComplexityForTheSeekFunctions(t_agenda agenda, int numSearches,char ** time_lvl0, char ** time_all_levels);

void saveAppointmentsToFile(t_agenda *agenda, const char *filename);

void menu(t_agenda agenda);

void ComplexityOfTheAddFunctions (t_agenda * agenda, int sizeRand, int numSearches, int max);

#endif //PROJETSM3ALGO_AGENDA_H
