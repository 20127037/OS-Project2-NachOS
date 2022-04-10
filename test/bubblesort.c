#include "syscall.h"

void swap(int *left, int *right)
{
  int temp = *left;
  *left = *right;
  *right = temp;
}

void bubbleSort(int arr[], int n)
{
  for(int i = 0; i < n -1; i++)
    for (int j = 0; j < n - i - 1; j++)
      if(arr[j] > arr[j+1])
        swap(&arr[j],&arr[j+1]);
}

void printArray(int arr[],int size)
{
  for(int i = 0; i < size; i++)
  {
    PrintInt(arr[i]);
    PrintChar(' ');
  }
  PrintChar('\n');
}

void inputArray(int arr[], int *n)
{
  PrintString("Enter the number of elements (0 < n <= 100): ");
  *n = ReadInt();
  if (n < 0 || n > 100)
  {
    PrintString("Please enter again!!");
    break;
  }
  for(int i = 0; i < n ; i++)
  {
    PrintString("Enter the elements ");
    PrintInt(i+1);
    PrintString(": ");
    arr[i] = ReadInt();
  }
}

int main()
{
  int arr[100];
  int n;
  inputArray(arr, &n);
  bubbleSort(arr, n);
  PrintString("Sorted array: ");
  printArray(arr,n);
  PrintString("\n");
  return 0;
}
