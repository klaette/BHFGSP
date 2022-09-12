#pragma once
#include<iostream>
#include<Windows.h>
#include "BHGFSP_Process.h"
#include"NEH.h"
#include<vector>

FILE* p;

void NormalSwap(vector <int>& p)
{
	//Find two distinct elements randomly;
	for (int i = 0; i < numOfFamiLy; ++i)
	{
		vector <int> p1(numOfFamiLy, 0);
		p1 = p;
		int  pt1, pt2;
		do
		{
			pt1 = rand() % numOfFamiLy;
			pt2 = rand() % numOfFamiLy;
		} while (pt1 == pt2);

		swap(p1[pt1], p1[pt2]);
		if (GetSpan(p1, jobSeqInFam) < GetSpan(p, jobSeqInFam))
		{
			p = p1;
		}
	}

}
void quickSwap(vector <int>& Seq, int len)
{

	vector <int>TempSeq(numOfFamiLy, -1);
	TempSeq = Seq;

	int i = 0;
	int j = len - 1;
	while (i < j)
	{
		if (i < j)
		{
			swap(TempSeq[i], TempSeq[j]);
			if (GetSpan(TempSeq, jobSeqInFam) < GetSpan(Seq, jobSeqInFam))
			{
				Seq = TempSeq;
			}
			else {
				TempSeq = Seq;
			}
			j--;
			i++;
		}
	}
}

int Evolve(vector<int> Seq, int d, int TimeLimit, int Temperature, int jP)
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

		if (GetSpan(tempJobSeq1, jobSeqInFam) < GetSpan(Seqb, jobSeqInFam)) {
			Seqb = tempJobSeq1;
		}

		int r = rand() * 1.0 / RAND_MAX;
		if (r < 0.1) {
			NormalSwap(tempJobSeq1);
		}
		int r1 = rand() * 1.0 / RAND_MAX;
		if (r1 < 0.1) {
			quickSwap(tempJobSeq1, numOfFamiLy);
		}

		if (GetSpan(tempJobSeq1, jobSeqInFam) <= GetSpan(Seq, jobSeqInFam))
		{
			Seq = tempJobSeq1;
			if (GetSpan(Seq, jobSeqInFam) < GetSpan(Seqb, jobSeqInFam)) {
				Seqb = tempJobSeq1;
			}
		}

		else if (((double)rand() / RAND_MAX) <= exp((-(GetSpan(tempJobSeq1, jobSeqInFam) - (GetSpan(Seq, jobSeqInFam)) / Temperature)))) {

			Seq = tempJobSeq1;

		}

	}

	return GetSpan(Seqb, jobSeqInFam);
}

int EvolveCurve(vector<int> Seq, int d, int TimeLimit, int Temperature, int jP, long InitTime)
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

		if (GetSpan(tempJobSeq1, jobSeqInFam) < GetSpan(Seqb, jobSeqInFam)) {
			Seqb = tempJobSeq1;
		}

		int r = rand() * 1.0 / RAND_MAX;
		if (r < 0.1) {
			NormalSwap(tempJobSeq1);
		}
		int r1 = rand() * 1.0 / RAND_MAX;
		if (r1 < 0.1) {
			quickSwap(tempJobSeq1, numOfFamiLy);
		}

		if (GetSpan(tempJobSeq1, jobSeqInFam) <= GetSpan(Seq, jobSeqInFam))
		{
			Seq = tempJobSeq1;
			if (GetSpan(Seq, jobSeqInFam) < GetSpan(Seqb, jobSeqInFam)) {
				Seqb = tempJobSeq1;
			}
		}

		else if (((double)rand() / RAND_MAX) <= exp((-(GetSpan(tempJobSeq1, jobSeqInFam) - (GetSpan(Seq, jobSeqInFam)) / Temperature)))) {

			Seq = tempJobSeq1;

		}

	}

	return GetSpan(Seqb, jobSeqInFam);
}

int IGDLM(int Ins, int TimeLimit)
{
	int d = 4;
	vector <int>pJobSeq(numOfFamiLy, 0);
	vector <int>pJobSeqb(numOfFamiLy, 0);
	int totalPtime = 0;
	for (int j = 0; j < numOfFamiLy; ++j)
	{
		for (int k = 0; k < numOfStage; ++k)
		{
			totalPtime += processTime[j][k];
		}
	}
	double Temperature = (totalPtime / 10.0 * numOfFamiLy * numOfStage) * 0.50;
	long initTime = GetTickCount64();
	NEH(pJobSeq);
	int bestValuesofar = EvolveCurve(pJobSeq, d, TimeLimit, Temperature, 0.4, initTime);
	return bestValuesofar;
}
