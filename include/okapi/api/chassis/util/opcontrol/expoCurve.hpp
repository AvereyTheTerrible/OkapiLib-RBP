//
// Created by aku on 11/29/23.
//

#pragma once

#include "okapi/api/chassis/util/opcontrol/driveCurve.hpp"

namespace okapi
{
	class ExpoCurve : public DriveCurve
	{
	public:
		/**
		 * Creates an exponential curve class based on
		 * 5225A Pilons. Link to Desmos graph:
		 * https://www.desmos.com/calculator/rcfjjg83zx
		 * (red graph is E-Bots Pilons)
		 * @param igain
		 */
		explicit ExpoCurve(const double igain);
		/**
		 * @param igain New curve gain
		 */
		void setGain(const double igain) override;

		/**
		 * @return The current curve gain
		 */
		double getGain() const override;

		/**
		 * @param iinput Uncurved stick input
		 * @return Curved stick output based on the curve
		 * function
		 */
		double calculate(const double iinput) override;
	};
} // namespace okapi
