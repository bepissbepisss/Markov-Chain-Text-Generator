#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

int chooseOutcome(const vector<float>& probs, default_random_engine& generator) {
    if (probs.empty()) {
        cerr << "Error: No probabilities provided.\n";
        return -1;
    }

    vector<float> cumulativeProbs(probs.size());
    cumulativeProbs[0] = probs[0];
    for (size_t i = 1; i < probs.size(); ++i) {
        cumulativeProbs[i] = cumulativeProbs[i - 1] + probs[i];
    }

    uniform_real_distribution<float> distribution(0.0, 1.0);
    float randomNum = distribution(generator);

    for (size_t i = 0; i < cumulativeProbs.size(); ++i) {
        if (randomNum <= cumulativeProbs[i]) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

int main() {
    cout << "How many words do you want to generate? ";
    int wordCount;
    cin >> wordCount;

    cout << "Enter the first two words: ";
    string firstWord, secondWord;
    cin >> firstWord >> secondWord;
    firstWord = trim(firstWord);
    secondWord = trim(secondWord);

    string currentPhrase = firstWord + " " + secondWord;
    cout << firstWord << " " << secondWord << " ";

    // Read column headers
    ifstream headerFile("weights.csv");
    string headerLine;
    getline(headerFile, headerLine);
    headerFile.close();

    vector<string> columnHeaders;
    stringstream ss(headerLine);
    string token;
    getline(ss, token, ','); // skip top-left cell
    while (getline(ss, token, ',')) {
        columnHeaders.push_back(trim(token));
    }

    random_device rd;
    default_random_engine generator(rd());

    for (int step = 0; step < wordCount; ++step) {
        ifstream file("weights.csv");
        string line, rowLabel;
        int foundRow = -1;
        int lineNumber = 0;

        getline(file, line); // skip header
        while (getline(file, line)) {
            stringstream rowStream(line);
            getline(rowStream, rowLabel, ',');
            rowLabel = trim(rowLabel);
            if (rowLabel == currentPhrase) {
                foundRow = lineNumber;
                break;
            }
            lineNumber++;
        }
        file.close();

        // If not found, fallback to just last word
        if (foundRow == -1) {
            currentPhrase = secondWord;
            lineNumber = 0;
            file.open("weights.csv");
            getline(file, line); // skip header
            while (getline(file, line)) {
                stringstream rowStream(line);
                getline(rowStream, rowLabel, ',');
                rowLabel = trim(rowLabel);
                if (rowLabel == currentPhrase) {
                    foundRow = lineNumber;
                    break;
                }
                lineNumber++;
            }
            file.close();
        }

        if (foundRow == -1) {
            cout << "[unknown] ";
            break;
        }

        // Get probabilities from that row
        file.open("weights.csv");
        getline(file, line); // skip header
        for (int i = 0; i < foundRow; ++i) {
            getline(file, line);
        }
        getline(file, line);
        file.close();

        stringstream rowStream(line);
        getline(rowStream, token, ','); // skip row label
        vector<float> probs;
        while (getline(rowStream, token, ',')) {
            token = trim(token);
            probs.push_back(token.empty() ? 0.0f : stof(token));
        }

        int index = chooseOutcome(probs, generator);
        if (index >= 0 && index < columnHeaders.size()) {
            string nextWord = columnHeaders[index];
            cout << nextWord << " ";
            firstWord = secondWord;
            secondWord = nextWord;
            currentPhrase = firstWord + " " + secondWord;
        } else {
            cout << "[none] ";
            break;
        }
    }

    cout << endl;
    return 0;
}
