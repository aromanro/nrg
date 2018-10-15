#include "stdafx.h"
#include "Operator.h"

namespace NRG {

	Operator::Operator(unsigned int size, bool extendChangeSign)
		: changeSign(extendChangeSign)
	{
		matrix = Eigen::MatrixXd::Zero(size, size);
	}


	Operator::~Operator()
	{
	}


	void Operator::Extend()
	{
		Eigen::MatrixXd newop;
		const unsigned int oldsize = static_cast<unsigned int>(matrix.rows());
		const unsigned int newsize = 4 * oldsize;

		newop = Eigen::MatrixXd::Zero(newsize, newsize);

		newop.block(0, 0, oldsize, oldsize) = matrix;

		if (changeSign)
		{
			newop.block(oldsize, oldsize, oldsize, oldsize) = -matrix;
			newop.block(2 * oldsize, 2 * oldsize, oldsize, oldsize) = -matrix;
		}
		else
		{
			newop.block(oldsize, oldsize, oldsize, oldsize) = matrix;
			newop.block(2 * oldsize, 2 * oldsize, oldsize, oldsize) = matrix;
		}

		newop.block(3 * oldsize, 3 * oldsize, oldsize, oldsize)	= matrix;

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
		matrix.block(0, subsize, subsize, subsize) = Eigen::MatrixXd::Identity(subsize, subsize);
		matrix.block(2 * subsize, 3 * subsize, subsize, subsize) = Eigen::MatrixXd::Identity(subsize, subsize);
	}

	//<0|down|down> = <0|0> = 1
	//<up|down|up, down> = -<up|up> = -1
	//the others are 0
	FDownOperator::FDownOperator(unsigned int dim)
		: Operator(dim)
	{
		ASSERT(dim % 4 == 0);

		const int subsize = dim / 4;
		matrix.block(0, 2 * subsize, subsize, subsize) = Eigen::MatrixXd::Identity(subsize, subsize);
		matrix.block(subsize, 3 * subsize, subsize, subsize) = -Eigen::MatrixXd::Identity(subsize, subsize);
	}

	
}



