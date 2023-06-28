#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

int main();

struct part{ //Structure used for the part details during part creation
    char partName[30];
    char partID[20];
    int partStock;
    char partAmountStatus[30];
};

struct supplier{ //Structure used for part details during part creation
    char supplierName[30];
};

bool validatePartName(char *name){ //Function is used to validate part name during part creation

    int length = strlen(name); //Gets length of part name
    if (length<3||length > 30) {
        printf("\nInvalid Input. Name length must be between 3 and 30 characters.\n\n");
        return false;
    }
    bool alphabets = false;
    bool spaces = false;
    for (int i =0; i < length;i++){
        if (isspace(name[i])) spaces = true; //Checks if there are spaces
        if (isalpha(name[i])) alphabets = true; //Checks if there are alphabets
    }
    if (spaces){
        printf("\nInvalid Input. Spaces not allowed\n\n."); //Invalid if there are spaces
        return false;
    }
    if (!alphabets ) return false; //Invalid if there are no alphabets

    return true;
}


int partIDGenerator(char filename[10]) //Counts the number of lines in the text file for part ID assigning
{
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) //If the file is equal to NULL prints error
    {
        printf("Error opening file.\n");
        return 1;
    }
    int currentLine = 1; //Counter variable for the line the cursor is on in the file
    char line;  //Used to store the contents of each line in the file
    do
    {
        line = fgetc(file);
        if (line == '\n') currentLine++; //If new line is found in the line then currentLine incremented by 1
    } while (line != EOF); //Loop ends when it reaches the end of file
    fclose(file);
    return currentLine;
}

//Function used to display part details when the right arguments are used
void displayPartDetails(char *partName, char *partID, char*stock, char *quantityStatus){
    printf("\nCar Part Details: "
           "\n---------------------------------------");
    printf("\nPart Name      : %s", partName);
    printf("\nPart ID        : %s", partID);
    printf("\nPart Stock     : %s", stock);
    printf("\nQuantity Status: %s", quantityStatus);
    printf("\n---------------------------------------\n");
}

//Function displays the car models for the user to select car based on warehouse in another function
void displayCarModels(){
    printf("\n--------------------"
           "\n| 1. Blaze (BZ)    |"
           "\n| 2. Silk  (SL)    |"
           "\n| 3. Armer (AR)    |"
           "\n--------------------\n");
}

int registerParts(char filename[50], char partID[10]){ //Function used to register new parts in the system inventory

    FILE *inventoryFilePtr, *fileSupplierPtr;
    char fileData[50];
    int findResult = 0;
    char nameValidation;

    printf("\nEXISTING PARTS:\n");
    printf("--------------------------------------\n");

    inventoryFilePtr = fopen(filename, "r"); //Opens inventory file for reading
    fileSupplierPtr = fopen("supplier.txt", "a"); //Opens supplier text file for appending
    if (fgets(fileData, 50, inventoryFilePtr) == NULL){
        printf("\nError! File is either empty or does not exist.\n"); //Displays error message if file is NULL
    }
    else{
        while (fgets(fileData, 50, inventoryFilePtr) != NULL){
            printf("%s", fileData); //Reads and displays the contents of the file as existing parts
        }
    }
    printf("\n--------------------------------------");
    fclose(inventoryFilePtr);

    strcpy(fileData, "");
    inventoryFilePtr = fopen(filename, "a+"); //Opens file for reading and appending

    struct part partDetails;            //Declare structures
    struct supplier supplierDetails;

    printf("\n\nINPUT NEW PART DETAILS"); //Input part details and supplier details
    printf("\n--------------------------------------");
    printf("\nPart Name     : ");
    scanf("\n%s", &partDetails.partName);
    nameValidation = validatePartName(partDetails.partName); //Validations for part name

    if (nameValidation == false){
        main();
    }
    if (nameValidation == true) {
        while (fgets(fileData, 100, inventoryFilePtr)) {
            if (strstr(fileData, partDetails.partName)) {
                findResult++;
            }
            if (findResult == 1) {
                printf("\nPart already exists.");
                return 0;
            }
        }

        strcpy(partDetails.partID, partID);
        printf("\nPart Quantity : ");
        scanf("%d", &partDetails.partStock);
        printf("\nSupplier Name : ");
        scanf("%s", &supplierDetails.supplierName);
        printf("--------------------------------------\n");

        //Sets part amount status based on stock amount given
        if (partDetails.partStock >= 10) {
            strcpy(partDetails.partAmountStatus, "Sufficient Stock");
        } else if (partDetails.partStock < 10) {
            strcpy(partDetails.partAmountStatus, "Low Stock");
        }

        //Writes part details and supplier details into the inventory and supplier text files
        fprintf(inventoryFilePtr, "\n%s:%s:%d:%s\n", partDetails.partName, partDetails.partID, partDetails.partStock,
                partDetails.partAmountStatus);
        fclose(inventoryFilePtr);
        fprintf(fileSupplierPtr, "\n%s:%s:%s\n", supplierDetails.supplierName, partDetails.partName,
                partDetails.partID);
        fclose(fileSupplierPtr);

        //Displays the part and supplier details to the user
        printf("\n----------------------------------");
        printf("\n| Part Name       | %s", partDetails.partName);
        printf("\n| Part ID         | %s", partDetails.partID);
        printf("\n| Part Quantity   | %d", partDetails.partStock);
        printf("\n| Quantity Status | %s", partDetails.partAmountStatus);
        printf("\n| Supplier Name   | %s", supplierDetails.supplierName);
        printf("\n----------------------------------\n");
        printf("\nPart Details Saved Successfully\n\n");
    }
    return 0;
}

void partCreation() { //Function used to select which car model warehouse to make a new part for

    printf("\nSelect car model from options below:\n");

    char carModel;

    displayCarModels(); //Displays all the car models to the user to select the warehouse inventory

    printf("\nUser Selection: ");
    scanf("\n%c", &carModel);

    switch(carModel)
    {
        case '1':{
            char filename[50] = "WBZ.txt"; //Set filename to the inventory text file
            int currentLine=partIDGenerator(filename); //Saves the final line in the
            char partID[10];
            snprintf(partID, 10, "BZ%d", currentLine/2); //Sets part ID for the new part, currentLine/2 because there are skipped lines in the text files
            printf("\nBlaze Car Warehouse Part Creation\n");
            registerParts(filename, partID); //This function called to register the part for the blaze car warehouse
            break;
        }

        case '2':{
            char filename[50] = "WSL.txt";
            int currentLine = partIDGenerator(filename);
            char partID[10];
            snprintf(partID, 10, "SL%d", currentLine/2);
            printf("\nSilk Car Warehouse Part Creation\n");
            registerParts(filename, partID);
            break;
        }

        case '3':{
            char filename[50] = "WAR.txt";
            int currentLine = partIDGenerator(filename);
            char partID[10];
            snprintf(partID, 10, "AR%d", currentLine/2);
            printf("\nArmer Car Warehouse Part Creation\n");
            registerParts(filename, partID);
            break;
        }

        default:
            printf("\nInvalid option selected"); //Error message if not one of the 3 options chosen
    }
}

//Adds stock to existing quantity of the part
int addStock(int partStock){
    int stockAdded = 0;
    printf("Quantity of Stock Added: "); //Stock input for new quantity
    scanf("%d", &stockAdded);
    partStock += stockAdded;
    return partStock;
}

//Removes stock from the existing quantity of the part to send to the assembly division
int removeStock(int partStock){
    int stockRemoved = 0;

    printf("Quantity of Stock Removed: "); //Stock input for new quantity
    scanf("%d", &stockRemoved);
    partStock -= stockRemoved;
    if (partStock < 0){ //Validation to ensure stock is never negative
        printf("\nInsufficient number of stock. Not enough parts in inventory.\n");
        main();
    }
    return partStock;
}

//Function used to replace a particular line in the file
void lineReplacement(char filename[50], int lineNum, char partInventory[50]){
    FILE *filePtr, *tempFile;

    char tempFileName[50]; //Name for temporary file
    char lineBuffer[50]; //Buffer for a line read from the text file

    strcpy(tempFileName, "temp"); //Copies the string "temp" into the variable
    strcat(tempFileName, filename); //Concatenates content of tempFileName and filename together

    filePtr = fopen(filename, "r");
    tempFile = fopen(tempFileName, "w");

    if (filePtr == NULL || tempFile == NULL){ //If either file NULL then display error message
        printf("Error opening file(s).\n");
    }

    bool keepReading = true;
    int currentLine = 1;

    do{
        fgets(lineBuffer, 50, filePtr); //Read lines in the text file until keepReading is false
        if (feof(filePtr)) {
            keepReading = false; //If reaches end of file then keepReading false
        }
        else if(currentLine == lineNum) {
            fputs(partInventory, tempFile); //Writes replacement new line in the temp file if the line numbers match
        }
        else {
            fputs(lineBuffer, tempFile); //Just rewrites the whole contents of the file to the temp file
        }

        currentLine++; //Line number incremented by 1 each time it reads a line
    }while (keepReading);

    fclose(filePtr); //Text files are both closed
    fclose(tempFile);

    remove(filename); //Original inventory text file is removed
    rename(tempFileName, filename); //Temporary file gets renamed to the original file name
}

//Function used to update the stock of a particular part in the inventory file
void updateStock(char *filename) {
    char partIDInput[10];

    printf("\nPart ID: "); //User inputs part ID
    scanf("%s", partIDInput);

    FILE *filePtr;
    int lineNum = 1;
    int findResult = 0;
    char temp[50]; //Buffer for storing each line when file is read

    if((filePtr = fopen(filename, "r")) == NULL) {
        printf("Error! File is either empty or does not exist.");
    }

    while(fgets(temp, 50, filePtr) != NULL) { //While the file is not NULL
        if((strstr(temp, partIDInput)) != NULL) { //If the partID is found in one of the lines
            findResult++; //Incremented by 1
        }
        if (findResult == 1){ //If the part ID is found it will break the loop
            break;
        }
        lineNum++; //line number incremented each time a line is read
    }

    if(findResult == 0) { //Part ID does not exist if findResult = 0
        printf("\nPart ID does not exist.\n");
        main();
    }

    //Close the file if still open.
    if(filePtr) {
        fclose(filePtr);
    }

    int x = 0;
    char *array[4]; //Array for storing segments of the line found

    char *token = strtok(temp, ":"); //Splits the line using delimiter and stores in array
    while( token != NULL ) {
        array[x] = token;
        x++;
        token = strtok(NULL, ":"); //Stop condition
    }
    char partInventory[100] = "";
    char partName[30];
    char partID[10];
    char partAmountStatus[20];
    strcpy(partName, array[0]);
    strcpy(partID, array[1]);
    strcpy(partAmountStatus, array[3]);

    //Displays part details to user
    displayPartDetails(array[0], array[1], array[2], array[3]);

    int newStockQuantity;
    int updateChoice;
    printf("\nSelect choice from the options below: ");  //User selects the type of update for the stock quantity

    printf("\n--------------------------------"
           "\n| 1. Add New Stock             |"
           "\n| 2. Send Parts to Assembly    |"
           "\n--------------------------------\n");

    printf("\nUser Selection: ");
    scanf("%d", &updateChoice);

    if (updateChoice == 1){ //If choice 1 then add stock
        newStockQuantity = atoi(array[2]); //Converts stock quantity back to integer to update
        newStockQuantity = addStock(newStockQuantity); //Adds stock function called
    }
    if (updateChoice == 2){ //If choice 2 then remove stock
        newStockQuantity = atoi(array[2]); //Converts stock quantity back to integer to update
        newStockQuantity = removeStock(newStockQuantity); //Remove stock function called
    }

    printf("Updated Stock: %d", newStockQuantity);

    if (newStockQuantity < 10){
        strcpy(partAmountStatus, "Low Stock");  //Changes part amount status depending on the stock quantity
    }

    if(newStockQuantity >= 10){
        strcpy(partAmountStatus, "Sufficient Stock");
    }

    char buffer[10];
    itoa(newStockQuantity,buffer,10); //Converts the new stock quantity to string data type

    //Concatenates the strings together to form one line
    strcat(partInventory, partName);
    strcat(partInventory, ":");
    strcat(partInventory, partID);
    strcat(partInventory, ":");
    strcat(partInventory, buffer);
    strcat(partInventory, ":");
    strcat(partInventory, partAmountStatus);
    strcat(partInventory, "\n");

    //Replaces the line to update the stock quantity in the inventory file
    lineReplacement(filename, lineNum, partInventory);
}

//Function used to select which warehouse to perform update stock
void partStockUpdate(){
    printf("\nSelect car model from options below:\n");

    char carModel;

    displayCarModels();

    printf("\nUser Selection: ");
    scanf("\n%c", &carModel);

    switch(carModel){
        case '1':{
            //WBZ Stuff
            printf("\nBlaze Warehouse Inventory Update");
            char filename[50] = "WBZ.txt";
            updateStock(filename);
            break;
        }

        case '2':{
            //WSL Stuff
            printf("\nSilk Warehouse Inventory Update");
            char filename[50] = "WSL.txt";
            updateStock(filename);
            break;

        }
        case '3':{
            // WAR Stuff
            printf("\nArmer Warehouse Inventory Update");
            char filename[50] = "WAR.txt";
            updateStock(filename);
            break;
        }

        default:
            printf("\nInvalid System Option Selected"); //Error message to if invalid option selected
    }
}

//Function used to display all the parts in the file
int displayFileContents(char *filename) {
    FILE*filePtr;
    char fileData[50];
    filePtr = fopen(filename, "r");
    char userChoice;
    int findResult = 0;

    printf("\n-------------------------------------------"
           "\n|1. Display All Parts                     |"
           "\n|2. Display Parts (Less than 10 Stock)    |"
           "\n-------------------------------------------\n");
    printf ("User choice:");
    scanf("\n%c",&userChoice);
    fflush(stdin);
    printf("\nExisting Parts in Warehouse: ");
    printf("\n-------------------------------------");
    switch (userChoice) {
        case '1': {
            while (fgets(fileData, 50, filePtr) != NULL) { //Reads file if not NULL
                printf("%s", fileData); //Prints out each line in the file
            }
            printf("\n-------------------------------------\n\n");
            return 0;
        }
        case '2': {
            while (fgets(fileData, 50, filePtr) != NULL) {
                if ((strstr(fileData, "Low Stock")) != NULL) { //Condition for lines only with "Low Stock"
                    findResult++;
                    printf("\n%s", fileData); //Prints out lines with "Low Stock"
                }
            }
            printf("\n-------------------------------------\n\n");
            break;
        }
        default: {
            printf("\nInvalid Option. Please Try Again."); //Error message if invalid option selected
            main();
        }
    }
    fclose(filePtr); //File pointer closed
    if (findResult == 0){ //If findResult =  0 then all stock has sufficient quantity
        printf("\nAll parts have sufficient stock. None less than 10.\n\n");
    }
    return 0;
}

//Function used to select which warehouse to display all the part details form
void partsTracking(){
    printf("\nSelect car model from options below:\n");

    char carModel;

    displayCarModels();

    printf("\nUser Selection: ");
    scanf("\n%c", &carModel);

    switch(carModel){
        case '1':{
            //WBZ Stuff
            printf("\nBlaze Warehouse Inventory Tracking");
            char filename[50] = "WBZ.txt";
            displayFileContents(filename);
            break;
        }

        case '2':{
            //WSL Stuff
            printf("\nSilk Warehouse Inventory Tracking");
            char filename[50] = "WSL.txt";
            displayFileContents(filename);
            break;

        }
        case '3':{
            // WAR Stuff
            printf("\nArmer Warehouse Inventory Tracking");
            char filename[50] = "WAR.txt";
            displayFileContents(filename);
            break;
        }

        default:
            printf("\nInvalid System Option Selected");
    }
}

//Function used to print out part details of a specific part in the inventory file
int partDetails(char *filename, char *partID) {
    FILE *filePointer;
    int findResult = 0;
    char partLine[50];

    if((filePointer = fopen(filename, "r")) == NULL) { //Ensures file is not NULL
        printf("Error! Files does not exist.");
    }

    while(fgets(partLine, sizeof(partLine), filePointer) != NULL) {
        if((strstr(partLine, partID)) != NULL) { //Looks for part ID in the line
            int x = 0;
            char *array[4];

            partLine[strcspn(partLine, "\n")] = 0; //Split lines and stores it inside of array
            char *token = strtok(partLine, ":");
            token[strcspn(token, "\n")] = 0;
            while( token != NULL ) {
                array[x] = token;
                x++;
                token = strtok(NULL, ":");//stop condition break the loop
            }

            //Displays the part details to the user
            displayPartDetails(array[0], array[1], array[2], array[3]);
            findResult++;
        }
        if (findResult == 1) {
            break;
        }
    }

    if(findResult == 0) { //If part ID not found then it does not exist in the inventory file
        printf("\nPart ID does not exist.\n");
    }

    //Close the file if still open.
    if(filePointer) {
        fclose(filePointer);
    }
    return 0;
}

//Function to display supplier details to the user
//Works same way as previous function but uses the supplier text file instead
int supplierDetails(char *filename, char *partID) {
    FILE *filePointer;
    int findResult = 0;
    char partLine[30];

    if((filePointer = fopen(filename, "r")) == NULL) {
        printf("Error! Files does not exist.");
    }

    while(fgets(partLine, 30, filePointer) != NULL) {
        if((strstr(partLine, partID)) != NULL) {
            int x = 0;
            char *array[3];

            partLine[strcspn(partLine, "\n")] = 0;
            char *token = strtok(partLine, ":");
            token[strcspn(token, "\n")] = 0;
            while( token != NULL ) {
                array[x] = token;
                x++;
                token = strtok(NULL, ":");
            }

            printf("\nSupplier Details: "
                   "\n-------------------------");
            printf("\nSupplier Name : %s", array[0]);
            printf("\nPart Name     : %s", array[1]);
            printf("\nPart ID       : %s", array[2]);
            printf("\n-------------------------\n");
            findResult++;
        }
        if (findResult == 1) {
            break;
        }
    }
    //Close the file if still open.
    if(filePointer) {
        fclose(filePointer);
    }
    return 0;
}

//Function is used to allow the user to select which inventory file they would like to search the part in
void partSearch() {
    printf("\nSelect car model from options below:\n");

    char carModel;

    displayCarModels();

    printf("\nUser Selection: ");
    scanf("\n%c", &carModel);

    switch(carModel)
    {
        case '1':{
            char filename[50] = "WBZ.txt";
            char supplierFilename[50] = "supplier.txt";
            char partID[10];

            printf("\nBlaze Car Warehouse\n");
            printf("\nPart ID: ");
            scanf("%s", partID);
            partDetails(filename, partID);
            supplierDetails(supplierFilename, partID);
            break;
        }

        case '2':{
            char filename[50] = "WSL.txt";
            char supplierFilename[50] = "supplier.txt";
            char partID[10];

            printf("\nSilk Car Warehouse\n");
            printf("\n\nPart ID: ");
            scanf("%s", partID);
            partDetails(filename, partID);
            supplierDetails(supplierFilename, partID);
            break;
        }

        case '3':{
            char filename[50] = "WAR.txt";
            char supplierFilename[50] = "supplier.txt";
            char partID[10];

            printf("\nArmer Car Warehouse\n");
            printf("\n\nPart ID: ");
            scanf("%s", partID);
            partDetails(filename, partID);
            supplierDetails(supplierFilename, partID);
            break;
        }

        default:
            printf("\nInvalid option. Please select one of the car models in the options provided.");

    }
}

int main() //Main function which acts as a main menu for the user to select which feature they want to use
{
    while(1) {
        char systemOption;
        printf("\n----------------------------------------------"
               "\n|AUTOMOBILE PARTS INVENTORY MANAGEMENT SYSTEM|"
               "\n----------------------------------------------\n");

        printf("\nSystem Options: ");
        printf("\n--------------------------------------------"
               "\n| 1. Parts Inventory Creation in Warehouses|"
               "\n| 2. Parts Inventory Update                |"
               "\n| 3. Parts Inventory Tracking              |"
               "\n| 4. Part Searching                        |"
               "\n| 5. Exit Program                          |"
               "\n--------------------------------------------");


        printf("\nUser Selection: ");
        scanf("\n%c", &systemOption);
        switch (systemOption) {
            case '1': { //If option number 1 selected
                printf("\n--------------------------"
                       "\n|Parts Inventory Creation|"
                       "\n--------------------------\n");
                partCreation(); //Calls function for part creation
                break;
            }

            case '2': { //If option number 2 selected
                printf("\n------------------------"
                       "\n|Parts Inventory Update|"
                       "\n------------------------\n");
                partStockUpdate(); //Calls function for stock update
                break;
            }

            case '3': { //If option number 3 selected
                printf("\n--------------------------"
                       "\n|Parts Inventory Tracking|"
                       "\n--------------------------\n");
                partsTracking();
                break;
            }

            case '4': { //If option number 4 selected
                printf("\n----------------"
                       "\n|Part Searching|"
                       "\n----------------\n");
                partSearch(); //Calls function for part searching
                break;
            }

            case '5': { //Option to terminate the program and exit the system but file contents are still saved
                printf("\nSaving Inventory Files...\n");
                sleep(1);
                printf("\nExiting System...\n");
                sleep(1);
                printf("\n              ---------------"
                       "\n              |SYSTEM EXITED|"
                       "\n              ---------------");
                exit(0); //Exit function to terminate the program
                return 0;
            }

            default:
                printf("\nInvalid System Option Selected.\n"); //Default statement if invalid choice keyed in by user
        }
    }
}