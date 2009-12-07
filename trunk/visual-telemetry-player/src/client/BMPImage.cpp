/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  BMPImage.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/17.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>
#include <string.h>

#include "BMPImage.h"
#define _TWS_BIGENDIAN 0
#define _TWS_LITTLEENDIAN 1

	namespace TWS
{
	BMPImage::BMPImage() {
	}

	BMPImage::~BMPImage() {
		free(m_image.data);
		std::cout << "TWS::Texture dying" << std::endl;
	}

	void BMPImage::reverseBytes(unsigned char *source, size_t len)
	{
		char temp[len];

		for (int i=0; i<len; i++)
			temp[i] = source[len-i-1];

		memcpy(source, temp, len);
	}

	int BMPImage::calcEndianess()
	{
		unsigned short test = 1;
		unsigned char *tst;

		tst = (unsigned char*)&test;
		if (*tst == 1)
			return _TWS_LITTLEENDIAN;
		else
			return _TWS_BIGENDIAN;
	}

	int BMPImage::loadFile(const char *filename)
	{
		FILE *file;
		unsigned long size;                 // size of the image in bytes.
		unsigned long i;                    // standard counter.
		unsigned short int planes;          // number of planes in image (must be 1) 
		unsigned short int bpp;             // number of bits per pixel (must be 24)
		char temp;                          // temporary color storage for bgr-rgb conversion.
		char errStr[1000];

		// make sure the file is there.
		if ((file = fopen(filename, "rb"))==NULL) {
			sprintf(errStr, "TWS::Texture::loadImage() - %s", filename);
			perror(errStr);
		}

		int byteOrder = this->calcEndianess();

		// seek through the bmp header, up to the width/height:
		fseek(file, 18, SEEK_CUR);

		// read the width
		if ((i = fread(&m_image.sizeX, 4, 1, file)) != 1)
			return 0;

		if (!byteOrder)
			this->reverseBytes((unsigned char *)&m_image.sizeX, sizeof(m_image.sizeX));

		// read the height 
		if ((i = fread(&m_image.sizeY, 4, 1, file)) != 1)
			return 0;

		if (!byteOrder)
			this->reverseBytes((unsigned char*)&m_image.sizeY, sizeof(m_image.sizeY));

		// calculate the size (assuming 24 bits or 3 bytes per pixel).
		size = m_image.sizeX * m_image.sizeY * 3;

		// read the planes
		if ((fread(&planes, 2, 1, file)) != 1) {
			printf("Error reading planes from %s.\n", filename);
			return 0;
		}

		if (!byteOrder)
			this->reverseBytes((unsigned char *)&planes, sizeof(planes));

		if (planes != 1) {
			printf("Planes from %s is not 1: %u\n", filename, planes);
			return 0;
		}

		// read the bpp
		if ((i = fread(&bpp, 2, 1, file)) != 1) {
			printf("Error reading bpp from %s.\n", filename);
			return 0;
		}
		if (!byteOrder)
			this->reverseBytes((unsigned char*)&bpp, sizeof(bpp));

		// seek past the rest of the bitmap header.
		fseek(file, 24, SEEK_CUR);

		// read the data. 
		m_image.data = (char *) malloc(size);
		if (m_image.data == NULL) {
			printf("Error allocating memory for color-corrected image data: %s", filename);
			return 0;	
		}

		if ((i = fread(m_image.data, size, 1, file)) != 1) {
			printf("Error reading image data from %s.\n", filename);
			return 0;
		}

		for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
			temp = m_image.data[i];
			m_image.data[i] = m_image.data[i+2];
			m_image.data[i+2] = temp;
		}

		// we're done.
		return 1;
	}
}
