/* ----------------------------------------------------------
 * file: test1.c
 * author: Haofeng Yang 3190105301@zju.edu.cn
  *        Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * ----------------------------------------------------------
 * function: sort the input integer and print sorted integer
 * description: quick sort based on the algorithm in
 *              Data Structure And Algorithm Analysis in C
 * ----------------------------------------------------------
 * create date: 2022/4/15 2:13 pm
 * last modified: 2022/4/15 2:53 pm
 * ----------------------------------------------------------
 * history: 2022/4/15 2:13 pm create the file
 *          2022/4/15 2:53 pm finish the codes
 */

//#include <stdio.h>

// swap the location of A and B
// :param arr: array
// :param pos1: index of the 1st integer
// :param pos2: index of the 2nd integer
void swap(int arr[], int pos1, int pos2)
{
	int temp = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = temp;
}

// Insertion Sort
// :param arr: array to sort
// :param N: number of integers to sort
void InsertionSort(int arr[], int N)
{
    int temp;
    int i, j;

    for (i = 1; i < N; i = i + 1) { // from the 2nd to last
        temp = arr[i];  // record arr[i] to insert eventually
        for (j = i; j > 0 && arr[j - 1] > temp; j = j - 1) {
            arr[j] = arr[j - 1]; // move backward to make room for i
        }
        arr[j] = temp; // find place of i
    }
}

// sort Left, Center and Right, move pivot and return
// :param arr: array
// :param Left: leftmost position
// :param right: rightmost position
// :return choosed pivot
int Median3(int arr[], int Left, int Right)
{
    int Center = (Left + Right) / 2;

    if (arr[Left] > arr[Center]) {
        swap(arr, Left, Center);  // now Left < Center
    }
    if (arr[Left] > arr[Right]) {
        swap(arr, Left, Right);   // now Left < Right
    }
    if (arr[Center] > arr[Right]) { 
        swap(arr, Center, Right); // now Center < Right
    }
    swap(arr, Center, Right - 1); // move Center/pivot
    return arr[Right - 1]; // return pivot
}

// sort Left and Right section of pivot and conduct recursion
// :param arr: array to sort
// :param: Left: leftmost position to sort
// :param: Right: rightmost position to sort
void QSort(int arr[], int Left, int Right)
{
    int i, j;
    int Pivot;

    if (Left + 3 <= Right) {
        Pivot = Median3(arr, Left, Right); // get pivot
        i = Left;
        j = Right - 1;
        while (1) {
            // stop until >= Pivot
            while (1) {
                i = i + 1;
                if(Pivot <= arr[i]) {
                    break;
                }
            }
            // stop until <= Pivot
            while (1) {
                j = j - 1;
                if(arr[j] <= Pivot) {
                    break;
                }
            }

            if (i < j) {
                swap(arr, i, j); // so arr[i] <= Pivot <= arr[j]
            }
            else {
                break;  // finish
            }
        }
        swap(arr, i, Right - 1); // move pivot to correct position

        QSort(arr, Left, i - 1);    // sort left section
        QSort(arr, i + 1, Right);   // sort right section
    }
    else { // length is too short, InsertionSort is faster
        InsertionSort(arr + Left, Right - Left + 1);
    }
}

// start Quick Sort
// :param arr: array to sort
// :param N: range to sort
void QuickSort(int arr[], int N)
{
    QSort(arr, 0, N - 1);
}

int main(void)
{
    int N; // # of number
    int arr[10000]; // arr of input number

    // read input
    scanf("%d", &N);
    for (int i = 0; i < N; i = i + 1) {
        scanf("%d", &(arr[i]));
    }

    // sort
    QuickSort(arr, N);

    // output
    for (int i = 0; i < N; i = i + 1) {
        printf("%d\n", arr[i]);
    }

    return 0;
}
