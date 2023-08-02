#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Menu {
public:
    static void Main_Menu() {
        cout << "Create Quiz    [1]" << endl
            << "Start Quiz     [2]" << endl
            << "Leader Board   [3]" << endl
            << "Exit           [0]" << endl;
    }
};

class Questions {
    string question;
    vector<string> answerChoices;
    int correctAnswerIndex;
    int point;
public:
    Questions(const string& question, const vector<string>& answerChoices, int correctAnswerIndex, int point)
        : question(question), answerChoices(answerChoices), correctAnswerIndex(correctAnswerIndex), point(point) {}

    const string& GetQuestion() const { return question; }

    const vector<string>& GetAnswerChoices() const { return answerChoices; }

    int GetCorrectAnswerIndex() const { return correctAnswerIndex; }

    int GetPoint() const { return point; }

    void SetPoint(int newPoint) { point = newPoint; }
};

class Quiz {
    vector<string> existingQuizzes;
    vector<Questions> q;
    string quizname;
    int totalPoints;
    int score;
    int numCorrect;
    int numSkipped;
    int numberofquestion;
public:
    Quiz() = default;

    Quiz(const string& quizname) : quizname(quizname), totalPoints(0), numberofquestion(0),
        numCorrect(0), numSkipped(0),score(0) {
        LoadExistingQuizzes();
    }

    void AddQuestion(const string& question, const vector<string>& answerChoices, int correctAnswerIndex, int point) {
        q.push_back(Questions(question, answerChoices, correctAnswerIndex, point));
        ++numberofquestion;
        totalPoints += point;
    }

    void SetNumberOfQuestions(int num) {
        if (numberofquestion!=0)
        {
            numberofquestion = num;
        }
    }

    void SetScore(int score) {
        this->score = score;
    }

    int GetScore() { return score; }

    int NumberOfQuestions() const {
        return q.size();
    }

    int GetNumberOfQuestions() const {
        return numberofquestion;
    }

    const string& GetQuizName() const {
        return quizname;
    }

    void SaveExistingQuizzes() {
        ofstream outFile("existing_quizzes.txt");
        if (!outFile) {
            cerr << "Error saving existing quizzes file." << endl;
            return;
        }

        for (const string& quizFile : existingQuizzes) {
            outFile << quizFile << endl;
        }

        outFile.close();
    }

    void LoadExistingQuizzes() {
        ifstream inFile("existing_quizzes.txt");
        if (!inFile) {
            return;
        }

        string quizFile;
        while (getline(inFile, quizFile)) {
            existingQuizzes.push_back(quizFile);
        }

        inFile.close();
    }

    void SaveQuizToFile() {
        ofstream outFile(quizname + ".txt");
        if (!outFile) {
            cout << "Error saving the quiz file." << endl;
            return;
        }

        for (const Questions& question : q) {
            outFile << question.GetQuestion() << endl;
            for (const string& answer : question.GetAnswerChoices()) {
                outFile << answer << endl;
            }
            outFile << question.GetCorrectAnswerIndex() << endl;
            outFile << question.GetPoint() << endl;
        }

        outFile.close();
        cout << "Quiz saved successfully!" << endl;
    }

    void LoadQuizFromFile() {
        ifstream inFile(quizname + ".txt");
        if (!inFile) {
            cerr << "Error loading the quiz file: " << quizname << ".txt" << endl;
            return;
        }

        string question;
        vector<string> answerChoices;
        int correctAnswerIndex, point;

        q.clear();

        while (getline(inFile, question)) {
            answerChoices.clear();
            for (int i = 0; i < 4; ++i) {
                string answer;
                getline(inFile, answer);
                answerChoices.push_back(answer);
            }
            inFile >> correctAnswerIndex;
            inFile >> point;
            inFile.ignore();

            q.push_back(Questions(question, answerChoices, correctAnswerIndex, point));
            ++numberofquestion;
            totalPoints += point;
        }

        inFile.close();
        cout << "Quiz loaded successfully!" << endl;
    }

    void CreateQuiz() {
        cin.ignore();
        cin.clear();
        cout << "Enter Quiz name: ";
        getline(cin, quizname);

        int currentQuestionIndex = 0;
        int totalQuestions = 0;

        while (true) {
            if (currentQuestionIndex == q.size()) {
                string question;
                vector<string> answerChoices(4);
                int correctAnswerIndex;
                int point;

                cin.ignore();
                cin.clear();
                cout << "Question " << currentQuestionIndex + 1 << ": ";
                getline(cin, question);

                for (int j = 0; j < 4; ++j) {
                    cout << "Answer " << j + 1 << ": ";
                    cin.ignore();
                    cin.clear();
                    getline(cin, answerChoices[j]);
                }

                cout << "Enter the correct answer (1-4): ";
                cin >> correctAnswerIndex;

                cout << "Enter the point for this question: ";
                cin >> point;

                q.push_back(Questions(question, answerChoices, correctAnswerIndex - 1, point));
                totalPoints += point;

                ++totalQuestions;
            }

            cout << endl;
            cout << "1. Next" << endl;
            if (currentQuestionIndex > 0) {
                cout << "2. Previous" << endl;
            }
            cout << "3. Save" << endl;
            cout << "4. Modify Current Question and Answers" << endl;
            cout << "Enter your choice: ";

            int option;
            cin >> option;

            if (option == 1) {
                ++currentQuestionIndex;
            }
            else if (option == 2 && currentQuestionIndex > 0) {
                --currentQuestionIndex;
            }
            else if (option == 3) {
                cout << "Saving the quiz..." << endl;
                SaveQuizToFile();
                break;
            }
            else if (option == 4) {
                ModifyCurrentQuestion(currentQuestionIndex);
            }
            else {
                cout << "Invalid option. Please try again." << endl;
            }

            cin.ignore(); 
            if (currentQuestionIndex >= 0 && currentQuestionIndex < q.size()) {
                cout << "Question " << currentQuestionIndex + 1 << ": " << q[currentQuestionIndex].GetQuestion() << endl;
                vector<string> answerChoices = q[currentQuestionIndex].GetAnswerChoices();
                for (int j = 0; j < 4; ++j) {
                    cout << "Answer " << j + 1 << ": " << answerChoices[j] << endl;
                }
            }

        }

        existingQuizzes.push_back(quizname);
        SaveExistingQuizzes();
    }

    void ModifyCurrentQuestion(int currentQuestionIndex) {
        if (currentQuestionIndex >= 0 && currentQuestionIndex < q.size()) {
            string question;
            vector<string> answerChoices(4); 
            int correctAnswerIndex;
            int point;

            cout << "Modify Question " << currentQuestionIndex + 1 << ": ";
            cin.ignore();
            getline(cin, question);

            for (int j = 0; j < 4; ++j) {
                cout << "Modify Answer " << j + 1 << ": ";
                getline(cin, answerChoices[j]);
            }

            cout << "Modify the correct answer (1-4): ";
            cin >> correctAnswerIndex;

            cout << "Modify the point for this question: ";
            cin >> point;


            totalPoints = totalPoints - q[currentQuestionIndex].GetPoint() + point;
            q[currentQuestionIndex] = Questions(question, answerChoices, correctAnswerIndex - 1, point);

            cout << "Question and answers modified successfully!" << endl;
        }
        else {
            cout << "Invalid question index." << endl;
        }
    }

    void StartQuiz() {
        
        int totalQuestions = q.size();
        if (totalQuestions == 0) {
            cout << "No questions available in this quiz." << endl;
            return;
        }

        int maxQuestions = min(totalQuestions, numberofquestion);
        vector<int> questionIndices;
        for (int i = 0; i < totalQuestions; ++i) {
            questionIndices.push_back(i);
        }

        int currentQuestionIndex = 0;
        int userAnswer;

        while (currentQuestionIndex < maxQuestions) {
            int questionIndex = questionIndices[currentQuestionIndex];
            const Questions& currentQuestion = q[questionIndex];

            cout << "Question " << currentQuestionIndex + 1 << ": " << currentQuestion.GetQuestion() << endl;
            const vector<string>& answerChoices = currentQuestion.GetAnswerChoices();
            for (size_t j = 0; j < answerChoices.size(); ++j) {
                cout << j + 1 << ": " << answerChoices[j] << endl;
            }

            cout << "Enter your answer (1-4, 0 to skip): ";
            cin >> userAnswer;

            if (userAnswer >= 1 && userAnswer <= 4) {
                if (userAnswer - 1 == currentQuestion.GetCorrectAnswerIndex()) {
                    cout << "Correct!" << endl;
                    score += currentQuestion.GetPoint();
                    numCorrect++;
                }
                else {
                    cout << "Incorrect!" << endl;
                }
            }
            else if (userAnswer == 0) {
                cout << "Skipped!" << endl;
                numSkipped++; 
            }
            else {
                cout << "Invalid choice. Skipping the question." << endl;
            }
            cout << endl;

            int choice;

            if (currentQuestionIndex == maxQuestions - 1) {

                cout << "1. Next" << endl;
            }
            if (currentQuestionIndex > 0) {
                cout << "2. Previous" << endl;
            }
            cout << "3. Save" << endl;

            if (currentQuestionIndex == maxQuestions - 1) {
                cout << "4. Finish " << endl;
            }

            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1) {
                ++currentQuestionIndex;
            }
            else if (choice == 2 && currentQuestionIndex > 0) {
                --currentQuestionIndex;
            }
            else if (choice == 3) {
                SaveQuizToFile();
                cout << "Quiz saved successfully!" << endl;
                cout << "Quiz finished!" << endl;
                break;
            }
            else if (choice == 4 && currentQuestionIndex == maxQuestions - 1) {
                cout << "Quiz finished!" << endl;
                break;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }

        cout << "Result" << endl;
        cout << "Score: " << score << " out of " << totalPoints << endl;
        cout << "Correct: " << numCorrect << endl;
        cout << "Incorrect: " << (maxQuestions - numCorrect - numSkipped) << endl;
        cout << "Skipped: " << numSkipped << endl;
        SetScore(score);
    }

    vector<string> GetExistingQuizzes() {
        ifstream inFile("existing_quizzes.txt");
        vector<string> existingQuizzes;
        if (inFile) {
            string quizName;
            while (getline(inFile, quizName)) {
                existingQuizzes.push_back(quizName);
            }
            inFile.close();
        }
        return existingQuizzes;
    }
};

class Leaderboard {
    class Entry {
    public:
        string name;
        int totalScore;

        Entry(const string& name) : name(name), totalScore(0) {}
    };

    vector<Entry> leaderboard;

public:
    void AddEntry(const string& name, int quizScore) {
        auto it = find_if(leaderboard.begin(), leaderboard.end(), [&](const Entry& entry) {
            return entry.name == name;
            });

        if (it != leaderboard.end()) {
            it->totalScore += quizScore;
        }
        else {
            leaderboard.push_back(Entry(name));
            leaderboard.back().totalScore = quizScore;
        }

        sort(leaderboard.begin(), leaderboard.end(), [](const Entry& a, const Entry& b) {
            return a.totalScore > b.totalScore;
            });
    }

    void ShowLeaderboard() {
        cout << "Leaderboard" << endl;
        cout << "Name\t\tTotal Score" << endl;
        for (const Entry& entry : leaderboard) {
            cout << entry.name << "\t\t" << entry.totalScore << endl;
        }
    }

    void SaveLeaderboardToFile() {
        ofstream outFile("leaderboard.txt");
        if (!outFile) {
            cerr << "Error saving leaderboard to file." << endl;
            return;
        }

        for (const Entry& entry : leaderboard) {
            outFile << entry.name << "\t" << entry.totalScore << endl;
        }

        outFile.close();
        cout << "Leaderboard saved successfully!" << endl;
    }

    void LoadLeaderboardFromFile() {
        ifstream inFile("leaderboard.txt");
        if (!inFile) {
            return;
        }

        leaderboard.clear();

        string name;
        int totalScore;

        while (inFile >> name >> totalScore) {
            leaderboard.push_back(Entry(name));
            leaderboard.back().totalScore = totalScore;
        }

        inFile.close();
        cout << "Leaderboard loaded successfully!" << endl;
    }
};

int main() {
    int choice=0;
    string Name;
    Quiz quiz;
    Leaderboard leaderboard;

    cout << "Enter your name: ";
    getline(cin, Name);

    leaderboard.LoadLeaderboardFromFile();

    while (choice != 0) {
        Menu::Main_Menu();
        cout << "Enter your choice: ";
        cin >> choice;
        vector<string> existingQuizzes;
        switch (choice) {
        case 1:
            quiz.CreateQuiz();
            break;

        case 2:
            existingQuizzes = quiz.GetExistingQuizzes();
            if (existingQuizzes.empty()) {
                cout << "No existing quizzes found. Please create a quiz first." << endl;
                break;
            }

            cout << "Existing Quizzes:" << endl;
            for (size_t i = 0; i < existingQuizzes.size(); ++i) {
                cout << i + 1 << ". " << existingQuizzes[i] << endl;
            }

            int selectedQuiz;
            cout << "Enter the number of the quiz you want to answer: ";
            cin >> selectedQuiz;

            if (selectedQuiz >= 1 && selectedQuiz <= static_cast<int>(existingQuizzes.size())) {
                string quizFileName = existingQuizzes[selectedQuiz - 1];
                quiz = Quiz(quizFileName);
                quiz.LoadQuizFromFile();
                int numQuestions = quiz.GetNumberOfQuestions();
                quiz.SetNumberOfQuestions(numQuestions);
                quiz.StartQuiz();
                leaderboard.AddEntry(Name, quiz.GetScore());
                leaderboard.SaveLeaderboardToFile();
            }
            else {
                cout << "Invalid quiz selection. Please try again." << endl;
            }

            break;

        case 3:
            leaderboard.ShowLeaderboard();
            break;

        case 0:
            cout << "Exiting the program..." << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }

    } 
}