#pragma once
#include "NRGAlgorithm.h"

namespace NRG {


	class TwoQDRKKY :
		public NRGAlgorithm
	{
	public:
		TwoQDRKKY(int nrsteps = 30, int maxSize = 600, double Lambda = 2);
		~TwoQDRKKY() override;

		void Init() override;
	};

}
