#include "creature.h"
#include <random>
#include <iostream>

using namespace std;

Creature::Creature(float foraging, float speed, int energy, float mutationChance, int daysSurvived)
    : foraging(foraging), speed(speed), energy(energy), mutationChance(mutationChance), daysSurvived(daysSurvived) {}

float Creature::getForaging() const {
    return foraging;
}

float Creature::getSpeed() const {
    return speed;
}

int Creature::getEnergy() const {
    return energy;
}

float Creature::getMutationChance() const {
    return mutationChance;
}

int Creature::getDaysSurvived() const {
    return daysSurvived;
}

void Creature::daySurvived() {
    daysSurvived++;
}

void Creature::setForaging(float f) {
    foraging = f;
}

void Creature::setSpeed(float s) {
    speed = s;
}

void Creature::setEnergy(int e) {
    energy = e;
}

int Creature::foodSearch(int& food_left, int initialFoodSupply) {
    int foodFound = 0;
    float prob1 = (static_cast<float>(food_left) / initialFoodSupply) * foraging * (static_cast<float>(energy) / 100.0f);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    if (dis(gen) < prob1) {
        foodFound++;
        food_left--;
    }

    // Search again with half the energy
    float prob2 = (static_cast<float>(food_left) / initialFoodSupply) * foraging * (static_cast<float>(energy) / 200.0f);

    if (dis(gen) < prob2) {
        foodFound++;
        food_left--;
    }

    return foodFound;
}

void Creature::mutateStats() {
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> distributionSpeed(speed, 1.0f); // Ensure float precision
    normal_distribution<float> distributionForaging(foraging, 1.0f); // Ensure float precision

    speed = max(0.0f, distributionSpeed(gen)); // Speed should not be negative
    foraging = max(0.0f, distributionForaging(gen)); // Foraging should not be negative
}

void Creature::displayStats() const {
    cout << "Foraging: " << foraging << ", Speed: " << speed
        << ", Energy: " << energy << ", Days Survived: " << daysSurvived << endl;
}
