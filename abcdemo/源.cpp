#include <fstream>
#include <iostream>
#include "time.h"

#include "lz4.h"
using namespace std;
int main()
{
	if (1)
	{
		int nInputSize = 1280 * 768 ;
		char *szSource = new char[nInputSize];
		for (int i = 0; i < nInputSize; ++i){
			szSource[i] = rand() % 128;
		}
		printf("data finished~\n");
		//int ii = LZ4_MAX_INPUT_VALUE;
		//char szSource[] = "2013-01-07 00:00:04,0.98644,0.98676 2013-01-07 00:01:19,0.98654,0.98676 2013-01-07 00:01:38,0.98644,0.98696";
		

		// compress szSource into pchCompressed
		char* pchCompressed = new char[nInputSize];
		const int max_dst_size = LZ4_compressBound(nInputSize);
		clock_t clockBegin, clockEnd;
		clockBegin = clock();
		int nCompressedSize = LZ4_compress_default(szSource, pchCompressed, nInputSize, max_dst_size);
		//int nCompressedSize = LZ4_compress(szSource, pchCompressed, nInputSize);
		clockEnd = clock();
		printf("%f\t\n", (clockEnd - clockBegin) / (float)CLOCKS_PER_SEC);
		return 0;
		// write pachCompressed to binary lz4.dat
		ofstream outBinaryFile("lz4.dat", ofstream::binary);
		outBinaryFile.write(pchCompressed, nCompressedSize);
		outBinaryFile.close();
		delete[] pchCompressed;
		pchCompressed = 0;

		//read compressed binary file (assume we pass/encode nInputSize but don't know nCompressedSize)
		ifstream infCompressedBinaryFile("lz4.dat", ifstream::binary);

		//Get compressed file size for buffer
		infCompressedBinaryFile.seekg(0, infCompressedBinaryFile.end);
		int nCompressedInputSize = infCompressedBinaryFile.tellg();
		infCompressedBinaryFile.clear();
		infCompressedBinaryFile.seekg(0, ios::beg);

		//Read file into buffer
		char* pchCompressedInput = new char[nCompressedInputSize];
		infCompressedBinaryFile.read(pchCompressedInput, nCompressedSize);
		infCompressedBinaryFile.close();

		// Decompress buffer
		char* pchDeCompressed = new char[nInputSize]; //(nCompressedInputSize *2) +8
		LZ4_uncompress(pchCompressedInput, pchDeCompressed, nInputSize);
		delete[] pchCompressedInput;
		pchCompressedInput = 0;

		// write decompressed pachUnCompressed to
		ofstream outFile("lz4.txt");
		outFile.write(pchDeCompressed, nInputSize);
		outFile.close();

		delete[] pchDeCompressed;
		pchDeCompressed = 0;
	}
}