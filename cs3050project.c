
#include "input_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

//Tyler James, Nick Pistolis
//tljwvf, nppg3c
//12/8/2016
//cs3050

#define VECTOR 2


typedef struct Entity {

	int speed;
	int direction[VECTOR];
	int location[VECTOR];
	bool moved;


}Entity;

typedef struct Robot {

	int location[VECTOR];
	int finish[VECTOR];

}Robot;

typedef struct Graph {

	enum {EMPTY, ENTITY, ROBOT} item;

	int size;
	struct Robot *robot;
	struct Entity *entity;
}Graph;

// void errorFlag();
void buildGraph(FILE*, long);
int *itemFinder(Graph* graph[]);
// int* printPath(Graph* graph[], bool finish, int* robotPath);
void moveRobot(Graph* graph[]);
void moveEntity(Graph* graph[]);
int *robotFinder(Graph* graph[]);

// bool printClarity(int weight, int duplication[], int graphLength);

int main (int argc, char* argv[]) 
{

//----------------------------command line arguments check----------------------//
	if (argc != 2) {
		printf("Error: Invalid Command Line Arguments.\n");
		printf("Requires 2, Amount Given: %d\n", argc);
		return 0;
	}

	FILE* file = fopen(argv[1], "r");
	char* buffer = (char*)malloc(sizeof(char)*1024);

//---------------------------file check-------------------------------//
	if (file == NULL) {
		exit(FILE_FAILED_TO_OPEN);
		return 0;
	}


//---------------------obtaints the first line of the file and passes the rest to the matrix builder------------------//
	char* ptr;
	long graphLength = strtol(fgets (buffer, 1024, file), &ptr, 10);
	
	//printf("Graph Size: %lu\n", graphLength);

	buildGraph(file, graphLength);
	
	//attempt to fix memory leak
	free(buffer);
	fclose(file);

	return 0;
}

void buildGraph(FILE* file, long graphLength) {

	char* buffer = (char*)malloc(sizeof(size_t));
	
	char* ptr; //again only used for strtol
	char* charlocationX;
	char* charlocationY;
	//int speed = 0;
	int i = 0, j = 0, counter = 0;
	Robot *robot = (Robot*)malloc(sizeof(Robot));
	Entity *entity = (Entity*)malloc(sizeof(Entity)); 

	Graph *graph[graphLength];
    for (i=0; i <= graphLength; i++) {
         graph[i] = (Graph*)malloc(graphLength * sizeof(Graph));
    }


//--------------------------------initialization of data structs---------------------------------------//

    // printf("HERE\n\n");
    robot->location[0] = -1;
    robot->location[1] = -1;
    robot->finish[0] = -1;
    robot->finish[1] = -1;

// printf("HERE\n\n");
    entity->speed = -1;
    entity->direction[0] = -1;
    entity->direction[1] = -1;
    entity->location[0] = -1;
    entity->location[1] = -1;
    entity->moved = false;

	// printf("\nGraph  	locationY     Weight\n");

	//initilizes the array to negative locationXs 
	// printf("HERE\n\n");
	for (i = 0; i <= graphLength; i++) {
		for(j = 0; j <= graphLength; j++) {
			graph[i][j].size = graphLength;
			graph[i][j].robot = NULL;
			graph[i][j].entity = NULL;
		}
	}

	//I reuse this as a counter
	i = 0;
	j = 0;

	// printf("HERE\n\n");


//------------------------------Obtains the robot and its data---------------------------------//

	while (fgets (buffer, 1024, file) != NULL) {
		counter++;
		// gets ride of the initial garbage '('
			charlocationX = strtok(buffer, "(");

			// printf("CharlocationX is: %s\n", charlocationX);

			charlocationX = (char*)strtok(charlocationX, ",");
			//printf("%s ->  ", charlocationX);

			// printf("CharlocationX is: %s \n", charlocationX);
			charlocationY = (char*)strtok(NULL, ")");

			//printf("    %s\n", charWeight);
			// printf("CharlocationX is: %s \n", charlocationX);

			int locationX = strtol(charlocationX, &ptr, 10);
			int locationY = strtol(charlocationY, &ptr, 10);


			//printf("locationX: %d\n", locationX);
			//printf("locationY: %d\n", locationY);

			if (counter == 1) {
				robot->location[0] = locationX;
				robot->location[1] = locationY;
			}
			else if (counter == 2) {
				robot->finish[0] = locationX;
				robot->finish[1] = locationY;
			}
			


		if (counter == 2) {
			break;
		}
	}

	// printf("Robot Location: (%d, %d)", robot->location[0], robot->location[1]);
	// printf("Robot Finish: (%d, %d)", robot->finish[0], robot->finish[1]);

	graph[robot->location[0]][robot->location[1]].item = 2;
	//printf("Item Value: %d\n", graph[robot->location[0]][robot->location[1]].item);

	graph[robot->location[0]][robot->location[1]].robot = robot;
	//printf("Is robot Valid: (%d, %d)\n", graph[robot->location[0]][robot->location[1]].robot->location[0], graph[robot->location[0]][robot->location[1]].robot->location[1]);


//------------------------------------------Just Variables------------------------------//

	counter = 0;
	int entityLocationX = 0;
	int entityLocationY = 0;
	int entityDirectionX = -10;
	int entityDirectionY = -10;
	int speed = -10;
	bool firstRun = false;
	Entity *newEntity = NULL;

	while (fgets (buffer, 1024, file) != NULL) {

		++counter;


//------------------------------------This is where the data is obtained -------------------------//

		if (counter == 1 || counter == 4) {

			charlocationX = strtok(buffer, "(");
			// printf("CharlocationX is: %s\n", charlocationX);

			charlocationX = (char*)strtok(charlocationX, ",");
			//printf("%s ->  ", charlocationX);

			// printf("CharlocationX is: %s \n", charlocationX);

			charlocationY = (char*)strtok(NULL, ")");
			// printf("CharlocationX is: %s \n", charlocationX);

			entityLocationX = strtol(charlocationX, &ptr, 10);
			entityLocationY = strtol(charlocationY, &ptr, 10);

			//printf("Entity's Location: %d, %d\n", entityLocationX, entityLocationY);

		}
		else if (counter == 2) {
			speed = strtol(buffer, &ptr, 10);
			
		}
		else if (counter == 3) {

			charlocationX = strtok(buffer, "(");
			// printf("CharlocationX is: %s\n", charlocationX);

			charlocationX = (char*)strtok(charlocationX, ",");
			//printf("%s ->  ", charlocationX);

			// printf("CharlocationX is: %s \n", charlocationX);

			charlocationY = (char*)strtok(NULL, ")");
			// printf("CharlocationX is: %s \n", charlocationX);

			entityDirectionX = strtol(charlocationX, &ptr, 10);
			entityDirectionY = strtol(charlocationY, &ptr, 10);

			//printf("Entity's Direction: %d, %d\n", entityDirectionX, entityDirectionY);

		}

//--------------------------------This is where entitys obtain their data and get assigned to the graph ------------------------//

		if (counter == 3) {

			//printf("here");
			//printf("EntityDirection Check: %d, %d\n", entityDirectionX, entityDirectionY);

			if (firstRun == false) {
				entity->speed = speed;
				entity->location[0] = entityLocationX;
				entity->location[1] = entityLocationY;
				entity->direction[0] = entityDirectionX;
				entity->direction[1] = entityDirectionY;

				//printf("%d, %d\n", entity->location[0], entity->location[1]);

				graph[entityLocationX][entityLocationY].item = 1;
				graph[entityLocationX][entityLocationY].entity = entity;

				firstRun = true;
			}
			else {

			//-------------------------------Creatation of N enities------------------------------------------//
				if (newEntity == NULL) {

					newEntity = (Entity*)malloc(sizeof(Entity));

					newEntity->speed = -1;
				    newEntity->direction[0] = -1;
				    newEntity->direction[1] = -1;
				    newEntity->location[0] = -1;
				    newEntity->location[1] = -1;
				    newEntity->moved = false;
				}

					newEntity->speed = speed;
					newEntity->location[0] = entityLocationX;
					newEntity->location[1] = entityLocationY;
					newEntity->direction[0] = entityDirectionX;
					newEntity->direction[1] = entityDirectionY;

					//printf("%d, %d\n", entity->location[0], entity->location[1]);

					if (graph[entityLocationX][entityLocationY].item == 1) {
						printf("Entity already exists at: x = %d, y = %d\nThus ignored!", entityLocationX, entityLocationY);
						free(newEntity);
					}
					else {
						graph[entityLocationX][entityLocationY].item = 1;
						graph[entityLocationX][entityLocationY].entity = newEntity;
					}
					
				
				
			}	


			// printf("Graph.Entity\n\n");
			// printf("Item ID: %d\n", graph[entityLocationX][entityLocationY].item);
			// printf("Speed: %d\n",graph[entityLocationX][entityLocationY].entity->speed);
			// printf("Location: %d, %d\n", graph[entityLocationX][entityLocationY].entity->location[0], graph[entityLocationX][entityLocationY].entity->location[1]);
			// printf("Direction: %d, %d\n", graph[entityLocationX][entityLocationY].entity->direction[0], graph[entityLocationX][entityLocationY].entity->direction[1]);
			
			//printf("Location: (%d, %d)", entityLocationX, entityLocationY);
			//printf("The Entity is at: (%d, %d)\n", graph[entity->location[0]][entity->location[1]].entity->location[0], graph[entity->location[0]][entity->location[1]].entity->location[1]);
			//printf("The Entity's speed is: %d\n", graph[entity->location[0]][entity->location[1]].entity->speed);	

		    counter = 0;
		}	

	}

//just here to see where entities/ robots are
	// for (i = 0; i <= 10; i++) {
	// 	// printf("[%d, %d]: ", i, j);
	// 	for (j = 0; j <= 10; j++) {
	// 		//if (graph[i][j].item == 1 || graph[i][j].item == 2)
	// 			printf("%d|", graph[i][j].item);
	// 	}
	// 	printf("\n");
	// 	printf("---------------------------");
	// 	printf("\n");	 
	// }




//--------------------------------SHIT HAPPENS HERE----------------------------------------//



	int *itemLocation = itemFinder(graph);

	//printf("(%d, %d)\n", graph[*(itemLocation)][*(itemLocation+1)].entity->location[0], graph[*(itemLocation)][*(itemLocation+1)].entity->location[1]);

	// bool finish = true;
	// int* robotPath = NULL;
	// robotPath = printPath(graph, finish , robotPath);

	//int loopBreak = 0;
	int *robotLocation = NULL;
	int robotX = 0;
	int robotY = 0;

	robotLocation = robotFinder(graph);

	printf("Robot Initial Location: <%d, %d>\n", robotLocation[0], robotLocation[1]);

	while (1) {
		//loopBreak++;

		if (graph[robotX][robotY].robot != NULL) {
			if (graph[robotX][robotY].robot->location[0] == graph[robotX][robotY].robot->finish[0]) {
				if (graph[robotX][robotY].robot->location[1] == graph[robotX][robotY].robot->finish[1]) {
					printf("Robot Achieved the Final Destination!\n");
					break;
				}
			}
		}
		//printf("LoopBreaker: %d ->  ", loopBreak);

		robotLocation = robotFinder(graph);

		//printf("robotLocation: %d, %d ", robotLocation[0], robotLocation[1]);

		robotX = robotLocation[0];
		robotY = robotLocation[1];

		//printf("LoopBreaker: %d\n", loopBreak);

		//moveEntity(graph);
		
		moveRobot(graph);


		
		
		// printf("OH my\n\n");
		// if (graph[robotX][robotY].robot->location[0] == graph[robotX][robotY].robot->finish[0] && graph[robotX][robotY].robot->location[1] == graph[robotX][robotY].robot->finish[1]) {
		// 	printf("\n-------------Robot has Arrived!--------------\n");
		// 	break;
		// }

		//printf("Oh noes");

		//free(robotLocation);

	}
	

	free(buffer);
	free(itemLocation);
	// free(robot);

	// for (i = 0; i <= 10; i++) {
	// 	// printf("[%d, %d]: ", i, j);
	// 	for (j = 0; j <= 10; j++) {
			
	// 		//printf("%d ", graph[i][j].item);
	// 		if (graph[i][j].item == 1) {
	// 			graph[i][j].item = 0;
	// 			free(graph[i][j].entity);
	// 			//break;
	// 		}
	// 		if (graph[i][j].item == 2) {
	// 			graph[i][j].item = 0;
	// 			free(graph[i][j].robot);
	// 		}					
			
	// 	}
	// }

	//free(graph);

}

int *robotFinder(Graph* graph[]) {

	int i = 0, j = 0;
	int* robotLocation = malloc(sizeof(int) * 2);

	for (i = 0; i <= 10; i++) {
		// printf("[%d, %d]: ", i, j);
		for (j = 0; j <= 10; j++) {
			
			//printf("%d ", graph[i][j].item);
			if (graph[i][j].item == 2) {
				break;
			}
			
			
		}
		
		if (graph[i][j].item == 2) {
			break;
		}
			
		
		//printf("\n");
	}

	*(robotLocation) = i;
	*(robotLocation + 1) = j;

	if (i == 11 && j == 11) {

		*(robotLocation) = -1;
		*(robotLocation + 1) = -1;
	}
	//printf("x: %d, y: %d\n", i, j);

	return robotLocation;

}





int *itemFinder(Graph* graph[]) {

	int i = 0, j = 0;
	int* itemLocation = malloc(sizeof(int) * 2);

	for (i = 0; i <= 10; i++) {
		// printf("[%d, %d]: ", i, j);
		for (j = 0; j <= 10; j++) {
			if (graph[i][j].item == 1) {
				//printf("%d ", graph[i][j].item);
				if (graph[i][j].entity->moved == false) {
					break;
				}
			}
			
		}
		if (graph[i][j].item == 1) {
			if (graph[i][j].entity->moved == false) {
				break;
			}
			
		} 
		//printf("\n");
	}

	itemLocation[0] = i;
	itemLocation[1] = j;

	if (i == 11 && j == 11) {

		itemLocation[0] = -1;
		itemLocation[1] = -1;
	}
	//printf("x: %d, y: %d\n", i, j);

	return itemLocation;

}

void moveRobot(Graph* graph[])	{
	
	int i=0; 
	int j=0;
	int x=0;
	int y=0;
	Robot* temp = NULL;
	//printf("here1\n");

	for (i = 0; i <= graph[0][0].size; i++) {
		for (j = 0; j <= graph[0][0].size; j++) {
			if (graph[i][j].item == 1) {
				graph[i][j].entity->moved = false;
			}
		}
	}


	for(i=0;i<11;i++)
	{
		for(j=0;j<11;j++)
		{
		if(graph[i][j].item==2)
		break;
		}
		if(graph[i][j].item==2)
		break;
	}
	//printf("here2\n");
	//printf("Robot Location: %d %d\n",i,j);
	if(graph[i][j].robot->location[0]<graph[i][j].robot->finish[0])
	{
		x=i+1;
		//printf("here3\n");

	}
	else if(graph[i][j].robot->location[0]>graph[i][j].robot->finish[0])
	{
		x=i-1;
		//printf("here4\n");
	}
	else
		x=i;
	
	if(graph[i][j].robot->location[1]<graph[i][j].robot->finish[1])
    {
		y=j+1;
		//printf("here5\n");

    }
    else if(graph[i][j].robot->location[1]>graph[i][j].robot->finish[1])
    {
		y=j-1;
		//printf("here6\n");
	}
	else
		y=j;
	//printf("%d %d\n",x,y);

	if (i > 11 || j > 10 || i < 0 || j < 0) {
		printf("Error Robot Moved Out of Bounds");
		return;
	}

	int locationX=graph[i][j].robot->location[0];
    int locationY=graph[i][j].robot->location[1];
    int finishX=graph[i][j].robot->finish[0];
    int finishY=graph[i][j].robot->finish[1]; 

   // printf("here7\n");

	if(graph[x][y].item==0)
	{
		//printf("here8\n");
		
		temp=graph[i][j].robot;
		graph[x][y].robot=temp;
		graph[i][j].robot=NULL;
		graph[x][y].item=2;
		graph[x][y].robot->location[0]=x;
		graph[x][y].robot->location[1]=y;
		graph[i][j].item=0;
		printf("Robot Moved Here: <%d %d>\n",graph[x][y].robot->location[0],graph[x][y].robot->location[1]);
	}
	else if(abs(locationX-finishX)%2==1 && abs(locationY-finishY)%2==1)
	{
		printf("Position Hold\n");
		//Entity. move reset	
	}
	else if(abs(locationX-finishX)%2 ==1)
	{
		//printf("here9\n");
		printf("%d, %d\n", x, j);
		if(graph[x][j].item==0)
        	{
        		//printf("Here10 X\n");
                	graph[i][j].item=0;
                	Robot* temp=graph[i][j].robot;
                	graph[x][j].robot=temp;
                	graph[i][j].robot=NULL;
                	graph[x][j].item=2;
                	graph[x][j].robot->location[0]=i+1;
                	graph[x][j].robot->location[1]=j;
                	//printf("%d %d\n",graph[x][j].robot->location[0],graph[x][j].robot->location[1]);
        	}
	}
	else if(abs(locationY-finishY)%2 == 1)
        {
        	//printf("here11\n");
        	if(graph[i][y].item==0)
        	{
        		//printf("Here12\n");
                	graph[i][j].item=0;
                	Robot* temp=graph[i][j].robot;
                	graph[i][y].robot=temp;
                	graph[i][j].robot=NULL;
                	graph[i][y].item=2;
                	graph[i][y].robot->location[0]=i+1;
                	graph[i][y].robot->location[1]=j;
                	//printf("%d %d\n",graph[i][y].robot->location[0],graph[i][y].robot->location[1]);
        	}
	}
	
	// entity move reset
	//printf("%d %d",i,j);
}

void moveEntity(Graph* graph[]) {
	
	int newX = -1, newY = -1, directionX = 0, directionY = 0, newLocationX = -1, newLocationY = -1;
	Entity* temp = NULL;

	int *entityLocation = itemFinder(graph);

	while (entityLocation != NULL) {

		entityLocation = itemFinder(graph);

		newX = entityLocation[0];
		newY = entityLocation[1];

		if (newX == -1 && newY == -1) {
			break;
		}

		//printf("Entity: %d, %d\n", newX, newY);

		if (graph[newX][newY].entity != NULL) {

			graph[newX][newY].entity->moved = true;
			int speed = graph[newX][newY].entity->speed;

				//printf("X: %d\nY: %d\n\n", newX, newY);

				directionX = graph[newX][newY].entity->direction[0];
				directionY = graph[newX][newY].entity->direction[1];
				newLocationX = newX;
				newLocationY = newY;

				if (directionX < 0 && directionY < 0) {

					newLocationX = newX - (directionX * speed);
					newLocationY = newY - (directionY * speed);

					if (newLocationX > 10 || newLocationX < 0) {

						graph[newX][newY].entity->direction[0] = graph[newX][newY].entity->direction[0] * -1;
						//printf("New Entity Location: %d\n", graph[newX][newY].entity->direction[0]);
						//graph[newX][newY].entity->direction[0] += graph[newX][newY].entity->direction[0] * -1

					}
					else if (newLocationY > 10 || newLocationY < 0) {
						graph[newX][newY].entity->direction[1] = graph[newX][newY].entity->direction[1] * -1;
						//printf("New Entity Location: %d\n", graph[newX][newY].entity->direction[1]);
					}


				}
				else if (directionX > 0 && directionY < 0) {

					newLocationX = newX + (directionX * speed);
					newLocationY = newY - (directionY * speed);

					if (newLocationX > 10 || newLocationX < 0) {

						graph[newX][newY].entity->direction[0] = graph[newX][newY].entity->direction[0] * -1;
						//graph[newX][newY].entity->direction[0] += graph[newX][newY].entity->direction[0] * -1

					}
					else if (newLocationY > 10 || newLocationY < 0) {
						//printf("New Entity Location: %d\n", graph[newX][newY].entity->direction[1]);
						graph[newX][newY].entity->direction[1] = graph[newX][newY].entity->direction[1] * -1;
					}
				}
				else if (directionX < 0 && directionY > 0) {
					
					newLocationX = newX - (directionX * speed);
					newLocationY = newY + (directionY * speed);

					if (newLocationX > 10 || newLocationX < 0) {

						graph[newX][newY].entity->direction[0] = graph[newX][newY].entity->direction[0] * -1;
						//graph[newX][newY].entity->direction[0] += graph[newX][newY].entity->direction[0] * -1

					}
					else if (newLocationY > 10 || newLocationY < 0) {
						//printf("New Entity Location: %d\n", graph[newX][newY].entity->direction[1]);
						graph[newX][newY].entity->direction[1] = graph[newX][newY].entity->direction[1] * -1;
					}
				}
				else {

					newLocationX = newX + (directionX * speed);
					newLocationY = newY + (directionY * speed);

					if (newLocationX > 10 || newLocationX < 0) {

						graph[newX][newY].entity->direction[0] = graph[newX][newY].entity->direction[0] * -1;
						//graph[newX][newY].entity->direction[0] += graph[newX][newY].entity->direction[0] * -1

					}
					else if (newLocationY > 10 || newLocationY < 0) {
						//printf("New Entity Location: %d\n", graph[newX][newY].entity->direction[1]);
						graph[newX][newY].entity->direction[1] = graph[newX][newY].entity->direction[1] * -1;
					}
				}

				// graph[newX][newY].entity->location[0] += graph[newX][newY].entity->direction[0];
				// graph[newX][newY].entity->location[1] += graph[newX][newY].entity->direction[1];
				//printf("New Location XXXXXX: %d, %d\n", graph[newX][newY].entity->location[0], graph[newX][newY].entity->location[1]);

			
				temp = graph[newX][newY].entity;
				graph[newLocationX][newLocationY].item = 1;
				graph[newLocationX][newLocationY].entity = temp;
				graph[newX][newY].entity = NULL;
				graph[newX][newY].item = 0;

				graph[newLocationX][newLocationY].entity->location[0] = newLocationX;
				graph[newLocationX][newLocationY].entity->location[1] = newLocationY;


		}


	}

}