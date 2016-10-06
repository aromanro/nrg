#include "stdafx.h"
#include "Hamiltonian.h"


namespace NRG {

	Hamiltonian::Hamiltonian(int size)
		: Operator(size, false)
	{
	}

	
	void Hamiltonian::Diagonalize()
	{
		solver.compute(matrix);

		ASSERT(solver.info() == Eigen::ComputationInfo::Success);
		
		// the eigenvalues and eigenvectors are already sorted
		// the eigenvectors are normalized
		// the SelfAdjointEigenSolver diagonalization from Eigen takes care of those
		// for another solver, sorting needs to be done afterwards
		
		matrix = eigenvalues().asDiagonal();
	}
}

