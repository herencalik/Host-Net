#ifndef _HOSTNET_H
#define _HOSTNET_H


#include <string>
using namespace std;
class HostNet{
public:
HostNet(const string hName);
HostNet(const HostNet& hNet); // copy contructor
 void listCommunicationsLowerThan(const string aname, const int
cost);
 void listCommunicationsOnHubHost(const string hname);
 void findConnectedComponents();
 void displayMostCentralHosts();
 void displayAverageClusteringCoefficients();
// ...
// define other public member functions here,
// if you have any
private:
string *names;
int **graph;
int numOfHosts;
int *components;
int **componentIds;
int numberOfComponents;
bool *marks;
bool allMarked;
void listCommunicationsLowerThan(int id, const int cost);
int listCommunicationsLowerThanCount(int id, const int cost);
void dfsMarkSecond(int positionOne,int positionTwo,int &positionThree);	
void markFalse();
void dfsMark(int position);
};


#endif