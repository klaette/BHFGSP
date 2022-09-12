// BHGFSP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#pragma warning(disable:4996);
#include <iostream>
#include <Windows.h>
#include "BHGFSP_Process.h"
//#include "GA.h"
//#include "SA.h"
//#include "ILS.h"
//#include "IGA.h"
//#include "IPSO.h"
//#include "IGDLM.h"
#include "IGS.h"
//#include "NIG.h"
#include <vector>
#include <sysinfoapi.h>
using namespace std;


void test(int nr, int ins, int CPU, int InJob, int InStage, int InFamily)
{

	int TimeLimit = numOfFamiLy * numOfStage * CPU;
	int bestValuesofar;

	for (int rep = 0; rep < nr; rep++)  
	{
		long initTime;
		initTime = GetTickCount64();
		srand(100 + rep);
		cout << rep << ":";
		//bestValuesofar = GA(200, 0.8, 0.1, ins, TimeLimit);
		//bestValuesofar = SA(200, ins, TimeLimit, 0.95);
		//bestValuesofar = IPSO(200, ins, TimeLimit);
		//bestValuesofar = ILS(ins, TimeLimit);
		//bestValuesofar = IGA(ins, TimeLimit,3);
		//bestValuesofar = IGDLM(ins, TimeLimit);
		  bestValuesofar = IGS(ins, TimeLimit, 6);
		//bestValuesofar = NIG(ins, TimeLimit,3);
		fprintf(p, "%d", bestValuesofar);
		fprintf(p, "\t");
		cout << "完工时间:" << "\t" << bestValuesofar << endl;
	}
	

}

void main01(int CPU)
{

	for (int Ins = 0; Ins < 108; Ins++)   //100 instances
	{
		int InPara = Ins;
		int InJob = Jobs[InPara / 9];
		int InStage = Stages[InPara % 3];
		int InFamily = families[Ins];

		char fileName[30];
		//sprintf(fileName, "GA_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);
		//sprintf(fileName, "SA_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);
		//sprintf(fileName, "DPSO_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);
		//sprintf(fileName, "ILS_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);
		//sprintf(fileName, "IGA_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);
		//sprintf(fileName, "IGDLM_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);
		//sprintf(fileName, "NIG_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);
		  sprintf(fileName, "IGS_J%dS%dF%d_t=%d.txt", InJob, InStage, InFamily, CPU);

		p = fopen(fileName, "a+");

		GenerateInstances(InJob, InStage, InFamily, Ins + 1234); //generate instances

		cout << "工件数：" << InJob << " 阶段数：" << InStage << " 组数：" << InFamily << endl;
		test(30, Ins, CPU, InJob, InStage, InFamily);   //test algorithm
		fprintf(p, "\n");
		fclose(p);
	}

	cout << "___End____" << endl;

}

void main()
{
	main01(100);
	//main01(7);
	//main01(9);
	//main01(20);
	//main01(30);
}
