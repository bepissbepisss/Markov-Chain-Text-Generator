
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <random>
#include <ctime>


using namespace std;

int chooseOutcome(const vector<float>& probs, default_random_engine& generator) 
    {
	vector<float> cumulativeProbs(probs.size());
		cumulativeProbs[0] = probs[0];
		for (int i = 1; i < probs.size(); i++) 
        {
			cumulativeProbs[i] = cumulativeProbs[i - 1] + probs[i];

		}
		uniform_real_distribution<float> distrubution(.0, 1);
		double randomNum = distrubution(generator);

		for (int i = 0; i < cumulativeProbs.size(); i++) 
        {
			if (randomNum <= cumulativeProbs[i]) 
            {
				return i;
			}

		}
		return -1;
    }

int numberOfRows()
{
    ifstream file("weights.csv");
    string s = "";
    int RowCount = 0;
    while (!file.eof())
    {
        getline(file, s);
        RowCount++;
    }
    
    return RowCount;
}

void skipUntilComma(ifstream& input)
{
    char temp='a';
    while(temp != ',')
    {
        input >> temp; 
    }

}
 
void skipLine(ifstream& input, int numberOfRows)
{
    for(int i=0; i<numberOfRows; i++ ) 
    {
        skipUntilComma(input);
    }
}


int main()
{
    
    int i = 0;
    vector<float> vectorProbs;
    vector<string> vectorNames;
    string generatedWord = "";
    string line = "";
    string firstWord = "";
    string probabilities = "";
    string wordName="";
    int location = 0;
    int rowNumber = 0;
    
    ifstream file("weights.csv");
    
   int nbOfRows = numberOfRows();
   
   
    cout << "Enter the first word: ";
    cin >> firstWord;
    
    while (firstWord != generatedWord)
    {
         getline(file, line);
         location = line.find(',');
         generatedWord = line.substr(0, location);
         probabilities = line.substr(location +1, line.length());
         ++i;
         if (i > nbOfRows + 1)
         {
             cout << "Unavailable word";
             return 0; 
         }


    }
    const int loctionOfWord = i;
    file.close();
    for(int k = 0; k < nbOfRows ; k++)
    {
        i = loctionOfWord;
        ifstream weights("weights.csv");
        float probability;
        for(;i > 1 ; i--)
        {
            skipLine(weights,nbOfRows);
        }

        for(int j = 0; j < k; j++)
        {
            skipUntilComma(weights);
        }

        weights >> probability ;
        if(probability != 0)
        {
            
            ifstream weights1("weights.csv");
            for(int l = 1; l < k; l++)
            skipUntilComma(weights1);
            {
                skipUntilComma(weights1);
            }
            weights1 >> wordName;     
            location = wordName.find(',');
            wordName = wordName.substr(0, location);
            vectorProbs.push_back(probability);
            vectorNames.push_back(wordName);
            rowNumber++;
            weights1.close();
            
        }
        
        weights.close();
    }
    random_device rd;
    default_random_engine generator(rd());
    int chosenIndex = chooseOutcome(vectorProbs, generator);
    cout << vectorNames[chosenIndex] << endl << vectorProbs[chosenIndex] << endl;
    firstWord = vectorNames[chosenIndex];
    vectorNames.clear();
    vectorProbs.clear();
    
    
    return 0;
}
