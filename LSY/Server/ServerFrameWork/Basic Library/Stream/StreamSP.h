#pragma once
#include "../Include.h"
#include "Stream.h"

class CStreamSP
{
private:
	CStream* Stream;

public:
	CStreamSP() { Stream = new CStream(); }
	~CStreamSP() { delete Stream; }

	CStream* operator->() { return Stream; }
	operator CStream*() { return Stream; }
};

