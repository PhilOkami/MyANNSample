#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;
#define in_n 2
#define hid_n 2
#define out_n 1
#define N 4
double w1[in_n][hid_n];
double w2[hid_n][out_n];
double theta_h[hid_n];
double theta_o[out_n];
double delta_o[out_n];
double delta_h[hid_n];
double hid_in[hid_n];
double hid_out[hid_n];
double output_in[out_n];
double output_out[out_n];
double input[N][in_n];
double output[N][out_n];
double error;
double epsilon;
double lr;
int NN;

//初始化函数
void initialize(){
	int i,j;
	error=0;
	for(i=0;i<hid_n;i++){
		//hid_in[i]=0;
		hid_out[i]=0;
		delta_h[i]=0;
		theta_h[i]=rand()/(RAND_MAX+1.0);
		//theta_h[i]=0.01;
	}
	for(i=0;i<out_n;i++){
		//output_in[i]=0;
		output_out[i]=0;
		theta_o[i]=rand()/(RAND_MAX+1.0);;
		delta_o[i]=0;
	} 
	for(i=0;i<in_n;i++)
		for(j=0;j<hid_n;j++)
			w1[i][j]=rand()/(RAND_MAX+1.0);
	for(i=0;i<hid_n;i++)
		for(j=0;j<out_n;j++)
			w2[i][j]=rand()/(RAND_MAX+1.0); 
	cout<<"初始化完成！"<<endl;
} 

double update(double lr,double input[N][in_n],double output[N][out_n]){
	int i,j,k;
	for(i=0;i<N;i++){
		//输入层到隐藏层前向计算
		for(j=0;j<hid_n;j++){ 
			hid_in[j]=0;
			for(k=0;k<in_n;k++)
				hid_in[j]+=w1[k][j]*input[i][k];
			hid_in[j]+=theta_h[j];
			hid_out[j]=1.0/(1.0+exp(-hid_in[j]));
		}
		//隐藏层到输出层前向计算
		for(j=0;j<out_n;j++){
			output_in[j]=0;
			for(k=0;k<hid_n;k++)
				output_in[j]+=w2[k][j]*hid_out[k];
			output_in[j]+=theta_o[j];
			output_out[j]=1.0/(1.0+exp(-output_in[j]));
		} 
		for(j=0;j<out_n;j++){
			delta_o[j]=output_out[j]*(1-output_out[j])*(output[i][j]-output_out[j]);
			theta_o[j]+=lr*delta_o[j];
		} 
		for(j=0;j<hid_n;j++){
			double z=0;
			for(k=0;k<out_n;k++)
				z+=w2[j][k]*delta_o[k];
			delta_h[j]=z*hid_out[j]*(1-hid_out[j]);
			theta_h[j]+=lr*delta_h[j];
		}
		for(j=0;j<hid_n;j++){
			for(k=0;k<out_n;k++){
				w2[j][k]+=lr*hid_out[j]*delta_o[k];
			}
		}
		for(j=0;j<in_n;j++){
			for(k=0;k<hid_n;k++){
				w1[j][k]+=lr*input[i][j]*delta_h[k];
			}
		}
		double delta=0;
		for(j=0;j<out_n;j++)
			delta+=pow((output_out[j]-output[i][j]),2);
		error+=delta/2;
	}
	return error;
}

int main(){
	int count; 
	cout<<"输入样本特征值:"<<endl;
	for(int i=0;i<N;i++){
		for(int j=0;j<in_n;j++)
			cin>>input[i][j];
	}
	cout<<"输入样本的真实标签:"<<endl; 
	for(int i=0;i<N;i++){
		for(int j=0;j<out_n;j++)
			cin>>output[i][j];
	}
	cout<<"输入学习率:";
	cin>>lr;
	cout<<endl<<"输入误差精度:";
	cin>>epsilon;
	cout<<endl<<"输入迭代次数:";
	cin>>NN;
	count=0;
	initialize();
	double s;
	s=update(lr,input,output);
	while(s>epsilon&&count<NN){
		count++;
		s=update(lr,input,output);
	}
	cout<<"迭代结束，输出预测值:"<<endl;
	/*for(int n=0;n<N;n++){
		cout<<"第"<<n<<"样本:"; 
		for(int j=0;j<out_n;j++)
			cout<<pred_out[n][j];
			cout<<endl;
	}*/
	for(int i=0;i<N;i++){
		for(int j=0;j<hid_n;j++){ 
			hid_in[j]=0;
			for(int k=0;k<in_n;k++)
				hid_in[j]+=w1[k][j]*input[i][k];
			hid_in[j]+=theta_h[j];
			hid_out[j]=1.0/(1.0+exp(-hid_in[j]));
		}
		for(int j=0;j<out_n;j++){
			output_in[j]=0;
			for(int k=0;k<hid_n;k++)
				output_in[j]+=w2[k][j]*hid_out[k];
			output_in[j]+=theta_o[j];
			output_out[j]=1.0/(1.0+exp(-output_in[j]));
			cout<<output_out[j]<<endl;
		} 
	}
	cout<<"隐藏层到输出层的权重:"<<endl; 
	for(int i=0;i<hid_n;i++){
		for(int j=0;j<out_n;j++)
			cout<<w2[i][j]<<endl;
	}
	return 0;
}


