#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Google_Sheet_Client.h>
#include <Arduino_JSON.h>
#include <GS_SDHelper.h>

#define WIFI_SSID "San's Mobile"
#define WIFI_PASSWORD "791384265"

// Google Project ID
#define PROJECT_ID "streetlightmoniteringsystem"

// Service Account's client email
#define CLIENT_EMAIL "streetlightmonieringsystem@streetlightmoniteringsystem.iam.gserviceaccount.com"

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDBz7s9u6tz2bEN\n9sAbn5ZWFj85LRp7c1HAGJXlLkrgQW6C3MAEmaigDm8ulzmPVPBafnXenSjG9+Ya\nDGyOJ8PCOc/VjBO8HgPlkwKBLhrbGctEYkaXa4g78N3rH+cAqjqxBFVMSX3CLkKI\ny/3H3YHqqoEcH0TCqPBz9rTe6U9xrIG6jJXWFucBQ0TDXtSKqZLX0q8dVlV7JUQs\nDqWklLu3rCCnIEq12imMSCy1HyoH0ieavajg1z2ANt4t/GNF0ttAnjNUiEwb+zii\naSHN/aPuABWxR9Ot9XSr2LbcpKpWwkHAcpj2OPugAywJsxipwvE+AqPfOCZ3Hg62\nrDvve90XAgMBAAECggEAAd8SA8Quh6CjR2s86tQqDQxYV+9jjhJmLuCnfqsC9hKS\nEP/PETxNyh3ZkC2abkcRR9s1ib/vh0KyUHx5bKkqbrmrYN7iLrLR5TZiLWhW9T6h\ntwTr5/jJCMS7bJ9f1gkB80U2pAhU1CDDBU2GI2PnWTvqoaB7qki1tnW8KvRw9YjP\nxR1eYqWeq1uUgMKLgEYpz0zRBUVHNORGaqZrrpb/oZ1i2p44/iQsKfAfNWm8XgI8\n5Lm2dkLYW1u0TkkoM70Bclxgq2FXAUATQ0L1J9fi0zJXf10Tqgpl75O1t/rGNwZn\nUg7vNXw2xzwVEDFWav/QkTP3bqlAWIkfMj5hxm2eQQKBgQDtxUgbODvY9b5svxOJ\nxOyN4Xjc8/Wo3ymjwPYfXm9aDd7xr1YHG6NzR9G0JNXGhjjZ+C/UeIZ+bb93RQux\nHVVEIz3eMiIGFZ70QnaAQZJ9ZUUwHuLJtxa9idYV4lyWTYcjFECRDeuL6j09StPB\nvG1DeHNQoAccF84JANeGQxiZqQKBgQDQq6n7GIN2glsmhiK7Ot7OUn87y5AOcSX6\n00sKO//pWCfv2zKiSC9Ofu0bYZ+6WGnZOFD9hTzqhQK0GjBq6yhRJBT+LERMGNtA\nHLP/47QvUeLoxADHpr5cXJIf+HbEBCBHlpH2la8RwOoPUnPONt5uQAD9Y89hCE6D\nN0vNS4p4vwKBgQC6p7zLYYHZ+FDdHcNAzA7H8tDm471eLJKLsteEGKZhk7IJGRP3\n0IKn5SGCwcfwaT4IHwWtTlCGv7Em+FUNXXGqY+AVplV/YUm/hs1wMgnF1tJRzG1M\n3SZE9QfTdjgo5gPITgXB2aOn2uLoUu2ZkUpVRSGBkZ8DNszP1JFdHR8awQKBgDDP\nlulXaJj2ESBHzCQDt06H2Z9BFKdsLDgp6KmyM0CAEId4vLVdMUOpSQlYoqZSPCBF\nnhxPaFprVqCOZ7j63OluCUMTU76BKfgrPqeiuek/vFY6UvxgsrKkLMIxuoZLeMmG\nHQqTJOqAN7/xx4j7KLidXB3plP7blJH5A1A4VWG9AoGAXpqPQuHaGWxrNRErM18G\nmN+IGhp2oKazpbub9oPtSGKvi/+AERrHfTu8hJmUvcWEADvMHdIiapTJ6XpyyKW0\nZNjB2KEAUcQFs5AtHRqn6InmJH0yTUYHu3N1c9H3nN0atIKaZVcpS0SFj6iWNBIO\nCQ9Xv9sW2pGb1iig7lXNMrg=\n-----END PRIVATE KEY-----\n";

// The ID of the spreadsheet where you'll publish the data
const char spreadsheetId[] = "15MYQLyqnlEffft9C7YfZ3IEASufB59uTRjh2ndykbKU";

void tokenStatusCallback(TokenInfo info);


void setup(){

    Serial.begin(115200);
    Serial.println();
    GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

    // Connect to Wi-Fi
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();


    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
    GSheet.setPrerefreshSeconds(10 * 60);

    // Begin the access token generation for Google API authentication
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
}

void loop(){
       writeToGoogleSheet("Sheet1!C15","Hi");
       delay(1000);
       writeToGoogleSheet("Sheet1!C15","Updated");
       readFromGooglSheet("Sheet1!A1");

}


void readFromGooglSheet(String Range){
        FirebaseJson response;
        Serial.println("\nGet spreadsheet by data filter...");
        Serial.println("----------------------------------");
        FirebaseJsonArray dataFiltersArr;
        FirebaseJson dataFilters1;
        dataFilters1.add("a1Range", Range);
        dataFiltersArr.add(dataFilters1);
        GSheet.getByDataFilter(&response, spreadsheetId,&dataFiltersArr , "true");
        // JSONVar myObject  = JSON.parse(Data);
        // String getData = myObject["sheets"]["stringValue"];

        //Understand the format of the output from google sheet 
        //library for Parsing 
        // Get the value and convert t string 
        
}

void writeToGoogleSheet(String SheetData,String Data){
       FirebaseJson response;

        Serial.println("\nAppend spreadsheet values...");
        Serial.println("----------------------------");
        FirebaseJson valueRange;
        valueRange.add("majorDimension", "COLUMNS");
        valueRange.set("values/[0]/[0]", Data);
        GSheet.values.update(&response , spreadsheetId , SheetData , &valueRange );
        //response.toString(Serial, true);
        valueRange.clear();
}

