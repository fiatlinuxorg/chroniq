/**
 * Scheduler class - scheduler.h
 *
 * This class is used to schedule tasks to be run at a certain interval.
 *
 * Author:  Mirko Lana
 * Date:    13/02/2024
 */

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <vector>

using namespace std;

struct sched_task {
    void (*task)(void);
    int hour, minute;

    sched_task(void (*task)(void), int hour, int minute);
    void run();
    bool same_time(int hour, int minute);
};

/**
 * Scheduler class
 */
class Scheduler {
    public:
        Scheduler(unsigned long interval);
        void add_task(sched_task task);
        void check_tasks();

    private:
        unsigned long last_check, check_interval;
        vector<sched_task> tasks;
};

#endif // __SCHEDULER_H__
