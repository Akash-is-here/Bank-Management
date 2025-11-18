#include <stdio.h>
#include <string.h>

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
        scanf("%d", &choose);

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
    Account acc;
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL)
    {
        printf("Unable to Open File!!");
        return;
    }
    int acc_no, flag = 0;
    char password[50];
    char c;
    printf("Enter Account Number: ");
    scanf("%d", &acc_no);
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
    printf("Enter Your Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while (fread(&acc, sizeof(acc), 1, file))
    {
        if (acc.acc_no == acc_no)
        {
            if (strcmp(acc.password, password) == 0)
            {
                flag = 1;
                break;
            }
        }
    }
    fclose(file);

    if (flag == 1)
    {
        printf("Login Sucessful, %s", acc.name);
        choose_fun();
    }
    else
    {
        printf("Invalid Credentials!!");
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
        printf("\n4. X IT");
        scanf("%d", &choice);

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
    Account acc;
    FILE *file = fopen(ACCOUNT_FILE, "ab+");
    if (file == NULL)
    {
        printf("Unable to Open File!!");
        return 0;
    }
    char c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
    printf("Enter Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    int ind = strcspn(acc.name, "\n");
    acc.name[ind] = '\0';
    printf("Enter Your Password: ");
    fgets(acc.password, sizeof(acc.password), stdin);
    int indx = strcspn(acc.password, "\n");
    acc.password[indx] = '\0';
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
    printf("\nAccount Passowrd: %s", acc.password);
    printf("\nAccount Created Sucessfully!");
    return 1;
}

int get_nxt_acc_no()
{
    FILE *file = fopen("last_accno.txt", "r+");
    int las_accno = 10000;
    if (file == NULL)
    {
        file = fopen("last_accno.txt", "w+");
        if (file == NULL)
        {
            printf("Unable to open the file!!");
            return 0;
        }
        fprintf(file, "%d", las_accno);
        fflush(file);
        rewind(file);
        return 0;
    }
    else
    {
        fscanf(file, "%d", &las_accno);
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
        printf("Unble to open the file!!");
        return;
    }

    int acc_no;
    float money;
    Account acc_to_depo;
    printf("Enter Account No. :");
    scanf("%d", &acc_no);
    printf("Enter Amount to Deposit :");
    scanf("%f", &money);

    while (fread(&acc_to_depo, sizeof(acc_to_depo), 1, file))
    {
        if (acc_to_depo.acc_no == acc_no)
        {
            acc_to_depo.balance += money;
            fseek(file, -sizeof(acc_to_depo), SEEK_CUR);
            fwrite(&acc_to_depo, sizeof(acc_to_depo), 1, file);
            fclose(file);
            printf("Sucessfully deposited Rs. %.2f", money);
            printf("\nNew Balance : Rs.%.2f", acc_to_depo.balance);
            return;
        }
    }
    fclose(file);
    printf("Account No. %d \n Not Found!!", acc_no);
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
    scanf("%d", &acc_no);
    printf("Enter Amount to Withdraw :");
    scanf("%f", &money);

    while (fread(&acc_witdr, sizeof(acc_witdr), 1, file))
    {
        if (acc_witdr.acc_no == acc_no)
        {
            if (acc_witdr.balance >= money)
            {
                acc_witdr.balance -= money;
                fseek(file, -sizeof(acc_witdr), SEEK_CUR);
                fwrite(&acc_witdr, sizeof(acc_witdr), 1, file);
                printf("Sucessfully Withdrawn Rs.%.2f", money);
                printf("\nRemaning Balance : Rs.%.2f", acc_witdr.balance);
            }
            else
            {
                printf("Inssuficient Balance!!");
            }
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Unable to Withdraw from Account No. %d \n Not Found!!", acc_no);
}
void Check_Balance()
{
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL)
    {
        printf("Unable to Open File!!");
    }
    Account acc_read;
    int acc_no;
    printf("Enter Your Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&acc_read, sizeof(acc_read), 1, file))
    {
        if (acc_read.acc_no == acc_no)
        {
            printf("Your Current Account Balance : Rs %.2f", acc_read.balance);
            fclose(file);
            return;
        }
    }

    printf("Account No: %d\nNot Found!!", acc_no);
}
