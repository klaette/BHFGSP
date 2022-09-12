#pragma once
#include<iostream>
#include<Windows.h>
#include "BHGFSP_Process.h"
#include"NEH.h"
#include<vector>

FILE* p;

void Group_RandomSwap(vector<int>& Seq)
{
	vector<int> tempSeq;
	tempSeq = Seq;

	int p, q;
	for (int i = 0; i < 4 * numOfFamiLy; ++i)
	{
		do
		{
			p = rand() % numOfFamiLy;
			q = rand() % numOfFamiLy;
		} while (p == q);

		swap(tempSeq[p], tempSeq[q]);

		if (GetSpan(tempSeq, jobSeqInFam) <= GetSpan(Seq, jobSeqInFam))
		{
			Seq = tempSeq;
		}
		else {
			tempSeq = Seq;
		}
	}

}

void Group_DisturbSwap(vector<int>& Seq)
{
	vector<int> tempSeq;
	tempSeq = Seq;

	for (int i = 0; i < numOfFamiLy - 1; ++i)
	{
		for (int j = i + 1; j < numOfFamiLy; j++)
		{
			swap(tempSeq[i], tempSeq[j]);
			if (GetSpan(tempSeq, jobSeqInFam) <= GetSpan(Seq, jobSeqInFam))
			{
				Seq = tempSeq;
			}
			else {
				tempSeq = Seq;
			}
		}
	}

}

void Group_IterativeSwap(vector<int>& Seq)
{
	vector<int> tempSeq;
	tempSeq = Seq;

	for (int i = 0; i < numOfFamiLy - 1; ++i)
	{
		int minSpan = GetSpan(tempSeq, jobSeqInFam);
		int minPos = -1;
		for (int j = i + 1; j < numOfFamiLy; j++)
		{
			swap(tempSeq[i], tempSeq[j]);
			int Span = GetSpan(tempSeq,jobSeqInFam);
			if (Span <= minSpan)
			{
				minSpan = Span;
				minPos = j;
			}
			tempSeq = Seq;
		}
		if (minPos != -1)
		{
			swap(tempSeq[i], tempSeq[minPos]);
		}
	}

}

void Group_SequentialSwap(vector<int>& Seq)
{
	vector<int> tempSeq;
	tempSeq = Seq;

	for (int i = 0; i < numOfFamiLy - 1; ++i)
	{
		int count = 0;
		vector<vector<int>> RecordSeq;
		RecordSeq.resize(numOfFamiLy - i - 1);
		for (int j = i; j < numOfFamiLy - 1; j++)
		{
			swap(tempSeq[j+1], tempSeq[j]);
			RecordSeq[count] = tempSeq;
			count++;
		}
		int bestSpan = GetSpan(Seq, jobSeqInFam);
		for (int k = 0; k < count; ++k)
		{
			int minSpan = GetSpan(RecordSeq[k], jobSeqInFam);
			if (minSpan <= bestSpan)
			{
				bestSpan = minSpan;
				Seq = RecordSeq[k];
			}
		}
		tempSeq = Seq;
		RecordSeq.clear();
	}

}

void SingleBlockingJob_Swap(vector<int>& Seq, int Fam)
{
	vector<int> tempSeq;
	tempSeq = Seq;
	int minPos = -1;
	int minSpan = GetSpan(tempSeq, jobSeqInFam);

	for (int f = 0; f < numOfFamiLy; ++f)
	{
		if (f != Fam)
		{
			swap(tempSeq[f], tempSeq[Fam]);
			int curSpan = GetSpan(tempSeq, jobSeqInFam);
			if (curSpan <= minSpan)
			{
				minSpan = curSpan;
				minPos = f;
			}
			tempSeq = Seq;
		}
	}
	if (minPos != -1)
	{
		swap(Seq[Fam],Seq[minPos]);
	}
}

void BlockingJob_RandomSwap(vector<int>& Seq,int job, vector<vector<int>>& TheJobSeqInFam)
{
	int Fam = JobBelongToOneFam[job];
	int random;
	int Pos;
	vector<vector<int>> tempSeq;
	tempSeq = TheJobSeqInFam;
	int bestSpan = GetSpan(Seq, TheJobSeqInFam);
	for (int j = 0; j < tempSeq[Fam].size(); j++)
	{
		if (tempSeq[Fam][j] == job)
		{
			Pos = j;
		}
	}

	for (int j = 0; j < 3 * tempSeq[Fam].size(); j++)
	{
		do
		{
			random = rand() % (tempSeq[Fam].size());
		} while (Pos == random);

		swap(tempSeq[Fam][random], tempSeq[Fam][Pos]);
		int Span = GetSpan(Seq, tempSeq);
		if (Span <= bestSpan)
		{
			TheJobSeqInFam = tempSeq;
			bestSpan = Span;
			Pos = random;
		}
		else
		{
			tempSeq = TheJobSeqInFam;
		}
		
	}

}

void BlockingJob_Greedy1Swap(vector<int>& Seq, int job, vector<vector<int>>& TheJobSeqInFam)
{
	int Fam = JobBelongToOneFam[job];
	int Pos;
	vector<vector<int>> tempSeq;
	tempSeq = TheJobSeqInFam;
	int bestSpan = GetSpan(Seq, TheJobSeqInFam);

	for (int j = 0; j < tempSeq[Fam].size(); j++)
	{
		if (tempSeq[Fam][j] == job)
		{
			Pos = j;
		}
	}

	for (int j = 0; j < tempSeq[Fam].size(); j++)
	{
		if (j != Pos)
		{
			swap(tempSeq[Fam][j], tempSeq[Fam][Pos]);
		}
		int Span = GetSpan(Seq, tempSeq);
		if (Span < bestSpan)
		{
			TheJobSeqInFam = tempSeq;
			bestSpan = Span;
			Pos = j;
		}
		else
		{
			tempSeq = TheJobSeqInFam;
		}
	}

}

void BlockingJob_Greedy2Swap(vector<int>& Seq, int job, vector<vector<int>>& TheJobSeqInFam)
{
	int Fam = JobBelongToOneFam[job];
	int Pos;
	int minPos = -1;
	vector<vector<int>> tempSeq;
	tempSeq = TheJobSeqInFam;
	int bestSpan = GetSpan(Seq, TheJobSeqInFam);

	for (int j = 0; j < tempSeq[Fam].size(); j++)
	{
		if (tempSeq[Fam][j] == job)
		{
			Pos = j;
		}
	}

	for (int j = 0; j < tempSeq[Fam].size(); j++)
	{
		if (j != Pos)
		{
			swap(tempSeq[Fam][j], tempSeq[Fam][Pos]);
		}
		int Span = GetSpan(Seq, tempSeq);
		if (Span <= bestSpan)
		{
			tempSeq = TheJobSeqInFam;
			bestSpan = Span;
			minPos = j;
		}
		else
		{
			tempSeq = TheJobSeqInFam;
		}
	}
	if (minPos != -1)
	{
		swap(tempSeq[Fam][minPos], tempSeq[Fam][Pos]);
		TheJobSeqInFam = tempSeq;
	}

}

void Probabilistic_Selection_Group_Strategy(vector<int>& Seq)
{
	int r = rand() % 4;
	if (r == 0)
	{
		Group_RandomSwap(Seq);
	}
	else if (r == 1)
	{
		Group_DisturbSwap(Seq);
	}
	else if (r == 2)
	{
		Group_IterativeSwap(Seq);
	}
	else
	{
		Group_SequentialSwap(Seq);
	}

}

void Probabilistic_Selection_JobBlocking_Strategy(vector<int>& Seq, vector<vector<int>>& TheJobSeqInFam)
{
	int minSpan = GetSpanWithBlocking(Seq, TheJobSeqInFam);
	BlockingJobSwapInit(RecordJobBlockTime);
	vector<int> tempSeq;
	tempSeq = Seq;
	int r;
	for (int j = 0; j < numOfJob; j++)
	{
		if (TheJobSeqInFam[JobBelongToOneFam[RecordJobBlockTime[j]]].size() == 1)
		{
			SingleBlockingJob_Swap(Seq, JobBelongToOneFam[RecordJobBlockTime[j]]);
		}
		else
		{
			r = rand() % 3;
			if (r == 0)
			{
				BlockingJob_RandomSwap(Seq, RecordJobBlockTime[j], TheJobSeqInFam);
			}
			else if (r == 1)
			{
				BlockingJob_Greedy1Swap(Seq, RecordJobBlockTime[j], TheJobSeqInFam);
			}
			else
			{
				BlockingJob_Greedy2Swap(Seq, RecordJobBlockTime[j], TheJobSeqInFam);
			}
		}
	}
	 
}

void Symmetric_SwapStrategy(vector<int>& Seq)
{
	vector<int> tempSeq;
	tempSeq = Seq;
	vector<vector<int>> tempJobSeq;
	tempJobSeq = jobSeqInFam;
	int j1; 
	int j2;
	
	for (int j = 0; j < numOfFamiLy; j++)
	{
		do
		{
			j1 = rand() % numOfFamiLy;
			j2 = rand() % numOfFamiLy;
		} while (j1 == j2);
		swap(tempSeq[j1], tempSeq[j2]);
		if (GetSpan(tempSeq, tempJobSeq) <= GetSpan(Seq, tempJobSeq))
		{
			Seq = tempSeq;
			if (tempJobSeq[j1].size() > 1)
			{
				int p, q;
				do
				{
					p = rand() % (tempJobSeq[j1].size());
					q = rand() % (tempJobSeq[j1].size());
				} while (p == q);
				swap(tempJobSeq[j1][p], tempJobSeq[j1][q]);
				if (GetSpan(tempSeq, tempJobSeq) <= GetSpan(tempSeq, jobSeqInFam))
				{
					jobSeqInFam = tempJobSeq;
				}
				else
				{
					tempJobSeq = jobSeqInFam;
				}
			}
			if (tempJobSeq[j2].size() > 1)
			{
				int p, q;
				do
				{
					p = rand() % (tempJobSeq[j2].size());
					q = rand() % (tempJobSeq[j2].size());
				} while (p == q);
				swap(tempJobSeq[j2][p], tempJobSeq[j2][q]);
				if (GetSpan(tempSeq, tempJobSeq) <= GetSpan(tempSeq, jobSeqInFam))
				{
					jobSeqInFam = tempJobSeq;
				}
				else
				{
					tempJobSeq = jobSeqInFam;
				}
			}
		}
		else {
			tempSeq = Seq;
		}
	}
		
}

int Evolve(vector<int> Seq, int d, int TimeLimit, int Temperature)
{
	vector<int> Seqb;
	Seqb = Seq;

	vector<int> tempJobSeq(numOfFamiLy, -1);
	vector<int> tempJobSeq1(numOfFamiLy, -1);
	vector<int> tempJobSeq_R(numOfFamiLy, -1);
	vector<int> bestSeqSofar(numOfFamiLy, -1);
	vector<int> tempJobSeq_other1(numOfFamiLy, -1);
	vector<int> tempJobSeq_other2(numOfFamiLy, -1);

	vector<int> minValue(numOfFamiLy + 1, INT_MAX);
	int count2 = 0;
	long time1 = GetTickCount64();

	while ((GetTickCount64() - time1) < TimeLimit)
	{
		int pt;
		int len = 0;
		int count = 1;

		vector<bool> PosIsSelected(numOfFamiLy, 0);

		tempJobSeq = Seq;

		//destruction
		while (count <= d)
		{
			pt = rand() % numOfFamiLy;
			if (PosIsSelected[pt] == false) {
				PosIsSelected[pt] = 1;
				count++;
				tempJobSeq_R[len] = tempJobSeq[pt];
				len++;
				tempJobSeq[pt] = -1;
			}

		}
		len = 0;
		for (int k = 0; k < numOfFamiLy; ++k)
		{
			if (tempJobSeq[k] != -1) {

				tempJobSeq_other1[len] = tempJobSeq[k];
				len++;
			}
		}

		//construction
		int q, w, e;
		bestSeqSofar = tempJobSeq_other1;

		for (e = 0; e < d; ++e)
		{
			tempJobSeq_other1 = bestSeqSofar;
			int minValue = INT_MAX;
			for (q = 0; q < numOfFamiLy - d + e; ++q)
			{
				tempJobSeq_other2 = tempJobSeq_other1;
				for (w = numOfFamiLy - d + e - 1; w >= q; --w)
				{
					tempJobSeq_other2[w + 1] = tempJobSeq_other2[w];
				}
				tempJobSeq_other2[q] = tempJobSeq_R[e];

				vector<int> tempJobSeq_other3(numOfFamiLy - d + e + 1, -1);
				tempJobSeq_other3 = tempJobSeq_other2;
				if (GetSpanOfsubSeq(tempJobSeq_other3, jobSeqInFam, numOfFamiLy - d + e + 1) < minValue)
				{
					minValue = GetSpanOfsubSeq(tempJobSeq_other3, jobSeqInFam, numOfFamiLy - d + e + 1);
					bestSeqSofar = tempJobSeq_other3;
				}
			}

		}

		 tempJobSeq1 = bestSeqSofar;
		 Probabilistic_Selection_Group_Strategy(tempJobSeq1);

		 vector <vector<int>> TheJobSeqInFam = jobSeqInFam;
		 Probabilistic_Selection_JobBlocking_Strategy(tempJobSeq1, TheJobSeqInFam);

		 if (GetSpan(tempJobSeq1, TheJobSeqInFam) <= GetSpan(Seq, jobSeqInFam)) {
			 Seq = tempJobSeq1;
			 jobSeqInFam = TheJobSeqInFam;
			 if (GetSpan(tempJobSeq1, TheJobSeqInFam) <= GetSpan(Seqb, jobSeqInFam)) {
				 Seqb = tempJobSeq1;
			 }
		 }
		 else if (((double)rand() / RAND_MAX) <= exp((-(GetSpan(tempJobSeq1, jobSeqInFam) - (GetSpan(Seq, jobSeqInFam)) / Temperature)))) {
			 Seq = tempJobSeq1;
			 jobSeqInFam = TheJobSeqInFam;
		 }

	}


	return GetSpan(Seqb, jobSeqInFam);
}

int EvolveCurve(vector<int> Seq, int d, int TimeLimit, int Temperature, long InitTime)
{
	vector<int> Seqb;
	Seqb = Seq;

	vector<int> tempJobSeq(numOfFamiLy, -1);
	vector<int> tempJobSeq1(numOfFamiLy, -1);
	vector<int> tempJobSeq_R(numOfFamiLy, -1);
	vector<int> bestSeqSofar(numOfFamiLy, -1);
	vector<int> tempJobSeq_other1(numOfFamiLy, -1);
	vector<int> tempJobSeq_other2(numOfFamiLy, -1);

	vector<int> minValue(numOfFamiLy + 1, INT_MAX);
	int count2 = 0;
	long time1 = GetTickCount64();

	while ((GetTickCount64() - time1) < TimeLimit)
	{
		long LocalTime = GetTickCount64() - InitTime;
		int LocalValue = GetSpan(Seqb, jobSeqInFam);
		cout << LocalTime << "\t" << LocalValue << "\t" << endl;

		int pt;
		int len = 0;
		int count = 1;

		vector<bool> PosIsSelected(numOfFamiLy, 0);

		tempJobSeq = Seq;

		//destruction
		while (count <= d)
		{
			pt = rand() % numOfFamiLy;
			if (PosIsSelected[pt] == false) {
				PosIsSelected[pt] = 1;
				count++;
				tempJobSeq_R[len] = tempJobSeq[pt];
				len++;
				tempJobSeq[pt] = -1;
			}

		}
		len = 0;
		for (int k = 0; k < numOfFamiLy; ++k)
		{
			if (tempJobSeq[k] != -1) {

				tempJobSeq_other1[len] = tempJobSeq[k];
				len++;
			}
		}

		//construction
		int q, w, e;
		bestSeqSofar = tempJobSeq_other1;

		for (e = 0; e < d; ++e)
		{
			tempJobSeq_other1 = bestSeqSofar;
			int minValue = INT_MAX;
			for (q = 0; q < numOfFamiLy - d + e; ++q)
			{
				tempJobSeq_other2 = tempJobSeq_other1;
				for (w = numOfFamiLy - d + e - 1; w >= q; --w)
				{
					tempJobSeq_other2[w + 1] = tempJobSeq_other2[w];
				}
				tempJobSeq_other2[q] = tempJobSeq_R[e];

				vector<int> tempJobSeq_other3(numOfFamiLy - d + e + 1, -1);
				tempJobSeq_other3 = tempJobSeq_other2;
				if (GetSpanOfsubSeq(tempJobSeq_other3, jobSeqInFam, numOfFamiLy - d + e + 1) < minValue)
				{
					minValue = GetSpanOfsubSeq(tempJobSeq_other3, jobSeqInFam, numOfFamiLy - d + e + 1);
					bestSeqSofar = tempJobSeq_other3;
				}
			}

		}

		tempJobSeq1 = bestSeqSofar;
		Probabilistic_Selection_Group_Strategy(tempJobSeq1);

		vector <vector<int>> TheJobSeqInFam = jobSeqInFam;
		Probabilistic_Selection_JobBlocking_Strategy(tempJobSeq1, TheJobSeqInFam);

		if (GetSpan(tempJobSeq1, TheJobSeqInFam) <= GetSpan(Seq, jobSeqInFam)) {
			Seq = tempJobSeq1;
			jobSeqInFam = TheJobSeqInFam;
			if (GetSpan(tempJobSeq1, TheJobSeqInFam) <= GetSpan(Seqb, jobSeqInFam)) {
				Seqb = tempJobSeq1;
			}
		}
		else if (((double)rand() / RAND_MAX) <= exp((-(GetSpan(tempJobSeq1, jobSeqInFam) - (GetSpan(Seq, jobSeqInFam)) / Temperature)))) {
			Seq = tempJobSeq1;
			jobSeqInFam = TheJobSeqInFam;
		}

	}


	return GetSpan(Seqb, jobSeqInFam);
}

int NIG(int Ins, int TimeLimit, int d)
{

	vector <int>Seq(numOfFamiLy, 0);

	vector <int> ch(numOfFamiLy, 0);
	for (int j = 0; j < numOfFamiLy; j++)
	{
		ch[j] = j;
		for (int k = 0; k < jobSeqInFam[j].size(); k++)
		{
			for (int s = 0; s < numOfStage; ++s)
			{
				ch[j] += processTime[s][jobSeqInFam[j][k]] + setupTime[s][j][j];
			}
		}
	}

	int minValue = INT_MAX;
	for (int j = 0; j < numOfFamiLy; j++)
	{
		if (ch[j] < minValue)
		{
			minValue = ch[j];
		}
	}

	double Temperature = (minValue / 10.0 * numOfFamiLy * numOfStage) * 0.9;
	long initTime = GetTickCount64();
	NEH(Seq);
	//int bestValuesofar = EvolveCurve(Seq, d, TimeLimit, Temperature, initTime);
	int bestValuesofar = Evolve(Seq, d, TimeLimit, Temperature);
	return bestValuesofar;
}
