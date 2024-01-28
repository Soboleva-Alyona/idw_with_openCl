#include <cstdint>
#include <OpenCL/cl.h>
#include <fcntl.h>
#include <opencl-c.h>

class SpatialData
{
public:
    SpatialData() = default;
    SpatialData(int64_t lon, int64_t lat, double d) : longitude(lon), latitude(lat), data(d) {}
public:
    int64_t longitude;
    int64_t latitude;
    double data;
};

__kernel void idw_interpolation(
        __global const SpatialData * input, __global const SpatialData * output
) {
    const int size = input.size();
    double r = 0;
    double divider = 0;

    size_t output_index = get_global_id(0);

    for (int k = 0; k < size; k++)
    {
        // Calculate the distance
        int64_t longitude_dif = input[k].longitude - output[output_index].longitude;
        int64_t latitude_dif = input[k].latitude - output[output_index].latitude;

        double d_k = sqrt(pow(longitude_dif, 2) + pow(latitude_dif, 2));
        r += input[k].data / d_k;
        divider += 1.0 / d_k;
    }

    output[output_index].data = r / divider;

}
