#include "stdafx.h"
#include "Operator.h"

namespace NRG {

	Operator::Operator(unsigned int size, bool extendChangeSign)
		: changeSign(extendChangeSign), matrix(Eigen::MatrixXd::Zero(size, size))
	{
	}


	void Operator::Extend()
	{
		const unsigned int oldsize = static_cast<unsigned int>(matrix.rows());
		const unsigned int newsize = 4 * oldsize;

		Eigen::MatrixXd newop = Eigen::MatrixXd::Zero(newsize, newsize);

		newop.block(0, 0, oldsize, oldsize) = matrix;

		if (changeSign)
		{
			newop.block(oldsize, oldsize, oldsize, oldsize) = -matrix;
			newop.block(2ULL * oldsize, 2ULL * oldsize, oldsize, oldsize) = -matrix;
		}
		else
		{
			newop.block(oldsize, oldsize, oldsize, oldsize) = matrix;
			newop.block(2ULL * oldsize, 2ULL * oldsize, oldsize, oldsize) = matrix;
		}

		newop.block(3ULL * oldsize, 3ULL * oldsize, oldsize, oldsize)	= matrix;

		matrix.swap(newop);
	}



	// convention: order of states is 0=|0>, 1=|up>, 2=|down>, 3=|up, down>

	//<0|up|up> = <0|0> = 1
	//<down|up|up, down> = <down|down> = 1
	//the others are 0
	FUpOperator::FUpOperator(unsigned int dim)
		: Operator(dim)
	{
		ASSERT(dim % 4 == 0);

		const int subsize = dim / 4;
		matrix.block(2ULL * subsize, 3ULL * subsize, subsize, subsize) = matrix.block(0, subsize, subsize, subsize) = Eigen::MatrixXd::Identity(subsize, subsize);
	}

	//<0|down|down> = <0|0> = 1
	//<up|down|up, down> = -<up|up> = -1
	//the others are 0
	FDownOperator::FDownOperator(unsigned int dim)
		: Operator(dim)
	{
		ASSERT(dim % 4 == 0);

		const int subsize = dim / 4;
		matrix.block(0, 2ULL * subsize, subsize, subsize) = Eigen::MatrixXd::Identity(subsize, subsize);
		matrix.block(subsize, 3ULL * subsize, subsize, subsize) = -Eigen::MatrixXd::Identity(subsize, subsize);
	}

	
}



