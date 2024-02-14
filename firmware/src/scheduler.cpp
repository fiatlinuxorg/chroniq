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

#include <Arduino.h>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Constructor for the sched_task struct
 *
 * @param task The task to be run
 * @param hour The hour at which the task should be run
 * @param minute The minute at which the task should be run
 */
sched_task::sched_task(void (*task)(void), int hour, int minute) :
    task(task), hour(hour), minute(minute) { /* Empty */ }

/**
 * Run the task
 */
void sched_task::run() {
    task();
}

/**
 * Check if the task should be run at the given time
 *
 * @param hour The hour to check
 * @param minute The minute to check
 * @return True if the task should be run at the given time, false otherwise
 */
bool sched_task::same_time(int hour, int minute) {
    return this->hour == hour && this->minute == minute;
}

/**
 * Constructor for the Scheduler class
 *
 * @param interval The interval at which the tasks should be checked
 */
Scheduler::Scheduler(unsigned long interval) {
    last_check = millis();
    check_interval = interval;
}

/**
 * Add a task to the scheduler
 *
 * @param task The task to be added
 */
void Scheduler::add_task(sched_task task) {
    tasks.push_back(task);
}

/**
 * Check the tasks and run them if they are due
 */
void Scheduler::check_tasks() {
    if (last_check + check_interval > millis()) {
        return;
    }

    Clock clock = Clock::get_instance();
    int hour, minute;

    hour = clock.get_hours();
    minute = clock.get_minutes();
    last_check = millis();

    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].same_time(hour, minute)) {
            tasks[i].run();
        }
    }
}
