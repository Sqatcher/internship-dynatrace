#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <date/date.h>
#include <nlohmann/json.hpp>

#include "internship.h"

using json = nlohmann::json;
using namespace date;
using namespace std::chrono;

// class used to hold only the most important OSes
class Holder {
public:
    int* bestScores;
    std::vector<std::string> bestScoresames;
    std::vector<std::string> bestSystems;
    int maxsize;

    Holder(int size)
    {
        maxsize = size;
        bestScores = new int[maxsize];
        for (int i = 0; i < maxsize; i++)
        {
            bestScores[i] = 0;
            bestScoresames.push_back("");
            bestSystems.push_back("");
        }
    }

    // insert the new contender at the right spot
    void addBest(int contender, std::string name, std::string system)
    {
        if (bestScores[0] >= contender)
            return;

        int index = 0;
        bestScores[index] = contender;
        for (index = 1; index < maxsize; index++)
        {
            if (bestScores[index] > contender)
            {
                addToBestNames(name, system, index - 1);    //index-1 since names are added after the right place has been found
                return;
            }
            bestScores[index - 1] = bestScores[index];
            bestScores[index] = contender;
        }
        addToBestNames(name, system, index - 1);          //index-1 - same as above
    }

    void printScores()
    {
        int i;
        for (i = maxsize-1; i >= 0; i--)
        {
            if (bestScores[i] == 0) break;
            if (i != maxsize - 1)             // as far as I understand, trailing endline is not allowed
            {
                std::cout << std::endl;
            }
            std::cout << bestSystems[i] << " " << bestScoresames[i] << " " << bestScores[i];
        }
    }

    ~Holder()
    {
        delete[] bestScores;
    }

    void addToBestNames(std::string name, std::string system, int index)
    {
        std::vector<std::string> names1;
        std::vector<std::string> names2;
        int flag = 1;                       // flag - offset until after the new element is inserted
        for (int i = 0; i < maxsize; i++)
        {
            if (i == index)
            {
                names1.push_back(system);
                names2.push_back(name);
                flag = 0;
                continue;
            }
            names1.push_back(bestSystems[i+flag]);
            names2.push_back(bestScoresames[i+flag]);
        }
        
        bestSystems = names1;
        bestScoresames = names2;
    }
};

namespace internship {
    // simple date validation
    int checkDate(int year, int month, int day)
    {
        if (year < 1492) return 0;
        if (month < 1 || month > 12) return 0;
        if (day < 1 || day > 31) return 0;
        return 1;
    }

    void my_solution(const std::string& jsonFileName, int elementsCount)
    {
        if (elementsCount <= 0) return;
        std::ifstream f(jsonFileName);
        json data = json::parse(f);

        Holder holder(elementsCount);

        for (const auto& [id, product] : data.items())
        {
            if (!product.contains(std::string{ "os" })) continue;
            if (product["os"] == false)                                   // if not os - dont bother
                continue;

            int localBest = 0;
            std::string bestName = "";
            for (const auto& osVersion : product["versions"])
            {
                // read important dates
                if (!osVersion.contains(std::string{ "releaseDate" }) || !osVersion["releaseDate"].is_string())
                {
                    continue;
                }
                std::string releaseDate = osVersion["releaseDate"];
                int yearR;
                int monthR;
                int dayR;
                char delim;
                sscanf(releaseDate.c_str(), "%4d%c%2d%c%2d", &yearR, &delim, &monthR, &delim, &dayR);
                if (!checkDate(yearR, monthR, dayR)) continue;

                if (!osVersion.contains(std::string{ "eol" }) || !osVersion["eol"].is_string())
                {
                    continue;
                }
                std::string endDate = osVersion["eol"];
                int yearE;
                int monthE;
                int dayE;
                sscanf(endDate.c_str(), "%4d%c%2d%c%2d", &yearE, &delim, &monthE, &delim, &dayE);
                if (!checkDate(yearE, monthE, dayE)) continue;

                // count day difference
                auto x = year{ yearR } / monthR / dayR;
                auto y = year{ yearE } / monthE / dayE;
                int diff = (sys_days{ y } - sys_days{ x }).count() + 1;

                // store only the best score for the OS
                if (localBest < diff)
                {
                    localBest = diff;
                    if (!osVersion.contains(std::string{ "cycle" })) continue;
                    bestName = osVersion["cycle"];
                }
            }
            if (!product.contains(std::string{ "name" })) continue;
            holder.addBest(localBest, bestName, (std::string)product["name"]);
        }

        holder.printScores();
    }

    // do not remove this function
    void solution(const std::string& jsonFileName, int elementsCount) {
        my_solution(jsonFileName, elementsCount); // put the call to your solution here
    }
}