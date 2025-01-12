//Programmer: OOI YEEZON
//USM Email: yeezon@student.usm.my
//GitHub Username: Yeezon1
//Matric No.: 23301291

//Selection expenses for joint

# include <iostream>
# include <string>
# include <set>
# include "S.E.joint.hpp"

using namespace std;

/*
MAXIMUM DEDUCTIONS FOR EACH EXPENSES CATEGORY
1. Personal Relief: 9000
2. Disability Relief: 6000
3. Lifestyle Relief: 2500
4. Medical Expenses Relief: 8000
5. Education Fees Relief: 7000
6. Married Couple Relief: 4000
7. Disabled Spouse Relief: 3500
8. Child Relief: 2000 per child
9. Parent Relief: 1500 per parent
*/

// Constant for the maximum deductions for each expenses category
const int MaxDeductions [5] = {9000, 6000, 2500, 8000, 7000,};

int main()
{
    // EXTRA PART
    // User input their income (annual)
    int income1, income2;
    cout << "Enter your income (annual):\n";
    cout << "Person 1 > RM";    
    cin >> income1;
    cout << "Person 2 > RM";    
    cin >> income2;

    //Sum the total of income
    int income = income1 + income2;

    // Set to track already selected categories
    // Prevent User repeated input same expenses category
    set<int> selectedCategories;

    // Total Expenses
    int TotalExpenses;
    TotalExpenses = 0;

    // Code of choice of expenses (User can only input 0-9)
    int CodeOfChoice; 
    
    // DISPLAY the allowed categories for EXPENSES
    cout << "\nThe following list is the allowed categories for expenses\n";
    cout << "1. Personal Relief\n";
    cout << "2. Disability Relief\n";
    cout << "3. Lifestyle Relief\n";
    cout << "4. Medical Expenses Relief\n";
    cout << "5. Education Fees Relief\n";
    cout << "6. Married Couple Relief\n";
    cout << "7. Disabled Spouse Relief\n";
    cout << "8. Child Relief\n";
    cout << "9. Parent Relief\n";
    cout << "0. Finish and Exit\n";

    // Guide for User to input
    cout << "\nEnter your expenses category (1-9) or '0' to finish\n";

    // User input Choice
    while (true)
    {
        //User input their choice here (0-7)
        cout << "\nCategory ";
        cin >> CodeOfChoice;

        // Condition to exit the loop
        if (CodeOfChoice == 0)
        {
            cout << "Finished selection\n";
            break;
        }

        // Avoid user repeated input same category
        if (selectedCategories.find(CodeOfChoice) != selectedCategories.end())
        {
            cout << "You have already selected this category. Please choose a different category.\n";
            continue;
        }

        // Amount of User expense
        int expense;
        expense = GetCodeOfChoice (CodeOfChoice);

        // Calculate amount deductible
        int deductible;
        if (CodeOfChoice == 6 || CodeOfChoice == 7 || CodeOfChoice == 8 || CodeOfChoice == 9)
        {
            deductible = expense; // No maximum limit
        } 
        else 
        {
            deductible = Calc_deductible(expense, CodeOfChoice);
        }
        TotalExpenses = TotalExpenses + deductible;

        // Add the category to the set of selected categories
        selectedCategories.insert(CodeOfChoice);

        // Display amount can be deducted
        if (CodeOfChoice == 6 || CodeOfChoice == 7 || CodeOfChoice == 8 || CodeOfChoice == 9)
        {
           // Direct show user amount deducted
           cout << "Deduct RM " << deductible << "\n"; 
        }
        else if (CodeOfChoice == 1 || CodeOfChoice == 2 || CodeOfChoice == 3 || CodeOfChoice == 4 || CodeOfChoice == 5)
        {
            // Show user the amount that can be deducted and maximum amount can be deducted
            cout << "Deduct RM " << deductible << " (max allowable: RM " << MaxDeductions[CodeOfChoice - 1] << ").\n";
        }

    }

    // Calculate Remaining Income
    // Remark: Remaining income is used for further tax calculation
    int RemainingIncome;
    RemainingIncome = income - TotalExpenses;

    // Display results
    cout << "\nIncome: RM" << income << "\n";
    cout << "Total deductible expenses: RM" << TotalExpenses << "\n";
    cout << "Remaining income after deductions: RM" << RemainingIncome << "\n";

    return 0;
}

int GetCodeOfChoice (int a)
{
    int category = a;
    int expense;

    switch (category)
    {
        case 1:
            cout << "1. Personal Relief\n";
            cout << "Enter the value of your personal expenses > ";
            cin >> expense;
            break;
        case 2:
            cout << "2. Disability Relief\n";
            cout << "Enter the value of your disability expenses > ";
            cin >> expense;
            break;
        case 3:
            cout << "3. Lifestyle Relief\n";
            cout << "Enter the value of your lifestyle expenses > ";
            cin >> expense;
            break;
        case 4:
            cout << "4. Medical Expenses Relief\n";
            cout << "Enter the value of your medical expenses > ";
            cin >> expense;
            break;
        case 5:
            cout << "5. Education Fees Relief\n";
            cout << "Enter the value of your education fee expenses > ";
            cin >> expense;
            break;
        case 6: // Direct deduction
            expense = 4000;
            cout << "6. Married Couple Relief\n";
            break;
        case 7: // Direct deduction
            expense = 3500;
            cout << "7. Disabled Spouse Relief\n";
            break;
        case 8: // Depend on Number of Child
            int NoOfChild; //Number of Child
            cout << "6. Child Relief\n";
            cout << "Enter the Number of child > ";
            cin >> NoOfChild;
            expense = NoOfChild * 2000;
            break;
        case 9: // Depend on Number of Parents
            int NoOfParent; //Number of Parents
            cout << "7. Parent Relief\n";
            cout << "Enter the Number of parent > ";
            cin >> NoOfParent;
            expense = NoOfParent * 1500;
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 9.\n";
            break;
    }
    return expense;
}

// Formula to calculate amount deductible
int Calc_deductible(int a, int b)
{
    int deductible;
    deductible = min(a, MaxDeductions[b - 1]);
    return deductible;
}