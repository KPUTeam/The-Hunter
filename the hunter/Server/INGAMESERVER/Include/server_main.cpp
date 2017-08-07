#include "NetworkManager.h"

int main()
{
	if (!CNetworkManager::GetInst()->Init(PORT))
	{
		CNetworkManager::DestroyInst();
		return 0;
	}

	CNetworkManager::GetInst()->Run();
}