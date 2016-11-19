#include <iostream>
#include "ELM.h"

using namespace std;

int main()
{
	ELM mac;

	mat x;
	mat y;

	/*Preparing Data */

	/*Load Train Data*/
	x.load("Data/x_train.csv",csv_ascii);
	y.load("Data/y_train.csv",csv_ascii);

	uint16_t Nh  = 2000; 	 //Number of Hidden Neurons
	uint16_t D   = x.n_cols; //Dimension of each X vector
	uint16_t N   = x.n_rows; //Number of data points
	uint16_t act = 0; 		 //Activation Type

	//cout<<"\n"<<D<<" "<<N<<"\n";

	/*Configure the ELM Parameters*/
	mac.Config_ELM(Nh,N,D);

	/*Train the Data*/
	bool r = mac.Train_ELM(x,y, act); 

	/*Load the Test Data*/
    x.load("Data/x_test.csv",csv_ascii);
	y.load("Data/y_test.csv",csv_ascii);

	/*Test the Data*/
	r = mac.Test_ELM(x,y);

	return 0;
}