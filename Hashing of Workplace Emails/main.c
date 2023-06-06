#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TABLE_SIZE = 11; // initial size of the hash table
int INSERTED = 0;  // number of elements inserted into the hash table
float LOAD_FACTOR = 0.0; // load factor of the hash table
struct email{ // struct for the emails
    int email_id;
    char sender[51];
    char recipient[51];
    int day_of_the_month;
    char *content;
    int no_of_words;
};
int hash(int email_id, char *sender) { // hash function
    int key = email_id + sender[0] - 65;
    return key % TABLE_SIZE;
}
int hash2(int email_id, char *sender) { // hash function that is used for double hashing
    int key = email_id + sender[0] - 65;
    return 5-(key % 5);
}

void initialize(struct email **hash_table) { // initialize the hash table with all elements set to NULL
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
}
void print_hash_table(struct email **hash_table) { // print the hash table with the required columns and rows
    printf("\tIndex\tEmail ID\tSender\t\tRecipient\tDay\tWords\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] == NULL) {
            printf("\t%i\t---\t\t---\t\t---\t\t---\t---\n", i);
        } else {
            printf("\t%i\t%i\t\t%s\t\t%s\t\t%i\t%i\n",i, hash_table[i]->email_id, hash_table[i]->sender, hash_table[i]->recipient, hash_table[i]->day_of_the_month, hash_table[i]->no_of_words);
        }
    }
}
int search_e_mail(int key, char *sender, struct email **hash_table) { // search for an email in the hash table
    int hash_index = hash(key, sender); // get the hash index
    int hash_index2 = hash2(key, sender); // get the hash index for double hashing
    for (int i = 0; i < TABLE_SIZE; i++) {
        int probe = (hash_index + i * hash_index2) % TABLE_SIZE;
        if (hash_table[probe] != NULL) {
            if (hash_table[probe]->email_id == key && strcmp(hash_table[probe]->sender, sender) == 0) {
                printf("Recipient: %s\n", hash_table[probe]->recipient);
                printf("Date: %d\n", hash_table[probe]->day_of_the_month);
                printf("Number of words: %d\n", hash_table[probe]->no_of_words);
                return 1;
            }
        }
    }
    return 0;
}
//insert email into hash table using linear probing
void insert_linear_probing(int key, struct email *email,struct email **hash_table) {
    int hash_index = hash(key, email->sender); // get the hash index
    if(search_e_mail(key, email->sender, hash_table) == 1) { // check if the email already exists
        printf("Email already exists\n");
        return;
    }
    for(int i = 0; i < TABLE_SIZE; i++){ // linear probing
        int probe = (hash_index + i) % TABLE_SIZE;
        if(hash_table[probe] == NULL){
            hash_table[probe] = email;
            INSERTED++;
            LOAD_FACTOR = (float)INSERTED / (float)TABLE_SIZE;
            return;
        }
    }
    if(hash_table[hash_index] != NULL) //
    {
        printf("Collision occurred\n");
    }
    hash_table[hash_index] = email;
}
//insert email into hash table using double hashing
void insert_double_hashing(int key, struct email *email,struct email **hash_table) {
    int hash_index = hash(key, email->sender); // get the hash index
    int hash_index2 = hash2(key, email->sender); // get the hash index for double hashing
    if(search_e_mail(key, email->sender, hash_table) == 1){ // check if the email already exists
        printf("Email already exists\n");
        return;
    }
    for(int i = 0; i < TABLE_SIZE; i++){ // double hashing
        int probe = (hash_index + i*hash_index2) % TABLE_SIZE;
        if(hash_table[probe] == NULL){
            hash_table[probe] = email;
            INSERTED++;
            LOAD_FACTOR = (float)INSERTED / (float)TABLE_SIZE;
            return;
        }
    }
    if(hash_table[hash_index] != NULL)
    {
        printf("Collision occurred\n");
    }
    hash_table[hash_index] = email;
}
int count_words(char *str) { // count the number of words in the email content
    int count = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            count++;
        }
        i++;
    }
    return count+1;
}
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
struct email *readEmails(char *path, int num) { // read the emails from the given path
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
        emails[i].no_of_words = count_words(emails[i].content);
        fclose(fp);
        j++;
    }
    printf("%d mails have been read successfully.\n", num);
    printf("****************\n");
    return emails;
}
int is_n_prime(int n) { // check if the number is prime
    if (n <= 1) {
        return 0;
    }
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}
int next_prime(int n) { // find the next prime number
    if (n <= 1) {
        return 2;
    }
    int prime = n;
    int found = 0;
    while (found == 0) {
        prime++;
        if (is_n_prime(prime)) {
            found = 1;
        }
    }
    return prime;
}
struct email **rehash(struct email **hash_table, int hash_type) { // rehash the hash table
    int old_size = TABLE_SIZE;
    TABLE_SIZE = next_prime(TABLE_SIZE*2); // double the size of the table and find the next prime number for the new size
    struct email **new_hash_table = malloc(sizeof(struct email *) * TABLE_SIZE); //dynamic allocation of the new hash table
    initialize(new_hash_table); // initialize the new hash table
    for (int i = 0; i < old_size; i++) {
        if (hash_table[i] != NULL) {
            if (hash_type == 1) {
                insert_double_hashing(hash_table[i]->email_id, hash_table[i], new_hash_table);
            } else {
                insert_linear_probing(hash_table[i]->email_id, hash_table[i], new_hash_table);
            }
        }
    }
    free(hash_table); // destroy the old hash table
    return new_hash_table;
}

    int main() {
        struct email** hash_table = malloc(sizeof(struct email *) * TABLE_SIZE); // dynamic allocation of the hash table
        initialize(hash_table); // initialize the hash table
        print_hash_table(hash_table); // print the hash table before inserting the emails to see the empty table
        printf("Enter the path of the directory which contains data files: ");
        char path[100];
        scanf("%s", path);
        printf("\nEnter the number of data files:\n");
        int num;
        scanf("%d", &num);
        struct email *emails = malloc(sizeof(struct email) * num); // dynamic allocation of the emails
        emails = readEmails(path, num); // read the emails from the given path

        //menu
        int choice;
        do { //menu for the user to choose the type of hashing
            printf("\n1. Insert emails into hash table using double hashing\n");
            printf("2. Insert emails into hash table using linear probing\n");
            printf("3. Search an email\n");
            printf("4. Print hash table\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    for (int i = 0; i < num; i++) {
                        if(LOAD_FACTOR > 0.5)
                        { // if the load factor is greater than 0.5, rehash the hash table
                            printf("Rehashing...\n");
                            hash_table = rehash(hash_table,1);
                        }
                        insert_double_hashing(emails[i].email_id, &emails[i], hash_table);
                        printf("Email %d inserted into hash table using double hashing\n", emails[i].email_id);
                        print_hash_table(hash_table);
                    }
                    break;
                case 2:
                    for (int i = 0; i < num; i++) {
                        if(LOAD_FACTOR > 0.5)
                        { // if the load factor is greater than 0.5, rehash the hash table
                            printf("Rehashing...\n");
                            hash_table = rehash(hash_table,2);
                        }
                        insert_linear_probing(emails[i].email_id, &emails[i], hash_table);
                        printf("Email %d inserted into hash table using linear probing\n", emails[i].email_id);
                        print_hash_table(hash_table);
                    }
                    break;
                case 3:
                    printf("Enter unique identifier: ");
                    int key;
                    scanf("%d", &key);
                    printf("Enter the sender: ");
                    char sender[100];
                    scanf("%s", sender);
                    if (search_e_mail(key, sender, hash_table)) {
                        printf("Email found\n");
                    } else {
                        printf("Email not found\n");
                    }
                    break;
                case 4:
                    print_hash_table(hash_table);
                    printf("Load factor: %f\n", LOAD_FACTOR);
                    printf("Number of emails inserted: %d\n", INSERTED);
                    printf("Size of the table: %d\n", TABLE_SIZE);
                    break;
                case 5:
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice\n");
            }
        } while (choice != 5);

        return 0;
    }


