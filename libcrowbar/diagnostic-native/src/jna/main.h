#ifndef __MAIN_H_0d2824517a2f371d4600333be87e82708ebd5008__
#define __MAIN_H_0d2824517a2f371d4600333be87e82708ebd5008__

extern "C" {
/**
 * @brief Entry point for JNA calls.
 *
 * This function receives as input a JSON request and outputs a JSON response.
 * The request has the following format:
 * {"type"="diagnostic", "spectrum"=<spectrum's json>, "system"=<diagnostic system's json>}
 * The response has the following format:
 * {"type"="diagnostic", "report"=<report's json>, "system"=<diagnostic system's json>}
 */
void run (const char * request,
          char ** response);
void cleanup (char * response);
}

#endif