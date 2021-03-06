#include<iostream>
#include<iomanip>
#include<fstream>
#include<math.h>
#include<assert.h>
#include<chrono>
#include<random>
#include<vector>
#include<limits>
using namespace std;

bool feq(double a,double b){
	return fabs(a-b)<std::numeric_limits<double>::epsilon();
}
void printArr(double *arr,int size){
	for(int i=0;i<size;i++){
		cout<<arr[i]<<"\t";
	}
}
void addArr(double *a,double *b,int size){
	//a+=b;
	for(int i=0;i<size;i++){
		a[i]+=b[i];
	}
}
void multiplyArr(double *a,int size,double scale){
	//a*=scale.
	for(int i=0;i<size;i++){
		a[i]*=scale;
	}
}
int getBestIndex(double *a,int size){
	int index=0;
	for(int i=1;i<size;i++){
		if(a[i]>a[index]){
			index=i;
		}
	}
	return index;
}
class NormalRand{
	private:
		unsigned lastSeed;
		default_random_engine generator;
		normal_distribution<double>distribution;
	public:
		NormalRand(double mean=0,double std=1.0):distribution(mean,std){
		}
		void seed(unsigned s=0){
			if(s==0){
				s=chrono::system_clock::now().time_since_epoch().count();
			}
			generator.seed(s);
			lastSeed=s;
		}
		void reset(){
			//used to replay the same sequence.
			//reset to last seed.
			generator.seed(lastSeed);
			distribution.reset();
		}
		double next(){
			return distribution(generator);
		}
};
class UniRand{
	private:
		unsigned lastSeed;
		default_random_engine generator;
		uniform_real_distribution<double>distribution;
	public:
		UniRand(double min=0.0,double max=1.0):distribution(min,max){
			//~[min,max)
		}
		void seed(unsigned s=0){
			if(s==0){
				s=chrono::system_clock::now().time_since_epoch().count();
			}
			generator.seed(s);
			lastSeed=s;
		}
		void reset(){
			//used to replay the same sequence.
			//reset to last seed.
			generator.seed(lastSeed);
			distribution.reset();
		}
		double next(){
			return distribution(generator);
		}
};

class UniDirectionRand{
	int numD;//number of dimension.
	vector<NormalRand>r;//Notice::(A common mistake).To get a random direction, each dimension should be generated by Normal Distribution.
	//If generated by Uniform Distribution, the direction is not random in every direction.
	static double getLength(vector<double>&a){
		double res=0;
		for(int i=0;i<a.size();i++){
			res+=a[i]*a[i];
		}
		return sqrt(res);
	}
	static void normalize(vector<double>&a){
		double l=getLength(a);
		if(feq(l,0.0))return ;
		for(int i=0;i<a.size();i++){
			a[i]/=l;
		}
	}
	public:
		UniDirectionRand(int numDimension=1):numD(numDimension),r(numD){
			for(int i=0;i<numD;i++){
				r[i].seed();
			}
		}
		void setD(int d){
			numD=d;
			r.resize(numD);
			for(int i=0;i<numD;i++){
				r[i].seed();
			}
		}
		vector<double>next(){
			vector<double>a(numD);
			for(int i=0;i<numD;i++){
				a[i]=r[i].next();
			}
			normalize(a);
			return a;
		}
};
class MPB{
#define NumPeak 5
#define NumDim 5
	public:
		double W[NumPeak],H[NumPeak];
		double X[NumPeak][NumDim];
		int numP,numD;
		NormalRand Wtheta[NumPeak],Htheta[NumPeak];
		UniDirectionRand Vtheta[NumPeak];
		MPB(int numd=5,int nump=5){
			//currently must:numD<=5,numP<=5
			numP=nump;
			numD=numd;
			init();
		}
		void init(){
			//NumPeak*(NumDim+2)
			double tmp[NumPeak][NumDim+2]={{8.0,64.0,67.0,55.0,4.0,0.1,50.0},
				{50.0,13.0,76.0,15.0,7.0,0.1,50.0},
				{9.0,19.0,27.0,67.0,24.0,0.1,50.0},
				{66.0,87.0,65.0,19.0,43.0,0.1,50.0},
				{76.0,32.0,43.0,54.0,65.0,0.1,50.0},
			};//data is from the paper(MPB)
			for(int i=0;i<NumPeak;i++){
				W[i]=tmp[i][NumDim];
				H[i]=tmp[i][NumDim+1];
				for(int j=0;j<NumDim;j++){
					X[i][j]=tmp[i][j];
				}
			}
			//seed the rands:
			for(int i=0;i<NumPeak;i++){
				Htheta[i].seed();
				Wtheta[i].seed();
				Vtheta[i].setD(numD);
			}
		}
		double evaluate(double *xs,int size){
			/* Let peak[i]=X[i][];
			 * Fmax=max(H[i]), with X*=peak[arg_max_i{H[i]}];
			 * i.e. F gets its optima H[i], when x is the X[i][:], with H[i] is the max of H[:].
			 *
			 * */
			assert(size==numD);
			double res=-std::numeric_limits<double>::infinity();
			double tmp=0;
			for(int i=0;i<numP;i++){
				double tmp1=0;
				for(int j=0;j<numD;j++){
					tmp1+=pow(xs[j]-X[i][j],2);
				}
				tmp=(H[i]/(1.0+W[i]*tmp1));
				if(tmp>res){
					res=tmp;
				}
			}
			return res;

		}
		void change(){
			for(int i=0;i<numP;i++){
				H[i]+=7.0*Htheta[i].next();
				W[i]+=0.01*Wtheta[i].next();
				vector<double>v=Vtheta[i].next();
				const double s=0.9;
				multiplyArr(&v[0],v.size(),s);
				addArr(X[i],&v[0],numD);
			}
		}
		double *getPeak(int i){
			return X[i];
		}
		double *getOptima(){
			int bi=getBestIndex(H,numP);
			return X[bi];
		}
		double *getH(){
			return H;
		}
		double *getW(){
			return W;
		}
};

int main(){
	//setup MPB, 2-dimension, 5-peaks.
	MPB p(2,5);
	//output five peaks positions
	for(int i=0;i<5;i++){
		printArr(p.getPeak(i),2);cout<<endl;
	}
	//output the position of the maximum, following by the H[:], after each change.
	for(int i=0;i<300;i++){
		printArr(p.getOptima(),2);printArr(p.getH(),5);cout<<endl;
		p.change();
	}
	return 0;
}

