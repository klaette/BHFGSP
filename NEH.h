#pragma once
//NEH.h

#include "BHGFSP_Process.h"

using namespace std;

void NEHInit(vector <int>& chrom) //the largest overall processing time
{
	Pair* ch = new Pair[numOfFamiLy];
	for (int j = 0; j < numOfFamiLy; j++)
	{
		ch[j].dim = j;
		ch[j].value = 0;
		for (int k = 0; k < jobSeqInFam[j].size(); k++)
		{
			for (int s = 0; s < numOfStage; ++s)
			{
				ch[j].value += processTime[s][jobSeqInFam[j][k]];
			}
		}
	}

	sort(ch, ch + numOfFamiLy, PairGreater()); //按每个family中所有工件各自的总加工时间降序排列
	for (int j = 0; j < numOfFamiLy; j++)
		chrom[j] = ch[j].dim;
	delete[]ch;
}

int NEHInsert(vector <int> Oldchrom, vector <int>& Newchrom)
{
	int h, min, i, j, pt;
	int tempch;
	vector <int> PartSeq(numOfFamiLy, 0);

	Newchrom[0] = Oldchrom[0];

	for (h = 1; h < numOfFamiLy; h++)
	{
		tempch = Oldchrom[h];
		min = INT_MAX;
		pt = -1;

		for (i = 0; i <= h; i++)
		{
			for (j = 0; j < i; j++)
				PartSeq[j] = Newchrom[j];

			PartSeq[j] = tempch;
			for (j++; j <= h; j++)
				PartSeq[j] = Newchrom[j - 1];

			int tempTF = GetSpan(PartSeq, jobSeqInFam);

			if (tempTF < min)
			{
				pt = i;
				min = tempTF;
			}
		}

		// insert ch in position pt
		for (i = h; i > pt; i--)
			Newchrom[i] = Newchrom[i - 1];
		Newchrom[pt] = tempch;
	}
	return min;
}

int NEH(vector <int>& Seq)
{
	vector <int> TempSeq(numOfFamiLy);
	NEHInit(TempSeq);
	int TF = NEHInsert(TempSeq, Seq);
	return TF;
}

void SPT(vector <int>& chrom) //the largest overall processing time
{
	Pair* ch = new Pair[numOfFamiLy];
	for (int j = 0; j < numOfFamiLy; j++)
	{
		ch[j].dim = j;
		ch[j].value = 0;
		for (int k = 0; k < numOfStage; k++)
		{
			ch[j].value += processTime[j][k];
		}
	}

	sort(ch, ch + numOfFamiLy, PairLess()); //按每个工件各自的总加工时间升序排列
	for (int j = 0; j < numOfFamiLy; j++)
		chrom[j] = ch[j].dim;
	delete[]ch;
}

void BlockingJobSwapInit(vector <int>& chrom) //the largest overall blocking time
{
	Pair* blocking = new Pair[numOfJob];
	for (int j = 0; j < numOfJob; j++)
	{
		blocking[j].dim = j;
		blocking[j].value = 0;
		blocking[j].value = chrom[j];
		
	}

	sort(blocking, blocking + numOfJob, PairGreater()); //按每个job在所有阶段各自的阻塞时间加和降序排列
	for (int j = 0; j < numOfJob; j++)
		chrom[j] = blocking[j].dim;
	delete[]blocking;
}