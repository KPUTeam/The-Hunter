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
	// NetworkSession���� ������ �����͸� ��Ŷ �м� ���� ��Ƶ� ���۸� �ʱ�ȭ
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
	// UDP�� TCP�� �޸� �ϳ��� ���ӿ� �� ��ü�� �������� �������� �۰�
	// �� ��ü�� ���� ������ �����ϹǷ� ���� �� ��Ŷ ��ȣ�� �����ؾ��Ѵ�.
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
	// NetworkSession Ŭ������ ���ۿ��� 
	// PacketSession Ŭ������ �����͸� �����ϴ� �Լ�
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
	// ���� �����Ϳ� ���� ������ �־� ��Ŷ���� ����� ������ ����� �ϴ� �Լ�
	// ����, ��������, ��Ŷ ��ȣ ���� �ٿ��� �����͸� ������ �ȴ�.
	// ������ ���� ���� ť�� �����͸� �ְ� ������ �����Ⱑ �Ϸ�Ǿ�����
	// WriteComplete �Լ��� �����͸� �����ϱ� ���ؼ��̴�.
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

	// ��Ŷ�� �ε����� �ٿ� ���� ������ �� �־�� �Ѵ�.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	memcpy(TempBuffer + sizeof(DWORD), &mCurrentPacketNumber, sizeof(DWORD));

	memcpy(TempBuffer + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD), packet, packetLength);

	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue�� �̿��ؼ� ��Ŷ�� ���� �Ϸᰡ �Ǿ������� �޸𸮸� ����д�.
	// �����͸� ������ �� �޸𸮰� �����Ǹ� ������ �Ǳ� �����̴�.
	// TempBuffer�� ���������� �Լ��� ����Ǵ� ���� �����Ͱ� �����
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

	// ��Ŷ�� �ε����� �ٿ� ���� ������ ���־�� �Ѵ�.
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
	// ������ �Ϸᰡ �Ǿ��� �� ť���� �����͸� �����ϴ� �Լ�
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
	// ���� �����͸� Ȯ���ؼ� �������� ��Ŷ�� ��� ��Ŷ�� �м��ϴ� ����
	// NetworkSession Ŭ������ ���ؼ� ���� �����͸� PacketSession Ŭ������ ī���ϰ�
	// �� ������ �ּ� 4����Ʈ �̻��� ���, �� ��Ŷ�� ����� �� �� ���� ��� 
	// �켱 ���� 4����Ʈ�� �о� ��Ŷ�� ���̸� �˰� ���� �����Ͱ� ��Ŷ ���̸�ŭ ���� ���
	// �������� ��ȣ, ��Ŷ ��ȣ, ��ȣȭ ���� �۾��� �ϰ� �ϳ��� �ϼ��� ��Ŷ�� ���
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

			// ���� �ּ�, ���� ��Ʈ�� ������ �̹� ���Ϳ� �ִ��� �˻��Ѵ�.
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
		// ���� ���ο� �ּҿ��� ���� �������� ��� ���� ���Ϳ� �߰��Ѵ�.
		READ_PACKET_INFO	ReadPacketInfo;
		ReadPacketInfo.PacketNumber = PacketNumber;
		ReadPacketInfo.RemotePort = remotePort;
		strcpy(ReadPacketInfo.RemoteAddress, remoteAddress);

		mLastReadPacketInfoVectorForUDP.push_back(ReadPacketInfo);
		
		return true;
	}

	return false;
}
