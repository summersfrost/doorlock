#include <EEPROM.h>

#define MAX_COLLECTION_SIZE 10 // Maximum number of elements in the collection
#define EEPROM_ADDRESS_START 0 // Start address in EEPROM for storing data
#define ELEMENT_SIZE  sizeof(int) // Size of each element in the collection

int collection[MAX_COLLECTION_SIZE]; // Array to store collection data
int collectionSize = 0; // This will determine the storage which determines if the instructor is inside the faculty room or not
int allFacultyRFID[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // We will put the faculty rfid here so built in lang sya
int allFacultySize = sizeof(allFacultySize) / sizeof(allFacultyRFID[0]); // so size sa rfid sa faculty, gi dynamic lang ni sya para incase kanunay tag add di na nato ni usbon autodefine nalang

// Function to save data to EEPROM mao ni pagsave sa array para dili mawala
void saveDataToEEPROM(int data, int address) {
  EEPROM.put(address, data);
}

// Function to read data from EEPROM so tan awon nato kinsay sulod ana nga address kay nakastore man sya so iretrieve nato ni para sa comparison if mag add batag faculty kay nisulod or nigaswas 
int readDataFromEEPROM(int address) {
  int data;
  EEPROM.get(address, data);
  return data;
}

// Function to initialize the collection from EEPROM mao ni muload once marestart ang arduino
void initializeCollectionFromEEPROM() {
  int address = EEPROM_ADDRESS_START;
  for (int i = 0; i < MAX_COLLECTION_SIZE; i++) {
    int data = readDataFromEEPROM(address);
    if (data != -1) {
      collection[collectionSize++] = data;
    }
    address += ELEMENT_SIZE;
  }
}


// once nakasulod nag instructor isave nato siya 
void addToCollection(int data) {
  if (collectionSize < MAX_COLLECTION_SIZE) {
    collection[collectionSize++] = data;
    saveDataToEEPROM(data, EEPROM_ADDRESS_START + (collectionSize - 1) * ELEMENT_SIZE);
  }
}
//debugger rani icheck if nausob ba or nasave or naunsa ang memory basin nyag di mao ang result hahaha
void printCollection() {
  Serial.println("Current collection:");
  for (int i = 0; i < collectionSize; i++) {
    Serial.print(collection[i]);
    Serial.print(" ");
  }
  Serial.println();
}

// If niuli  di papha siya iremove na siyas collection
void removeFromCollection(int data) {
  for (int i = 0; i < collectionSize; i++) {
    if (collection[i] == data) {
      // Move elements after the removed one to fill the gap
      for (int j = i; j < collectionSize - 1; j++) {
        collection[j] = collection[j + 1];
        // Update EEPROM data for the shifted element
        saveDataToEEPROM(collection[j], EEPROM_ADDRESS_START + j * ELEMENT_SIZE);
      }
      // Clear the last element
      collection[collectionSize - 1] = -1;
      // Update EEPROM data for the last element
      saveDataToEEPROM(-1, EEPROM_ADDRESS_START + (collectionSize - 1) * ELEMENT_SIZE);
      collectionSize--;
      break;
    }
  }
}


// Mao ni mucheck if na ka login naba or nigawas
bool existsInCollection(int data) {
  for (int i = 0; i < collectionSize; i++) {
    if (collection[i] == data) {
      return true;
    }
  }
  return false;
}

// I check if naa bay sya permission makasulod or mugawas
bool existsInAllFacultyRFID(int data) {
  for (int i = 0; i < allFacultySize; i++) {
    if (allFacultyRFID[i] == data) {
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  initializeCollectionFromEEPROM();
}

void loop() {
  // Read user input from serial monitor
  if (Serial.available() > 0) {
    int inputData = Serial.parseInt(); // Read the integer input from the serial monitor

    // Check if input data is valid
    if (inputData != 0) {
      // Check if input data exists in the collection
      if(existsInAllFacultyRFID(inputData)){
      if (existsInCollection(inputData)) {
        // If it exists, remove it
        removeFromCollection(inputData);
        Serial.println("Data removed from collection");
          printCollection();
      } else {
        // If it doesn't exist, add it
        addToCollection(inputData);
        Serial.println("Data added to collection");
          printCollection();
      }}else{
        Serial.println("Data Doesnt Exist ");
      }
    } else {
      // Invalid input
      Serial.println("Invalid input. Please enter an integer.");
    }


  }

 // delay(1000); // Delay for stability
}

