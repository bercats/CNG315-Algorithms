#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Berke Diler 2401503
 * CNG 315 - Assignment 4
 * e-mail spam analyzer
 */
//A structure for e-mails
typedef struct email{
    int emailID;
    char sender[51];
    char receipent[51];
    int day_of_the_month;
    char isSpam[8];
    char *content;
}email;
//linked list for spam e-mails
typedef struct spam {
    email *e;
    struct spam *next;
}spam;
//linked list for non-spam e-mails
typedef struct nonspam {
    email *e;
    struct nonspam *next;
}nonspam;
//function prototypes
char *int_to_string(int num);
char *concatenate(char *path, int num);
void readEmails(char *path, int numberOfFiles, spam **spamList, nonspam **nonspamList);
void searchPattern(char *pattern, spam *spamList, nonspam *nonspamList);
int rabinKarpMatch(char *text, char *pattern, int d, int q);
char* highlightPattern(char *text, char *pattern);
int main() {
    //create two empty linked lists
    spam *spamList = NULL;
    nonspam *nonspamList = NULL;
    //ask for the path to the directory which contains the data files
    char path[100];
    printf("Enter the path to the directory which contains the data files: ");
    scanf("%s", path);
    //ask for the number of data files
    int numberOfFiles;
    printf("Enter the number of data files: ");
    scanf("%d", &numberOfFiles);
    //call the readEmails function
    readEmails(path, numberOfFiles, &spamList, &nonspamList);
    //ask for a pattern to search
    char pattern[100];
    printf("\nEnter a pattern to search: ");
    scanf("%s", pattern);
    //call the searchPattern function
    searchPattern(pattern, spamList, nonspamList);

    return 0;
}
//function definitions
char *int_to_string(int num) { // convert an integer to a string
    char *str = malloc(10);
    sprintf(str, "%d", num);
    return str;
}
char *concatenate(char *path, int num) { // concatenate the path with the number of the email
    char *str = malloc(100);
    strcpy(str, path);
    strcat(str, "/");
    strcat(str, int_to_string(num));
    strcat(str, ".txt");
    return str;
}

void readEmails(char *path, int numberOfFiles, spam **spamList, nonspam **nonspamList){
    FILE *fp;
    struct email *emails = malloc(numberOfFiles * sizeof(struct email));
    char *filepath = malloc(sizeof(char) * 100);
    strcpy(filepath, path);
    //count the number of files in the directory
    int count = 0;
    while(fp != NULL){
        count++;
        fp = fopen(filepath, "r");
    }
    //check if the number of files is more than the one found in the entered directory
    if(count > numberOfFiles){
        printf("The number of files is more than the one found in the entered directory!");
        exit(1);
    }
    //read the files
    printf("Reading the files...");
    int j = 1;
    for(int i = 0; i<numberOfFiles ; i++)
    {
        path = concatenate(filepath, j);
        fp = fopen(path, "r");
        if(fp == NULL){
            printf("The file does not exist!");
            exit(1);
        }
        emails[i].content = malloc(1000);
        fscanf(fp, "%d \n From: %s \n To: %s \n Date: %d \n Type: %s \n", &emails[i].emailID, emails[i].sender, emails[i].receipent, &emails[i].day_of_the_month, emails[i].isSpam);
        fgets(emails[i].content, 1000, fp);
       //if the e-mail is spam, add it to the spam linked list
        if(strcmp(emails[i].isSpam, "Spam") == 0){
            spam *newSpam = malloc(sizeof(spam));
            newSpam->e = &emails[i];
            newSpam->next = *spamList;
            *spamList = newSpam;
        }
        //if the e-mail is not spam, add it to the non-spam linked list
        else{
            nonspam *newNonspam = malloc(sizeof(nonspam));
            newNonspam->e = &emails[i];
            newNonspam->next = *nonspamList;
            *nonspamList = newNonspam;
        }
        fclose(fp);
        j++;
    }

}
char* highlightPattern(char *text, char *pattern) {
    int n = strlen(text);
    int m = strlen(pattern);
    char *result = malloc(sizeof(char) * (n + 2 * m + 1));
    int j = 0;
    int k = 0;
    while (j < n) {
        int i;
        for (i = 0; i < m; i++) {
            if (j + i >= n || text[j + i] != pattern[i]) {
                break;
            }
        }
        if (i == m) {
            result[k++] = '[';
            for (int l = 0; l < m; l++) {
                result[k++] = text[j++];
            }
            result[k++] = ']';
        } else {
            result[k++] = text[j++];
        }
    }
    result[k] = '\0';
    return result;
}

//function to search for a pattern in the e-mails
void searchPattern(char *pattern, spam *spamList, nonspam *nonspamList) {
    // constants for Rabin-Karp algorithm
    const int d = 128;
    const int q = 256;

    // print spam emails containing the pattern
    printf("Spam emails containing the pattern:\n");
    int spamCount = 0;
    int totalCount = 0;
    spam *temp = spamList;
    while (temp != NULL) {
        int count = rabinKarpMatch(temp->e->content, pattern, d, q);
        if (count > 0) {
            printf("%d. %d pattern(s) detected in email\n", ++spamCount, count);
            printf("%s\n", highlightPattern(temp->e->content, pattern));
            totalCount += count;
        }
        temp = temp->next;
    }
    printf("%d pattern(s) detected in %d email(s)\n", totalCount, spamCount);

    // print non-spam emails containing the pattern
    printf("Non-spam emails containing the pattern:\n");
    int nonspamCount = 0;
    int totalCount2 = 0;
    nonspam *temp2 = nonspamList;
    while (temp2 != NULL) {
        int count = rabinKarpMatch(temp2->e->content, pattern, d, q);
        if (count > 0) {
            printf("%d. %d pattern(s) detected in email\n", ++nonspamCount, count);
            printf("%s\n", highlightPattern(temp2->e->content, pattern));
            totalCount2 += count;
        }
        temp2 = temp2->next;
    }
    printf("%d pattern(s) detected in %d email(s)\n", totalCount2, nonspamCount);
}
//function to find the number of times a pattern appears in a text
int rabinKarpMatch(char *text, char *pattern, int d, int q) {
    int n = strlen(text);
    int m = strlen(pattern);
    int h = 1;
    int p = 0;
    int t = 0;
    int count = 0;
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }
    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }
    for (int s = 0; s <= n - m; s++) {
        if (p == t) {
            int j;
            for (j = 0; j < m; j++) {
                if (text[s + j] != pattern[j]) {
                    break;
                }
            }
            if (j == m) {
                count++;
            }
        }
        if (s < n - m) {
            t = (d * (t - text[s] * h) + text[s + m]) % q;
            if (t < 0) {
                t += q;
            }
        }
    }
    return count;
}
