//
// Created by aku on 11/29/23.
//

#pragma once

namespace okapi
{
	/**
	 * Abstract class that applies a "curve" function to
	 * an input, usually from a joystick
	 */
	class DriveCurve
	{
	public:
		/**
		 *
		 * @param igain new curve gain
		 */
		virtual void setGain(double igain) = 0;

		/**
		 *
		 * @return the current curve gain
		 */
		virtual double getGain() = 0;

		/**
		 *
		 * @param iinput uncurved stick input
		 * @return curved stick output based on the curve
		 * function
		 */
		virtual double calculate(double iinput) = 0;
	protected:
		double gain = 0.0;
	};
} // namespace okapi