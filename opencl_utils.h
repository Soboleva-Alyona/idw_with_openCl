#include <OpenCL/cl.h>
#include <iostream>
#include <fstream>

cl_device_id get_device() {
    cl_platform_id platform;
    cl_device_id device;
    cl_int err = 0;

    err |= clGetPlatformIDs(1, &platform, nullptr);
    err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (err == CL_DEVICE_NOT_FOUND) {
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, nullptr);
    }
    if (err) {
        throw;
    }
    return device;
};

void executeKernel(const char *&program_text, const size_t &program_len, const char *kernel_name, size_t res_size, int &res,
                   int &input, size_t input_size, size_t glob_size) {

    cl_device_id device_id = get_device();
    cl_int err;
    cl_context context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &err);
    if (err) {
        throw std::runtime_error("Error while creating context");
    }

    cl_program program = clCreateProgramWithSource(
            context,
            1,
            &program_text,
            &program_len,
            &err
    );
    if (err) {
        throw std::runtime_error("Error while creating program with source");
    }

    err |= clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);

    if (err) {
        throw std::runtime_error("Error while building program");
    }

    cl_kernel kernel = clCreateKernel(program, kernel_name, &err);
    if (err) {
        throw std::runtime_error("Error creating kernel");
    }

    cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &err);

    cl_mem res_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, res_size, nullptr, nullptr);

    err |= clSetKernelArg(kernel, 0, input_size, &input);

    size_t global_size[1] = {glob_size};
    err |= clEnqueueNDRangeKernel(
            queue,
            kernel,
            1,
            nullptr,
            global_size,
            nullptr,
            0,
            nullptr,
            nullptr
    );

    err |= clEnqueueReadBuffer(queue, res_buffer, CL_TRUE, 0, res_size, &res, 0, nullptr, nullptr);

    clFinish(queue);
}

