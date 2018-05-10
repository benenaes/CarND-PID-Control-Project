#ifndef TWIDDLE_H
#define TWIDDLE_H

#include <iostream>

class TwiddleOptimizer
{
public:
	friend std::ostream& operator<<(std::ostream&, const TwiddleOptimizer&);

	TwiddleOptimizer(
		const double tau_proportional,
		const double tau_integral,
		const double tau_differential);

	void update_cycle(double new_error);

	double get_tau_proportional() const;
	double get_tau_differential() const;
	double get_tau_integral() const;

private:
	void updateCurrentStep(const double delta_factor);
	void updateCurrentStepDelta(const double factor);
	void nextStepParameter();

	enum class TauStep : int
	{
		PROPORTIONAL_STEP = 0,
		DIFFERENTIAL_STEP = 1,
		INTEGRAL_STEP = 2
	};

	enum class StepState : int
	{
		STEP_INCREASE = 0,
		STEP_DECREASE = 1
	};

	double m_tau_proportional;
	double m_tau_integral;
	double m_tau_differential;

	double m_tau_proportional_delta;
	double m_tau_integral_delta;
	double m_tau_differential_delta;

	TauStep m_CurrentStep;
	StepState m_CurrentStepState;

	double m_BestError;
};

#endif