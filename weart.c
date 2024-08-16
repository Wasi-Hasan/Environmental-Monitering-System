#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

#define API_KEY "4c52ab63a1f5c4ebe2aadc3d32809aad"
#define CITY_NAME "karachi"

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Function to write data to a text file
void writeToTextFile(const char *filename, const char *data) {
    FILE *file = fopen(filename, "w");  // Open file in write mode
    if (file != NULL) {
        fprintf(file, "%s\n", data);
        fclose(file);
        printf("Data written to %s\n", filename);
    } else {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
    }
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return 0; // Abort the transfer
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void fetchWeatherData() {
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        char url[256];
        sprintf(url, "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s", CITY_NAME, API_KEY);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            cJSON *json = cJSON_Parse(chunk.memory);
            if (json != NULL) {
                cJSON *mainObj = cJSON_GetObjectItem(json, "main");
                cJSON *feelsLikeObj = cJSON_GetObjectItem(mainObj, "feels_like");
                const char *cityName = cJSON_GetObjectItem(json, "name")->valuestring;

                if (mainObj != NULL && feelsLikeObj != NULL) {
                    // Temperature in Kelvin
                    double temperatureKelvin = cJSON_GetObjectItem(mainObj, "temp")->valuedouble;
                    double feelsLikeKelvin = feelsLikeObj->valuedouble;

                    // Convert Kelvin to Celsius
                    double temperatureCelsius = temperatureKelvin - 273.15;
                    double feelsLikeCelsius = feelsLikeKelvin - 273.15;

                    // Create a string with the data
                    char dataString[100];
                    snprintf(dataString, sizeof(dataString), "City: %s\nTemperature: %.2f°C\nFeels Like: %.2f°C", cityName, temperatureCelsius, feelsLikeCelsius);

                    // Write data to text file
                    writeToTextFile("weather_data.txt", dataString);
                } else {
                    fprintf(stderr, "Error parsing JSON: Missing required fields\n");
                }

                cJSON_Delete(json);
            } else {
                fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
            }
        }

        curl_easy_cleanup(curl);
        free(chunk.memory);
    }

    curl_global_cleanup();
}

int main() {
    fetchWeatherData();
    return 0;
}
