#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Berke Diler 2401503
 * CNG 315 - Assignment 1
 * The time complexities of the functions are at the bottom where the functions are written.
 */
//A structure for e-mails
struct email {
    int email_id;
    char sender[50];
    char recipient[50];
    int day_of_the_month;
    char *content;
};
//function to count the words in the content of a string
int count_words(char *str);
//heapsort fucntions
void heapify(struct email *emails, int n, int i, int sorting_criteria);
void buildHeap(struct email *emails, int n,int sorting_criteria);
void heapSort(struct email *emails, int n,int sorting_criteria);
//heapsort functions ended
//binary search function
void binSearch(struct email *emails, int num, int id);
//function that converts integer to string
char *int_to_string(int num);
//function that concatenate the file number to the path with a .txt extension
char *concatenate(char *path, int num);
struct email *readEmails(char *path,int num);
//function that prints the emails
void printEmails(struct email *emails, int num);
//menu function
void menu(struct email *emails, int num) {
    printf("****************\n");
    printf("Please select an option:\n");
    do{
        printf("1. Display e-mails sorted by ID\n");
        printf("2. Display e-mails sorted by number of words\n");
        printf("3. Display e-mails sorted by date\n");
        printf("4. Search e-mail by ID\n");
        printf("5. Print all e-mails\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printf("You have chosen 1\n");
            heapSort(emails, num, 1);
            printEmails(emails, num);
            break;
        case 2:
            printf("You have chosen 2\n");
            heapSort(emails, num, 2);
            printEmails(emails, num);
            break;
        case 3:
            printf("You have chosen 3\n");
            heapSort(emails, num, 3);
            printEmails(emails, num);
            break;
        case 4:
            printf("You have chosen 4\n");
            printf("Enter the ID of the e-mail: ");
            int id;
            scanf("%d", &id);
            if((id>num) && (id<1)){
                printf("There is no e-mail with this ID\n");
                break;
            }
            binSearch(emails, num, id);

            break;
        case 5:
            printf("You have chosen 5\n");
            printEmails(emails, num);
            break;
        case 6:
            printf("You have chosen 6\n");
            exit(1);
        default:
            printf("Invalid choice\n");
            break;
        }
    } while (1);
}//time complexity: O(n) Reason: there is a do while loop that iterates n times



    int main() {
        printf("Enter the path of the directory which contains data files: ");
        char path[100];
        scanf("%s", path);
        printf("\nEnter the number of data files:\n");
        int num;
        scanf("%d", &num);
        struct email *emails = readEmails(path, num);
        menu(emails, num);


        return 0;
    }

int count_words(char *str) {
    int count = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            count++;
        }
        i++;
    }
    return count+1;
}//time complexity: O(n) Reason: there is a while loop that iterates n times
void printEmails(struct email *emails, int num) {
    for (int i = 0; i<num; i++) {
        printf("Email ID: %d \n From: %s \n To: %s \n Date: %d \n Content: %s \n", emails[i].email_id, emails[i].sender, emails[i].recipient, emails[i].day_of_the_month, emails[i].content);
    }
}//time complexity: O(n) Reason: there is a for loop that iterates n times
void swap (struct email *a, struct email *b) {
    struct email temp = *a;
    *a = *b;
    *b = temp;
}//time complexity: O(1) Reason: there is no loop
void heapify(struct email *emails, int n, int i, int sorting_criteria) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (sorting_criteria == 1) {
        if (l < n && emails[l].email_id > emails[largest].email_id)
            largest = l;
        if (r < n && emails[r].email_id > emails[largest].email_id)
            largest = r;
    }
    else if (sorting_criteria == 2) {
        if (l < n && count_words(emails[l].content) > count_words(emails[largest].content))
            largest = l;
        if (r < n && count_words(emails[r].content) > count_words(emails[largest].content))
            largest = r;
    }
    else if (sorting_criteria == 3) {
        if (l < n && emails[l].day_of_the_month > emails[largest].day_of_the_month)
            largest = l;
        if (r < n && emails[r].day_of_the_month > emails[largest].day_of_the_month)
            largest = r;
    }
    if (largest != i) {
        swap(&emails[i], &emails[largest]);
        heapify(emails, n, largest, sorting_criteria);
    }
}//time complexity: O(n) Reason: the function calls itself recursively n times
void buildHeap(struct email *emails, int n, int sorting_criteria) {
    for (int i = n/2 - 1; i >= 0; i--) {
        heapify(emails, n, i,sorting_criteria);
    }
}//time complexity: O(n) Reason: there is a for loop that iterates n times

void heapSort(struct email *emails, int n, int sorting_criteria) {
    buildHeap(emails, n,sorting_criteria);
    for (int i = n - 1; i >= 0; i--) {
        swap(&emails[0], &emails[i]);
        heapify(emails, i, 0,sorting_criteria);
    }
}//time complexity: O(n) Reason: there is a for loop that iterates n times


char *int_to_string(int num) {
    char *str = malloc(10);
    sprintf(str, "%d", num);
    return str;
}//time complexity: O(1) Reason: there is no loop

char *concatenate(char *path, int num) {
    char *str = malloc(100);
    strcpy(str, path);
    strcat(str, "/");
    strcat(str, int_to_string(num));
    strcat(str, ".txt");
    return str;
}//time complexity: O(1) Reason: there is no loop

struct email *readEmails(char *path, int num) {
    FILE *fp;
    struct email *emails = malloc(sizeof(struct email) * num);
    char *file_path = malloc(100);
    strcpy(file_path, path);
    printf("****************\nOpening file %s...\n", path);
    int j = 1;
    for (int i = 0; i<num; i++) {
        path = concatenate(file_path, j);
        fp = fopen(path, "r");
        if (fp == NULL) {
            printf("Entered number is more than the existing files\n");
            exit(1);
        }
        //read the file and store the data in the structure
        emails[i].content = malloc(100);
        fscanf(fp, "%d \n From: %s \n To: %s \n Date: %d \n ", &emails[i].email_id, emails[i].sender, emails[i].recipient, &emails[i].day_of_the_month);
        fgets(emails[i].content, 100, fp);
        fclose(fp);
        j++;
    }
    printf("%d mails have been read succesfully.\n", num);
    printf("****************\n");
    return emails;
}//time complexity: O(n) Reason: there is a for loop that iterates n times

void binSearch(struct email *emails, int num, int id) {
    heapSort(emails, num, 1);
    int low = 0;
    int high = num - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (emails[mid].email_id == id) {
            printf("Email ID: %d \n From: %s \n To: %s \n Date: %d \n Content: %s \n", emails[mid].email_id, emails[mid].sender, emails[mid].recipient, emails[mid].day_of_the_month, emails[mid].content);
            return;
        }
        else if (emails[mid].email_id < id) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    printf("Email not found\n");
}//time complexity: O(logn) Reason: there is a while loop that iterates logn times

