
#include "NetworkManager.h"


int main()
{
	if (!GET_SINGLE(CNetworkManager)->Init())
	{
		DESTROY_SINGLE(CNetworkManager);
		return 0;
	}

	GET_SINGLE(CNetworkManager)->Run();

	DESTROY_SINGLE(CNetworkManager);
	
	return 0;
}