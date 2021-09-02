// ANIMESH BARUA
// 18CS10003
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <time.h>
// struct for a point
typedef struct point
{
	double x;
	double y;
} point;
// struct for an arc
typedef struct arc
{
	point center;
	double start_angle;
	double end_angle;
} arc;
// struct for a tangent
typedef struct tangent
{
	point start;
	point end;
} tangent;
// merge sort
void merge(point* arr, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	point* L;
	point* R;
	L = (point*)malloc(n1 * sizeof(point));
	R = (point*)malloc(n2 * sizeof(point));
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];
	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2) {
		if (L[i].x <= R[j].x) {
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

void mergeSort(point* arr, int l, int r)
{
	if (l < r) {

		int m = l + (r - l) / 2;

		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}
// function for printing the array after sorting
void printArray(point* A, int size)
{
	printf("+++ Circles after sorting\n");
	for (int i = 0; i < size; i++)
	{
		printf(" %0.15f %0.15f \n", A[i].x, A[i].y);
	}
	printf("\n");
}
// data structure of stack for implimenting grahm's scan
typedef struct stackNode
{
	point data;
	struct stackNode* next;
} stackNode;
bool isEmpty(stackNode* st)
{
	if (st == NULL)
		return true;
	return false;
}
stackNode* createStack()
{
	stackNode* s = NULL;
	return s;
}
stackNode* newNode(point p)
{
	struct stackNode* sNode = (stackNode*)malloc(sizeof(struct stackNode));
	sNode->data = p;
	sNode->next = NULL;
	return sNode;
}
void push(stackNode** root, point data)
{
	stackNode* sNode = newNode(data);
	sNode->next = *root;
	*root = sNode;
	// printf("%d pushed to stack\n", data);
}

void pop(stackNode** root)
{
	if (isEmpty(*root))
		return;
	stackNode* temp = *root;
	*root = (*root)->next;
	free(temp);
}
point top(stackNode* st)
{
	// check if st is not null
	// if (st == NULL)
	// {
	//     printf("helo\n");
	// }
	return st->data;
}
point top_next(stackNode* st)
{
	// before calling this check if st->next!=NULL
	if (st->next != NULL)
	{
		return (st->next)->data;
	}
}
int size_stack(stackNode* s)
{
	stackNode* t = s;
	int count = 0;
	while (t != NULL)
	{
		count++;
		t = t->next;
	}
	return count;
}
// cross product function for calculating a left turn or a right turn
bool calculate_angle(point present, point prev, point prev_prev)
{
	point v1, v2;
	v1.x = present.x - prev.x;
	v1.y = present.y - prev.y;
	v2.x = prev.x - prev_prev.x;
	v2.y = prev.y - prev_prev.y;
	// v2 cross v1
	double cross_product = (v2.x * v1.y) - (v2.y * v1.x);
	if (cross_product > 0)
	{
		// left turn
		return false;
	}
	else
	{
		// right turn
		return true;
	}
}
void swap(point *xp, point *yp)
{
	point temp = *xp;
	*xp = *yp;
	*yp = temp;
}
void reverse(point* arr, int n)
{
	int i = 0;
	int j = n;
	while (i < j)
	{
		swap(&arr[i], &arr[j]);
		i++;
		j--;
	}
}
int CH(point* S, int n, bool flag, point* H)
{
	// flag ==true  ---> UH
	// flag == false  ---> LH
	stackNode* s = createStack();
	if (flag == true)
	{
		// calculate upper hull
		push(&s, S[0]);
		push(&s, S[1]);
		for (int i = 2; i < n; ++i)
		{
			point present = S[i];
			point prev = top(s);
			point prev_prev = top_next(s);
			bool check = calculate_angle(present, prev, prev_prev);
			if (check == false)
			{
				// left turn
				// pop the prev while check == false,and then push the present
				// printf(" left %lf\n", present.y );
				while (check == false)
				{
					pop(&s);
					if (size_stack(s) <= 1)
						break;
					prev = top(s);
					point prev_prev = top_next(s);
					check = calculate_angle(present, prev, prev_prev);
				}
				push(&s, present);
			}
			else
			{
				// right turn
				// push the present
				// printf(" right %lf\n", present.y);
				push(&s, present);
			}
		}
	}
	else
	{
		// reverse(S, n);
		push(&s, S[n - 1]);
		push(&s, S[n - 2]);
		for (int i = n - 3; i >= 0; --i)
		{
			point present = S[i];
			point prev = top(s);
			point prev_prev = top_next(s);
			bool check = calculate_angle(present, prev, prev_prev);
			if (check == false)
			{
				// right turn
				while (check == false)
				{
					pop(&s);
					if (size_stack(s) <= 1)
						break;
					prev = top(s);
					point prev_prev = top_next(s);
					check = calculate_angle(present, prev, prev_prev);
				}
				push(&s, present);
			}
			else
			{
				// left turn
				push(&s, present);

			}
		}
	}
	int ans = size_stack(s); // the number of vertices
	for (int i = 0; i < ans; ++i)
	{
		H[i] = top(s);
		pop(&s);
	}
	return ans - 1;    // -1 because edges are required to return
}
void contzone(point* UH, int u, point* LH, int l, double r, tangent* T, arc* A)
{
	// for the tangents
	int it = 0;
	for (int i = 0; i < u - 1; ++i)
	{
		// the upper hull tangents
		double theta = atan2((UH[i + 1].y - UH[i].y), (UH[i + 1].x - UH[i].x));
		double cosine = cos(theta);
		double sine = sin(theta);
		// printf("sin cos%0.15lf %0.15lf\n", sin, cos );
		// printf("%0.15lf\n", r  );
		T[it].start.x = UH[i].x - ((r) * sine);
		T[it].end.x = UH[i + 1].x - (r * sine);
		T[it].start.y = UH[i].y + (r * cosine);
		T[it].end.y = UH[i + 1].y + (r * cosine);
		it++;
	}
	for (int i = 0; i < l - 1; ++i)
	{
		// the lower hull tangents
		double theta = atan2((LH[i + 1].y - LH[i].y), (LH[i + 1].x - LH[i].x));
		double cosine = cos(theta);
		double sine = sin(theta);
		// printf("sin cos%0.15lf %0.15lf\n", sin, cos );
		// printf("%0.15lf\n", r  );
		T[it].start.x = LH[i].x - ((r) * sine);
		T[it].end.x = LH[i + 1].x - (r * sine);
		T[it].start.y = LH[i].y + (r * cosine);
		T[it].end.y = LH[i + 1].y + (r * cosine);
		it++;
	}
	// calculating the arc
	it = 0;
	// for upper hull arcs
	A[0].center.x = UH[0].x;
	A[0].center.y = UH[0].y;
	A[0].start_angle = 3.141592653589793;
	double prev_slope = (T[0].end.y - T[0].start.y) / (T[0].end.x - T[0].start.x);
	double angle = atan(prev_slope);
	A[0].end_angle = angle + (3.141592653589793 / 2);
	it = 1;
	for (int i = 1; i < u; ++i)
	{
		A[it].start_angle = angle + (3.141592653589793 / 2);
		A[it].center.x = UH[i].x;
		A[it].center.y = UH[i].y;
		if (i == u - 1)
		{
			A[it].end_angle = 0;
			it++;
			continue;
		}
		prev_slope = (T[i].end.y - T[i].start.y) / (T[i].end.x - T[i].start.x);
		angle = atan(prev_slope);
		A[it].end_angle = angle + (3.141592653589793 / 2);
		it++;
	}

	// for lower hull arcs

	A[it].center.x = LH[0].x;
	A[it].center.y = LH[0].y;
	A[it].start_angle = 0;
	prev_slope = (T[u - 1].end.y - T[u - 1].start.y) / (T[u - 1].end.x - T[u - 1].start.x);
	angle = atan(prev_slope);
	A[it].end_angle = angle - 3.141592653589793 / 2;
	it++;
	for (int i = 1; i < l; i++)
	{

		A[it].start_angle = angle - 3.141592653589793 / 2;
		A[it].center.x = LH[i].x;
		A[it].center.y = LH[i].y;
		if (i == l - 1)
		{
			A[it].end_angle = -3.141592653589793;
			continue;
		}
		prev_slope = (T[it - 1].end.y - T[it - 1].start.y) / (T[it - 1].end.x - T[it - 1].start.x);
		angle = atan(prev_slope);
		A[it].end_angle = angle - 3.141592653589793 / 2;
		it++;
	}
}
int main()
{
	double r, n;
	scanf("%lf", &n);
	scanf("%lf", &r);
	point* s;
	s = (point*)malloc(n * sizeof(point));
	for (int i = 0; i < n; ++i)
	{
		scanf("%lf", &s[i].x);
		scanf("%lf", &s[i].y);
	}
	mergeSort(s, 0, n - 1);
	printArray(s, n);
	point* upper_hull, *lower_hull;
	upper_hull = (point*)malloc(n * sizeof(point));
	lower_hull = (point*)malloc(n * sizeof(point));
	int num_uh = CH(s, n, true, upper_hull);
	int num_lh = CH(s, n, false, lower_hull);
	// printf("%d\n", num_uh);
	printf("+++ Upper hull\n");
	mergeSort(upper_hull, 0, num_uh);
	for (int i = 0; i < num_uh + 1; ++i)
	{
		printf("%.015lf %0.15lf\n", upper_hull[i].x, upper_hull[i].y );
	}
	printf("\n");
	printf("+++ Lower hull\n");
	reverse(lower_hull, num_lh);
	for (int i = 0; i < num_lh + 1; ++i)
	{
		printf("%.015lf %0.15lf\n", lower_hull[i].x, lower_hull[i].y );
	}
	printf("\n");
	printf("+++ The Containment Zone\n");
	tangent* t = (tangent*)malloc((num_lh + num_uh) * sizeof(tangent));
	arc* a = (arc*)malloc((num_lh + num_uh + 2) * sizeof(arc));
	contzone(upper_hull, num_uh + 1, lower_hull, num_lh + 1, r, t, a);
	// printf("printing tangents\n");
	printf("--- Upper section\n");
	int i_arc = 0;
	int i_tangent = 0;
	while ((i_arc < num_uh + 1) || (i_tangent < num_uh))
	{
		if (i_arc < num_uh + 1)
		{
			printf("Arc 	: (%0.15lf,%0.15lf) From %0.15lf to %0.15lf\n", a[i_arc].center.x, a[i_arc].center.y, a[i_arc].start_angle, a[i_arc].end_angle );
			i_arc++;
		}
		if (i_tangent < num_uh)
		{
			printf("Tangent : From (%0.15lf,%0.15lf) to (%0.15lf, %0.15lf)\n", t[i_tangent].start.x, t[i_tangent].start.y, t[i_tangent].end.x, t[i_tangent].end.y );
			i_tangent++;
		}
	}
	printf("--- Lower section\n");
	while ((i_arc < num_uh + num_lh + 2) || (i_tangent < num_uh + num_lh))
	{
		if (i_arc < num_uh + num_lh + 2)
		{
			printf("Arc 	: (%0.15lf,%0.15lf) From %0.15lf to %0.15lf\n", a[i_arc].center.x, a[i_arc].center.y, a[i_arc].start_angle, a[i_arc].end_angle );
			i_arc++;
		}
		if (i_tangent < num_uh + num_lh)
		{
			printf("Tangent : From (%0.15lf,%0.15lf) to (%0.15lf, %0.15lf)\n", t[i_tangent].start.x, t[i_tangent].start.y, t[i_tangent].end.x, t[i_tangent].end.y );
			i_tangent++;
		}
	}
}