# include <string>

using namespace std;

// function declaration
bool askQuestion(const string& question);
int getNumberInput(const string& prompt);
void AskQuestionForSingle(int dexpenses[]);
void AskQuestionForMarried(int dexpenses[]);
int calculateTotalDeductible(int dexpenses[], int size);
void displayDeductibleTable(int dexpenses[]);
void selectionexpenses();