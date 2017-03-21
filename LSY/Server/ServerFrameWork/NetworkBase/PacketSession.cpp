#include "PacketSession.h"
#include "../System Library/Crypt/Crypt.h"



CPacketSession::CPacketSession()
{
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength = 0;
	mCurrentPacketNumber = 0;

	mLastReadPacketNumber = 0;
}


CPacketSession::~CPacketSession()
{
}

BOOL CPacketSession::Begin()
{
	CThreadSync	Sync;
	// NetworkSession에서 가져온 데이터를 패킷 분석 전에 담아둘 버퍼를 초기화
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;

	if (!WriteQueue.Begin())
		return FALSE;

	return CNetworkSession::Begin();
}

BOOL CPacketSession::End()
{
	CThreadSync	Sync;
	// UDP는 TCP와 달리 하나의 접속에 한 개체의 개념으로 접근하지 앟고
	// 한 개체가 여러 접속을 관리하므로 받은 곳 패킷 번호를 관리해야한다.
	mLastReadPacketInfoVectorForUDP.clear();

	if (!WriteQueue.End())
		return FALSE;

	return CNetworkSession::End();
}

BOOL CPacketSession::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync	Sync;

	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return false;

	mRemainLength += readLength;

	return true;
}

BOOL CPacketSession::ReadPacketForEventSelect()
{
	CThreadSync	Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadForEventSelect(mPacketBuffer + mRemainLength, ReadLength))
		return false;

	mRemainLength += ReadLength;

	return true;
}

BOOL CPacketSession::ReadFromPacketForIocp(LPSTR remoteAddress, USHORT & remotePort, DWORD readLength)
{
	// NetworkSession 클래스의 버퍼에서 
	// PacketSession 클래스로 데이터를 복사하는 함수
	CThreadSync	Sync;

	if (!CNetworkSession::ReadFromForIocp(remoteAddress, remotePort, mPacketBuffer, readLength))
		return false;

	mRemainLength += readLength;

	return true;
}

BOOL CPacketSession::ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT & remotePort)
{
	CThreadSync	Sync;

	DWORD	ReadLength = 0;

	if (!CNetworkSession::ReadFromForEventSelect(remoteAddress, remotePort, mPacketBuffer, ReadLength))
		return false;

	mRemainLength += ReadLength;

	return true;
}

BOOL CPacketSession::WritePacket(DWORD protocol, const BYTE * packet, DWORD packetLength)
{
	// 보낼 데이터에 각종 정보를 넣어 패킷으로 만들어 보내는 기능을 하는 함수
	// 길이, 프로토콜, 패킷 번호 등을 붙여서 데이터를 보내게 된다.
	// 보내기 전에 따로 큐에 데이터를 넣고 데이터 보내기가 완료되었을때
	// WriteComplete 함수로 데이터를 삭제하기 위해서이다.
	CThreadSync	Sync;

	if (!packet)
		return false;

	DWORD PacketLength = sizeof(DWORD) +
		sizeof(DWORD) +
		sizeof(DWORD) + packetLength;

	if (PacketLength >= MAX_BUFFER_LENGTH)
		return false;

	mCurrentPacketNumber++;

	BYTE	TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// 패킷에 인덱스를 붙여 순서 보정을 해 주어야 한다.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	memcpy(TempBuffer + sizeof(DWORD), &mCurrentPacketNumber, sizeof(DWORD));

	memcpy(TempBuffer + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD), packet, packetLength);

	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue를 이용해서 패킷이 전송 완료가 되었을까지 메모리를 살려둔다.
	// 데이터를 보내는 중 메모리가 삭제되면 문제가 되기 때문이다.
	// TempBuffer는 지역변수라서 함수가 종료되는 순간 포인터가 사라짐
	BYTE* WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::Write(WriteData, PacketLength);
}

BOOL CPacketSession::WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE * packet, DWORD packetLength)
{
	CThreadSync	Sync;

	if (!packet)
		return false;

	DWORD PacketLength = sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) + packetLength;

	if (PacketLength >= MAX_BUFFER_LENGTH)
		return false;

	mCurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// 패킷에 인덱스를 붙여 순서 보정을 해주어야 한다.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	memcpy(TempBuffer + sizeof(DWORD), &mCurrentPacketNumber, sizeof(DWORD));

	memcpy(TempBuffer + sizeof(DWORD) + sizeof(DWORD), &protocol, sizeof(DWORD));

	memcpy(TempBuffer + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD), packet, packetLength);

	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength);

	BYTE* WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::WriteTo(remoteAddress, remotePort, WriteData, PacketLength);
}



BOOL CPacketSession::WriteComplete()
{
	// 보내기 완료가 되었을 때 큐에서 데이터를 삭제하는 함수
	CThreadSync	Sync;

	return WriteQueue.Pop();
}

BOOL CPacketSession::ResetUDP()
{
	CThreadSync	Sync;

	mLastReadPacketInfoVectorForUDP.clear();

	return true;
}

BOOL CPacketSession::GetPacket(DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	// 받은 데이터를 확인해서 정상적인 패킷일 경우 패킷을 분석하는 역할
	// NetworkSession 클래스를 통해서 받은 데이터를 PacketSession 클래스로 카피하고
	// 그 내용이 최소 4바이트 이상일 경우, 즉 패킷의 사이즈를 알 수 있을 경우 
	// 우선 앞의 4바이트를 읽어 패킷의 길이를 알고 이후 데이터가 패킷 길이만큼 있을 경우
	// 프로토콜 번호, 패킷 번호, 복호화 등의 작업을 하고 하나의 완성된 패킷을 출력
	CThreadSync	Sync;

	if (!packet)
		return false;

	if (mRemainLength < sizeof(DWORD))
		return false;

	int PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(int));

	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0)
	{
		mRemainLength = 0;
		return FALSE;
	}

	if (PacketLength <= mRemainLength)
	{
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD),
			mPacketBuffer + sizeof(DWORD),
			PacketLength - sizeof(DWORD));

		DWORD	PacketNumber = 0;
		DWORD	Protocol = 0;

		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD), sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer +
			sizeof(DWORD) + sizeof(DWORD), sizeof(DWORD));

		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD) -	// Length
			sizeof(DWORD) - // PacketNumber
			sizeof(DWORD);  // Protocol

		memcpy(packet, mPacketBuffer +
			sizeof(DWORD) +
			sizeof(DWORD) +
			sizeof(DWORD), packetLength);

		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		mRemainLength -= PacketLength;

		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		if (PacketNumber <= mLastReadPacketNumber)
			return false;

		else
		{
			mLastReadPacketNumber = PacketNumber;
			return true;
		}
	}
	return false;
}

BOOL CPacketSession::GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	CThreadSync	Sync;

	if (!packet)
		return false;

	if (mRemainLength < sizeof(DWORD))
		return false;

	int PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(int));

	if (PacketLength > MAX_BUFFER_LENGTH || packetLength <= 0)
	{
		mRemainLength = 0;
		return false;
	}

	if (PacketLength <= mRemainLength)
	{
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD),
			mPacketBuffer + sizeof(DWORD),
			PacketLength - sizeof(DWORD));

		DWORD	PacketNumber = 0;
		DWORD	Protocol = 0;

		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD), sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer +sizeof(DWORD) + sizeof(DWORD), sizeof(DWORD));

		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD) -
			sizeof(DWORD) -
			sizeof(DWORD);

		memcpy(packet, mPacketBuffer + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD), packetLength);

		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		mRemainLength -= PacketLength;

		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		for (DWORD i = 0; i < mLastReadPacketInfoVectorForUDP.size(); ++i)
		{
			READ_PACKET_INFO ReadPacketInfo = mLastReadPacketInfoVectorForUDP[i];

			// 같은 주소, 같은 포트의 정보가 이미 벡터에 있는지 검사한다.
			if (!strcmp(ReadPacketInfo.RemoteAddress, remoteAddress) && ReadPacketInfo.RemotePort == remotePort)
			{
				if (ReadPacketInfo.PacketNumber < PacketNumber)
				{
					mLastReadPacketInfoVectorForUDP[i].PacketNumber = PacketNumber;

					return true;
				}
				else
					return false;
			}
		}
		// 만약 새로운 주소에서 받은 데이터일 경우 새로 벡터에 추가한다.
		READ_PACKET_INFO	ReadPacketInfo;
		ReadPacketInfo.PacketNumber = PacketNumber;
		ReadPacketInfo.RemotePort = remotePort;
		strcpy(ReadPacketInfo.RemoteAddress, remoteAddress);

		mLastReadPacketInfoVectorForUDP.push_back(ReadPacketInfo);
		
		return true;
	}

	return false;
}
