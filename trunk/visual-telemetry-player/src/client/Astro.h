/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Astro.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/12.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <cstdlib>
 
namespace TWS_Astro
{
	#define JulianCentury 36525.0
	#define J2000 2451545.0
	#define fixangle(a) ((a) - 360.0 * (floor((a) / 360.0)))
	#define rtd(x) ((x) / (PI / 180.0))
	#define dtr(x) ((x) * (PI / 180.0))

	void getsuncoords(double *coords);
	void sunpos(double jd, int apparent, double *ra, double *dec, double *rv, double *slong);
	double juliandate();
	double ucttoj(long year, int mon, int mday, int hour, int min, int sec);
	double gmst(double jd);
}
