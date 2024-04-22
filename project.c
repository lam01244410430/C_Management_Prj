#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define NUM_USERS 999
#define MAX_USERNAME_LENGTH 25
#define MAX_PASSWORD_LENGTH 25
#define FILE_NAME_1 "teacher.txt"
#define FILE_NAME_2 "labo_staff.txt"
#define FILE_NAME_3 "off_staff.txt"

struct user{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

typedef struct emp{
    char name[10001];
    char number[10001];
    int wage;
    int hour;
    struct emp *next;
} emp;

emp *head;

// 检查用户名和密码的函数
int login(struct user * users, int numUSERS, const char *username, const char *password){
    for (int i = 0; i < numUSERS; i++){
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login Successfully! Press Enter to continue...");
            getch();
            system("cls");
            return 1;
        }
    }
    return 0;
}

// 统计文件中“用户名，密码”的个数
int readAccountsFromFile(FILE *file, struct user *users) {
    int numUsers = 0;
    if(users == NULL){
        return 0;
    }

    while (fscanf(file, "%s %s", users[numUsers].username, users[numUsers].password) == 2) {
        numUsers++;
    }
    return numUsers;
}

// 菜单1选择学院的人员类型之一
int option1(emp *head){
    int choice;
    do{
        printf("========================= COLLEGE FACULTY AND STAFF MANAGEMENT SYSTEM  =========================\n\n");
        printf("[1] Teacher\n");
        printf("[2] Laboratory staff\n");
        printf("[3] Office staff\n");
        printf("[0] Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        system("cls");
        
        switch(choice){
        case 1:
            teacher(head, 1);
            break;
        case 2:
            labostaff(head, 2);
            break;
        case 3:
            offstaff(head, 3);
            break;
        case 0:
            printf("Exiting...\n");
            fflush(stdin);
            system("cls");
            return 0;
        default:
            printf("INVALID SELECTION, PLEASE CHOOSE AGAIN!\n");
            printf("Enter to continue...\n");
            getch();
            fflush(stdin);
            system("cls");
            return option1(head);
        } 
    }while(choice!=0);
}

//菜单2选择功能人员信息相关的函数
void option2(FILE *file, emp *head, int b){
    printf("[1] View\n");
    printf("[2] Create\n");
    printf("[3] Delete\n");
    printf("[4] Search\n");
    printf("[0] Exit\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);

    system("cls");
    char numdel[5];

    switch(choice){
        case 1:
            viewEmp(file, head);
            fclose(file);
            break;
        case 2:
            createEmp(file, head);
            fclose(file);
            break;
        case 3:
            printf("Enter number of person need to remove: ");
            scanf("%s", numdel);
            deleteEmp(numdel, b);
            break;
        case 4:
            searchEmp(file, head);
            fclose(file);
            break;
        case 0:
            printf("Exiting...\n");
            getch();
            fflush(stdin);
            system("cls");
            return option1(head);
            break;
        default:
            printf("INVALID SELECTION, PLEASE CHOOSE AGAIN!\n");
            printf("Enter to continue...\n");
            getch();
            fflush(stdin);
            system("cls");
            return option2(file, head, b);
    }
}


//display all infomations one of type of employees
void viewEmp(FILE *file, emp *head) {
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char name[20], number[20];
    int wage, hour;

    printf("=====================================================================================\n");
    printf("%-20s %-20s %-19s %-10s %-20s\n", "Name", "Number", "Hourly wage", "Hour", "Salary");
    while (fscanf(file, "%19s %19s %d %d", name, number, &wage, &hour) != EOF) {
        int salary = wage * hour;
        printf("%-20s %-20s %-19d %-10d %-20d\n", name, number, wage, hour, salary);
    }
    printf("=====================================================================================\n");
    printf("Press any key to continue...");
    getch();
    fflush(stdin);
    system("cls");
    return option1(head);
}

// 添加新人员信息的函数
void createEmp(FILE *file, emp *head) {
    if (file == NULL) {
        printf("File is not open.\n");
        return;
    }

    emp newEmp;

    printf("Enter name: "); scanf(" %s", newEmp.name);
    printf("Enter number: "); scanf(" %s", newEmp.number);
    printf("Enter wage: "); scanf("%d", &newEmp.wage);
    printf("Enter worked hours: "); scanf("%d", &newEmp.hour);

    // 检查每小时工资额和工作小时是否有效
    if (newEmp.wage < 0 || newEmp.hour < 0 || newEmp.hour > 72) {
        printf("Invalid hourly wage or hours. Please try again...\n");
        getch();
        return;
    }

    rewind(file); // 从文件开头开始
    char line[1024]; // 存储从指向的文件中读取的每一行文本
    char existName[10001];
    char existNumber[10001];
    
    // 查看输入的编号有没有存在
    while (fgets(line, sizeof(line), file) != NULL){
        if (sscanf(line, "%s %s", existName, existNumber) != EOF){
            if (strcmp(existNumber, newEmp.number) == 0){
                printf("Error: This person with this number already exists!\n");
                printf("Press Enter to continue...");
                getch();
                system("cls");
                return createEmp(file, head);
            }
            
        }
    }

    fprintf(file, "\n%s %s %d %d", newEmp.name, newEmp.number, newEmp.wage, newEmp.hour);
    fflush(file);  // 确保数据写入文件

    printf("Employee added successfully! Press Enter to continue...\n");
    getch();
    system("cls");
}

// 根据人员编号删除人员信息的函数
void deleteEmp(char delnum[5], int c, emp *head) {
    const char *filename;
    switch (c) {
        case 1: filename = FILE_NAME_1; break;
        case 2: filename = FILE_NAME_2; break;
        case 3: filename = FILE_NAME_3; break;
        default: printf("Invalid file selection.\n"); return;
    }
    // 通知用户正在打开哪个文件
    printf("Attempting to open file: %s\n", filename);

    // 打开文件进行读取和追加
    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // 创建临时文件，用于存储不删除的数据
    FILE *tempFile = fopen("temp.txt", "a+");
    if (tempFile == NULL) {
        printf("Cannot create the temp file.\n");
        fclose(file);
        return;
    }

    emp staff;
    int found = 0;

    // 从文件中读取人员数据直到EOF
    while (fscanf(file, "%s %s %d %d", staff.name, staff.number, &staff.wage, &staff.hour) != EOF) {
        // 检查当前记录是否需要删除的人员编号匹配
        if (strcmp(staff.number, delnum) != 0) {
            // 如果找到不匹配，则将数据写入临时文件
            fprintf(tempFile, "%s %s %d %d\n", staff.name, staff.number, staff.wage, staff.hour);
        } else {
            // 如果找到匹配，则与人员信息确认删除
            printf("Name: %s\nNumber: %s\nHourly wage: %d\nWorked hours: %d\n", staff.name, staff.number, staff.wage, staff.hour);
            printf("Match found. Are you sure to delete this person? [Yes/No]\n");
            char yn[3];
            scanf("%s", yn);
            if (strcmp(yn, "Yes") == 0 || strcmp(yn, "YES") == 0 || strcmp(yn, "yes") == 0) {
                found = 1;
            } else {
                system("cls");
                printf("Deletion canceled! Press Enter to continue...\n");
                getch();
                return;
            }
        }
        
    }
    fclose(file);
    fclose(tempFile);
    //更新数据
    remove(filename);
    rename("temp.txt", filename);
    if (found){
        printf("Delete completed! Press Enter to continue...");
        getch();
    } else {
        printf("No match found! Press Enter to try again...");
        getch();
        system("cls");
        return (delnum, c, head);
    }
    system("cls");
}

//查询人员信息
void searchEmp(FILE *file, emp *head) {
    char searchNum[80];
    int found = 0;

    printf("Enter staff number need to search: ");
    scanf("%s", searchNum);
    while (getchar() != '\n');

    char line[256]; //存储从指向的文件中读取的每一行文本
    //逐行读取行
    while (fgets(line, sizeof(line), file) != NULL) {
        
        char name[80], number[80];
        int wage, hour;
        
        //检查当前行的编号是否与搜索到的编号相匹配
        if (sscanf(line, "%s %s %d %d", name, number, &wage, &hour) == 4) {
            if (strcmp(number, searchNum) == 0) {
                int salary = wage * hour;
                printf("Name: %s\n", name);
                printf("Number: %s\n", number);
                printf("Hourly wage: %d\n", wage);
                printf("Worked hours: %d\n", hour);
                printf("Salary: %d\n", salary);
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("This person does not exist!\n");
    }
    printf("Press Enter to continue...\n");
    getch();
    fflush(stdin);
    system("cls");
    
    return option1(head);
}

void teacher(emp *head, int a){

    FILE *pt = fopen("teacher.txt", "a+");
    
    if (pt == NULL) {
        printf("Error opening file!\n"); //如果找不到文件则打印错误
        return;
    }

    option2(pt, head, a);

}

void labostaff(emp *head, int b){

    FILE *pt = fopen("laboratory_staff.txt", "a+");

    if (pt == NULL) {
        printf("Error opening file!\n"); //如果找不到文件则打印错误
        return;
    }

    option2(pt, head, b);

}

void offstaff(emp *head, int b){

    FILE *pt = fopen("office_staff.txt", "a+");
    if (pt == NULL) {
        printf("Error opening file!\n"); //如果找不到文件则打印错误
        return;
    }

    option2(pt, head, b);

}

int main(){
    struct user st[NUM_USERS];
    char inputUser[MAX_USERNAME_LENGTH];
    char inputPass[MAX_PASSWORD_LENGTH];
    int numUsers;

    FILE *file = fopen("accounts.txt", "r");
    if (!file) {
        perror("File Error!");
        return 1;
    }

    numUsers = readAccountsFromFile(file, st); //数据库中的用户数
    fclose(file);

    int logSuccess = 0;

    do {
        printf("Username: ");
        fgets(inputUser, sizeof(inputUser), stdin);
        inputUser[strcspn(inputUser, "\n")] = 0;

        printf("Password: ");
        fgets(inputPass, sizeof(inputPass), stdin);
        inputPass[strcspn(inputPass, "\n")] = 0;
        fflush(stdin);
        system("cls");
        if (login(st, numUsers, inputUser, inputPass)==1) {
            printf("               ===============================               \n");
            printf("               |          WELCOME TO         |              \n");
            printf("               |  COLLEGE FACULTY AND STAFF  |              \n");
            printf("               |       MANAGEMENT SYSTEM     |              \n");
            printf("               ===============================               \n\n");
            printf("Press any key to Continue...\n");
            getch();
            logSuccess = 1;
        } else {
            printf("Login failed! Please check your username or password. Press any key to continue...\n");
            getch();
            system("cls");
        }
    } while (!logSuccess);

    fflush(stdin);
    system("cls");
 
    emp *head = NULL;

    option1(head);

    return 0;
}