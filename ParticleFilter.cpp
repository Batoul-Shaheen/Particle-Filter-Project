
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

const int N = 100;
const int EnvSize = 80;

struct PFStruct {
    float x, y, theta;
    float w;
};

char Env[EnvSize][EnvSize];

void initEnv() {
    for (int i = 0; i < EnvSize; i++) {
        for (int j = 0; j < EnvSize; j++) {
            char squareValue = ((i / 10) + (j / 10)) % 2 == 0 ? 'B' : 'W';

            for (int k = 0; k < 10; k++) {
                for (int l = 0; l < 10; l++) {
                    if ((i + k) < EnvSize && (j + l) < EnvSize) {
                        Env[i + k][j + l] = squareValue;
                    }
                }
            }
        }
    }
}

void update(float& w, const PFStruct& U) {
    int gridX = static_cast<int>(U.x * EnvSize);
    int gridY = static_cast<int>(U.y * EnvSize);
    float Pdark = 0.7, Pwhite = 0.3;

    char gridValue = (gridX < EnvSize&& gridY < EnvSize) ? Env[gridX][gridY] : 'W';

    if (gridValue == 'B') {
        w *= Pdark;
    }
    else if (gridValue == 'W') {
        w *= (1 - Pdark);
    }
}

void normalize(PFStruct* S) {
    float sum = 0;
    for (int i = 0; i < N; i++) {
        sum += S[i].w;
    }

    for (int i = 0; i < N; i++) {
        S[i].w /= sum;
    }
}

void initi(PFStruct* S) {
    for (int i = 0; i < N; i++) {
        S[i].x = static_cast<float>(rand()) / RAND_MAX;
        S[i].y = static_cast<float>(rand()) / RAND_MAX;
        S[i].theta = static_cast<float>(rand()) / RAND_MAX * 360;
        S[i].w = 1.0 / N;
    }
}

int sample(const PFStruct S[]) {
    float rand_val = static_cast<float>(rand()) / RAND_MAX;
    float cumulative_prob = 0;

    for (int i = 0; i < N; i++) {
        cumulative_prob += S[i].w;
        if (rand_val <= cumulative_prob) {
            return i;
        }
    }

    return N - 1;
}

void moveForward(PFStruct& U, float numSteps) {
    U.x += numSteps * cos(U.theta * M_PI / 180);
    U.y += numSteps * sin(U.theta * M_PI / 180);
}

void rotate(float angle, PFStruct& U) {
    U.theta += angle;
    if (U.theta >= 360.0) {
        U.theta -= 360.0;
    }
}

void ParticleFilter(PFStruct S[], PFStruct& U, float angle, float displacement) {
    PFStruct NewS[N];

    for (int j = 0; j < N; j++) {
        int i = sample(S);
        moveForward(S[i], displacement);
        rotate(angle, S[i]);
        update(S[i].w, U);

        NewS[j] = S[i];
    }

    normalize(NewS);

    for (int i = 0; i < N; i++) {
        S[i] = NewS[i];
    }
}

void readCSVLine(const string& filename, int lineNumber, float& x, float& y, float& displacement, float& angle) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    for (int i = 1; i <= lineNumber; i++) {
        if (!getline(file, line)) {
            cout << "Error reading line " << lineNumber << " from file: " << filename << endl;
            return;
        }
        if (i == lineNumber) {
            istringstream iss(line);
            string value;
            int count = 0;
            while (getline(iss, value, ',')) {
                count++;
                switch (count) {
                case 1:
                    x = stof(value);
                    break;
                case 2:
                    y = stof(value);
                    break;
                case 3:
                    displacement = stof(value);
                    break;
                case 4:
                    angle = stof(value);
                    break;
                }
            }
            break;
        }
    }
    file.close();
}


void report(PFStruct S[], float robotX, float robotY) {
    float meanParticleX = 0, meanParticleY = 0, varParticleX = 0, varParticleY = 0;
    float sumX = 0, sumY = 0, sumVarX = 0, sumVarY = 0;

    for (int i = 0; i < N; i++) {
        sumX += S[i].x;
        sumY += S[i].y;
    }

    meanParticleX = sumX / (float)N;
    meanParticleY = sumY / (float)N;

    for (int i = 0; i < N; i++) {
        sumVarX += (S[i].x - meanParticleX) * (S[i].x - meanParticleX);
        sumVarY += (S[i].y - meanParticleY) * (S[i].y - meanParticleY);
    }

    varParticleX = sumVarX / (float)N;
    varParticleY = sumVarY / (float)N;

    cout << "Robot: " << robotX << " " << robotY << " ";
    cout << "Particles: " << meanParticleX << " " << meanParticleY << " " << varParticleX << " " << varParticleY << endl;
}

int main() {

    PFStruct S[N];
    initi(S);
    initEnv();

    for (int step = 0; step < 500; step++) {
        float robotX, robotY, displacement, angle;
        readCSVLine("C:/Users/DELL/Downloads/robot.csv", step + 2, robotX, robotY, displacement, angle);

        PFStruct U = { robotX, robotY, displacement,angle };
        ParticleFilter(S, U, displacement, angle);
        report(S, robotX, robotY);
    }

    return 0;
}