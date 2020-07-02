#include <iostream>

using namespace std;

class node
{
public:
    int fork;
    int is_correctPath;
    int count = 0;

    node *first;
    node *second;
    node *third;
};

class info
{
public:
    int pathNum = 0;
    int nextRow[3] = {0};
    int nextCol[3] = {0};
};

class matrix
{
public:
    matrix(){};
    matrix(int, int, int);
    void print();

    // test print
    void printMatrix();

    int col, row, serial;
    int difficulty, N1, N2, L1, L2;
    int **maze;
    int deadEnd[20] = {0};

    node *root;
};

matrix::matrix(int serial, int col, int row)
{
    this->col = col;
    this->row = row;
    this->serial = serial;

    difficulty = 0;
    L1 = L2 = N1 = N2 = 0;

    root = NULL;

    maze = new int *[row];
    for (int i = 0; i < row; i++)
        maze[i] = new int[col];

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            maze[i][j] = 0;
}

void matrix::print()
{
    cout << serial << endl;

    cout << difficulty << " " << N1 << " " << N2 << " " << L1 << " " << L2;
}

void matrix::printMatrix()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int QuickSortOnce(int *a, int low, int high)
{
    int pivot = a[low];
    int i = low, j = high;

    while (i < j)
    {
        while (a[j] >= pivot && i < j)
            j--;

        a[i] = a[j];

        while (a[i] <= pivot && i < j)
            i++;

        a[j] = a[i];
    }

    a[i] = pivot;

    return i;
}

void QuickSort(int *a, int low, int high)
{
    if (low >= high)
        return;

    int pivot = QuickSortOnce(a, low, high);

    QuickSort(a, low, pivot - 1);

    QuickSort(a, pivot + 1, high);
}

int EvaluateMedian(int *a, int n)
{
    QuickSort(a, 0, n - 1);

    if (n % 2 != 0)
        return a[n / 2];
    else
        return (a[n / 2] + a[n / 2 - 1]) / 2;
}

node *creatNode()
{
    node *newnode = new node;
    newnode->fork = newnode->is_correctPath = 0;
    newnode->first = newnode->second = newnode->third = NULL;
    return newnode;
}

info pathExist(matrix *maze, int row, int col)
{
    info current;
    int index = 0;

    maze->maze[row][col] = 1;

    if (col != maze->col - 1 && !maze->maze[row][col + 1]) // right
    {
        current.nextRow[index] = row;
        current.nextCol[index++] = col + 1;

        current.pathNum++;
    }

    if (row != maze->row - 1 && !maze->maze[row + 1][col]) // down
    {
        current.nextRow[index] = row + 1;
        current.nextCol[index++] = col;

        current.pathNum++;
    }

    if (col != 0 && !maze->maze[row][col - 1]) // left
    {
        current.nextRow[index] = row;
        current.nextCol[index++] = col - 1;

        current.pathNum++;
    }

    if (row != 0 && !maze->maze[row - 1][col]) // up
    {
        current.nextRow[index] = row - 1;
        current.nextCol[index++] = col;

        current.pathNum++;
    }

    return current;
}

node *DFStree(matrix *maze, int row, int col, int count)
{
    node *root = creatNode();
    info msg = pathExist(maze, row, col);
    count++;
    root->count = count;

    if (msg.pathNum)
        root->fork = msg.pathNum - 1;

    if (!msg.pathNum)
    {
        if (row == maze->row - 2 && col == maze->col - 1) // is end
            root->is_correctPath = 1;

        return root;
    }

    if (msg.pathNum == 3)
    {
        root->third = DFStree(maze, msg.nextRow[2], msg.nextCol[2], count);
        root->second = DFStree(maze, msg.nextRow[1], msg.nextCol[1], count);
    }

    else if (msg.pathNum == 2)
        root->second = DFStree(maze, msg.nextRow[1], msg.nextCol[1], count);

    root->first = DFStree(maze, msg.nextRow[0], msg.nextCol[0], count);
}

int countL2 = 0;

void preorderTraveral(node *p, matrix *maze, int count)
{
    if (p)
    {
        // test print
        cout << p->is_correctPath << " ";

        preorderTraveral(p->first, maze, count);

        preorderTraveral(p->second, maze, count);

        preorderTraveral(p->third, maze, count);
    }
}

void determineValue(matrix *maze)
{
    countL2 = 0;

    //preorderTraveral(maze->root, maze, 0);
    //cout << endl;

    maze->L2 = EvaluateMedian(maze->deadEnd, maze->N2);

    maze->difficulty = ((maze->N1 + maze->N2) * (maze->N1 + maze->N2));
}

int main()
{
    int num, col, row, serial;
    char tmp;
    matrix *input;

    // input
    cin >> num;

    input = new matrix[num];

    for (int i = 0; i < num; i++)
    {
        cin >> serial >> col >> row;

        input[i] = matrix(serial, col, row);

        for (int j = 0; j < row; j++)
        {
            for (int k = 0; k < col; k++)
            {
                cin >> tmp;

                if (tmp == 'N')
                    input[i].maze[j][k] = 1;
            }
        }
    }

    input[0].printMatrix();

    // process
    for (int i = 0; i < num; i++)
    {
        input[i].root = DFStree(&input[i], 1, 0, 0);

        determineValue(&input[i]);
    }

    // test print

    node *p;

    for (int i = 0; i < 1; i++)
    {
        preorderTraveral(input[i].root, &input[i], 0);

        cout << endl;
    }

    p = input[0].root;

    cout << p->count << " ";
    p = p->first;
    cout << p->count << " ";
    p = p->first;
    cout << p->count << " ";
    p = p->second;
    cout << p->count << " ";
    p = p->first;
    cout << p->count << " ";
    p = p->third;
    cout << p->count << " ";

    cout << endl;

    /*
    for (int i = 0; i < num; i++)
    {
        input[i].print();
        cout << endl;
    }
*/
    //input[0].print();

    return 0;
}
