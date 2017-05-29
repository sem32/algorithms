

void merge(int* A, int start, int end, int mid)
{
    int* memory = new int[end - start + 1];

    int i = start;
    int j = mid + 1;
    int k = 0;

    // Test special cases
    if (A[mid] < A[mid + 1])
    {
        delete [] memory;
        return;
    }

    while ((i <= mid) && (j <= end))
    {
        if (A[i] < A[j])
        {
            memory[k++] = A[i];
            i = i + 1;
        }
        else
        {
            memory[k++]= A[j];
            j = j + 1;
        }
    }

    while (i <= mid)
    {
        memory[k++] = A[i];
        i = i + 1;
    }

    while (j <= end)
    {
        memory[k++] = A[j];
        j = j + 1;
    }

    // Copy result to A. K - length of
    for (i = 0; i < k; i++)
    {
        A[start + i] = memory[i];
    }

    delete [] memory;
}

void mergeSort(int* A, int start, int end)
{
    int mid = (end + start) / 2;

    if (end > start)
    {
        mergeSort(A, start, mid);
        mergeSort(A, mid + 1, end);
        merge(A, start, end, mid);
    }
    return;
}

void mergeSortBU(int* A, int length)
{
    int sectionLength = 1;

    while (sectionLength < length)
    {
        for (int k = 0; k < length / sectionLength; k = k + 2)
        {
            merge(A, k*sectionLength, (k+2)*sectionLength - 1, (k+1)*sectionLength - 1);
        }

        sectionLength = sectionLength * 2;
    }
}
