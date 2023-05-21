#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

string cipherBlockEncrypt(string infile, string keyfile, string outfile);
string cipherBlockDecrypt(string infile, string keyfile, string outfile);
string cipherStreamEncrypt(string infile, string keyfile, string outfile);
string cipherStreamDecrypt(string infile, string keyfile, string outfile);

int main(int argc, char* argv[]) {

	if (argc < 6) {
		cerr << "Error: Must enter 5 arguments: B or S for Block or Stream, input file name, output filename, key file name, E or D for mode";
		exit(-1);
	}

	string arg1 = argv[1];
	string arg5 = argv[5];
	

	if (arg1 == "B" || arg1 != "S") {
		cout << "using block cipher function \n";
	}
	else if (arg1 == "S") {
		cout << "using stream cipher function \n";
	}
	else{
		cerr << "Error: Invalid first argument: " << arg1 << " \nFirst argument can only be \"B\" or \"S\" \n";
		exit(-1);
	}

	if (argv[2]){
		ifstream plainText;
		plainText.open(argv[2]);
		if (plainText.fail()) {
			cerr << "Error: Failed opening input filename " << argv[2] << "\n";
			exit(-1);
		}
	}

	if (argv[4]) {
		ifstream keyfile;
		keyfile.open(argv[2]);
		if (keyfile.fail()) {
			cerr << "Error: Failed opening key filename " << argv[4] << "\n";
			exit(-1);
		}
	}
	if (arg5 == "E") {
		cout << "using encryption mode \n";
	}
	else if (arg5 == "D") {
		cout << "using decryption mode \n";
	}
	else {
		cerr << "Error: Invalid fifth argument: " << arg5 << " \nFirst argument can only be \"E\" or \"D\" for mode \n";
		exit(-1);
	}

	if (argc < 6) {
		cerr << "Error: Must enter 5 arguments: B or S for Block or Stream, input file name, output filename, key file name, E or D for mode";
		exit(-1);
	}

	if (strcmp(argv[1], "B") == 0 && strcmp(argv[5], "E") == 0) {
		cipherBlockEncrypt(argv[2], argv[4], argv[3]);
	}

	if (strcmp(argv[1], "B") == 0 && strcmp(argv[5], "D") == 0) {
		cipherBlockDecrypt(argv[2], argv[4], argv[3]);
	}

	if (strcmp(argv[1], "S") == 0 && strcmp(argv[5], "E") == 0) {
		cipherStreamEncrypt(argv[2], argv[4], argv[3]);
	}
	if (strcmp(argv[1], "S") == 0 && strcmp(argv[5], "D") == 0) {
		cipherStreamDecrypt(argv[2], argv[4], argv[3]);
	}
	return 0;
}


string cipherBlockEncrypt(string inFile, string key, string outFile) {

	cout << "Running block cipher with text: " << inFile << " and key: " << key << '\n';
	stringstream inputFilePath;
	stringstream outputFilePath;
	stringstream keyFilePath;
	inputFilePath << "./" << inFile;
	keyFilePath << "./" << key;
	outputFilePath << "./" << outFile;
	ifstream plainText;
	ifstream keyfile;
	plainText.open(inputFilePath.str());
	keyfile.open(keyFilePath.str());

	if (plainText.fail()) {
		cout << "Input file opening failed.\n";
		exit(1);
	}
	if (keyfile.fail()) {
		cout << "Key file opening failed.\n";
		exit(1);
	}


	char c;
	stringstream PTtoHex;
	stringstream keytoHex;
	int PTCharCount = 0;

	while (plainText.get(c)) {
		bool end = false;
		while ((int)c < 16) {
			PTtoHex << 0;
			PTtoHex << hex << (int)c;
			plainText.get(c);
			if (plainText.eof()) {
				end = true;
				break;
			}
		}
		if (end == true) {
			break;
		}
		PTCharCount += 1;
		PTtoHex << hex << (int)c;
	}

	//padding input file to make sure it is a multiple of 128 bits.
	if (PTCharCount < 16) {
		for (PTCharCount; PTCharCount < 16;PTCharCount++) {
			PTtoHex << hex << 0x81;
		}
	}
	while (PTCharCount % 16 != 0) {
		PTtoHex << hex << 0x81;
		PTCharCount++;
	}

	int KeyCharCount = 0;
	while (keyfile.get(c)) {
		keytoHex << hex << (int)c;
		KeyCharCount++;
		if (KeyCharCount > 16) {
			cerr << "Error: key file must be 128 bit or only 16 bytes in length";
				exit(-1);
		}

	}

	string hexPT;
	string hexKey;

	hexPT = PTtoHex.str();
	hexKey = keytoHex.str();
	cout << "hexPT is " << hexPT << '\n';
	cout << "hexKey is " << hexKey << '\n';
	plainText.close();
	keyfile.close();


	stringstream PTXorKeyToHex;
	stringstream XorStrStream;
	char pth; 
	char kh;
	int PTXorKey;
	int h = 0;
	for (size_t i = 0; i < hexPT.length(); i++) {
		pth = hexPT.at(i);
		kh = hexKey.at(h);
		PTXorKey = stoi((string(1, pth)), nullptr, 16) ^ stoi((string(1, kh)), nullptr, 16);
		XorStrStream << hex << PTXorKey;
		cout << hexPT.at(i) << " XOR " << hexKey.at(h) << " = " << hex <<  PTXorKey << '\n';
		h++;
		if (h == 32) { //always a 16 bit key is used. 
			h = 0;
		}
	}

	cout << "XOR string is: " << XorStrStream.str() << '\n';

	string XorString;
	XorString = XorStrStream.str();

	int s = 0; //start pointer increment
	int e = XorString.length() - 1;; //end pointer decrement
	/*now we mox2 between the key. 
	0 do nothing and go to next char in xor string. 
	1 swap start with end and increment start and decrement
	* end and repeat.  
	*/
	int modXorKey;
	h = 0;
	bool collision = false;
	cout << "before swapping: \n" << XorString << '\n';
	for (size_t i = 0; i < XorString.length(); i++) {
		char xth = XorString.at(i);
		kh = hexKey.at(h);
		modXorKey = stoi((string(1, kh)), nullptr, 16) % 2;
		cout << hexKey.at(h) << " Mod2  or % 2 = " << modXorKey << '\n';
		h++;
		if (h == 32) {
			h = 0;
		}

		if (collision == true) {
			break;
		}
		//here comes the swapping
		if (modXorKey == 1) {
			int temp;
			temp = XorString.at(s + 1);
			XorString.at(s + 1) = XorString.at(e);
			XorString.at(e) = temp;
			temp = XorString.at(s);
			XorString.at(s) = XorString.at(e - 1);
			XorString.at(e - 1) = temp;
			for (int incr = 1, i = 0; i <= incr; i++) {
				s++;
				if (s == e) {
					collision = true;
					break;
				}
				e--;
				if (s == e) {
					collision = true;
					break;
				}
			}
		}
		else if (modXorKey == 0) {
			for (int incr = 1, i = 0; i <= incr; i++) {
				s++;
				if (s == e) {
					collision = true;
					break;
				}
			}
		}
	}
	
	cout << "after swapping Xor String is now: \n" << XorString;
	ofstream encryptedfile(outputFilePath.str());
	string ascii = "";
	for (size_t i = 0; i < XorString.length(); i += 2) {
		string part = XorString.substr(i, 2);

		char ch = stoul(part, nullptr, 16);
		

		ascii += ch;
	}
	encryptedfile << ascii;
	encryptedfile.close(); 
	return XorString;
}

string cipherBlockDecrypt(string inFile, string key, string outFile) {
	
	cout << "Running block cipher with text: " << inFile << " and key: " << key << " outputting to: " << outFile << '\n';
	
	stringstream inputFilePath;
	stringstream keyFilePath;
	stringstream outputFilePath;
	inputFilePath << "./" << inFile;
	keyFilePath << "./" << key;
	outputFilePath << "./" << outFile;
	ifstream plainText;
	ifstream keyfile;
	plainText.open(inputFilePath.str());
	keyfile.open(keyFilePath.str());

	if (plainText.fail()) {
		cout << "Input file open failed.\n";
		exit(1);
	}
	if (keyfile.fail()) {
		cout << "Key file open failed.\n";
		exit(1);
	}

	unsigned char ptc = plainText.get();
	char hc;
	stringstream PTtoHex;
	stringstream keytoHex;



	while (plainText.good()) {
		if ((int)ptc < 16) {
			PTtoHex << 0;
		}
		PTtoHex << hex << (int)ptc;
		ptc = plainText.get();
		if (plainText.bad()) {
			cout << "Ran into coruption while reading input file: " << inputFilePath.str() << '\n';
			exit(1);
		}
	}

	while (keyfile.get(hc)) {
		keytoHex << hex << (int)hc;
	}

	string hexPT;
	string hexKey;
	hexPT = PTtoHex.str();
	hexKey = keytoHex.str();
	cout << "hexPT is " << hexPT << '\n';
	cout << "hexKey is " << hexKey << '\n';
	plainText.close();
	keyfile.close();

	int s = 0; //start pointer increment
	int e = hexPT.length() - 1; //end pointer decrement
	
	/*now we mox2 between the key.
	0 do nothing and go to next char in xor string.
	1 swap start with end and increment start and decrement
	* end and repeat.
	*/
	int modXorKey;
	int h = 0;
	char kh;
	bool collision = false;
	cout << "before swapping: \n" << hexPT << '\n';
	for (size_t i = 0; i < hexPT.length(); i++) {
		char xth = hexPT.at(i);
		kh = hexKey.at(h);
		modXorKey = stoi((string(1, kh)), nullptr, 16) % 2;
		cout << hexKey.at(h) << " Mod2  or % 2 = " << modXorKey << '\n';
		h++;
		if (h == 32) {
			h = 0;
		}

		if (collision == true) {
			break;
		}
		//here comes the swapping
		if (modXorKey == 1) {
			int temp;
			temp = hexPT.at(s + 1);
			hexPT.at(s + 1) = hexPT.at(e);
			hexPT.at(e) = temp;
			temp = hexPT.at(s);
			hexPT.at(s) = hexPT.at(e - 1);
			hexPT.at(e - 1) = temp;
			for (int incr = 1, i = 0; i <= incr; i++) {
				s++;
				if (s == e) {
					collision = true;
					break;
				}
				e--;
				if (s == e) {
					collision = true;
					break;
				}
			}
		}
		else if (modXorKey == 0) {
			for (int incr = 1, i = 0; i <= incr; i++) {
				s++;
				if (s == e) {
					collision = true;
					break;
				}
			}
		}
	}

	cout << "after swapping hexPT String is now: \n" << hexPT << '\n';


	//now we Xor hexPT with key:
	stringstream XorStrStream;
	string XorString;
	char pth;
	char keyh;
	int PTXorKey;
	h = 0;
	for (size_t i = 0; i < hexPT.length(); i++) {
		pth = hexPT.at(i);
		keyh = hexKey.at(h);
		PTXorKey = stoi((string(1, pth)), nullptr, 16) ^ stoi((string(1, keyh)), nullptr, 16);
		XorStrStream << hex << PTXorKey;
		cout << hexPT.at(i) << " XOR " << hexKey.at(h) << " = " << hex << PTXorKey << '\n';
		h++;
		if (h == 32) { //always a 16 bit key is used. 
			h = 0;
		}
	}
	XorString = XorStrStream.str();
	cout << "Xorstring is : " << XorString;


	//now we remove padding
	int end = XorString.length() - 1;
	int cur1 = XorString.at(end);
	int cur2 = XorString.at(end - 1);
	while (cur1 == 49 && cur2 == 56) {
		XorString.erase(end - 1, end);
		end = end - 2;
		cur1 = XorString.at(end);
		cur2 = XorString.at(end - 1);
	}




	ofstream encryptedfile(outputFilePath.str());
	string ascii = "";
	for (size_t i = 0; i < XorString.length(); i += 2) {
		string part = XorString.substr(i, 2);

		char ch = stoul(part, nullptr, 16);
		ascii += ch;
	}
	encryptedfile << ascii;
	encryptedfile.close();

	return "bannanna";
}


string cipherStreamEncrypt(string inFile, string key, string outFile) {

	cout << "Running stream cipher encrypt with text: " << inFile << " and key: " << key << '\n';
	stringstream inputFilePath;
	stringstream outputFilePath;
	stringstream keyFilePath;
	inputFilePath << "./" << inFile;
	keyFilePath << "./" << key;
	outputFilePath << "./" << outFile;
	ifstream plainText;
	ifstream keyfile;
	plainText.open(inputFilePath.str());
	keyfile.open(keyFilePath.str());

	if (plainText.fail()) {
		cout << "Input file opening failed.\n";
		exit(1);
	}
	if (keyfile.fail()) {
		cout << "Key file opening failed.\n";
		exit(1);
	}
	char c;
	string hexKey;
	stringstream PTtoHex;
	stringstream keytoHex;
	stringstream XorStrStream;
	int PTCharCount = 0;

	while (keyfile.get(c)) {
		keytoHex << hex << (int)c;
	}

	hexKey = keytoHex.str();
	int h = 0;
	while (plainText.get(c)) {
		bool end = false;
		while ((int)c < 16) {
			PTtoHex << hex << 0;
			PTtoHex << hex << (int)c;
			plainText.get(c);
			if (plainText.eof()) {
				end = true;
				break;
			}
		}
		if (end == true) {
			break;
		}
		PTCharCount += 1;
		PTtoHex << hex << (int)c;

		char pth;
		char kh;
		int PTXorKey;


		for (size_t i = 0; i < PTtoHex.str().length(); i++){
			pth = PTtoHex.str().at(i);
			kh = hexKey.at(h);
			PTXorKey = stoi(string(1, pth), nullptr, 16) ^ stoi((string(1, kh)), nullptr, 16);
			XorStrStream << hex << PTXorKey;
			cout << hex <<  pth << " XOR " << hexKey.at(h) << " = " << hex << PTXorKey << '\n';
			h++;
			if (h == 32) { //always a 16 bit key is used. 
				h = 0;
			}
		}
		PTtoHex.str("");

	}

	string XorString = XorStrStream.str();

	ofstream encryptedfile(outputFilePath.str());
	string ascii = "";
	for (size_t i = 0; i < XorString.length(); i += 2) {
		string part = XorString.substr(i, 2);

		char ch = stoul(part, nullptr, 16);


		ascii += ch;
	}
	encryptedfile << ascii;
	encryptedfile.close();
	return XorString;
}

string cipherStreamDecrypt(string inFile, string key, string outFile) {
	cout << "Running stream cipher encrypt with text: " << inFile << " and key: " << key << '\n';
	stringstream inputFilePath;
	stringstream outputFilePath;
	stringstream keyFilePath;
	inputFilePath << "./" << inFile;
	keyFilePath << "./" << key;
	outputFilePath << "./" << outFile;
	ifstream plainText;
	ifstream keyfile;
	plainText.open(inputFilePath.str());
	keyfile.open(keyFilePath.str());

	if (plainText.fail()) {
		cout << "Input file opening failed.\n";
		exit(1);
	}
	if (keyfile.fail()) {
		cout << "Key file opening failed.\n";
		exit(1);
	}
	char c;
	string hexKey;
	stringstream PTtoHex;
	stringstream keytoHex;
	stringstream XorStrStream;
	int PTCharCount = 0;

	while (keyfile.get(c)) {
		keytoHex << hex << (int)c;
	}

	hexKey = keytoHex.str();
	int h = 0;
	while (plainText.get(c)) {
		bool end = false;
		while ((int)c < 16) {
			PTtoHex << hex << 0;
			PTtoHex << hex << (int)c;
			plainText.get(c);
			if (plainText.eof()) {
				end = true;
				break;
			}
		}
		if (end == true) {
			break;
		}
		PTCharCount += 1;
		PTtoHex << hex << (int)c;

		char pth;
		char kh;
		int PTXorKey;


		for (size_t i = 0; i < PTtoHex.str().length(); i++) {
			pth = PTtoHex.str().at(i);
			kh = hexKey.at(h);
			PTXorKey = stoi(string(1, pth), nullptr, 16) ^ stoi((string(1, kh)), nullptr, 16);
			XorStrStream << hex << PTXorKey;
			cout << hex << pth << " XOR " << hexKey.at(h) << " = " << hex << PTXorKey << '\n';
			h++;
			if (h == 32) { //always a 16 bit key is used. 
				h = 0;
			}
		}
		PTtoHex.str("");

	}

	string XorString = XorStrStream.str();

	ofstream encryptedfile(outputFilePath.str());
	string ascii = "";
	for (size_t i = 0; i < XorString.length(); i += 2) {
		string part = XorString.substr(i, 2);

		char ch = stoul(part, nullptr, 16);


		ascii += ch;
	}
	encryptedfile << ascii;
	encryptedfile.close();
	return XorString;

}
