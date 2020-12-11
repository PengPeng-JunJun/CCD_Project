#include "stdafx.h"
#include "KeyMatcher.h"


CKeyMatcher::CKeyMatcher(void)
	: m_pDetector(nullptr)
	, m_pExtractor(nullptr)
	, m_pMatcher(nullptr)
	, m_dFactor(2)
{
}


CKeyMatcher::~CKeyMatcher(void)
{
	ReleasePtr();
}


void CKeyMatcher::ReleasePtr(void)
{
	delete m_pDetector;
	m_pDetector = nullptr;

	delete m_pExtractor;
	m_pExtractor = nullptr;

	delete m_pMatcher;
	m_pMatcher = nullptr;
}
