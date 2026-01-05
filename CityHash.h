#pragma once

NS_BEGIN

// Based upon CityHash version 1.1.1, copyright 2011 Google, Inc.
// by Geoff Pike and Jyrki Alakuijala

uint32_t CityHash32(PCBYTE pBuf, const size_t nSize);

uint64_t CityHash64(PCBYTE pBuf, const size_t nSize);

NS_END
