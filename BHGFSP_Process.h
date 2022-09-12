#pragma once
#include<vector>
#include<algorithm>

using namespace std;

// -------------for ordering----------
typedef struct
{
	int dim;
	int value;
} Pair;

class PairGreater {
public:
	bool operator () (Pair a, Pair b)    //函数对象 也称之为伪函数
	{
		return a.value > b.value;
	}
};

class PairLess {
public:
	bool operator () (Pair a, Pair b)
	{
		return a.value < b.value;
	}
};

//-----------------------
int Jobs[12] = {80,100,120,140,160,180,200,220,240,260,280,300};
int Stages[3] = {3,5,8};
int families[108] = 
{20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60,
20,20,20,40,40,40,60,60,60
};
//-----------------------

int numOfJob;
int numOfStage;
int numOfFamiLy;
vector<int> numOfMachineAtEachStage;
vector<int>RecordJobBlockTime;
vector<vector<int>>processTime;
vector<vector<vector<int>>>setupTime;
vector<int> famSeq;
vector<vector<int>> jobSeqInFam;
vector<int> JobBelongToOneFam;

void GenerateInstances(int InJob, int InStage, int InFamily, int seed)
{
	srand(seed);
	numOfJob = InJob;
	numOfStage = InStage;
	numOfFamiLy = InFamily;
	numOfMachineAtEachStage.resize(InStage);
	famSeq.resize(InFamily);
	jobSeqInFam.resize(InFamily);
	JobBelongToOneFam.resize(InJob);

	for (int i = 0; i < numOfStage; i++)
	{
		numOfMachineAtEachStage[i] = 1 + rand() % 5;  //range of machines [1-5]	
	}

	for (int i = 0; i < numOfFamiLy; i++)
	{
		famSeq[i] = i;
	}


	for (int j = 0; j < numOfFamiLy; j++)
	{
		jobSeqInFam[j].push_back(j);
		JobBelongToOneFam[j] = j;
	}
	
	for (int j = numOfFamiLy; j < numOfJob; ++j)
	{
		int rands = rand()% numOfFamiLy;
				
		jobSeqInFam[rands].push_back(j);
		JobBelongToOneFam[j] = rands;
	}

	processTime.resize(numOfJob);
	for (int i = 0; i < numOfJob; i++)
	{
		processTime[i].resize(numOfStage);
	}
	for (int i = 0; i < numOfJob; i++)
	{
		for (int j = 0; j < numOfStage; j++)
		{
			processTime[i][j] = 50 + rand() % 50;  //range of processing time [50-99]	

		}
	}

	setupTime.resize(numOfStage);
	for (int k = 0; k < numOfStage; k++)
	{
		setupTime[k].resize(numOfFamiLy);
	}
	for (int k = 0; k < numOfStage; k++)
	{
		for (int j = 0; j < numOfFamiLy; j++)
		{
			setupTime[k][j].resize(numOfFamiLy);
		}
	}
	for (int k = 0; k < numOfStage; k++)
	{
		for (int j = 0; j < numOfFamiLy; j++)
		{
			for (int l = 0; l < numOfFamiLy; l++)
			{
				setupTime[k][j][l] = 10 + rand() % 11; //range of Setup time [10-20]	
			}
		}
	}

	RecordJobBlockTime.resize(numOfJob, 0);

}

int GetSpan(vector<int>& familySeqence,
	vector<vector<int>>& jobSequenceInEachFamily)
{
	vector<vector<int>> jobCompleteTimeAtEachStage(numOfJob, vector<int>(numOfStage, 0)); //每个工件在每个阶段的完工时间
	vector<vector<int>> jobDepartureTimeAtEachStage(numOfJob, vector<int>(numOfStage, 0)); //每个工件在每个阶段的离开时间
	vector<vector<int>> machineIdleTime(numOfStage); //每个阶段的每个机器的空闲时间
	vector<vector<int>> processFamily(numOfStage); //每个阶段的每个机器最近加工的组


	for (int s = 0; s < numOfStage; ++s)
	{
		machineIdleTime[s].resize(numOfMachineAtEachStage[s], 0);
		processFamily[s].resize(numOfMachineAtEachStage[s], -1); //-1表示这个阶段的这个机器没有加工任何组
	}

	for (int curFamily : familySeqence)
	{
		vector<int> selectedMachineAtEachStage(numOfStage); //记录对于curFamily，每个阶段选择的机器
		//对于curFamily，每个阶段进行机器的选择
		for (int s = 0; s < numOfStage; ++s)
		{
			int selectedMachine;
			int minIdle = INT_MAX;
			for (int m = 0; m < numOfMachineAtEachStage[s]; m++)
			{
				if (processFamily[s][m] == -1)
				{
					if (machineIdleTime[s][m] + setupTime[s][curFamily][curFamily] < minIdle)
					{
						minIdle = machineIdleTime[s][m] + setupTime[s][curFamily][curFamily];
						selectedMachine = m;
					}
				}
				else
				{
					int preFamliy = processFamily[s][m];
					if (machineIdleTime[s][m] + setupTime[s][preFamliy][curFamily] < minIdle)
					{
						minIdle = machineIdleTime[s][m] + setupTime[s][preFamliy][curFamily];
						selectedMachine = m;
					}
				}
			}
			selectedMachineAtEachStage[s] = selectedMachine;
			processFamily[s][selectedMachine] = curFamily;
			machineIdleTime[s][selectedMachine] = minIdle;
		}

		for (int job : jobSequenceInEachFamily[curFamily])
		{
			int selectedMachineAtFirstStage = selectedMachineAtEachStage[0]; //第一个阶段选择的机器
			jobCompleteTimeAtEachStage[job][0] = machineIdleTime[0][selectedMachineAtFirstStage] + processTime[job][0];
			jobDepartureTimeAtEachStage[job][0] = jobCompleteTimeAtEachStage[job][0];
			machineIdleTime[0][selectedMachineAtFirstStage] = jobDepartureTimeAtEachStage[job][0];

			for (int s = 1; s < numOfStage; ++s) //其他阶段
			{
				int selectedMachineAtCurrentStage = selectedMachineAtEachStage[s]; //当前阶段s选择的机器
				jobCompleteTimeAtEachStage[job][s] =
					max(machineIdleTime[s][selectedMachineAtCurrentStage], jobCompleteTimeAtEachStage[job][s - 1])
					+ processTime[job][s];
				jobDepartureTimeAtEachStage[job][s] = jobCompleteTimeAtEachStage[job][s];
				machineIdleTime[s][selectedMachineAtCurrentStage] = jobDepartureTimeAtEachStage[job][s];

				int selectedMachineAtPreviousStage = selectedMachineAtEachStage[s - 1]; //当前阶段s的前一个阶段选择的机器
				jobDepartureTimeAtEachStage[job][s - 1] = jobCompleteTimeAtEachStage[job][s] - processTime[job][s];
				machineIdleTime[s - 1][selectedMachineAtPreviousStage] = jobDepartureTimeAtEachStage[job][s - 1];
			}
			
		}
	}

	//for (int job = 0; job < numOfJob; ++job)
	//{
	//	for (int s = 0; s < numOfStage; ++s)
	//	{
	//		cout << jobCompleteTimeAtEachStage[job][s] << ", " << jobDepartureTimeAtEachStage[job][s] << endl;
	//	}
	//}

	int makeSpan = 0;
	for (int job = 0; job < numOfJob; ++job)
	{
		if (jobDepartureTimeAtEachStage[job][numOfStage - 1] > makeSpan)
		{
			makeSpan = jobDepartureTimeAtEachStage[job][numOfStage - 1];
		}
	}
	return makeSpan;
}

int GetSpanWithBlocking(vector<int>& familySeqence,
	vector<vector<int>>& jobSequenceInEachFamily)
{
	vector<vector<int>> jobCompleteTimeAtEachStage(numOfJob, vector<int>(numOfStage, 0)); //每个工件在每个阶段的完工时间
	vector<vector<int>> jobDepartureTimeAtEachStage(numOfJob, vector<int>(numOfStage, 0)); //每个工件在每个阶段的离开时间
	vector<vector<int>> machineIdleTime(numOfStage); //每个阶段的每个机器的空闲时间
	vector<vector<int>> processFamily(numOfStage); //每个阶段的每个机器最近加工的组


	for (int s = 0; s < numOfStage; ++s)
	{
		machineIdleTime[s].resize(numOfMachineAtEachStage[s], 0);
		processFamily[s].resize(numOfMachineAtEachStage[s], -1); //-1表示这个阶段的这个机器没有加工任何组
	}

	for (int curFamily : familySeqence)
	{
		vector<int> selectedMachineAtEachStage(numOfStage); //记录对于curFamily，每个阶段选择的机器
		//对于curFamily，每个阶段进行机器的选择
		for (int s = 0; s < numOfStage; ++s)
		{
			int selectedMachine;
			int minIdle = INT_MAX;
			for (int m = 0; m < numOfMachineAtEachStage[s]; m++)
			{
				if (processFamily[s][m] == -1)
				{
					if (machineIdleTime[s][m] + setupTime[s][curFamily][curFamily] < minIdle)
					{
						minIdle = machineIdleTime[s][m] + setupTime[s][curFamily][curFamily];
						selectedMachine = m;
					}
				}
				else
				{
					int preFamliy = processFamily[s][m];
					if (machineIdleTime[s][m] + setupTime[s][preFamliy][curFamily] < minIdle)
					{
						minIdle = machineIdleTime[s][m] + setupTime[s][preFamliy][curFamily];
						selectedMachine = m;
					}
				}
			}
			selectedMachineAtEachStage[s] = selectedMachine;
			processFamily[s][selectedMachine] = curFamily;
			machineIdleTime[s][selectedMachine] = minIdle;
		}

		for (int job : jobSequenceInEachFamily[curFamily])
		{
			int selectedMachineAtFirstStage = selectedMachineAtEachStage[0]; //第一个阶段选择的机器
			jobCompleteTimeAtEachStage[job][0] = machineIdleTime[0][selectedMachineAtFirstStage] + processTime[job][0];
			jobDepartureTimeAtEachStage[job][0] = jobCompleteTimeAtEachStage[job][0];
			machineIdleTime[0][selectedMachineAtFirstStage] = jobDepartureTimeAtEachStage[job][0];

			for (int s = 1; s < numOfStage; ++s) //其他阶段
			{
				int selectedMachineAtCurrentStage = selectedMachineAtEachStage[s]; //当前阶段s选择的机器
				jobCompleteTimeAtEachStage[job][s] =
					max(machineIdleTime[s][selectedMachineAtCurrentStage], jobCompleteTimeAtEachStage[job][s - 1])
					+ processTime[job][s];
				jobDepartureTimeAtEachStage[job][s] = jobCompleteTimeAtEachStage[job][s];
				machineIdleTime[s][selectedMachineAtCurrentStage] = jobDepartureTimeAtEachStage[job][s];

				int selectedMachineAtPreviousStage = selectedMachineAtEachStage[s - 1]; //当前阶段s的前一个阶段选择的机器
				jobDepartureTimeAtEachStage[job][s - 1] = jobCompleteTimeAtEachStage[job][s] - processTime[job][s];
				machineIdleTime[s - 1][selectedMachineAtPreviousStage] = jobDepartureTimeAtEachStage[job][s - 1];
			}

		}
	}

	//for (int job = 0; job < numOfJob; ++job)
	//{
	//	for (int s = 0; s < numOfStage; ++s)
	//	{
	//		cout << jobCompleteTimeAtEachStage[job][s] << ", " << jobDepartureTimeAtEachStage[job][s] << endl;
	//	}
	//}

	for (int job = 0; job < numOfJob; ++job)
	{
		for (int s = 0; s < numOfStage; ++s)
		{
			RecordJobBlockTime[job] += (jobDepartureTimeAtEachStage[job][s] - jobCompleteTimeAtEachStage[job][s]);
		}
	}

	int makeSpan = 0;
	for (int job = 0; job < numOfJob; ++job)
	{
		if (jobDepartureTimeAtEachStage[job][numOfStage - 1] > makeSpan)
		{
			makeSpan = jobDepartureTimeAtEachStage[job][numOfStage - 1];
		}
	}
	return makeSpan;
}

int GetSpanOfsubSeq(vector<int>& familySeqence,
	vector<vector<int>>& jobSequenceInEachFamily,int length)
{
	vector<vector<int>> jobCompleteTimeAtEachStage(numOfJob, vector<int>(numOfStage, 0)); //每个工件在每个阶段的完工时间
	vector<vector<int>> jobDepartureTimeAtEachStage(numOfJob, vector<int>(numOfStage, 0)); //每个工件在每个阶段的离开时间
	vector<vector<int>> machineIdleTime(numOfStage); //每个阶段的每个机器的空闲时间
	vector<vector<int>> processFamily(numOfStage); //每个阶段的每个机器最近加工的组


	for (int s = 0; s < numOfStage; ++s)
	{
		machineIdleTime[s].resize(numOfMachineAtEachStage[s], 0);
		processFamily[s].resize(numOfMachineAtEachStage[s], -1); //-1表示这个阶段的这个机器没有加工任何组
	}

	for (int curFamily = 0; curFamily < length; curFamily++)
	{
		vector<int> selectedMachineAtEachStage(numOfStage); //记录对于curFamily，每个阶段选择的机器
		//对于curFamily，每个阶段进行机器的选择
		for (int s = 0; s < numOfStage; ++s)
		{
			int selectedMachine;
			int minIdle = INT_MAX;
			for (int m = 0; m < numOfMachineAtEachStage[s]; m++)
			{
				if (processFamily[s][m] == -1)
				{
					if (machineIdleTime[s][m] + setupTime[s][curFamily][curFamily] < minIdle)
					{
						minIdle = machineIdleTime[s][m] + setupTime[s][curFamily][curFamily];
						selectedMachine = m;
					}
				}
				else
				{
					int preFamliy = processFamily[s][m];
					if (machineIdleTime[s][m] + setupTime[s][preFamliy][curFamily] < minIdle)
					{
						minIdle = machineIdleTime[s][m] + setupTime[s][preFamliy][curFamily];
						selectedMachine = m;
					}
				}
			}
			selectedMachineAtEachStage[s] = selectedMachine;
			processFamily[s][selectedMachine] = curFamily;
			machineIdleTime[s][selectedMachine] = minIdle;
		}

		for (int job : jobSequenceInEachFamily[curFamily])
		{
			int selectedMachineAtFirstStage = selectedMachineAtEachStage[0]; //第一个阶段选择的机器
			jobCompleteTimeAtEachStage[job][0] = machineIdleTime[0][selectedMachineAtFirstStage] + processTime[job][0];
			jobDepartureTimeAtEachStage[job][0] = jobCompleteTimeAtEachStage[job][0];
			machineIdleTime[0][selectedMachineAtFirstStage] = jobDepartureTimeAtEachStage[job][0];

			for (int s = 1; s < numOfStage; ++s) //其他阶段
			{
				int selectedMachineAtCurrentStage = selectedMachineAtEachStage[s]; //当前阶段s选择的机器
				jobCompleteTimeAtEachStage[job][s] =
					max(machineIdleTime[s][selectedMachineAtCurrentStage], jobCompleteTimeAtEachStage[job][s - 1])
					+ processTime[job][s];
				jobDepartureTimeAtEachStage[job][s] = jobCompleteTimeAtEachStage[job][s];
				machineIdleTime[s][selectedMachineAtCurrentStage] = jobDepartureTimeAtEachStage[job][s];

				int selectedMachineAtPreviousStage = selectedMachineAtEachStage[s - 1]; //当前阶段s的前一个阶段选择的机器
				jobDepartureTimeAtEachStage[job][s - 1] = jobCompleteTimeAtEachStage[job][s] - processTime[job][s];
				machineIdleTime[s - 1][selectedMachineAtPreviousStage] = jobDepartureTimeAtEachStage[job][s - 1];
			}

		}
	}

	//for (int job = 0; job < numOfJob; ++job)
	//{
	//	for (int s = 0; s < numOfStage; ++s)
	//	{
	//		cout << jobCompleteTimeAtEachStage[job][s] << ", " << jobDepartureTimeAtEachStage[job][s] << endl;
	//	}
	//}

	int makeSpan = 0;
	for (int job = 0; job < numOfJob; ++job)
	{
		if (jobDepartureTimeAtEachStage[job][numOfStage - 1] > makeSpan)
		{
			makeSpan = jobDepartureTimeAtEachStage[job][numOfStage - 1];
		}
	}
	return makeSpan;
}