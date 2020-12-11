#pragma once

#include "ResizeRect.h"

class AFX_EXT_CLASS CResizeElipse : public CResizeRect
{
public:
	CResizeElipse(void);
	~CResizeElipse(void);

	virtual void Draw(CDC * pDC);
};

