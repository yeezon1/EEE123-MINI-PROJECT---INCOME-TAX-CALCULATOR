#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>

enum class AssessmentType { INDIVIDUAL, JOINT, SOLE_PROPRIETOR };

struct IncomeSource {
    std::string type;
    double amount;
};

struct Expense {
    std::string category;
    std::string description;
    double amount;
};

// List of allowed expense categories as per Malaysian tax laws
const std::vector<std::string> ALLOWED_CATEGORIES = {
    "Medical",
    "Insurance",
    "Education",
    "Donations",
    "Parental Care",
    "Savings",
    "Lifestyle",
    "Books and Equipment"
};

class TaxCalculator {
public:
    TaxCalculator(const std::string& name, const std::string& icNo, AssessmentType type)
        : name(name), icNo(icNo), assessmentType(type), totalDeductions(0) {}

    void addIncomeSource(const std::string& type, double amount) {
        incomeSources.push_back({type, amount});
    }

    void addExpense(const std::string& category, const std::string& description, double amount) {
        if (isCategoryAllowed(category)) {
            expenses.push_back({category, description, amount});
        } else {
            std::cerr << "Category '" << category << "' is not allowed as per tax laws.\n";
        }
    }

    double calculateTaxableIncome() {
        double totalIncome = 0;
        for (const auto& income : incomeSources) {
            totalIncome += income.amount;
        }
        return totalIncome - calculateTotalDeductions();
    }

    double calculateTax() {
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

    void generateTaxSummary(const std::string& filename) {
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
        outFile << std::setw(30) << "Expense Category" << std::setw(20) << "Description" << std::setw(15) << "Amount (RM)" << "\n";
        outFile << "--------------------------------------------------------\n";

        for (const auto& expense : expenses) {
            outFile << std::setw(30) << expense.category << std::setw(20) << expense.description << std::setw(15) << expense.amount << "\n";
        }

        outFile << "--------------------------------------------------------\n";
        outFile << std::setw(30) << "Total Deductions" << std::setw(15) << calculateTotalDeductions() << "\n";
        outFile << std::setw(30) << "Taxable Income" << std::setw(15) << calculateTaxableIncome() << "\n";
        outFile << std::setw(30) << "Income Tax" << std::setw(15) << calculateTax() << "\n";
        outFile << "========================================================\n";

        outFile.close();
        std::cout << "Tax summary written to " << filename << std::endl;
    }

private:
    std::string name;
    std::string icNo;
    AssessmentType assessmentType;
    std::vector<IncomeSource> incomeSources;
    std::vector<Expense> expenses;
    double totalDeductions;

    bool isCategoryAllowed(const std::string& category) {
        return std::find(ALLOWED_CATEGORIES.begin(), ALLOWED_CATEGORIES.end(), category) != ALLOWED_CATEGORIES.end();
    }

    double calculateTotalDeductions() {
        totalDeductions = 0;
        for (const auto& expense : expenses) {
            totalDeductions += expense.amount;
        }
        return totalDeductions;
    }

    std::string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return std::string(buffer);
    }

    std::string getTaxDeadline() {
        // Assume tax deadline is April 30th of the current year
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        ltm->tm_mon = 3; // April (0-based month)
        ltm->tm_mday = 30;
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return std::string(buffer);
    }

    int getDaysRemaining() {
        std::string deadline = getTaxDeadline();
        std::string currentDate = getCurrentDate();

        tm deadlineTm = {};
        tm currentTm = {};
        std::istringstream deadlineStream(deadline);
        deadlineStream >> std::get_time(&deadlineTm, "%Y-%m-%d");

        std::istringstream currentStream(currentDate);
        currentStream >> std::get_time(&currentTm, "%Y-%m-%d");

        time_t deadlineTime = mktime(&deadlineTm);
        time_t currentTime = mktime(&currentTm);

        double secondsRemaining = difftime(deadlineTime, currentTime);
        return static_cast<int>(secondsRemaining / (60 * 60 * 24));
    }

    double calculateIndividualTax(double taxableIncome) {
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

    double calculateJointTax(double taxableIncome) {
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

    double calculateSoleProprietorTax(double taxableIncome) {
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
};

void compareAssessments(const std::string& name1, const std::string& icNo1, double income1,
                        const std::string& name2, const std::string& icNo2, double income2,
                        const std::vector<Expense>& expenses, const std::string& filename) {
    // Create Individual Assessment for Person 1
    TaxCalculator individual1(name1, icNo1, AssessmentType::INDIVIDUAL);
    for (const auto& expense : expenses) {
        individual1.addExpense(expense.category, expense.description, expense.amount);
    }
    individual1.addIncomeSource("Salary", income1);

    // Create Individual Assessment for Person 2
    TaxCalculator individual2(name2, icNo2, AssessmentType::INDIVIDUAL);
    for (const auto& expense : expenses) {
        individual2.addExpense(expense.category, expense.description, expense.amount);
    }
    individual2.addIncomeSource("Salary", income2);

    // Create Joint Assessment
    TaxCalculator joint(name1 + " & " + name2, icNo1 + ", " + icNo2, AssessmentType::JOINT);
    for (const auto& expense : expenses) {
        joint.addExpense(expense.category, expense.description, expense.amount);
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

    // Input expenses by category
    std::vector<Expense> expenses;
    std::cout << "Enter your expenses (type 'done' to finish):\n";
    while (true) {
        std::string category, description;
        double amount;

        // Display allowed categories
        std::cout << "Allowed categories:\n";
        for (const auto& cat : ALLOWED_CATEGORIES) {
            std::cout << "- " << cat << "\n";
        }

        std::cout << "Expense category (or 'done'): ";
        std::getline(std::cin, category);
        if (category == "done") break;

        std::cout << "Expense description: ";
        std::getline(std::cin, description);

        std::cout << "Amount (RM): ";
        std::cin >> amount;
        std::cin.ignore(); // Clear the input buffer

        expenses.push_back({category, description, amount});
    }

    // Compare assessments
    if (assessmentChoice == 2) {
        compareAssessments(name1, icNo1, income1, name2, icNo2, income2, expenses, "comparison.txt");
    } else {
        // Individual Assessment only
        TaxCalculator individual(name1, icNo1, AssessmentType::INDIVIDUAL);
        for (const auto& expense : expenses) {
            individual.addExpense(expense.category, expense.description, expense.amount);
        }
        individual.addIncomeSource("Salary", income1);
        individual.generateTaxSummary("individual_tax_summary.txt");
    }

    return 0;
}