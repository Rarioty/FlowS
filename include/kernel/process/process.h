#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <kernel/utils/registers.h>
#include <stddef.h>

/**
 * \file    process.h
 * \brief   This file handle the process structure and loading function
 */

#define MAX_PID     32                      /*!<    Maximum number of PIDs          */

/**
 * \struct  process
 * \brief   This structure handle a process in the OS
 */
struct process
{
    uint32_t    pid;                        /*!<    Process ID                                                                  */
    process_registers regs;                 /*!<    Process registers state                                                     */

    // User stack
    struct
    {
        uint32_t    esp0;                   /*!<    Kernel esp                                                                  */
        uint16_t    ss0;                    /*!<    Kernel stack                                                                */
    } kernel_stack __attribute__((packed));

    struct page_directory* pd;              /*!<    Process page directory                                                      */
    struct page_list* pglist;               /*!<    Process page list                                                           */
    int state;                              /*!<    States of the process: 0 -> not used, 1 -> ready/running, 2 -> sleeping     */
} __attribute__((packed));

#ifdef __PLIST__
struct process p_list[MAX_PID + 1];          /*!<    List of processes (32 max for now !)    */
struct process *current = 0;        /*!<    Current process executed                */
uint32_t n_proc = 0;                /*!<    Number of process ready                 */
#else
extern struct process p_list[];     /*!<    List of processes (32 max for now !)    */
extern struct process* current;     /*!<    Current process executed                */
extern uint32_t n_proc;             /*!<    Number of process ready                 */
#endif

/**
 * \brief   Load a task into memory and ready it !
 *
 * \param[in] code_phys_addr    Physical address where we want to put the code
 * \param[in] fn                Address of the function to copy
 * \param[in] code_size         Size of the code we want to copy
 */
int load_task(char* fn, uint32_t code_size);

#endif
