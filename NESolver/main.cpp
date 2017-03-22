
#include <iostream>
#include <fstream>
#include <string>
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"

using namespace std;
using namespace NESolver;

int main()
{
	ifstream fileIn;
	fileIn.open("in.txt");
	string line;
	MAT X(2, 0), Y(1, 0);
	if (fileIn.is_open())
	{
		while (getline(fileIn, line))
		{
			cout << line << '\n';

			istringstream iss(line);
			char strInput[256];

			iss >> strInput;
			REAL in1 = (REAL)atof(strInput);
			iss >> strInput;
			REAL in2 = (REAL)atof(strInput);
			iss >> strInput; // '=' char read
			iss >> strInput;
			REAL out = (REAL)atof(strInput);

			VEC x(2);
			x(0) = in1;
			x(1) = in2;
			VEC y(1);
			y(0) = out;

			X.conservativeResize(X.rows(), X.cols() + 1);
			X.col(X.cols() - 1) = x;

			Y.conservativeResize(Y.rows(), Y.cols() + 1);
			Y.col(Y.cols() - 1) = y;
		}
		fileIn.close();
	}
	MAT YOH = EncodeOneHot(Y, 2);

	//vector<UINT16> hT;
	//hT.push_back(3);

	//NeuralNetwork nn(2, 2, hT);
	//nn.Train(X, YOH, 1000, 0.005f, 0.01f);
	//MAT Y_results;
	//nn.Classify(X, &Y_results);
	//cout << Y_results << endl << endl;

	NNTChromosomeProducer cp(5, 2, 7, 5);
	GeneticAlgorithm ga(cp);
	ga.Initialize();
	ga.SelectCrossAndMutate();
	while (true)
	{
		NNTChromosome c1;
		cp.GenerateChromosome(&c1);
		NNTChromosome c2(c1);
		cp.GenerateChromosome(&c2);

		NNTChromosome c3;
		cp.Crossover(c1, c2, &c3);

		NNTChromosome c4(c2 = c3);

		cp.Mutate(0.5f, &c4);
		cp.ChangeChromosomeSize(&c4);
		c4.CalculateError(X, YOH);
		cout << c4.GetError() << endl << endl;
	}
	return 0;
}