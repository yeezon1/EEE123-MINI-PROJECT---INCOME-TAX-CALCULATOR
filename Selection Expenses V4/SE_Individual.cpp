//Programmer: OOI YEEZON
//USM Email: yeezon@student.usm.my
//GitHub Username: yeezon1
//Matric No.: 23301291
//Selection expenses for individual

#include <iostream>
#include <iomanip>
#include <string>
#include "SE_Individual.hpp"

using namespace std;

// Maximum deductions for each expense category
const int MaxDeductions[23] = {
    9000,  // 1. Individual and dependent relatives
    8000,  // 2. Expenses for parents (medical, dental, etc.)
    6000,  // 3. Purchase of basic supporting equipment for disabled
    6000,  // 4. Disabled individual
    7000,  // 5. Education fees (self)
    10000, // 6. Medical expenses (serious diseases, fertility, etc.)
    1000,  // 7. Expenses (medical examination, COVID-19, mental health)
    4000,  // 8. Expenses for child (intellectual disability, early intervention)
    2500,  // 9. Lifestyle (books, computers, internet, courses)
    1000,  // 10. Lifestyle (sports equipment, gym membership)
    1000,  // 11. Breastfeeding equipment
    3000,  // 12. Child care fees
    8000,  // 13. Skim Simpanan Pendidikan Nasional
    4000,  // 14. Husband/wife/alimony
    5000,  // 15. Disabled husband/wife
    2000,  // 16. Unmarried child under 18
    2000,  // 17. Unmarried child 18+ (A-Level, diploma, etc.)
    6000,  // 18. Disabled child
    7000,  // 19. Life insurance and EPF
    3000,  // 20. Deferred Annuity and PRS
    3000,  // 21. Education and medical insurance
    350,   // 22. SOCSO contribution
    2500   // 23. Electric vehicle charging facilities
};

// Array of category descriptions
const string categories[23] = {
    "individual and dependent relatives",
    "expenses for parents (medical, dental, etc.)",
    "purchase of basic supporting equipment for disabled",
    "disabled individual",
    "education fees (self)",
    "medical expenses (serious diseases, fertility, etc.)",
    "expenses (medical examination, COVID-19, mental health)",
    "expenses for child (intellectual disability, early intervention)",
    "lifestyle (books, computers, internet, courses)",
    "lifestyle (sports equipment, gym membership)",
    "breastfeeding equipment",
    "child care fees",
    "Skim Simpanan Pendidikan Nasional",
    "husband/wife/alimony",
    "disabled husband/wife",
    "unmarried child under 18",
    "unmarried child 18+ (A-Level, diploma, etc.)",
    "disabled child",
    "life insurance and EPF",
    "deferred annuity or PRS",
    "education or medical insurance",
    "SOCSO contributions",
    "electric vehicle charging facilities"
};

// Function to handle the selection of expenses
void selectionexpenses()
{
    int dexpenses[23] = {0}; // Initialize all deductible expenses to 0

    // Display the allowed categories for expenses
    cout << "\n================================================================================================\n";
    cout << "                               <Part 3. Relief Selection>\n";
    cout << "================================================================================================\n";
    cout << "The following list is the allowed categories for expenses:\n";
    cout << "+----+-------------------------------------------------------------------+---------------------+\n";
    cout << "| No | Category                                                          | Maximum Deduction   |\n";
    cout << "+----+-------------------------------------------------------------------+---------------------+\n";
    for (int i = 0; i < 23; i++)
    {
        cout << "| " << setw(2) << i + 1 << " | " << left << setw(65) << setfill(' ')
             << categories[i] // Use the array for category descriptions
             << " | RM " << setw(16) << right << MaxDeductions[i] << " |\n";
    }
    cout << "+----+-------------------------------------------------------------------+---------------------+\n";

    // Ask for marital status
    char maritalstatus;
    while (true)
    {
        cout << "\nSelect your marital status:\n";
        cout << "S. Single\n";
        cout << "M. Married\n";
        cout << "D. Divorced\n";
        cout << "Enter your choice (S, M or D): ";
        string input; // Use a string to read the entire line
        getline(cin, input); // Read the entire line

        // Check if the input is a single character and is either 'S', 'M', or 'D'
        if (input.length() == 1 && (input[0] == 'S' || input[0] == 'M' || input[0] == 'D'))
        {
            maritalstatus = input[0]; // Assign the valid character to maritalstatus
            break; // Exit the loop
        }
        cout << "Invalid input. Please enter only a single character (S, M or D).\n";
    }

    // Handle expenses based on marital status
    if (maritalstatus == 'S')
    {
        AskQuestionForSingle(dexpenses);
    }
    else
    {
        AskQuestionForMarried(dexpenses);
    }

    // Display the deductible amounts in a table
    displayDeductibleTable(dexpenses);

    // Calculate and display the total deductible
    int total_deductible = calculateTotalDeductible(dexpenses, 23);
    cout << "\nTotal deductible amount for all categories: RM " << total_deductible << ".\n";
}

// Function to ask a yes/no question
bool askQuestion(const string& question)
{
    string input;

    while (true)
    {
        cout << "\n" << question << "\n";
        cout << "Y. Yes\n";
        cout << "N. No\n";
        cout << "Enter your choice (Y or N): ";
        getline(cin, input); // Read the entire line

        // Check if the input is a single character and is either 'Y' or 'N'
        if (input.length() == 1 && (input[0] == 'Y' || input[0] == 'N'))
        {
            return (input[0] == 'Y'); // Return true for 'Y'
        }
        cout << "Invalid input. Please enter only a single character (Y or N).\n";
    }
}

// Function to get a valid number input
int getNumberInput(const string& prompt)
{
    int num;

    while (true)
    {
        cout << prompt;
        cin >> num;
        
        if (cin.fail() || num < 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter again.\n";
        }
        else
        {
            cin.ignore(1000, '\n'); // Clear the input buffer
            return num;
        }
    }
}

// Function to handle questions for single individuals
void AskQuestionForSingle(int dexpenses[])
{
    int categoriesForSingle[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 18, 19, 20, 21, 22}; // Categories for single
    
    for (int i : categoriesForSingle)
    {
        string category = categories[i]; // Get category description from the array

        if (askQuestion("Do you have expenses for " + category + "?"))
        {
            int expenses = getNumberInput("Please enter your expenses for " + category + " (RM): ");
            dexpenses[i] = min(expenses, MaxDeductions[i]);
            cout << "Deductible amount: RM " << dexpenses[i] << ".\n";
        }
        else
        {
            cout << "You do not have expenses for " << category << ".\n";
        }
    }
}

// Function to handle questions for married individuals
void AskQuestionForMarried(int dexpenses[])
{
    for (int i = 0; i < 23; i++)
    {
        // Ask if the user has any children
        bool hasChildren = askQuestion("Do you have any children?");
        
        // Categories to ask if the user has children
        int childCategories[] = {7, 11, 15, 16, 17}; // 8, 12, 16, 17, 18 (adjusted for 0-based index)
        
        for (int i = 0; i < 23; i++)
        {
            // Skip child-related categories if the user has no children
            if (!hasChildren && (i == 7 || i == 11 || i == 15 || i == 16 || i == 17))
            {
                continue; // Skip these categories
            }

            string category = categories[i]; // Get category description from the array

            if (askQuestion("Do you have expenses for " + category + "?"))
            {
                if (i == 15 || i == 16) // Categories depending on the number of children
                {
                    int numChildren = getNumberInput("Enter the number of " + category + ": ");
                    dexpenses[i] = numChildren * 2000;
                }
                else if (i == 17) // Categories depending on the number of children
                {
                    int numChildren = getNumberInput("Enter the number of " + category + ": ");
                    dexpenses[i] = numChildren * 6000;
                }
                else
                {
                    int expenses = getNumberInput("Please enter your expenses for " + category + " (RM): ");
                    dexpenses[i] = min(expenses, MaxDeductions[i]);
                }
                cout << "Deductible amount: RM " << dexpenses[i] << ".\n";
            }
            else
            {
                cout << "You do not have expenses for " << category << ".\n";
            }
        }
    }
}

// Function to calculate total deductible
int calculateTotalDeductible(int dexpenses[], int size)
{
    int total = 0;

    for (int i = 0; i < size; i++)
    {
        total += dexpenses[i];
    }
    return total;
}

// Function to display the deductible amounts in a table
void displayDeductibleTable(int dexpenses[])
{
    cout << "\n===============================================================================================\n";
    cout << "                               <Deductible Amounts>\n";
    cout << "================================================================================================\n";
    cout << "+----+-------------------------------------------------------------------+---------------------+\n";
    cout << "| No | Category                                                          | Deductible Amount   |\n";
    cout << "+----+-------------------------------------------------------------------+---------------------+\n";
    for (int i = 0; i < 23; i++)
    {
        if (dexpenses[i] > 0)
        {
            cout << "| " << setw(2) << i + 1 << " | " << left << setw(65) << setfill(' ')
                 << categories[i] // Use the array for category descriptions
                 << " | RM " << setw(16) << right << dexpenses[i] << " |\n";
        }
    }
    cout << "+----+-------------------------------------------------------------------+---------------------+\n";
}

// Main function
int main() {
    selectionexpenses();
    return 0;
}