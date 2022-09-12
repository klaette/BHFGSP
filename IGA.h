#pragma once
#include<iostream>
#include<Windows.h>
#include "BHGFSP_Process.h"
#include"NEH.h"
#include<vector>

FILE* p;

void neighborhood2(vector<int> Seq, vector<int>& NewSeq)
{
	NewSeq = Seq;
	int p, q;
	do
	{
		p = rand() % numOfFamiLy;
		q = rand() % numOfFamiLy;
	} while (p >= q);

	int Gap = q - p;
	int numOfneighbor = 0;
	for (int i = 0; i <= Gap; ++i)
	{
		numOfneighbor += i;
	}

	vector<vector<int>> neighborhoods;
	neighborhoods.resize(numOfneighbor);

	int pop = 0;

	for (int j1 = q; j1 > p; j1--)
	{
		for (int j2 = j1; j2 > p; j2--)
		{
			swap(NewSeq[j2-1], NewSeq[j2]);
			neighborhoods[pop] = NewSeq;
			pop++;	
		}
		NewSeq = Seq;
	}

	int minIndex = 0;
	int minValue = GetSpan(neighborhoods[0], jobSeqInFam);
	for (int i = 1; i < numOfneighbor; ++i)
	{
		int currentValue = GetSpan(neighborhoods[i], jobSeqInFam);
		if (minValue > currentValue)
		{
			minValue = currentValue;
			minIndex = i;
		}
	}

	NewSeq = neighborhoods[minIndex];
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
		vector<int> NewSeq(numOfFamiLy, 0);
		neighborhood2(tempJobSeq1, NewSeq);

		if (GetSpan(NewSeq, jobSeqInFam) <= GetSpan(tempJobSeq1, jobSeqInFam))
		{
			tempJobSeq1 = NewSeq;
			
		}
		if (GetSpan(tempJobSeq1, jobSeqInFam) <= GetSpan(Seq, jobSeqInFam)) {
			Seq = tempJobSeq1;
			if (GetSpan(tempJobSeq1, jobSeqInFam) <= GetSpan(Seqb, jobSeqInFam)) {
				Seqb = tempJobSeq1;
			}
		}
		else if (((double)rand() / RAND_MAX) <= exp((-(GetSpan(tempJobSeq1, jobSeqInFam) - (GetSpan(Seq, jobSeqInFam)) / Temperature)))) {
			Seq = tempJobSeq1;
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
		vector<int> NewSeq(numOfFamiLy, 0);
		neighborhood2(tempJobSeq1, NewSeq);

		if (GetSpan(NewSeq, jobSeqInFam) <= GetSpan(tempJobSeq1, jobSeqInFam))
		{
			tempJobSeq1 = NewSeq;

		}
		if (GetSpan(tempJobSeq1, jobSeqInFam) <= GetSpan(Seq, jobSeqInFam)) {
			Seq = tempJobSeq1;
			if (GetSpan(tempJobSeq1, jobSeqInFam) <= GetSpan(Seqb, jobSeqInFam)) {
				Seqb = tempJobSeq1;
			}
		}
		else if (((double)rand() / RAND_MAX) <= exp((-(GetSpan(tempJobSeq1, jobSeqInFam) - (GetSpan(Seq, jobSeqInFam)) / Temperature)))) {
			Seq = tempJobSeq1;
		}


	}


	return GetSpan(Seqb, jobSeqInFam);
}

int IGA(int Ins, int TimeLimit, int d)
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