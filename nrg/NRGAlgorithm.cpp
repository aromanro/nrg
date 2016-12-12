#include "stdafx.h"
#include "NRGAlgorithm.h"
#include "Hamiltonian.h"



namespace NRG {

	NRGAlgorithm::NRGAlgorithm(int nrsteps, int maxsize, double lambda, int startHamiltonianSize)
		: NrSteps(nrsteps), Lambda(lambda), t(0), maxSize(maxsize), curMatrixSize(startHamiltonianSize),
		fUpOperator(startHamiltonianSize), fDownOperator(startHamiltonianSize),
		passData(NULL), controller(NULL), startIteration(0)
	{
		SqrtLambda = sqrt(Lambda);
		Rescale = (1. + 1. / Lambda) / 2.;
	}


	NRGAlgorithm::~NRGAlgorithm()
	{
		for (auto &op : staticOperators) delete op;
		for (auto &op : spectralOperators) delete op;
	}

	double NRGAlgorithm::GetCouplingForIteration(int iter)
	{
		return (1. - pow(Lambda, -iter - 1.)) / sqrt((1. - pow(Lambda, -2 * iter - 1))*(1. - pow(Lambda, -2 * iter - 3)));
	}

	void NRGAlgorithm::AddSite()
	{
		// diagonal blocks: sqrt(Lambda) * Hamiltonian
		hamiltonian.matrix *= SqrtLambda;
		hamiltonian.Extend(); //now the size is 4 * curMatrixSize

		// off diagonal blocks
		Eigen::MatrixXd fUpOperatorTmatrix = fUpOperator.matrix.adjoint();
		Eigen::MatrixXd fDownOperatorTmatrix = fDownOperator.matrix.adjoint();

		//first 'row'
		hamiltonian.matrix.block(0, curMatrixSize, curMatrixSize, curMatrixSize) = t * fUpOperatorTmatrix;
		hamiltonian.matrix.block(0, 2 * curMatrixSize, curMatrixSize, curMatrixSize) = t * fDownOperatorTmatrix;

		// second 'row'
		hamiltonian.matrix.block(curMatrixSize, 0, curMatrixSize, curMatrixSize) = t * fUpOperator.matrix;
		hamiltonian.matrix.block(curMatrixSize, 3 * curMatrixSize, curMatrixSize, curMatrixSize) = t * fDownOperatorTmatrix;

		// third 'row'
		hamiltonian.matrix.block(2 * curMatrixSize, 0, curMatrixSize, curMatrixSize) = t * fDownOperator.matrix;
		hamiltonian.matrix.block(2 * curMatrixSize, 3 * curMatrixSize, curMatrixSize, curMatrixSize) = -t * fUpOperatorTmatrix;

		// last 'row'
		hamiltonian.matrix.block(3 * curMatrixSize, curMatrixSize, curMatrixSize, curMatrixSize) = t * fDownOperator.matrix;
		hamiltonian.matrix.block(3 * curMatrixSize, 2 * curMatrixSize, curMatrixSize, curMatrixSize) = -t * fUpOperator.matrix;
	}

	
	void NRGAlgorithm::Step(int iter)
	{		
		AddSite();

		int enlargedMatrixSize = 4 * curMatrixSize;
		int nextMatrixSize = min(enlargedMatrixSize, maxSize);

		// diagonalize the hamiltonian
		// the eigenvalues and eigenvectors are already sorted
		// the eigenvectors are normalized
		// the diagonalization from eigen takes care of those
		// the SelfAdjointEigenSolver does that, for another solver sorting might need be done afterwards
		hamiltonian.Diagonalize();
		Eigen::VectorXd evals = hamiltonian.eigenvalues();
		Eigen::MatrixXd evecs = hamiltonian.eigenvectors();


		// transform the hamiltonian and the operators to the new truncated basis

		// switch the hamiltonian to the diagonalized one
		hamiltonian.matrix = hamiltonian.matrix.block(0, 0, nextMatrixSize, nextMatrixSize).eval();

		// truncate the eigenbasis
		Eigen::MatrixXd Ut = evecs.block(0, 0, enlargedMatrixSize, nextMatrixSize);
		Eigen::MatrixXd U = Ut.adjoint();
		
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
		if (startIteration == -1)
		{
			// adjust for the energy scale of the -1 iteration
			hamiltonian.matrix /= Lambda;

			t /= SqrtLambda;

			// the Rescale factor was taken out
			t /= Rescale;
		}

		hamiltonian.matrix /= Rescale;
	}

	void NRGAlgorithm::Calculate()
	{
		bool stopped = false;

		AdjustForEnergyScale();

		// start with a diagonalized Hamiltonian 
		// for the simple Anderson model in this program it already is in diagonal form
		// but for Kondo it might not be, unless it's diagonalized in Init
		// the same for the double quantum dot system
				
		hamiltonian.Diagonalize();

		Eigen::MatrixXd Ut = hamiltonian.eigenvectors();
		Eigen::MatrixXd U = Ut.adjoint();

		// put the operators in the same basis as H
		fUpOperator.matrix = U * fUpOperator.matrix * Ut;
		fDownOperator.matrix = U * fDownOperator.matrix * Ut;

		for (auto &op : staticOperators) op->matrix = U * op->matrix * Ut;
		for (auto &op : spectralOperators) op->matrix = U * op->matrix * Ut;		
		
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


