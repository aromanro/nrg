#include "stdafx.h"
#include "NRGAlgorithm.h"
#include "Hamiltonian.h"



namespace NRG {

	NRGAlgorithm::NRGAlgorithm(int nrsteps, int maxsize, double lambda, int startHamiltonianSize)
		: NrSteps(nrsteps), Lambda(lambda), t(0), maxSize(maxsize), curMatrixSize(startHamiltonianSize),
		fUpOperator(startHamiltonianSize), fDownOperator(startHamiltonianSize),
		passData(nullptr), controller(nullptr), startIteration(0)
	{
		SqrtLambda = sqrt(Lambda);
		Rescale = 0.5 * (1. + 1. / Lambda);
	}


	NRGAlgorithm::~NRGAlgorithm()
	{
		for (auto &op : staticOperators) delete op;
		for (auto &op : spectralOperators) delete op;
	}

	double NRGAlgorithm::GetCouplingForIteration(int iter) const
	{
		return (1. - pow(Lambda, -iter - 1.)) / sqrt((1. - pow(Lambda, -2 * iter - 1))*(1. - pow(Lambda, -2 * iter - 3)));
	}

	void NRGAlgorithm::AddSite()
	{
		// diagonal blocks: sqrt(Lambda) * Hamiltonian
		hamiltonian.matrix *= SqrtLambda;
		hamiltonian.Extend(); //now the size is 4 * curMatrixSize

		// off diagonal blocks
		const Eigen::MatrixXd fUpOperatormatrix = t * fUpOperator.matrix;
		const Eigen::MatrixXd fDownOperatormatrix = t * fDownOperator.matrix;
		
		const Eigen::MatrixXd fUpOperatorTmatrix = fUpOperatormatrix.adjoint();
		const Eigen::MatrixXd fDownOperatorTmatrix = fDownOperatormatrix.adjoint();

		//first 'row'
		hamiltonian.matrix.block(0, curMatrixSize, curMatrixSize, curMatrixSize) = fUpOperatorTmatrix;
		hamiltonian.matrix.block(0, 2ULL * curMatrixSize, curMatrixSize, curMatrixSize) = fDownOperatorTmatrix;

		// second 'row'
		hamiltonian.matrix.block(curMatrixSize, 0, curMatrixSize, curMatrixSize) = fUpOperatormatrix;
		hamiltonian.matrix.block(curMatrixSize, 3ULL * curMatrixSize, curMatrixSize, curMatrixSize) = fDownOperatorTmatrix;

		// third 'row'
		hamiltonian.matrix.block(2ULL * curMatrixSize, 0, curMatrixSize, curMatrixSize) = fDownOperatormatrix;
		hamiltonian.matrix.block(2ULL * curMatrixSize, 3ULL * curMatrixSize, curMatrixSize, curMatrixSize) = -fUpOperatorTmatrix;

		// last 'row'
		hamiltonian.matrix.block(3ULL * curMatrixSize, curMatrixSize, curMatrixSize, curMatrixSize) = fDownOperatormatrix;
		hamiltonian.matrix.block(3ULL * curMatrixSize, 2ULL * curMatrixSize, curMatrixSize, curMatrixSize) = -fUpOperatormatrix;
	}

	
	void NRGAlgorithm::Step(int iter)
	{		
		AddSite();

		const int enlargedMatrixSize = 4 * curMatrixSize;
		const int nextMatrixSize = min(enlargedMatrixSize, maxSize);

		// diagonalize the hamiltonian
		// the eigenvalues and eigenvectors are already sorted
		// the eigenvectors are normalized
		// the diagonalization from eigen takes care of those
		// the SelfAdjointEigenSolver does that, for another solver sorting might need be done afterwards
		hamiltonian.Diagonalize();
		const Eigen::VectorXd& evals = hamiltonian.eigenvalues();
		const Eigen::MatrixXd& evecs = hamiltonian.eigenvectors();


		// transform the hamiltonian and the operators to the new truncated basis

		// switch the hamiltonian to the diagonalized one
		hamiltonian.matrix = hamiltonian.matrix.block(0, 0, nextMatrixSize, nextMatrixSize).eval();

		// truncate the eigenbasis
		const Eigen::MatrixXd Ut = evecs.block(0, 0, enlargedMatrixSize, nextMatrixSize);
		const Eigen::MatrixXd U = Ut.adjoint();
		
		// the operators for the added site must be also in the new basis
		FUpOperator currentfUpOperator(enlargedMatrixSize);
		FDownOperator currentfDownOperator(enlargedMatrixSize);
		
		fUpOperator.matrix = U * currentfUpOperator.matrix * Ut;
		fDownOperator.matrix = U * currentfDownOperator.matrix * Ut;

		// now change the basis for the static and spectral operators, too
		for (auto &op : staticOperators)
		{
			op->Extend();
			op->matrix = U * op->matrix * Ut;
		}

		for (auto &op : spectralOperators)
		{
			op->Extend();
			op->matrix = U * op->matrix * Ut;

			op->PassSpectral(iter, Rescale * pow(Lambda, -(iter - 1.)/2.), evals);
		}

		// pass eigenvalues for the renormalization group flow chart
		if (passData) passData->PassEigenvalues(iter, evals, Rescale);

		// change values for the next iteration
		curMatrixSize = nextMatrixSize;
		t = GetCouplingForIteration(iter);
	}

	void NRGAlgorithm::AdjustForEnergyScale()
	{
		const double InvRescale = 1. / Rescale;
		if (startIteration == -1)
		{
			// adjust for the energy scale of the -1 iteration
			hamiltonian.matrix /= Lambda;

			t /= SqrtLambda;

			// the Rescale factor was taken out
			t *= InvRescale;
		}

		hamiltonian.matrix *= InvRescale;
	}

	void NRGAlgorithm::Calculate()
	{
		bool stopped = false;

		AdjustForEnergyScale();


		// There is no need to start with a diagonalized Hamiltonian

		
		/*
		// start with a diagonalized Hamiltonian 
		// for the simple Anderson model in this program it already is in diagonal form
		// but for Kondo it might not be, unless it's diagonalized in Init
		// the same for the double quantum dot system

		hamiltonian.Diagonalize();

		const Eigen::MatrixXd& Ut = hamiltonian.eigenvectors();
		const Eigen::MatrixXd U = Ut.adjoint();

		// put the operators in the same basis as H
		fUpOperator.matrix = U * fUpOperator.matrix * Ut;
		fDownOperator.matrix = U * fDownOperator.matrix * Ut;

		for (auto &op : staticOperators) op->matrix = U * op->matrix * Ut;
		for (auto &op : spectralOperators) op->matrix = U * op->matrix * Ut;		
		*/

		// the iteration over the Wilson chain
		for (int iter = startIteration + 1; iter <= NrSteps; ++iter)
		{
			Step(iter);

			TRACE("Iteration number: %d\n", iter);

			if (controller && controller->ShouldCancel()) {
				stopped = true;
				break;
			}
		}

		if (passData) passData->Finished(stopped ? nullptr : this);
	}
}


