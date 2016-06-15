#pragma once

#include <eigen>

namespace NRG {

	class Operator
	{
	protected:
		bool changeSign;
	public:
		// extendChangeSign should be true for fermionic operators
		// false for bosonic operators
		Operator(unsigned int size = 4, bool extendChangeSign = true);
		virtual ~Operator();

		Eigen::MatrixXd matrix;
		void Extend();
	};

	class FUpOperator : public Operator
	{
	public:
		FUpOperator(unsigned int dim = 4);
	};

	class FDownOperator : public Operator
	{
	public:
		FDownOperator(unsigned int dim = 4);
	};
	
}
