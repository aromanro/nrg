#pragma once

#include "Operator.h"

namespace NRG {

	class Hamiltonian : public Operator
	{
	protected:
		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver;
	public:
		Hamiltonian(int size = 4);

		void Diagonalize();
		const Eigen::MatrixXd& eigenvectors() const { return solver.eigenvectors(); }
		const Eigen::VectorXd& eigenvalues() const { return solver.eigenvalues(); }
	};

}
