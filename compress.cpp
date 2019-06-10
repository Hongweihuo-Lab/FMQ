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
		//����������
		connect();              //�õ�Context
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
		//cout << "�жϵ�λ��j: "<<j<<"   l: " << l << "  r:" << r << endl;
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
		//cout << "�������:" << diffnum << endl;	
	/*	for (int k = 0; k < t + 1; k++)
			if (M[k]) delete[] M[k];
		if (M) delete[] M;*/
	}
	cout << "�������:" << diffnum << endl;
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
		//cout << "�жϵ�λ��j: "<<j<<"   l: " << l << "  r:" << r << endl;
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
		//cout << "�������:" << diffnum << endl;	
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
		//cout << "�жϵ�λ��j: "<<j<<"   l: " << l << "  r:" << r << endl;
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
	//cout << "�������:" << diffnum << endl;
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
			cout << "λ��:" << i << "������:" << s2[i] << "ʵ����:" << vs2[i] << endl;
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
	/*�����ĵ�ѹ��*/
	CC = new FM((unsigned char *)Context, diffnum*(2 * q + 2)+2, 1);
	//CC->getAlphabetSize();
	/* F���ö���InArray��*/
	FF = new InArray(diffnum, 2);
	for (int i = 0; i < diffnum; i++)
	{
		FF->SetValue(i, F[i]);
	}

	/* BV��ѹ��*/
	
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
	//cout << "all0�����:" << all0nums;
	//cout << "            loc�����:" << locnums << endl;
}
void compress::compreMethod2()
{
	/*�����ĵ�ѹ��*/
	CC = new FM((unsigned char *)Context, diffnum*(2 * q + 2) + 2, 1);
	/* F���ö���InArray��*/
	FF = new InArray(diffnum, 2);
	for (int i = 0; i < diffnum; i++)
	{
		FF->SetValue(i, F[i]);
	}

	/* BV��ѹ��*/

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
	//cout << "���:" << bindex << endl;
	//cout << "����ţ�" << sbindex << endl;
	int hop = V->GetValue(bindex);
	int offset = pos% blockSize;
	//cout << "ƫ��:" << offset << endl;
	int nums =0;                 //�˿���1�ĸ���
	int start = bindex*blockSize;
	int end = start + blockSize - 1;
	int rank1 = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
	//cout << "֮ǰ1�ĸ���:" << rank1 << endl;
	int decode = S->GetValue(rank1);
	int prenums = rank1;        //��¼��ǰ���Ѿ�������1�ĸ���
	int flag = 0;                       //��ʾ��������һ��
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
	//cout << "�˿���1�ĸ���:" << nums << endl;
	int limit = prenums + nums;        //�������rank1����С��limit
	//��¼��posǰ�ܹ���Ҫ�ƶ��Ĳ���
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
		//cout << "ƫ��:" << decode << " ";
	}
	//cout << endl;
	//cout << "Ҫ����������"<<hop << endl;
	if ((decode >offset) && (rank1<limit))
	{
		//֤��B[pos]=0
	}
	else if (decode == offset)
	{
		//֤��B[pos]=1
	}
	else if (rank1 >= limit)
	{
		//֤��B[pos]=0
		//����������һdecode��ʼ�����ж���0
	}
    //�����ĵ��ϵĽ��о���ֵ����ת
	if (hop == 0)
	{
		return pos;
	}
	//�����ת
	else if (hop > 0)
    {
		if ((decode >offset) && (rank1<limit))
		{
			//֤��B[pos]=0
			if ((decode - offset) > hop)
			{
				return pos + hop;
			}
			else
			{
				pos = pos + decode - offset - 1;
				hop == hop - (decode - offset - 1);
			}
			//�˿�pos+1λ���ǲ���
			//��ʾ�����S��λ����rank1
		}
		else if ((decode == offset) && (rank1<limit))
		{
			//֤��B[pos]=1
			if (FF->GetValue(rank1) == 0)  //�����sub
			{

			}
			else if (FF->GetValue(rank1) == 1)  //�����ins
			{
				if (hop == 1)
				{
					insflag = 2;
					return pos;   //��Ϊ�ο������ϵ��ַ�
				}
				hop--;
			}
			else if (FF->GetValue(rank1) == 2)   //�����del
			{
				hop++;
				//�貹��
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
				bindex++;           //������һ����
				if (flag == 0)       //֤��pos�ǳ�������һ��
				{
					sbindex++;
				}
				//decode��pos����һ����1�ĵ�һ��λ��
				//�˿̼���pos����ֱ������λ�õĶ���
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
				//���ʱ��decode�ǲ��죬��λ����rank1;
				//��Ҫ��������1�ĸ���nums
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
			//֤��B[pos]=0
			//����������һdecode��ʼ�����ж���0
			bindex++;           //������һ����
			if (flag == 0)       //֤��pos�ǳ�������һ��
			{
				sbindex++;
			}
			//decode��pos����һ����1�ĵ�һ��λ��
			//�˿̼���pos����ֱ������λ�õĶ���
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
			//���ʱ��decode�ǲ��죬��λ����rank1;
			//��Ҫ��������1�ĸ���nums
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
		//�˿�pos+1�ǲ���λ�� Ҳ����decode S[rank1]=decode limit��rank1������
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
					return pos+1; //�ַ�ʱ������ַ�
				}
				if(hop==2)
				{
					insflag = 2;
					return pos+1;  //�ַ�ʱ�ο����е��ַ�
				}
				pos=pos+1;
				hop=hop-2;
			}
			else if(FF->GetValue(rank1)==2)  //del
			{
				pos=pos+1;
			}
            //������������decode
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
				bindex++;           //������һ����
				if (flag == 0)       //֤��pos�ǳ�������һ��
				{
					sbindex++;
				}
				//decode��pos����һ����1�ĵ�һ��λ��
				//�˿̼���pos����ֱ������λ�õĶ���
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
				//���ʱ��decode�ǲ��죬��λ����rank1;
				//��Ҫ��������1�ĸ���nums
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
		if (decode > offset)                    //˵���˳�ѭ���������ǵ�ǰ����1�ĸ�����û�б�����
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
			else if (FF->GetValue(rank1)==1)   //��ins
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
		else                         //decode<offset  ˵��rank1Ϊ��һ���е�1��ʼ��λ�� pos��endȫΪ0
		{
			if (pos + hop <= end)
			{
				return pos + hop;
			}
			else                          //˵����ǰ����ɺ�hop����0,�������߼���
			{
				pos = end;
				hop = hop - (end - pos);
				bindex++;
				if (flag == 0)
				{
					sbindex++;
				}
				prenums = rank1;
				//�����һ����1�ĸ���nums
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
	
		while (hop != 0)                       //  pos�ǵ�decodeǰ���Ǹ�λ�ã���S[rank1] = decode;
		{
			if (FF->GetValue(rank1) == 0)   //sub
			{
				cout << "jedgvkj";
			}
			else if (FF->GetValue(rank1) == 1) //ins
			{
				if (hop == 1)
				{
					return pos + 1;              //�ַ�Ϊ������Ǹ��ַ�
				}
				if (hop == 2)
				{
					return pos + 1;              //�ַ��ǲο������ϵ��ַ�(�����ַ�������ַ�)
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
		cout << "�����hop<0 Ҳ����˵����ת";
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
			cout << "�һ�û�ж�!" << endl;
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
	int l = 0;            //����±�
	int ll = 0;           //�ο��Ӷ��±�
	int rank1 = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
    
	// step1: �ȴ�Reference��ȡ��һ����������Ӵ�>len
	// step2: ���ж���һ��λ�����ǲ��죬�����滻
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
					break;                //�滻
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
				break;                //����
			case 2: ll++;  break;                //ɾ��
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
bool compress::verifyREF(int pos, int length)     //����ǲ���ȫ0 �����е�����
{
	int bindex = pos / blockSize;
	int sbindex = pos / superBlockSize;
	int offset = pos % blockSize;
	int nums = 0;
	int rank1 = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
	int limit = rank1;
	if (Header->GetValue(bindex) == 0)
	{
		//cout << "0ѽ" << endl;
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
			//cout << "�����" <<pos<< endl;
			//cout << "����" << sbindex << "��" << bindex <<"ƫ��"<< offset << "����" << blockSize - offset << endl;
		}
	}
	else if (bindex == (max1 - 1) / blockSize)  //��������һ��Ļ�
	{
		//cout << "�������" << endl;
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
		//cout << "�������" << endl;
		nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
		//cout << "����" << sbindex << "��" << bindex << "ƫ��" << offset << "����" << blockSize - offset << endl;
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
	//cout << "զ��" << endl;
	if (decode == offset)
	{
		return false;
	}
	if (rank1 + 1 >= limit&&decode<offset)
	{
		//cout << "������" << endl;
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
	//step1: �ڲο���������  ���º���B[pos]��ʼ��λ�ú�length�����ǲ���ȫ0 �������posӳ��Ϊ��ѹ�������ϵ�λ�÷���
	int nnum = 0;
	int * pos1 =  REF->locating(pattern, nnum);
	int len = max1;
	//int len = max1;
	int * resultpos = new int[len];
	long t = 0;
	for (int i = 0; i < nnum; i++)
	{
		//cout << "λ��:" << pos1[i] << endl;
		if (verifyREF(pos1[i], length))
		{
			//cout << "λ��:" << pos1[i] << endl;
			//�˿̽�λ�ü��뵽�������
			resultpos[t++] = pos1[i];
		}
	}
	//cout << "������:" << t << endl;
	int * posCC = CC->locating(pattern, nnum);  //��
	//cout << pattern << endl;
	//cout << "�������:" << nnum << endl;
	int nnn = 0;
	//assert(posCC);
	//cout << posCC[0] << endl;
	//cout << CC->extracting(0, 2 * q + 2) << endl;
	for (int i = 0; i < nnum; i++)
	{
		int sel = posCC[i] / (2 * q + 2); //�ڼ���%,�ڼ������� �±�0��ʼ
		int oo = posCC[i] % (2 * q + 2);    //�ڲ��촮�е�ƫ��λ�� �±�0��ʼ
		//cout << CC->extracting(posCC[i] - oo, 2 * q + 2);
		//cout << "�ڵ�" << sel << "�������еĵ�" << oo << "��λ��" << endl;
		//���������Ҵ˲������ĸ�������
		int p = select(sel+1);
		if (FF->GetValue(sel) == 0)      //�����sub
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
		//cout << "λ����:" << p << endl;
		//�������Ҳ������ĸ�����
		//cout << "���:" << spj << endl;

		//�����е�һ�� spj*(sb/b) ���һ���ǣ�spj+1��*(sb/b)-1
	}
    //count distinct
	int ttt = 11;
	sort(resultpos, resultpos + t);
	ttt = unique(resultpos, resultpos + t) - resultpos;
	num = ttt;
	//cout << "�ܹ�λ�õĸ���:" << num << endl;
	//��Ҫselect�����õ�����λ��
	//����������ʱ��Ҫȥ���ظ�
	if (pos1) delete[] pos1;
	if (posCC) delete[] posCC;
	return resultpos;
	//step2 : ��C������ �ҵ��Ļ��ӵ�������
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
		//������ģʽ������ϣ��
		tables.push_back(tmp_table);
	}
	for (int i = 0; i < pos_nums[0]; i++)
	{
		int rpos = p1[i];  // ת��string
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
				//cout << "�ҵ���:" << endl;
			}
			else
			{
				break;
			}
		}
		if (value == 1)
		{
			//p1[i]�����ս��
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
		//������ģʽ������ϣ��
		tables.push_back(tmp_table);
		//if (tmp) delete[] tmp;
	}
	for (int i = 0; i < pos_nums[0]; i++)
	{
		int rpos = p1[i];  // ת��string
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
				//cout << "�ҵ���:" << endl;
			}
			else
			{
				break;
			}
		}
		if (value == 1)
		{
			//p1[i]�����ս��
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
	//int p_length = pattern.size();               //ģʽ����
	string str = pattern;
	int p_length = length;
	int p_nums = 0;           //���ֵ���ģʽ�ĸ���
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
	//cout << "λ��" << pos;
	pos = mappos(pos);
	//cout << "test��Ӧλ����:" << pos << endl;
	//��posת����result_pos
	int bindex = pos / blockSize;
	int offset = pos % blockSize;
	int sbindex = pos / superBlockSize;
	int l = 0;            //����±�
	int ll = 0;           //�ο��Ӷ��±�
	int rank1 = DNA[number].SBrank->GetValue(sbindex) + DNA[number].Brank->GetValue(bindex);
	// step1: �ȴ�Reference��ȡ��һ����������Ӵ�>len
	// step2: ���ж���һ��λ�����ǲ��죬�����滻
	unsigned char * tmp_result = REF->extracting(pos, length + 10);
	//cout << tmp_result << endl;
	unsigned char * result = (unsigned char *)malloc(sizeof(unsigned char)*(length + 1)); 
	memset(result, 0, length + 1);
	
	int nums = 0;
	int flag = 0;
	int limit = rank1;
	int decode = DNA[number].S->GetValue(rank1);
	//����˿��е�nums
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
			break;                //�滻
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
			//cout << "�ǲ���Ŷ!" << endl;
			break;                //����
		case 2: ll++;  break;                //ɾ��
		case 3: cout << "δ��ɹ���"; break;                //���
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
	cout << "������:" << byte << "�ֽ�" << endl;
	cout << "�ܴ�С:" << totolByte << "�ֽ�" << endl;
	cout << "������:" << (double)(byte / 1024.0) << endl;
	cout << "������:" << (double)(totolByte / (1024.0*1024)) << endl;
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
	//cout << "�ܴ�С:" << totolByte << "�ֽ�" << endl;
	//cout << "������:" << (double)(totolByte / (1024.0 * 1024)) << endl;
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
int compress::select(int key)              //index �±�1��ʼ
{
	//���ö��ֲ����ҵ�����Ӧ���ڵĳ�����
	//����������жϾ�������һ��
	//����λ��
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
	//cout << "���ڳ���Ϊ:" << low - 1 << endl;;
	//cout << "����ǰ1�ĸ���:" << SBrank->GetValue(low-1) << endl;
	int spj = low - 1;
	int key2 = key - SBrank->GetValue(spj);
	low = spj*(superBlockSize / blockSize);
	//cout << "��һ��:" << low << endl;
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
	//cout << "���ڿ�Ϊ:" <<bpj << endl;;
	int decode = S->GetValue(key-1);
	//cout<<S->GetValue(989)<<endl;
	//cout << "����λ��:" << decode << endl;
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
void splitconnect()              //ƴ����һ��
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
		//itoa(pos[i], (char *)r_pos[i].c_str(), 10);       //û�з���ռ�string string ��СҲ����ȷ�� ��������
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
	int * posCC = CC->locating(pattern, nnum);  //��
	int nnn = 0;
	for (int i = 0; i < nnum; i++)
	{
		int sel = posCC[i] / (2 * q + 2); //�ڼ���%,�ڼ������� �±�0��ʼ
		int oo = posCC[i] % (2 * q + 2);    //�ڲ��촮�е�ƫ��λ�� �±�0��ʼ
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
				cout << "С��0" << tmp[j] << endl;
				exit(0);
			}
			tables[i-1][tmp[j]] = 1;
		}
		if (tmp) delete[] tmp;
	}
	for (int i = 0; i < pos_nums[0]; i++)
	{
		int rpos = p1[i];  // ת��string
		bool value = 0;
		for (int j = 0; j < pattnum - 1; j++)
		{
			rpos += q;
			if (rpos < 0)
			{
				cout << "С��02" << rpos << endl;
				exit(0);
			}
			value = tables[j][rpos];
			if (value == 1)
			{
				//cout << "�ҵ���:" << endl;
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


