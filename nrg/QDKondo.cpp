#include "stdafx.h"
#include "QDKondo.h"

#include "nrg.h"
#include "Options.h"


namespace NRG {

	QDKondo::QDKondo(int nrsteps, int maxSize, double Lambda)
		: NRGAlgorithm(nrsteps, maxSize, Lambda, 8)
	{
		startIteration = 0;

		t = GetCouplingForIteration(0);
	}

	void QDKondo::Init()
	{
		const double B = theApp.options.B;
		const double J = theApp.options.J;
		
		hamiltonian.matrix = Eigen::MatrixXd::Zero(curMatrixSize, curMatrixSize);

		static const unsigned int ImpUp = 0;
		static const unsigned int ImpDown = 1;

		static const unsigned int SiteVac = 0;
		static const unsigned int SiteUp = (1 << 1);
		static const unsigned int SiteDown = (2 << 1);
		static const unsigned int SiteUpDown = SiteUp + SiteDown;
		
		// on diagonal values, J * S^z * s^z - B S^z
		hamiltonian.matrix(ImpUp + SiteUp, ImpUp + SiteUp) =         0.25 * J       - 0.5 * B;
		hamiltonian.matrix(ImpDown + SiteUp, ImpDown + SiteUp) =    -0.25 * J       + 0.5 * B;
		hamiltonian.matrix(ImpUp + SiteDown, ImpUp + SiteDown) =    -0.25 * J       - 0.5 * B;
		hamiltonian.matrix(ImpDown + SiteDown, ImpDown + SiteDown) = 0.25 * J       + 0.5 * B;

		hamiltonian.matrix(ImpUp + SiteVac, ImpUp + SiteVac) =                         - 0.5 * B;
		hamiltonian.matrix(ImpUp + SiteUpDown, ImpUp + SiteUpDown) =                   - 0.5 * B;

		hamiltonian.matrix(ImpDown + SiteVac, ImpDown + SiteVac) =                       0.5 * B;
		hamiltonian.matrix(ImpDown + SiteUpDown, ImpDown + SiteUpDown) =                 0.5 * B;

		// off diagonal values, S^+ * s^- and S^- * s^+ with the 1/2 factor
		hamiltonian.matrix(ImpDown + SiteUp, ImpUp + SiteDown) =     0.5 * J;
		hamiltonian.matrix(ImpUp + SiteDown, ImpDown + SiteUp) =     0.5 * J;
		
		// need this operator for the spectral function
		
		/*
		SpectralOperator *sz = new SpectralOperator(2, false);
		sz->matrix(0, 0) = 0.5;
		sz->matrix(1, 1) = -0.5;

		sz->Extend();
		spectralOperators.push_back(sz);
		*/
		
		SpectralOperator *down = new SpectralOperator(curMatrixSize); 
		// this is the Fup^t for the zero site
		down->matrix = fDownOperator.matrix.adjoint();

		spectralOperators.push_back(down);				
	}

}
