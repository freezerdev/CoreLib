#pragma once

#include <mutex>
#include <random>

NS_BEGIN

//#################################################################################################
class CRandom final
{
public:
	CRandom(void) = delete;
	CRandom(const CRandom &src) = delete;
	CRandom(CRandom &&src) = delete;
	~CRandom(void) = delete;

	CRandom &operator=(const CRandom &src) = delete;
	CRandom &operator=(CRandom &&src) = delete;

	static void Init(void);
	static void Free(void);
	static void Seed(const unsigned int nSeed);

	static bool GetBool(void);

	//#################################################################################################
	template<typename INTTYPE>
	static INTTYPE GetInteger(const INTTYPE nMin, const INTTYPE nMax)
	{	// Returns a non-cryptographically-secure integer in the range [nMin, nMax]
		std::uniform_int_distribution<INTTYPE> dist(nMin, nMax);

		// Mersenne twister is not thread-safe, protect with a mutex
		std::lock_guard<std::mutex> lock(*s_pRandMutex);
		return dist(*s_pMT);
	}

	//#################################################################################################
	template<typename FLOATTYPE>
	static FLOATTYPE GetFloat(const FLOATTYPE dMin, const FLOATTYPE dMax)
	{	// Returns a non-cryptographically-secure floating-point number in the range [dMin, dMax)
		std::uniform_real_distribution<FLOATTYPE> dist(dMin, dMax);

		// Mersenne twister is not thread-safe, protect with a mutex
		std::lock_guard<std::mutex> lock(*s_pRandMutex);
		return dist(*s_pMT);
	}

private:
	static std::unique_ptr<std::mt19937_64> s_pMT;
	static std::unique_ptr<std::mutex> s_pRandMutex;
};

NS_END
