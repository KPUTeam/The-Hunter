#include "Include.h"
#include "MemoryPool\MemoryPool.h"

class CDataMP : public CMemoryPool<CDataMP>
{
private:
	BYTE a[1024];
	BYTE b[1024];
};

int main()
{
	CDataMP* pData = new CDataMP();

	delete pData;
}