#include "PID.h"

#include <iostream>

using namespace std;

PID::PID():
	m_Kp(0),
	m_Ki(0),
	m_Kd(0),
	m_p_error(0),
	m_i_error(0),
	m_d_error(0)
{
}

PID::~PID() 
{
}

void PID::Init(double Kp, double Ki, double Kd) 
{
	m_Kp = Kp;
	m_Ki = Ki;
	m_Kd = Kd;

	m_p_error = 0;
	m_i_error = 0;

	m_TotalSquareError = 0;
	m_Iterations = 0;
}

void PID::UpdateError(const double cte, const std::chrono::duration<double> duration) 
{
	if (duration.count() <= 0)
	{
		m_d_error = 0;
	}
	else
	{
		if (duration.count() >= 0.08)
		{
			std::cout << "Current delta t > 0.08: " << duration.count() << std::endl;
		}

		m_d_error = (cte - m_p_error) / duration.count();
	}

	m_p_error = cte;
	m_i_error += cte;

	m_TotalSquareError += cte * cte;

	++m_Iterations;
}

double PID::TotalError() const
{
	return (-m_Kp * m_p_error - m_Ki * m_i_error - m_Kd * m_d_error);
}

double PID::AverageError() const
{
	return m_TotalSquareError / m_Iterations;
}
