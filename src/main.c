#include <json_packer.h>

int main() {

    const char *filepath = "../resources/records.txt";
    int status = JSONp_Pack(filepath);
    return status;

}
