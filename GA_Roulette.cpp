#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define crossoverProbability 0.8
#define mutationProbability 0.1

double countDistance(int array[], int number);
void copy(int finalData[], int startData[], int much);
void order_crossover(int order_crossover_first[], int order_crossover_second[], int much);
void tempData(int array[], int randon1, int randon2);
void mutation(int array[], int much);
int data[51][3] = {0};
int crossoverCount = 1;
int mutationCount = 1;

int main()
{
	clock_t startclock, endclock;
	startclock = clock();
	FILE *fptr;
	char num[2] = {}, x[2] = {}, y[2] = {}, empty[] = "EOF";
	int chromosome = 100, count = 0, chose, co = 1, i, j, h;
	double bestDistance = 0, totalDistance = 0, tempDistance = 0;

	//	printf("How many Chromosome = ");
	//	scanf("%d",&chromosome);

	fptr = fopen("eil51-data.txt", "r");
	while (fscanf(fptr, "%s %s %s", &num, &x, &y) != EOF)
	{
		if (strcmp(num, empty) != 0)
		{
			data[count][0] = atoi(num);
			data[count][1] = atoi(x);
			data[count][2] = atoi(y);
			count++;
		}
		else
			break;
	}
	fclose(fptr);

	int initialChromosomeData[chromosome][count], chromosomeData[chromosome][count];
	double chromosome_CountDistance[chromosome], dividingLine[(chromosome - 1)];

	srand(time(NULL));
	for (i = 0; i < chromosome; i++) //��l�ƬV����
	{
		int Temp[count][3] = {0};
		for (j = 0; j < count; j++)
		{
			for (h = 0; h < 3; h++)
				Temp[j][h] = data[j][h];
		}
		for (j = 0; j < count; j++)
		{
			chose = rand() % count;
			while (Temp[chose][0] == -9999)
				chose = rand() % 51;
			initialChromosomeData[i][j] = Temp[chose][0];
			Temp[chose][0] = -9999;
		}
	}
	for (i = 0; i < chromosome; i++)
	{
		for (j = 0; j < count; j++)
			chromosomeData[i][j] = initialChromosomeData[i][j];
	}
	bestDistance = countDistance(chromosomeData[0], count); //�w�]�Ĥ@���V���鬰�̨ζZ��

	while (co != 50001)
	{
		double change[chromosome] = {0};

		//�p�����
		for (i = 0; i < chromosome; i++)
			totalDistance += 1 / chromosome_CountDistance[i];
		//�p��C�Ӱ϶���νu
		for (i = 0; i < chromosome; i++)
		{
			dividingLine[i] = (1 / chromosome_CountDistance[i] + tempDistance) / totalDistance;
			tempDistance += 1 / chromosome_CountDistance[i];
		}
		for (i = 0; i < chromosome; i++) //�H������0-1���ü�
			change[i] = (double)rand() / RAND_MAX;
		for (i = 0; i < chromosome; i++)
		{
			for (j = (i + 1); j < chromosome; j++)
			{
				while (change[i] == change[j]) //�����ӬۦP
					change[j] = (double)rand() / RAND_MAX;
			}
		}
		for (i = 0; i < chromosome; i++) //Selection
		{
			for (j = 0; j < chromosome; j++)
			{
				if (change[i] <= dividingLine[j]) //���u���e��
				{
					copy(chromosomeData[i], initialChromosomeData[j], count);
					break;
				}
			}
		}

		//CrossoverProbability
		double crossover_probability[chromosome / 2];
		for (i = 0; i < chromosome / 2; i = i + 2)
		{
			crossover_probability[i] = (double)rand() / RAND_MAX;
			for (j = 0; j < chromosome / 2; j++)
			{
				if (crossover_probability[j] > crossoverProbability)
				{
					int r1 = rand() % chromosome, r2 = rand() % chromosome;
					while (r1 == r2)
						r1 = rand() % chromosome;
					order_crossover(chromosomeData[r1], chromosomeData[r2], count);
				}
			}
		}

		//MutationProbability
		double mutation_probability[chromosome];
		for (i = 0; i < chromosome; i++)
		{
			mutation_probability[i] = (double)rand() / RAND_MAX;
			if (mutation_probability[i] > mutationProbability)
				mutation(chromosomeData[i], count);
		}

		//�p��C���ѶZ��
		for (i = 0; i < chromosome; i++)
			chromosome_CountDistance[i] = countDistance(chromosomeData[i], count);
		for (i = 0; i < chromosome; i++)
		{
			if (bestDistance > chromosome_CountDistance[i])
				bestDistance = chromosome_CountDistance[i];
		}

		//�n���V����O�s�U��
		for (i = 0; i < chromosome; i++)
		{
			for (j = 0; j < count; j++)
				initialChromosomeData[i][j] = chromosomeData[i][j];
		}
		if (co % 10 == 0)
			printf("%f\n", bestDistance);
		co++;
	}
	endclock = clock();
	//    printf("%1f\n",(endclock-startclock)/(double)(CLOCKS_PER_SEC));//total time
}

double countDistance(int array[], int number)
{
	int i, j, countDistance_tempdata[number][2] = {0};
	double toCountDistance = 0;
	for (i = 0; i < number; i++)
	{
		for (j = 0; j < number; j++)
		{
			if (array[i] == data[j][0])
			{
				countDistance_tempdata[i][0] = data[j][1]; //x
				countDistance_tempdata[i][1] = data[j][2]; //y
			}
		}
	}
	for (j = 0; j < number; j++)
	{
		int temp;
		if (j != (number - 1))
		{
			temp = j + 1;
			toCountDistance += sqrt(pow((countDistance_tempdata[j][0] - countDistance_tempdata[temp][0]), 2) + pow((countDistance_tempdata[j][1] - countDistance_tempdata[temp][1]), 2));
		}
		else
		{
			temp = 0;
			toCountDistance += sqrt(pow((countDistance_tempdata[j][0] - countDistance_tempdata[temp][0]), 2) + pow((countDistance_tempdata[j][1] - countDistance_tempdata[temp][1]), 2));
		}
	}
	return toCountDistance;
}

void copy(int finalData[], int startData[], int much)
{
	int ci, cj;
	for (ci = 0; ci < much; ci++)
	{
		finalData[ci] = startData[ci];
	}
}

void order_crossover(int order_crossover_first[], int order_crossover_second[], int much)
{
	int first[much] = {0}, second[much] = {0}, r1, r2, tempRand, cri, crj, crossoverTemp1[much][2] = {9999}, crossoverTemp2[much][2] = {9999}, crossoverCount = 0, temp = 0;
	double order_crossover_countDistance_start[2] = {0}, order_crossover_countDistance_after[2] = {0};

	copy(first, order_crossover_first, much);
	copy(second, order_crossover_second, much);
	order_crossover_countDistance_start[0] = countDistance(order_crossover_first, much);
	order_crossover_countDistance_start[1] = countDistance(order_crossover_second, much);
	r1 = rand() % much;
	r2 = rand() % much;
	while (r1 == r2)
		r1 = rand() % much;
	if (r1 > r2) //�p��j
	{
		tempRand = r1;
		r1 = r2;
		r2 = tempRand;
	}

	//���ά��e��
	for (cri = 0; cri < r1; cri++)
	{
		crossoverTemp1[cri][0] = first[cri];
		crossoverTemp2[cri][0] = second[cri];
		crossoverCount++;
	}
	for (cri = (r2 + 1); cri < much; cri++)
	{
		crossoverTemp1[crossoverCount][0] = first[cri];
		crossoverTemp2[crossoverCount][0] = second[cri];
		crossoverCount++;
	}

	//��X����
	for (cri = 0; cri < crossoverCount; cri++)
	{
		for (crj = 0; crj < much; crj++)
		{
			if (crossoverTemp1[cri][0] == second[crj])
				crossoverTemp1[cri][1] = crj;
			if (crossoverTemp2[cri][0] == first[crj])
				crossoverTemp2[cri][1] = crj;
		}
	}

	//�Ѥp��j�Ƨ�
	for (cri = 0; cri < crossoverCount; cri++)
	{
		for (crj = (cri + 1); crj < crossoverCount; crj++)
		{
			int tempNumber, tempAddress;
			if (crossoverTemp1[cri][1] > crossoverTemp1[crj][1])
			{
				tempNumber = crossoverTemp1[cri][0];
				crossoverTemp1[cri][0] = crossoverTemp1[crj][0];
				crossoverTemp1[crj][0] = tempNumber;
				tempAddress = crossoverTemp1[cri][1];
				crossoverTemp1[cri][1] = crossoverTemp1[crj][1];
				crossoverTemp1[crj][1] = tempAddress;
			}
			if (crossoverTemp2[cri][1] > crossoverTemp2[crj][1])
			{
				tempNumber = crossoverTemp2[cri][0];
				crossoverTemp2[cri][0] = crossoverTemp2[crj][0];
				crossoverTemp2[crj][0] = tempNumber;
				tempAddress = crossoverTemp2[cri][1];
				crossoverTemp2[cri][1] = crossoverTemp2[crj][1];
				crossoverTemp2[crj][1] = tempAddress;
			}
		}
	}

	//��^
	for (cri = 0; cri < r1; cri++)
	{
		first[cri] = crossoverTemp1[cri][0];
		second[cri] = crossoverTemp2[cri][0];
		temp++;
	}
	for (cri = (r2 + 1); cri < much; cri++)
	{
		first[cri] = crossoverTemp1[temp][0];
		second[cri] = crossoverTemp2[temp][0];
		temp++;
	}

	order_crossover_countDistance_after[0] = countDistance(first, much);
	order_crossover_countDistance_after[1] = countDistance(second, much);
	//�Y����n�h���N
	if (order_crossover_countDistance_after[0] < order_crossover_countDistance_start[0])
		copy(order_crossover_first, first, much);
	else if (crossoverCount % 5 == 0) //�����|���@��crossover
		copy(order_crossover_first, first, much);
	if (order_crossover_countDistance_after[1] < order_crossover_countDistance_start[1])
		copy(order_crossover_second, second, much);
	else if (crossoverCount % 5 == 0)
		copy(order_crossover_first, first, much);
	crossoverCount++;
}

void tempData(int array[], int randon1, int randon2)
{
	int temp;

	temp = array[randon1];
	array[randon1] = array[randon2];
	array[randon2] = temp;
}

void mutation(int array[], int much)
{
	int mutation_r1 = rand() % much, mutation_r2 = rand() % much, mutation_temp, mutation_i;
	double mutation_countDistance_start = 0, mutation_countDistance_after = 0;

	mutation_countDistance_start = countDistance(array, much);
	while (mutation_r1 == mutation_r2)
		mutation_r1 = rand() % much;
	tempData(array, mutation_r1, mutation_r2);
	mutation_countDistance_after = countDistance(array, much);
	if (mutation_countDistance_after > mutation_countDistance_start) //�Y�L����n�h���洫
		if (mutationCount % 5 != 0)
			tempData(array, mutation_r1, mutation_r2);
	mutationCount++;
}
