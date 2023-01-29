#pragma once


void Rc4Crypt(unsigned char* data, unsigned long len_data, unsigned char* key, unsigned long len_key);

bool VerifyKey(char* Key);

bool VerifyData(unsigned char* Data, int Len);
