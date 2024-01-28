#include "opencl_utils.h"
#include "io_utils.h"

#define SIZE 200

class IdwGpu {

public:

    IdwGpu(const std::vector<SpatialData>& input, const std::vector<SpatialData>& grid) {
        this->input = input;
    };

    static void Calculate() {
        std::string program_str = stringFromFile("cl.h");
        const char* program_text = program_str.c_str();
        const size_t program_len = program_str.length();

        const char *kernel_name = "idw_interpolation";
        size_t res_size = 0; // todo
        int res = 0; // todo

        int input_ = 0;
        size_t input_size = 0;

        executeKernel(program_text, program_len, kernel_name, res_size, res, input_, input_size, SIZE);

    };

    std::vector<SpatialData> outputData;

private:
    std::vector<SpatialData> input;

};