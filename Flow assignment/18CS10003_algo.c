// ANIMESH BARUA
// 18CS10003
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
typedef struct edge
{
	int y; // end point
	int c; // capacity
	int f; // flow value
	struct edge *next; // pointer to next edge
} edge;
typedef struct vertex
{
	int x; // id
	int n; // need value
	edge *p; // pointer to edge node
} vertex;
typedef struct graph
{
	int v; // number of vertexes
	int e; // number of edges
	vertex *h; // pointer to array of vertices
} graph;
graph *ReadGraph(const char *fname)
{
	FILE * filePointer;
	filePointer = fopen(fname, "r");
	int n, m;
	fscanf(filePointer, "%d %d", &n, &m);
	graph *g = (graph*)malloc(sizeof(graph));
	g->v = n;
	g->e = m;
	g->h = (vertex*)malloc((n + 2) * sizeof(vertex));
	for (int i = 1; i <= n; ++i)
	{
		/* code */
		(g->h)[i].x = i;
		int temp;
		fscanf(filePointer, "%d", &temp);
		(g->h)[i].n = temp;
		(g->h)[i].p = NULL;
	}
	for (int i = 1; i <= m; ++i)
	{
		int from, to, capacity;
		fscanf(filePointer, "%d %d %d", &from, &to, &capacity);
		edge *e	= (edge*)malloc(sizeof(edge));
		e->y = to;
		e->c = capacity;
		e->f = 0;
		e->next = (g->h)[from].p;
		(g->h)[from].p = e;
	}
	return g;
}
void PrintGraph(graph *g)
{
	for (int i = 1; i <= g->v ; ++i)
	{
		/* code */
		printf("%d", i);
		edge *head = (g->h)[i].p;
		// if (head == NULL)
		// 	printf("yes\n");
		while (head != NULL)
		{
			// y c f
			if (head->y == g->v + 1)
			{
				head = head->next;
				continue;
			}
			printf("-> (%d, %d, %d)", head->y, head->c, head->f);
			head = head->next;
		}
		printf("\n");
	}
}
graph* compute_residual_graph(graph* g)
{
	graph* r_graph = (graph*)malloc(sizeof(graph));
	r_graph->v = g->v;
	r_graph->e = g->e;
	int n = g->v;
	r_graph->h = (vertex*)malloc((g->v + 2) * sizeof(vertex));
	for (int i = 0; i <= n + 1; ++i)
	{
		r_graph->h[i].p = NULL;
		(r_graph->h)[i].x = (g->h)[i].x;
		(r_graph->h)[i].n = (g->h)[i].n;
		edge* head = (g->h)[i].p;
		while (head != NULL)
		{
			edge* t = (edge*)malloc(sizeof(edge));
			t->y = head->y;
			t->c = head->c;
			t->f = head->c;
			t->next = (r_graph->h)[i].p;
			(r_graph->h)[i].p = t;
			head = head->next;
		}
	}
	for (int u = 0; u <= n + 1; ++u)
	{
		edge* head = (r_graph->h)[u].p;
		while (head != NULL)
		{
			int v = head->y;
			// check if v to u edge exists
			edge* temp = (r_graph->h)[v].p;
			int flag = 0;
			while (temp != NULL)
			{
				if (temp->y == u)
				{
					flag = 1;
					break;
				}
				temp = temp->next;
			}
			if (flag == 0)
			{
				// no edge exists
				edge* t = (edge*)malloc(sizeof(edge));
				t->y = u;
				t->c = head->c;
				t->f = 0;
				t->next = (r_graph->h)[v].p;
				(r_graph->h)[v].p = t;
			}
			head = head->next;
		}
	}
	return r_graph;
}
typedef struct path_box
{
	int v;
	struct path_box* next;
} path_box;
typedef struct box
{
	// this struct is the data that each queue node stores
	int flow_val; // this is the flow possible in the path i.e. min(all flows)
	int value;
	path_box* path;// this is for the path
} box;
typedef struct QNode {
	box b;
	struct QNode* next;
} QNode;

typedef struct Queue {
	struct QNode *front, *rear;
} Queue;

struct QNode* newNode(box k)
{
	struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
	temp->b = k;
	temp->next = NULL;
	return temp;
}

struct Queue* createQueue()
{
	struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
	q->front = q->rear = NULL;
	return q;
}
box top(Queue* q)
{
	return (q->front)->b;
}
void push(struct Queue* q, box k)
{
	struct QNode* temp = newNode(k);
	if (q->rear == NULL) {
		q->front = q->rear = temp;
		return;
	}

	q->rear->next = temp;
	q->rear = temp;
}

void pop(struct Queue* q)
{
	if (q->front == NULL)
		return;

	struct QNode* temp = q->front;

	q->front = q->front->next;

	if (q->front == NULL)
		q->rear = NULL;

	free(temp);
}
int size(Queue* q)
{
	QNode* head = q->front;
	int count = 0;
	while (head != NULL)
	{
		count++;
		head = head->next;
	}
	return count;
}
path_box* create_pathbox(int k)
{
	path_box* b = (path_box*)malloc(sizeof(path_box));
	b->v = k;
	b->next = NULL;
	return b;
	// returns a path box with value k and next=null
}
void print_path(path_box* p)
{
	if (p == NULL)
		printf("\n");
	path_box* head = p;
	while (head != NULL)
	{
		printf("%d  ", head->v );
		head = head->next;
	}
	printf("\n");
}
void update_residual(graph* r_graph, path_box* res_path, int k)
{
	path_box* head = res_path;
	while (head != NULL)
	{
		int from = head->v;
		edge* e = (r_graph->h)[from].p;
		int to ;
		if (head->next != NULL)
			to = (head->next)->v;
		else if (head->next == NULL)
			return;
		while (e != NULL)
		{
			if (e->y == to)
			{
				(e->f) += k;
				break;
			}
			e = e->next;
		}
		edge* e1 = (r_graph->h)[to].p;
		while (e1 != NULL)
		{
			if (e1->y == from)
			{
				(e1->f) -= k;
				break;
			}
			e1 = e1->next;
		}
		head = head->next;
	}
}
void update_main_graph(graph* g, path_box* res_path, int k)
{
	path_box* head = res_path;
	while (head != NULL)
	{
		int to = head->v;// w
		int from;// v
		if (head->next != NULL)
			from = head->next->v;
		else if (head->next == NULL)
			return;
		edge* e = (g->h)[from].p;
		bool flag = false;
		while (e != NULL)
		{
			if (e->y == to)
			{
				e->f += k;
				flag = true;
				break;
			}
			e = e->next;
		}
		if (flag == false)
		{
			edge* e1 = (g->h)[to].p;
			while (e1 != NULL)
			{
				if (e1->y == from)
				{
					e1->f -= k;
					break;
				}
				e1 = e1->next;
			}
		}
		head = head->next;
	}
}
void ComputeMaxFlow(graph *g, int s, int t)
{
	int ans = 0; // the flow value to output
	graph* r_graph = compute_residual_graph(g);
	// PrintGraph(r_graph);
	bool check = true;
	// int me = 2;
	while (check == true)
	{
		Queue* q = createQueue();
		box a;
		a.flow_val = INT_MAX;
		a.value = s;
		a.path = create_pathbox(s);
		push(q, a);
		bool visited[g->v + 2];
		for (int i = 0; i <= g->v + 1; ++i)
		{
			visited[i] = false;
		}
		visited[s] = true;
		bool out = true;
		int res_flow_val = 0;
		path_box* res_path;
		int level = -2;
		while (1)
		{
			int count = size(q);
			level++;
			// printf("count %d\n", count );
			while (count > 0)
			{
				box up = top(q);
				pop(q);
				edge* head = (r_graph->h)[(up.path)->v].p;
				while (head != NULL)
				{
					int v = head->y;
					box temp = up;
					if ( head->f > 0)
					{
						// it can be visited and added to queue
						// update the flow value, make it visited, add the path
						// printf("%d\n", v );
						visited[v] = true;
						if (head->f < temp.flow_val)
							temp.flow_val = head->f;
						path_box* b = create_pathbox(v);
						b->next = temp.path;
						temp.path = b;
						push(q, temp);
					}
					head = head->next;
				}
				count--;
			}
			if (size(q) == 0 || level > g->v)
			{
				// means path cannot be found and stop checking
				check = false;
				out = false;
				break;
			}
			if (visited[t] == true)
			{
				// means path is found
				while (size(q) > 0)
				{
					box up = top(q);
					pop(q);

					if ((up.path)->v != t)
					{
						// printf("hey\n");
						continue;
					}
					if (up.flow_val > res_flow_val)
					{
						res_flow_val = up.flow_val;
						res_path = up.path;
					}

				}
				break;
			}
		}
		// out is true means a path is found
		if (out == true)
		{
			// res_flow_val is the present flow value
			// res_path keeps the present path
			// a path is given do the operations
			// update the residual graph and the main graph g
			ans += res_flow_val;
			update_residual(r_graph, res_path, res_flow_val);
			update_main_graph(g, res_path, res_flow_val);
		}
	}
	// PrintGraph(g);
	printf("The maximum flow possible from source to sink ( by ComputeMaxFlow ) is = %d\n", ans );
}
void NeedBasedFlow(graph *g)
{
	// 0 is taken as source
	// n+1 is taken as sink
	printf("====================== Computing Need Based Flow =====================================\n");
	// printf("\n");
	(g->h)[0].x = 0;
	(g->h)[0].n = 0;
	(g->h)[0].p = NULL;
	(g->h)[g->v + 1].x = g->v + 1;
	(g->h)[g->v + 1].n = 0;
	(g->h)[g->v + 1].p = NULL;
	int n = g->v;
	int producer_need = 0;
	int consumer_need = 0;
	for (int i = 1; i <= n; ++i)
	{
		if ((g->h)[i].n < 0)
			producer_need += (-1 * (g->h)[i].n);
		else if ((g->h)[i].n > 0)
			consumer_need += (g->h)[i].n;
	}
	// printf("%d \n %d", producer_need, consumer_need );
	if (consumer_need != producer_need)
	{
		printf("No need based flow is possible because consumer need is not equal to producer need\n");
		return;
	}
	for (int i = 1; i <= n; ++i)
	{
		if ((g->h)[i].n < 0)
		{
			// producer :- so connect source to the producer
			// printf("producer %d\n", i );
			edge *e	= (edge*)malloc(sizeof(edge));
			e->y = i;
			e->c = -1 * (g->h)[i].n;
			e->f = 0;
			e->next = (g->h)[0].p;
			(g->h)[0].p = e;
		}
		else if ((g->h)[i].n > 0)
		{
			// consumer :- so connect consumer to sink
			// printf("consumer %d\n", i );
			edge *e	= (edge*)malloc(sizeof(edge));
			e->y = n + 1;
			e->c = (g->h)[i].n;
			e->f = 0;
			e->next = (g->h)[i].p;
			(g->h)[i].p = e;

		}
	}
	ComputeMaxFlow(g, 0, n + 1);
	int it = 0;
	edge* e = (g->h)[0].p;
	while (e != NULL)
	{
		it += (e->f);
		e = e->next;
	}
	printf("The need of producer and consumer is %d\n", producer_need);
	if (it != producer_need)
	{
		printf("Hence, No Need Based flow is possible because the max flow( using ComputeMaxFlow ) is not equal to producer and consumer's need\n");
		for (int i = 1; i <= n; ++i)
		{
			edge* e = (g->h)[i].p;
			while (e != NULL)
			{
				e->f = 0;
				e = e->next;
			}
		}
	}
	else
	{
		printf("Hence, as the producer and consumer need are equal to the max flow, The Need Based Flow is equal to ");
		edge* e = (g->h)[0].p;
		int k = 0;
		while (e != NULL)
		{
			k += e->f ;
			e = e->next;
		}
		printf("%d\n", k );
	}
}
int main()
{
	char *fname = (char*)malloc(22 * sizeof(char));
	printf("Enter a file name to read the graph from( the file name should be of .txt extension):- ");
	scanf("%s", fname);
	graph *ans = ReadGraph(fname);
	PrintGraph(ans);
	int source, sink;
	printf("Please enter a source from [1,%d]: ", ans->v );
	scanf("%d", &source);
	printf("Please enter a sink from [1,%d] ( and sink!=%d ): ", ans->v, source );
	scanf("%d", &sink);
	ComputeMaxFlow(ans, source, sink );
	PrintGraph(ans);
	graph *ans_new = ReadGraph(fname);
	NeedBasedFlow(ans_new);
	PrintGraph(ans_new);
}