#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

/*
 * The cipherBlockEncrypt(string, string, string) function will take 3 string arguments and perform a Block Cipher Encryption on data.
 * First argument is input text file to be encrypted.
 * Second argument is key file used to compare and do mod comparisons to generate output encrypted file.
 * Third argument is name of generated output file that will be created.
 * */
string cipherBlockEncrypt(string infile, string keyfile, string outfile);

/*
 * The cipherBlockDecrypt(string, string, string) function will take 3 string arguments and perform a Block Cipher Decryption on data.
 * First argument is input text file to be Decrypted. This will be the output file from the cipherBlockEncrypt().
 * Second argument is key file used to reverse the encryption algorithm that was done from cipherBlockEncrypt()
 * Third argument is name of generated output file that will be created. Output should be original text that was input
 * for cipherBlockEncrypt()
 * */
string cipherBlockDecrypt(string infile, string keyfile, string outfile);


/*
 * The cipherStreamEncrypt(string, string, string) function will take 3 string arguments and perform a Block Stream Encryption on data.
 * First argument is input text file to be encrypted.
 * Second argument is key file used to compare and do mod comparisons to generate output encrypted file.
 * Third argument is name of generated output file that will be created.
 * */
string cipherStreamEncrypt(string infile, string keyfile, string outfile);

/*
 * The cipherStreamDecrypt(string, string, string) function will take 3 string arguments and perform a Block Stream Decryption on data.
 * First argument is input text file to be Decrypted. This will be the output file from the cipherStreamEncrypt().
 * Second argument is key file used to reverse the encryption algorithm that was done from cipherStreamEncrypt()
 * Third argument is name of generated output file that will be created. Output should be original text that was input
 * for cipherStreamEncrypt()
 * */
string cipherStreamDecrypt(string infile, string keyfile, string outfile);


/*
 * Main function will take arguments to determine which of encryption / decryption algorithms will be run for specified files.
 * argv[0] = name of program being run.
 * argv[1] = either B or S characters to specify Block or Stream algorithms respectively.
 * argv[2] = the text file name used for input. should be in same directory where program is run.
 * argv[3] = the text file name used for output. This file will be created and will not exist upon running program.
 * argv[4] = the key txt file that will be used with input file to run encryption or decryption
 *
 * */
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

    //attempt  opening intput text
	if (argv[2]){
		ifstream plainText;
		plainText.open(argv[2]);
		if (plainText.fail()) {
			cerr << "Error: Failed opening input filename " << argv[2] << "\n";
			exit(-1);
		}
	}

    // attempt opening key text file
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

    // list of if statements to determine which function to run based on argv[] received data.
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


/**
 *  cipherBlockEncrypt() definition:
 *  receives the input, key, and output text files and performs encryption on file Cipher Block style.
 *  If input file bytes does not mod2 = 0, then input file is padded with 0x81 until size is divisible by 128 bits.
 */
string cipherBlockEncrypt(string inFile, string key, string outFile) {

	cout << "Running block cipher with text: " << inFile << " with key: " << key << '\n';
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

    // collect characters from input text file into hex values in a separate stringStream variable, PTtoHex.
	while (plainText.get(c)) {
		bool end = false;
		while ((int)c < 16) { // for consistency of having a two digit hex value per character.
			PTtoHex << 0; // put a 0 in front of hex value that will come into string stream. first 4 bits.
			PTtoHex << hex << (int)c; //this char getting stored is the second set of 4 bits for the char value.
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

	//Block part of ciphering. Padding input file to make sure it is a multiple of 128 bits.
	if (PTCharCount < 16) {
		for (PTCharCount; PTCharCount < 16;PTCharCount++) {
			PTtoHex << hex << 0x81;
		}
	}
	while (PTCharCount % 16 != 0) {
		PTtoHex << hex << 0x81;
		PTCharCount++;
	}

    // collect characters from key file into hex values in a separate stringStream variable, keytoHex.
	int KeyCharCount = 0;
	while (keyfile.get(c)) {
		keytoHex << hex << (int)c;
		if (c == '\n'){ //don't count return character.
            continue;
        }
        KeyCharCount++;
		if (KeyCharCount > 16) { //key file can only have 16 characters.
			cerr << "Error: key file must be 128 bit or only 16 bytes in length\n";
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


    /*
     * at this point, we have the text of our plainText converted to hex in the string var, hexPT,
     * We also have the text of our key file converted to hex in the string var, hexKey.
     *
     * Below is the logic used to do a XOR on each hex value between the hexPT and hexKey strings.
     */

	stringstream PTXorKeyToHex;
	stringstream XorStrStream; //will hold collected XOR calculations.
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
		if (h == 32) { //We start over going through the key values as we continue through the XOR operations with input Text data.
			h = 0;
		}
	}

	cout << "XOR string is: " << XorStrStream.str() << '\n';

	string XorString;
	XorString = XorStrStream.str();  //This string has the resulting hex values of input text XOR key text files. We will swap bytes in this string.

	int s = 0; //start index increment
	int e = XorString.length() - 1;; //end index decrement
	/*now we mod2 or %2 between the key.
	if result, or modXorKey, is 0 do nothing and go to next char indexes in XorString.
	if result is 1, swap the pair of start and end values (4 values) and increment s and decrement e variables.
	* end and repeat.  
	*/
	int modXorKey;
	h = 0;
	bool collision = false;
	cout << "before swapping: \n" << XorString << '\n';
	for (size_t i = 0; i < XorString.length(); i++) {
		kh = hexKey.at(h);
		modXorKey = stoi((string(1, kh)), nullptr, 16) % 2;
		cout << hexKey.at(h) << " Mod2  or % 2 = " << modXorKey << '\n'; //debugging bread crumb.
		h++;
		if (h == 32) {
			h = 0;
		}

		if (collision == true) {
			break;
		}
		/*
		 * here comes the swapping. we are swapping one byte at a time, that's two rounds of 4 bits in one go.
		 * we track the start and end of XorString and swap the bytes between the front and end of the string
		 * depending on what indexes we are on for s and e int variables.
		 */
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

    /*
     * We now create the output file and start storing the generated encrypted data into the output file.
     * We store 2 characters at a time since the XorString is still in Hex.
     */
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

/**
 *  cipherBlockDecrypt() definition:
 *  receives the following files as input in the following order:  1.input text, 2. key, and 3. output text
 *  Performs Decryption on input file with provided key in Cipher Block style.
 *  File that is used for input text should be the output file of cipherBlockEncrypt().
 *  0x81 padded data on input is removed (Block).
 * */
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


    //take input text file and store as hex values in hexPT
	while (plainText.good()) {
		if ((int)ptc < 16) {
			PTtoHex << 0; //put a 0 in front of hex value to keep it 2 digit hex value.
		}
		PTtoHex << hex << (int)ptc;
		ptc = plainText.get();
		if (plainText.bad()) {
			cout << "Ran into corruption while reading input file: " << inputFilePath.str() << '\n';
			exit(1);
		}
	}

    //take key text file and store as hex values in hexKey
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

	int s = 0; //start index increment
	int e = hexPT.length() - 1; //end index decrement
	
	/*now we mod2 with values in hexKey.
	* if mod2 = 0, do nothing and go to next char in xor string.
	* if mod2 = 1 swap start with end and increment start and decrement. we do this for every byte (two 4 bit segments
    *  of s and e index values of hexPT string.)
	* once s and e collide, we are done.
	*/
	int modXorKey;
	int h = 0;
	char kh;
	bool collision = false;
	cout << "before swapping: \n" << hexPT << '\n';
	for (size_t i = 0; i < hexPT.length(); i++) {
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



    //now we create output file that should be identical to the input file data that was used for cipherBlockEncrypt().
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

        //here we do XOR operation between key and input text and collect results into XorStrStream string stream var.
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
