#include <stdio.h>
#include <time.h>
#include <math.h>
#include <float.h>

#include "ast.h"

#define GENERATION_SIZE 100

#define TRIALS_COUNT 1000
#define CROSSOVERS_COUNT 5

/* Function to approximate */
double func(double x)
{
	return 2 * x + sin(x);
}

/* Get the error of an expression */
double get_error(Node* root, double* var)
{
	double error = 0.0;

	for(*var = -100; *var <= 100; *var += 1.0)
	{
		double value = node_parse(root);

		error += (value - func(*var)) * (value - func(*var));
	}

	return error;
}

int main()
{
	srand(time(NULL));

	/* A pointer to this variable will be stored in all trees */
	double var = 0;

	Node* generation[GENERATION_SIZE];
	for(int i = 0; i < GENERATION_SIZE; ++i)
	{
		generation[i] = node_binop_create(get_binop_random());
		generation[i]->left = node_base_create_random(&var);
		generation[i]->right = node_base_create_random(&var);
	}

	for(int trials = 0; trials < TRIALS_COUNT; ++trials)
	{
		/* Crossovering */

		for(int i = 0; i < CROSSOVERS_COUNT; ++i)
		{
			int first = rand() % GENERATION_SIZE;	
			int second = rand() % GENERATION_SIZE;	

			if(first == second)
			{
				continue;  /* don't fuck yourself! */
			}

			node_crossover(&generation[first], &generation[second]);
		}

		
		/* Tournament selection */	

		int winners[GENERATION_SIZE] = {0}; 
		for(int i = 0; i < GENERATION_SIZE; ++i)
		{
			int first_best = rand() % GENERATION_SIZE;
			int sec_best   = rand() % GENERATION_SIZE;

			if(first_best == sec_best)
			{
				continue;
			}

			if(get_error(generation[first_best], &var) < get_error(generation[sec_best], &var))
			{
				winners[first_best] = 1;
				winners[sec_best] = 0;
			} else {
				winners[sec_best] = 1;
				winners[first_best] = 0;
			}
		}

		/* Leave only the best individuals. Randomly generate new ones */

		for(int i = 0; i < GENERATION_SIZE; ++i)
		{
			if(winners[i] == 0)
			{
				node_free(generation[i]);

				generation[i] = node_binop_create(get_binop_random());
				generation[i]->left = node_base_create_random(&var);
				generation[i]->right = node_base_create_random(&var);
			}
		}
	}

	long double error_avrg = 0.0;
	double error_min = DBL_MAX;

	for(int i = 0; i < GENERATION_SIZE; ++i)
	{
		double error = get_error(generation[i], &var);
		if(error_min > error)
		{
			error_min = error;
		}

		error_avrg += error;

		printf("Entity No. %d, error %f\n", i, error);
		node_print(generation[i]);
		puts("\n");

		node_free(generation[i]);
	}

	printf("\nAverage error of a population: %Lf\n", error_avrg / GENERATION_SIZE);
	printf("Least error of a population: %f\n", error_min);

	return 0;
}

