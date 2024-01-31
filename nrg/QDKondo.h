#pragma once
#include "NRGAlgorithm.h"


namespace NRG {

	class QDKondo :
		public NRGAlgorithm
	{
	public:
		QDKondo(int nrsteps = 50, int maxSize = 200, double Lambda = 2.);
		~QDKondo() override;

		void Init() override;
	};

}
