#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <vcruntime_string.h>
#include <string.h>

void LoginScreen();
void StdMenu();
void EmpMenu();
void stdInfo();
void listStudents();
void listEmployee();
int FindStudent(char[], char[], char[], char[], int);
int FindEmployee(char[], char[]);
void editGrades(char[], char[], char[], int, int, int, int, int); 
void addStudent(char[], char[], int, char[], char[]);
void addEmployee(char[], char[], char[], char[]);
void removeStudent(char[]);
void removeEmployee(char[], char[]);
int FindEmployee(struct Users*);

struct Users
{
	char username[30];
	char password[10];
	char eMail[50];
	char role[6];
};
struct Students {
	char name[30];
	char surname[30];
	int age;
	char studentNumber[10];
	char department[30];
	int grades[5]; // 5 grades for the student
};
int main() {
	
	int choise;
	LoginScreen();
	scanf_s("%d", &choise);

	//LOGIN SCREEN SWITCH CASE START
	switch (choise) {
	case 1:  //login
	{
		printf("YOU CAN NOW LOGIN TO YOUR ACCOUNT\n");
		struct Users user;
		printf("PLEASE ENTER YOUR USERNAME: ");
		scanf_s("%s", user.username, (unsigned)_countof(user.username));
		printf("PLEASE ENTER YOUR PASSWORD: ");
		scanf_s("%s", user.password, (unsigned)_countof(user.password));

		int found = FindEmployee(&user);

		if (found == 0) {
			printf("USERNAME OR PASSWORD NOT FOUND.\n");
			LoginScreen();
		}
		else {
			printf("LOGIN SUCCESSFUL. WELCOME %s\n", user.username);

			if (strcmp(user.role, "user") == 0) { //if you are user
				stdInfo();
			}
			else if (strcmp(user.role, "admin") == 0) { //if you are admin
				printf("WHICH ONE DO YOU CHOISE?\n");
				printf("1. STUDENT TRANSACTION\n");
				printf("2. EMPLOYEE TRANSACTION\n");
				int admin_choise;
				printf("PLEASE CHOOSE AN OPTION: ");
				scanf("%d", &admin_choise);
				if (admin_choise == 1) {
					printf("STUDENT TRANSACTION SELECTED.\n");
					StdMenu();
				}
				else if (admin_choise == 2) {
					printf("EMPLOYEE TRANSACTION SELECTED.\n");
					EmpMenu();
				}
				else {
					printf("INVALID OPTION, PLEASE TRY AGAIN.\n");
				}
			}
			else {
				printf("YOU ARE NOT AUTHORIZED TO ACCESS THIS SYSTEM.\n");
			}
		}
		break;
	}
	case 2: //signup
	{
		printf("YOU CAN NOW OPEN A NEW ACCOUNT\n");
		FILE* file;
		struct Users newuser;
		printf("PLEASE ENTER YOUR USERNAME: ");
		scanf_s("%s", newuser.username, (unsigned)_countof(newuser.username));
		printf("PLEASE ENTER YOUR PASSWORD: ");
		scanf_s("%s", newuser.password, (unsigned)_countof(newuser.password));
		if (strlen(newuser.password) > 10) {
			printf("PASSWORD MUST BE LESS THAN 10 CHARACTERS.\n");
			return 1;
		}
		printf("PLEASE ENTER YOUR E-MAIL: ");
		scanf_s("%s", newuser.eMail, (unsigned)_countof(newuser.eMail));
		strcpy(newuser.role, "user");

		file = fopen("users.txt", "a");
		fprintf(file, "%s,%s,%s,%s\n", newuser.username, newuser.password, newuser.eMail, newuser.role);
		fclose(file);
		if (errno != 0) {
			printf("Error: %d\n", errno);
			perror("Error message:");
		}
		else {
			printf("YOUR ACCOUNT HAS BEEN CREATED SUCCESSFULLY.\n");
		}
		break;
	}
	case 3: //exit
		printf("THANKS FOR USE THIS SYSTEM. SEE YOU AGAIN.");
		break;
	default:
		printf("INVALID OPTION, PLEASE TRY AGAIN.\n");
		LoginScreen();
		break;
	}  
	// LOGIN SCREEN SWITCH CASE END

	return 0;
}
void LoginScreen() {
	printf("WELCOME TO STUDENT REGISTRATION SYSYTEM\n");
	printf("---------------------------------------\n");
	printf("PLEASE LOGIN TO CONTINUE OR YOU HAVEN'T AN ACCOUNT YOU SINGUP\n");
	printf("1- LOGIN\n");
	printf("2- SIGNUP\n");
	printf("3- EXIT\n");
	printf("PLEASE CHOOSE AN OPTION: ");
}

//just users can access this function
void stdInfo() { //all of the users can access this function
	printf("--------------------------------\n");
	printf("STUDENT GRADE INFORMATION SYSTEM\n");
	printf("1. LIST STUDENTS\n");
	printf("2. ENTER COURSE GRADE\n");
	printf("3. EXIT\n");

	int choise;
	printf("PLEASE CHOOSE AN OPTION: ");
	scanf("%d", &choise);

	switch (choise)
	{
	case 1: //list students
		printf("LISTING STUDENTS...\n");
		listStudents();
		stdInfo();
		break;
	case 2: //enter course grade
	{
		printf("ENTER COURSE GRADE\n");
		char Name[30], Surname[30], StudentNumber[10], DeptName[30] = " ";
		int Age = 0, grade1, grade2, grade3, grade4, grade5;
		printf("PLEASE ENTER STUDENT NAME: ");
		scanf("%s", Name);
		printf("PLEASE ENTER STUDENT SURNAME: ");
		scanf("%s", Surname);
		printf("PLEASE ENTER STUDENT NUMBER: ");
		scanf("%s", StudentNumber);

		int sonuc = FindStudent(Name, Surname, StudentNumber, DeptName, Age);
		if (sonuc) {  // if student found
			printf("PLEASE ENTER STUDENT GRADE 1: ");
			scanf("%d", &grade1);
			printf("PLEASE ENTER STUDENT GRADE 2: ");
			scanf("%d", &grade2);
			printf("PLEASE ENTER STUDENT GRADE 3: ");
			scanf("%d", &grade3);
			printf("PLEASE ENTER STUDENT GRADE 4: ");
			scanf("%d", &grade4);
			printf("PLEASE ENTER STUDENT GRADE 5: ");
			scanf("%d", &grade5);

			editGrades(Name, Surname, StudentNumber, grade1, grade2, grade3, grade4, grade5);
			stdInfo(); // return to student info menu
		}
		break;
	}
	case 3: //exit
		printf("THANKS FOR USING THIS SYSTEM. SEE YOU AGAIN.\n");
		break;

	default:
		printf("INVALID OPTION, PLEASE TRY AGAIN.\n");
		stdInfo();
		break;
	}
}

void editGrades(char name[],char surname[],char stdNumber[],int grd1,int grd2,int grd3,int grd4,int grd5) {
	FILE* file = fopen("students.txt", "r+");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	char line[200];
	char fileName[30], fileSurname[30], fileStudentNumber[10], fileDepartment[30];
	int fileAge, fgrade1, fgrade2, fgrade3, fgrade4, fgrade5;
	while (fgets(line, sizeof(line), file) != NULL) {
		int result = sscanf_s(line, "%29[^,],%29[^,],%d,%9[^,],%29[^,],%d,%d,%d,%d,%d",
			fileName, (unsigned)_countof(fileName),
			fileSurname, (unsigned)_countof(fileSurname),
			&fileAge, // int için adresini geç
			fileStudentNumber, (unsigned)_countof(fileStudentNumber),
			fileDepartment, (unsigned)_countof(fileDepartment),
			&fgrade1, &fgrade2, &fgrade3, &fgrade4, &fgrade5);
		if (result == 10) 
		{
			if (strcmp(fileName, name) == 0 && strcmp(fileSurname, surname) == 0 && strcmp(fileStudentNumber, stdNumber) == 0) {
				// Student found, update grades
				fgrade1 = grd1;
				fgrade2 = grd2;
				fgrade3 = grd3;
				fgrade4 = grd4;
				fgrade5 = grd5;
				// Move the file pointer to the beginning of the line
				fseek(file, -(long)strlen(line), SEEK_CUR);

				fprintf(file, "%s,%s,%d,%s,%s,%d,%d,%d,%d,%d\n", fileName, fileSurname, fileAge, fileStudentNumber,
					fileDepartment, fgrade1, fgrade2, fgrade3, fgrade4, fgrade5);
				printf("GRADES UPDATED SUCCESSFULLY.\n");
				fclose(file);
				return; // Exit after updating
			}
			else {
				printf("STUDENT NOT FOUND.\n");
				fclose(file);
				return; // Student not found;
			}
		}
	}
}

//if student is found, return 1, if not return 0
int FindStudent(char Name[], char Surname[],char StudentNumber[], char DeptName[], int Age) {
	FILE* file = fopen("students.txt", "r");
	if (file == NULL) {
		perror("Error opening file");
		return 0;
	}
	char line[200];
	char fileName[30], fileSurname[30], fileStudentNumber[10], fileDepartment[30];
	int fileAge, fgrade1, fgrade2, fgrade3, fgrade4, fgrade5;
	while (fgets(line, sizeof(line), file) != NULL) {
		int result = sscanf_s(line, "%29[^,],%29[^,],%d,%9[^,],%29[^,],%d,%d,%d,%d,%d",
			fileName, (unsigned)_countof(fileName),
			fileSurname, (unsigned)_countof(fileSurname),
			&fileAge, // int için adresini geç
			fileStudentNumber, (unsigned)_countof(fileStudentNumber),
			fileDepartment, (unsigned)_countof(fileDepartment),
			&fgrade1, &fgrade2, &fgrade3, &fgrade4, &fgrade5);
		if (result == 10) {
			if (strcmp(fileName,Name)==0 && strcmp(fileSurname,Surname)==0 && strcmp(fileStudentNumber,StudentNumber)==0) {
				strcpy(Name, fileName);
				strcpy(Surname, fileSurname);
				strcpy(StudentNumber, fileStudentNumber);
				strcpy(DeptName, fileDepartment);
				Age = fileAge; // Update age
				printf("STUDENT FOUND: %s %s, STUDENT NUMBER: %s, AGE: %d, DEPARTMENT: %s\n", Name, Surname, StudentNumber, Age, DeptName);
				fclose(file);
				return 1; // Student found
			}
		}
	}
	fclose(file);
	return 0; // Student not found
}

void StdMenu() { // also 'admin' can access this function
	printf("---------------------------\n");
	printf("STUDENT REGISTRATION SYSTEM\n");
	printf("1. ADD STUDENT\n");
	printf("2. REMOVE STUDENT\n");
	printf("3. LIST STUDENTS\n");
	printf("4. EXIT\n");

	int choise;
	printf("PLEASE CHOOSE AN OPTION: ");
	scanf("%d",&choise);

	switch (choise) {
	case 1: // add student
		printf("WELCOME TO ADDING TRANSACTION!\n");
		char Name[30], Surname[30], StudentNumber[10], Department[30];
		int Age;
		printf("PLEASE ENTER STUDENT'S NAME: ");
		scanf("%s",Name);
		printf("PLEASE ENTER STUDENT'S SURNAME: ");
		scanf("%s",Surname);
		printf("PLEASE ENTER STUDENT'S AGE: ");
		scanf("%d", &Age);
		printf("PLEASE ENTER STUDENT'S DEPARTMENT: ");
		scanf("%s", Department);
		typingNumber:
		printf("PLEASE ENTER STUDENT'S NUMBER: ");
		scanf("%s", StudentNumber);
		if (strlen(StudentNumber) > 10) {
			printf("STUDENT NUMBER MUST BE LESS THAN 10 CHARACTERS.TYR AGAIN!\n");
			goto typingNumber; // Exit if student number is too long
		}
		addStudent(Name, Surname, Age, StudentNumber, Department);
		StdMenu();
		break;
	case 2: // remove student
		printf("WELCOME TO REMOVING TRANSACTION!\n");
		char number[11];
		printf("PLEASE ENTER THE STUDENT NUMBER YOU WANT TO DELETE: ");
		scanf("%s", number);

		removeStudent(number);
		printf("REMOVING TRANSACTION COMPLETED.\n");
		StdMenu();
		break;
	case 3: // list students
		printf("LISTING STUDENTS...\n");
		listStudents();
		StdMenu();
		break;
	case 4: // exit
		printf("THANKS FOR USING THIS SYSTEM. SEE YOU AGAIN.\n");
		break;
	default:
		printf("INVALID OPTION, PLEASE TRY AGAIN.\n");
		StdMenu();
		break;
	}
}

void addStudent(char Name[],char Surname[], int Age, char StudentNumber[], char Department[]) { // also 'admin' can access this function
	FILE* file = fopen("students.txt", "a");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	if (FindStudent(Name, Surname, StudentNumber, Department, Age)) {
		printf("STUDENT ALREADY EXISTS.\n");
		fclose(file);
		return; // Student already exists
	}
	struct Students newStudent;
	strcpy(newStudent.name, Name);
	strcpy(newStudent.surname, Surname);
	newStudent.age = Age;
	strcpy(newStudent.studentNumber, StudentNumber);
	strcpy(newStudent.department, Department);
	newStudent.grades[0] = 0; // Initialize grades to 0
	newStudent.grades[1] = 0;
	newStudent.grades[2] = 0;
	newStudent.grades[3] = 0;
	newStudent.grades[4] = 0;
	// Write the new student to the file
	fprintf(file, "%s,%s,%d,%s,%s,%d,%d,%d,%d,%d\n", newStudent.name, newStudent.surname, newStudent.age, newStudent.studentNumber,
		newStudent.department,newStudent.grades[0], newStudent.grades[1], newStudent.grades[2], newStudent.grades[3], newStudent.grades[4]);
	printf("STUDENT %s %s HAS BEEN ADDED SUCCESSFULLY.\n", newStudent.name, newStudent.surname);
	fclose(file);
}

void listStudents() { //all of the users can access this function
	FILE* file = fopen("students.txt", "r");
	if (file == NULL) {
		perror("Error opening file\n");
		return;
	}
	char line[200];
	int i = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		printf("%d. %s", i+1, line);
		i++;
	}
	printf("END OF STUDENTS LIST\nCOUNT OF THE STUDENTS ARE %d", i);
	printf("\n");
	fclose(file);
}

void removeStudent(char stdnum[]) { // also 'admin' can access this function
	FILE* newFile = fopen("newStdFile.txt", "w");
	FILE* oldFile = fopen("students.txt", "r");

	if (newFile == NULL || oldFile == NULL) {
		perror("Error opening file");
		return;
	}
	char line[200];
	int rmvline = 0; 
	while(fgets(line,sizeof(line),oldFile))
	{
		char fileName[30], fileSurname[30], fileStudentNumber[10], fileDepartment[30];
		int fileAge, fgrade1, fgrade2, fgrade3, fgrade4, fgrade5;
		int result = sscanf_s(line, "%29[^,],%29[^,],%d,%9[^,],%29[^,],%d,%d,%d,%d,%d",
			fileName, (unsigned)_countof(fileName),
			fileSurname, (unsigned)_countof(fileSurname),
			&fileAge,
			fileStudentNumber, (unsigned)_countof(fileStudentNumber),
			fileDepartment, (unsigned)_countof(fileDepartment),
			&fgrade1, &fgrade2, &fgrade3, &fgrade4, &fgrade5);
		if (result == 10) {
			if (strcmp(fileStudentNumber, stdnum) == 0) {
				rmvline = 1; 
				printf("STUDENT %s %s HAS BEEN REMOVED SUCCESSFULLY.\n", fileName, fileSurname);
			}
			else {
				fprintf(newFile, "%s", line); // write the line to the new file
			}
		}
	}

	fclose(oldFile);
	fclose(newFile);
	remove("students.txt"); // remove the old file
	rename("newStdFile.txt", "students.txt"); // rename the new file to old file name
}

void EmpMenu() { //Also 'admin' can access this function
	printf("----------------------------\n");
	printf("EMPLOYEE REGISTRATION SYSTEM\n");
	printf("1. ADD EMPLOYEE\n");
	printf("2. REMOVE EMPLOYEE\n");
	printf("3. LIST EMPLOYEES\n");
	printf("4. EXIT\n");

	int choise;
	printf("PLEASE CHOOSE AN OPTION: ");
	scanf("%d", &choise);

	switch (choise) 
	{
	case 1: // add employee

		printf("WELCOME TO ADDING TRANSACTION!\n");
		struct Users user;
		printf("PLEASE ENTER EMPLOYEE'S USERNAME: ");
		scanf("%s", user.username);
		typingPsw:
		printf("PLEASE ENTER EMPLOYEE'S PASSWORD: ");
		scanf("%s", user.password);
		if (strlen(user.password) > 10) {
			printf("PASSWORD MUST BE LESS THAN 10 CHARACTERS.\n");
			goto typingPsw; // Exit if password is too long
		}
		printf("PLEASE ENTER EMPLOYEE'S E-MAIL: ");
		scanf("%s", user.eMail);
		typingRole:
		printf("PLEASE ENTER EMPLOYEE'S ROLE (user/admin): ");
		scanf("%s", user.role);

		if (strcmp(user.role, "user") != 0 && strcmp(user.role, "admin") != 0) {
			printf("ROLE MUST BE 'user' OR 'admin'.\n");
			goto typingRole; // Exit if role is invalid
		}

		if (FindEmployee(user.username, user.password)) {
			printf("EMPLOYEE ALREADY EXISTS.\n");
			EmpMenu();
			return; // Exit if employee already exists
		}
		addEmployee(user.username, user.password, user.eMail, user.role);
		EmpMenu();
			break;
		case 2: // remove employee
			printf("WELCOME TO REMOVING TRANSACTION!\n");
			printf("PLEASE ENTER THE EMPLOYEE'S USERNAME YOU WANT TO DELETE: ");
			scanf("%s", user.username);
			printf("PLEASE ENTER THE EMPLOYEE'S PASSWORD YOU WANT TO DELETE: ");
			scanf("%s", user.password);

			if (FindEmployee(user.username, user.password) == 0) {
				printf("EMPLOYEE NOT FOUND.\n");
				EmpMenu();
				return; // Exit if employee not found
			}
			removeEmployee(user.username, user.password);
			EmpMenu();
			break;
		case 3: // list employees
			printf("LISTING EMPLOYEES...\n");

			listEmployee();
			EmpMenu();
			break;
		case 4: // exit
			printf("THANKS FOR USING THIS SYSTEM. SEE YOU AGAIN.\n");
			break;
		default:
			printf("INVALID OPTION, PLEASE TRY AGAIN.\n");

			EmpMenu();
			break;
	}
}

// if employee is found, return 1, if not return 0
int FindEmployee(char username[], char password[]) {
	FILE* file = fopen("users.txt","r");
	if(file==NULL)
	{
		perror("Error opening file");
		return 0; // File not found
	}
	char line[200];
	char fileName[30], filepassword[10], fileEmail[50], fileRole[6];
	while (fgets(line, sizeof(line), file) != NULL) {
		int result = sscanf_s(line, "%29[^,],%9[^,],%49[^,],%5[^,]",
			fileName, (unsigned)_countof(fileName),
			filepassword, (unsigned)_countof(filepassword),
			fileEmail, (unsigned)_countof(fileEmail),
			fileRole, (unsigned)_countof(fileRole));
		if (result == 4) {
			if (strcmp(fileName,username) == 0 && strcmp(filepassword,password) == 0) {
				strcpy(username, fileName);
				strcpy(password, filepassword);
				printf("EMPLOYEE FOUND: %s \n", username);
				fclose(file);
				return 1; // user found
			}
		}
	}
	fclose(file);
	return 0; // user not found
}

void addEmployee(char UserName[], char Password[], char email[], char role[]) {
	FILE* file = fopen("users.txt", "a");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	if (FindEmployee(UserName, Password)) {
		printf("EMPLOYEE ALREADY EXISTS.\n");
		fclose(file);
		return; // Student already exists
	}
	struct Users newUser;
	strcpy(newUser.username, UserName);
	strcpy(newUser.password, Password);
	strcpy(newUser.eMail, email);
	strcpy(newUser.role, role);
	
	// Write the new user to the file
	fprintf(file, "%s,%s,%s,%s\n", newUser.username, newUser.password, newUser.eMail, newUser.role);
	printf("EMPLOYEE %s HAS BEEN ADDED SUCCESSFULLY.\n", newUser.username);
	fclose(file);
}

void listEmployee() {
	FILE* file = fopen("users.txt", "r");
	if (file == NULL) {
		perror("Error opening file\n");
		return;
	}
	char line[200];
	int i = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		printf("%d. %s", i+1, line);
		i++;
	}
	printf("END OF EMPLOYEES LIST\nCOUNT OF THE EMPLOYEES ARE %d", i);
	printf("\n");
	fclose(file);
}

void removeEmployee(char Name[], char Password[]) { // also 'admin' can access this function
	FILE* newFile = fopen("newEmpFile.txt", "w");
	FILE* oldFile = fopen("users.txt","r");
	if (newFile == NULL || oldFile == NULL) {
		perror("Error opening file");
		return;
	}
	char line[200];
	int rmvline = 0;
	while (fgets(line,sizeof(line),oldFile)) {
		char fileName[30], filepassword[10], fileEmail[50], fileRole[6];
	
		int result = sscanf_s(line, "%29[^,],%9[^,],%49[^,],%5[^,]",
			fileName, (unsigned)_countof(fileName),
			filepassword, (unsigned)_countof(filepassword),
			fileEmail, (unsigned)_countof(fileEmail),
			fileRole, (unsigned)_countof(fileRole));
		if (result == 4) {
			if (strcmp(fileName,Name)==0 && strcmp(filepassword,Password)==0) {
				rmvline = 1;
				printf("EMPLOYEE %s HAS BEEN REMOVED SUCCESSFULLY.\n", fileName);
			}
			else {
				fprintf(newFile, "%s", line); // write the line to the new file
			}
		}
	}
	fclose(oldFile);
	fclose(newFile);
	remove("users.txt"); // remove the old file
	rename("newEmpFile.txt", "users.txt"); // rename the new file to old file name
}

int FindEmployee(struct Users* user) {
	FILE* file = fopen("users.txt", "r");
	if (file == NULL) return 0;

	char line[200];
	char uname[30], pass[10], mail[50], role[6];

	while (fgets(line, sizeof(line), file)) {
		sscanf(line, "%[^,],%[^,],%[^,],%[^,\n]", uname, pass, mail, role);
		if (strcmp(uname, user->username) == 0 && strcmp(pass, user->password) == 0) {
			strcpy(user->eMail, mail);
			strcpy(user->role, role);
			fclose(file);
			return 1;
		}
	}
	fclose(file);
	return 0;
}