#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

//every neure
struct neure {
	double z;//Unactivated value
	double a;//Value after activation

};

double relu(double x) {
	if (x < 0) {
		return 0;
	}
	else {
		return x;
	}
}

double sigmoid(double x) {
	return 1.0 / (1 + exp(-x));
}

double desigmoid(double x) {
	return sigmoid(x)*(1 - sigmoid(x));
}

/*Calculate the cumulative sum of individual neurons
	W is the weight matrix of this layer
	N is the value of the previous layer
	P is the number of nodes in this layer
	B is offset
*/
double singleCalculate(vector<vector<double>> w, vector<neure> n, int p, int b) {

	double ansUnactive = 0;
	//double ans = 0;
	for (int i = 0; i < n.size(); i++) {
		//cout << "v:" << n[i].a << " w:" << w[i][p] << endl;
		ansUnactive += n[i].a * w[i][p];
	}
	ansUnactive += b;//offset

	//cout << ansUnactive << endl;
	//ans = sigmoid(ansUnactive);
	return ansUnactive;
}

/*Calculate each neuron in each layer
	weight is the entire weight matrix
	start is the input value
*/
vector<vector<neure>> forwardCalculate(vector<vector<vector<double>>> weight, vector<double> start) {



	double ans = 0;
	int layerNumber = weight.size();

	//Initialize and pass the start input to allValue
	vector<neure>valueStart;
	for (int i = 0; i < start.size(); i++) {
		neure startNeure;
		startNeure.a = start[i];
		startNeure.z = 0;
		valueStart.push_back(startNeure);
	}
	vector<vector<neure>> allValue;
	allValue.push_back(valueStart);

	//Forward propagation calculates the value in allValue
	for (int i = 0; i < layerNumber; i++) {
		int pNumInEachLayer = weight[i].size();//Find the number of neurons in each layer
		vector<neure> layerNeureTmp;
		for (int j = 0; j < pNumInEachLayer; j++) {
			neure ntmp;
			ntmp.z = singleCalculate(weight[i], allValue[i], j, 1);
			ntmp.a = sigmoid(ntmp.z);
			layerNeureTmp.push_back(ntmp);
		}
		allValue.push_back(layerNeureTmp);
	}
	return allValue;
}

/* Backpropagation calculation
	weight is the entire weight matrix
	allValue have z and a for each node
	y is expected value
	n is learning rate
	*/
vector<vector<vector<double>>> backCalculate(vector<vector<vector<double>>> weight, vector<vector<neure>> allValue, vector<double> y, double n) {

	//Backpropagation calculation between the penultimate layer and the final output layer
	const int wnum = 2;
	double we2[wnum][wnum] = {};//The weight matrix is fixed here to be 2 * 2.
	for (int i = 0; i < wnum; i++) {
		for (int j = 0; j < wnum; j++) {
			we2[i][j] = weight[1][i][j];
		}
	}

	double midTmp[wnum][wnum] = {};

	for (int k = 0; k < allValue.size() - 1; k++) {
		for (int i = allValue.size() - 1; i > 1; i--) {
			for (int j = 0; j < allValue[i].size(); j++) {
				double td = 1;
				double error = allValue[i][j].a - y[j];
				td *= error;
				double ds = desigmoid(allValue[i][j].z);
				td *= ds;
				midTmp[i - 1][j] = td;
				td *= allValue[i - 1][k].a;
				we2[k][j] -= n * td;
			}
		}
	}
	//for (int i = 0; i < wnum; i++) {
	//	for (int j = 0; j < wnum; j++) {
	//		cout << we2[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	double we1[wnum][wnum] = {};//New weights for the first layer
	for (int i = 0; i < wnum; i++) {
		for (int j = 0; j < wnum; j++) {
			we1[i][j] = weight[0][i][j];
		}
	}

	//Update the weight between the first and second layers
	for (int k = 0; k < allValue.size() - 1; k++) {
		for (int i = allValue.size() - 2; i > 0; i--) {
			for (int j = 0; j < allValue[i].size(); j++) {
				double td = 1;
				double preCalError = midTmp[i][0] * we2[j][0] + midTmp[i][1] * we2[j][1];
				td *= preCalError;
				td *= desigmoid(allValue[i][j].z);
				midTmp[i - 1][j] = td;
				td *= allValue[i - 1][k].a;
				we1[k][j] -= n * td;
			}
		}
	}

	//for (int i = 0; i < wnum; i++) {
	//	for (int j = 0; j < wnum; j++) {
	//		cout << we1[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	vector<vector<vector<double>>> newWeight;
	vector<vector<double>> layer1Tmp;
	for (int i = 0; i < 2; i++) {
		vector<double> tmp;
		for (int j = 0; j < 2; j++) {
			tmp.push_back(we1[i][j]);
		}
		layer1Tmp.push_back(tmp);
	}
	newWeight.push_back(layer1Tmp);
	vector<vector<double>> layer2Tmp;
	for (int i = 0; i < 2; i++) {
		vector<double> tmp;
		for (int j = 0; j < 2; j++) {
			tmp.push_back(we2[i][j]);
		}
		layer2Tmp.push_back(tmp);
	}
	newWeight.push_back(layer2Tmp);

	/*for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++){
				cout << newWeight[i][j][k] << " ";
			}
			cout << endl;
		}
		cout << "-----" << endl;
	}*/
	return newWeight;

}


//train
vector<vector<vector<double>>> train() {
	//cout << sigmoid(1.689037) << endl;
	
	vector<vector<vector<double>>> weight;
	vector<double> start = { 0.94, 0.06 };// input value
	//vector<vector<double>> value;
	//value.push_back(start);

	vector<vector<neure>> allValue;

	//vector<double> z;//Unactivated intermediate value array

	vector<double> end = { 0.95,0.05 };//Our expected output value


	//0.15 0.20 0.25 0.30 0.40 0.45 0.50 0.88
	int pnum = 2;//Number of nodes per layer
	int layerNumber = 2;//Number of layers
	double w;

	cout << "Input init weight:" << endl;
	for (int k = 0; k < layerNumber; k++) {
		vector < vector<double> > layer;
		for (int i = 0; i < pnum; i++) {
			vector<double> oneToOther; //Weights from the jth node to the next layer of nodes
			for (int j = 0; j < pnum; j++) {
				cin >> w;
				oneToOther.push_back(w);
			}
			layer.push_back(oneToOther);
		}
		weight.push_back(layer);
	}
	//Show weights
	/*for (int k = 0; k < layerNumber; k++) {
		cout <<k+1<<"-th layer weight" << endl;
		for (int i = 0; i < pnum; i++) {
			for (int j = 0; j < pnum; j++) {
				cout <<i<<"-to-"<<j<<" weight: "<< weight[k][i][j] << endl;
			}
		}
		cout << endl;
	}*/

	//Forward propagation
	allValue = forwardCalculate(weight, start);
	cout << "O1 output :" << allValue[allValue.size() - 1][0].a << " O2 output :" << allValue[allValue.size() - 1][1].a << endl;//output
	//cout << "0 z :" << allValue[allValue.size() - 1][0].z << ", 1 z :" << allValue[allValue.size() - 1][1].z << endl;

	double error = abs(allValue[allValue.size() - 1][0].a - end[0]);
	cout << error << endl;
	vector<vector<vector<double>>> newWeight;
	vector<vector<vector<double>>> prenewWeight = weight;

	int train_epoch = 0;

	while (error >= 0.001) {

		//newWeight.clear();
		//Back Propagation
		newWeight = backCalculate(prenewWeight, allValue, end, 0.05);

		train_epoch++;
		cout << "train epoch :" << train_epoch << endl;

		allValue.clear();
		allValue = forwardCalculate(newWeight, start);
		cout << "O1 output :" << allValue[allValue.size() - 1][0].a << " O2 output :" << allValue[allValue.size() - 1][1].a << endl;
		error = abs(allValue[allValue.size() - 1][0].a - end[0]);
		prenewWeight.clear();
		prenewWeight = newWeight;

		
	}
	
	cout << "The end weight : " << endl;
	int wcount = 1;
	for (int k = 0; k < layerNumber; k++) {
		for (int i = 0; i < pnum; i++) {
			for (int j = 0; j < pnum; j++) {
				cout << "w" << wcount << " is:" << newWeight[k][i][j] <<" ";
				wcount++;
			}
		}
	}
	cout << endl;
	cout << "train finish!" << endl;
	return newWeight;
}

void predict(vector<vector<vector<double>>> weight, vector <double> start) {
	vector<vector<neure>> ans;
	ans = forwardCalculate(weight, start);
	cout << "O1 output :" << ans[ans.size() - 1][0].a << " O2 output :" << ans[ans.size() - 1][1].a << endl;//output
}

int main() {

	vector<double> start;
	int pnum = 2;
	
	vector<vector<vector<double>>> weight;

	//train
	weight = train();

	cout << endl;

	//input
	cout << "please input x1 x2:" << endl;
	for (int i = 0; i < pnum; i++) {
		double x;
		cin >> x;
		start.push_back(x);
	}

	//predict
	predict(weight, start);

	system("pause");
	return 0;

}
