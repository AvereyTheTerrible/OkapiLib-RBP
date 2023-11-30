//
// Created by aku on 11/30/23.
//

#include "okapi/api/chassis/util/opcontrol/expoCurve.hpp"

namespace okapi
{
	ExpoCurve::ExpoCurve(const double igain)
	: gain(igain)
	{
	}

	/**
	 * Implementation taken line for line from LemLib: https://github.com/LemLib/LemLib
	 */
	double ExpoCurve::calculate(const double iinput)
	{
		if (gain != 0) {
			return (powf(2.718, -(gain / 10))
			+ powf(2.718, (fabs(input) - 127) / 10) *
			(1 - powf(2.718, -(gain / 10)))) * input;
		}
		return input;
	}

	double getGain() const override
	{
		return gain;
	}

	void setGain(const double igain)
	{
		gain = igain;
	}
} // namespace okapi