#include "tax_calculator.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void compareAssessments(const std::string& name1, const std::string& icNo1, double income1,
                        const std::string& name2, const std::string& icNo2, double income2,
                        const std::vector<Expense>& expenses, const std::string& filename) {
    // Create Individual Assessment for Person 1
    TaxCalculator individual1(name1, icNo1, AssessmentType::INDIVIDUAL);
    for (const auto& expense : expenses) {
        individual1.addExpense(expense.description, expense.amount);
    }
    individual1.addIncomeSource("Salary", income1);

    // Create Individual Assessment for Person 2
    TaxCalculator individual2(name2, icNo2, AssessmentType::INDIVIDUAL);
    for (const auto& expense : expenses) {
        individual2.addExpense(expense.description, expense.amount);
    }
    individual2.addIncomeSource("Salary", income2);

    // Create Joint Assessment
    TaxCalculator joint(name1 + " & " + name2, icNo1 + ", " + icNo2, AssessmentType::JOINT);
    for (const auto& expense : expenses) {
        joint.addExpense(expense.description, expense.amount);
    }
    joint.addIncomeSource("Salary", income1 + income2);

    // Calculate taxes
    double individualTax1 = individual1.calculateTax();
    double individualTax2 = individual2.calculateTax();
    double totalIndividualTax = individualTax1 + individualTax2;
    double jointTax = joint.calculateTax();

    // Write comparison to file
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    outFile << "===================== TAX COMPARISON =====================\n";
    outFile << std::setw(30) << std::left << "Assessment Type" << std::setw(20) << "Tax (RM)" << "\n";
    outFile << "--------------------------------------------------------\n";
    outFile << std::setw(30) << "Individual Assessment (Person 1)" << std::setw(20) << individualTax1 << "\n";
    outFile << std::setw(30) << "Individual Assessment (Person 2)" << std::setw(20) << individualTax2 << "\n";
    outFile << std::setw(30) << "Total Individual Tax" << std::setw(20) << totalIndividualTax << "\n";
    outFile << std::setw(30) << "Joint Assessment" << std::setw(20) << jointTax << "\n";
    outFile << "--------------------------------------------------------\n";

    if (jointTax < totalIndividualTax) {
        outFile << "Joint Assessment provides lower tax.\n";
    } else if (jointTax > totalIndividualTax) {
        outFile << "Individual Assessment provides lower tax.\n";
    } else {
        outFile << "Both assessments result in the same tax.\n";
    }

    outFile << "========================================================\n";

    outFile.close();
    std::cout << "Tax comparison written to " << filename << std::endl;
}