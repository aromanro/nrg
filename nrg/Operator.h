#pragma once

#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI


#include <eigen>

namespace NRG {

	class Operator
	{
	protected:
		bool changeSign;

	public:
		// extendChangeSign should be true for fermionic operators
		// false for bosonic operators
		Operator(unsigned int size = 4, bool extendChangeSign = true);
		virtual ~Operator();

		Eigen::MatrixXd matrix;
		void Extend();

		inline static Eigen::MatrixXd KroneckerProduct(const Eigen::MatrixXd& mat1, const Eigen::MatrixXd& mat2)
			{
				Eigen::MatrixXd result(mat1.rows() * mat2.rows(), mat1.cols() * mat2.cols());

				for (int i = 0; i < mat1.rows(); ++i)
					for (int j = 0; j < mat1.cols(); ++j)
						result.block(i * mat2.rows(), j * mat2.cols(), mat2.rows(), mat2.cols()) = mat1(i, j) * mat2;

				return result;
			}


			inline static Eigen::MatrixXd KroneckerProductWithIdentity(const Eigen::MatrixXd& mat, int identitySize)
			{
				Eigen::MatrixXd result(mat.rows() * identitySize, mat.cols() * identitySize);

				for (int i = 0; i < mat.rows(); ++i)
					for (int j = 0; j < mat.cols(); ++j)
						result.block(i * identitySize, j * identitySize, identitySize, identitySize) = mat(i, j) * Eigen::MatrixXd::Identity(identitySize, identitySize);

				return result;
			}

			inline static Eigen::MatrixXd IdentityKronecker(int identitySize, const Eigen::MatrixXd& mat)
			{
				Eigen::MatrixXd result = Eigen::MatrixXd::Zero(mat.rows() * identitySize, mat.cols() * identitySize);

				for (int i = 0; i < identitySize; ++i)
					result.block(i * mat.rows(), i * mat.cols(), mat.rows(), mat.cols()) = mat;

				return result;
			}
	};

	class FUpOperator : public Operator
	{
	public:
		FUpOperator(unsigned int dim = 4);
	};

	class FDownOperator : public Operator
	{
	public:
		FDownOperator(unsigned int dim = 4);
	};
	
}
