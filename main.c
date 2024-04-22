#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "database.db"

typedef struct Data
{
  int value;
  int id;
} Data;

void addToDatabase(Data *data, FILE *fptr);
void deleteFromDatabase(int id, FILE *fptr);
void printData(FILE *fptr);
int getTotalItems(FILE *fptr);

int main(void)
{
  // Variables
  FILE *fptr;
  Data *newData1, *newData2, *newData3;

  // Create new data
  newData1 = malloc(sizeof(Data) * 1);
  newData1->value = 10;
  newData1->id = 1;

  newData2 = malloc(sizeof(Data) * 1);
  newData2->value = 20;
  newData2->id = 2;

  newData3 = malloc(sizeof(Data) * 1);
  newData3->value = 30;
  newData3->id = 3;

  // Open the file
  fptr = fopen(FILE_NAME, "w+");

  // Manipulate the database
  addToDatabase(newData1, fptr);
  addToDatabase(newData2, fptr);
  addToDatabase(newData3, fptr);

  deleteFromDatabase(1, fptr);

  printData(fptr);

  // Close the file
  fclose(fptr);

  // Free data
  free(newData1);
  free(newData2);
  free(newData3);

  return 0;
}

int getTotalItems(FILE *fptr)
{
  int totalItems;

  fseek(fptr, 0, SEEK_END);
  totalItems = ftell(fptr) / sizeof(Data);
  fseek(fptr, 0, SEEK_SET);

  return totalItems;
}

void addToDatabase(Data *data, FILE *fptr)
{
  fwrite(data, sizeof(Data), 1, fptr);
}

void deleteFromDatabase(int id, FILE *fptr)
{
  Data *data, *newData = NULL;
  int totalItems, i, found = 0, j = 0;

  // Get the total number of items in the database
  totalItems = getTotalItems(fptr);

  if (totalItems == 0)
  {
    return;
  }

  // allocate memory for the data
  data = malloc(sizeof(Data) * totalItems);
  fread(data, sizeof(Data), totalItems, fptr);

  for (i = 0; i < totalItems; i++)
  {
    if (data[i].id == id)
    {
      found = 1;

      continue;
    }

    newData = realloc(newData, sizeof(Data) * ++j);
    newData[j - 1] = data[i];
  }

  if (found == 1)
  {
    // Re-open the file in write mode to clear existing data
    fptr = freopen(FILE_NAME, "w", fptr);

    // Write the new data
    fwrite(newData, sizeof(Data), totalItems - 1, fptr);
  }

  free(data);
  free(newData);
}

void printData(FILE *fptr)
{
  Data *data = NULL;
  int totalItems, i;

  // Re-open the file in read mode
  fptr = freopen(FILE_NAME, "r+", fptr);

  // Get the total number of items in the database
  totalItems = getTotalItems(fptr);

  if (totalItems == 0)
  {
    printf("There are no items in the database.\n");

    return;
  }

  data = malloc(sizeof(data) * totalItems);
  fread(data, sizeof(Data), totalItems, fptr);

  for (i = 0; i < totalItems; i++)
  {
    printf("[DATA]\nID: %d\nValue: %d\n\n", data[i].id, data[i].value);
  }

  // Free memory
  free(data);
}
