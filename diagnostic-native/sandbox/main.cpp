#include "crowbar/diagnostic/jna/main.h"

#include <iostream>

int main() {
    const char * json = "{\"class\":\"io.crowbar.diagnosis.DiagnosticSystem\",\"connections\":[{\"class\":\"io.crowbar.diagnosis.Connection\",\"from\":0,\"to\":0},{\"class\":\"io.crowbar.diagnosis.Connection\",\"from\":0,\"to\":1},{\"class\":\"io.crowbar.diagnosis.Connection\",\"from\":1,\"to\":1}],\"generators\":[{\"class\":\"io.crowbar.diagnosis.Algorithm\",\"configs\":null,\"name\":\"single_fault\"},{\"class\":\"io.crowbar.diagnosis.Algorithm\",\"configs\":null,\"name\":\"mhs2\"}],\"rankers\":[{\"class\":\"io.crowbar.diagnosis.Algorithm\",\"configs\":{\"type\":\"ochiai\"},\"name\":\"similarity\"},{\"class\":\"io.crowbar.diagnosis.Algorithm\",\"configs\":{\"type\":\"ochiai\"},\"name\":\"similarity\"},{\"class\":\"io.crowbar.diagnosis.Algorithm\",\"configs\":{\"type\":\"ochiai\"},\"name\":\"similarity\"},{\"class\":\"io.crowbar.diagnosis.Algorithm\",\"configs\":null,\"name\":\"fuzzinel\"}]}";

    char * response = NULL;
    run_diagnostic(json, &response);

    std::cout << response << std::endl;

    return 0;
}
