// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <iostream>

// Integer types
typedef uint_least32_t u32;
typedef uint_least16_t u16;
typedef uint_least8_t   u8;
typedef  int_least8_t   s8;

namespace GamePak
{
	std::vector<u8> ROM, VRAM(0x2000); // ¿que significa (0x2000)?
	unsigned mappernum;
	
	const unsigned ROM_Granularity = 0x2000, ROM_Pages = 0x10000 / ROM_Granularity;     //  area of ROM used to store the program code. Siempre sale 8
	const unsigned VROM_Granularity = 0x0400, VROM_Pages = 0x2000 / VROM_Granularity;   //  area of ROM used to store the graphics information. Siempre sale 8
	
	unsigned char* banks[ROM_Pages] = {};
	unsigned char* Vbanks[VROM_Pages] = {};

	// Aún no se que hace la NRAM/PRAM, de las siguientes 2 lineas pasamos por el momento
	unsigned char NRAM[0x1000], PRAM[0x2000];
	unsigned char *Nta[4] = { NRAM + 0x0000, NRAM + 0x0400, NRAM + 0x0000, NRAM + 0x0400 };

	template<unsigned npages, unsigned char*(&b)[npages], std::vector<u8>& r, unsigned granu>
	static void SetPages(unsigned size, unsigned baseaddr, unsigned index)
	{
		for (unsigned v = r.size() + index * size,
			p = baseaddr / granu;
			p < (baseaddr + size) / granu && p < npages;
			++p, v += granu)
			b[p] = &r[v % r.size()];
	}

	auto& SetROM = SetPages< ROM_Pages, banks, ROM, ROM_Granularity>;
	auto& SetVROM = SetPages<VROM_Pages, Vbanks, VRAM, VROM_Granularity>;

	void Init()
	{
		SetVROM(0x2000, 0x0000, 0);
		for (unsigned v = 0; v < 4; ++v) SetROM(0x4000, v * 0x4000, v == 3 ? -1 : 0);

		std::cout << "ROM PAGES" << ROM_Pages << "VROM Pages" << VROM_Pages << "\n" ; // Siempre devuelve 8 y 8
	}
}

int main()
{
	//Open ROM file
	errno_t err;

	FILE *fp;

	err = fopen_s(&fp, "nes.nes", "rb");

	if (err == 0)
	{
		printf("The file was opened\n");
	}
	else
	{
		printf("The file was not opened\n");
	}
	
	// Read the ROM file header/4 bytes
	assert(fgetc(fp) == 'N' && fgetc(fp) == 'E' && fgetc(fp) == 'S' && fgetc(fp) == '\32'); // Abort if the header is not "NES "
	
	// reading 4 bytes of the header
	u8 rom16count = fgetc(fp);
	u8 vrom8count = fgetc(fp);
	u8 ctrlbyte   = fgetc(fp);
	u8 mappernum  = fgetc(fp) | (ctrlbyte>>4);

	// 8 bytes from the header skipped
	fgetc(fp); fgetc(fp); fgetc(fp); fgetc(fp); fgetc(fp); fgetc(fp); fgetc(fp); fgetc(fp);

	// ???????? Don't know why https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/

	if (mappernum >= 0x40) mappernum &= 15;
	GamePak::mappernum = mappernum;

	// read the ROM data

	if (rom16count) GamePak::ROM.resize(rom16count * 0x4000);   //16 bits
	if (vrom8count) GamePak::VRAM.resize(vrom8count * 0x2000);  //8  bits
	fread(&GamePak::ROM[0], rom16count, 0x4000, fp);
	fread(&GamePak::VRAM[0], vrom8count, 0x2000, fp);

	// Close ROM file
	err = fclose(fp);

	printf("%u * 16kB ROM, %u * 8kB VROM, mapper %u, ctrlbyte %02X\n", rom16count, vrom8count, mappernum, ctrlbyte);

	// Start Emulation
	GamePak::Init();

    std::cout << "Hello World!\n"; 
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
