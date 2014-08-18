#include "crowbar/diagnostic/jna/main.h"

#include <iostream>

int main() {
    const char * json = "{\"spectra\":\"0 0 \",\"system\":{\"connections\":[{\"from\":0,\"to\":0},{\"from\":0,\"to\":1},{\"from\":1,\"to\":1}],\"generators\":[{\"name\":\"single_fault\"},{\"name\":\"mhs2\"}],\"rankers\":[{\"configs\":{\"type\":\"ochiai\"},\"name\":\"similarity\"},{\"name\":\"fuzzinel\"}]},\"type\":\"diagnostic\"}";

    char * response = NULL;
    run(json, &response);

    std::cout << response << std::endl;

    return 0;
}
