#include "Verify.h"
#include <stdint.h>
#include <string.h>
#include <chrono>
#include "KeyHandler.hpp"


char Rc4Key[] = "!@##$asdcgfxxxop";

bool VerifyKey(char* Key)
{
	uint64_t ExpiredTime = KeyHandler->QueryKey(Key);

	uint64_t now = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	if (ExpiredTime > 0 && ExpiredTime < 400)	//天卡月卡年卡
	{
		KeyHandler->ModifyKey({ Key ,now + ExpiredTime * 24 * 3600 });
		ExpiredTime = KeyHandler->QueryKey(Key);
	}

	if (ExpiredTime > now)
	{
		return true;
	}
	return false;
}

bool VerifyData(unsigned char* Data, int Len)
{
	char* Key;
	int MagicNumber = 0;

	Rc4Crypt(Data, Len, (unsigned char*)Rc4Key, strlen(Rc4Key));

	Key = (char*)&Data[0];
	MagicNumber = *(int*)&Data[Len - 5];

	if (VerifyKey(Key) == false)
		return false;

	memset(Data, 0, Len);
	*(uint64_t*)&Data[0] = 0xDFFDABCD03007677;
	MagicNumber += 0x55;
	*(int*)&Data[Len - 5] = MagicNumber;

	Rc4Crypt(Data, Len, (unsigned char*)Rc4Key, strlen(Rc4Key));
	return true;
}

void Rc4Init(unsigned char* s, unsigned char* key, unsigned long len_key)
{
	int i = 0, j = 0;
	unsigned char k[256] = { 0 };
	unsigned char tmp = 0;
	for (i = 0; i < 256; i++) {
		s[i] = i;
		k[i] = key[i % len_key];
	}
	for (i = 0; i < 256; i++) {
		j = (j + s[i] + k[i]) % 256;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}
}

void Rc4Crypt(unsigned char* data, unsigned long len_data, unsigned char* key, unsigned long len_key)
{
	unsigned char s[256];
	int i = 0, j = 0, t = 0;
	unsigned long k = 0;
	unsigned char tmp;
	Rc4Init(s, key, len_key);
	for (k = 0; k < len_data; k++) {
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
		t = (s[i] + s[j]) % 256;
		data[k] = data[k] ^ s[t];
	}
}
