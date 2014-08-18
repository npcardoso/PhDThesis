#ifndef __MAIN_H_0d2824517a2f371d4600333be87e82708ebd5008__
#define __MAIN_H_0d2824517a2f371d4600333be87e82708ebd5008__

extern "C" {
    void run_diagnostic(const char * request, char ** response);
    void cleanup_diagnostic(char * response);
}

#endif
