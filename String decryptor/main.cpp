

#include <iostream>
#include "cryptography.h"


using namespace std;


int main()
{
	string encCode;
	cout << "String code decryptor:";
	cin >> encCode;

	const auto dec_text = DecodeBase64(encCode);

	string dec = decrypt_text(dec_text);

	cout << dec << endl;
				
				
	return 0;
}
