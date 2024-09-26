#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include "cJSON.h"

long long decodeBase(const std::string &value, int base) {
    return std::strtoll(value.c_str(), nullptr, base);
}

double lagrangeInterpolation(int *x, long long *y, int k) {
    double c = 0.0;
    for (int i = 0; i < k; i++) {
        double term = y[i];
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (double)(-x[j]) / (x[i] - x[j]);
            }
        }
        c += term;
    }
    return c;
}

std::string readFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file." << std::endl;
        exit(1);
    }
    std::string data((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    return data;
}

int main() {
    std::string jsonString = readFile("input.json");
    cJSON *json = cJSON_Parse(jsonString.c_str());
    if (json == NULL) {
        std::cerr << "Error parsing JSON." << std::endl;
        exit(1);
    }

    cJSON *keys = cJSON_GetObjectItem(json, "keys");
    int n = cJSON_GetObjectItem(keys, "n")->valueint;
    int k = cJSON_GetObjectItem(keys, "k")->valueint;

    int *x_values = new int[k];
    long long *y_values = new long long[k];

    int count = 0;
    for (int i = 1; i <= n; i++) {
        char key[10];
        sprintf(key, "%d", i);
        cJSON *point = cJSON_GetObjectItem(json, key);
        if (point != NULL) {
            int base = std::atoi(cJSON_GetObjectItem(point, "base")->valuestring);
            std::string value = cJSON_GetObjectItem(point, "value")->valuestring;
            x_values[count] = i;
            y_values[count] = decodeBase(value, base);
            count++;
            if (count == k) {
                break;
            }
        }
    }

    double constant = lagrangeInterpolation(x_values, y_values, k);
    std::cout << "The constant term (c) is: " << constant << std::endl;

    delete[] x_values;
    delete[] y_values;
    cJSON_Delete(json);

    return 0;
}
