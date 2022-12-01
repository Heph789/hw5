

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    DailySchedule& sched,
    size_t shiftsUsed[],
    bool * scheduledTodayMtrx[],
    size_t counter
);
bool isValid(size_t day, size_t worker, const AvailabilityMatrix& avail, const size_t shiftsUsed[], bool * scheduledTodayMtrx[]);
void printSchedules(const DailySchedule& sched);
void resetDay(bool scheduledToday[], size_t totalWorkers);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    const size_t totalWorkers = avail[0].size();
    const size_t totalDays = avail.size();

    // populate sched w/ invalids
    for (size_t day = 0; day < totalDays; ++day)
    {
        sched.push_back(vector<Worker_T>(dailyNeed));
    }
    // printSchedules(sched);

    // create an array to keep track of shifts already used
    size_t shiftsUsed[totalWorkers];
    for (size_t i = 0; i < totalWorkers; ++i) {
        shiftsUsed[i] = maxShifts;
    }

    bool * scheduledTodayMtrx[totalDays]; // keeps track of whether a worker has already been scheduled today, for every day
    for (size_t i = 0; i < totalDays; ++i) {
        scheduledTodayMtrx[i] = new bool[totalWorkers];
        resetDay(scheduledTodayMtrx[i], totalWorkers);
    }

    bool ret = scheduleHelper(avail, dailyNeed, sched, shiftsUsed, scheduledTodayMtrx, 0);
    for (size_t i = 0; i < totalDays; ++i) {
        delete scheduledTodayMtrx[i];
    }
    return ret;
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    DailySchedule& sched,
    size_t shiftsUsed[],
    bool* scheduledTodayMtrx[],
    size_t counter
)
{
    const size_t totalWorkers = avail[0].size();
    const size_t totalDays = sched.size();

    if (counter == totalDays*dailyNeed) return true; // Made it to the end without returning false

    // if (counter == 2 && sched[0][1] == 1) cout << "Pos 2" << endl;
    // if (counter == 3 && sched[1][0] == 2) cout << "Pos 3" << endl;

    const size_t day = counter / dailyNeed;
    const size_t slot = counter % dailyNeed;
    // cout << "Day: " << day << "; Slot: " << slot << endl;

    if (slot == 0) resetDay(scheduledTodayMtrx[day], totalWorkers);

    for (size_t i = 0; i < totalWorkers; ++i)
    {
        if (isValid(day, i, avail, shiftsUsed, scheduledTodayMtrx)) {
            --shiftsUsed[i];
            scheduledTodayMtrx[day][i] = true;
            sched[day][slot] = i;
            if (scheduleHelper(avail, dailyNeed, sched, shiftsUsed, scheduledTodayMtrx, counter+1)) return true;
            ++shiftsUsed[i]; // reset variables to keep track of workers availability
            scheduledTodayMtrx[day][i] = false;
        }
    }
    return false; // no valid options
}

bool isValid(size_t day, size_t worker, const AvailabilityMatrix& avail, const size_t shiftsUsed[], bool * scheduledTodayMtrx[]) {
    if (shiftsUsed[worker] == 0) return false; // worker must have available shifts
    if (avail[day][worker] == false) return false; // worker must have availability on that day
    if (scheduledTodayMtrx[day][worker] == true) return false;
    return true;
}

void resetDay(bool scheduledToday[], size_t totalWorkers)
{
    for (size_t i = 0; i < totalWorkers; ++i)
    {
        scheduledToday[i] = false;
    }
}

void printSchedules(const DailySchedule& sched)
{
    int day = 0;
    for(auto s : sched)
    {
        cout << "Day " << day << ": ";
        for(auto nurse : s)
        {
            cout << nurse << " ";
        }
        cout << endl;
        day++;
    }
}