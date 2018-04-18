// EvolutionDataMining2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <sstream>
#include <cassert>
#include <array>


#define generations 500
#define Psize 150
#define MutationRate 0.015
#define MutationStep 0.2
#define CrossoverRate 0.9
#define ruleNo 5
#define rows 2000  
#define testrows 667
#define trainrows 1333
#define var 13
#define inp 6
#define N (((inp*2)+1)*ruleNo)  

#define outputfile "GAdataset3-rprt5.csv"

using namespace std;


/////////////////////////////////////////////////////////////////////////////Creating the individuals' structures//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	float gene[N];
	int fitness;
}individual;       //creating a datatype called individual with associated genes and a fitness number(that shows how good those gense are)

/////////////////////////////////////////////////////////////////////////////Creating rule structures////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	float cond[inp];
	int output;
}ruletype;

typedef struct {
	float cond[inp*2];
	int output;
}ruletype1;

/////////////////////////////////////////////////////////////////////////////Declaring the global varaibles////////////////////////////////////////////////////////////////////////////////////////////
ruletype1 rules[ruleNo];
individual population[Psize];   // creating an array of datatype individual called "population" with a size
individual showdog;
ruletype thedata[rows];
ruletype datatrain[trainrows];
ruletype datatest[testrows];
vector <float> mydata1; 

//vector <float> trainingdata;
//vector <float> testdata;

//////////////////////////////////////////////////////////////////////the fitness evaluation function////////////////////////////////////////////////////////////////////////////////////////////////////.
int fitnessEval(individual f, ruletype g[], int r) //takes in an individual, data sorted into rules and num of data rows as arg... returns individualfitness
{
	int  i, j, k, n, s, t,l;
	float temp;
	i = 0;
	n = 0;
	l = 0;

	//function takes in an individual and then breaks it down into a set number of rules
	for (k = 0; k < ruleNo; k++)
	{
		for (j = 0; j < (inp*2); j++)
		{
			rules[k].cond[j] = f.gene[i++];
			//if (f.gene[j] == 1)
			//f.fitness++;
		}
		rules[k].output = f.gene[i++];  
	}
	////rules are then sorted so number on the left is the smaller number and number on right is greater a < x < b
	for (k = 0; k < ruleNo; k++)
	{
		for (j = 0; j < (inp * 2); j++)
		{
			if (rules[k].cond[j] > rules[k].cond[j + 1])
			{
				temp = rules[k].cond[j];
				rules[k].cond[j] = rules[k].cond[j + 1];
				rules[k].cond[j + 1] = temp;
			}
			j = j + 1;
		}
	}
	//putting the data from file into a structure for ease of use

	/*for (k = 0; k < r; k++)
	{
		for (j = 0; j < inp; j++)
		{
			thedata[k].cond[j] = d[n++];
		}
		thedata[k].output = d[n++];

	}*/

	//Matching fucntion.....comparing data from file with my rules
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < ruleNo; j++)
		{
			s = 0;
			l = 0;
			for (t = 0; t < inp; t++)
			{
				if ((g[k].cond[t] > rules[j].cond[l]) && (g[k].cond[t] < rules[j].cond[l+1]))
				{
					s++; //counts up if conditions have matched
				}
				l= l + 2;
			}

			if (s == inp) //if my counter is its max value then check the output
			{
				if (rules[j].output == g[k].output)
				{
					f.fitness++;  //increment fitness when rule condition and output match
								  //cout << j << "\n";	
				}
				j = ruleNo;  //sets j to the end of the for loop when we find the first rule that matches.
			}

		}
	}

	return f.fitness;
}

int main()
{

	int performance = 0;
	int i, j, s, popfitness;
	srand(time(NULL));             //seeding the rand function with a distinctive runtime value which is diffrenet each time
								   //without srand(time) rand would generate the same sequence of numbers because the seed is the same	
								   // creating random intial solution

								   //opening the file and putting it into an integer array

/////////////////////////////////////////////////////////////////////////////Opening the input file as string////////////////////////////////////////////////////////////////////////////////////////////////
	string data1, dummyline;
	ifstream takedata;
	takedata.open("data3.txt");

	if (takedata.is_open())
	{
		getline(takedata, dummyline);  //getting rid of the first line
		getline(takedata, data1, '\0');
	}
	else {
		// show message:
		cout << "Error opening input file" << "\n";
		system("pause");
	}

	//////////////get rid of new line and replace with space instead
	for (int i = 0; i < data1.length(); i++)
	{
		if (data1[i] == '\n')
		{
			data1[i] == ' ';

		}
	}
/////////////////////////////////////////////////////////////////////////////Convert String into Float Array//////////////////////////////////////////////////////////////////////////////////////////////////

///turning the string into an array of floats
	istringstream ss(data1);
	copy(
		istream_iterator <float>(ss),
		istream_iterator <float>(),
		back_inserter(mydata1)
		);

////////////////////////////////////////////////////////////////////////Spliting the Data up into Training and Testing/////////////////////////////////////////////////////////////////////////////////////////////////
	int ones, zeros, k, m, n ;
	ones = 0;
	zeros = 0;
	m = 0;
	n = 0;
	
	size_t const split =( (mydata1.size() *2)/ 3)-2;         ///Split two thirds/ one third
	//size_t const split = ((mydata1.size()*9)/ 10);     //Split 90/10
	vector<float> trainingdata (mydata1.begin(), mydata1.begin() + split);  
	vector <float> testdata(mydata1.begin() + split, mydata1.end());

	//for (int i = 0; i < trainingdata.size(); i++)
	//{
	//	if (trainingdata[i] == 1)
	//		ones++; //value is 965
	//	if (trainingdata[i] == 0)
	//		zeros++; //value is 1035
	//}

	/////////////////////////splitting training data/////////////////////////
	for (k = 0; k < trainrows; k++)
	{
		for (j = 0; j < inp; j++)
		{
			datatrain[k].cond[j] = trainingdata[m++];
		}
		datatrain[k].output = trainingdata[m++];

	}
	/////////////////////////splitting test data/////////////////////////
	for (k = 0; k < testrows; k++)
	{
		for (j = 0; j < inp; j++)
		{
			datatest[k].cond[j] = testdata[n++];
		}
		datatest[k].output = testdata[n++];

	}



///////////////////////////////////////////////////////////////////////////////////Populating the Problem Space/////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////populating my problem space with multiple indviduals with a fitenss of 0
	for (i = 0; i < Psize; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (((j + 1) % var) == 0)  //it can only can only equal 0 when j divides var without a remainder which is every var*n(th) bit. These are the output bits which need to be either 1 or 0 
				population[i].gene[j] = rand() % 2;	 //when on var*n bits gene can be either 1 or 0
			else
				population[i].gene[j] = (float)(rand() / (float)RAND_MAX); //when on other bits it can be a float between 0 and 1
		}
		population[i].fitness = 0;
	}

/////////////////////////////////////////////////////////////////////////////////// Fitness Evaluation (Training DATA) ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (i = 0; i < Psize; i++)
	{
			population[i].fitness = fitnessEval(population[i], datatrain, trainrows);
		
	}

/////////////////////////////////////////////////////////////////////////////////// Opening Output File for Writing //////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//opening the output file that will be filled with data
	ofstream myfile;

	myfile.open(outputfile); //generating a comma sepearated results file
	if (myfile.is_open())
	{
		
		myfile << "Best Individual,";
		myfile << "Mean,";
		myfile << "TestData Fitness" << "\n";
	}
	else {
		// show message:
		cout << "Error opening output file" << "\n";
		system("pause");
	}



/////////////////////////////////////////////////////////////////////////////Genetic Algorithm Loop//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (s = 0; s < generations; s++)  // (Termination condition could be reaching a specific mean  fitness etc)
	{
		
		
		
		individual kids[Psize];
		individual bestparents[Psize];
		int parent1, parent2, parent3, parent4, winner1, winner2;
		popfitness = 0;

		if ((s % 10) == 0) //every 50th generation, Check the best individual against the test data.
		{
			
			showdog.fitness = 0;
			performance =fitnessEval(showdog, datatest, testrows);
			cout << "TEST DATA PREDICTION IS                                      " << performance << "\n";
			//system("pause");
			//myfile << "yo,"  << "\n";
		}

///////////////////////////////////////////////////////////////////////////////////////Selection///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		for (i = 0; i < Psize; i++)
		{
			parent1 = rand() % Psize;
			parent2 = rand() % Psize;
			parent3 = rand() % Psize;
			parent4 = rand() % Psize;

			if ((population[parent1].fitness >= population[parent2].fitness) && (population[parent1].fitness >= population[parent3].fitness) && (population[parent1].fitness >= population[parent4].fitness))
				bestparents[i] = population[parent1];
			else if ((population[parent2].fitness >= population[parent3].fitness) && (population[parent2].fitness >= population[parent4].fitness))
				bestparents[i] = population[parent2];
			else if (population[parent3].fitness >= population[parent4].fitness)
				bestparents[i] = population[parent3];
			else
				bestparents[i] = population[parent4];

			//if (population[parent1].fitness >= population[parent2].fitness)
			//{
			//	if (population[parent1].fitness >= population[parent3].fitness)
			//	bestparents[i] = population[parent1];
			//	else																	//<---three man tourni
			//	bestparents[i] = population[parent3];
			//}
			//else
			//{
			//	if (population[parent2].fitness >= population[parent3].fitness)
			//	bestparents[i] = population[parent2];
			//	else
			//	bestparents[i] = population[parent3];
			//}




			//if (population[parent1].fitness >= population[parent2].fitness)
			//	bestparents[i] = population[parent1];                              ///two man tourni
			//else
			//	bestparents[i] = population[parent2]
		}

//////////////////////////////////////////////////////////////////////////////////////Recombination////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		for (i = 0; i < Psize; i += 2)
		{
			float temp[N];
			float temp2[N];
			float crossrate;
			crossrate = (float)(rand() / (float)RAND_MAX);

			if (crossrate < CrossoverRate)
			{

				int crosspoint = rand() % N;
				for (j = 0; j < N; j++)
				{
					if (j < crosspoint) {
						temp[j] = bestparents[i].gene[j];
						temp2[j] = bestparents[i + 1].gene[j];
					}
					else
					{
						temp2[j] = bestparents[i].gene[j];
						temp[j] = bestparents[i + 1].gene[j];
					}
				}
				for (j = 0; j < N; j++)
				{
					kids[i].gene[j] = temp[j];
					kids[i + 1].gene[j] = temp2[j];
				}
			}
			else
			{
				for (j = 0; j < N; j++)
				{
					kids[i].gene[j] = bestparents[i].gene[j];
					kids[i + 1].gene[j] = bestparents[i + 1].gene[j];
				}
			}

		}
/////////////////////////////////////////////////////////////////////////////////////////////Mutation/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (i = 0; i < Psize; i++)
		{
			float mutate;
			float stepsize;
			for (j = 0; j < N; j++)
			{
				mutate = (float)(rand() / (float)RAND_MAX);
				stepsize = (float) (rand()) / ((float) (RAND_MAX / MutationStep)); //generate a number from 0.0 to MutationStep

				if (rand() % 2) //evaluates as true or false 1 or 0
					stepsize = stepsize;
				else
					stepsize = stepsize * (-1);// if randomly false then make stepsize negative

				//cout << stepsize << "\n"  ;

				if (mutate < MutationRate)
				{
					//cout << mutate << "\n";
					//system("pause");
					if (((j + 1) % var) == 0)
					{
						if (kids[i].gene[j] == 0)
							kids[i].gene[j] = 1;
						else
							kids[i].gene[j] = 0;
					}
					else
					{
						kids[i].gene[j] = kids[i].gene[j] + stepsize;  //add stepsize to randomly change the gene in either direction
						if (kids[i].gene[j] > 1)
							kids[i].gene[j] = 1;
						if (kids[i].gene[j] < 0)
							kids[i].gene[j] = 0;
					}
				}
			}

		}


///////////////////////////////////////////////////////////////////////////Elitism (Holding on to the best Individual)////////////////////////////////////////////////////////////////////////////////////////////////////////
		int best;
		int mostfit;
		individual topdog; //best individual in this generation
		best = 0;

		for (i = 0; i < Psize; i++)
		{
			if (population[i].fitness > best)    //before we turn kids into parents we to find the most fit in the population and save it
			{
				best = population[i].fitness;
			}
			if (population[i].fitness == best)
				topdog = population[i];

		}

/////////////////////////////////////////////////////////////////////////////////////Reassignment//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (i = 0; i < Psize; i++)
		{
			population[i] = kids[i];
			population[i].fitness = 0;

		}
		//loop evaluation of fitness for kids (new generation of population)
		int bestkid, worstkid;
		bestkid = 0;
		worstkid = rows; //start iterating downwards from best possible fitness value(Not very Effecient)

/////////////////////////////////////////////////////////////////////////////////// Fitness Evaluation - Training Data (Loop) ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (i = 0; i < Psize; i++)
		{
			population[i].fitness = fitnessEval(population[i], datatrain, trainrows);
		}



///////////////////////////////////////////////////////////////////Discarding Worst Individual and Replacing with Best from Previous Generation (Elitism)///////////////////////////////////////////////////////////////////
		for (i = 0; i < Psize; i++)
		{
			if (population[i].fitness < worstkid)
				worstkid = population[i].fitness;

		}
		//and replacing with best of old gen
		for (i = 0; i < Psize; i++)
		{
			if (population[i].fitness == worstkid)
			{
				population[i] = topdog;
				i = Psize;
				//break;
			}
		}

/////////Finding best individual in this generation for printing
		for (i = 0; i < Psize; i++)
		{

			if (population[i].fitness > bestkid)    //comparing each evaluated individual to find the most fit in the population
			{
				bestkid = population[i].fitness;
			}
			if (population[i].fitness == bestkid)
			{
				showdog = population[i];
				//break;
			}
		}
///////////////////////////////////////////////////////////////////////Calculating the total fitness of the population////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (i = 0; i < Psize; i++)
		{
			popfitness += population[i].fitness;  //adding up all fitnesses to calculate total population fitness
		}


//////////////////////////////////////////////////////////////////////////Writing To Console for Debugging////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//for (j = 0; j <N; j++)
		//{
		////////printf(" the fitness for ind is %d\n " ,   population[i].fitness);
		//cout << " the genes for best individual " << " is " << (float)showdog.gene[j] << "\n";
		//}

		cout << " s is                                 " << s << "\n";
		cout << "total population fitness is " << popfitness << "\n";
		cout << "the best fitness in this population is       " << bestkid << "\n";
		cout << "showdog's fitness is " << showdog.fitness << "\n";
		cout << "mean population fitness is " << (float)popfitness / (float)Psize << "\n";


/////////////////////////////////////////////////////////////////////////////////Writing Best and Mean Fitness's to File//////////////////////////////////////////////////////////////////////////////////////////////////////////

		myfile << bestkid << ",";
		myfile << (float)popfitness / (float)Psize << ",";
		if(performance==0)
			myfile << " " << "\n";
		else
			myfile << performance << "\n";

		//system("pause");
	}


/////////Writing best individual's genes to the  file///////////////////////////
	myfile << ",,,,Best individuals genes" << "\n";
	int itr = 0;
	for (j = 0; j < ruleNo; j++)
	{
		myfile << "," << "," << ",";
		for (i = 0; i < var; i++)
		{
			myfile << showdog.gene[itr++];
			myfile << ",";
		}
		myfile << "\n";
	}
	
	myfile.close();
	return 0;
}
