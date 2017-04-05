#include <iostream>
#include "HostNet.h"
#include <string>
#include <fstream>

using namespace std;

void HostNet::dfsMark(int position) { 
	marks[position] = true;
	for (int i = 0; i<numOfHosts; i++) {
   		if(graph[position][i]==1&&marks[i]!=true){
   			dfsMark(i);
		}
	}
}//depth first traversel

void HostNet::dfsMarkSecond(int positionOne,int positionTwo,int &positionThree) { 
										  
	if(marks[positionOne]!=true){
		componentIds[positionTwo][positionThree]=positionOne;
		positionThree++;
		marks[positionOne]=true;
	}

	for (int i = 0; i<numOfHosts; i++) {
   		if(graph[positionOne][i]==1&&marks[i]!=true){
   			dfsMarkSecond(i,positionTwo,positionThree);
		}
	}	
}//depth first traversel for 
 //putting components into the 2d array

void HostNet::markFalse(){ 
	for(int i = 0; i<numOfHosts;i++){
		marks[i]=false;
	}
	allMarked=false;

}//marks the mark array elements false


HostNet::HostNet(const string hName){

	fstream in;
 	
	
	in.open(hName.c_str());
    
	if (in.is_open()) {
  

	
        in >> numOfHosts;
		names = new string[numOfHosts];
		graph = new int*[numOfHosts];
		components = new int[numOfHosts];
		marks = new bool[numOfHosts];

		for(int i = 0;i<numOfHosts;i++){
			graph[i]= new int[numOfHosts];
		}
		int tempNum,x,y;
	        while (in >> x) {
			in >> names[x];
			in >> y;
			for(int i = 0; i<y; i++){
				in>>tempNum;
				graph[x][tempNum]=1;
			}
		}
        //reads the file and initializes data members accordingly
		in.close();
    
	

		markFalse();
	
		numberOfComponents = 0;
		int index=0;
		while(!allMarked){
	
			dfsMark(index);
	
			bool isIt=false;
			for(int i = 0;i<numOfHosts;i++){
				isIt=true;
				if(marks[i]==false){
					isIt=false;
					index=i;
					break;
				}
			}
			
			if(isIt==true)
				allMarked=true;
			
			numberOfComponents++;
			for(int j = 0;j<numOfHosts;j++){
				if(marks[j]==true)
						components[numberOfComponents-1]++;
			}

			if(numberOfComponents>0)
				for(int k = 1;k<=numberOfComponents;k++)
					components[numberOfComponents-1]=components[numberOfComponents-1]-components[numberOfComponents-k-1];	
		
		}//finds the number of components and host numbers of each component and puts them into arrays
		
		componentIds = new int*[numberOfComponents];
		for(int z = 0;z<numberOfComponents;z++)
			componentIds[z]=new int[components[z]];
		for(int z = 0;z<numberOfComponents;z++)
			for(int l = 0; l<components[z];l++)
				componentIds[z][l]=-1;
		
		markFalse();

		index=0;
		int numberOfComp=0;
		int index2;
		
		while(!allMarked){
			index2=0;
			dfsMarkSecond(index,numberOfComp,index2);
	
			bool isIt=false;
			for(int i = 0;i<numOfHosts;i++){
				isIt=true;
				if(marks[i]==false){
					isIt=false;
					index=i;
					break;
				}
			}
			numberOfComp++;
			if(isIt==true)
				allMarked=true;
			
		}//finds the host ids in each seperate component and puts them into a 2d array
		
		markFalse();

		for(int i = 0;i<numOfHosts;i++) //fixes the adjacency matrix(makes it undirected)
			for(int j = 0; j<numOfHosts;j++)
				if(graph[i][j]==1)
					graph[j][i]=1;
		
	}
	else{
		names = NULL;
		graph = NULL;
		components = NULL;
		marks = NULL;
		numOfHosts=0;
	} //empty network if there is no file
}//constructor

void HostNet::listCommunicationsLowerThan(int id, const int cost){
	if(cost==0)
		return;
	else{
		if(marks[id]!=true){
			cout<<names[id]<<" ";
			marks[id] = true;
		}
		for (int i = 0; i<numOfHosts; i++) {
   			if(graph[id][i]==1&&marks[i]!=true)
   				listCommunicationsLowerThan(i,cost-1);
		}
	}
		
}//recursive private function that prints reachable hosts with given cost

void HostNet::listCommunicationsLowerThan(const string aname , const int cost){
	bool isThere=false;
	int i;
	for(i = 0;i<numOfHosts;i++){
		if(names[i]==aname){
			isThere=true;
			break;
		}
	}
	if(isThere==true){
		markFalse(); 
		marks[i]=true;
		int count;
		for(int j=0;j<numOfHosts;j++)
			if(graph[i][j]==1){
				count++;
			}
		markFalse();
		if(count!=0){
			cout<<"From \""<<aname<<"\" "<<count<<" hosts are directly reachable with cost lower than "<<cost<<": ";
			marks[i]=true; 
			listCommunicationsLowerThan(i,cost);
			cout<<endl;
			markFalse(); 
		}
		else
			cout<<"From \""<<aname<<" \" no hosts are reachable with cost lower than "<<cost<<"."<<endl;
			cout <<endl;
	}
	else{
		cout <<"\""<<aname<<"\""<<"does not exist in the host network."<<endl;
		cout <<endl; 
	}
}// public function that prints the reachable hosts with given cost


void HostNet::listCommunicationsOnHubHost(const string hname){
	bool isThere=false;
	int i;
	bool isThereSecond=false;
	for(i = 0;i<numOfHosts;i++){
		if(names[i]==hname){
			isThere=true;
			break;
		}
	}
	if(isThere){
		int *ids=new int[numOfHosts];
		int count=0;
		for(int j=0;j<numOfHosts;j++)
			if(graph[i][j]==1){
				ids[count]=j;
				count++;
			}//finds neighbours
		cout<<"If "<<hname<<" is considered as hub these routes are possible:"<<endl;
		for(int k = 0; k<count;k++){
			for(int j=k+1;j<count;j++){
				if(k!=j&&graph[ids[k]][ids[j]]!=1){
					cout<<"<"<<names[ids[k]]<<","<<names[ids[j]]<<">"<<endl;				
				}//checks if there is a connection between neighbours
			}
		
		}
		delete[] ids;	
		cout <<endl;
		
	}
	else
		cout <<"\""<<hname<<"\""<<"does not exist in the host network."<<endl;
}

void HostNet::findConnectedComponents(){
	if(numberOfComponents!=0){
		cout<<"There are "<<numberOfComponents<<" connected components:"<<endl;
		for(int i = 0; i<numberOfComponents;i++){
			cout<<"For component number"<<i+1<<": "<<components[i]<<" nodes"<<endl;
		}
	}
	else cout<<"The host is empty."<<endl;
	cout<<endl;
}//prints connected components and their host numbers

void HostNet::displayMostCentralHosts(){	
}

void HostNet::displayAverageClusteringCoefficients(){

	int *ids;
	int count=0,secondCount=0;
	double average=0;
		
	for(int i = 0;i<numberOfComponents;i++){
		average=0;
		for(int z=0;z<components[i];z++){
			count=0;
			secondCount=0;
			ids=new int[numOfHosts];
			for(int j=0;j<numOfHosts;j++)
				if(graph[componentIds[i][z]][j]==1){
					ids[count]=j;
					count++;
				}//finds number of neighbours
			for(int k = 0; k<count;k++){
				for(int j=0;j<count;j++){
					if(k!=j&&graph[ids[k]][ids[j]]==1){
						secondCount++;				
					}
				}
		
			}//finds the number of connection between neighbours multiplied by two
		//cout<<"count "<<count<<" secondCount "<<secondCount<<endl;
			if(count>=2){
				double x= secondCount,y=count;
				average+=(x)/(y*(y-1)); //calculates the average according to formula
			}
			else average+=0;
			delete[] ids;
		}
		double result = average/components[i];
		cout<<"Average clustering coefficient for component "<<i<<": "<< result<<endl; //prints the result
	}

}//prints clustering coefficiency
