#pragma once
#include "SmartImage.h"


class AFX_EXT_CLASS CKeyMatcher
{
public:
	CKeyMatcher(void);
	~CKeyMatcher(void);

	void ReleasePtr(void);

	FeatureDetector		*m_pDetector;
	DescriptorExtractor	*m_pExtractor;
	DescriptorMatcher	*m_pMatcher;

	vector<KeyPoint>	m_KeyPointsObject;
	Mat					m_mObject;
	Mat					m_DescriptorsObject;
	double				m_dFactor;
};

