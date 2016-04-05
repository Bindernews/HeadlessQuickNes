#ifndef __HQN_MAIN_H__
#define __HQN_MAIN_H__

namespace hqn
{

/*
 * Performs cleanup of all global state and exit the program. This should be
 * called before exiting to ensure things get closed down correctly.
 * To be honest this function is a cop-out becuase I'm not sure how to safely
 * exit the Lua script from within a C function.
 */
void endItAll();



}


#endif