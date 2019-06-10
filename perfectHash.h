#pragma once
#include "stdafx.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <string>
#include <iostream>
#include <time.h>

using namespace std;

class CPerfectHashTable {
public:
	typedef std::pair<string, int> TKeyValuePair;
	typedef std::vector<TKeyValuePair> TKeyValueVector;
	struct SKeyValueVectorBucket {
		TKeyValueVector m_vector;
		size_t          m_bucketIndex;
	};
	typedef std::vector<struct SKeyValueVectorBucket> TKeyValueVectorBuckets;
	void Calculate(const TKeyValueVector& data);
	bool GetValue(const char* key);

private:
	unsigned int MurmurHash2(const void * key, int len, unsigned int seed);
	unsigned int FirstHash(const char* key) {
		return MurmurHash2(key, strlen(key), 435) & m_bucketMask;
	}
	void FindSaltForBucket(const SKeyValueVectorBucket& bucket, std::vector<bool>& slotsClaimed);
	static size_t NumBucketsForSize(size_t size) {
		if (!size)
			return 1;

		size_t ret = 1;
		size = size >> 1;
		while (size) {
			ret = ret << 1;
			size = size >> 1;
		}
		return ret;
	}
	std::vector<int> m_salts;
	std::vector<TKeyValuePair> m_data;
	size_t m_numItems;
	size_t m_numBuckets;
	size_t m_bucketMask;
};
