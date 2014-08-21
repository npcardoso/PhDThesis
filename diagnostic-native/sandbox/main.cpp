#include "crowbar/diagnostic/jna/main.h"

#include <iostream>

int main() {
    const char * json = "{\"spectrum\":\"3 2 1 1 0 1 1 0 1 1 \",\"system\":{\"connections\":[{\"from\":0,\"to\":0},{\"from\":1,\"to\":0},{\"from\":1,\"to\":1}],\"generators\":[{\"name\":\"single_fault\"},{\"name\":\"mhs2\"}],\"rankers\":[{\"configs\":{\"type\":\"ochiai\"},\"name\":\"similarity\"},{\"name\":\"fuzzinel\"}]},\"type\":\"diagnostic\"}";

    char * response = NULL;
    run(json, &response);


    return 0;
}
