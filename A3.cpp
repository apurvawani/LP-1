#include <omp.h>
#include <chrono>		//library used to deal with date and time
#include <time.h>
#include <iostream>

//bubble Sort Serial
void bubbleSortSerial(int a[], int n)
{
	bool swap = 0;

	for (int i = n - 1; i > 0; i--)
	{
		swap = false;

		for (int j = 0; j < i; j++)
		{
			if (a[j] > a[j + 1])
			{
				swap = true;

				std::swap(a[j], a[j + 1]);
			}
		}

		if (!swap)
			break;
	}
}

// Bubble Sort Parallel

void bubbleSortParallel(int a[], int n, int threads)
{
	omp_set_num_threads(threads);

	int first = 0;

	bool swap = false;

	for (int i = 0; i < n; i++)
	{
		first = i % threads;

		#pragma omp parallel for shared(a, swap, first)
		//parallely
		for (int j = first; j < n - 1; j += threads)
		{
			if (a[j] > a[j + 1])
			{
				swap = true;
				std::swap(a[j], a[j + 1]);
			}
		}

		if (!swap)
			break;
	}
}

// -------------------------------------------------------------------

void merge(int a[], int low, int mid, int high)
{

	int len = (high - low) + 1;

	int *temp = new int[len];

	int current = 0;
	int i = low, j = mid + 1;

	while (i <= mid && j <= high)
	{
		if (a[i] < a[j])
		{
			temp[current] = a[i];
			i++;
		}
		else
		{
			temp[current] = a[j];
			j++;
		}

		current++;
	}

	while (i <= mid)
	{
		temp[current] = a[i];
		i++;
		current++;
	}

	while (j <= high)
	{
		temp[current] = a[j];
		j++;
		current++;
	}

	current = 0;

	for (i = low; i <= high; i++)
	{
		a[i] = temp[current];
		current++;
	}
}
// Merge Sort Parallel
void mergeSortParallel(int a[], int low, int high)
{
	if (low < high)
	{
		int mid = (low + high) / 2;

		#pragma omp parallel sections
		{
			#pragma omp section
			mergeSortParallel(a, low, mid);

			#pragma omp section
			mergeSortParallel(a, mid + 1, high);
		}

		merge(a, low, mid, high);
	}
}

// Merge Sort Serial
void mergeSortSerial(int a[], int low, int high)
{
	if (low < high)
	{
		int mid = (low + high) / 2;

		mergeSortSerial(a, low, mid);
		mergeSortSerial(a, mid + 1, high);

		merge(a, low, mid, high);
	}
}

void displayArray(int a[], int n)
{
	for (int i = 0; i < n; i++)
		std::cout << a[i] << " ";
}

int main()
{
	srand(time(0));			//srand() gives a new seed(starting point) to the random function
	//time(0) gives the time in seconds since the Unix epoch, which is a pretty good "unpredictable" seed 
	//(guarantees your seed will be the same only once, unless you start your program multiple times within the same second).				

	int n = 10;

	std::cout << "\nEnter the size of array: ";
	std::cin >> n;

	int *a = new int[n], *b = new int[n], *c = new int[n];

	omp_set_num_threads(2);

	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % 100;

		b[i] = a[i];

		c[i] = a[i];
	}

	displayArray(a , n);

	int option = 0;

	std::cout << std::endl;
	std::cout << "\nBubble Sort or Merge Sort? (0 / 1): ";
	std::cin >> option;

	if (!option)
	{
		int threads = 2;

		auto start = std::chrono::steady_clock::now();

		bubbleSortParallel(a, n, threads);

		auto end = std::chrono::steady_clock::now();

		std::cout << "\nParallel time " << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;

		bubbleSortSerial(b, n);

		end = std::chrono::steady_clock::now();

		std::cout << "\nSerial time: " << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;

		displayArray(c , n);
		cout << endl;
	}
	else
	{
		auto start = std::chrono::steady_clock::now();

		mergeSortParallel(a, 0, n - 1);

		auto end = std::chrono::steady_clock::now();

		std::cout << "\nParallel time: " << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;

		// ----------------------------------------------------------------------------------------------------------------------

		start = std::chrono::steady_clock::now();

		mergeSortSerial(b, 0, n - 1);

		end = std::chrono::steady_clock::now();

		std::cout << "\nSerial time: " << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;

		displayArray(a , n);
		cout << endl;

		// ----------------------------------------------------------------------------------------------------------------------

		/*std::cout << std::endl;
		displayArray(b, n);
		std::cout << std::endl;*/
	}

	return 0;
}