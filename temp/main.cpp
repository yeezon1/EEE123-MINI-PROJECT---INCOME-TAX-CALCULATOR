#include <iostream>
#include "tax_calculator.h"



void compareAssessments(const std::string& name1, const std::string& icNo1, double income1, const std::string& name2, const std::string& icNo2, double income2, const std::vector<Expense>& expenses, const std::string& filename);

int main() {
    std::string name1, icNo1, name2, icNo2;
    double income1, income2;
    int assessmentChoice;

    // Input for Person 1
    std::cout << "Enter Person 1's name: ";
    std::getline(std::cin, name1);
    std::cout << "Enter Person 1's IC No.: ";
    std::getline(std::cin, icNo1);
    std::cout << "Enter Person 1's annual income (RM): ";
    std::cin >> income1;
    std::cin.ignore(); // Clear the input buffer

    // Choose assessment type
    std::cout << "Choose assessment type:\n";
    std::cout << "1. Individual\n";
    std::cout << "2. Joint\n";
    std::cout << "Enter your choice (1-2): ";
    std::cin >> assessmentChoice;
    std::cin.ignore(); // Clear the input buffer

    // Input for Person 2 (only if Joint Assessment is chosen)
    if (assessmentChoice == 2) {
        std::cout << "Enter Person 2's name: ";
        std::getline(std::cin, name2);
        std::cout << "Enter Person 2's IC No.: ";
        std::getline(std::cin, icNo2);
        std::cout << "Enter Person 2's annual income (RM): ";
        std::cin >> income2;
        std::cin.ignore(); // Clear the input buffer
    }

    // Common expenses
    std::vector<Expense> expenses = {
        {"EPF Contribution", 6000},
        {"Life Insurance", 3000},
        {"Medical Insurance", 2000},
        {"Education Fees", 4000},
        {"Donations", 1000},
        {"Parental Care", 2000},
        {"SSPN Savings", 3000},
        {"Lifestyle Expenses", 1500},
        {"Medical Expenses", 2500},
        {"Books and Equipment", 1000}
    };

    // Compare assessments
    if (assessmentChoice == 2) {
        compareAssessments(name1, icNo1, income1, name2, icNo2, income2, expenses, "comparison.txt");
    } else {
        // Individual Assessment only
        TaxCalculator individual(name1, icNo1, AssessmentType::INDIVIDUAL);
        for (const auto& expense : expenses) {
            individual.addExpense(expense.description, expense.amount);
        }
        individual.addIncomeSource("Salary", income1);
        individual.generateTaxSummary("individual_tax_summary.txt");
    }

    return 0;
}