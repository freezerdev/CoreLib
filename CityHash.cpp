#include "Base.h"
#include "CityHash.h"

NS_BEGIN

// Some primes between 2^63 and 2^64
static constexpr uint64_t g_nPrime1 = 0xC3A5C85C97CB3127;
static constexpr uint64_t g_nPrime2 = 0xB492B66FBE98F273;
static constexpr uint64_t g_nPrime3 = 0x9AE16A3B2F90404F;

// Magic numbers for 32-bit hashing, copied from Murmur3
static constexpr uint32_t g_nMagic1 = 0xCC9E2D51;
static constexpr uint32_t g_nMagic2 = 0x1B873593;

#define SHIFTMIX(a)		((a) ^ ((a) >> 47))

// Avoid shifting by 32, doing so yields an undefined result
#define ROTATE32(val, shift)	(((shift) == 0) ? (val) : (((val) >> (shift)) | ((val) << (32 - (shift)))))

// Avoid shifting by 64, doing so yields an undefined result
#define ROTATE64(val, shift)	(((shift) == 0) ? (val) : (((val) >> (shift)) | ((val) << (64 - (shift)))))

//#################################################################################################
inline uint64_t ByteSwap64(const uint64_t n) noexcept
{
	return ((n & 0xFF00000000000000) >> 56) | ((n & 0xFF000000000000) >> 40) | ((n & 0xFF0000000000) >> 24) | ((n & 0xFF00000000) >> 8) |
		((n & 0xFF000000) << 8) | ((n & 0xFF0000) << 24) | ((n & 0xFF00) << 40) | ((n & 0xFF) << 56);
}

//#################################################################################################
inline uint32_t ByteSwap32(const uint32_t n) noexcept
{
	return ((n & 0xFF000000) >> 24) | ((n & 0xFF0000) >> 8) | ((n & 0xFF00) << 8) | ((n & 0xFF) << 24);
}

//#################################################################################################
inline uint64_t Hash128to64(const uint64_t &nLow, const uint64_t &nHigh) noexcept
{
	const uint64_t nMul = 0x9DDFEA08EB382D69;
	uint64_t nA = (nLow ^ nHigh) * nMul;
	nA ^= (nA >> 47);
	uint64_t nB = (nHigh ^ nA) * nMul;
	nB ^= (nB >> 47);
	nB *= nMul;

	return nB;
}

//#################################################################################################
uint32_t Fetch32(PCBYTE pBuf)
{
	uint32_t nResult;
	std::memcpy(&nResult, pBuf, sizeof(nResult));

	return nResult;
}

//#################################################################################################
uint64_t Fetch64(PCBYTE pBuf)
{
	uint64_t nResult;
	std::memcpy(&nResult, pBuf, sizeof(nResult));

	return nResult;
}

//#################################################################################################
uint32_t fmix(uint32_t nH) noexcept
{
	nH ^= nH >> 16;
	nH *= 0x85EBCA6B;
	nH ^= nH >> 13;
	nH *= 0xC2B2AE35;
	nH ^= nH >> 16;

	return nH;
}

//#################################################################################################
uint32_t Mur(uint32_t nA, uint32_t nH) noexcept
{	// Helper from Murmur3 for combining two 32-bit values
	nA *= g_nMagic1;
	nA = ROTATE32(nA, 17);
	nA *= g_nMagic2;
	nH ^= nA;
	nH = ROTATE32(nH, 19);

	return nH * 5 + 0xE6546B64;
}

//#################################################################################################
uint32_t Hash32Len0to4(PCBYTE pBuf, uint32_t nLen) noexcept
{
	uint32_t nB = 0;
	uint32_t nC = 9;
	for(uint32_t n = 0; n < nLen; ++n)
	{
		nB = nB * g_nMagic1 + pBuf[n];
		nC ^= nB;
	}

	return fmix(Mur(nB, Mur(nLen, nC)));
}

//#################################################################################################
uint32_t Hash32Len5to12(PCBYTE pBuf, const uint32_t nLen)
{
	uint32_t nA = nLen;
	uint32_t nB = nLen * 5;
	uint32_t nC = 9;
	uint32_t nD = nB;
	nA += Fetch32(pBuf);
	nB += Fetch32(pBuf + nLen - 4);
	nC += Fetch32(pBuf + ((nLen >> 1) & 4));

	return fmix(Mur(nC, Mur(nB, Mur(nA, nD))));
}

//#################################################################################################
uint32_t Hash32Len13to24(PCBYTE pBuf, const uint32_t nLen)
{
	uint32_t nA = Fetch32(pBuf + (nLen >> 1) - 4);
	uint32_t nB = Fetch32(pBuf + 4);
	uint32_t nC = Fetch32(pBuf + nLen - 8);
	uint32_t nD = Fetch32(pBuf + (nLen >> 1));
	uint32_t nE = Fetch32(pBuf);
	uint32_t nF = Fetch32(pBuf + nLen - 4);
	uint32_t nH = nLen;

	return fmix(Mur(nF, Mur(nE, Mur(nD, Mur(nC, Mur(nB, Mur(nA, nH)))))));
}

//#################################################################################################
uint64_t Hash64Len16(const uint64_t &nU, const uint64_t &nV, const uint64_t &nMul) noexcept
{	// Murmur-inspired hashing
	uint64_t nA = (nU ^ nV) * nMul;
	nA ^= (nA >> 47);
	uint64_t nB = (nV ^ nA) * nMul;
	nB ^= (nB >> 47);
	nB *= nMul;

	return nB;
}

//#################################################################################################
uint64_t Hash64Len0to16(PCBYTE pBuf, const uint32_t nLen)
{
	uint64_t nResult = g_nPrime3;

	if(nLen >= 8)
	{
		uint64_t nMul = g_nPrime3 + (uint64_t)nLen * 2;
		uint64_t nA = Fetch64(pBuf) + g_nPrime3;
		uint64_t nB = Fetch64(pBuf + nLen - 8);
		uint64_t nC = ROTATE64(nB, 37) * nMul + nA;
		uint64_t nD = (ROTATE64(nA, 25) + nB) * nMul;

		nResult = Hash64Len16(nC, nD, nMul);
	}
	else if(nLen >= 4)
	{
		uint64_t nMul = g_nPrime3 + (uint64_t)nLen * 2;
		uint64_t nA = Fetch32(pBuf);

		nResult = Hash64Len16(nLen + (nA << 3), Fetch32(pBuf + nLen - 4), nMul);
	}
	else if(nLen != 0)
	{
		BYTE nA = pBuf[0];
		BYTE nB = pBuf[nLen >> 1];
		BYTE nC = pBuf[nLen - 1];
		uint32_t nY = (uint32_t)nA + ((uint32_t)nB << 8);
		uint32_t nZ = (uint32_t)nLen + ((uint32_t)nC << 2);

		nResult = SHIFTMIX(nY * g_nPrime3 ^ nZ * g_nPrime1) * g_nPrime3;
	}

	return nResult;
}

//#################################################################################################
uint64_t Hash64Len17to32(PCBYTE pBuf, const uint32_t nLen)
{
	uint64_t nMul = g_nPrime3 + (uint64_t)nLen * 2;
	uint64_t nA = Fetch64(pBuf) * g_nPrime2;
	uint64_t nB = Fetch64(pBuf + 8);
	uint64_t nC = Fetch64(pBuf + nLen - 8) * nMul;
	uint64_t nD = Fetch64(pBuf + nLen - 16) * g_nPrime3;

	return Hash64Len16(ROTATE64(nA + nB, 43) + ROTATE64(nC, 30) + nD, nA + ROTATE64(nB + g_nPrime3, 18) + nC, nMul);
}

//#################################################################################################
uint64_t Hash64Len33to64(PCBYTE pBuf, const uint32_t nLen)
{
	uint64_t nMul = g_nPrime3 + (uint64_t)nLen * 2;
	uint64_t nA = Fetch64(pBuf) * g_nPrime3;
	uint64_t nB = Fetch64(pBuf + 8);
	uint64_t nC = Fetch64(pBuf + nLen - 24);
	uint64_t nD = Fetch64(pBuf + nLen - 32);
	uint64_t nE = Fetch64(pBuf + 16) * g_nPrime3;
	uint64_t nF = Fetch64(pBuf + 24) * 9;
	uint64_t nG = Fetch64(pBuf + nLen - 8);
	uint64_t nH = Fetch64(pBuf + nLen - 16) * nMul;
	uint64_t nU = ROTATE64(nA + nG, 43) + (ROTATE64(nB, 30) + nC) * 9;
	uint64_t nV = ((nA + nG) ^ nD) + nF + 1;
	uint64_t nW = ByteSwap64((nU + nV) * nMul) + nH;
	uint64_t nX = ROTATE64(nE + nF, 42) + nC;
	uint64_t nY = (ByteSwap64((nV + nW) * nMul) + nG) * nMul;
	uint64_t nZ = nE + nF + nC;

	nA = ByteSwap64((nX + nZ) * nMul + nY) + nB;
	nB = SHIFTMIX((nZ + nA) * nMul + nD + nH) * nMul;

	return nB + nX;
}

//#################################################################################################
void WeakHashLen32WithSeeds(PCBYTE pBuf, uint64_t nA, uint64_t nB, uint64_t &nLow, uint64_t &nHigh)
{
	uint64_t nW = Fetch64(pBuf);
	uint64_t nX = Fetch64(pBuf + 8);
	uint64_t nY = Fetch64(pBuf + 16);
	uint64_t nZ = Fetch64(pBuf + 24);

	nA += nW;
	nB = ROTATE64(nB + nA + nZ, 21);
	uint64_t nC = nA;
	nA += nX;
	nA += nY;
	nB += ROTATE64(nA, 44);

	nLow = nA + nZ;
	nHigh = nB + nC;
}

//#################################################################################################
uint32_t CityHash32(PCBYTE pBuf, const size_t nSize)
{
	uint32_t nResult;

	if(nSize > 24)
	{
		uint32_t nH = (uint32_t)nSize;
		uint32_t nG = g_nMagic1 * (uint32_t)nSize;
		uint32_t nF = nG;
		uint32_t nA0 = ROTATE32(Fetch32(pBuf + nSize - 4) * g_nMagic1, 17) * g_nMagic2;
		uint32_t nA1 = ROTATE32(Fetch32(pBuf + nSize - 8) * g_nMagic1, 17) * g_nMagic2;
		uint32_t nA2 = ROTATE32(Fetch32(pBuf + nSize - 16) * g_nMagic1, 17) * g_nMagic2;
		uint32_t nA3 = ROTATE32(Fetch32(pBuf + nSize - 12) * g_nMagic1, 17) * g_nMagic2;
		uint32_t nA4 = ROTATE32(Fetch32(pBuf + nSize - 20) * g_nMagic1, 17) * g_nMagic2;
		nH ^= nA0;
		nH = ROTATE32(nH, 19);
		nH = nH * 5 + 0xE6546B64;
		nH ^= nA2;
		nH = ROTATE32(nH, 19);
		nH = nH * 5 + 0xE6546B64;
		nG ^= nA1;
		nG = ROTATE32(nG, 19);
		nG = nG * 5 + 0xE6546B64;
		nG ^= nA3;
		nG = ROTATE32(nG, 19);
		nG = nG * 5 + 0xE6546B64;
		nF += nA4;
		nF = ROTATE32(nF, 19);
		nF = nF * 5 + 0xE6546B64;

		size_t n = (nSize - 1) / 20;
		do{
			uint32_t nA5 = ROTATE32(Fetch32(pBuf) * g_nMagic1, 17) * g_nMagic2;
			uint32_t nA6 = Fetch32(pBuf + 4);
			uint32_t nA7 = ROTATE32(Fetch32(pBuf + 8) * g_nMagic1, 17) * g_nMagic2;
			uint32_t nA8 = ROTATE32(Fetch32(pBuf + 12) * g_nMagic1, 17) * g_nMagic2;
			uint32_t nA9 = Fetch32(pBuf + 16);
			nH ^= nA5;
			nH = ROTATE32(nH, 18);
			nH = nH * 5 + 0xE6546B64;
			nF += nA6;
			nF = ROTATE32(nF, 19);
			nF = nF * g_nMagic1;
			nG += nA7;
			nG = ROTATE32(nG, 18);
			nG = nG * 5 + 0xE6546B64;
			nH ^= nA8 + nA6;
			nH = ROTATE32(nH, 19);
			nH = nH * 5 + 0xE6546B64;
			nG ^= nA9;
			nG = ByteSwap32(nG) * 5;
			nH += nA9 * 5;
			nH = ByteSwap32(nH);
			nF += nA5;
			uint32_t nTmp = nG;
			nG = nH;
			nH = nF;
			nF = nTmp;
			pBuf += 20;
		}while(--n != 0);

		nG = ROTATE32(nG, 11) * g_nMagic1;
		nG = ROTATE32(nG, 17) * g_nMagic1;
		nF = ROTATE32(nF, 11) * g_nMagic1;
		nF = ROTATE32(nF, 17) * g_nMagic1;
		nH = ROTATE32(nH + nG, 19);
		nH = nH * 5 + 0xE6546B64;
		nH = ROTATE32(nH, 17) * g_nMagic1;
		nH = ROTATE32(nH + nF, 19);
		nH = nH * 5 + 0xE6546B64;
		nH = ROTATE32(nH, 17) * g_nMagic1;

		nResult = nH;
	}
	else if(nSize > 12)
		nResult = Hash32Len13to24(pBuf, (uint32_t)nSize);
	else if(nSize > 4)
		nResult = Hash32Len5to12(pBuf, (uint32_t)nSize);
	else if(nSize > 0)
		nResult = Hash32Len0to4(pBuf, (uint32_t)nSize);
	else
		nResult = 0;

	return nResult;
}

//#################################################################################################
uint64_t CityHash64(PCBYTE pBuf, const size_t nSize)
{
	uint64_t nResult;

	if(nSize > 64)
	{
		uint64_t nX = Fetch64(pBuf + nSize - 40);
		uint64_t nY = Fetch64(pBuf + nSize - 16) + Fetch64(pBuf + nSize - 56);
		uint64_t nZ = Hash128to64(Fetch64(pBuf + nSize - 48) + nSize, Fetch64(pBuf + nSize - 24));
		uint64_t nVLow;
		uint64_t nVHigh;
		uint64_t nWLow;
		uint64_t nWHigh;
		WeakHashLen32WithSeeds(pBuf + nSize - 64, nSize, nZ, nVLow, nVHigh);
		WeakHashLen32WithSeeds(pBuf + nSize - 32, nY + g_nPrime2, nX, nWLow, nWHigh);

		nX = nX * g_nPrime2 + Fetch64(pBuf);

		size_t nSizeRemaining = (nSize - 1) & ~63;
		do{
			nX = ROTATE64(nX + nY + nVLow + Fetch64(pBuf + 8), 37) * g_nPrime2;
			nY = ROTATE64(nY + nVHigh + Fetch64(pBuf + 48), 42) * g_nPrime2;
			nX ^= nWHigh;
			nY += nVLow + Fetch64(pBuf + 40);
			nZ = ROTATE64(nZ + nWLow, 33) * g_nPrime2;
			WeakHashLen32WithSeeds(pBuf, nVHigh * g_nPrime2, nX + nWLow, nVLow, nVHigh);
			WeakHashLen32WithSeeds(pBuf + 32, nZ + nWHigh, nY + Fetch64(pBuf + 16), nWLow, nWHigh);
			nZ ^= nX;
			nX ^= nZ;
			nZ ^= nX;
			pBuf += 64;
			nSizeRemaining -= 64;
		}while(nSizeRemaining);

		nResult = Hash128to64(Hash128to64(nVLow, nWLow) + SHIFTMIX(nY) * g_nPrime2 + nZ, Hash128to64(nVHigh, nWHigh) + nX);
	}
	else if(nSize > 32)
		nResult = Hash64Len33to64(pBuf, (uint32_t)nSize);
	else if(nSize > 16)
		nResult = Hash64Len17to32(pBuf, (uint32_t)nSize);
	else if(nSize > 0)
		nResult = Hash64Len0to16(pBuf, (uint32_t)nSize);
	else
		nResult = 0;

	return nResult;
}

NS_END
