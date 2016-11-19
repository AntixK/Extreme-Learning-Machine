#ifndef ELM_H_
#define ELM_H_

#include <armadillo>
#include <random>
#include <math.h>

using namespace arma;

class ELM
{
private:
	uint16_t N_prime; //Number of Hidden Neurons
	uint16_t Dim; //Data Dimension
	uint16_t NI; //Number of data points
	mat Weight;
	vec bias;
	mat beta;
	uint16_t Activation;

	double Train_time;
	double Test_time;

	double Train_Accuracy;
	double Test_Accuracy;

public: 
	void Set_Dim(uint16_t Nh, uint16_t D, uint16_t N);
	void Init_Weight_bias(); //Initialise Weights and Biases
	void Config_ELM(uint16_t Nh, uint16_t N, uint16_t D);
	bool Train_ELM(mat &x_train, mat &y_train, uint16_t Act);
	bool Test_ELM(mat &x_test, mat &y_test);

	void Save_Model();
};


void ELM::Init_Weight_bias()
{
	bias.randu(N_prime,1);

	std::mt19937 engine(time(0));  // Mersenne twister random number engine
	std::uniform_real_distribution<double> distr(1.0, 2.0); 
	Weight.set_size(N_prime, Dim); 
	Weight.imbue( [&]() { return distr(engine); } );


}

void ELM::Set_Dim(uint16_t Nh, uint16_t D, uint16_t N)
{
	N_prime = Nh;
	Dim = D;
	NI = N;

}

/* Wrapper function for ELM initialization -
Input : 
---> Number of Hidden Neurons Nh
---> Number of Datapoint N
*/

void ELM::Config_ELM(uint16_t Nh, uint16_t N, uint16_t D)
{
	Set_Dim(Nh, D, N);
	arma_rng::set_seed_random();  
	Init_Weight_bias();
}

/*
Train ELM
Input : 
---> Training Data set x_train and y_train; 
---> Activation function
		0 - Sigmoid Function
		1 - Sine Function
		2 - Hardlim Function
		3 - Triangular Bias Function
		4 - Radial Basis Function

Output: Returns 1 in case of successful training
*/
bool ELM::Train_ELM(mat &x_train, mat &y_train, uint16_t Act)
{
	wall_clock timer;
	mat param;

	//cout<<"\n X_train\n"<<x_train;

	timer.tic();
		param = x_train*Weight.t(); /* (NI x N_prime) = (NI x Dim)*(Dim x N_prime)*/

		//cout<<"\n Param \n"<<param;

		mat H = zeros(NI,N_prime);

		double alpha = 0.2;
		double lambda = 5;
		Activation = Act;

		switch(Activation)
		{
			case 0 : //Sigmoid Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = 1.0 / (1.0 + exp(- (param(i,j)+bias(j))));
						}
					}
					break;
			case 1 :  //Sine Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = sin(param(i,j)+bias(j));
						}
					}
					break;
			case 2 :  //Hardlim Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = ((param(i,j)+bias(j)) > 0)? 1 : 0;
						}
					}
					break;
			case 3 :  //Traingular Bias Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = ((param(i,j)+bias(j)) <= 1)&&((param(i,j)+bias(j)) >= -1) ? 
									  (1-abs(param(i,j)+bias(j))) : 0.0;
						}
					}
					break;
			case 4 ://Radial Basis	Activation Function
					
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = exp(- ((param(i,j)+bias(j))*(param(i,j)+bias(j))));
						}
					}
					break;
			default : cout<< "Undefined Activation function\n";
					  return false;

		}

		mat H_inv = pinv(H);

		//cout<<"\n H Pseudoinverse \n"<<H_inv;

		beta = H_inv*y_train;

		//cout<< "\n Beta \n"<<beta;

	Train_time = timer.toc();
	mat y_out = H*beta;

	vec temp = y_train - y_out;

	double error = stddev(temp);
	cout<<"\n Train RMSE = "<<error <<"\n";

	//cout<<"\n Y out\n"<<y_out;

	//cout<<"\n Y train\n"<<y_train;

	cout<<"\n Training Time = "<<Train_time<<" seconds\n";
	Save_Model();

	return true;
}

void ELM::Save_Model()
{
	Weight.save("Model/Weight.csv",csv_ascii);
	bias.save("Model/Bias.csv",csv_ascii);
	beta.save("Model/Beta.csv",csv_ascii);
	//y_out.save("Results/y_out.csv",csv_ascii);

}


bool ELM::Test_ELM(mat &x_test, mat &y_test)
{
	wall_clock timer;
	mat param;

	//cout<<"\n X_test\n"<<x_test;

	timer.tic();
		param = x_test*Weight.t(); /* (NI x N_prime) = (NI x Dim)*(Dim x N_prime)*/

		//cout<<"\n Param \n"<<param;

		mat H = zeros(NI,N_prime);

		double alpha = 0.2;
		double lambda = 5;
	
		switch(Activation)
		{
			case 0 : //Sigmoid Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = 1.0 / (1.0 + exp(- (param(i,j)+bias(j))));
						}
					}
					break;
			case 1 :  //Sine Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = sin(param(i,j)+bias(j));
						}
					}
					break;
			case 2 :  //Hardlim Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = ((param(i,j)+bias(j)) > 0)? 1 : 0;
						}
					}
					break;
			case 3 :  //Traingular Bias Activation Function
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = ((param(i,j)+bias(j)) <= 1)&&((param(i,j)+bias(j)) >= -1) ? 
									  (1-abs(param(i,j)+bias(j))) : 0.0;
						}
					}
					break;
			case 4 ://Radial Basis	Activation Function
					
					for(int i=0;i<H.n_rows;++i)
					{
						for(int j=0; j<H.n_cols;++j)
						{
							H(i,j) = exp(- ((param(i,j)+bias(j) - alpha)*(param(i,j)+bias(j) - alpha))/lambda);
						}
					}
					break;
			default : cout<< "Undefined Activation function\n";
					  return false;

		}

	Test_time = timer.toc();

	mat y_out = H*beta;

	vec temp = y_test - y_out;

	double error = stddev(temp);
	cout<<"\n Test RMSE = "<<error <<"\n";

	cout<<"\n Testing Time = "<<Test_time<<" seconds\n";

	y_out.save("Results/y_out.csv",csv_ascii);
	return true;
}

#endif