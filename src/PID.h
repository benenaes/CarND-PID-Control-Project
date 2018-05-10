#ifndef PID_H
#define PID_H

#include <chrono>

class PID 
{
public:
	/*
	* Constructor
	*/
	PID();

	/*
	* Destructor.
	*/
	virtual ~PID();

	/*
	* Initialize PID.
	*/
	void Init(double Kp, double Ki, double Kd);

	/*
	* Update the PID error variables given cross track error.
	*/
	void UpdateError(const double cte, const std::chrono::duration<double>);

	/*
	* Calculate the total PID error.
	*/
	double TotalError() const;

	double AverageError() const;

private:
	/*
	* Coefficients
	*/
	double m_Kp;
	double m_Ki;
	double m_Kd;

	/*
	* Errors
	*/
	double m_p_error;
	double m_i_error;
	double m_d_error;

	double m_TotalSquareError;
	unsigned int m_Iterations;
};

#endif /* PID_H */
