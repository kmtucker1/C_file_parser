
// Kyler Tucker
// 04/03/2022
// Computer Science 350 Section 001
// Homework #2: West Virginia Population Problem
// GCC compiler


/*
 TODO:
 delete unneeded functions
 remove print statements
 do documentation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include "hw2_wvpp.h"



struct node {
    struct node* next_ptr;
    char county_name[20];
    char fpis_code[20];
    char seat[20];
    int population;
    int area;
};

list* create_list() {
    list* list_ptr = malloc(sizeof(struct list));
    list_ptr->headPtr = malloc(sizeof(struct node));
    list_ptr->headPtr->area = 0;
    return list_ptr;
}

struct node* get_head_node(list* list_ptr) {
    return list_ptr->headPtr;
}


double get_population_density(struct node* nodePtr) {
    return (double)nodePtr->population / (double)nodePtr->area;
}

const char* get_county_name(struct node* nodePtr) {
    return nodePtr->county_name;
}

const char* get_fips_code(struct node* nodePtr) {
    return nodePtr->fpis_code;
}

const char* get_seat(struct node* nodePtr) {
    return nodePtr->seat;
}

int get_population(struct node* nodePtr) {
    return nodePtr->population;
}

int get_area(struct node* nodePtr) {
    return nodePtr->population;
}

struct node* get_next_node(struct node* nodePtr) {
    return nodePtr->next_ptr;
}


void add_sorted_county(list* listPtr, const char* county_name, const char* fpis_code, const char* seat, int population, int area) {
    
    printf("FPIS CODE AFTER PUT: %s", fpis_code);
    
    //create new node
    struct node* new_ptr = malloc(sizeof(struct node));
    if(new_ptr == NULL) {
        puts("could not allocate memory");
        exit(1);
    }
    //allocate attributes of new node
    strncpy(new_ptr->county_name, county_name, 20);
    new_ptr->county_name[19] = '\0';
    strncpy(new_ptr->fpis_code, fpis_code, 20);
    
    new_ptr->fpis_code[19] = '\0';
    strncpy(new_ptr->seat, seat, 20);
    new_ptr->seat[19] = '\0';
    new_ptr->population = population;
    new_ptr->area = area;
    new_ptr->next_ptr = NULL;
    
    printf("area before put: %d\n", listPtr->headPtr->area);
    //check to see if list is empty
    if(listPtr->headPtr->area != 0) {
        //other nodes in list already
        struct node* cur_ptr = listPtr->headPtr;
        printf("cur_pointer namd: %s\n", cur_ptr->county_name);
        
        //if new node needs to become the new head of the list
        if( get_population_density(new_ptr) > get_population_density(cur_ptr)) {
            listPtr->headPtr = new_ptr;
            new_ptr->next_ptr = cur_ptr;
            return;
        //if there is only one node in list and new node isn't new start
        } else if(cur_ptr->next_ptr == NULL) {
            listPtr->headPtr->next_ptr = new_ptr;
            return;
        }
        //iterate through nodes and insert in the correct place
        while(cur_ptr->next_ptr != NULL) {
            //check to see if new node belongs in front of current node
            if(get_population_density(new_ptr) > get_population_density(cur_ptr->next_ptr)) {
                new_ptr->next_ptr = cur_ptr->next_ptr;
                cur_ptr->next_ptr = new_ptr;
                return;
            
            } else {
                cur_ptr = cur_ptr->next_ptr;
            }
        }
        //we are at the end of the list, so put new_ptr at the end
        cur_ptr->next_ptr = new_ptr;
        return;
    }
    //list is empty, make new_ptr the head
    listPtr->headPtr = new_ptr;
    
    printf("area before return: %d\n", listPtr->headPtr->area);
    return;
}



//TODO fix this function....first node delete works currently
int delete_by_county_name(list* listPtr, const char* county_name) {
    
    //instantiate current node
    struct node* cur_ptr = listPtr->headPtr;
    
    //if node to be deleted is first node
    if(!strcmp(listPtr->headPtr->county_name, county_name)) {
        listPtr->headPtr = listPtr->headPtr->next_ptr;
        return 0;
    }else{
        //node to be deleted is not first node
        cur_ptr = listPtr->headPtr;
        
        while(cur_ptr->next_ptr != NULL) {
            //node to be deleted is in middle of list
            if(!strcmp(cur_ptr->next_ptr->county_name, county_name)){
                
                cur_ptr->next_ptr = cur_ptr->next_ptr->next_ptr;
                return 0;
            }
            cur_ptr = cur_ptr->next_ptr;
        }
    }
    return 1;
}

void load_counties(list* listPtr, const char* filename) {
    //open file and store in counties_file
    FILE* file_ptr = fopen(filename, "r");
    if(file_ptr == NULL) {
        puts("couldn't open file");
        exit(1);
    }
    //throw away first line;
    char trash[80];
    fgets(trash, 80, file_ptr);
    
    //iterate through file and store data
    while(!feof(file_ptr)) {
        char* temp_name = malloc(sizeof(char)*20);
        char* temp_fpis_code = malloc(sizeof(char)*20);
        char* temp_seat = malloc(sizeof(char)*20);
        char* temp_population = malloc(sizeof(char)*20);
        char* temp_area = malloc(sizeof(char)*20);
        
        //gets county_name from file & remove whitespace
        printf("name: %s\n", fgets(temp_name, 17, file_ptr));
        strcpy(temp_name, rtrim(temp_name));
        //gets fpis_code from file
        printf("code: %s\n", fgets(temp_fpis_code, 12, file_ptr));
        printf("FPIS BEFORE TRIM %s\n", temp_fpis_code);
        strcpy(temp_fpis_code, rtrim(temp_fpis_code));
        printf("FPIS AFTER TRIM %s\n", temp_fpis_code);
        //gets seat from file
        printf("seat: %s\n", fgets(temp_seat, 22, file_ptr));
        strcpy(temp_seat, rtrim(temp_seat));
        //gets population from file
        printf("pop: %s\n", fgets(temp_population, 13, file_ptr));
        //gets area from file
        printf("area: %s\n", fgets(temp_area, 10, file_ptr));
        
        printf("FPIS before return: %s\n", temp_fpis_code);
        add_sorted_county(listPtr, temp_name, temp_fpis_code, temp_seat, atoi(temp_population), atoi(temp_area));
    }
       
}


void save_counties(list* listPtr, const char* filename) {
    FILE* file_ptr = fopen(filename, "w");
    if(file_ptr == NULL) {
        puts("couldn't open file");
        exit(1);
    }
    struct node* current_node = listPtr->headPtr;
    
    fprintf(file_ptr, "County Name,FPIS Code,Seat,Population,Area,Population Demsity\n");
   while(current_node != NULL) {
        fprintf(file_ptr, "%s,%s,%s,%d,%d,%.2f\n", current_node->county_name, current_node->fpis_code, current_node->seat, current_node->population, current_node->area, ((double)current_node->population / (double)current_node->area));
       current_node = current_node->next_ptr;
   }
}



const char* rtrim(const char* string) {
    char* newStr;
    int i = 0;
    
    //create malloc memory that exists outside of function
    newStr = malloc(20*sizeof(char));
    if(newStr == NULL) {
        puts("error allocatime malloc memory");
        exit(1);
    }
   
   //put input string value in malloc memory
   strcpy(newStr, string);
   //move curser to end of string
   while(newStr[i] != '\0') {
       i++;
   }
   //move curser back one index because '\0' occured past the end of the spaces
   i--;
    printf("%d\n",i);
   //remove whitespace starting at the end
   while(isspace(newStr[i])) {
       newStr[i] = '\0';
       i--;
   }
    return newStr;
}


const char* substring(const char* string, int start, int end) {
    //malloc string memory
    char* newStr = malloc(20*sizeof(char));
    //is start is out of bounds
    if(start < 0 || start > 19) {
        puts("start must be between 0 and 19");
        return string;
    //if end is out of bounds
    } else if( end < 1 || end > 20) {
        puts("end must be between 1 and 20");
        return string;
    } else {
        //create substring
        int j = 0;
        for(int i = start; i < end; i++) {
            newStr[j] = string[i];
            j++;
        }
        return newStr;
    }
}



//TODO delete this function
void print_list(list* listPtr) {
    struct node* current_ptr = listPtr->headPtr;
    printf("%s: %2f\n", listPtr->headPtr->county_name, (float)(listPtr->headPtr->population / listPtr->headPtr->area));
    while(current_ptr->next_ptr != NULL) {
        current_ptr = current_ptr->next_ptr;
        printf("%s: %2f\n", current_ptr->fpis_code, (float)(current_ptr->population / current_ptr->area));
    }
}


const char* get_programmer_name(void) {
    char* newStr = malloc(20*sizeof(char));
    newStr = "Kyler Tucker";
    return newStr;
}

