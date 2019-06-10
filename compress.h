#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include <stack>
#include "FM.h"
#include "InArray.h"
#include "perfectHash.h"
using namespace std;

#define SUM 10

struct sequence
{
	InArray * S;                   //the compresses of BV
	InArray * Brank;
	InArray * SBrank;
	InArray * Header;         //the flag of block  all0 is 0 else is 1
	InArray * V;                  //the offset of position in BV and to-be-compressed sequence  
	FM * CC;                //上下文FM， 压缩完Context后释放             the real stored C
	InArray * FF;          //真正存储的差异类型， 赋值结束后释放F      the type of difference (sub is 0 ins is 1 del is 2)
	int diffnum; //差异个数                  the nums of difference
};
class compress
{
public:
	compress(const char * filename, int nums, int length);
	compress(char * s1, char * s2,char * filename, int m1, int m2);
	compress(char * s1, char * s2,char * filename, int m1, int m2, int q);	
	compress(char * s1,char * s2,int blockSize,int superBlockSize,int q);
	compress(string s2, string s3, string s4, string s5);
	char * s1;
	char * s2;
	char * reference;
	void find2Globol(int * * M, int * * b, string s2, int i, int j, int l);
	sequence * DNA;
	char * Filename;
	int DNAnums = 0;
	int max1;   //参考序列长度      the length of reference
	int max2;   //待压缩序列长度   the length of to-be-compressed sequence
	int blockSize;   //块长度          the length of block
	int superBlockSize;  //超块长度     the length of superblock
	int q;   //上下文长度                       the length of context
	int diffnum; //差异个数                  the nums of difference
	int testallN = 0;
	int * BV;            
	char * *C;
	int * F;                   //差异类型
	InArray * S;                   //the compresses of BV
	InArray * Brank;
	InArray * SBrank;          
	InArray * Header;         //the flag of block  all0 is 0 else is 1
	InArray * V;                  //the offset of position in BV and to-be-compressed sequence  
	FM * CC;                //上下文FM， 压缩完Context后释放             the real stored C
	FM * REF;
	InArray * FF;          //真正存储的差异类型， 赋值结束后释放F      the type of difference (sub is 0 ins is 1 del is 2)
	char * Context;     //差异连成的串
	int chashannum = 0;  //插入删除个数        the nums of (ins and del)
	//char * tempC;
	stack<char *> tempC;              //the temp stack of C 
	stack<int> tempF;                   //the temp stack of F
	int testmul = 0;
	int insflag = 0;         //表示要取的是插入的字符还是之后的字符
public:
	void map();                        //match reference and to-be-compressed sequence to get difference
	void mapKorea();                        //match reference and to-be-compressed sequence to get difference
	void mapGlobolKorea(int length);                        //match reference and to-be-compressed sequence to get difference
	int mapmul(char * s1, char * s2);                        //match reference and to-be-compressed sequence to get difference
	void find(int * * M, string y, int i, int j, int l);
	void find2(int * * M, int * * b, string y, int i, int j, int l);
	void find3(int * * M, int * * b, string y, int i, int j, int l);
	bool verify();                   //verify the string == ? to-be-compressed sequence  if != return false 
	void connect();               //get Context made of C
	void compreMethod();   //compress Context to FM
	void compreMethod2();   //compress Context to FM
	unsigned char * extract(int pos,int length);   
	unsigned char * testextract(int pos, int length, int number);
	void compReference();
	int mappos(int pos);
	int * locating(char * pattern,int length, int & num);
	void locatingadd(char * pattern, int length, int & num,  int * pos1, int nnum, vector<int> &resultpos);
	int  *locatingadd(char * pattern, int length, int & num,  int * pos1, int nnum);
	
	vector<int> locate2(char * pattern, int length, int & num);
	vector<int> locate2add(char * pattern, int length, int & num);
	//vector<int> locate3add(int * p11, string * patts2, int pattnum2, vector<int *> tablesRef2, 
	//int & num, int * flagpos_nums);
	void locate3add(int * p11, string * patts, int &pattnum, vector<int* >  &tablesRef, int &num, int * flagpos_nums, vector<int> &resultpos);	
	vector<int> locate3add(int * p11, string * patts2, int &pattnum2, vector<int *> &tablesRef2, int & num, int * flagpos_nums);
	
	string* split(char * pattern, int length,int & pattnum);
	bool verifyREF(int pos, int length);
	int sizeInByte();
	int sizeInByte1();
	int sizeInByteP();
	int sizeInByteSC();
	int sizeInByteBrank();
	int sizeInByteC();
	int sizeInByteF();
	double compressRatio();
	double compressRatio2();
	int select(int index);       //第i个差异对应的待压缩序列上的位置
	int ff(int low, int high, int key);
	int getN();
	CPerfectHashTable createHashTable(int pos[], int length);
	void transferstring(int * pos, int length);
};