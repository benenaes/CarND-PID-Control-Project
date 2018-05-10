#include "twiddle.h"

#include <limits>

TwiddleOptimizer::TwiddleOptimizer(
	const double tau_proportional,
	const double tau_integral,
	const double tau_differential) :
		m_tau_proportional(tau_proportional),
		m_tau_integral(tau_integral),
		m_tau_differential(tau_differential),
		m_tau_proportional_delta(tau_proportional / 2),
		m_tau_integral_delta(tau_integral / 2),
		m_tau_differential_delta(tau_differential / 2),
		m_CurrentStep(TauStep::PROPORTIONAL_STEP),
		m_CurrentStepState(StepState::STEP_INCREASE),
		m_BestError(std::numeric_limits<double>::max())
{
}

void TwiddleOptimizer::update_cycle(double new_error)
{
	std::cout << "New error: " << new_error << " , best error: " << m_BestError << std::endl;

	switch (m_CurrentStepState)
	{
	case StepState::STEP_INCREASE:
	{
		if (new_error < m_BestError)
		{
			m_BestError = new_error;
			updateCurrentStepDelta(1.1);

			nextStepParameter();
			updateCurrentStep(1);
		}
		else
		{
			updateCurrentStep(-2);

			m_CurrentStepState = StepState::STEP_DECREASE;
		}
		break;
	}
	case StepState::STEP_DECREASE:
	{
		if (new_error < m_BestError)
		{
			m_BestError = new_error;
			updateCurrentStepDelta(1.1);
		}
		else
		{
			updateCurrentStep(1);

			updateCurrentStepDelta(0.9);
		}

		nextStepParameter();
		updateCurrentStep(1);
		m_CurrentStepState = StepState::STEP_INCREASE;
		break;
	}
	}
}

void TwiddleOptimizer::updateCurrentStep(const double delta_factor)
{
	switch (m_CurrentStep)
	{
	case TauStep::PROPORTIONAL_STEP:
	{
		m_tau_proportional += delta_factor * m_tau_proportional_delta;

		break;
	}
	case TauStep::DIFFERENTIAL_STEP:
	{
		m_tau_differential += delta_factor * m_tau_differential_delta;

		break;
	}
	case TauStep::INTEGRAL_STEP:
	{
		m_tau_integral += delta_factor * m_tau_integral_delta;

		break;
	}
	}
}

void TwiddleOptimizer::nextStepParameter()
{
	switch (m_CurrentStep)
	{
	case TauStep::PROPORTIONAL_STEP:
	{
		m_CurrentStep = TauStep::DIFFERENTIAL_STEP;

		break;
	}
	case TauStep::DIFFERENTIAL_STEP:
	{
		m_CurrentStep = TauStep::INTEGRAL_STEP;

		break;
	}
	case TauStep::INTEGRAL_STEP:
	{
		m_CurrentStep = TauStep::PROPORTIONAL_STEP;

		break;
	}
	}
}

void TwiddleOptimizer::updateCurrentStepDelta(const double factor)
{

	switch (m_CurrentStep)
	{
	case TauStep::PROPORTIONAL_STEP:
	{
		m_tau_proportional_delta *= factor;
		break;
	}
	case TauStep::DIFFERENTIAL_STEP:
	{
		m_tau_differential_delta *= factor;
		break;
	}
	case TauStep::INTEGRAL_STEP:
	{
		m_tau_integral_delta *= factor;
		break;
	}
	}
}

double TwiddleOptimizer::get_tau_proportional() const
{
	return m_tau_proportional;
}

double TwiddleOptimizer::get_tau_differential() const
{
	return m_tau_differential;
}

double TwiddleOptimizer::get_tau_integral() const
{
	return m_tau_integral;
}

std::ostream& operator<<(std::ostream& os, const TwiddleOptimizer& twiddle)
{
	os << "Current twiddle optimizer state: " << std::endl;

	os << "Current step: ";
	switch (twiddle.m_CurrentStep)
	{
	case TwiddleOptimizer::TauStep::PROPORTIONAL_STEP:
	{
		os << "proportional" << std::endl;
		break;
	}
	case TwiddleOptimizer::TauStep::DIFFERENTIAL_STEP:
	{
		os << "differential" << std::endl;
		break;
	}
	case TwiddleOptimizer::TauStep::INTEGRAL_STEP:
	{
		os << "integral" << std::endl;
		break;
	}
	}
	os << "Current delta state: ";
	switch (twiddle.m_CurrentStepState)
	{
	case TwiddleOptimizer::StepState::STEP_INCREASE:
	{
		os << "increase" << std::endl;
		break;
	}
	case TwiddleOptimizer::StepState::STEP_DECREASE:
	{
		os << "decrease" << std::endl;
		break;
	}
	}
	os << "Tau proportional: " << twiddle.get_tau_proportional() << std::endl;
	os << "Tau integral: " << twiddle.get_tau_integral() << std::endl;
	os << "Tau differential: " << twiddle.get_tau_differential() << std::endl;
	os << "Tau proportional delta: " << twiddle.m_tau_proportional_delta << std::endl;
	os << "Tau integral delta: " << twiddle.m_tau_integral_delta << std::endl;
	os << "Tau differential delta: " << twiddle.m_tau_differential_delta << std::endl << std::endl;
	return os;
}