#include <iostream>
#include <vector>
#include <random>
#include <ctime>
using namespace std;
int chooseOutcome(const vector<double>& probs, default_random_engine& generator) {
	vector<double> cumulativeProbs(probs.size());
		cumulativeProbs[0] = probs[0];
		for (int i = 1; i < probs.size(); i++) {
			cumulativeProbs[i] = cumulativeProbs[i - 1] + probs[i];

		}
		uniform_real_distribution<double> distrubution(.0, 1);
		double randomNum = distrubution(generator);

		for (int i = 0; i < cumulativeProbs.size(); i++) {
			if (randomNum <= cumulativeProbs[i]) {
				return i;
			}

		}
		return -1;

}


int main()
{
	vector <double> wordP = { .999,.001 };

	random_device rd;
	default_random_engine generator(rd());


	int chosenIndex = chooseOutcome(wordP, generator);

	cout << "The index is " << chosenIndex;


}