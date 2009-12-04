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

#include "WProgram.h"
#include "Kalman.h"

// Important: Vars declared static are shared among instances with different data
// this is ok as they are initialized on each iteration to the result of a
// calculation which uses only member variables.

Kalman::Kalman(float dt, float R_angle, float Q_gyro, float Q_angle) {
	_bias = 0.0;
	_rate = 0.0;
	_angle = 0.0;

	_dt = dt;

	_R_angle = R_angle;

	_Q_gyro = Q_gyro;
	_Q_angle = Q_angle;

	_P_00 = 1.0;
	_P_01 = 0.0;
	_P_10 = 0.0;
	_P_11 = 1.0;
}

void Kalman::set_dt(float dt){
	_dt = dt;
}

void Kalman::state_update(float gyro_rate)
{
	static float gyro_rate_unbiased;
	static float Pdot_00, Pdot_01, Pdot_10, Pdot_11;

	gyro_rate_unbiased = gyro_rate - _bias;

	Pdot_00 = _Q_angle - _P_01 - _P_10;
	Pdot_01 = -_P_11;
	Pdot_10 = -_P_11;
	Pdot_11 = _Q_gyro;

	_rate = gyro_rate_unbiased;

	_angle += gyro_rate_unbiased * _dt;

	_P_00 += Pdot_00 * _dt;
	_P_01 += Pdot_01 * _dt;
	_P_10 += Pdot_10 * _dt;
	_P_11 += Pdot_11 * _dt;
}

void Kalman::kalman_update(float angle_measured)
{
	static float angle_error;
	static float C_0;
	static float PCt_0, PCt_1;
	static float E;
	static float K_0, K_1;
	static float t_0, t_1;

	angle_error = angle_measured - _angle;

	C_0 = 1.0;
	PCt_0 = C_0 * _P_00;
	PCt_1 = C_0 * _P_10;

	E = _R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * _P_01;

	_P_00 -= K_0 * t_0;
	_P_01 -= K_0 * t_1;
	_P_10 -= K_1 * t_0;
	_P_11 -= K_1 * t_1;

	_bias  += K_1 * angle_error;
	_angle += K_0 * angle_error;
}
