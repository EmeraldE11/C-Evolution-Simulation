#include <iostream>
#include <list>
#include <algorithm>
#include <random>
#include <string>
#include "creature.h"

using namespace std;

// Function prototypes
int randomNumber(int min, int max);
void Day(list<Creature>& population, int& food_left, int init_food_supply, int& dailyBabies, int& dailyMutations, int& diedToday);
void displayInfoAndAverages(list<Creature>& population, int totalDays, int totalBabies, int totalMutations, int totalDeaths);

int main() {
    srand(time(0));

    int population_size;
    cout << "Enter population size: ";
    cin >> population_size;

    int init_food_supply;
    cout << "Enter initial food supply: ";
    cin >> init_food_supply;

    int days;
    cout << "How many days do you want the simulation to run for? ";
    cin >> days;

    if (population_size <= 0 || init_food_supply <= 0 || days <= 0) {
        cerr << "Invalid input. Population size, initial food supply, and days must be positive integers." << endl;
        return 1;
    }

    list<Creature> population;
    for (int i = 0; i < population_size; ++i) {
        Creature creature(randomNumber(1, 10), randomNumber(1, 10), 100, 0.05, 0); // Random initial values for speed and foraging
        population.push_back(creature);
    }

    int food_left = init_food_supply;
    int totalBabies = 0;
    int totalMutations = 0;
    int totalDeaths = 0;

    for (int d = 0; d < days; ++d) {
        cout << "Day " << d + 1 << ": Starting with " << population.size() << " creatures." << endl;

        food_left = init_food_supply; // Reset the food supply at the start of each day

        int dailyBabies = 0;
        int dailyMutations = 0;
        int diedToday = 0;
        Day(population, food_left, init_food_supply, dailyBabies, dailyMutations, diedToday);

        totalBabies += dailyBabies;
        totalMutations += dailyMutations;
        totalDeaths += diedToday;

        cout << "Day " << d + 1 << ": Ending with " << population.size() << " creatures." << endl;
        cout << "Died today: " << diedToday << ", Babies born today: " << dailyBabies << ", Mutations today: " << dailyMutations << endl;
    }

    // Display data
    displayInfoAndAverages(population, days, totalBabies, totalMutations, totalDeaths);

    return 0;
}

int randomNumber(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

void Day(list<Creature>& population, int& food_left, int init_food_supply, int& dailyBabies, int& dailyMutations, int& diedToday) {
    population.sort([](const Creature& a, const Creature& b) {
        return a.getSpeed() > b.getSpeed();
        });

    list<Creature> newBabies;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (auto it = population.begin(); it != population.end();) {
        if (food_left > 0) {
            int foodFound = it->foodSearch(food_left, init_food_supply);
            if (foodFound >= 1) {
                it->setEnergy(100);
                it->daySurvived();
                if (foodFound == 2) {
                    Creature baby(it->getForaging(), it->getSpeed(), 100, it->getMutationChance(), 0);

                    // Check for mutation chance
                    if (dis(gen) < it->getMutationChance()) {
                        baby.mutateStats();
                        dailyMutations++;
                    }

                    newBabies.push_back(baby);
                    dailyBabies++;
                }
            }
            else {
                it = population.erase(it);
                diedToday++;
                continue; // Skip to the next iteration after erasing
            }
        }
        else {
            it = population.erase(it);
            diedToday++;
            continue; // Skip to the next iteration after erasing
        }
        ++it;
    }

    // Add new babies to the population after processing the initial population
    population.insert(population.end(), newBabies.begin(), newBabies.end());
}

void displayInfoAndAverages(list<Creature>& population, int totalDays, int totalBabies, int totalMutations, int totalDeaths) {
    float totalForaging = 0.0;
    float totalSpeed = 0.0;
    int totalDaysSurvived = 0;

    for (const auto& creature : population) {
        totalForaging += creature.getForaging();
        totalSpeed += creature.getSpeed();
        totalDaysSurvived += creature.getDaysSurvived();
    }

    int populationSize = population.size();
    float averageForaging = populationSize ? totalForaging / populationSize : 0;
    float averageSpeed = populationSize ? totalSpeed / populationSize : 0;
    float averageDaysSurvived = populationSize ? static_cast<float>(totalDaysSurvived) / populationSize : 0;

    cout << "Average foraging: " << averageForaging << endl;
    cout << "Average speed: " << averageSpeed << endl;
    cout << "Average days survived: " << averageDaysSurvived << endl;
    cout << "Total babies born: " << totalBabies << " (Average per day: " << static_cast<float>(totalBabies) / totalDays << ")" << endl;
    cout << "Total mutations: " << totalMutations << " (Average per day: " << static_cast<float>(totalMutations) / totalDays << ")" << endl;
    cout << "Total deaths: " << totalDeaths << " (Average per day: " << static_cast<float>(totalDeaths) / totalDays << ")" << endl;
}
