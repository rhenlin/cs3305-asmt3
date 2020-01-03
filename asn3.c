/**
 * student name: yuying chen
 * student number:250991675
 * student id: yche982
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * define a bank account structure to save data
 */
struct bankAccount{
    int number;
    int type;
    int balance;
    int depositfee;
    int withdrawfee;
    int transferfee;
    int transactionfee;
    int transactions;
    int overdraft;
    int overdraftfee;
};

pthread_mutex_t lock; //create a lock
struct bankAccount accounts[300];


/**
 * main program
 * @return 0
 */
int main(){
    pthread_mutex_init(&lock, NULL);
    int size = 0;
    FILE* fptr;
    fptr = fopen("assignment_3_input_file.txt", "r"); //open file
    char line[256];

//-----------------------------when the first character is a--------------------------------

    while (fgets(line,256, fptr) && (line[0] == 'a')){


        accounts[size].number = size+1; //get the account number
        accounts[size].balance = 0; //initial balance is 0
        char *ptr = strtok(line, " ");
        ptr = strtok(NULL, " ");
        ptr = strtok(NULL, " ");
        if (strcmp(ptr,"personal")==0){ //check if it is personal account
            accounts[size].type = 0; //personal account
        }
        else{
            accounts[size].type = 1; //business account
        }
        ptr = strtok(NULL, " ");
        ptr = strtok(NULL, " ");
        accounts[size].depositfee = atoi(ptr);      //get deposit fee
        ptr = strtok(NULL, " ");
        ptr = strtok(NULL, " ");
        accounts[size].withdrawfee = atoi(ptr); //get withdraw fee
        ptr = strtok(NULL, " ");
        ptr = strtok(NULL, " ");
        accounts[size].transferfee = atoi(ptr); //get transfer fee
        ptr = strtok(NULL, " ");
        ptr = strtok(NULL, " ");
        accounts[size].transactions = atoi(ptr); //get transaction fee
        ptr = strtok(NULL, " ");
        accounts[size].transactionfee = atoi(ptr); //get transaction fee
        ptr = strtok(NULL, " ");
        ptr = strtok(NULL, " ");
        if (strcmp(ptr,"Y")==0){ //check if this account have overdraft protection
            accounts[size].overdraft = 1; //has overdraft protection
            ptr = strtok(NULL, " ");
            accounts[size].overdraftfee = atoi(ptr); //get overdraft fee
        }
        else{
            accounts[size].overdraft = 0; //have no overdraft protection and fee is 0
            accounts[size].overdraftfee = 0;
        }
        size++;
    }


    //------------------------------when the first character is d--------------------------


    char *ptr = strtok(line, " ");
    ptr = strtok(NULL, " ");
    ptr = strtok(NULL, " ");

    while (ptr != NULL){
        int account, amount,i;
        char num[10] = ""; //save the account number
        for(i = 0; i<strlen(ptr);i++){
            num[i] = ptr[i+1];

        }
        account = atoi(num);

        ptr = strtok(NULL, " ");

        amount = atoi(ptr);


        //depositors deposit money into account
        if(amount - accounts[account-1].depositfee>0|| (accounts[account-1].overdraft==1&&amount - accounts[account].depositfee>-5000)) {

            accounts[account - 1].balance = accounts[account - 1].balance + amount;
            accounts[account].transactions =accounts[account].transactions-1;

            pthread_mutex_unlock(&lock);

        }


        if (ptr != NULL){
            ptr = strtok(NULL, " ");

        }
        if (ptr != NULL){
            ptr = strtok(NULL, " ");
        }

    }


    while (fgets(line,256, fptr) && (line[0] == 'd')){

        char *pt1 = strtok(line, " ");

        pt1 = strtok(NULL, " ");

        pt1 = strtok(NULL, " ");



        while (pt1 != NULL){
            int account, amount,i;
            char num[10] = "";
            for(i = 0; i<strlen(pt1);i++){
                num[i] = pt1[i+1];

            }
            account = atoi(num);

            pt1 = strtok(NULL, " ");


            amount = atoi(pt1);

            if(amount - accounts[account-1].depositfee>0|| (accounts[account-1].overdraft==1&&amount - accounts[account].depositfee>-5000)) {


                accounts[account - 1].balance = accounts[account - 1].balance + amount;
                accounts[account].transactions =accounts[account].transactions-1;
                pthread_mutex_unlock(&lock);
            }

            if (pt1 != NULL){
                pt1 = strtok(NULL, " ");

            }
            if (pt1 != NULL){
                pt1 = strtok(NULL, " ");
            }

        }

    }
    //---------------------------------when the first character is c---------------------------


    char *pt1 = strtok(line, " ");

    while (pt1 != NULL) {
        int account1, account2,amount,i;
        char num[10] = ""; //save the account number
        pt1 = strtok(NULL, " ");
        if (pt1 == NULL) {
            break;
        }


        if (strcmp(pt1, "d") == 0) { //if the type is deposit
                pt1 = strtok(NULL, " ");
                for (i = 0; i < strlen(pt1); i++) { //get account number and transaction amount
                    num[i] = pt1[i + 1];
                    account1 = atoi(num);

                }

                pt1 = strtok(NULL, " ");
                amount = atoi(pt1);




            int transFee;
            //if transaction limit is reached then we have a transaction fee
            if(accounts[account1-1].transactions>1){
                transFee = 0;
            }
            //otherwise transaction fee is 0
            else{
                transFee = accounts[account1-1].transactionfee;
            }

            //check balance is available for transaction
            if(accounts[account1-1].balance+amount-accounts[account1-1].depositfee-transFee>0||(accounts[account1-1].overdraft==1&&accounts[account1-1].balance+amount-accounts[account1-1].depositfee-transFee>-5000)){
                pthread_mutex_unlock(&lock);
                accounts[account1-1].balance = accounts[account1-1].balance + amount - accounts[account1-1].depositfee - transFee;
                accounts[account1-1].transactions =accounts[account1-1].transactions-1;
                pthread_mutex_unlock(&lock);
            }

            //calculate the overdraft range
            if(accounts[account1-1].balance<0&&accounts[account1-1].balance>-5000){
                int plus = -accounts[account1-1].balance;
                int overfee = (plus-1)/(500+1);
                overfee = overfee*accounts[account1-1].overdraftfee;
                accounts[account1-1].balance = accounts[account1-1].balance - overfee;
            }


        }
        else if (strcmp(pt1, "w") == 0) { //if the type is withdraw

            pt1 = strtok(NULL, " ");
            for (i = 0; i < strlen(pt1); i++) { //get account number
                num[i] = pt1[i + 1];
                account1 = atoi(num);

            }
            pt1 = strtok(NULL, " ");
            amount = atoi(pt1);
            int transFee;
            //if transaction limit is reached then we have a transaction fee, otherwise it is 0
            if(accounts[account1-1].transactions>1){
                transFee = 0;
            }
            else{
                transFee = accounts[account1-1].transactionfee;
            }

            //check balance is available for transaction
            if(accounts[account1-1].balance - amount-accounts[account1-1].withdrawfee-transFee>0||(accounts[account1-1].overdraft==1&&accounts[account1-1].balance-amount-accounts[account1-1].withdrawfee-transFee>-5000)){
                accounts[account1-1].balance = accounts[account1-1].balance - amount - accounts[account1-1].withdrawfee - transFee;
                accounts[account1-1].transactions =accounts[account1-1].transactions-1;
                pthread_mutex_unlock(&lock);
            }


            //calculate the overdraft range
            if(accounts[account1-1].balance<0&&accounts[account1-1].balance>-5000){
                int plus = -accounts[account1-1].balance;
                int overfee = (plus-1)/(500+1);
                overfee = overfee*accounts[account1-1].overdraftfee;
                accounts[account1-1].balance = accounts[account1-1].balance - overfee;
            }

        }
        else if (strcmp(pt1, "t")==0){ //if the type is transfer
            pt1 = strtok(NULL, " ");
            for (i = 0; i < strlen(pt1); i++) { //get 1st account number
                num[i] = pt1[i + 1];
                account1 = atoi(num);

            }
            pt1 = strtok(NULL, " ");
            for (i = 0; i < strlen(pt1); i++) { //get 2nd account number
                num[i] = pt1[i + 1];
                account2 = atoi(num);

            }
            pt1 = strtok(NULL, " ");
            amount = atoi(pt1);


            int transFee1,transFee2;
            //calculate transaction fees of 1st and 2nd account
            if(accounts[account1-1].transactions>1){
                transFee1 = 0;
            }
            else{
                transFee1 = accounts[account1-1].transactionfee;
            }

            if(accounts[account2-1].transactions>1){
                transFee2 = 0;
            }
            else{
                transFee2 = accounts[account2-1].transactionfee;
            }

            //check balance is available for transaction
            if(accounts[account1-1].balance-amount - transFee1 - accounts[account1-1].transferfee>0||(accounts[account1-1].overdraft==1&&accounts[account1-1].balance-amount-transFee1-accounts[account1-1].transferfee>-5000)){
                accounts[account1-1].balance = accounts[account1-1].balance - amount - accounts[account1-1].transferfee-transFee1;
                accounts[account1-1].transactions =accounts[account1-1].transactions-1;
                pthread_mutex_unlock(&lock);
            }

            //check balance is available for transaction
            if(accounts[account2-1].balance-amount - transFee2 - accounts[account2-1].transferfee>0||(accounts[account2-1].overdraft==1&&accounts[account2-1].balance-amount-transFee2-accounts[account2-1].transferfee>-5000)){
                accounts[account2-1].balance = accounts[account2-1].balance + amount - accounts[account2-1].transferfee-transFee2;
                accounts[account2-1].transactions =accounts[account2-1].transactions-1;
                pthread_mutex_unlock(&lock);
            }

            //calculate the overdraft range
            if(accounts[account1-1].balance<0&&accounts[account1-1].balance>-5000){
                int plus = -accounts[account1-1].balance;
                int overfee = (plus-1)/500+1;
                overfee = overfee*accounts[account1-1].overdraftfee;
                accounts[account1-1].balance = accounts[account1-1].balance - overfee;
            }
            //calculate the overdraft range
            if(accounts[account2-1].balance<0&&accounts[account2-1].balance>-5000){
                int plus = -accounts[account2-1].balance;
                int overfee = (plus-1)/500+1;
                overfee = overfee*accounts[account2-1].overdraftfee;
                accounts[account2-1].balance = accounts[account2-1].balance - overfee;
            }

        }

        }

    while (fgets(line, 256, fptr)&&line[0]=='c') {
        char *ptr = strtok(line, " ");
        int act1, act2, amt, i;
        char num[10] = "";
        while (ptr != NULL) {
            ptr = strtok(NULL, " ");
            if (ptr == NULL) {
                break;
            }
            if (strcmp(ptr, "d") == 0) { //the type is deposit
                ptr = strtok(NULL, " ");
                for (i = 0; i < strlen(ptr); i++) { //get account number
                    num[i] = ptr[i + 1];
                    act1 = atoi(num);

                }

                ptr = strtok(NULL, " ");
                amt = atoi(ptr);

                int transFee;
                //calculate transaction fee
                if(accounts[act1-1].transactions>1){
                    transFee = 0;
                }
                else{
                    transFee = accounts[act1-1].transactionfee;
                }

                //check if the balance is available
                if(accounts[act1-1].balance+amt-accounts[act1-1].depositfee-transFee>0||(accounts[act1-1].overdraft==1&&accounts[act1-1].balance+amt-accounts[act1-1].depositfee-transFee>-5000)){
                    accounts[act1-1].balance = accounts[act1-1].balance + amt - accounts[act1-1].depositfee - transFee;
                    accounts[act1-1].transactions =accounts[act1-1].transactions-1;
                    pthread_mutex_unlock(&lock);
                }

                //calcute the overdraft range
                if(accounts[act1-1].balance<0&&accounts[act1-1].balance>-5000){
                    int plus = -accounts[act1-1].balance;
                    int overfee = (plus-1)/(500+1);
                    overfee = overfee*accounts[act1-1].overdraftfee;
                    accounts[act1-1].balance = accounts[act1-1].balance - overfee;
                }
            }
            else if (strcmp(ptr, "w") == 0) { //type is withdraw

                ptr = strtok(NULL, " ");
                for (i = 0; i < strlen(ptr); i++) { //get account number
                    num[i] = ptr[i + 1];
                    act1 = atoi(num);

                }
                ptr = strtok(NULL, " ");
                amt = atoi(ptr);
                int transFee;
                //calculate the transFee
                if(accounts[act1-1].transactions>1){
                    transFee = 0;
                }
                else{
                    transFee = accounts[act1-1].transactionfee;
                }

                //check balance availibility
                if(accounts[act1-1].balance - amt-accounts[act1-1].withdrawfee-transFee>0||(accounts[act1-1].overdraft==1&&accounts[act1-1].balance-amt-accounts[act1-1].withdrawfee-transFee>-5000)){
                    accounts[act1-1].balance = accounts[act1-1].balance - amt - accounts[act1-1].withdrawfee - transFee;
                    accounts[act1-1].transactions =accounts[act1-1].transactions-1;
                    pthread_mutex_unlock(&lock);
                }

                //caculate the overdraft range

                if(accounts[act1-1].balance<0&&accounts[act1-1].balance>-5000){
                    int plus = -accounts[act1-1].balance;
                    int overfee = (plus-1)/(500+1);
                    overfee = overfee*accounts[act1-1].overdraftfee;
                    accounts[act1-1].balance = accounts[act1-1].balance - overfee;
                }
            }

            else if (strcmp(ptr, "t")==0){ //type is tranfer
                ptr = strtok(NULL, " ");
                for (i = 0; i < strlen(ptr); i++) {
                    num[i] = ptr[i + 1]; //get 1st account number
                    act1 = atoi(num);

                }
                ptr = strtok(NULL, " ");
                for (i = 0; i < strlen(ptr); i++) { //get 2nd account number
                    num[i] = ptr[i + 1];
                    act2 = atoi(num);

                }
                ptr = strtok(NULL, " ");
                amt = atoi(ptr);
                int transFee1,transFee2;

                //calculate the transaction fee
                if(accounts[act1-1].transactions>1){
                    transFee1 = 0;
                }
                else{
                    transFee1 = accounts[act1-1].transactionfee;
                }
                if(accounts[act2-1].transactions>1){
                    transFee2 = 0;
                }
                else{
                    transFee2 = accounts[act2-1].transactionfee;
                }
                //check balance is available for transaction in account1

                if(accounts[act1-1].balance-amt - transFee1 - accounts[act1-1].transferfee>0||(accounts[act1-1].overdraft==1&&accounts[act1-1].balance-amt-transFee1-accounts[act1-1].transferfee>-5000)){
                    accounts[act1-1].balance = accounts[act1-1].balance - amt - accounts[act1-1].transferfee-transFee1;
                    accounts[act1-1].transactions =accounts[act1-1].transactions-1;
                    pthread_mutex_unlock(&lock);
                }
                //check balance is available for transaction in account2

                if(accounts[act2-1].balance-amt - transFee2 - accounts[act2-1].transferfee>0||(accounts[act2-1].overdraft==1&&accounts[act2-1].balance-amt-transFee2-accounts[act2-1].transferfee>-5000)){
                    accounts[act2-1].balance = accounts[act2-1].balance + amt - accounts[act2-1].transferfee-transFee2;
                    accounts[act2-1].transactions =accounts[act2-1].transactions-1;
                    pthread_mutex_unlock(&lock);
                }
                //check overdraft range for 1st and 2nd account

                if(accounts[act1-1].balance<0&&accounts[act1-1].balance>-5000){
                    int plus = -accounts[act1-1].balance;
                    int overfee = (plus-1)/500+1;
                    overfee = overfee*accounts[act1-1].overdraftfee;
                    accounts[act1-1].balance = accounts[act1-1].balance - overfee;
                }
                if(accounts[act2-1].balance<0&&accounts[act2-1].balance>-5000){
                    int plus = -accounts[act2-1].balance;
                    int overfee = (plus-1)/500+1;
                    overfee = overfee*accounts[act2-1].overdraftfee;
                    accounts[act2-1].balance = accounts[act2-1].balance - overfee;
                }

            }
        }
    }

    int t;

    pthread_mutex_destroy(&lock);

    FILE *fptr1;
    fptr1 = fopen("assignment_3_output_file.txt", "w"); //write the result in to output file

    for (t =0; t<=size-1; t++){ // print out the result in order
        if (accounts[t].type == 0){

            printf("a%d type personal %d \n", accounts[t].number, accounts[t].balance);
            fprintf(fptr1,"a%d type personal %d \n", accounts[t].number, accounts[t].balance);
        }

        else{
            printf("a%d type business %d \n", accounts[t].number, accounts[t].balance);
            fprintf(fptr1,"a%d type business %d \n", accounts[t].number, accounts[t].balance);

        }
    }
    return 0;






}
