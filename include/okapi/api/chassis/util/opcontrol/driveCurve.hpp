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
		virtual double getGain() const = 0;

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

	struct TankDriveCurves
	{
		std::shared_ptr<DriveCurve> leftCurve;
		std::shared_ptr<DriveCurve> rightCurve;

		TankDriveCurves(const std::shared_ptr<DriveCurve> ileftCurve,
						const std::shared_ptr<DriveCurve> irightCurve)
		{
			leftCurve = std::move(ileftCurve);
			rightCurve = std::move(irightCurve);
		}
	};

	struct ArcadeDriveCurves
	{
		std::shared_ptr<DriveCurve> forwardCurve;
		std::shared_ptr<DriveCurve> yawCurve;

		ArcadeDriveCurves(const std::shared_ptr<DriveCurve> iforwardCurve,
						  const std::shared_ptr<DriveCurve> iyawCurve)
		{
			forwardCurve = std::move(iforwardCurve);
			yawCurve = std::move(iyawCurve);
		}
	};

	struct CurvatureDriveCurves
	{
		std::shared_ptr<DriveCurve> forwardCurve;
		std::shared_ptr<DriveCurve> curvatureCurve;

		CurvatureDriveCurves(const std::shared_ptr<DriveCurve> iforwardCurve,
							 const std::shared_ptr<DriveCurve> icurvatureCurve)
		{
			forwardCurve = std::move(iforwardCurve);
			curvatureCurve = std::move(icurvatureCurve);
		}
	};

	enum class DriveCurves { TankDriveCurves, ArcadeDriveCurves, CurvatureDriveCurves };
} // namespace okapi