#include <json_packer.h>

int main() {

    const char *filepath = "../resources/records.txt";
    int status = JSONp_Packer(filepath);
    return status;

}
