#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include "Data.h"

std::string get_device_name(cl_device_id device) {
    size_t size;
    clGetDeviceInfo(device, CL_DEVICE_NAME, 0, nullptr, &size);
    char *name = new char[size];
    clGetDeviceInfo(device, CL_DEVICE_NAME, size, name, nullptr);
    std::string res(name);
    delete[] name;
    return res;
}

cl_device_id get_device() {
    cl_platform_id platform;
    cl_device_id device;
    cl_int err = 0;

    err |= clGetPlatformIDs(1, &platform, nullptr);
    err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (err == CL_DEVICE_NOT_FOUND) {
        printf("Took cpu\n");
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, nullptr);
    } else {
        printf("Took gpu\n");
    }
    if (err) {
        throw;
    }
    printf("%s ", get_device_name(device).c_str());
    return device;
};

void executeKernel(const char *&program_text, const size_t &program_len, const char *kernel_name, size_t res_size, const SpatialData * res,
                   const SpatialData * input, size_t input_size, size_t in_size, size_t glob_size) {

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
        size_t len;
        char buffer[2048];
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        throw std::runtime_error("Error: Failed to build program executable!\n" + std::string(buffer));
    }

    cl_kernel kernel = clCreateKernel(program, kernel_name, &err);
    if (err) {
        throw std::runtime_error("Error: Failed create kernel!\n");
    }

    cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &err);

    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, input_size, nullptr, nullptr);
    if (!input_buffer) { throw std::runtime_error("Error: Failed to allocate device memory!\n"); }

    err |= clEnqueueWriteBuffer(queue, input_buffer, CL_FALSE, 0, input_size, input, 0, nullptr, nullptr);
    if (err != CL_SUCCESS) { throw std::runtime_error("Error: Failed to write to source array!\n"); }

    err |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    if (err) {
        throw std::runtime_error("Error: Failed to set argument 0!\n");
    }
    cl_mem res_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, res_size, nullptr, nullptr);
    err |= clEnqueueWriteBuffer(queue, res_buffer, CL_FALSE, 0, res_size, res, 0, nullptr, nullptr);
    if (err != CL_SUCCESS) { throw std::runtime_error("Error: Failed to write to res array!\n"); }

    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &res_buffer);
    if (err) {
        throw std::runtime_error("Error: Failed to set argument 1!\n");
    }
    err |= clSetKernelArg(kernel, 2, sizeof(cl_uint), &in_size);
    if (err) {
        throw std::runtime_error("Error: Failed to set argument 2!\n");
    }

    size_t global_size[1] = {glob_size };
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
    if (err) {
        throw std::runtime_error("Error: Failed while clEnqueueNDRangeKernel!\n");
    }

    err |= clEnqueueReadBuffer(queue, res_buffer, CL_TRUE, 0, res_size, (void *)res, 0, nullptr, nullptr);

    clFinish(queue);
}



