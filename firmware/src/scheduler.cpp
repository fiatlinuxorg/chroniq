/**
 * Scheduler class implementation - scheduler.cpp
 *
 * This class is used to schedule tasks to be run at a certain interval.
 *
 * Author:  Mirko Lana
 * Date:    13/02/2024
 */

#include "scheduler.h"
#include "clock.h"
#include "world.h"

#include <Arduino.h>
#include <iostream>
#include <vector>

using namespace std;

/**
 * @biref Constructor for the sched_task_t struct
 *
 * @param action The action to be performed
 * @param hours The hours at which the task should be run
 * @param minutes The minutes at which the task should be run
 * @param arg The argument to be passed to the task
 */
sched_task_t::sched_task_t(task_action_e action, int hours, int minutes,
        void* arg) : action(action), arg(arg) {
    this->hours = (hours < 24) ? hours : 0;
    this->minutes = (minutes < 60) ? minutes : 0;
}

/**
 * @brief Check if the task should be run at the given time
 *
 * @param hours The hours to check
 * @param minutes The minutes to check
 * @return True if the task should be run at the given time, false otherwise
 */
bool sched_task_t::same_time(int hours, int minutes) {
    return this->hours == hours && this->minutes == minutes;
}

/**
 * @biref Run the task
 */
void sched_task_t::run() {
    switch (action) {
        case TASK_ACTION_BACKLIGHT:
            set_backlight(true, true);
            break;
        case TASK_ACTION_NO_BACKLIGHT:
            set_backlight(false, true);
            break;
        case TASK_ACTION_SET_VIEW:
            set_view((int)arg);
            break;
    }
}

/**
 * @brief Constructor for the Scheduler class
 *
 * @param interval The interval at which the tasks should be checked
 */
Scheduler::Scheduler(unsigned long interval) : check_interval(interval) {
    last_check = millis();
}

Scheduler::Scheduler(unsigned long interval, vector<sched_task_t> tasks) 
    : Scheduler(interval) {
    this->tasks = tasks;
}

/**
 * @brief Add a task to the scheduler
 *
 * @param task The task to be added
 */
void Scheduler::add_task(sched_task_t task) {
    tasks.push_back(task);
}

/**
 * @biref Check the tasks and run them if they are due
 */
void Scheduler::check_tasks() {
    if (last_check + check_interval > millis()) {
        return;
    }

    Clock clock = Clock::get_instance();
    int hours, minutes;

    hours = clock.get_hours();
    minutes = clock.get_minutes();
    last_check = millis();

    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].same_time(hours, minutes)) {
            tasks[i].run();
        }
    }
}

int current_view_idx = 0;

/**
 * @brief Set the current view
 *
 * @param idx The index of the view to be set
 */
static void set_view(int idx) {
   current_view_idx = idx;
}

