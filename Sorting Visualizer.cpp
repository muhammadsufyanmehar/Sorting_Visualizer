#include <SDL.h>
#include <iostream>
#include <limits>
#include <time.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 910;
const int SCREEN_HEIGHT = 750;

const int arrSize = 130;
const int barWidth = 7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool complete = false;

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
        success = false;
    }
    else
    {
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout << "Warning: Linear Texture Filtering not enabled.\n";
        }

        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
                success = false;
            }
        }
    }

    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void visualize(int x = -1, int y = -1, int z = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j = 0;
    for (int i = 0; i <= SCREEN_WIDTH - barWidth; i += barWidth)
    {
        SDL_PumpEvents();

        SDL_Rect rect = {i, SCREEN_HEIGHT - arr[j], barWidth, arr[j]};
        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (j == x || j == z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void heapify(int arr[], int n, int i)
{
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // left = 2*i + 1
    int right = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i)
    {
        swap(arr[i], arr[largest]); // Swap root with largest

        // Visualize the swap
        visualize(i, largest);
        SDL_Delay(50);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void buildMaxHeap(int arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }
}

void heapSort(int arr[], int n)
{
    buildMaxHeap(arr, n);

    // One by one extract elements from heap
    for (int i = n - 1; i >= 0; i--)
    {
        // Move current root to end
        swap(arr[0], arr[i]);

        // Visualize the move of root to end
        visualize(0, i, i);
        SDL_Delay(75);

        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

int partition_array(int arr[], int start, int end)
{
    int pivotValue = arr[start];
    int count_small = 0;

    for (int i = (start + 1); i <= end; i++)
    {
        if (arr[i] <= pivotValue)
        {
            count_small++;
        }
    }
    int pivotIndex = start + count_small;

    swap(arr[pivotIndex], arr[start]);

    visualize(pivotIndex, start);

    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex)
    {
        while (arr[i] <= pivotValue)
        {
            visualize(i);
            i++;
        }
        while (arr[j] > pivotValue)
        {
            visualize(j);
            j--;
        }
        if (i < pivotIndex && j > pivotIndex)
        {
            swap(arr[i], arr[j]);

            visualize(i, j);
            SDL_Delay(70);

            i++;
            j--;
        }
    }
    return pivotIndex;
}

void quickSort(int arr[], int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int pivot = partition_array(arr, start, end);
    quickSort(arr, start, pivot - 1);
    quickSort(arr, pivot + 1, end);
}

void mergeTwoSortedArrays(int arr[], int start, int end)
{
    int size_output = (end - start) + 1;
    int *output = new int[size_output];

    int mid = start + (end - start) / 2;
    int i = start, j = mid + 1, k = 0;
    while (i <= mid && j <= end)
    {
        if (arr[i] <= arr[j])
        {
            output[k] = arr[i];
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            output[k] = arr[j];
            visualize(i, j);
            j++;
            k++;
        }
    }
    while (i <= mid)
    {
        output[k] = arr[i];
        visualize(-1, i);
        i++;
        k++;
    }
    while (j <= end)
    {
        output[k] = arr[j];
        visualize(-1, j);
        j++;
        k++;
    }
    int x = 0;
    for (int l = start; l <= end; l++)
    {
        arr[l] = output[x];
        visualize(l);
        SDL_Delay(25);
        x++;
    }
    delete[] output;
}

void mergeSort(int arr[], int start, int end)
{
    if (start >= end)
    {
        return;
    }
    int mid = start + (end - start) / 2;

    mergeSort(arr, start, mid);
    mergeSort(arr, mid + 1, end);

    mergeTwoSortedArrays(arr, start, end);
}

void insertionSort()
{
    for (int i = 1; i < arrSize; i++)
    {
        int temp = arr[i];
        int j = i - 1;
        while (j >= 0)
        {
            if (arr[j] > temp)
            {
                arr[j + 1] = arr[j];

                visualize(i, j, j + 1);
                SDL_Delay(15);
            }
            // if not greater mean previous array is sorted
            else
            {
                break;
            }
            j--;
        }
        arr[j + 1] = temp;
    }
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void bubbleSort()
{
    for (int i = 0; i < arrSize - 1; i++)
    {
        bool swaped = false;

        for (int j = 0; j < arrSize - 1 - i; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                swap(arr[j], arr[j + 1]);

                swaped = true;

                visualize(j + 1, j, arrSize - i);
            }
            SDL_Delay(1);
        }
        // Condition to check if the array is already sorted to stop further iterations.
        if (swaped == false)
        {
            break;
        }
    }
}

void selectionSort()
{
    for (int i = 0; i < arrSize - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < arrSize; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
                visualize(i, minIndex);
            }
            SDL_Delay(1);
        }
        swap(arr[i], arr[minIndex]);
    }
}

void loadArr()
{
    memcpy(arr, Barr, sizeof(int) * arrSize);
}

void randomizeAndSaveArray()
{
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);
    for (int i = 0; i < arrSize; i++)
    {
        int random = rand() % (SCREEN_HEIGHT);
        Barr[i] = random;
    }
}

void execute()
{
    if (!init())
    {
        cout << "SDL Initialization Failed.\n";
    }
    else
    {
        randomizeAndSaveArray();
        loadArr();

        SDL_Event e;
        bool quit = false;
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    complete = false;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                    case (SDLK_q):
                        quit = true;
                        complete = false;
                        cout << "\nEXITING SORTING VISUALIZER.\n";
                        break;
                    case (SDLK_0):
                        randomizeAndSaveArray();
                        complete = false;
                        loadArr();
                        cout << "\nNEW RANDOM LIST GENERATED.\n";
                        break;
                    case (SDLK_1):
                        loadArr();
                        cout << "\nSELECTION SORT STARTED.\n";
                        complete = false;
                        selectionSort();
                        complete = true;
                        cout << "\nSELECTION SORT COMPLETE.\n";
                        break;
                    case (SDLK_2):
                        loadArr();
                        cout << "\nINSERTION SORT STARTED.\n";
                        complete = false;
                        insertionSort();
                        complete = true;
                        cout << "\nINSERTION SORT COMPLETE.\n";
                        break;
                    case (SDLK_3):
                        loadArr();
                        cout << "\nBUBBLE SORT STARTED.\n";
                        complete = false;
                        bubbleSort();
                        complete = true;
                        cout << "\nBUBBLE SORT COMPLETE.\n";
                        break;
                    case (SDLK_4):
                        loadArr();
                        cout << "\nMERGE SORT STARTED.\n";
                        complete = false;
                        mergeSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nMERGE SORT COMPLETE.\n";
                        break;
                    case (SDLK_5):
                        loadArr();
                        cout << "\nQUICK SORT STARTED.\n";
                        complete = false;
                        quickSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nQUICK SORT COMPLETE.\n";
                        break;
                    case (SDLK_6):
                        loadArr();
                        cout << "\nHEAP SORT STARTED.\n";
                        complete = false;
                        heapSort(arr, arrSize);
                        complete = true;
                        cout << "\nHEAP SORT COMPLETE.\n";
                        break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controls()
{
    cout
        << " =============================================================================\n"
        << " = WARNING: Giving repetitive commands may cause latency and the visualizer  =\n"
        << " = may behave unexpectedly. Please give a new command only after the current =\n"
        << " = command's execution is done.                                              =\n"
        << " =============================================================================\n\n"
        << " Available Controls inside Sorting Visualizer:\n"
        << "    Use 0 to Generate a different randomized list.\n"
        << "    Use 1 to start Selection Sort Algorithm.\n"
        << "    Use 2 to start Insertion Sort Algorithm.\n"
        << "    Use 3 to start Bubble Sort Algorithm.\n"
        << "    Use 4 to start Merge Sort Algorithm.\n"
        << "    Use 5 to start Quick Sort Algorithm.\n"
        << "    Use 6 to start Heap Sort Algorithm.\n"
        << "    Use q to exit out of Sorting Visualizer.\n\n"
        << " PRESS ENTER TO START SORTING VISUALIZER...\n\n"
        << " OR type -1 and press ENTER to quit the program.";

    string s;
    getline(cin, s);

    // Check if the user wants to quit the program
    if (s == "-1")
    {
        return false;
    }

    // If the user presses ENTER without any input, start the sorting visualizer
    if (s.empty())
    {
        return true;
    }

    // For any other input, also start the sorting visualizer (as per the original intent)
    return true;
}

void intro()
{
    cout << "============================== Sorting Visualizer ==============================\n\n"
         << " Visualization of different sorting algorithms in C++ with the SDL2 Library.\n"
         << " A sorting algorithm arranges the elements of a list in a certain order. While\n"
         << " many sorting algorithms exist, a few predominate in practical implementations.\n"
         << " In this sorting visualizer, we will look at some of these algorithms and visually\n"
         << " comprehend their workings.\n\n"

         << " The sorting algorithms covered here are:\n"
         << " - Selection Sort\n"
         << " - Insertion Sort\n"
         << " - Bubble Sort\n"
         << " - Merge Sort\n"
         << " - Quick Sort\n"
         << " - Heap Sort\n\n"

         << " The list size is fixed at 130 elements. You can randomize the list and select\n"
         << " any type of sorting algorithm to apply from the given options. All sorting\n"
         << " algorithms will sort the elements in ascending order. Note that the sorting\n"
         << " time visualized for an algorithm is not exactly the same as their actual time\n"
         << " complexities. The relatively faster algorithms like Merge Sort have been delayed\n"
         << " so they can be properly visualized.\n\n"

         << " Press ENTER to show controls...";

    string s;
    getline(cin, s);
    if (s == "\n")
    {
        return;
    }
}

int main(int argc, char *args[])
{
    intro();

    while (1)
    {
        cout << '\n';
        if (controls())
            execute();
        else
        {
            cout << "\nEXITING PROGRAM.\n";
            break;
        }
    }

    return 0;
}
