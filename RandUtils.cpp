#include "Base.h"
#include "RandUtils.h"

NS_BEGIN

std::unique_ptr<std::mt19937_64> CRandom::s_pMT;
std::unique_ptr<std::mutex> CRandom::s_pRandMutex;

//#################################################################################################
void CRandom::Init(void)
{
	if(!s_pRandMutex)
		s_pRandMutex = std::make_unique<std::mutex>();

	if(!s_pMT)
	{
		std::random_device rd;
		s_pMT = std::make_unique<std::mt19937_64>(rd());		// Seed the Mersenne Twister with hardware-based entropy
	}
}

//#################################################################################################
void CRandom::Free(void)
{
	s_pMT.reset();
	s_pRandMutex.reset();
}

//#################################################################################################
void CRandom::Seed(const unsigned int nSeed)
{
	s_pMT->seed(nSeed);
}

//#################################################################################################
bool CRandom::GetBool(void)
{	// Mersenne twister is not thread-safe, protect with a mutex
	Assert(s_pMT->min() == 0);
	Assert(s_pMT->max() == std::numeric_limits<uint64_t>::max());

	std::lock_guard<std::mutex> lock(*s_pRandMutex);
	return ((*s_pMT)() >= 0x8000000000000000);
}

NS_END
