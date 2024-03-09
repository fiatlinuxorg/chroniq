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

enum task_action_e {
    TASK_ACTION_BACKLIGHT,
    TASK_ACTION_NO_BACKLIGHT,
    TASK_ACTION_SET_VIEW
};

/**
 * @brief Struct to hold a scheduled task
 */
struct sched_task_t {
    task_action_e action;
    int hours, minutes;
    void* arg;

    sched_task_t(task_action_e action, int hours, int minutes, void* arg=nullptr);
    bool same_time(int hours, int minutes);
    void run();
};

/**
 * @bief Scheduler class
 */
class Scheduler {
    public:
        Scheduler(unsigned long interval);
        Scheduler(unsigned long interval, vector<sched_task_t> tasks);
        void add_task(sched_task_t task);
        void check_tasks();

    private:
        unsigned long last_check, check_interval;
        vector<sched_task_t> tasks;
};

/**
 * @biref reference to the current view index
 */
extern int current_view_idx;

/**
 * @brief Set the current view
 *
 * @param idx The index of the view to set
 */
static void set_view(int idx);


#endif // __SCHEDULER_H__
