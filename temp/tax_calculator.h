#ifndef TAX_CALCULATOR_H
#define TAX_CALCULATOR_H

#include <string>
#include <vector>
#include <ctime>
#include <map>


struct Expense {
    std::string description;
    double amount;
};

void compareAssessments(const std::string& name1, const std::string& icNo1, double income1, const std::string& name2, const std::string& icNo2, double income2, const std::vector<Expense>& expenses, const std::string& filename);
enum class AssessmentType { INDIVIDUAL, JOINT, SOLE_PROPRIETOR };

struct IncomeSource {
    std::string type;
    double amount;
};

class TaxCalculator {
public:
    TaxCalculator(const std::string& name, const std::string& icNo, AssessmentType type);
    void addIncomeSource(const std::string& type, double amount);
    void addExpense(const std::string& description, double amount);
    double calculateTaxableIncome();
    double calculateTax();
    void generateTaxSummary(const std::string& filename);

private:
    std::string name;
    std::string icNo;
    AssessmentType assessmentType;
    std::vector<IncomeSource> incomeSources;
    std::vector<Expense> expenses;
    double totalDeductions;
    double calculateTotalDeductions();
    std::string getCurrentDate();
    std::string getTaxDeadline();
    int getDaysRemaining();
    double calculateIndividualTax(double taxableIncome);
    double calculateJointTax(double taxableIncome);
    double calculateSoleProprietorTax(double taxableIncome);
};

#endif