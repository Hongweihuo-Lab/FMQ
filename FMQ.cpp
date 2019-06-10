#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include "FM.h"
#include<iostream>
#include "FMQ.h"
#include <ctime>
#include<fstream>
#include "perfectHash.h"
#include <sstream>
#include <algorithm>
using namespace std;
#define DIFF 160000

int * * LCS(string x, string y)
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
			}
			else
			{
				if (M[i - 1][j] > M[i][j - 1])
				{
					M[i][j] = M[i - 1][j];
				}
				else
				{
					M[i][j] = M[i][j - 1];
				}
			}
		}
	return M;
}

FMQ::FMQ(string T, string S, int Tlen, int Slen, int q)
{
	this->T = T;
	this->S = S;
	this->Tlen = Tlen;
	this->Slen = Slen;
	BV = new int[Tlen];
	this->q = q;
	blockSize = 5;
	superBlockSize = 15;
	for (int k = 0; k < Tlen; k++)
	{
		BV[k] = 0;
	}
	C = new char *[DIFF];
	for (int k = 0; k < DIFF; k++)

	{
		C[k] = new char[2 * q + 1];
	}
	F = new int[DIFF];
}

void FMQ::findDiff(int * * M, int i, int j, int l, int t)
{
	if (i == 0 && j == 0)
	{
		return;
	}

	if (i > 1)
	{
		if (M[i][j] == M[i - 1][j] && M[i][j] == M[i - 2][j])
		{
			cout << "clear!!!" << endl;
			while (!tempC.empty())
			{
				char *tmp = tempC.top();
				tempC.pop();
				if (tmp) free(tmp);
			}
			while (!tempF.empty())
			{
				tempF.pop();
			}
			for (int k = t; k >= 1; k--)
			{
				BV[l + k - 1] = 1;
				int hh = l + k - 1 - q;
				char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
				if (hh < 0)
				{
					hh = 0 - hh;
					int d = 0;
					for (int kk = 0; kk < hh; kk++)
					{
						tmp[kk] = '-';
					}
					for (int kk = hh; kk < 2 * q - 1; kk++)
					{
						tmp[kk] = S[d++];
					}
				}
				else {
					for (int kk = 0; kk < 2 * q + 1; kk++)
					{
						tmp[kk] = S[hh++];
					}
				}
				tmp[2 * q + 1] = '\0';
				tempC.push(tmp);
				tempF.push(0);
				cout << "(p,c,f)=(" << l + k - 1 << "," << tmp << ",sub)" << endl;
			}
			return;
		}
	}

	if (j > 1)
	{
		if (M[i][j] == M[i][j - 1] && M[i][j] == M[i][j - 2])
		{
			cout << "clear!!!" << endl;
			while (!tempC.empty())
			{
				char *tmp = tempC.top();
				tempC.pop();
				if (tmp) free(tmp);
			}
			while (!tempF.empty())
			{
				tempF.pop();
			}
			for (int k = t; k >= 1; k--)
			{
				BV[l + k - 1] = 1;
				int hh = l + k - 1 - q;
				char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
				if (hh < 0)
				{
					hh = 0 - hh;
					int d = 0;
					for (int kk = 0; kk < hh; kk++)
					{
						tmp[kk] = '-';
					}
					for (int kk = hh; kk < 2 * q - 1; kk++)
					{
						tmp[kk] = S[d++];
					}
				}
				else {
					for (int kk = 0; kk < 2 * q + 1; kk++)
					{
						tmp[kk] = S[hh++];
					}
				}
				tmp[2 * q + 1] = '\0';
				tempC.push(tmp);
				tempF.push(0);
				cout << "(p,c,f)=(" << l + k - 1 << "," << tmp << ",sub)" << endl;
			}
			return;
		}
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
				tmp[k] = S[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = S[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(1);
		cout << "(p,c,f)=(" << l << "," << tmp << ",ins)" << endl;
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
				tmp[k] = S[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = S[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(2);
		cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",del)" << endl;
		return;
	}
	if (M[i][j] == M[i - 1][j] && M[i][j] == M[i][j - 1])
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
				tmp[k] = S[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = S[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(0);
		cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",sub)" << endl;
		findDiff(M, i - 1, j - 1, l, t);
	}
	else if (M[i][j] == M[i - 1][j])
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
				tmp[k] = S[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = S[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(2);
		cout << "(p,c,f)=(" << l + i - 1 << "," << tmp << ",del)" << endl;
		findDiff(M,i - 1, j, l, t);
	}
	else if (M[i][j] == M[i][j - 1])
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
				tmp[k] = S[d++];
			}
		}
		else {
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = S[hh++];
			}
		}
		tmp[2 * q + 1] = '\0';
		tempC.push(tmp);
		tempF.push(1);
		cout << "(p,c,f)=(" << l + i << "," << tmp << ",ins)" << endl;
		findDiff(M, i, j - 1, l, t);
	}
	else
	{
		findDiff(M, i - 1, j - 1, l, t);
	}
}
void FMQ::getDiff()
{
	int k = 0;
	int l, r = -1;
	diffnum = 0;
	while(k<Tlen)
	{
		while (k < Tlen && S[k] == T[k])
		{
			k++;
		}
		if (k >= Tlen)
		{
			break;
		}
		l = k;
		while (k < Tlen && S[k] != T[k])
		{
			k++;
		}
		if (k >= Tlen)
		{
			r = Tlen;
		}
		r = k;

		char * x = (char *)malloc(sizeof(char)*(r - l + 1));
		char * y = (char *)malloc(sizeof(char)*(r - l + 1));
		int kk = 0;
		for (int kkk = l; kkk < r; kkk++)
		{
			x[kk] = T[kkk];
			y[kk] = S[kkk];
			kk = kk + 1;
		}
		x[kk] = '\0';
		y[kk] = '\0';

		int t = r - l;
		if (t == 1)
		{
			BV[l] = 1;
			int hh = l - q;
			char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
			for (int k = 0; k < 2 * q + 1; k++)
			{
				tmp[k] = S[hh++];
			}
			tmp[2 * q + 1] = '\0';
			C[diffnum] = tmp;
			F[diffnum] = 0;
			cout << "(p,c,f)=(" << l << "," << tmp << ",sub)" << endl;
			diffnum++;
		}
		else
		{
			int * * M = new int *[t + 1];
			for (int k = 0; k < t + 1; k++)
			{
				M[k] = new int[t + 1];
			}
			M = LCS(x, y);

			findDiff(M, t, t, l, t);
			while (!tempC.empty())
			{
				char * tmp = (char *)malloc(sizeof(char)*(2 * q + 2));
				tmp = tempC.top();
				tempC.pop();
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
	cout << "差异个数:" << diffnum << endl;

}

void FMQ::compress()
{
	//压缩上下文
	u64 all0nums = 0;
	u64 locnums = 0;
	/*上下文的压缩*/
	CC = new FM((unsigned char *)Context, diffnum*(2 * q + 2) + 2, 1);
	//CC->getAlphabetSize();
	/* F串用定长InArray编*/
	FF = new InArray(diffnum, 2);
	for (int i = 0; i < diffnum; i++)
	{
		FF->SetValue(i, F[i]);
	}

	/* BV串压缩*/

	int n = Tlen;
	int t = diffnum;
	int bitslength = log(blockSize) / log(2) + 1;
	int blocknum = n / blockSize;
	cout << "块数" << blocknum << endl;
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
	SC = new InArray(t, bitslength);
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
			SC->SetValue(k, i%blockSize);
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

int FMQ::select(int key)              //index 下标1开始
{
	int low = 0, high = SBrank->GetNum() - 1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (SBrank->GetValue(middle) > key)
		{
			high = middle - 1;
		}
		else
		{
			low = middle + 1;
		}
	}
	int spj = high;
	int key2 = key - SBrank->GetValue(spj);
	low = spj*(superBlockSize / blockSize);
	high = min((spj + 1)*(superBlockSize / blockSize) - 1, (Tlen - 1) / blockSize);
	int bpj = -1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (Brank->GetValue(middle) > key2)
		{
			high = middle - 1;
		}
		else
		{
			low = middle + 1;
		}
	}
	bpj = high;
	int decode = SC->GetValue(key);
	int pppos = (bpj*blockSize) + decode;
	return pppos;
}

void FMQ::buildContext()
{
	Context = (char *)malloc(sizeof(char)*(diffnum*(2 * q + 2) + 2));
	int t = 0;
	for (int i = 0; i < diffnum; i++)
	{
		Context[t++] = '%';
		for (int j = 0; j < 2 * q + 1; j++)
		{
			Context[t++] = C[i][j];
		}
	}
	Context[diffnum*(2 * q + 2)] = '0';
	Context[diffnum*(2 * q + 2) + 1] = '\0';
}

void FMQ::compReference(FM * ref)
{
	REF = ref;
}

int FMQ::getRefPos(int pos)
{
	cout << "参考序列位置：" << pos;
	int bindex = pos / blockSize;
	int sbindex = pos / superBlockSize;
	int offset = pos % blockSize;
	int hop = V->GetValue(bindex);
	int index = SBrank->GetValue(sbindex) + Brank->GetValue(bindex) - 1;
	for (int k = 0; k <= offset; k++)
	{
		if (access(pos - offset + k) == 1)
		{
			index = index + 1;
			if (FF->GetValue(index) == 1)
			{
				hop = hop - 1;
			}
			else if (FF->GetValue(index) == 2)
			{
				hop = hop + 1;
			}
		}
	}
	if (access(pos) == 1 && FF->GetValue(index) == 1)
	{
		if (hop == -1 || hop == 1)
		{
			//C字符
		}
	}
	cout << hop;
	if (hop > 0)
	{
		while (hop > 0)
		{
			if (access(pos + 1) == 1)
			{
				index = index + 1;
				if (FF->GetValue(index) == 0)
				{
					pos = pos + 1;  hop = hop - 1;
				}
				else if (FF->GetValue(index) == 2)
				{
					pos = pos + 1;
				}
				else
				{
					pos = pos + 1; hop = hop - 2;
				}
			}
			else
			{
				pos = pos + 1; hop = hop - 1;
			}
		}
	}
	else if (hop < 0)
	{
		while (hop < 0)
		{
			if (access(pos - 1) == 1)
			{
				index = index - 1;
				if (FF->GetValue(index) == 0)
				{
					pos = pos - 1;  hop = hop + 1;
				}
				else if (FF->GetValue(index) == 2)
				{
					pos = pos - 1;
				}
				else
				{
					pos = pos - 1; hop = hop + 2;
				}
			}
			else
			{
				pos = pos - 1; hop = hop + 1;
			}
		}
	}
	cout << hop;
	cout << "   对应的非参考序列位置：" << pos << endl;
	if (hop < 0)
	{
		//插入的字符
	}

	return pos;
}
unsigned char * FMQ::extract(unsigned char * posT, int pos, int len)
{
	int bindex = pos / blockSize;
	int sbindex = pos / superBlockSize;
	int offset = pos % blockSize;
	int hop = V->GetValue(bindex);
	int index = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
	for (int k = 0; k < offset; k++)
	{
		if (access(pos - offset + k) == 1)
		{
			index = index + 1;
			if (FF->GetValue(index) == 1)
			{
				hop = hop - 1;
			}
			else if (FF->GetValue(index) == 2)
			{
				hop = hop + 1;
			}
		}
	}
	//
	return NULL;
}

int FMQ::getnums(int bindex, int sbindex)
{
	int nums = 0;
	if (Header->GetValue(bindex) == 0)
	{
		nums = 0;
	}
	else if (bindex == (Tlen - 1) / blockSize)
	{
		nums = diffnum - SBrank->GetValue(sbindex);
	}
	else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
	{
		nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
	}
	else
	{
		nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
	}
}
bool FMQ::access(int k)
{
	int bindex = k / blockSize;
	int sbindex = k / superBlockSize;
	int offset = k % blockSize;
	int nums = 0;
	if (Header->GetValue(bindex) == 0)
	{
		return false;
	}
	else if (bindex == (Tlen - 1) / blockSize)
	{
		nums = diffnum - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
	}
	else if ((bindex + 1) % (superBlockSize / blockSize) == 0)
	{
		nums = SBrank->GetValue(sbindex + 1) - SBrank->GetValue(sbindex) - Brank->GetValue(bindex);
	}
	else
	{
		nums = Brank->GetValue(bindex + 1) - Brank->GetValue(bindex);
	}
	int prenums = SBrank->GetValue(sbindex) + Brank->GetValue(bindex);
	for (int i = 0; i < nums; i++)
	{
		if (SC->GetValue(prenums) == offset)
		{
			return true;
		}
		if (SC->GetValue(prenums) > offset)
		{
			return false;
		}
		prenums++;
	}
	return false;
}