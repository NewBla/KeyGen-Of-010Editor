#include <iostream>
#include<Windows.h>
#include "NumberTable.h"
using namespace std;

#define LICENSE_TYPE_SINGLE_USER 0
#define LICENSE_TYPE_SITE 1
#define LICENSE_TYPE_MULTI_USER 2

DWORD MagicCalc(PCHAR UserName, BYTE UserCounts)
{
	WORD EBPMinus8 = 0xC22E;
	BYTE EBPMinus5 = 1;
	PCHAR Local1 = UserName;
	DWORD Local3 = 0;

	DWORD Local5 = (UserCounts << 4)- UserCounts;
	DWORD Local6 = EBPMinus8 + (EBPMinus8 << 4);
	DWORD Local8 = 0;

	USHORT Length = strlen(UserName);

	if (Length == 0) return 0;

	DWORD Local4 = 1;//Ñ­»·¼ÆÊý
	DWORD EDX = 0;
	DWORD ECX = 0;
	DWORD EDI = 0;
	DWORD Local7 = 0;
	while (Length != 0)
	{
		BYTE AL = Local1[Local4 - 1];
		if (AL >= 0x61 && AL <= 0x7A)
		{
			AL -= 0x20;
		}
		EDX = 0;
		EDX = Local7 & 0xFF;
		EDX = NumberTables[EDX];

		ECX = 0;
		ECX = Local5 & 0xFF;
		EDX += NumberTables[ECX];

		ECX = 0;
		ECX = Local6 & 0xFF;
		EDX += NumberTables[ECX];

		ECX = 0;
		ECX = AL;
		EDI = NumberTables[ECX+0xD];

		ECX = 0;
		ECX = AL;
		ECX= NumberTables[ECX];
		ECX += Local3;

		EDI ^= ECX;
		EDI = EDI * NumberTables[AL + 0x2F];

		EDX = EDX + EDI;
		Local3 = EDX;


		Local7 += 0x13;
		Local8 += 0x7;
		Local6 += 0x9;
		Local5 += 0xD;
		Local4++;
		Length--;
	}
	return Local3;

}




LPSTR KeyGen(PCHAR UserName, BYTE LicenseType,BYTE UserCounts)
{
	USHORT SI = 0;
	BYTE Key[10] = { 0 };
	BYTE KeyString[40] = { 0 };
	if (UserName == NULL || LicenseType > 2) return 0;

	switch (LicenseType)
	{
	   case LICENSE_TYPE_SINGLE_USER:
		   SI = 1;
		   break;
	   case LICENSE_TYPE_SITE:
		   SI = 0x3E8;
		   break;
	   case LICENSE_TYPE_MULTI_USER:
		   SI = UserCounts;
		   if (SI == 0 || SI >= 1000)
		   {
			   srand(GetTickCount64());
			   SI = (rand() % 1000) + 1;
		   }
		   break;
	}

	DWORD EDX = SI;
	Key[3] = 0xAC;
	DWORD MagicNumber=MagicCalc(UserName, SI);

	Key[4] = MagicNumber & 0xFF;

	Key[5] = (MagicNumber >> 0x8) & 0xFF;

	Key[6] = (MagicNumber >> 0x10) & 0xFF;

	Key[7] = (MagicNumber >> 0x18) & 0xFF;

	DWORD EBX= (((SI * 0xB) ^ 0x3421) - 0x4D30) ^ 0x7892;
	EDX = EBX;
	EDX=EDX >> 8;

	Key[1] = (EDX & 0xFF) ^ Key[7];

	Key[2] = (EBX & 0xFF) ^ Key[5];


	Key[0] = (0xFF95D981 & 0xFF) ^ Key[6];

	Key[8]=((0xFF95D981 >> 8)&0xFF)^Key[4];

	Key[9] = ((0xFF95D981 >> 0x10) & 0xFF) ^ Key[5];

	int j = 0;
	for (int i = 0; i <= 9; i++)
	{
		sprintf_s((CHAR*)& KeyString[j], 40, "%02X", Key[i]);
		j+=2;
	}

	j = 0;
	LPSTR Result = (LPSTR)malloc(40);
	memset(Result, 0, 40);

	memcpy_s(Result, 40, KeyString, 4);
	Result[4] = 0x2D;//ASCII "-"

	memcpy_s(Result+5, 40, KeyString+4, 4);
	Result[9] = 0x2D;

	memcpy_s(Result + 10, 40, KeyString + 8, 4);
	Result[14] = 0x2D;

	memcpy_s(Result + 15, 40, KeyString + 12, 4);
	Result[19] = 0x2D;

	memcpy_s(Result + 20, 40, KeyString + 16, 4);


	return Result;
}
int main()
{
	CHAR UserName[30] = { 0 };
	LPSTR License = NULL;
	BYTE UserType;

	cout << "Please Input Your UserName: ";
	cin >> UserName;
	cout << endl;
	
	cout << "Please Input Your UserType: " << endl;
	cout << "0 For Single User." << endl;
	cout << "1 For Site User." << endl;
	cout << "2 For Multi User." << endl;
	cout << endl;
	cin >> UserType;

	if (UserType-0x30 == LICENSE_TYPE_MULTI_USER)
	{
		DWORD UserCount;
		cout << "Please Input Your User Counts: ";
		cin >> UserCount;
		License = KeyGen(UserName, UserType-0x30, UserCount);

	}

	else
	{
		License = KeyGen(UserName, UserType-0x30, 1);
	}


	printf("Your License Key Is:  %s", License);
	cout << endl;
	system("pause");
	return 0;

}


