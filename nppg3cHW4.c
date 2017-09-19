//I could not get the function to work properly, but I did get it to work when I would provide data preloaded into a 2D array instead of from an input file.
//Nick Pistolis
//11-12-16
#include <stdio.h>
#include <limits.h>
#include<stdbool.h> 
#include"input_error.h"
#define numVert 30 
int minimum(int key[], bool mstSet[]);
//int makeGraph(FILE* fp);
int printTree(int parent[],FILE* fp2, int graph[numVert][numVert]);
void primMST(int graph[numVert][numVert],FILE* fp2);
int main(int argc,char *argv[])
{
if(argc != 3)
{
	exit(1);
}

//FILE* fp=fopen(argv[1],"r");
//if(fp==NULL)
//{
//return 0;// fail failed to open

FILE*fp2=fopen(argv[2],"w");
if(fp2==NULL)
{
exit(2);
}

//int makeGraph(fp); call to function that does not work


	int graph[numVert][numVert];
  primMST(graph,fp2);
//fclose(fp);// closes file fp
fclose(fp2);
if(fp2!=NULL)
{
exit(3);
}
 
    return 0;

}
// could not get function to work without hard coding in a value for numVerts
/*int[][] makeGraph(FILE* fp)
{
	char str[60];
	fgets(str,60,fp);
	int size=atoi(str);
	int row;
	int col;
	int weight;
	#define V size
	int graph[V][V];
	while(fgets(str,60,fp)!=NULL)
	{
		fscanf(fp,"(%d,%d,%d)"row,col,weight);
		printf("%d %d", row,col,weight);
		graph[row][col]=weight;

	}
	return graph[][];
*/

int minimum(int Solution[], bool MinSpanTree[])// This function finds the next vert to look at
{
   
   int max = INT_MAX; 
   int minVert;
 	int count=0;
   for (count =0; count < numVert; count++)
	{
     if (MinSpanTree[count] == false && Solution[count] < max)
	{
         max = Solution[count];
	 minVert = count;
	}
	}
 
   return minVert;
}
 
int printTree(int parent[],FILE* fp2, int graph[numVert][numVert])//function writes to the file
{
	int i=1;
   for ( i = 1; i < numVert; i++)
	{
     fprintf(fp2,"(%d, %d) \n", parent[i], i);
	}
	return 0;
}		
 

void primMST(int graph[numVert][numVert],FILE* fp2)// function runs prim's allg. on the graph
{
     int parent[numVert]; 
     int Solution[numVert];   
     bool MinSpanTree[numVert];  
   	int count2;
	int count3;
     for ( count2 = 0; count2 < numVert; count2++)
        Solution[count2] = INT_MAX, MinSpanTree[count2] = false;
 
     
     Solution[0] = 0;         
parent[0] = -1; 
          for ( count3 = 0; count3 < numVert-1; count3++)
     {
        
        
        int tracker = minimum(Solution, MinSpanTree);
 
        
        MinSpanTree[tracker] = true;
 
        int count4;
        for ( count4 = 0; count4 < numVert; count4++)
 
           
          if (graph[tracker][count4] && MinSpanTree[count4] == false && graph[tracker][count4] <  Solution[count4])
		{
             parent[count4]  = tracker, Solution[count4] = graph[tracker][count4];
		}
     }
 
     printTree(parent,fp2, graph);
}

