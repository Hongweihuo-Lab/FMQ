#include "perfectHash.h"

using namespace std;
void CPerfectHashTable::Calculate(const TKeyValueVector& data)
{
	// ----- Step 1: hash each data item and collect them into buckets.
	m_numItems = data.size();
	m_numBuckets = NumBucketsForSize(m_numItems);
	m_bucketMask = m_numBuckets - 1;
	m_salts.resize(m_numBuckets);
	m_data.resize(m_numItems);
	TKeyValueVectorBuckets buckets;
	buckets.resize(m_numBuckets);
	for (size_t i = 0; i < m_numBuckets; ++i)
		buckets[i].m_bucketIndex = i;

	for (const TKeyValuePair& pair : data) {
		size_t bucket = FirstHash(pair.first.c_str());
		buckets[bucket].m_vector.push_back(pair);
	}
	// ----- Step 2: sort buckets from most items to fewest items
	std::sort(
		buckets.begin(),
		buckets.end(),
		[](const SKeyValueVectorBucket& a, const SKeyValueVectorBucket& b) {
		return a.m_vector.size() > b.m_vector.size();
	}
	);
	// ----- Step 3: find salt values for each bucket such that when all items
	// are hashed with their bucket's salt value, that there are no collisions.
	// Note that we can stop when we hit a zero sized bucket since they are sorted
	// by length descending.
	std::vector<bool> slotsClaimed;
	slotsClaimed.resize(m_numItems);
	for (size_t bucketIndex = 0, bucketCount = buckets.size(); bucketIndex < bucketCount; ++bucketIndex) {
		if (buckets[bucketIndex].m_vector.size() == 0)
			break;
		FindSaltForBucket(buckets[bucketIndex], slotsClaimed);
	}
}
bool CPerfectHashTable::GetValue(const char* key)
{
	// do the first hash lookup and get the salt to use
	size_t bucket = FirstHash(key);
	int salt = m_salts[bucket];

	// if the salt is negative, it's absolute value minus 1 is the index to use.
	size_t dataIndex;
	if (salt < 0)
	{
		dataIndex = (size_t)((salt * -1) - 1);
	}
	// else do the second hash lookup to get where the key value pair should be
	else
		dataIndex = MurmurHash2(key, strlen(key), (unsigned int)salt) % m_data.size();
	// if the keys match, we found it, else it doesn't exist in the table
	if (m_data[dataIndex].first.compare(key) == 0)
		//return true;
		return 1;
	//return false;
	return 0;
}

unsigned int CPerfectHashTable::MurmurHash2(const void * key, int len, unsigned int seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;
	while (len >= 4)
	{
		unsigned int k = *(unsigned int *)data;
		//cout << "k" << k << endl;
		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array
	switch (len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h;
}

void CPerfectHashTable::FindSaltForBucket(const SKeyValueVectorBucket& bucket, std::vector<bool>& slotsClaimed)
{
	if (bucket.m_vector.size() == 1) {
		for (size_t i = 0, c = slotsClaimed.size(); i < c; ++i)
		{
			if (!slotsClaimed[i])
			{
				slotsClaimed[i] = true;
				m_salts[bucket.m_bucketIndex] = (i + 1)*-1;
				m_data[i] = bucket.m_vector[0];
				return;
			}
		}
		// we shouldn't ever get here
		assert(false);
	}

	// find the salt value for the items in this bucket that cause these items to take
	// only unclaimed slots
	for (int salt = 0;; ++salt) {
		// if salt ever overflows to a negative number, that's a problem.
		assert(salt >= 0);
		std::vector<size_t> slotsThisBucket;
		bool success = std::all_of(
			bucket.m_vector.begin(),
			bucket.m_vector.end(),
			[this, &slotsThisBucket, salt, &slotsClaimed](const TKeyValuePair& keyValuePair) -> bool {
			const char* key = keyValuePair.first.c_str();
			unsigned int slotWanted = MurmurHash2(key, strlen(key), (unsigned int)salt) % m_numItems;
			if (slotsClaimed[slotWanted])
				return false;
			if (std::find(slotsThisBucket.begin(), slotsThisBucket.end(), slotWanted) != slotsThisBucket.end())
				return false;
			slotsThisBucket.push_back(slotWanted);
			return true;
		}
		);

		// When we find a salt value that fits our constraints, remember the salt
		// value and also claim all the buckets.
		if (success)
		{
			m_salts[bucket.m_bucketIndex] = salt;
			for (size_t i = 0, c = bucket.m_vector.size(); i < c; ++i)
			{
				m_data[slotsThisBucket[i]] = bucket.m_vector[i];
				slotsClaimed[slotsThisBucket[i]] = true;
			}
			return;
		}
	}
}