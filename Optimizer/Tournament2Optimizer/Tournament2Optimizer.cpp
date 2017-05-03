//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/ahnt/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/ahnt/MABE/wiki/License

#include "Tournament2Optimizer.h"

using namespace std;

shared_ptr<ParameterLink<string>> Tournament2Optimizer::optimizeFormulaPL = Parameters::register_parameter("OPTIMIZER_TOURNAMENT2-optimizeValue", (string) "DM[score]", "value to optimize");
shared_ptr<ParameterLink<string>> Tournament2Optimizer::surviveFormulaPL = Parameters::register_parameter("OPTIMIZER_TOURNAMENT2-surviveFormula", (string) "CONST[0]", "chance that an organism will survive");
shared_ptr<ParameterLink<int>> Tournament2Optimizer::elitismPL = Parameters::register_parameter("OPTIMIZER_TOURNAMENT2-elitism", 0, "The highest scoring organism will be included in the next generation this many times (0 = no elitism)?");
shared_ptr<ParameterLink<int>> Tournament2Optimizer::tournamentSizePL = Parameters::register_parameter("OPTIMIZER_TOURNAMENT2-tournamentSize", 5, "number of organisms considered in each tournament?");
shared_ptr<ParameterLink<double>> Tournament2Optimizer::selfRatePL = Parameters::register_parameter("OPTIMIZER_TOURNAMENT2-selfRate", 0.0, "prabablity that organsim will not choose mate, and rather will cross with self");

void Tournament2Optimizer::makeNextGeneration(vector<shared_ptr<Organism>> &population) {
	vector<shared_ptr<Organism>> nextPopulation;
	set<shared_ptr<Organism>> survivors;
	int p1, p2;  // parent 1 and 2
	int challanger;  // used when picking best of
	double surviveChance = 0;
	bool orgSurvived = 0;
	int selfCount = 0;
	int otherCount = 0;
	int surviveCount = 0;

	vector<double> Scores;
	double aveScore = 0;
	vector<double> surviveRates;

	for (auto org : population) {
		Scores.push_back(optimizeFormula->eval(org->dataMap, PT)[0]);

		surviveRates.push_back(surviveFormula->eval(org->dataMap, PT)[0]);
		aveScore += Scores.back();
	}

	aveScore /= population.size();

	int best = findGreatestInVector(Scores);
	double maxScore = Scores[best];

//	{  // check to make sure there are at least 2 genders.
//		bool same = true;
//		for (int index = 0; index < (int) population.size() - 2; index++) {
//			if (population[index]->gender != population[index + 1]->gender) {
//				same = false;
//			}
//		}
//		if (same == true) {  // if there are not atleast 2 genders, make population[0] have a diffrent gender
//			(population[0]->gender == 1) ? population[0]->gender = 0 : population[0]->gender = 1;
//		}
//	}

	while (nextPopulation.size() < population.size()) {  // while we have not filled up the next generation
		//cout << nextPopulation.size() << " < " << population.size() << endl;
		// chance for each pick that this org survives to the next population
		//cout << "picking p1..." << flush;
		if ((int) nextPopulation.size() < elitismLPL->lookup()) {  // if next population has less members then elitism, then p1 is best.
			//cout << "E" << endl;
			p1 = best;
		} else {  // otherwise, p1 is the best of tournamentSize random picks
			//cout << "R" << endl;
			p1 = Random::getIndex(population.size());
			for (int i = 0; i < tournamentSizeLPL->lookup() - 1; i++) {
				challanger = Random::getIndex(population.size());
				if (Scores[challanger] > Scores[p1]) {
					p1 = challanger;
				}
			}
		}
		//cout << p1 << flush;
		//cout << "  picking p2..." << flush;

		orgSurvived = 0;  // clear orgSurvived. if population[p1] survives this will become 1 and we will not pick a mate
		if (Random::P(surviveRates[p1]) && survivors.find(population[p1]) == survivors.end()) {  // if this org survives and has not already survived
			surviveCount++;
			nextPopulation.push_back(population[p1]);  // push them to the next population
			orgSurvived = 1;
			survivors.insert(population[p1]);
		}
		
		if (!orgSurvived) {
			p2 = p1;  // make these the same to prime the while loop
			//while ((p1 == p2) || (population[p1]->gender == population[p2]->gender)) {  // keep picking until you have 2 diffrent parents with 2 diffrent genders
			if ((int)nextPopulation.size() >= elitismLPL->lookup()) {  // if next population has less members then elitism, then p1 is best.
				//cout << "R" << endl;
				if (Random::P(selfRateLPL->lookup())) {
					// self!
					selfCount++;
					p2 = p1;
				}
				else { // mate wit other
					otherCount++;
					while ((p1 == p2)) {  // keep picking until you have 2 diffrent parents with 2 diffrent genders
						//cout << p2 << " " << p1 << endl;
						p2 = Random::getIndex(population.size());
						//cout << "  " << p2 << " " << p1 << endl;

						for (int i = 0; i < tournamentSizeLPL->lookup() - 1; i++) {
							challanger = Random::getIndex(population.size());
							while ((p1 == challanger)) {  // if we pick p1, pick again (it is possible that p2 was select from less then tournament size, it the same challenger/p2 is picked twice
								challanger = Random::getIndex(population.size());
							}
							//cout << "  ch = " << challanger << "\n";
							if (Scores[challanger] > Scores[p2]) {
								//cout << "p2 assigned" << endl;
								p2 = challanger;
							}
						}
					}
				}
			}
			//cout << "\n " << nextPopulation.size() << " " << population.size() << endl;
			nextPopulation.push_back(population[p1]->makeMutatedOffspringFromMany( { population[p1], population[p2] }));
			//cout << p1 << " " << p2 << endl;
			//nextPopulation.push_back(population[p1]->makeMutatedOffspring(population[p1]));
			//cout << " " << nextPopulation.size() << " " << population.size() << endl;
		}

	}
	for (auto org : population) {
		if (!survivors.count(org)) {  // if this org did not survive to the next generation, it must be killed (sad.)
			org->kill();
		}
	}
	population = nextPopulation;

	cout << "max = " << to_string(maxScore) << "   ave = " << to_string(aveScore) << "  survive,other,self: " << surviveCount << "," << otherCount << "," << selfCount << endl;
}

