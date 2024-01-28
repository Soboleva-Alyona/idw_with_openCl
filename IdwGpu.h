#include "opencl_utils.h"
#include "io_utils.h"

#define SIZE 200

class IdwGpu {

public:

    IdwGpu(const SpatialData * input, const SpatialData * grid, size_t in_size, size_t out_size) {
        this->input = input;
        this->outputData = grid;
        this->in_size = in_size;
        this->out_size = out_size;
    };

    void Calculate() {
        std::string program_str = stringFromFile("cl.h");
        const char* program_text = program_str.c_str();
        const size_t program_len = program_str.length();

        const char *kernel_name = "idw_interpolation";
        size_t res_size = sizeof(SpatialData) * out_size;

        size_t input_size = sizeof(SpatialData) * in_size;

        executeKernel(program_text, program_len, kernel_name, res_size, outputData, input, input_size, in_size, out_size);

    };

    const SpatialData * outputData;

private:
    const SpatialData * input;
    size_t in_size;
    size_t out_size;

};