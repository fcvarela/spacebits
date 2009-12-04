/*
 * (c) 2005-9 Filipe Varela
 *
 * Kalman is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * IMU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Kalman; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef Kalman_h
#define Kalman_h

#include "WProgram.h"

class Kalman
{
	public:
		Kalman(float dt, float R_angle, float Q_gyro, float Q_angle);
		void set_dt(float dt);
		void state_update(float gyro_rate);
		void kalman_update(float angle_measured);
		float _bias;
		float _rate;
		float _angle;

	private:
		/* Covariance of estimation error matrix. */
		float _P_00;
		float _P_01;
		float _P_10;
		float _P_11;

		/* State constants. */
		float _dt;
		float _R_angle;
		float _Q_gyro;
		float _Q_angle;
};

#endif
