#include <stdio.h>
#include <string.h>

void create_account();
void Deposit_Money();
void Withdraw_Money();
void Check_Balance();

const char *ACCOUNT_FILE = "account.dat";

typedef struct
{
    char name[60];
    int acc_no;
    float balance;
} Account;

int main()
{

    while (1)
    {
        int choice;
        printf("\n\n*** Bank Management System ***\n");
        printf("\n1. Create Account");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Check Balance");
        printf("\n5. X IT");
        printf("\nEnter Your Choice...");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            create_account();
            break;
        case 2:
            Deposit_Money();
            break;
        case 3:
            Withdraw_Money();
            break;
        case 4:
            Check_Balance();
            break;
        case 5:
            printf("Closing The Bank");
            return 0;
            break;
        default:
            printf("Invalid Choice !!!");
            break;
        }
    }
    return 0;
}

void create_account()
{
    Account acc;
    FILE *file = fopen(ACCOUNT_FILE, "ab+");
    if (file == NULL)
    {
        printf("Unable to Open File!!");
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
    printf("Enter Your Account Number: ");
    scanf("%d", &acc.acc_no);
    acc.balance = 0;
    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);

    printf("\nAccount Balance : %.2f", acc.balance);
    printf("\nAccount Created Sucessfully!");
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
