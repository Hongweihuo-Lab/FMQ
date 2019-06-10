/*============================================
# Filename: BitMap.h
# Ver 1.0 2014-06-08
# Copyright (C) 2014 ChenLonggang (chenlonggang.love@163.com)
#
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 or later of the License.
#
# Description: Hybrid-bitmap,support rank(int pos) and rank(int pos,int &bit)
=============================================*/
#ifndef WT_NODE_H
#define WT_NODE_H
#include<string.h>
#include"loadkit.h"
#include"savekit.h"
#include"InArray.h"
#include"BaseClass.h"
#include<math.h>
#include<iostream>
using namespace std;
class BitMap
{
	public:
		int NumForGamma;/////////暂停
		
		
		BitMap(unsigned long long int * bitbuff,int bit_len,int level,int block_size=1024,unsigned char label='\0',uchar ** tables=NULL);
		
		
		BitMap(){};
		BitMap(uchar ** tables):Z(tables[0]),R(tables[1]){}
		~BitMap();

		int Rank(int pos);
		int Rank(int pos,int &bit);
		void Rank(int pos_left,int pos_right,int &rank_left,int &rank_right);


		void Left(BitMap * left);
	
		BitMap * Left(){return left;};
	

		void Right(BitMap * right);


		BitMap * Right(){return right;};


		unsigned char Label();
		int Load(loadkit & s);
		int Save(savekit & S);
		int SizeInByte();
	private:
		uchar* Z;
		//uchar* R1;
		//uchar* R2;
		//uchar* R3;
		//uchar* R4;
		uchar *R;
		BitMap(const BitMap &);
		BitMap & operator =(const BitMap& right);
		void Coding();
		int GetBit(u64 * data,int index);

		u16 Zeros(u16 x){return (Z[x>>8]==8)?Z[x>>8]+Z[(uchar)x]:Z[x>>8];}
		u64 GetBits(u64 * buff,int &index,int bits);
	
	
		int GetZerosRuns(u64 * buff,int &index);

		

		int FixedDecode(u64 * buff,int &index);
		int GammaDecode(u64 * buff,int &index);
 		
		
		int GetRuns(u64 * data,int &index,int &bit);
		
		void Append_g(u64 * temp,int &index,u32 value);

		void BitCopy(u64 * temp,int &index,u64 value);

		void RL_Rank(u64 * buff,int &index,int bits_left,int bits_right,int &rank_left,int &rank_right,int rl_type);
		int  RL_Rank(u64 * buff,int &index,int bits_num,int rl_type);
		int  RL_Rank(u64 * buff,int &index,int bits_num,int rl_type,int &bit);
	
		int RL0_Rank(u64 * buff,int &index,int bits_num);
		int RL0_Bit(u64 * buff,int &index,int bits);
		int RL0_Rank(u64 * buff,int &index,int bits,int &bit);
		
		int RL1_Rank(u64 * buff,int &index,int bits);
		int RL1_Bit(u64 * buff,int & index,int bits);
		int RL1_Rank(u64 * buff,int &index,int bits,int &bit);
	
		void Plain_Rank(u64 *buff,int &index,int bits_left,int bits_right,int &rank_left,int&rank_right);
		int Plain_Rank(u64 * buff,int &index,int bits);
		int Plain_Bit(u64 * buff,int &index,int bits);
		int Plain_Rank(u64 * buff,int &index,int bits,int & bit);

		
		int level;
		
		unsigned char label;
		

		unsigned long long int * data;
	

		int bitLen;
	

		int memorysize;
		int block_size;
		int block_width;
		int super_block_size;

		BitMap * left;
		BitMap * right;

		InArray *superblock;
		InArray *block;

		InArray *coding_style;
		
		unsigned long long int * buff;
};

#endif







