#include "stdafx.h"
#include <assert.h>
#include <string>
#include "compress.h"
#include "FMQ.h"
#include<fstream>
#include <iostream>
#include "FM.h"
#include <vector>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;


void dataread(ifstream &in, char *s, int &k)
{
	k = 0;
	char ch;
	while (in >> ch)
	{
		s[k] = ch;
		k++;
	}
}

u64 data_read(const char *file, int total, vector<char *> &data, int &len)
{
	FILE * fp = fopen(file, "r+"); assert(fp);
	fseek(fp, 0, SEEK_END);
	u64 size = ftell(fp);
	rewind(fp);

	assert(size % total == 0);
	len = size / total;
	data.clear(); data.resize(total, NULL);

	for (int i = 0; i < total; i++)
	{
		char *text_data = new char[len + 1];
		memset(text_data, 0, sizeof(char) * (len + 1)); //append a char '0' 
		fread(text_data, sizeof(char), len, fp);
		data[i] = text_data;
	}
	return size;
}

int main(int argc, char *argv[])
{
	
	int para = argc;
	if(argc == 5) //build index 
	{
		char *file = argv[1];
		char *fileref = argv[2];
		int total = atoi(argv[3]);
		int q = atoi(argv[4]);
		vector<char *> text_data;
		int len;
		u64 size = data_read(file, total, text_data, len);
		vector<compress *> Index(total - 1, NULL);
		assert(len);
		//---------------------------------------------------------------------------
		cout << "-------------begin compress-----------" << endl;
		cout << endl;

		char *ref = text_data[0];   
		FM * RR = new FM(fileref, 1);
		u64 compress_ratio = 0;
		u64 p_radio = 0;
		u64 c_radio = 0;
		u64 f_radio = 0;
		u64 sc_radio = 0;
		u64 brank_radio = 0;
		u64 all0_radio = 0;
		u64 loc_radio = 0;

		int diffnum = 0;
		clock_t st = clock();
		for (int i = 1; i < total; i++)
		{
			Index[i - 1] = new compress(ref, text_data[i], fileref, len, len, q);
			Index[i - 1]->mapKorea();
			Index[i - 1]->connect();
			Index[i - 1]->compreMethod();
			Index[i - 1]->compReference();
			if (i == 1)
			{
				compress_ratio += Index[i - 1]->REF->sizeInByte();
			}
			compress_ratio += Index[i - 1]->sizeInByte1();
			p_radio += Index[i - 1]->sizeInByteP();
			c_radio += Index[i - 1]->sizeInByteC();
			f_radio += Index[i - 1]->sizeInByteF();
			sc_radio += Index[i - 1]->sizeInByteSC();
			brank_radio += Index[i - 1]->sizeInByteBrank();
			diffnum += Index[i - 1]->diffnum;
		}
		clock_t en = clock();
		cout << "build time " << (en - st) * 1.0 / CLOCKS_PER_SEC << endl;
		cout << "compress over" << endl;
		//---------------------------------------------------------------------------------
		cout << "diffnum: " << diffnum << endl;
		cout << "filename:" << file << endl;
		cout << "b:" << Index[0]->blockSize << " sb:" << Index[0]->superBlockSize
			<< " q:" << Index[0]->q << endl;
		cout << "compress ratio: " << compress_ratio * 1.0 / size << endl;
		cout << "p radio:" << p_radio * 1.0 / compress_ratio << endl;
		cout << "c radio:" << c_radio * 1.0 / compress_ratio << endl;
		cout << "f radio:" << f_radio * 1.0 / compress_ratio << endl;
		cout << "ref radio:" << Index[0]->REF->sizeInByte() * 1.0 / compress_ratio << endl;
		cout << "sc radio:" << sc_radio * 1.0 / p_radio << endl;
		cout << "brank radio:" << brank_radio * 1.0 / p_radio << endl;
		cout << "compression index(Byte)" << compress_ratio << endl;
		cout << "p index(MByte)" << p_radio * 1.0 / (1024 * 1024) << endl;
		cout << "C index(MByte)" << c_radio * 1.0 / (1024 * 1024) << endl;
		cout << "F index(MByte)" << f_radio * 1.0 / (1024 * 1024) << endl;
		cout << "SC index(MByte)" << sc_radio * 1.0 / (1024 * 1024) << endl;
		cout << "Brank index(MByte)" << brank_radio*1.0 / (1024 * 1024) << endl;
	}
	
	else if(para == 6)
	{
		char *file = argv[1];
		char *fileref = argv[2];
		int total = atoi(argv[3]);
		char *patternfile = argv[4];
		int q = atoi(argv[5]);

		ifstream fin(patternfile, ios::in);
		vector<char *> patterns;
	
		string tmp;	
		int plen, pnums = 0;
		while (fin >> tmp)
		{		
			plen = tmp.size();
			char *p = new char [plen + 1]; memcpy(p, tmp.c_str(), plen); p[plen] = '\0';
			patterns.push_back(p);	
			pnums++;
		}
		
		fin.close();
		assert(plen == strlen(patterns[0]));
		
		//---------------------------------------------------------------------------
		vector<char *> text_data;
		int len;
		u64 size = data_read(file, total, text_data, len);
		vector<compress *> Index(total - 1, NULL);
		assert(len);



		//---------------------------------------------------------------------------
		cout << "-------------begin compress-----------" << endl;
		cout << endl;
		
		char *ref = text_data[0];   
		FM * RR = new FM(fileref, 1);
		u64 compress_ratio = 0;
		u64 p_radio = 0;
		u64 c_radio = 0;
		u64 f_radio = 0;
		u64 sc_radio = 0;
		u64 brank_radio = 0;
		u64 all0_radio = 0;
		u64 loc_radio = 0;

		int diffnum = 0;
		clock_t st = clock();
		for (int i = 1; i < total; i++)
		{
			Index[i - 1] = new compress(ref, text_data[i], fileref, len, len, q);
			Index[i - 1]->mapKorea();
			Index[i - 1]->connect();
			Index[i - 1]->compreMethod();
			Index[i - 1]->compReference();
			if (i == 1)
			{
				compress_ratio += Index[i - 1]->REF->sizeInByte();
			}
			compress_ratio += Index[i - 1]->sizeInByte1();
			p_radio += Index[i - 1]->sizeInByteP();
			c_radio += Index[i - 1]->sizeInByteC();
			f_radio += Index[i - 1]->sizeInByteF();
			sc_radio += Index[i - 1]->sizeInByteSC();
			brank_radio += Index[i - 1]->sizeInByteBrank();
			diffnum += Index[i - 1]->diffnum;
		}
		clock_t en = clock();
		cout << "build time " << (en - st) * 1.0 / CLOCKS_PER_SEC << endl;
		cout << "compress over" << endl;
		//---------------------------------------------------------------------------------
		cout << "diffnum: " << diffnum << endl;
		cout << "filename:" << file << endl;
		cout << "b:" << Index[0]->blockSize << " sb:" << Index[0]->superBlockSize
			<< " q:" << Index[0]->q << endl;
		cout << "compress ratio: " << compress_ratio * 1.0 / size << endl;
		cout << "p radio:" << p_radio * 1.0 / compress_ratio << endl;
		cout << "c radio:" << c_radio * 1.0 / compress_ratio << endl;
		cout << "f radio:" << f_radio * 1.0 / compress_ratio << endl;
		cout << "ref radio:" << Index[0]->REF->sizeInByte() * 1.0 / compress_ratio << endl;
		cout << "sc radio:" << sc_radio * 1.0 / p_radio << endl;
		cout << "brank radio:" << brank_radio * 1.0 / p_radio << endl;
		cout << "compression index(Byte)" << compress_ratio << endl;
		cout << "p index(MByte)" << p_radio * 1.0 / (1024 * 1024) << endl;
		cout << "C index(MByte)" << c_radio * 1.0 / (1024 * 1024) << endl;
		cout << "F index(MByte)" << f_radio * 1.0 / (1024 * 1024) << endl;
		cout << "SC index(MByte)" << sc_radio * 1.0 / (1024 * 1024) << endl;
		cout << "Brank index(MByte)" << brank_radio*1.0 / (1024 * 1024) << endl;

		
		cout << "--------------begin locate---------------" << endl;
		cout << endl;
		
		if(q < plen)
		{	
			clock_t start = clock();
			long long allnums = 0;
			for (int i = 0; i < pnums; i++)
			{
				char * pattern = patterns[i];
				//cout << i << " " << pattern << endl;
				int flagnums = 0;
				int pattnum = 0;
				string * patts = Index[0]->split(pattern, plen, pattnum);
				int * pos_nums = new int[pattnum];
				vector<int *> tables;
				int * p1 = RR->locating((char *)patts[0].c_str(), pos_nums[0]);
				for (int i = 1; i < pattnum; i++)
				{
					int * tmp = RR->locating((char *)patts[i].c_str(), pos_nums[i]);      
					tables.push_back(tmp);
				}
				for (int j = 0; j < total - 1; j++)
				{
					int nnum = 0;
					vector<int> resultpos;
					Index[j]->locate3add(p1, patts, pattnum, tables, nnum, pos_nums, resultpos);
					allnums += nnum;
				}
				for (int k = 0; k < tables.size(); k++)
				{
					if (tables[k]) delete[] tables[k];
				}
				if (p1) delete[] p1;
			}

			int tmpnums = 0;
			for (int i = 0; i < pnums; i++)
			{
				Index[0]->REF->locating(patterns[i], tmpnums);
				allnums += tmpnums;
			}
			clock_t end = clock();
			int alltime = end - start;
			cout << "#occ:" << allnums << endl;
			cout << "Total time (s):" << alltime * 1.0 / CLOCKS_PER_SEC  << endl;
			cout << "Average time/occ (us):" << alltime * 1000000.0/ (CLOCKS_PER_SEC * allnums) << endl;
		}
		else 
		{

			clock_t start = clock();
			long long  allnums = 0;
			//cout << pnums << " " << plen << endl;	
			for (int i = 0; i < pnums; i++)
			{
				char * pattern = patterns[i];
				int flagnums = 0;
				int * pos1 = RR->locating(patterns[i], flagnums);
				vector<int> resultpos;
				for (int j = 0; j < total - 1; j++)
				{
					int nnum = 0;
					Index[j]->locatingadd(patterns[i], plen, nnum, pos1, flagnums, resultpos);
					allnums += nnum;
				}
				if (pos1) delete[] pos1;
			}
			int tmpnums = 0;
			for (int i = 0; i < pnums; i++)
			{
				Index[0]->REF->locating(patterns[i], tmpnums); //meizhuanhuan zhiqian 
				allnums += tmpnums;
			}
			clock_t end = clock();
			int alltime = end - start;
			cout << "#occ:" << allnums << endl;
			cout << "Total time (s):" << alltime * 1.0 / CLOCKS_PER_SEC  << endl;
			cout << "Average time/occ (us):" << alltime * 1000000.0/ (CLOCKS_PER_SEC * allnums) << endl;
		}
	}
	else if(para == 7)
	{
		char *file = argv[1];
		char *fileref = argv[2];
		int total = atoi(argv[3]);
		char *position_file = argv[4];
		int plen = atoi(argv[5]);
		int q = atoi(argv[6]);
	
		ifstream fin(position_file, ios::in);
		vector<int > position;
		
		int tmp = 0;
		while (fin >> tmp)
		{		
			position.push_back(tmp);
		}
		
		fin.close();
		
		//---------------------------------------------------------------------------
		vector<char *> text_data;
		int len;
		u64 size = data_read(file, total, text_data, len);
		vector<compress *> Index(total - 1, NULL);
		assert(len);



		//---------------------------------------------------------------------------
		cout << "-------------begin compress-----------" << endl;
		cout << endl;
		
		char *ref = text_data[0];   
		FM * RR = new FM(fileref, 1);
		u64 compress_ratio = 0;
		u64 p_radio = 0;
		u64 c_radio = 0;
		u64 f_radio = 0;
		u64 sc_radio = 0;
		u64 brank_radio = 0;
		u64 all0_radio = 0;
		u64 loc_radio = 0;

		int diffnum = 0;
		clock_t st = clock();
		for (int i = 1; i < total; i++)
		{
			Index[i - 1] = new compress(ref, text_data[i], fileref, len, len, q);
			Index[i - 1]->mapKorea();
			Index[i - 1]->connect();
			Index[i - 1]->compreMethod();
			Index[i - 1]->compReference();
			if (i == 1)
			{
				compress_ratio += Index[i - 1]->REF->sizeInByte();
			}
			compress_ratio += Index[i - 1]->sizeInByte1();
			p_radio += Index[i - 1]->sizeInByteP();
			c_radio += Index[i - 1]->sizeInByteC();
			f_radio += Index[i - 1]->sizeInByteF();
			sc_radio += Index[i - 1]->sizeInByteSC();
			brank_radio += Index[i - 1]->sizeInByteBrank();
			diffnum += Index[i - 1]->diffnum;
		}
		clock_t en = clock();
		cout << "build time " << (en - st) * 1.0 / CLOCKS_PER_SEC << endl;
		cout << "compress over" << endl;
		//---------------------------------------------------------------------------------
		cout << "diffnum: " << diffnum << endl;
		cout << "filename:" << file << endl;
		cout << "b:" << Index[0]->blockSize << " sb:" << Index[0]->superBlockSize
			<< " q:" << Index[0]->q << endl;
		cout << "compress ratio: " << compress_ratio * 1.0 / size << endl;
		cout << "p radio:" << p_radio * 1.0 / compress_ratio << endl;
		cout << "c radio:" << c_radio * 1.0 / compress_ratio << endl;
		cout << "f radio:" << f_radio * 1.0 / compress_ratio << endl;
		cout << "ref radio:" << Index[0]->REF->sizeInByte() * 1.0 / compress_ratio << endl;
		cout << "sc radio:" << sc_radio * 1.0 / p_radio << endl;
		cout << "brank radio:" << brank_radio * 1.0 / p_radio << endl;
		cout << "compression index(Byte)" << compress_ratio << endl;
		cout << "p index(MByte)" << p_radio * 1.0 / (1024 * 1024) << endl;
		cout << "C index(MByte)" << c_radio * 1.0 / (1024 * 1024) << endl;
		cout << "F index(MByte)" << f_radio * 1.0 / (1024 * 1024) << endl;
		cout << "SC index(MByte)" << sc_radio * 1.0 / (1024 * 1024) << endl;
		cout << "Brank index(MByte)" << brank_radio*1.0 / (1024 * 1024) << endl;
		
		
		cout << "--------------begin extract---------------" << endl;
		cout << endl;
		
		st = clock();	
		for (int i = 0; i < position.size(); i++)
		{
			int ttt = min(position[i], 1024 * 1024 - 100);
			for (int j = 0; j < total - 1; j++)
			{
				Index[j]->extract(ttt, plen);
			}
		}
		en = clock();
		cout << "extract speed (MB/s)" << plen * 1.0 *  (total - 1) * position.size() * CLOCKS_PER_SEC /(1024.0 * 1024.0 * (en - st)) << endl;		
		return 0;
	}
	else 
	{
		cout << "input format: \n" 
			 << "1. compress: Geomone ref num q \n" 
			 << "2. locate: Geomone ref num pattern q\n"
			 << "3. extract: Geomone ref num position len q\n"
			 << endl;
	}
	return 0;
	
}
