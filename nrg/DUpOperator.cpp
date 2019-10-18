#include "stdafx.h"
#include "DUpOperator.h"

namespace NRG {

	DUpOperator::DUpOperator(int dim)
		: SpectralOperator(dim)
	{
		ASSERT(dim % 4 == 0);

		const int subsize = dim / 4;
		matrix.block(0, subsize, subsize, subsize) = Eigen::MatrixXd::Identity(subsize, subsize);
		matrix.block(2ULL * subsize, 3ULL * subsize, subsize, subsize) = Eigen::MatrixXd::Identity(subsize, subsize);
	}

}