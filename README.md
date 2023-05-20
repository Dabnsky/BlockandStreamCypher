# BlockandStreamCypher
Practice on very simple encryption and decryption algorithms
Author: David Habinsky

The tar file provided includes:

|-- main.cc [This is the the code to be compiled for the program]
|-- CMakeList.txt [required for compilation and creates this tar file ]
|-- README.txt [This file]

Use case: 
This program is a small encryption example that uses XOR operations. Encryptions can be used with either block or stream techniques with the input file. 
The algorithm used in this encryption is for learning purposes only and will not hold up against any current encryption standards. 

To compile:
cmake . && make

To run:
./Encryption [followed by the 5 arguments.]

Program requires being suppled no less than the 5 following arguments:

1. B or S characters. B for Block Cipher mode. S for Stream Cipher mode.

2. Input text file to be used for encryption or decryption. 

3. output text file to be produced by the program. This is a new file and will not exist prior to running the program.

4. key text file. Key file must be a single line of 16 characters.

5. E or D characters. E for encrypt mode. D for decrypt mode. 

Note: 
	All files used as input are assumed to be in the same directory as ./Encryption
	Don't mix Block or Stream modes. i.e. Don't use a Block Encryption output file as the input for Stream Decrypt.
	Only block encrypt output is used for block decrypt input. Stream encrypt output is used for stream decrypt input. 

Below are the 4 correct ways of supplying these 5 arguemnts in the CLI 

For example:
./Encryption B input.txt output.txt key E [will encrypt input.txt in Block Cipher mode and output Cipher text in file called output.txt]
./Encryption B input.txt output.txt key D [will Decrypt input.txt in Block Cipher mode and output Plain text in file called output.txt.]
./Encryption S input.txt output.txt key E [will Decrypt input.txt in Stream Cipher mode and output Cipher text in file called output.txt]
./Encryption S input.txt output.txt key D [will Decrypt input.txt in Stream Cipher mode and output Plain text in file called output.txt]

Features:
Block Cipher mode: 
	Will take the input text file and apply a block cipher algorithym that involves padding the data with 0x81 bytes until the length of the text is divisible by 128 bits. Encryption will add 0x81 bytes while Decrypt removes them.

Stream Cipher mode: 
	Will take the input text file and run the xor based operations with provided key as input file is read. No padding is done here.

Encryption: 
	Encryption will use the input and key files together, convert them to hex, and use XOR operations between the two. Based on the results of the XOR calculations, different characters / bytes of the input text file will be swapped. 

Decryption: 
	Decryption mode is typically run with the output file produced from running the Encrytpion mode. so Block Encryption output file should be run as input text for Block Decryption. Stream Encrypt output should be used as input for Stream Decrypt. 
	Decryption will use the input and key and will simply run the same algorithm which will swap the bytes back into their original place. Same key that was used for decryption must be used.

