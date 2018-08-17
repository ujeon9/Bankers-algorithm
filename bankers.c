#include <stdio.h>

typedef struct pcb {
	int pid;
	int allocation[3];
	int max[3];
	int need[3];
	int finish;
	struct pcb *next;
}PCB;

int compare(int[], int[]);

int main(){
	int temp, i = 0, j = 0, cnt = 0;
	int avail[3], avail2[3], running[5], request[3], rPid;

	PCB *head = NULL;
	PCB *cur = NULL;

	
		FILE *fp = fopen("scenario2.txt", "r");
		if (fp == NULL) {
			printf("Could not open file\n");
			exit(-1);
		}

		//read available
		for (i = 0; i < 3; i++) { 
			fscanf(fp, "%d", &avail[i]);
			avail2[i] = avail[i];
		}

		//read pid, allocation, max
		for (cnt = 0; cnt < 5; cnt++) {
			PCB *node = (PCB*)malloc(sizeof(PCB));
			fscanf(fp, "%d", &node->pid);
			fscanf(fp, "%d %d %d", &node->allocation[0], &node->allocation[1], &node->allocation[2]);
			fscanf(fp, "%d %d %d", &node->max[0], &node->max[1], &node->max[2]);

			if (head == NULL) {
				head = node;
				cur = node;
			}

			//link nodes
			else {
				cur->next = node;
				cur = node;
			}
			cur->finish = 0;
		}

		//read request and it's pid
		fscanf(fp, "%d", &rPid);
		for (i = 0; i < 3; i++) {
			fscanf(fp, "%d", &request[i]);
		}
	
		fclose(fp);
	
		cur->next = NULL;
		cur = head;

		//calculate need
		for (j = 0; j < 5; j++) {
			for (i = 0; i < 3; i++) {
				cur->need[i] = cur->max[i] - cur->allocation[i];
			}
			cur = cur->next;
		}
	
		cnt = 0;
		//find the safe sequence
		for (i = 0; i < 5; i++) {
			int curFinish = 0;
			temp = 0;
			cur = head;
			while ((cur != NULL) && (curFinish != 1)) {
				if (cur->finish == 0) {
					//if available > need, set temp '1'
					if (!compare(avail, cur->need))
						temp = 0;
					else {
						temp = 1;
					}
				}
				if (temp == 1) {
					cur->finish = 1; //if it terminated by available resource, set finish '1'
					curFinish = 1;
					running[cnt] = cur->pid;
					cnt++;

					//add allocation to available
					for (j = 0; j < 3; j++) 
						avail[j] += cur->allocation[j];
				}
				cur = cur->next;
			}
		}

		//if it has safe sequence, print
		if (cnt == 5) {
			printf("current state: ");
			for (i = 0; i < 4; i++) {
				printf("p%d->", running[i]);
			}
			printf("p%d\n", running[4]);
		}
		else {
			printf("unsafe!\n");
			exit(-1);
		}

		cur = head;
	
		while(cur!=NULL) {
			if (rPid == cur->pid) {
				for (j = 0; j < 3; j++) {
					//check request can allow
					if ((cur->max[j] >= request[j]) && (cur->need[j] >= request[j])) {
						avail2[j] -= request[j];		
						cur->allocation[j] += request[j];
						cur->need[j] -= request[j];
					}
					else {
						printf("After arriving request: unsafe!\n");
						exit(-1);
					}
				}
			}
			cur = cur->next;
		}
		cur = head;
		
		for (i = 0; i < 5; i++) {
			cur->finish = 0;
			cur = cur->next;
		}
		cur = head;
		cnt = 0;
		//find the safe sequence
		for (i = 0; i < 5; i++) {
			int curFinish = 0;
			temp = 0;
			cur = head;
			while ((cur != NULL) && (curFinish != 1)) {
				if (cur->finish == 0) {
					//if available > need, set temp '1'
					if (!compare(avail2, cur->need))
						temp = 0;
					else {
						temp = 1;
					}
				}
				if (temp == 1) {
					cur->finish = 1; //if it terminated by available resource, set finish '1'
					curFinish = 1;
					running[cnt] = cur->pid;
					cnt++;

					//add allocation to available
					for (j = 0; j < 3; j++)
						avail2[j] += cur->allocation[j];
				}
				cur = cur->next;
			}
		}
	
		if (cnt == 5) {
			printf("After arriving request: ");
			for (i = 0; i < 4; i++) {
				printf("p%d->", running[i]);
			}
			printf("p%d\n", running[4]);
		}
		else {
			printf("After arriving request: unsafe!\n");
			exit(-1);
		}
	}


/*
Description: compare available and need
Input:  Array of available and need
output: return 1 if all availabe are larger than need. else retun 0
*/
int compare(int avail[], int need[]){
	int cnt = 0;
	for (int i = 0; i < 3; i++) {
		if (avail[i] >= need[i]) 			
			cnt++;
	}
	if (cnt == 3) {
		return 1;
	}
	else
		return 0;
}

