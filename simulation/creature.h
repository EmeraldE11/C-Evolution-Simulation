#pragma once

using namespace std;

class Creature {
private:
    float foraging; // % chance to find food
    float speed; // Determines order that creatures get chance to find food
    int energy; // How much energy left until death

    float mutationChance; // Chance to mutate when replicating, value never changes

    int daysSurvived;

public:
    Creature(float foraging, float speed, int energy, float mutationChance, int daysSurvived);

    // Getters
    float getForaging() const;
    float getSpeed() const;
    int getEnergy() const;
    float getMutationChance() const;
    int getDaysSurvived() const;

    // Setters
    void setForaging(float f);
    void setSpeed(float s);
    void setEnergy(int e);

    void daySurvived();

    int foodSearch(int& food_left, int initialFoodSupply);

    // Mutation function
    void mutateStats();

    // Display function
    void displayStats() const;
};