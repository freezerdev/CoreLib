#include "Base.h"
#include "Object.h"
#include <CoreFoundation/CFNumber.h>

NS_BEGIN

/////////////////////////////////////////////////
// CCFObject
/////////////////////////////////////////////////

//#################################################################################################
CCFObject::CCFObject(void)
	: m_cf(nullptr),
	m_bAutoRelease(false)
{
}

// #################################################################################################
CCFObject::CCFObject(const CCFObject &src)
	: m_cf(nullptr),
	m_bAutoRelease(false)
{
	if(src.m_cf)
	{
		m_cf = CFRetain(src.m_cf);
		m_bAutoRelease = src.m_bAutoRelease;
	}
}

//#################################################################################################
CCFObject::CCFObject(CCFObject &&src) noexcept
	: m_cf(src.m_cf),
	m_bAutoRelease(src.m_bAutoRelease)
{
	src.m_cf = nullptr;
	src.m_bAutoRelease = false;
}

//#################################################################################################
CCFObject::CCFObject(const CFTypeRef cf, const bool bAutoRelease)
	: m_cf(cf),
	m_bAutoRelease(bAutoRelease)
{
}

//#################################################################################################
CCFObject::~CCFObject(void)
{
	Release();
}

//#################################################################################################
bool CCFObject::IsValid(void) const noexcept
{
	return (m_cf != nullptr);
}

//#################################################################################################
void CCFObject::Release(void)
{
	if(m_bAutoRelease && m_cf)
		CFRelease(m_cf);

	m_cf = nullptr;
	m_bAutoRelease = false;
}

//#################################################################################################
CCFObject &CCFObject::Attach(const CFTypeRef cf, const bool bAutoRelease)
{
	Release();

	if(cf)
	{
		m_cf = cf;
		m_bAutoRelease = bAutoRelease;
	}

	return *this;
}

//#################################################################################################
CFTypeRef CCFObject::Detach(bool *pbAutoRelease) noexcept
{
	CFTypeRef cf = m_cf;
	if(pbAutoRelease)
		*pbAutoRelease = m_bAutoRelease;

	m_cf = nullptr;
	m_bAutoRelease = false;
	return cf;
}

//#################################################################################################
CCFObject::operator CFTypeRef(void) const noexcept
{
	return m_cf;
}

//#################################################################################################
CFTypeRef CCFObject::Get(void) const noexcept
{
	return m_cf;
}

//#################################################################################################
CFTypeRef &CCFObject::Receive(const bool bAutoRelease) noexcept
{
	Release();
	m_bAutoRelease = bAutoRelease;
	return m_cf;
}

//#################################################################################################
CCFObject &CCFObject::operator=(const CCFObject &src)
{
	if(this != &src)
	{
		Release();
		if(src.m_cf)
		{
			m_cf = CFRetain(src.m_cf);
			m_bAutoRelease = src.m_bAutoRelease;
		}
	}

	return *this;
}

//#################################################################################################
CCFObject &CCFObject::operator=(CCFObject &&src) noexcept
{
	if(this != &src)
	{
		Release();
		if(src.m_cf)
		{
			m_cf = src.m_cf;
			m_bAutoRelease = src.m_bAutoRelease;

			src.m_cf = nullptr;
			src.m_bAutoRelease = false;
		}
	}

	return *this;
}

//#################################################################################################
CFTypeID CCFObject::GetType(void) const
{
	return (m_cf) ? CFGetTypeID(m_cf) : 0;
}

//#################################################################################################
bool CCFObject::GetBoolean(const bool bDefault) const
{
	return (GetType() == CFBooleanGetTypeID()) ? (CFBooleanGetValue((CFBooleanRef)m_cf) != false) : bDefault;
}

//#################################################################################################
int64_t CCFObject::GetInteger(const int64_t nDefault) const
{
	int64_t n = nDefault;

	if(GetType() == CFNumberGetTypeID())
	{
#ifdef _DEBUG
		auto eType = CFNumberGetType((CFNumberRef)m_cf);
		Assert(eType == kCFNumberSInt8Type || eType == kCFNumberSInt16Type || eType == kCFNumberSInt32Type || eType == kCFNumberSInt64Type);
#endif
		Verify(CFNumberGetValue((CFNumberRef)m_cf, kCFNumberSInt64Type, &n) != 0);
	}

	return n;
}

//#################################################################################################
CStr CCFObject::GetString(PCSTR szDefault) const
{
	CStr str(szDefault);

	if(GetType() == CFStringGetTypeID())
	{
		auto nLength = CFStringGetLength((CFStringRef)m_cf);
		if(nLength)
		{
			auto nMaxLength = CFStringGetMaximumSizeForEncoding(nLength, kCFStringEncodingUTF8);
			if(nMaxLength && nMaxLength != kCFNotFound)
			{
				if(CFStringGetCString((CFStringRef)m_cf, str.GetBuffer(nMaxLength), nMaxLength, kCFStringEncodingUTF8))
					str.ReleaseBuffer(nLength);
				else
					str.Empty(true);
			}
		}
	}

	return str;
}

//#################################################################################################
CMemBuffer CCFObject::GetData(void) const
{
	CMemBuffer buf;

	if(GetType() == CFDataGetTypeID())
	{
		auto nSize = CFDataGetLength((CFDataRef)m_cf);
		if(nSize)
		{
			buf.SetBufferSize(nSize);
			CFDataGetBytes((CFDataRef)m_cf, CFRangeMake(0, nSize), buf.GetBuffer());
			buf.SetDataSize(nSize);
		}
	}

	return buf;
}

//#################################################################################################
size_t CCFObject::GetArrayCount(void) const
{
	return (GetType() == CFArrayGetTypeID()) ? (size_t)CFArrayGetCount((CFArrayRef)m_cf) : 0;
}

//#################################################################################################
CCFObject CCFObject::GetArrayObject(const size_t nIndex) const
{
	Assert(GetType() == CFArrayGetTypeID());
	return CCFObject(CFArrayGetValueAtIndex((CFArrayRef)m_cf, nIndex), false);
}

//#################################################################################################
CCFObject CCFObject::GetDictionaryEx(PCSTR szProperty) const
{
	CCFObject cf;

	if(GetType() == CFDictionaryGetTypeID())
	{
		CCFObject cfKey = CFStringCreateWithCString(kCFAllocatorDefault, szProperty, kCFStringEncodingUTF8);
		if(cfKey)
			cf.Attach(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfKey), false);
	}

	return cf;
}

//#################################################################################################
bool CCFObject::GetDictionaryBooleanEx(PCSTR szProperty, const bool bDefault) const
{
	bool b = bDefault;

	if(GetType() == CFDictionaryGetTypeID())
	{
		CCFObject cfKey = CFStringCreateWithCString(kCFAllocatorDefault, szProperty, kCFStringEncodingUTF8);
		if(cfKey)
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfKey), false);
			b = cfValue.GetBoolean();
		}
	}

	return b;
}

//#################################################################################################
int64_t CCFObject::GetDictionaryIntegerEx(PCSTR szProperty, const int64_t nDefault) const
{
	int64_t n = nDefault;

	if(GetType() == CFDictionaryGetTypeID())
	{
		CCFObject cfKey = CFStringCreateWithCString(kCFAllocatorDefault, szProperty, kCFStringEncodingUTF8);
		if(cfKey)
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfKey), false);
			n = cfValue.GetInteger();
		}
	}

	return n;
}

//#################################################################################################
CStr CCFObject::GetDictionaryStringEx(PCSTR szProperty, PCSTR szDefault) const
{
	CStr str(szDefault);

	if(GetType() == CFDictionaryGetTypeID())
	{
		CCFObject cfKey = CFStringCreateWithCString(kCFAllocatorDefault, szProperty, kCFStringEncodingUTF8);
		if(cfKey)
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfKey), false);
			str = cfValue.GetString();
		}
	}

	return str;
}

//#################################################################################################
CMemBuffer CCFObject::GetDictionaryDataEx(PCSTR szProperty) const
{
	CMemBuffer buf;

	if(GetType() == CFDictionaryGetTypeID())
	{
		CCFObject cfKey = CFStringCreateWithCString(kCFAllocatorDefault, szProperty, kCFStringEncodingUTF8);
		if(cfKey)
		{
			CCFObject cfValue(CFDictionaryGetValue((CFDictionaryRef)m_cf, cfKey), false);
			buf = cfValue.GetData();
		}
	}

	return buf;
}




/////////////////////////////////////////////////
// CIOObject
/////////////////////////////////////////////////

//#################################################################################################
CIOObject::CIOObject(void)
	: m_io(0)
{
}

//#################################################################################################
CIOObject::CIOObject(const CIOObject &src)
	: m_io(0)
{
	if(src.m_io && IOObjectRetain(src.m_io) == KERN_SUCCESS)
		m_io = src.m_io;
}

//#################################################################################################
CIOObject::CIOObject(CIOObject &&src) noexcept
	: m_io(src.m_io)
{
	src.m_io = 0;
}

//#################################################################################################
CIOObject::CIOObject(const io_object_t io)
	: m_io(io)
{
}

//#################################################################################################
CIOObject::~CIOObject(void)
{
	Release();
}

//#################################################################################################
bool CIOObject::IsValid(void) const noexcept
{
	return (m_io != 0);
}

//#################################################################################################
void CIOObject::Release(void)
{
	if(m_io)
	{
		IOObjectRelease(m_io);
		m_io = 0;
	}
}

//#################################################################################################
CIOObject &CIOObject::Attach(io_object_t io)
{
	Release();

	if(io)
		m_io = io;

	return *this;
}

//#################################################################################################
io_object_t CIOObject::Detach(void) noexcept
{
	io_object_t io = m_io;
	m_io = 0;
	return io;
}

//#################################################################################################
CIOObject::operator io_object_t(void) const noexcept
{
	return m_io;
}

//#################################################################################################
io_object_t CIOObject::Get(void) const noexcept
{
	return m_io;
}

//#################################################################################################
io_object_t &CIOObject::Receive(void) noexcept
{
	Release();
	return m_io;
}

//#################################################################################################
CIOObject &CIOObject::operator=(const CIOObject &src)
{
	if(this != &src)
	{
		Release();
		if(src.m_io)
		{
			IOObjectRelease(src.m_io);
			m_io = src.m_io;
		}
	}

	return *this;
}

//#################################################################################################
CIOObject &CIOObject::operator=(CIOObject &&src) noexcept
{
	if(this != &src)
	{
		Release();
		if(src.m_io)
		{
			m_io = src.m_io;
			src.m_io = 0;
		}
	}

	return *this;
}

//#################################################################################################
CStr CIOObject::GetName(void) const
{
	CStr str;

	if(m_io)
	{
		io_name_t ioName;
		if(IORegistryEntryGetName(m_io, ioName) == KERN_SUCCESS)
			str = (PCSTR)ioName;
	}

	return str;
}

NS_END
