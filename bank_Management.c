#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int create_account();
void Deposit_Money();
void Withdraw_Money();
void Check_Balance();
void login();
void choose_fun();
int get_nxt_acc_no();

const char *ACCOUNT_FILE = "account.dat";

typedef struct
{
    char name[60];
    int acc_no;
    float balance;
    char password[50];
} Account;

int main()
{
    while (1)
    {
        int choose;
        printf("\n\n*** Bank Management System ***\n");
        printf("\n1. Create Account");
        printf("\n2. Login");
        printf("\n3. EXIT");
        printf("\nEnter Your Choice... ");
        if (scanf("%d", &choose) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        switch (choose)
        {
        case 1:
            create_account();
            break;
        case 2:
            login();
            break;
        case 3:
            printf("Thanks for Visit");
            return 0;
            break;
        default:
            return 0;
        }
    }

    return 0;
}
void login()
{
    Account acc = {0};
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL)
    {
        printf("Unable to Open File!!");
        return;
    }
    int acc_no, flag = 0;
    char password[50];
    int ch;
    printf("Enter Account Number: ");
    if (scanf("%d", &acc_no) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Invalid account number input\n");
        fclose(file);
        return;
    }
    while ((ch = getchar()) != '\n' && ch != EOF); // clear rest of line
    printf("Enter Your Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("Error reading password\n");
        fclose(file);
        return;
    }
    password[strcspn(password, "\r\n")] = '\0';

    while (fread(&acc, sizeof(acc), 1, file))
    {
        if (acc.acc_no == acc_no)
        {
            acc.password[strcspn(acc.password, "\r\n")] = '\0';

            if (strcmp(acc.password, password) == 0)
            {
                flag = 1;
                break;
            }
            else
            {
                // Helpful debug output — comment out in production if you don't want to reveal passwords
                printf("stored:[%s] entered:[%s]\n", acc.password, password);
            }
        }
    }
    fclose(file);

    if (flag == 1)
    {
        printf("Login Successful, %s\n", acc.name);
        choose_fun();
    }
    else
    {
        printf("Invalid Credentials!!\n");
    }
}

void choose_fun()
{
    while (1)
    {
        int choice;
        printf("\n1. Deposit Money");
        printf("\n2. Withdraw Money");
        printf("\n3. Check Balance");
        printf("\n4. EXIT");
        printf("\nChoose an option: ");
        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Invalid input\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            Deposit_Money();
            break;
        case 2:
            Withdraw_Money();
            break;
        case 3:
            Check_Balance();
            break;
        case 4:
            printf("Closing The Bank");
            return;
            break;
        default:
            printf("Invalid Choice !!!");
            break;
        }
    }
}
int create_account()
{
    Account acc = {0};
    FILE *file = fopen(ACCOUNT_FILE, "ab+");
    if (file == NULL)
    {
        printf("Unable to Open File!!");
        return 0;
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("Enter Name: ");
    if (fgets(acc.name, sizeof(acc.name), stdin) == NULL) {
        fclose(file);
        printf("Error reading name\n");
        return 0;
    }
    acc.name[strcspn(acc.name, "\r\n")] = '\0';

    printf("Enter Your Password: ");
    if (fgets(acc.password, sizeof(acc.password), stdin) == NULL) {
        fclose(file);
        printf("Error reading password\n");
        return 0;
    }
    acc.password[strcspn(acc.password, "\r\n")] = '\0';

    acc.balance = 0;
    acc.acc_no = get_nxt_acc_no();
    if (acc.acc_no == 0)
    {
        fclose(file);
        printf("Failed to Generate Account Number");
        return 0;
    }
    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);

    printf("\nAccount Owner Name : %s", acc.name);
    printf("\nAccount Number : %d", acc.acc_no);
    printf("\nAccount Balance : %.2f", acc.balance);
    printf("\nAccount Password: %s", acc.password);
    printf("\nAccount Created Successfully!\n");
    return 1;
}

int get_nxt_acc_no()
{
    const char *fname = "last_accno.txt";
    FILE *file = fopen(fname, "r+");
    int las_accno = 10000;
    if (file == NULL)
    {
        file = fopen(fname, "w+");
        if (file == NULL)
        {
            printf("Unable to open the file!!");
            return 0;
        }
        fprintf(file, "%d", las_accno);
        fflush(file);
        rewind(file);
    }
    else
    {
        if (fscanf(file, "%d", &las_accno) != 1)
            las_accno = 10000;
    }

    las_accno++;
    rewind(file);
    fprintf(file, "%d", las_accno);
    fflush(file);
    fclose(file);

    return las_accno;
}

void Deposit_Money()
{
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL)
    {
        printf("Unable to open the file!!");
        return;
    }

    int acc_no;
    float money;
    Account acc_to_depo;
    printf("Enter Account No. :");
    if (scanf("%d", &acc_no) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Invalid account number input\n");
        fclose(file);
        return;
    }
    printf("Enter Amount to Deposit :");
    if (scanf("%f", &money) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Invalid amount\n");
        fclose(file);
        return;
    }

    while (fread(&acc_to_depo, sizeof(acc_to_depo), 1, file))
    {
        if (acc_to_depo.acc_no == acc_no)
        {
            acc_to_depo.balance += money;
            fseek(file, -sizeof(acc_to_depo), SEEK_CUR);
            fwrite(&acc_to_depo, sizeof(acc_to_depo), 1, file);
            fclose(file);
            printf("Successfully deposited Rs. %.2f", money);
            printf("\nNew Balance : Rs.%.2f\n", acc_to_depo.balance);
            return;
        }
    }
    fclose(file);
    printf("Account No. %d \n Not Found!!\n", acc_no);
}
void Withdraw_Money()
{
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL)
    {
        printf("Unable to Open File!!");
        return;
    }

    int acc_no;
    float money;
    Account acc_witdr;
    printf("Enter Your Account Number: ");
    if (scanf("%d", &acc_no) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Invalid account number input\n");
        fclose(file);
        return;
    }
    printf("Enter Amount to Withdraw :");
    if (scanf("%f", &money) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Invalid amount\n");
        fclose(file);
        return;
    }

    while (fread(&acc_witdr, sizeof(acc_witdr), 1, file))
    {
        if (acc_witdr.acc_no == acc_no)
        {
            if (acc_witdr.balance >= money)
            {
                acc_witdr.balance -= money;
                fseek(file, -sizeof(acc_witdr), SEEK_CUR);
                fwrite(&acc_witdr, sizeof(acc_witdr), 1, file);
                printf("Successfully Withdrawn Rs.%.2f", money);
                printf("\nRemaining Balance : Rs.%.2f\n", acc_witdr.balance);
            }
            else
            {
                printf("Insufficient Balance!!\n");
            }
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Unable to Withdraw from Account No. %d \n Not Found!!\n", acc_no);
}
void Check_Balance()
{
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL)
    {
        printf("Unable to Open File!!\n");
        return;
    }
    Account acc_read;
    int acc_no;
    printf("Enter Your Account Number: ");
    if (scanf("%d", &acc_no) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Invalid account number input\n");
        fclose(file);
        return;
    }

    while (fread(&acc_read, sizeof(acc_read), 1, file))
    {
        if (acc_read.acc_no == acc_no)
        {
            printf("Your Current Account Balance : Rs %.2f\n", acc_read.balance);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Account No: %d\nNot Found!!\n", acc_no);
}