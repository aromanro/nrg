#pragma once

#include "Operator.h"
#include "SpectralOperator.h"

namespace NRG {

	class DUpOperator : public SpectralOperator
	{
	public:
		DUpOperator(int dim = 4);
	};

}