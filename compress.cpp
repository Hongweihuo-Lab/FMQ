#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include "FM.h"
#include<iostream>
#include "compress.h"
#include <ctime>
#include<fstream>
#include "perfectHash.h"
#include <sstream>
#include <algorithm>
using namespace std;

#define DIFF 160000
int * * LCS(string x, string y, int * * b)
{
	int l = x.size();
	int * * M = new int *[l + 1];
	for (int k = 0; k < l + 1; k++)
	{
		M[k] = new int[l + 1];
	}
	for (int i = 0; i < l + 1; i++)
	{
		M[i][0] = 0;
	}
	for (int j = 0; j < l + 1; j++)
	{
		M[0][j] = 0;
	}
	for (int i = 1; i < l + 1; i++)
		for (int j = 1; j < l + 1; j++)
		{
			if (x[i - 1] == y[j - 1])
			{
				M[i][j] = M[i - 1][j - 1] + 1;
				b[i][j] = 2;
			}
			else
			{
				if (M[i - 1][j] > M[i][j - 1])
				{
					M[i][j] = M[i - 1][j];
					b[i][j] = 0;
				}
				else
				{
					M[i][j] = M[i][j - 1];
					b[i][j] = 1;
				}
			}
		}
	return M;
}
int * * LCS2(string x, string y, int * * b,int num)
{
	int l = num;
	int * * M = new int *[l + 1];
	for (int k = 0; k < l + 1; k++)
	{
		M[k] = new int[l + 1];
	}
	for (int i = 0; i < l + 1; i++)
	{
		M[i][0] = 0;
	}
	for (int j = 0; j < l + 1; j++)
	{
		M[0][j] = 0;
	}
	for (int i = 1; i < l + 1; i++)
		for (int j = 1; j < l + 1; j++)
		{
			if (x[i - 1] == y[j - 1])
			{
				M[i][j] = M[i - 1][j - 1] + 1;
				b[i][j] = 2;
			}
			else
			{
				if (M[i - 1][j] > M[i][j - 1])
				{
					M[i][j] = M[i - 1][j];
					b[i][j] = 0;
				}
				else
				{
					M[i][j] = M[i][j - 1];
					b[i][j] = 1;
				}
			}
		}
	return M;
}
const char* itoa1(int val)
{
	static char result[sizeof(int) << 3 + 2];
	unsigned int tempval = val;
	if (val<0) tempval = -val;
	int i = sizeof(int) << 3 + 1;
	do{
		result[i] = "0123456789"[tempval % 10];
		tempval /= 10; --i;
	} while (tempval);
	if (val<0) result[i--] = '-';
	return &result[i + 1];
}
void compress::find(int * * M, string y, int i, int j, int l)
{
	if (i == 0 && j == 0)
	{
		return;
	}
	if (i == 0)
	{
		diffnum++;
		int hh = l - q;
		char * tmp = (char *) malloc(sizeof(char)*(2 * q + 2));
		for (int k = 0; k < 2 * q + 1; k++)
		{
			tmp[k] = s2[hh++];
		}
		tmp[2 * q + 1] = '\0';
		chashannum++;
		return;
	}
	if (j == 0)
	{
		diffnum++;
		int hh = l - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		for (int k = 0; k < 2 * q + 1; k++)
		{
			tmp[k] = s2[hh++];
		}
		tmp[2 * q + 1] = '\0';
		chashannum++;
		return;
	}
	if (M[i][j] == M[i - 1][j] && M[i][j] == M[i][j - 1])
	{
		diffnum++;
		int hh = l +j-1-q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		find(M, y, i - 1, j - 1, l);
	}
	else if (M[i][j] == M[i - 1][j])
	{
		diffnum++;
		int hh = l +j-1-q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		for (int k = 0; (k < 2 * q + 1); k++)
		{
			tmp[k] = s2[hh++];
		}
		tmp[2 * q + 1] = '\0';
		chashannum++;
		find(M, y, i - 1, j, l);
	}
	else if (M[i][j] == M[i][j - 1])
	{
		diffnum++;
		int hh = l +j-1-q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		for (int k = 0; k < 2 * q + 1; k++)
		{
			tmp[k] = s2[hh++];
		}
		tmp[2 * q + 1] = '\0';
		chashannum++;
		find(M, y, i, j - 1, l);
	}
	else
	{
		find(M, y, i - 1, j - 1, l);
	}
}
void compress::find2Globol(int * * M, int * * b, string s2, int i, int j, int l)
{
	if (i == 0 && j == 0)
	{
		return;
	}
	if (i == 0)
	{
		BV[l] = 1;
		int hh = l - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else 
		{
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		if (tmp) free(tmp);
		chashannum++;
		diffnum++;
		return;
	}
	if (j == 0)
	{
		BV[l + i - 1] = 1;
		int hh = l - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		if (tmp) free(tmp);
		chashannum++;
		diffnum++;
		return;
	}
	if (M[i][j] == M[i - 1][j - 1] + 1 && b[i][j] == 2)
	{
		find2(M, b, s2, i - 1, j - 1, l);
	}
	else if (M[i][j] == M[i - 1][j] && M[i][j] == M[i][j - 1])
	{
		BV[l + i - 1] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		if (tmp) free(tmp);	
		find2(M, b, s2, i - 1, j - 1, l);
	}
	else if (M[i][j] == M[i - 1][j] && b[i][j] == 0)
	{
		BV[l + i - 1] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		if (tmp) free(tmp);	
		chashannum++;
		diffnum++;
		find2(M, b, s2, i - 1, j, l);
	}
	else if (M[i][j] == M[i][j - 1] && b[i][j] == 1)
	{
		BV[l + i] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		if (tmp) free(tmp);
		chashannum++;
		diffnum++;
		find2(M, b, s2, i, j - 1, l);
	}
}
void compress::find2(int * * M, int * * b,string s2, int i, int j, int l)
{
	if (i == 0 && j == 0)
	{
		return;
	}
	if (i == 0)
	{
		BV[l] = 1;
		int hh = l - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(1);
		chashannum++;
		return;
	}
	if (j == 0)
	{
		BV[l + i - 1] = 1;
		int hh = l - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(2);
		chashannum++;
		return;
	}
	if (M[i][j] == M[i - 1][j - 1] + 1 && b[i][j] == 2)
	{
		find2(M,b, s2, i - 1, j - 1, l);
	}
	else if (M[i][j] == M[i - 1][j] && M[i][j] == M[i][j - 1])
	{
		BV[l + i - 1] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		//if (tmp) free(tmp);
		//C[diffnum] = tmp;
		tempF.push(0);
		//F[diffnum] = 0;
		//cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",sub)" << endl;
		//diffnum++;
		find2(M, b,s2, i - 1, j - 1, l);
	}
	else if (M[i][j] == M[i - 1][j] && b[i][j]==0)
	{
		BV[l + i - 1] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		//if (tmp) free(tmp);
		//C[diffnum] = tmp;
		tempF.push(2);
		//F[diffnum] = 2;
		chashannum++;
		//cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",del)" << endl;
		//diffnum++;
		find2(M, b,s2, i - 1, j, l);
	}
	else if (M[i][j] == M[i][j - 1]&& b[i][j]==1)
	{
		BV[l + i] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		//C[diffnum] = tmp;
		//if (tmp) free(tmp);
		tempF.push(1);
		//F[diffnum] = 1;
		chashannum++;
		//cout << "(p,c,f)=(" << l + i << "," << tmp << ",ins)" << endl;
		//diffnum++;
		find2(M,b, s2, i, j - 1, l);
	}
}
void compress::find3(int * * M, int * * b, string y, int i, int j, int l)
{
	if (i == 0 && j == 0)
	{
		return;
	}
	if (i == 0)
	{
		BV[l] = 1;
		int hh = l - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		//if (tmp) free(tmp);
		//C[diffnum] = tmp;
		tempF.push(1);
		//F[diffnum] = 0;
		chashannum++;
		if (j == 1) 
		{
			cout << "(p,c,f)=(" << l << "," << tmp << ",ins)" << endl;
		}
		else
		{
			cout << "(p,c,f)=(" << l << "," << tmp << ",mul)"<< j << endl;
		}
		//diffnum++;
		return;
	}
	if (j == 0)
	{
		BV[l + i - 1] = 1;
		int hh = l - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(2);
		chashannum++;
		cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",del)" << endl;
		find3(M, b, y, i - 1, j, l);
		//return;
	}
	if (M[i][j] == M[i - 1][j - 1] + 1 && b[i][j] == 2)
	{
		find3(M, b, y, i - 1, j - 1, l);
	}
	else if (M[i][j] == M[i - 1][j] && M[i][j] == M[i][j - 1])
	{
		BV[l + i - 1] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(0);
		cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",sub)" << endl;
		find3(M, b, y, i - 1, j - 1, l);
	}
	else if (M[i][j] == M[i - 1][j] && b[i][j] == 0)
	{
		BV[l + i - 1] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(2);
		chashannum++;
		cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",del)" << endl;
		find3(M, b, y, i - 1, j, l);
	}
	else if (M[i][j] == M[i][j - 1] && b[i][j] == 1)
	{
		if (BV[l + i] == 1)
		{
			testmul++;
		}
		BV[l + i] = 1;
		int hh = l + j - 1 - q;
		char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
		if (hh < 0)
		{
			hh = 0 - hh;
			int d = 0;
			for (int k = 0; k < hh; k++)
			{
				tmp[k] = '-';
			}
			for (int k = hh; k < 2 * q - 1; k++)
			{
				tmp[k] = s2[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(1);
		chashannum++;
		cout << "(p,c,f)=(" << l + i << "," << tmp << ",ins)" << endl;
		find3(M, b, y, i, j - 1, l);
	}
}
compress::compress(char * s1, char * s2,char * filename, int m1, int m2, int q)
{
	Filename = filename;
	this->s1 = s1;
	this->s2 = s2;
	this->max1 = m1;
	this->max2 = m2;
	blockSize = 2048;
	superBlockSize = blockSize * 16;
	this->q = q;
	BV = new int[max1];
	for (int k = 0; k < max1; k++)
	{
		BV[k] = 0;
	}
	C = new char *[DIFF];
	for (int k = 0; k < DIFF; k++)

	{
		C[k] = new char[2*q+1];
	}
	F = new int[DIFF];
}

compress::compress(char * s1, char * s2,char * filename, int m1, int m2)
{
	Filename = filename;
	this->s1 = s1;
	this->s2 = s2;
	max1 = m1;
	max2 = m2;
	blockSize = 2048;
	superBlockSize = blockSize * 16;
	q = 5;
	BV = new int[max1];
	for (int k = 0; k < max1; k++)
	{
		BV[k] = 0;
	}
	C = new char *[DIFF];
	for (int k = 0; k < DIFF; k++)

	{
		C[k] = new char[2*q+1];
	}
	F = new int[DIFF];
}
compress::compress(char * s1, char * s2, int blockSize, int superBlockSize, int q)
{
	this->s1 = s1;
	this->s2 = s2;
	max1 = 1024 * 1024;
	max2 = 1024 * 1024;
	this->blockSize = blockSize;
	this->superBlockSize = superBlockSize;
	this->q = q;
	BV = new int[max1];
}
compress::compress(const char * filename, int nums, int length)
{
	max1 = length;
	max2 = length;
	blockSize = 3000;
	superBlockSize = 21000;
	q = 5;
	DNAnums = nums;
	ifstream fin(filename, ios::in);                 //sequence
	reference = new char[length+1];
	fin.read(reference, length);
	reference[length] = '\0';
	REF = new FM("D:\\dna1.txt",1);
	DNA = new sequence[nums - 1];
	s1 = new char[length + 1];
	s1 = reference;
	//this->s1 = reference;
	for (int i = 0; i < nums - 1; i++)
	{
		BV = new int[length];
		for (int k = 0; k <length; k++)
		{
			BV[k] = 0;
		}
		C = new char *[DIFF];
		for (int k = 0; k < DIFF; k++)
		{
			C[k] = new char[2 * q + 1];
		}
		F = new int[DIFF];
		char * tmpDNA = new char[length + 1];

		fin.read(tmpDNA, length);
		tmpDNA[length] = '\0';
		//fin.read(tmpDNA, length);
		//this->s2 = tmpDNA;
		DNA[i].diffnum = mapmul(reference, tmpDNA);
		//连接上下文
		connect();              //得到Context
		compreMethod2();
		DNA[i].CC = CC;
		DNA[i].Brank  = Brank;
		DNA[i].FF = FF;
		DNA[i].Header = Header;
		DNA[i].SBrank = SBrank;
		DNA[i].S = S;
		DNA[i].V = V;		
	}
}
void compress::map()
{
	int j = 0;
	int l = 0;
	int r = 0;
	diffnum = 0;
	//cout << "test2" << endl;
	int jilu = 0;
	int jilu2 = 0;
	while (j<max1)
	{
		while (s1[j] == s2[j] && j < max1)
		{
			j++;		
		}
		l = j;
		if (j == max1)
		{
			break;
		}
		while (s1[j] != s2[j] && j < max1)
		{
			j++;			
		}
		//if (j >= max1 - 1) break;
		r = j;	
		char * x = (char *)malloc(sizeof(char)*(r - l + 1));
		char * y = (char *)malloc(sizeof(char)*(r - l + 1));
		int kk = 0;
		for (int k = l; k < r; k++)
		{
			x[kk] = s1[k];
			y[kk] = s2[k];
			kk = kk + 1;
		}
		//cout << "判断的位置j: "<<j<<"   l: " << l << "  r:" << r << endl;
		x[kk] = '\0';
		y[kk] = '\0';
		//cout << "x=" << x << endl;
		//cout << "y=" << y << endl;
		//cout << "over";
		int t = r - l;
		if (t == 1)
		{
			jilu2++;
			BV[l] = 1;
			int hh = l - q;
			char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
			tmp[2 * q + 1] = '\0';
			//cout << "(p,c,f)=(" << l << "," << tmp << ",sub)" << endl;
			C[diffnum] = tmp;
			F[diffnum] = 0;
			//F[diffnum] = 0;
			diffnum++;
		}
		else
		{
			if (t > 15) {
				//cout << "x=" << x << endl;
				//cout << "y=" << y << endl;
				jilu++;
			}
			int * * M = new int *[t + 1];
			for (int k = 0; k < t + 1; k++)
			{
				M[k] = new int[t + 1];
			}
			int * * b = new int *[t + 1];
			for (int k = 0; k < t + 1; k++)
			{
				b[k] = new int[t + 1];
			}
			M = LCS(x, y, b);
	
			/*	if (M[t][t] == 1&&t>2)
				{
					cout << "x=" << x << endl;
					cout << "y=" << y << endl;
				}
		*/
			find2(M, b, s2, t, t, l);
			while (!tempC.empty())
			{
				char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
				tmp = tempC.top();
				tempC.pop();
				//C[diffnum] = tmp; 
				memcpy(C[diffnum], tmp, sizeof(char) * (2 * q + 2));
				F[diffnum] = tempF.top();
				tempF.pop();
				diffnum++;	
				if (tmp) free(tmp);
			}
			if (x) free(x); if (y) free(y);
			for (int k = 0; k < t + 1; k++)
				if (M[k]) delete[] M[k];
			if (M) delete[] M;
		}
		//cout << "差异个数:" << diffnum << endl;	
	/*	for (int k = 0; k < t + 1; k++)
			if (M[k]) delete[] M[k];
		if (M) delete[] M;*/
	}
	cout << "差异个数:" << diffnum << endl;
}
void compress::mapGlobolKorea(int length)
{
	int * * M = new int *[length+ 1];
	cout << "hds" << endl;
	for (int k = 0; k < length + 1; k++)
	{
		M[k] = new int[length + 1];
	}
	cout << "hds" << endl;
	int * * b = new int *[length + 1];
	for (int k = 0; k < length + 1; k++)
	{
		b[k] = new int[length + 1];
	}
	M = LCS2(s1, s2, b, length);
	cout << "kkk" << endl;
	//find2Globol(M, b, s2, 16 * 1024, 16 * 1024, 0);
}
void compress::mapKorea()
{
	int j = 0;
	int l = 0;
	int r = 0;
	diffnum = 0;
	//cout << "test2" << endl;
	int jilu = 0;
	int jilu2 = 0;
	while (j<max1)
	{
		//cout << j << "  ";
		while (s1[j] == s2[j] && j < max1)
		{
			j++;
		}
		l = j;
		if (j == max1)
		{
			break;
		}
		while (s1[j] != s2[j] && j < max1)
		{
			j++;
		}
		//if (j >= max1 - 1) break;
		r = j;
		char * x = (char *)malloc(sizeof(char)*(r - l + 1));
		char * y = (char *)malloc(sizeof(char)*(r - l + 1));
		int kk = 0;
		for (int k = l; k < r; k++)
		{
			x[kk] = s1[k];
			y[kk] = s2[k];
			kk = kk + 1;
		}
		//cout << "判断的位置j: "<<j<<"   l: " << l << "  r:" << r << endl;
		x[kk] = '\0';
		y[kk] = '\0';
		//cout << "x=" << x << endl;
		//cout << "y=" << y << endl;
		//cout << "over";
		int t = r - l;
		if (t == 1)
		{
			jilu2++;
			BV[l] = 1;
			int hh = l - q;
			char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
			if (hh < 0)
			{
				int k = 0;
				for (; hh <= -1; k++)
				{
					tmp[k] = '#';
					hh++;
				}
				for (k; k < 2 * q + 1; k++)
				{
					tmp[k] = s2[hh++];
				}
				tmp[2 * q + 1] = '\0';
			}
			else 
			{
				for (int k = 0; k < 2 * q + 1; k++)
				{
					tmp[k] = s2[hh++];
				}
				tmp[2 * q + 1] = '\0';
			}
			//cout << "(p,c,f)=(" << l << "," << tmp << ",sub)" << endl;
			C[diffnum] = tmp;
			F[diffnum] = 0;
			//F[diffnum] = 0;
			diffnum++;
		}
		else
		{
			//cout << "erroe" << endl;
			if (t > 15) {
				//cout << "x=" << x << endl;
				//cout << "y=" << y << endl;
				jilu++;
			}
			int * * M = new int *[t + 1];
			for (int k = 0; k < t + 1; k++)
			{
				M[k] = new int[t + 1];
			}
			int * * b = new int *[t + 1];
			for (int k = 0; k < t + 1; k++)
			{
				b[k] = new int[t + 1];
			}
			M = LCS(x, y, b);

			/*	if (M[t][t] == 1&&t>2)
			{
			cout << "x=" << x << endl;
			cout << "y=" << y << endl;
			}
			*/
			
			find2(M, b, s2, t, t, l);
			while (!tempC.empty())
			{
				char * tmp = tempC.top();
				tempC.pop();
				//C[diffnum] = tmp; 
				memcpy(C[diffnum], tmp, sizeof(char) * (2 * q + 2));
				F[diffnum] = tempF.top();
				tempF.pop();
				diffnum++;
				if (tmp) free(tmp);
			}
			if (x) free(x); if (y) free(y);
			for (int k = 0; k < t + 1; k++)
				if (M[k]) delete[] M[k];
			if (M) delete[] M;
			
		}
		//cout << "差异个数:" << diffnum << endl;	
		/*	for (int k = 0; k < t + 1; k++)
		if (M[k]) delete[] M[k];
		if (M) delete[] M;*/
	}
}
int compress::mapmul(char * s1, char * s2)
{
	int j = 0;
	int l = 0;
	int r = 0;
	diffnum = 0;
	//cout << "test2" << endl;
	int jilu = 0;
	int jilu2 = 0;
	while (j<max1)
	{
		while (s1[j] == s2[j] && j < max1)
		{
			j++;
		}
		l = j;
		if (j == max1)
		{
			break;
		}
		while (s1[j] != s2[j] && j < max1)
		{
			j++;
		}
		//if (j >= max1 - 1) break;
		r = j;
		char * x = (char *)malloc(sizeof(char)*(r - l + 1));
		char * y = (char *)malloc(sizeof(char)*(r - l + 1));
		int kk = 0;
		for (int k = l; k < r; k++)
		{
			x[kk] = s1[k];
			y[kk] = s2[k];
			kk = kk + 1;
		}
		//cout << "判断的位置j: "<<j<<"   l: " << l << "  r:" << r << endl;
		x[kk] = '\0';
		y[kk] = '\0';
		//cout << "x=" << x << endl;
		//cout << "y=" << y << endl;
		//cout << "over";
		int t = r - l;
		if (t == 1)
		{
			jilu2++;
			BV[l] = 1;
			int hh = l - q;
			char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = s2[hh++];
			}
			tmp[2 * q + 1] = '\0';
			//cout << "(p,c,f)=(" << l << "," << tmp << ",sub)" << endl;
			C[diffnum] = tmp;
			F[diffnum] = 0;
			//F[diffnum] = 0;
			diffnum++;
		}
		else
		{
			int * * M = new int *[t + 1];
			for (int k = 0; k < t + 1; k++)
			{
				M[k] = new int[t + 1];
			}
			int * * b = new int *[t + 1];
			for (int k = 0; k < t + 1; k++)
			{
				b[k] = new int[t + 1];
			}
			M = LCS(x, y, b);
			find2(M, b, s2, t, t, l);
			while (!tempC.empty())
			{
				char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
				tmp = tempC.top();
				tempC.pop();
				//C[diffnum] = tmp; 
				memcpy(C[diffnum], tmp, sizeof(char) * (2 * q + 2));
				F[diffnum] = tempF.top();
				tempF.pop();
				diffnum++;
				if (tmp) free(tmp);
			}
			if (x) free(x); if (y) free(y);
			for (int k = 0; k < t + 1; k++)
				if (M[k]) delete[] M[k];
			if (M) delete[] M;
		}
	}
	//cout << "差异个数:" << diffnum << endl;
	return diffnum;
}
bool compress::verify()
{
	char * vs2 = (char *)malloc(sizeof(char)*max2+1);
	int t = 0;
	int k = 0;
	/*
	for (int i = 0; i < max1; i++)
	{
		cout << BV[i];
	}
	cout << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << F[i];
	}
	cout << endl;
	*/
	for (int i = 0; i < max1; i++)
	{
		if (BV[i] == 0)
		{
			vs2[k++] = s1[i];
			//cout << s1[i];
		}
		else
		{
			switch (F[t])
			{
			case 0: vs2[k++] = C[t][q];   /*cout << C[t][q]; */ break;
			case 1: vs2[k++] = C[t][q]; vs2[k++] = C[t][q+1]; /*cout << C[t][q]<<C[t][q+1];*/ break;
			case 2: break;
			}
			t++;
		}
	}
	vs2[max2] = '\0';
	for (int i = 0; i < max2; i++)
	{
		if (vs2[i] != s2[i])
		{
			cout << "false" << endl;
			cout << "位置:" << i << "本该是:" << s2[i] << "实际是:" << vs2[i] << endl;
			return false;
		}
	}
	return true;
}
void compress::connect()
{
	Context = (char *)malloc(sizeof(char)*(diffnum*(2 * q + 2) + 2));
	int t = 0;
	for (int i = 0; i < diffnum; i++)
	{
		Context[t++] = '%';
		for (int j = 0; j < 2 * q + 1; j++)
		{
			Context[t++] = C[i][j];
			if (C[i][j] == 'N')
			{
				testallN++;
			}
		}
	}
	Context[diffnum*(2 * q + 2)] = '0';
	Context[diffnum*(2 * q + 2)+1] = '\0';
}
void compress::compreMethod()
{
	u64 all0nums = 0;
	u64 locnums = 0;
	/*上下文的压缩*/
	CC = new FM((unsigned char *)Context, diffnum*(2 * q + 2)+2, 1);
	//CC->getAlphabetSize();
	/* F串用定长InArray编*/
	FF = new InArray(diffnum, 2);
	for (int i = 0; i < diffnum; i++)
	{
		FF->SetValue(i, F[i]);
	}

	/* BV串压缩*/
	
	int n =max1;
	int t = diffnum;
	int bitslength = log(blockSize) / log(2) + 1;
	int blocknum = n / blockSize;
	if (n % blockSize != 0)
	{
		blocknum++;
	}
	int superblocknum = n / superBlockSize;
	if (n % superBlockSize != 0)
	{
		superblocknum++;
	}
	Header = new InArray(blocknum, 1);
	S = new InArray(t, bitslength);
	Brank = new InArray(blocknum, log(superBlockSize - blockSize) / log(2) + 1);
	SBrank = new InArray(superblocknum, log(t) / log(2) + 1);
	V = new InArray(blocknum, 1);

	int ranktemp = 0;
	int superank = 0;
	int flag = 0;
	int blockindex = 0;
	int prerank = 0;
	int superblockindex = 0;
	int k = 0;
	for (int i = 0; i <n; i++)
	{
		if (i%blockSize == 0)
		{
			blockindex = i / blockSize;
			ranktemp += prerank;
			if (blockindex == 0)
			{
				continue;
			}
			if (prerank == 0)
			{
				Header->SetValue(blockindex - 1, 0);
				all0nums++;
			}
			else {
				Header->SetValue(blockindex - 1, 1);
				locnums++;
			}
			if (i%superBlockSize != 0) {
				Brank->SetValue(blockindex, ranktemp);
			}
			prerank = 0;
		}
		if (i%superBlockSize == 0)
		{
			superank += ranktemp;
			superblockindex = i / superBlockSize;
			SBrank->SetValue(superblockindex, superank);
			ranktemp = 0;
			Brank->SetValue(blockindex, ranktemp);
		}
		if (BV[i] == 1)
		{
			prerank++;
			S->SetValue(k, i%blockSize);
			k++;
		}
	}
	if (prerank == 0)
	{
		Header->SetValue(blockindex, 0);
		all0nums++;
	}
	else
	{
		Header->SetValue(blockindex, 1);
		locnums++;
	}	
	//cout << "all0块个数:" << all0nums;
	//cout << "            loc块个数:" << locnums << endl;
}
void compress::compreMethod2()
{
	/*上下文的压缩*/
	CC = new FM((unsigned char *)Context, diffnum*(2 * q + 2) + 2, 1);
	/* F串用定长InArray编*/
	FF = new InArray(diffnum, 2);
	for (int i = 0; i < diffnum; i++)
	{
		FF->SetValue(i, F[i]);
	}

	/* BV串压缩*/

	int n = max1;
	int t = diffnum;
	int bitslength = log(blockSize) / log(2) + 1;
	int blocknum = n / blockSize;
	if (n % blockSize != 0)
	{
		blocknum++;
	}
	int superblocknum = n / superBlockSize;
	if (n % superBlockSize != 0)
	{
		superblocknum++;
	}
	Header = new InArray(blocknum, 1);
	S = new InArray(t, bitslength);
	Brank = new InArray(blocknum, log(superBlockSize - blockSize) / log(2) + 1);
	SBrank = new InArray(superblocknum, log(t) / log(2) + 1);
	V = new InArray(blocknum, 1);
	for (int i = 0; i < blocknum; i++)
	{
		V->SetValue(i, 0);
	}
	int ranktemp = 0;
	int superank = 0;
	int flag = 0;
	int blockindex = 0;
	int prerank = 0;
	int superblockindex = 0;
	int k = 0;
	for (int i = 0; i <n; i++)
	{
		if (i%blockSize == 0)
		{
			blockindex = i / blockSize;
			ranktemp += prerank;
			if (blockindex == 0)
			{
				continue;
			}
			if (prerank == 0)
			{
				Header->SetValue(blockindex - 1, 0);
			}
			else {
				Header->SetValue(blockindex - 1, 1);
			}
			if (i%superBlockSize != 0) {
				Brank->SetValue(blockindex, ranktemp);
			}
			prerank = 0;
		}
		if (i%superBlockSize == 0)
		{
			superank += ranktemp;
			superblockindex = i / superBlockSize;
			SBrank->SetValue(superblockindex, superank);
			ranktemp = 0;
			Brank->SetValue(blockindex, ranktemp);
		}
		if (BV[i] == 1)
		{
			prerank++;
			S->SetValue(k, i%blockSize);
			k++;
		}
	}
	if (prerank == 0)
	{
		Header->SetValue(blockindex, 0);
	}
	else
	{
		Header->SetValue(blockindex, 1);
	}
}
int compress::mappos(int pos)
{
	int bindex = pos / blockSize;
	int sbindex = pos / superBlockSize;
	//int hop = V2[bindex];
	//cout << "块号:" << bindex << endl;
	//cout << "超快号：" << sbindex << endl;
	int hop = V->GetValue(bindex);
	int offset = pos% blockSize;
	//cout << "偏移:" << offset << endl;
	int nums =0;                 //此块中1的个数
	int start = bindex*blockSize;
	int end = start + blockSize - 1;
	int rank1 = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
	//cout << "之前1的个数:" << rank1 << endl;
	int decode = S->GetValue(rank1);
	int prenums = rank1;        //记录当前块已经遍历的1的个数
	int flag = 0;                       //表示超块的最后一块
	if (Header->GetValue(bindex) == 0)
	{
		//cout << "all0" << endl;
		nums = 0;
	}
	else if ((bindex+1) % (superBlockSize / blockSize) == 0)
	{
		nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
		flag = 0;
	}
	else
	{
		nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
		flag = 1;
	}
	//cout << "此块中1的个数:" << nums << endl;
	int limit = prenums + nums;        //在这块中rank1必须小于limit
	//记录在pos前总共需要移动的步数
	while ((decode < offset)&&(rank1<limit))
	{
		if (FF->GetValue(rank1) == 0)                    //subtitution
		{

		}
		else if (FF->GetValue(rank1) == 1)              //insertion
		{
			hop=hop-1;
		}
		else if (FF->GetValue(rank1) == 2)              //delete
		{
			hop++;
		}
		rank1++;
		decode = S->GetValue(rank1);
		//cout << "偏移:" << decode << " ";
	}
	//cout << endl;
	//cout << "要跳的跳数："<<hop << endl;
	if ((decode >offset) && (rank1<limit))
	{
		//证明B[pos]=0
	}
	else if (decode == offset)
	{
		//证明B[pos]=1
	}
	else if (rank1 >= limit)
	{
		//证明B[pos]=0
		//并且这块从上一decode开始的所有都是0
	}
    //利用文档上的进行具体值的跳转
	if (hop == 0)
	{
		return pos;
	}
	//向后跳转
	else if (hop > 0)
    {
		if ((decode >offset) && (rank1<limit))
		{
			//证明B[pos]=0
			if ((decode - offset) > hop)
			{
				return pos + hop;
			}
			else
			{
				pos = pos + decode - offset - 1;
				hop == hop - (decode - offset - 1);
			}
			//此刻pos+1位置是差异
			//表示差异的S中位置是rank1
		}
		else if ((decode == offset) && (rank1<limit))
		{
			//证明B[pos]=1
			if (FF->GetValue(rank1) == 0)  //如果是sub
			{

			}
			else if (FF->GetValue(rank1) == 1)  //如果是ins
			{
				if (hop == 1)
				{
					insflag = 2;
					return pos;   //且为参考序列上的字符
				}
				hop--;
			}
			else if (FF->GetValue(rank1) == 2)   //如果是del
			{
				hop++;
				//需补充
			}
			rank1++;
			decode = S->GetValue(rank1);
			if (rank1 < limit)
			{
				int can2 = decode - offset-1;
				if (hop <= can2)
				{
					return pos + hop;
				}
				else
				{
					pos = pos + can2;
					hop = hop - can2;
				}
			}
			else
			{
				bindex++;           //跳到下一块中
				if (flag == 0)       //证明pos是超块的最后一块
				{
					sbindex++;
				}
				//decode是pos的下一块中1的第一个位置
				//此刻计算pos可以直接跳的位置的多少
				int can = blockSize - offset - 1 + decode;
				if (hop <= can)
				{
					return pos + hop;
				}
				else
				{
					pos = pos + can;
					hop = hop - can;
				}
				//这个时候decode是差异，且位置是rank1;
				//需要求出这块中1的个数nums
				if (Header->GetValue(bindex) == 0)
				{
					nums = 0;
				}
				else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
				{
					nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
					flag = 0;
				}
				else
				{
					nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
					flag = 1;
				}
				limit += nums;
			}
		}
		else if (rank1 >= limit)
		{
			//证明B[pos]=0
			//并且这块从上一decode开始的所有都是0
			bindex++;           //跳到下一块中
			if (flag == 0)       //证明pos是超块的最后一块
			{
				sbindex++;
			}
			//decode是pos的下一块中1的第一个位置
			//此刻计算pos可以直接跳的位置的多少
			int can = blockSize - offset -1+ decode;
			if (hop <= can)
			{
				return pos + hop;
			}
			else
			{
				pos = pos + can;
				hop = hop - can;
			}
			//这个时候decode是差异，且位置是rank1;
			//需要求出这块中1的个数nums
			if (Header->GetValue(bindex) == 0)
			{
				nums = 0;
			}
			else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
			{
				nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
				flag = 0;
			}
			else
			{
				nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
				flag = 1;
			}
			limit+=nums;
		}
		//此刻pos+1是差异位置 也就是decode S[rank1]=decode limit是rank1的限制
		while (hop!=0)
		{
			if(FF->GetValue(rank1)==0)        //sub
			{
				pos=pos+1;
				hop=hop-1;
			}
			else if(FF->GetValue(rank1)==1)    //ins
			{
				if(hop==1)
				{
					insflag = 0;
					return pos+1; //字符时插入的字符
				}
				if(hop==2)
				{
					insflag = 2;
					return pos+1;  //字符时参考序列的字符
				}
				pos=pos+1;
				hop=hop-2;
			}
			else if(FF->GetValue(rank1)==2)  //del
			{
				pos=pos+1;
			}
            //接下来继续求decode
			rank1++;
			decode=S->GetValue(rank1);
			if (rank1 < limit)
			{
				int can2 = decode - offset-1;
				if (hop <= can2)
				{
					return pos + hop;
				}
				else
				{
					pos = pos + can2;
					hop = hop - can2;
				}
			}
			else
			{
				bindex++;           //跳到下一块中
				if (flag == 0)       //证明pos是超块的最后一块
				{
					sbindex++;
				}
				//decode是pos的下一块中1的第一个位置
				//此刻计算pos可以直接跳的位置的多少
				int can = blockSize - offset - 1 + decode;
				if (hop <= can)
				{
					return pos + hop;
			}
				else
				{
					pos = pos + can;
					hop = hop - can;
				}
				//这个时候decode是差异，且位置是rank1;
				//需要求出这块中1的个数nums
				if (Header->GetValue(bindex) == 0)
				{
					nums = 0;
				}
				else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
				{
					nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
					flag = 0;
				}
				else
				{
					nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
					flag = 1;
				}
				limit += nums;
		}
		}
#if 0
		if (decode > offset)                    //说明退出循环的条件是当前块中1的个数还没有遍历完
		{
			if (offset + hop < decode)
			{
				return pos + hop;
			}
			else
			{
				pos = pos + (decode - offset - 1);
				hop = hop - (decode - offset - 1);
			}
		}
		else if (decode == offset)
		{
			if (FF->GetValue(rank1) == 0)      //sub
			{
				cout<<"FF->setValue("")" << endl;
				if (Header->GetValue(rank1) == 0)
				{
					cout << "" << endl;
				}
			}
			else if (FF->GetValue(rank1)==1)   //是ins
			{
				
			}
			else if (FF->GetValue(rank1) == 2)   //del
			{
				pos++;
			}
			else                              //mul
			{
			
			}
			rank1++;
			decode = S->GetValue(rank1);
		}
		else                         //decode<offset  说明rank1为下一块中的1开始的位置 pos到end全为0
		{
			if (pos + hop <= end)
			{
				return pos + hop;
			}
			else                          //说明当前块完成后，hop不是0,还需再走几步
			{
				pos = end;
				hop = hop - (end - pos);
				bindex++;
				if (flag == 0)
				{
					sbindex++;
				}
				prenums = rank1;
				//求得下一块中1的个数nums
				if ((bindex + 1) % (superBlockSize / blockSize) == 0)
				{
					nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
					flag = 0;
				}
				else
				{
					nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
					flag = 1;
				}
				if (rank1 < prenums + nums)
				{
					decode = S->GetValue(rank1);
				}
			}
		}
	
		while (hop != 0)                       //  pos是第decode前的那个位置，且S[rank1] = decode;
		{
			if (FF->GetValue(rank1) == 0)   //sub
			{
				cout << "jedgvkj";
			}
			else if (FF->GetValue(rank1) == 1) //ins
			{
				if (hop == 1)
				{
					return pos + 1;              //字符为插入的那个字符
				}
				if (hop == 2)
				{
					return pos + 1;              //字符是参考序列上的字符(插入字符后面的字符)
				}
				pos = pos + 1;
				hop = hop - 2;
			}
			else if (FF->GetValue(rank1) == 2)   //del
			{
				pos++;
			}
			else                              //mul
			{
				
			}
		}
#endif
	}
    else              //hop<0
	{
		cout << "这块是hop<0 也就是说向左转";
		cout << hop << endl;
		cout << pos << endl;
		if (hop == -1)
		{
			pos--;
			insflag = 2;
		}
		return pos;
	}
	/*
	if (decode < offset)
	{
		if (pos + hop <= end)
		{
			pos = pos + hop;
			return pos;
		}
		else if (pos + hop > end)
		{
			pos = end+1;
			hop = hop - blockSize + offset;
			bindex++;
			if (flag == 0)
			{
				sbindex++;
			}
		}
		else
		{
			cout << "我还没判断!" << endl;
		}
	}
	while (hop != 0)
	{
		if (decode > offset)
		{
			
		}
	}
	while (hop != 0)
	{
		if (Header->GetValue(bindex) == 0)
		{
			if ((hop + pos <= end) && (hop + pos >= start))
			{
				return (hop + pos);
			}
			else if (hop + pos > end)
			{
				hop = hop - end + pos;
				bindex++;
				offset = 0;
			}
			else if (hop + pos < start)
			{

			}
		}
		else
		{
			int decode=S->GetValue(rank1);
			while (decode < offset)
			{
				if (FF->GetValue(rank1) == 0)                    //subtitution
				{

				}
				else if (FF->GetValue(rank1)==1)              //insertion
				{
					hop--;
				}
				else if (FF->GetValue(rank1) == 2)              //delete
				{
					hop++;
				}
				rank1++;
				decode = S->GetValue(rank1);
			}
			//if (decode==offset)
		}
	}
	*/
}
unsigned char * compress::extract(int pos, int length)
{
	int bindex = pos / blockSize;
	int offset = pos % blockSize;
	int sbindex = pos / superBlockSize;
	int l = 0;            //结果下标
	int ll = 0;           //参考子段下标
	int rank1 = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
    
	// step1: 先从Reference中取出一个尽量大的子串>len
	// step2: 再判断哪一个位置上是差异，进行替换
	unsigned char * tmp_result = REF->extracting(pos, length + 10);
	unsigned char * result = (unsigned char *)malloc(sizeof(unsigned char)*(length + 1)); memset(result, 0, length + 1);
	int nums = 0;
	int flag = 0;
	int limit = rank1;
	int decode = S->GetValue(rank1);
	if (Header->GetValue(bindex) == 0)
	{
		nums = 0;
	}
	else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
	{
		nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
		flag = 0;
	}
	else
	{
		nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
		flag = 1;
	}

	limit += nums;
	while (decode<offset&&rank1 < limit)
	{
		rank1++;
		if (rank1 >= S->GetNum())
		{
			while (l < length)
			{
				result[l++] = tmp_result[ll++];
			}
			result[length] = '\0';
			return result;
		}
		decode = S->GetValue(rank1);
	}
	if (rank1 >= limit)
	{
		int kk = bindex + 1;
		while (Header->GetValue(kk) == 0)
		{
			kk++;
		}
		kk = kk - bindex - 1;
		int can = blockSize - offset + kk*blockSize  +decode;
		if (length <= can)
		{
			for (int k = 0; k < length; k++)
			{
				result[l++] = tmp_result[k];
			}
			result[length] = '\0';
			return result;
		}
		else
		{
			for (int k = 0; k < can; k++)
			{
				result[l++] = tmp_result[k];
			}
			ll = can;
		}
		bindex= bindex + kk +1;
		if (flag == 0)
		{
			sbindex++;
		}
		
		if (Header->GetValue(bindex) == 0)
		{
			nums = 0;
		}
		else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
		{
			nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
			flag = 0;
		}
		else
		{
			nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
			flag = 1;
		}
		limit += nums;
	}
	else if (decode>offset)
	{
		int can = decode - offset;
		if (length <= can)
		{
			for (int k = 0; k < length; k++)
			{
				result[l++] = tmp_result[k];
			}
			result[length] = '\0';
			return result;
		}
		else
		{
			for (int k = 0; k < can; k++)
			{
				result[l++] = tmp_result[ k];
			}
			ll = can;
		}
	}
	else if (decode == offset)
	{
		
	}
	while (l < length)
	{

		int f = FF->GetValue(rank1);
		unsigned char *tr = 0;
		switch (f)
		{
			case 0: 
				tr = CC->extracting((2 * q + 2)*rank1 + q+1, 1);  result[l++] = tr[0]; 
				if (tr) delete[] tr; ll++;
					break;                //替换
			case 1: 
				if (insflag == 0) 
				{
					tr = CC->extracting((2 * q + 2)*rank1 + q + 1, 1);
					result[l++] = tr[0];
					if (l < length)
					{
						result[l++] = tmp_result[ll++];
					}
				}
				else
				{
					result[l++] = tmp_result[ll++];
					insflag = 0;
				}
				if (tr) delete[] tr;
				break;                //插入
			case 2: ll++;  break;                //删除
			default: cout << "extract error"; break;
		}

		offset = (decode + 1) % blockSize;
		if (decode == blockSize - 1)
		{
			if ((bindex + 1) % (superBlockSize / blockSize) == 0)
			{
				sbindex++;
			}
			bindex++;
			if (Header->GetValue(bindex) == 0)
			{                                                                                                                                    
				nums = 0;
			}
			else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
			{
				nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
				flag = 0;
			}
			else
			{
				nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
				flag = 1;
			}
			limit += nums;
		}
		rank1++;
		if (rank1 >= S->GetNum())
		{
			while (l < length)
			{
				result[l++] = tmp_result[ll++];
			}
			result[length] = '\0';
			return result;
		}
		decode = S->GetValue(rank1);
		int can = -1;
		if(rank1 < limit)
		{
			can = decode - offset;
		}
		else
		{
			int kk = bindex + 1; 
			while (Header->GetValue(kk) == 0)
			{
				kk++;
			}
			kk = kk - bindex - 1;
			can = blockSize - offset + kk*blockSize + decode;
			bindex = bindex + kk +1;
			if (flag == 0)
			{
				sbindex++;
			}

			if (Header->GetValue(bindex) == 0)
			{
				nums = 0;
			}
			else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
			{
				nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
				flag = 0;
			}
			else
			{
				nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
				flag = 1;
			}
			limit += nums;
		}
		if (l >= length)
		{
			break;
		}
		int tmp = length - l;
		//cout << "can:" << can << endl;
		if (tmp <= can)
		{
			for (int k = 0; k < tmp; k++)
			{
				result[l++] = tmp_result[ll+k];
			}
			result[length] = '\0';
			return result;
		}
		else
		{
			for (int k = 0; k < can; k++)
			{
				result[l++] = tmp_result[ll+k];
			}
			ll = ll+can;
		}
	}
	result[length] = '\0';
	return result;
}
void compress::compReference()
{
	REF = new FM(Filename,1);
	//REF = ref;
	//cout << REF->extracting(1026045, 4) << endl;
}
bool compress::verifyREF(int pos, int length)     //检查是不是全0 函数有点问题
{
	int bindex = pos / blockSize;
	int sbindex = pos / superBlockSize;
	int offset = pos % blockSize;
	int nums = 0;
	int rank1 = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
	int limit = rank1;
	if (Header->GetValue(bindex) == 0)
	{
		//cout << "0呀" << endl;
		nums = 0;
		if (blockSize - offset >= length)
		{
			return true;
		}
		else
		{
			if (Header->GetValue(bindex + 1) == 0)
			{
				return true;
			}
			int decode = S->GetValue(rank1);
			if (blockSize - offset + decode >= length)
			{
				return true;
			}
			else
			{
				return false;
			}
			//cout << "跨快了" <<pos<< endl;
			//cout << "超块" << sbindex << "块" << bindex <<"偏移"<< offset << "个数" << blockSize - offset << endl;
		}
	}
	else if (bindex == (max1 - 1) / blockSize)  //如果是最后一块的话
	{
		//cout << "进入这块" << endl;
		nums = diffnum - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
		int decode = 0;
		for (int i = rank1; i < rank1 + nums; i++)
		{
			decode = S->GetValue(i);
			if (decode == offset)
			{
				return false;
			}
			else if (decode > offset)
			{
				break;
			}
		}
		if (decode > offset)
		{
			int can = decode - offset;
			if (length <= can)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}
	else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
	{
		//cout << "是这块吗" << endl;
		nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
		//cout << "超块" << sbindex << "块" << bindex << "偏移" << offset << "个数" << blockSize - offset << endl;
	}
	else
	{
		//cout << "what" << endl;
		nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
	}
	limit = limit +nums;
	int decode = S->GetValue(rank1);
	//cout << "rank1" << rank1 << "  nums:" << nums << "  offset" << offset << "  decode" << decode << endl;
	while (decode < offset && rank1+1 < limit)
	{
		rank1++;
		decode = S->GetValue(rank1);
		//cout << "rank" << rank1 << "decode" << decode << endl;;
	}
	//cout << "咋样" << endl;
	if (decode == offset)
	{
		return false;
	}
	if (rank1 + 1 >= limit&&decode<offset)
	{
		//cout << "是里面" << endl;
		if (blockSize - offset >= length)
		{
			return true;
		}
		if (Header->GetValue(bindex + 1) == 0)
		{
			return true;
		}
		int decode = S->GetValue(limit);
		if (blockSize - offset + decode >= length)
		{
			return true;
		}
		else
		{
			return false;
		}
		/*
		int kk = bindex + 1;
		while (Header->GetValue(kk) == 0)
		{
			kk++;
			cout << "kk" << kk << endl;
		}
		kk = kk - bindex - 1;
		int can = blockSize - offset + kk*blockSize + decode;
		if (length <= can)
		{
			return true;
		}
		else
		{
			return false;
		}*/
	}
	else if (decode>offset)
	{
		int can = decode - offset;
		if (length <= can)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (decode == offset)
	{
		return false;
	}
	return true;
}      
void compress::locatingadd(char * pattern, int length, int & num,  int * pos1, int nnum, vector<int> &resultpos)
{
	
	resultpos.clear();
	int len = max1;	
	
	for (int i = 0; i < nnum; i++)
	{
		if (verifyREF(pos1[i], length))
		{
			resultpos.push_back(pos1[i]);
		}
	}

	int * posCC = CC->locating(pattern, nnum); 
	int nnn = 0;

	for (int i = 0; i < nnum; i++)
	{
		int sel = posCC[i] / (2 * q + 2); 
		int oo = posCC[i] % (2 * q + 2); 
		
		int p = select(sel + 1);	
		
		if (oo == q + 1)
		{

		}
		else if (oo <= q)
		{
			p = p - (q + 1 - oo);
		}
		else
		{
			p = p + (oo - q - 1);
		}	
		resultpos.push_back(p);
	}

	sort(resultpos.begin(), resultpos.end());
	vector<int>::iterator end_unique =  unique(resultpos.begin(), resultpos.end()); 
	//resultpos.erase(end_unique, resultpos.end());
	num = end_unique - resultpos.begin();
	//num = resultpos.size();	
	if(posCC) delete[] posCC;
}
int * compress::locating(char * pattern, int length, int & num)
{
	//step1: 在参考序列中找  完事后检查B[pos]开始的位置后length长的是不是全0 如果是则pos映射为待压缩序列上的位置返回
	int nnum = 0;
	int * pos1 =  REF->locating(pattern, nnum);
	int len = max1;
	//int len = max1;
	int * resultpos = new int[len];
	long t = 0;
	for (int i = 0; i < nnum; i++)
	{
		//cout << "位置:" << pos1[i] << endl;
		if (verifyREF(pos1[i], length))
		{
			//cout << "位置:" << pos1[i] << endl;
			//此刻将位置加入到结果集中
			resultpos[t++] = pos1[i];
		}
	}
	//cout << "公共的:" << t << endl;
	int * posCC = CC->locating(pattern, nnum);  //在
	//cout << pattern << endl;
	//cout << "差异个数:" << nnum << endl;
	int nnn = 0;
	//assert(posCC);
	//cout << posCC[0] << endl;
	//cout << CC->extracting(0, 2 * q + 2) << endl;
	for (int i = 0; i < nnum; i++)
	{
		int sel = posCC[i] / (2 * q + 2); //第几个%,第几个差异 下标0开始
		int oo = posCC[i] % (2 * q + 2);    //在差异串中的偏移位置 下标0开始
		//cout << CC->extracting(posCC[i] - oo, 2 * q + 2);
		//cout << "在第" << sel << "个差异中的第" << oo << "个位置" << endl;
		//接下来先找此差异在哪个超快中
		int p = select(sel+1);
		if (FF->GetValue(sel) == 0)      //如果是sub
		{
			if (oo == q + 1)
			{

			}
			else if (oo <= q)
			{
				p = p - (q + 1 - oo);
			}
			else
			{
				p = p + (oo - q - 1);
			}
		}
		assert(t < len);
		resultpos[t++] = p;
		//cout << "位置在:" << p << endl;
		//接下来找差异在哪个块中
		//cout << "快快:" << spj << endl;

		//超块中第一块 spj*(sb/b) 最后一块是（spj+1）*(sb/b)-1
	}
    //count distinct
	int ttt = 11;
	sort(resultpos, resultpos + t);
	ttt = unique(resultpos, resultpos + t) - resultpos;
	num = ttt;
	//cout << "总共位置的个数:" << num << endl;
	//需要select操作得到最终位置
	//加入结果集合时需要去除重复
	if (pos1) delete[] pos1;
	if (posCC) delete[] posCC;
	return resultpos;
	//step2 : 在C串中找 找到的话加到集合中
}
vector<int> compress::locate2add(char * pattern, int length, int & num)
{
	vector<int> resultpos;
	int pattnum = 0;
	string * patts = split(pattern, length, pattnum);
	//cout << pattnum;
	int * pos_nums = new int[pattnum];

	int num0 = 0;
	//vector<int *> p;
	vector<CPerfectHashTable> tables;
	int * p1 = locating((char *)patts[0].c_str(), q, pos_nums[0]);   //modify 1
	//p.push_back(p1);

	for (int i = 1; i < pattnum; i++)
	{

		int * tmp = locating((char *)patts[i].c_str(), q, pos_nums[i]);      //modify 2
		CPerfectHashTable tmp_table = createHashTable(tmp, pos_nums[i]);
		//给其余模式创建哈希表
		tables.push_back(tmp_table);
	}
	for (int i = 0; i < pos_nums[0]; i++)
	{
		int rpos = p1[i];  // 转化string
		bool value = 0;
		//cout << p1[i] << endl;
		for (int j = 0; j < pattnum - 1; j++)
		{
			rpos += q;
			const char * rr = itoa1(rpos);
			//cout << "rpos"<<rpos<<" "<<rr << endl;
			value = tables[j].GetValue(rr);
			if (value == 1)
			{
				//cout << "找到了:" << endl;
			}
			else
			{
				break;
			}
		}
		if (value == 1)
		{
			//p1[i]是最终结果
			resultpos.push_back(p1[i]);
		}
	}
	num = resultpos.size();
	return resultpos;
}
vector<int> compress::locate2(char * pattern, int length, int & num)
{
	vector<int> resultpos;
	int pattnum = 0;
	string * patts = split(pattern, length,pattnum);
	//cout << pattnum;
	int * pos_nums = new int[pattnum];
	
	int num0 = 0;
	//vector<int *> p;
	vector<CPerfectHashTable> tables;
	int * p1 = locating((char *)patts[0].c_str(), q, pos_nums[0]);
	//p.push_back(p1);

	for (int i = 1; i < pattnum; i++)
	{
		
		int * tmp = locating((char *)patts[i].c_str(), q, pos_nums[i]);
		CPerfectHashTable tmp_table = createHashTable(tmp, pos_nums[i]);
		//给其余模式创建哈希表
		tables.push_back(tmp_table);
		//if (tmp) delete[] tmp;
	}
	for (int i = 0; i < pos_nums[0]; i++)
	{
		int rpos = p1[i];  // 转化string
		bool value = 0;
		//cout << p1[i] << endl;
		for (int j = 0; j < pattnum - 1; j++)
		{
			rpos += q;
			const char * rr = itoa1(rpos);
			//cout << "rpos"<<rpos<<" "<<rr << endl;
			value = tables[j].GetValue(rr);
			if (value == 1)
			{
				//cout << "找到了:" << endl;
			}
			else
			{
				break;
			}
		}
		if (value == 1)
		{
			//p1[i]是最终结果
			resultpos.push_back(p1[i]);
		}
	}
	num = resultpos.size();
	//if (p1) delete[] p1;
	return resultpos;
}
string * compress::split(char * pattern, int length,int & pattnum)
{
	//char * p = (char *)pattern;
	//int p_length = pattern.size();               //模式长度
	string str = pattern;
	int p_length = length;
	int p_nums = 0;           //划分的子模式的个数
	if (p_length % q == 0)
	{
		p_nums = p_length / q;
	}
	else
	{
		p_nums = p_length / q + 1;
	}
	string *QPattern = new string[p_nums];
	pattnum = p_nums;
	for (int k = 0; k < p_nums; k++)
	{
		int len = min(q, length - q * k);
		QPattern[k]= str.substr(k * q, len);
	}	
	/*for (int i = 0; i < p_nums; i++)
	{
		cout << QPattern[i] << endl;
	}*/
	return QPattern;
}
unsigned char * compress::testextract(int pos, int length, int number)
{
	Header = DNA[number].Header;
	Brank = DNA[number].Brank;
	CC = DNA[number].CC;
	diffnum = DNA[number].diffnum;
	FF = DNA[number].FF;
	S = DNA[number].S;
	SBrank = DNA[number].SBrank;
	V = DNA[number].V;
	//cout << "位置" << pos;
	pos = mappos(pos);
	//cout << "test对应位置是:" << pos << endl;
	//将pos转换成result_pos
	int bindex = pos / blockSize;
	int offset = pos % blockSize;
	int sbindex = pos / superBlockSize;
	int l = 0;            //结果下标
	int ll = 0;           //参考子段下标
	int rank1 = DNA[number].SBrank->GetValue(sbindex) + DNA[number].Brank->GetValue(bindex);
	// step1: 先从Reference中取出一个尽量大的子串>len
	// step2: 再判断哪一个位置上是差异，进行替换
	unsigned char * tmp_result = REF->extracting(pos, length + 10);
	//cout << tmp_result << endl;
	unsigned char * result = (unsigned char *)malloc(sizeof(unsigned char)*(length + 1)); 
	memset(result, 0, length + 1);
	
	int nums = 0;
	int flag = 0;
	int limit = rank1;
	int decode = DNA[number].S->GetValue(rank1);
	//先算此块中的nums
	if (DNA[number].Header->GetValue(bindex) == 0)
	{
		nums = 0;
	}
	else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
	{
		nums = DNA[number].SBrank->GetValue(sbindex + 1) - DNA[number].SBrank->GetValue(sbindex) - DNA[number].Brank->GetValue(bindex);
		flag = 0;
	}
	else
	{
		nums = DNA[number].Brank->GetValue(bindex + 1) - DNA[number].Brank->GetValue(bindex);
		flag = 1;
	}
	limit += nums;

	while (decode<offset&&rank1 < limit)
	{
		rank1++;
		decode = DNA[number].S->GetValue(rank1);
	}
	if (rank1 >= limit)
	{
		int kk = bindex + 1;
		while (DNA[number].Header->GetValue(kk) == 0)
		{
			kk++;
		}
		kk = kk - bindex - 1;
		int can = blockSize - offset + kk*blockSize + decode;
		if (length <= can)
		{
			for (int k = 0; k < length; k++)
			{
				result[l++] = tmp_result[k];
			}
			result[length] = '\0';
			return result;
		}
		else
		{
			for (int k = 0; k < can; k++)
			{
				result[l++] = tmp_result[k];
			}
			ll = can;
		}
		bindex = bindex + kk + 1;
		if (flag == 0)
		{
			sbindex++;
		}

		if (DNA[number].Header->GetValue(bindex) == 0)
		{
			nums = 0;
		}
		else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
		{
			nums = DNA[number].SBrank->GetValue(sbindex + 1) - DNA[number].SBrank->GetValue(sbindex) - DNA[number].Brank->GetValue(bindex);
			flag = 0;
		}
		else
		{
			nums = DNA[number].Brank->GetValue(bindex + 1) - DNA[number].Brank->GetValue(bindex);
			flag = 1;
		}
		limit += nums;
	}
	else if (decode>offset)
	{
		int can = decode - offset;
		if (length <= can)
		{
			for (int k = 0; k < length; k++)
			{
				result[l++] = tmp_result[k];
			}
			result[length] = '\0';
			return result;
		}
		else
		{
			for (int k = 0; k < can; k++)
			{
				result[l++] = tmp_result[k];
			}
			ll = can;
		}
	}
	else if (decode == offset)
	{
		
	}
	while (l < length)
	{
		int f = DNA[number].FF->GetValue(rank1);
		unsigned char *tr = 0;
		switch (f)
		{
		case 0:
			tr = DNA[number].CC->extracting((2 * q + 2)*rank1 + q + 1, 1);  result[l++] = tr[0];
			if (tr) delete[] tr; ll++;
			break;                //替换
		case 1:
			if (insflag == 0) {
				tr = DNA[number].CC->extracting((2 * q + 2)*rank1 + q + 1, 1);
				result[l++] = tr[0];
				if (l < length)
				{
					result[l++] = tmp_result[ll++];
				}
				else
				{
					result[length] = '\0';
					return result;
				}
			}
			else
			{
				result[l++] = tmp_result[ll++];
				insflag = 0;
			}
			if (tr) delete[] tr;
			//cout << "是插入哦!" << endl;
			break;                //插入
		case 2: ll++;  break;                //删除
		case 3: cout << "未完成功能"; break;                //多插
		default: cout << "extract error"; break;
		}
		offset = (decode + 1) % blockSize;
		if (decode == blockSize - 1) {
			if ((bindex + 1) % (superBlockSize / blockSize) == 0)
			{
				sbindex++;
			}
			bindex++;
			if (Header->GetValue(bindex) == 0)
			{
				nums = 0;
			}
			else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
			{
				nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
				flag = 0;
			}
			else
			{
				nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
				flag = 1;
			}
			limit += nums;
		}
		rank1++;
		decode = DNA[number].S->GetValue(rank1);
		int can = -1;
		if (rank1 < limit)
		{
			can = decode -offset;
		}
		else
		{
			int kk = bindex + 1;
			while (DNA[number].Header->GetValue(kk) == 0)
			{
				kk++;
			}
			kk = kk - bindex - 1;
			can = blockSize - offset + kk*blockSize + decode;
			bindex = bindex + kk + 1;
			if (flag == 0)
			{
				sbindex++;
			}

			if (DNA[number].Header->GetValue(bindex) == 0)
			{
				nums = 0;
			}
			else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
			{
				nums = DNA[number].SBrank->GetValue(sbindex + 1) - DNA[number].SBrank->GetValue(sbindex) - DNA[number].Brank->GetValue(bindex);
				flag = 0;
			}
			else
			{
				nums = DNA[number].Brank->GetValue(bindex + 1) - DNA[number].Brank->GetValue(bindex);
				flag = 1;
			}
			limit += nums;
		}
		if (l >= length)
		{
			break;
		}
		int tmp = length - l;
		if (tmp <= can)
		{
			for (int k = 0; k < tmp; k++)
			{
				result[l++] = tmp_result[ll + k];
			}
			result[length] = '\0';
			return result;
		}
		else
		{
			for (int k = 0; k < can; k++)
			{
				result[l++] = tmp_result[ll + k];
			}
			ll = ll + can;
		}
	}
	result[length] = '\0';
	return result;
	
}
int compress::sizeInByte()
{
	int totolByte = REF->sizeInByte();
	int byte = 0;
	for (int i = 0; i < DNAnums - 1; i++)
	{
		byte += DNA[i].CC->sizeInByte();
		int tmp = DNA[i].Brank->GetNum() + DNA[i].SBrank->GetNum() + DNA[i].Header->GetNum()
			+ DNA[i].S->GetNum() + DNA[i].V->GetNum() + DNA[i].FF->GetNum() + DNA[i].CC->sizeInByte();
		totolByte += tmp;
	}
	cout << "上下文:" << byte << "字节" << endl;
	cout << "总大小:" << totolByte << "字节" << endl;
	cout << "上下文:" << (double)(byte / 1024.0) << endl;
	cout << "上下文:" << (double)(totolByte / (1024.0*1024)) << endl;
	return totolByte;
}
int compress::sizeInByteP()
{
	int tmp = Brank->GetMemorySize() + SBrank->GetMemorySize() + Header->GetMemorySize()
		+ S->GetMemorySize() + V->GetMemorySize();
	return tmp;
}
int compress::sizeInByteSC()
{
	return S->GetMemorySize();
}
int compress::sizeInByteBrank()
{
	int tmp = Brank->GetMemorySize() + SBrank->GetMemorySize() + Header->GetMemorySize()
		+  V->GetMemorySize();
	return tmp;
}
int compress::sizeInByteC()
{
	return CC->sizeInByte();
}
int compress::sizeInByteF()
{
	return FF->GetMemorySize();
}
int compress::sizeInByte1()
{
	int totolByte = 0;
	//int tmp = Brank->GetNum() + SBrank->GetNum() + Header->GetNum()
		//+ S->GetNum() + V->GetNum() + FF->GetNum() + CC->sizeInByte();
	int tmp = Brank->GetMemorySize() + SBrank->GetMemorySize() + Header->GetMemorySize()
		+ S->GetMemorySize() + V->GetMemorySize() + FF->GetMemorySize() + CC->sizeInByte();
	totolByte += tmp;
	//totolByte = REF->sizeInByte() + 99 * totolByte;
	//cout << "总大小:" << totolByte << "字节" << endl;
	//cout << "上下文:" << (double)(totolByte / (1024.0 * 1024)) << endl;
	return totolByte;
}
double compress::compressRatio2()
{
	return sizeInByte1() / (max1*1.0);
}
int compress::getN()
{
	return DNAnums*max1;
}
double compress::compressRatio()
{
	return sizeInByte() / (getN()*1.0);
}
int compress::ff(int low, int high, int key)
{
	if (low < high)
	{
		int middle = (low + high) / 2;
		if (SBrank->GetValue(middle) >= key)
		{
			high = middle - 1;
			ff(low,high,key);
		}
		else
		{
			low = middle + 1;
			ff(low, high, key);
		}
	}
	if (low == high)
	{
		if (SBrank->GetValue(low) < key)
		{
			return low;
		}
		else if (SBrank->GetValue(low) == key)
		{
			return (low - 1);
		}
		else
		{
			cout << "error" << endl;
		}
	}
}
int compress::select(int key)              //index 下标1开始
{
	//利用二分查找找到他多应该在的超块中
	//锁定超块后判断具体在哪一块
	//返回位置
	int low = 0, high = SBrank->GetNum();
	while (low < high)
	{
		int middle = (low + high) / 2;
		if (SBrank->GetValue(middle) >= key)
		{
			high = middle;
		}
		else
		{
			low = middle + 1;
		}
	}
	//cout << "所在超快为:" << low - 1 << endl;;
	//cout << "超块前1的个数:" << SBrank->GetValue(low-1) << endl;
	int spj = low - 1;
	int key2 = key - SBrank->GetValue(spj);
	low = spj*(superBlockSize / blockSize);
	//cout << "第一块:" << low << endl;
	//high = (spj + 1)*(superBlockSize / blockSize) - 1;
	high = min((spj + 1)*(superBlockSize / blockSize) - 1, (max1 - 1) / blockSize );
	/*for (int ll = 0; ll < 3; ll++)
	{
		cout << Brank->GetValue(low + ll) << "  ";
	}
	cout << endl;*/
	//cout << high << endl;
	int bpj = -1;
	if (key2>Brank->GetValue(high))
	{
		bpj = high;
	}
	else {
		while (low < high)
		{
			int middle = (low + high) / 2;
			if (Brank->GetValue(middle) >= key2)
			{
				high = middle;
			}
			else
			{
				low = middle + 1;
			}
		}
		bpj = low - 1;
	}
	//cout << "所在快为:" <<bpj << endl;;
	int decode = S->GetValue(key-1);
	//cout<<S->GetValue(989)<<endl;
	//cout << "差异位置:" << decode << endl;
	int pppos = (bpj*blockSize) + decode;
	//cout << pppos << endl;
	//cout << "BV" << BV[pppos] << endl;
	return pppos;
}
CPerfectHashTable compress:: createHashTable(int pos1[], int length)
{
	string * pos = new string[length];
	for (int i = 0; i < length; i++)
	{
		stringstream ss;
		ss << pos1[i];
		pos[i] = ss.str();
	}
	CPerfectHashTable resultable;
	vector<pair<string, int> > data;
	for (int i = 0; i < length; i++)
	{
		data.push_back(make_pair(pos[i], i));
	}
	resultable.Calculate(data);
	return resultable;
}
void splitconnect()              //拼接在一起
{
	int ch = 1000;
	char tmp = 'c';
	string str = "huan";
	int llnums = 5;
	
	int * first = new int[100];
	for (int i = 0; i < 100; i++)
	{
	}
	cout << " " << endl;
}
void compress::transferstring(int * pos, int length)
{
	string * r_pos = new string[length];
	for (int i = 0; i < length; i++)
	{
		stringstream ss;
		ss << pos[i];
		r_pos[i] = ss.str();
		//itoa(pos[i], (char *)r_pos[i].c_str(), 10);       //没有分配空间string string 大小也不能确定 会有乱码
	}
	//CPerfectHashTable table1 = createHashTable(r_pos,length);
}


void compress::locate3add(int * p11, string * patts, int &pattnum, vector<int* >  &tablesRef, 
	int &num, int * flagpos_nums, vector<int> &resultpos)
{
	resultpos.clear();	
	assert(pattnum > 0);	
	vector<int> pos_nums(pattnum, 0);
	int num0 = 0;
	int flagnums = 0; 
	
	//vector<int > t_tmp(max1, 0);	
	//vector<vector<int > > tables(pattnum - 1, t_tmp);
	vector<vector<bool > > tables; 
	vector<int> p1; 
	
	locatingadd((char *)patts[0].c_str(), patts[0].size(), pos_nums[0], p11, flagpos_nums[0], p1); //first 
	vector<int > tmp;		
	
	for (int i = 1; i < pattnum; i++)
	{	
		locatingadd((char *)patts[i].c_str(), patts[i].size(), pos_nums[i], tablesRef[i - 1], flagpos_nums[i], tmp); //i-th
		vector<bool> ttmp(max1, 0);
		for (int j = 0; j < pos_nums[i]; j++)
		{
			assert(tmp[j] >= 0 && tmp[j] < max1);
			//tables[i - 1][tmp[j]] = 1;
			ttmp[tmp[j]] = 1;
 		}
		tables.push_back(ttmp);
	}	
	for (int i = 0; i < pos_nums[0]; i++)
	{
		int rpos = p1[i];  		
		bool value = 0; 
		for (int j = 0; j < pattnum - 1; j++)
		{
			rpos += q;
			value = tables[j][rpos]; 
			if (value == 1)
			{
			}
			else
			{
				break;
			}
		}
		if (value == 1)
		{
			resultpos.push_back(p1[i]);
		}
	}	
	num = resultpos.size();
	return ;
}

int* compress::locatingadd(char * pattern, int length, int & num,  int * pos1, int nnum)
{
	int len = max1;
	int * resultpos = new int[len];
	int t = 0;
	for (int i = 0; i < nnum; i++)
	{
		if (verifyREF(pos1[i], length))
		{
			resultpos[t++] = pos1[i];
		}
	}
	int * posCC = CC->locating(pattern, nnum);  //在
	int nnn = 0;
	for (int i = 0; i < nnum; i++)
	{
		int sel = posCC[i] / (2 * q + 2); //第几个%,第几个差异 下标0开始
		int oo = posCC[i] % (2 * q + 2);    //在差异串中的偏移位置 下标0开始
		int p = select(sel + 1);
		if (oo == q + 1)
		{
			
		}
		else if (oo <= q)
		{
			p = p - (q + 1 - oo);
		}
		else
		{
			p = p + (oo - q - 1);
		}		
		assert(t < len);
		resultpos[t++] = p;
	}
	int ttt = 11;
	sort(resultpos, resultpos + t);
	ttt = unique(resultpos, resultpos + t) - resultpos;
	num = ttt;
	if (posCC) delete[] posCC;
	return resultpos;
}

vector<int> compress::locate3add(int * p11, string * patts2, int &pattnum2, vector<int *>  &tablesRef2, 
	int & num, int * flagpos_nums)
{
	vector<int> resultpos;
	string * patts = patts2;
	int pattnum = pattnum2;
	int * pos_nums = new int[pattnum];
	int num0 = 0;
	int flagnums = 0; 
		
	vector<int *> tablesRef = tablesRef2;
	vector<bool> t_tmp(max1, 0);	
	vector<vector<bool > > tables(pattnum - 1, t_tmp);
	
	int * p1 = locatingadd((char *)patts[0].c_str(), patts[0].size(), pos_nums[0], p11, flagpos_nums[0]); //first 
	for (int i = 1; i < pattnum; i++)
	{
		int * tmp = locatingadd((char *)patts[i].c_str(), patts[i].size(), pos_nums[i], tablesRef[i - 1], flagpos_nums[i]); //i-th 
		for (int j = 0; j < pos_nums[i]; j++)
		{
			if (tmp[j] < 0)
			{
				cout << "小于0" << tmp[j] << endl;
				exit(0);
			}
			tables[i-1][tmp[j]] = 1;
		}
		if (tmp) delete[] tmp;
	}
	for (int i = 0; i < pos_nums[0]; i++)
	{
		int rpos = p1[i];  // 转化string
		bool value = 0;
		for (int j = 0; j < pattnum - 1; j++)
		{
			rpos += q;
			if (rpos < 0)
			{
				cout << "小于02" << rpos << endl;
				exit(0);
			}
			value = tables[j][rpos];
			if (value == 1)
			{
				//cout << "找到了:" << endl;
			}
			else
			{
				break;
			}
		}
		if (value == 1)
		{
			resultpos.push_back(p1[i]);
		}
	}	
	if(p1) delete[] p1;
	num = resultpos.size();
	return resultpos;
}


