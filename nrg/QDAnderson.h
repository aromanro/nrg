#pragma once
#include "NRGAlgorithm.h"

namespace NRG {

	class QDAnderson :
		public NRGAlgorithm
	{
	public:
		QDAnderson(int nrsteps = 50, int maxSize = 200, double Lambda = 2.);
		virtual ~QDAnderson();

		virtual void Init();
	};


}
