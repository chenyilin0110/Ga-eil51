#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define crossoverProbability 0.1
#define mutationProbability 0.1

double countDistance(int array[],int number);
void copy(int finalData[],int startData[],int much);
void order_crossover(int order_crossover_first[],int order_crossover_second[],int much);
void tempData(int array[],int randon1,int randon2);
void mutation(int array[],int much);
int data[51][3] = {0};
int crossoverCount = 1;
int mutationCount = 1;

int main()
{
	clock_t startclock,endclock;
	startclock = clock();
	FILE *fptr;
	char num[2]={},x[2]={},y[2]={},empty[] = "EOF";
	int chromosome=100,tournament=25,count = 0,chose,co = 1,i,j,h;
	double bestDistance = 0;
	
//	printf("How many Chromosome = ");
//	scanf("%d",&chromosome);
//	printf("How many Tournament player = ");
//	scanf("%d",&tournament);

	fptr = fopen("eil51-data.txt","r");
	while(fscanf(fptr,"%s %s %s",&num,&x,&y) != EOF)
	{
		if(strcmp(num,empty) != 0)
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
	
	int initialChromosomeData[chromosome][count],chromosomeData[chromosome][count],tournamentData[tournament][count];
	double chromosome_CountDistance[chromosome],dividingLine[(chromosome-1)];
	
	srand(time(NULL));
	for(i=0;i<chromosome;i++)//初始化染色體 
	{
		int Temp[count] = {0};
		for(j=0;j<count;j++) 
			Temp[j] = data[j][0];
		for(j=0;j<count;j++)
		{
			chose = rand()%count;
			while(Temp[chose] == -9999)
				chose = rand()%51;
			initialChromosomeData[i][j]= Temp[chose];
			Temp[chose] = -9999;
		}
	}
	for(i=0;i<chromosome;i++)
	{
		for(j=0;j<count;j++)
			chromosomeData[i][j] = initialChromosomeData[i][j];
	}
	bestDistance = countDistance(chromosomeData[0],count);//預設第一條染色體為最佳距離 

	while(co != 50001)
	{
		//tournament
		int tournament_rand_data[tournament]={0};
		double tournament_countDistance[tournament]={0};
		for(i=0;i<chromosome;i++)
		{
			double shortest;			
			for(j=0;j<tournament;j++)
			{
				tournament_rand_data[j] = rand()%chromosome;
				tournament_countDistance[j] = countDistance(initialChromosomeData[tournament_rand_data[j]],count);								
			}
			shortest = tournament_countDistance[0];//設定tournament_countDistance[0]為最小值 
			copy(chromosomeData[i],initialChromosomeData[tournament_rand_data[0]],count);
			for(j=1;j<tournament;j++)//找出player最短的
			{
				if(shortest > tournament_countDistance[j])				
				{
					shortest = tournament_countDistance[j];
					copy(chromosomeData[i],initialChromosomeData[tournament_rand_data[j]],count);					
				} 
			}			
		}
		
		//CrossoverProbability
		double crossover_probability[chromosome/2];
		for(i=0;i<chromosome/2;i++)
		{
			crossover_probability[i] = (double)rand()/RAND_MAX;
			for(j=0;j<chromosome/2;j++)
			{
				if(crossover_probability[j] > crossoverProbability)
				{
					int r1 = rand()%chromosome,r2 = rand()%chromosome;
					while(r1 == r2)
						r1 = rand()%chromosome;					
					order_crossover(chromosomeData[r1],chromosomeData[r2],count);				
				}
			}
		}	

		//MutationProbability
		double mutation_probability[chromosome];
		for(i=0;i<chromosome;i++)
		{
			mutation_probability[i] = (double)rand()/RAND_MAX;
			if(mutation_probability[i] > mutationProbability)
				mutation(chromosomeData[i],count);
		}

		//計算每條解距離
		for(i=0;i<chromosome;i++)
			chromosome_CountDistance[i] = countDistance(chromosomeData[i],count);
		for(i=0;i<chromosome;i++)
		{
			if(bestDistance > chromosome_CountDistance[i])
				bestDistance = chromosome_CountDistance[i];
		}
		
		//好的染色體保存下來 
		for(i=0;i<chromosome;i++)
		{
			for(j=0;j<count;j++)
				initialChromosomeData[i][j] = chromosomeData[i][j];
		}
		printf("%f\n",bestDistance);		
		co++;
	}
	endclock = clock();	
//	printf("%1f\n",(endclock-startclock)/(double)(CLOCKS_PER_SEC));//total time
}

double countDistance(int array[],int number)
{	
	int i,j,countDistance_tempdata[number][2]={0};
	double toCountDistance = 0;	
	for(i=0;i<number;i++)
	{
		for(j=0;j<number;j++)
		{
			if(array[i] == data[j][0])
			{
				countDistance_tempdata[i][0] = data[j][1];//x
				countDistance_tempdata[i][1] = data[j][2];//y
			}
		}
	}
	for(j=0;j<number;j++)	
	{
		int temp;
		if(j!=(number-1))
		{
			temp = j + 1;		
			toCountDistance += sqrt(pow((countDistance_tempdata[j][0]-countDistance_tempdata[temp][0]),2)+pow((countDistance_tempdata[j][1]-countDistance_tempdata[temp][1]),2));
		}
		else
		{
			temp = 0;
			toCountDistance += sqrt(pow((countDistance_tempdata[j][0]-countDistance_tempdata[temp][0]),2)+pow((countDistance_tempdata[j][1]-countDistance_tempdata[temp][1]),2));
		}
	}
	return toCountDistance;
}

void copy(int finalData[],int startData[],int much)
{
	int ci,cj;
	for(ci=0;ci<much;ci++)
		finalData[ci] = startData[ci];
}

void order_crossover(int order_crossover_first[],int order_crossover_second[],int much)
{
	int first[much]={0},second[much]={0},r1,r2,tempRand,cri,crj,crossoverTemp1[much][2]={9999},crossoverTemp2[much][2]={9999},crossoverCount=0,temp = 0;
	double order_crossover_countDistance_start[2] = {0},order_crossover_countDistance_after[2] = {0};
	
	copy(first,order_crossover_first,much);
	copy(second,order_crossover_second,much);
	order_crossover_countDistance_start[0] = countDistance(order_crossover_first,much);
	order_crossover_countDistance_start[1] = countDistance(order_crossover_second,much);
	r1 = rand()%much;          
	r2 = rand()%much;
	while(r1 == r2)
		r1 = rand()%much;
	if(r1 > r2)//小到大 
	{
		tempRand = r1;
		r1 = r2;
		r2 = tempRand;
	}

	//切割為前後
	for(cri=0;cri<r1;cri++)
	{
		crossoverTemp1[cri][0] = first[cri];
		crossoverTemp2[cri][0] = second[cri];
		crossoverCount++;
	}
	for(cri=(r2+1);cri<much;cri++)
	{
		crossoverTemp1[crossoverCount][0] = first[cri];
		crossoverTemp2[crossoverCount][0] = second[cri];
		crossoverCount++;
	}
		
	//找出順序
	for(cri=0;cri<crossoverCount;cri++)
	{
		for(crj=0;crj<much;crj++)
		{							
			if(crossoverTemp1[cri][0] == second[crj])
				crossoverTemp1[cri][1] = crj;				
			if(crossoverTemp2[cri][0] == first[crj])
				crossoverTemp2[cri][1] = crj;
		}		
	}
	
	//由小到大排序
	for(cri=0;cri<crossoverCount;cri++)
	{
		for(crj=(cri+1);crj<crossoverCount;crj++)
		{
			int tempNumber,tempAddress;
			if(crossoverTemp1[cri][1] > crossoverTemp1[crj][1])
			{
				tempNumber = crossoverTemp1[cri][0];
				crossoverTemp1[cri][0] = crossoverTemp1[crj][0];
				crossoverTemp1[crj][0] = tempNumber;
				tempAddress = crossoverTemp1[cri][1];
				crossoverTemp1[cri][1] = crossoverTemp1[crj][1];
				crossoverTemp1[crj][1] = tempAddress;
			}
			if(crossoverTemp2[cri][1] > crossoverTemp2[crj][1])
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
	
	//放回
	for(cri=0;cri<r1;cri++)
	{
		first[cri] = crossoverTemp1[cri][0];
		second[cri] = crossoverTemp2[cri][0];		
		temp++;
	}
	for(cri=(r2+1);cri<much;cri++)
	{
		first[cri] = crossoverTemp1[temp][0];
		second[cri] = crossoverTemp2[temp][0];
		temp++;
	}
	
	order_crossover_countDistance_after[0] = countDistance(first,much);
	order_crossover_countDistance_after[1] = countDistance(second,much);
	//若比較好則取代 
	if(order_crossover_countDistance_after[0] < order_crossover_countDistance_start[0])
		copy(order_crossover_first,first,much);
	else if(crossoverCount % 5 == 0)//五次會做一次crossover 
		copy(order_crossover_first,first,much);
	if(order_crossover_countDistance_after[1] < order_crossover_countDistance_start[1])
		copy(order_crossover_second,second,much);
	else if(crossoverCount % 5 == 0)
		copy(order_crossover_first,first,much);
	crossoverCount++;
}

void tempData(int array[],int randon1,int randon2)
{
	int temp;
	
	temp = array[randon1];
	array[randon1] = array[randon2];
	array[randon2] = temp;
}

void mutation(int array[],int much)
{
	int mutation_r1 = rand()%much,mutation_r2 = rand()%much,mutation_temp,mutation_i;
	double mutation_countDistance_start = 0,mutation_countDistance_after = 0;
	
	mutation_countDistance_start = countDistance(array,much);
	while(mutation_r1 == mutation_r2)
		mutation_r1 = rand()%much;
	tempData(array,mutation_r1,mutation_r2);
	mutation_countDistance_after = countDistance(array,much);
	if(mutation_countDistance_after > mutation_countDistance_start)//若無比較好則不交換
		if(mutationCount % 5 !=0)//五次會做一次mutation 
			tempData(array,mutation_r1,mutation_r2);
	mutationCount++;
} 
