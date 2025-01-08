#include "tax_calculator.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>



TaxCalculator::TaxCalculator(const std::string& name, const std::string& icNo, AssessmentType type)
    : name(name), icNo(icNo), assessmentType(type), totalDeductions(0) {}

void TaxCalculator::addIncomeSource(const std::string& type, double amount) {
    incomeSources.push_back({type, amount});
}

void TaxCalculator::addExpense(const std::string& description, double amount) {
    expenses.push_back({description, amount});
}

double TaxCalculator::calculateTotalDeductions() {
    totalDeductions = 0;
    for (const auto& expense : expenses) {
        totalDeductions += expense.amount;
    }
    return totalDeductions;
}

double TaxCalculator::calculateTaxableIncome() {
    double totalIncome = 0;
    for (const auto& income : incomeSources) {
        totalIncome += income.amount;
    }
    return totalIncome - calculateTotalDeductions();
}

double TaxCalculator::calculateIndividualTax(double taxableIncome) {
    double tax = 0;

    // Malaysian individual tax rates for 2023 (example rates)
    if (taxableIncome <= 5000) {
        tax = 0;
    } else if (taxableIncome <= 20000) {
        tax = (taxableIncome - 5000) * 0.01;
    } else if (taxableIncome <= 35000) {
        tax = 150 + (taxableIncome - 20000) * 0.03;
    } else if (taxableIncome <= 50000) {
        tax = 600 + (taxableIncome - 35000) * 0.06;
    } else if (taxableIncome <= 70000) {
        tax = 1800 + (taxableIncome - 50000) * 0.11;
    } else if (taxableIncome <= 100000) {
        tax = 4400 + (taxableIncome - 70000) * 0.19;
    } else if (taxableIncome <= 250000) {
        tax = 10300 + (taxableIncome - 100000) * 0.25;
    } else {
        tax = 50300 + (taxableIncome - 250000) * 0.28;
    }

    return tax;
}

double TaxCalculator::calculateJointTax(double taxableIncome) {
    double tax = 0;

    // Malaysian joint tax rates for 2023 (example rates)
    if (taxableIncome <= 10000) {
        tax = 0;
    } else if (taxableIncome <= 40000) {
        tax = (taxableIncome - 10000) * 0.02;
    } else if (taxableIncome <= 70000) {
        tax = 600 + (taxableIncome - 40000) * 0.05;
    } else if (taxableIncome <= 100000) {
        tax = 2100 + (taxableIncome - 70000) * 0.10;
    } else if (taxableIncome <= 200000) {
        tax = 5100 + (taxableIncome - 100000) * 0.16;
    } else if (taxableIncome <= 500000) {
        tax = 21100 + (taxableIncome - 200000) * 0.21;
    } else {
        tax = 84100 + (taxableIncome - 500000) * 0.24;
    }

    return tax;
}

double TaxCalculator::calculateSoleProprietorTax(double taxableIncome) {
    double tax = 0;

    // Malaysian sole proprietor tax rates for 2023 (example rates)
    if (taxableIncome <= 50000) {
        tax = taxableIncome * 0.15;
    } else if (taxableIncome <= 100000) {
        tax = 7500 + (taxableIncome - 50000) * 0.20;
    } else if (taxableIncome <= 200000) {
        tax = 17500 + (taxableIncome - 100000) * 0.25;
    } else {
        tax = 42500 + (taxableIncome - 200000) * 0.30;
    }

    return tax;
}

double TaxCalculator::calculateTax() {
    double taxableIncome = calculateTaxableIncome();

    switch (assessmentType) {
        case AssessmentType::INDIVIDUAL:
            return calculateIndividualTax(taxableIncome);
        case AssessmentType::JOINT:
            return calculateJointTax(taxableIncome);
        case AssessmentType::SOLE_PROPRIETOR:
            return calculateSoleProprietorTax(taxableIncome);
        default:
            return 0;
    }
}

std::string TaxCalculator::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    return std::string(buffer);
}

std::string TaxCalculator::getTaxDeadline() {
    // Assume tax deadline is April 30th of the current year
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[11];
    ltm->tm_mon = 3; // April (0-based month)
    ltm->tm_mday = 30;
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    return std::string(buffer);
}

int TaxCalculator::getDaysRemaining() {
    std::string deadline = getTaxDeadline();
    std::string currentDate = getCurrentDate();

    tm deadlineTm = {};
    tm currentTm = {};
    sscanf(deadline.c_str(), "%d-%d-%d", &deadlineTm.tm_year, &deadlineTm.tm_mon, &deadlineTm.tm_mday);
    sscanf(currentDate.c_str(), "%d-%d-%d", &currentTm.tm_year, &currentTm.tm_mon, &currentTm.tm_mday);
    deadlineTm.tm_year -= 1900; // Adjust year
    deadlineTm.tm_mon -= 1; // Adjust month (0-based)
    currentTm.tm_year -= 1900; // Adjust year
    currentTm.tm_mon -= 1; // Adjust month (0-based)

    time_t deadlineTime = mktime(&deadlineTm);
    time_t currentTime = mktime(&currentTm);

    double secondsRemaining = difftime(deadlineTime, currentTime);
    return static_cast<int>(secondsRemaining / (60 * 60 * 24));
}

void TaxCalculator::generateTaxSummary(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    outFile << "===================== TAX SUMMARY =====================\n";
    outFile << std::setw(20) << std::left << "Name" << ": " << name << "\n";
    outFile << std::setw(20) << "IC No." << ": " << icNo << "\n";
    outFile << std::setw(20) << "Assessment Type" << ": ";
    switch (assessmentType) {
        case AssessmentType::INDIVIDUAL:
            outFile << "Individual\n";
            break;
        case AssessmentType::JOINT:
            outFile << "Joint\n";
            break;
        case AssessmentType::SOLE_PROPRIETOR:
            outFile << "Sole Proprietor\n";
            break;
    }
    outFile << std::setw(20) << "Current Date" << ": " << getCurrentDate() << "\n";
    outFile << std::setw(20) << "Tax Deadline" << ": " << getTaxDeadline() << "\n";
    outFile << std::setw(20) << "Days Remaining" << ": " << getDaysRemaining() << "\n";
    outFile << "--------------------------------------------------------\n";
    outFile << std::setw(30) << "Income Source" << std::setw(15) << "Amount (RM)" << "\n";
    outFile << "--------------------------------------------------------\n";

    for (const auto& income : incomeSources) {
        outFile << std::setw(30) << income.type << std::setw(15) << income.amount << "\n";
    }

    outFile << "--------------------------------------------------------\n";
    outFile << std::setw(30) << "Total Income" << std::setw(15) << calculateTaxableIncome() + calculateTotalDeductions() << "\n";
    outFile << "--------------------------------------------------------\n";
    outFile << std::setw(30) << "Expense Description" << std::setw(15) << "Amount (RM)" << "\n";
    outFile << "--------------------------------------------------------\n";

    for (const auto& expense : expenses) {
        outFile << std::setw(30) << expense.description << std::setw(15) << expense.amount << "\n";
    }

    outFile << "--------------------------------------------------------\n";
    outFile << std::setw(30) << "Total Deductions" << std::setw(15) << calculateTotalDeductions() << "\n";
    outFile << std::setw(30) << "Taxable Income" << std::setw(15) << calculateTaxableIncome() << "\n";
    outFile << std::setw(30) << "Income Tax" << std::setw(15) << calculateTax() << "\n";
    outFile << "========================================================\n";

    outFile.close();
    std::cout << "Tax summary written to " << filename << std::endl;
}