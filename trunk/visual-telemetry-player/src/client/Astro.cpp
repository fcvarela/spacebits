/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Astro.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/12.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <math.h>
#include <time.h>
#include <assert.h>

#include "Astro.h"

#define PI 3.14159265358979323846
#define Asec(x) ((x) / 3600.0)
#define EPSILON 1E-6

namespace TWS_Astro
{
	double add = 0;
	static double obliqeq(double jd);

	static double kepler(double m, double ecc) {
		double e, delta;

		e = m = dtr(m);
		do {
			delta = e - ecc * sin(e) - m;
			e -= delta / (1 - ecc * cos(e));
		} while (abs(delta) > EPSILON);
		return e;
	}

	/*  SUNPOS  --
		Calculate position of the Sun. JD is the Julian  date
		of  the  instant for which the position is desired and
		APPARENT should be nonzero if  the  apparent  position
		(corrected  for  nutation  and aberration) is desired.
		The Sun's co-ordinates are returned  in  RA  and  DEC,
		both  specified  in degrees (divide RA by 15 to obtain
		hours).  The radius vector to the Sun in  astronomical
		units  is returned in RV and the Sun's longitude (true
		or apparent, as desired) is  returned  as  degrees  in
		SLONG.	*/

	void sunpos(double jd, int apparent, double *ra, double *dec, double *rv, double *slong) {
		double t, t2, t3, l, m, e, ea, v, theta, omega, eps;

		/*
		Time, in Julian centuries of 36525 ephemeris days,
		measured from the epoch 1900 January 0.5 ET.
		*/

		t = (jd - 2415020.0) / JulianCentury;
		t2 = t * t;
		t3 = t2 * t;

		/*
		Geometric mean longitude of the Sun, referred to the
		mean equinox of the date.
		*/

		l = fixangle(279.69668 + 36000.76892 * t + 0.0003025 * t2);
		// Sun's mean anomaly
		m = fixangle(358.47583 + 35999.04975 * t - 0.000150 * t2 - 0.0000033 * t3);
		// Eccentricity of the Earth's orbit.
		e = 0.01675104 - 0.0000418 * t - 0.000000126 * t2;
		
		//Eccentric anomaly
		ea = kepler(m, e);

	 	//True anomaly */
		v = fixangle(2 * rtd(atan(sqrt((1 + e) / (1 - e))  * tan(ea / 2))));

		// Sun's true longitude
		theta = l + v - m;

		// Obliquity of the ecliptic. */
		eps = obliqeq(jd);

		// Corrections for Sun's apparent longitude, if desired.
		if (apparent) {
			omega = fixangle(259.18 - 1934.142 * t);
			theta = theta - 0.00569 - 0.00479 * sin(dtr(omega));
			eps += 0.00256 * cos(dtr(omega));
		}

		// Return Sun's longitude and radius vector
		*slong = theta;
		*rv = (1.0000002 * (1 - e * e)) / (1 + e * cos(dtr(v)));
		
		// Determine solar co-ordinates
		*ra = fixangle(rtd(atan2(cos(dtr(eps)) * sin(dtr(theta)), cos(dtr(theta)))));
		*dec = rtd(asin(sin(dtr(eps)) * sin(dtr(theta))));
	}

	static double obliqeq(double jd) {
		static double oterms[10] = {
			Asec(-4680.93),
			Asec(-1.55),
			Asec(1999.25),
			Asec(-51.38),
			Asec(-249.67),
			Asec(-39.05),
			Asec(7.12),
			Asec(27.87),
			Asec(5.79),
			Asec(2.45)
		};

		double eps = 23 + (26 / 60.0) + (21.448 / 3600.0), u, v;
		int i;

		v = u = (jd - J2000) / (JulianCentury * 100);
		if (abs(u) < 1.0) {
			for (i = 0; i < 10; i++) {
				eps += oterms[i] * v;
				v *= u;
			}
		}
		return eps;
	}

	double juliandate() {
		time_t now;
		struct tm *ts;
		now = time(NULL);
		ts = localtime(&now);
		double result = ucttoj(ts->tm_year+1900,ts->tm_mon,ts->tm_mday,ts->tm_hour,ts->tm_min,ts->tm_sec);
		add+=0.001;
		return result+add;
	}

	double ucttoj(long year, int mon, int mday, int hour, int min, int sec) {
		/* Algorithm as given in Meeus, Astronomical Algorithms, Chapter 7, page 61 */
		int a, b, m;
		long y;

		assert(mon >= 0 && mon < 12);
		assert(mday >  0 && mday < 32);
		assert(hour >= 0 && hour < 24);
		assert(min >= 0 && min < 60);
		assert(sec >= 0 && sec < 60);

		m = mon + 1;
		y = year;

		if (m <= 2) {
			y--;
			m += 12;
		}

		/*
		Determine whether date is in Julian or Gregorian calendar based on
		canonical date of calendar reform.
		*/

		if ((year < 1582) || ((year == 1582) && ((mon < 9) || (mon == 9 && mday < 5)))) {
			b = 0;
		} else {
			a = ((int) (y / 100));
			b = 2 - a + (a / 4);
		}

		return (((long) (365.25 * (y + 4716))) + ((int) (30.6001 * (m + 1))) +
			mday + b - 1524.5) +
			((sec + 60L * (min + 60L * hour)) / 86400.0);
	}

	void getsuncoords(double *coords) {
		double jt = juliandate();
		double sunra, sundec, sunrv, sunlong, gt, subslong;

		sunpos(jt, 0, &sunra, &sundec, &sunrv, &sunlong);
		gt = gmst(jt);

		subslong = (gt * 15) - sunra;
		if (subslong > 180) subslong = -(360 - subslong);
		else if (subslong < -180) subslong += 360;

		coords[0] = sundec;
		coords[1] = subslong;
	}

	double gmst(double jd) {
		double t, theta0;

		/*
		Time, in Julian centuries of 36525 ephemeris days,
		measured from the epoch 1900 January 0.5 ET
		*/

		t = ((floor(jd + 0.5) - 0.5) - 2415020.0) / JulianCentury;
		theta0 = 6.6460656 + 2400.051262 * t + 0.00002581 * t * t;
		t = (jd + 0.5) - (floor(jd + 0.5));
		theta0 += (t * 24.0) * 1.002737908;
		theta0 = (theta0 - 24.0 * (floor(theta0 / 24.0)));
		return theta0;
	}
}
