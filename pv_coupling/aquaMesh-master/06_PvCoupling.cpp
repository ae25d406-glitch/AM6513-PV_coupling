//g++ -O2 06_PvCoupling.cpp src/*.cpp -Iinclude -std=gnu++17 -o cavity
// ./cavity case=single Re=100 N=64 algorithm=SIMPLE

#include <iostream>
#include <string>
#include <filesystem>
#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Mesh.h"
#include "CavityFlow.h"

using namespace std;

int main(int argc,char* argv[])
{
	// input
	FlowSettings set;
	string caseName="single",outDir="results",runName="";
	int N=64;

	for(int k=1;k<argc;k++)
	{
		string arg=argv[k];
		size_t eq=arg.find('=');
		if(eq==string::npos){cerr<<"Inputs must look like name=value: "<<arg<<endl;return 1;}
		string key=arg.substr(0,eq),val=arg.substr(eq+1);

		if(key=="case")caseName=val;
		else if(key=="Re")set.Re=stod(val);
		else if(key=="N")N=stoi(val);
		else if(key=="algorithm")set.algorithm=val;
		else if(key=="scheme")set.scheme=val;
		else if(key=="alphaU")set.alphaU=stod(val);
		else if(key=="alphaP")set.alphaP=stod(val);
		else if(key=="dt")set.dt=stod(val);
		else if(key=="nOuter")set.nOuter=stoi(val);
		else if(key=="nInner")set.nInner=stoi(val);
		else if(key=="pSolver")set.pSolver=val;
		else if(key=="tol")set.tol=stod(val);
		else if(key=="maxIter")set.maxIter=stoi(val);
		else if(key=="maxTime")set.maxTime=stod(val);
		else if(key=="print")set.printEvery=stoi(val);
		else if(key=="out")outDir=val;
		else if(key=="name")runName=val;
		else{cerr<<"Unknown input: "<<key<<endl;return 1;}
	}

	if(caseName=="single"){set.Utop=1.0;set.Ubottom=0.0;}
	else if(caseName=="parallel"){set.Utop=1.0;set.Ubottom=1.0;}
	else if(caseName=="antiparallel"){set.Utop=1.0;set.Ubottom=-1.0;}
	else{cerr<<"Unknown case: "<<caseName<<endl;return 1;}

	if(runName.empty())
		runName=caseName+"_Re"+to_string((int)set.Re)+"_N"+to_string(N)+"_"+set.algorithm;
	filesystem::create_directories(outDir);

	cout<<"Lid-driven cavity : "<<caseName<<", Re = "<<set.Re<<", "<<N<<" x "<<N<<", "<<set.algorithm<<", "<<set.scheme<<" scheme"<<endl;
   // geometry 
	Point p0(0,0.0,0.0),p1(1,1.0,0.0),p2(2,1.0,1.0),p3(3,0.0,1.0);
	Line bottom(0,&p0,&p1),right(1,&p1,&p2),top(2,&p2,&p3),left(3,&p3,&p0);

	Surface cavity(0);
	cavity.addBoundary(&bottom);
	cavity.addBoundary(&right);
	cavity.addBoundary(&top);
	cavity.addBoundary(&left);

	// cartesian mesh
	Mesh mesh;
	mesh.generateCartesian(cavity,N,N);
	
	//solve
	CavityFlow flow(mesh,set);
	flow.solve();

	cout<<(flow.converged?"Converged":"NOT converged")<<" after "<<flow.iterations<<(set.algorithm=="SIMPLE"?" iterations":" time steps")<<", CPU time "<<flow.cpuSeconds<<" s"<<endl;
	
	// write results
	flow.writeResults(mesh,outDir+"/"+runName);
	return 0;
}