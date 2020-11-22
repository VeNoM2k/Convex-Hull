//Ashish Gour
//18CS30008
//Assignment 2

#include<stdio.h>
#include <stdlib.h>
#include<math.h>

#define pi 3.141592653589793

typedef struct CENTER {
	double x;
	double y;
} center;

void merge(center* arr, int l, int m, int r) {  
    int i, j, k;  
    int n1 = m - l + 1;  
    int n2 = r - m;
    center L[n1];
    center R[n2];
    for (i = 0; i < n1; i++)  
        L[i] = arr[l + i];  
    for (j = 0; j < n2; j++)  
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {  
        if ((L[i].x) <= (R[j].x)) {  
            arr[k] = L[i];  
            i++;  
        }  
        else {  
            arr[k] = R[j];  
            j++;  
        }  
        k++;  
    }
    while (i < n1) {  
        arr[k] = L[i];  
        i++;  
        k++;  
    }
    while (j < n2) {  
        arr[k] = R[j];  
        j++;  
        k++;  
    }  
}  

void mergeSort(center* arr, int l, int r) {  
    if (l < r) {
        int m = l + (r - l) / 2;  
        mergeSort(arr, l, m);  
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);  
    }  
}
//graham scan for convex hull
int CH(center* arr, int st[], int size, int flag) {
	int top = 0;
	//for upper hull
	if (flag == 0) {
		st[top] = 0;
		int i=1;
		while(i<size) {
			if(top<1) {
				//push
				top++;
				st[top] = i;
			}
			else {
				center a = arr[st[top-1]];
				center b = arr[st[top]];
				center c = arr[i];
				if((a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y)) <= 0) {
					//push
					top++;
					st[top] = i;
				}
				else {
					//pop
					top--;
					i--;			//since we again check turn using same next vertex but changed previous and prev previous vertex
				}
			}
			i++;
		}
	}
	//for lower hull
	else if (flag == 1) {
		st[top] = size-1;
		int i=size-2;
		while(i>=0) {
			if(top<1) {
				//push
				top++;
				st[top] = i;
			}
			else {
				center a = arr[st[top-1]];
				center b = arr[st[top]];
				center c = arr[i];
				if((a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y)) <= 0) {
					//push
					top++;
					st[top] = i;
				}
				else {
					//pop
					top--;
					i++;			//since we again check turn using same next vertex but changed previous and prev previous vertex
				}
			}
			i--;
		}
	}
	return top+1;
}

void contzone(center* S, int UH[], int u, int LH[], int l, double r, double T[][4],double A[][4]) {
	//for upper hull
	A[0][0] = S[UH[0]].x;
	A[0][1] = S[UH[0]].y;
	A[0][2] = pi;
	int i;
	for(i=1;i<u;i++) {
		//calculating theta
		center a = S[UH[i-1]];
		center b = S[UH[i]];
		double slope = (a.y-b.y)/(a.x-b.x);
		double theta = atan(slope);
		theta = theta + (pi/2);
		//arc
		A[i-1][3] = theta;
		A[i][0] = S[UH[i]].x;
		A[i][1] = S[UH[i]].y;
		A[i][2] = theta;
		//tangent
		T[i-1][0] = A[i-1][0] + r*cos(theta);
		T[i-1][1] = A[i-1][1] + r*sin(theta);
		T[i-1][2] = A[i][0] + r*cos(theta);
		T[i-1][3] = A[i][1] + r*sin(theta);
	}
	A[i-1][3] = 0;
	
	//for lower hull
	A[u][0] = S[LH[0]].x;
	A[u][1] = S[LH[0]].y;
	A[u][2] = 0;
	int j;
	for(i=1;i<l;i++) {
		j = u+i;		//so insert arc and tangent in same array as used for upper hull's arc and tangents
		//calculating theta
		center a = S[LH[i-1]];
		center b = S[LH[i]];
		double slope = (a.y-b.y)/(a.x-b.x);
		double theta = atan(slope);
		theta = theta - (pi/2);
		//arc
		A[j-1][3] = theta;
		A[j][0] = S[LH[i]].x;
		A[j][1] = S[LH[i]].y;
		A[j][2] = theta;
		//tangent
		T[j-2][0] = A[j-1][0] + r*cos(theta);
		T[j-2][1] = A[j-1][1] + r*sin(theta);
		T[j-2][2] = A[j][0] + r*cos(theta);
		T[j-2][3] = A[j][1] + r*sin(theta);
	}
	A[u+l-1][3] = -1*pi;
}

void printcontzone(int u, int l, double T[][4], double A[][4]) {
	printf("The containment zone\n");
	printf("Upper section\n");
	int i;
	for(i=0;i<u-1;i++) {
		printf("Arc     : (%1.15lf,%1.15lf) From %1.15lf to %1.15lf\n",A[i][0],A[i][1],A[i][2],A[i][3]);
		printf("Tangent : From (%1.15lf,%1.15lf) to (%1.15lf,%1.15lf)\n",T[i][0],T[i][1],T[i][2],T[i][3]);
	}
	printf("Arc     : (%1.15lf,%1.15lf) From %1.15lf to %1.15lf\n",A[i][0],A[i][1],A[i][2],A[i][3]);
	printf("Lower section\n");
	int j;
	for(i=0;i<l-1;i++) {
		j = u+i;
		printf("Arc     : (%1.15lf,%1.15lf) From %1.15lf to %1.15lf\n",A[j][0],A[j][1],A[j][2],A[j][3]);
		printf("Tangent : From (%1.15lf,%1.15lf) to (%1.15lf,%1.15lf)\n",T[j-1][0],T[j-1][1],T[j-1][2],T[j-1][3]);
	}
	j=u+i;
	printf("Arc     : (%1.15lf,%1.15lf) From %1.15lf to %1.15lf\n",A[j][0],A[j][1],A[j][2],A[j][3]);
}

int main() {
	int n;
	double r;
	//printf("Enter the no. of circles: ");
	scanf("%d",&n);
	//printf("Enter the radius of circles: ");
	scanf("%lf",&r);
	center* S = (center*)malloc(n*sizeof(center));
	//printf("Enter the co-ordinates of all n points:\n");
	int i;
	for(i=0;i<n;i++) {
		double xinput, yinput;
		scanf("%lf",&xinput);
		scanf("%lf",&yinput);
		S[i].x = xinput;
		S[i].y = yinput;
	}
	//sorting using mergesort
	mergeSort(S, 0, n-1);
	//printing
	printf("\nCircles after sorting\n");
	for(i=0;i<n;i++) {
		printf("%1.15lf %1.15lf\n",S[i].x,S[i].y);
	}
	printf("\n");
	
	//upper hull
	int upper[n];
	int uppertop;
	uppertop = CH(S, upper, n, 0);
	//printing upper hull
	printf("Upper hull\n");
	for(i=0;i<uppertop;i++) {
		printf("%1.15lf %1.15lf\n",S[upper[i]].x,S[upper[i]].y);
	}
	printf("\n");
	//lower hull
	int lower[n];
	int lowertop;
	lowertop = CH(S, lower, n, 1);
	//printing lower hull
	printf("Lower hull\n");
	for(i=0;i<lowertop;i++) {
		printf("%1.15lf %1.15lf\n",S[lower[i]].x,S[lower[i]].y);
	}
	printf("\n");
	
	double arc[uppertop + lowertop][4];
	double tangent[uppertop + lowertop][4];
	//calculationg containment zone
	contzone(S, upper, uppertop, lower, lowertop, r, tangent, arc);
	//printing containment zone
	printcontzone(uppertop, lowertop, tangent, arc);
	return 0;
}
