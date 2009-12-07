/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  BMPImage.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/17.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <cstdlib>

#ifndef _TWS_TEXTURE
#define _TWS_TEXTURE

namespace TWS {
	class BMPImage
	{
		struct image_t {
			unsigned long sizeX,sizeY;
			char *data;
		};

	public:
		BMPImage();
		~BMPImage();
		int loadFile(const char *filename);
		image_t m_image;

	private:
		void reverseBytes(unsigned char *source, size_t len);
		int calcEndianess();
	};
}

#endif
