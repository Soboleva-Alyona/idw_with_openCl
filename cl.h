#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef struct
{
    int longitude;
    int latitude;
    float data;
} SpatialData;

__kernel void idw_interpolation(
        __global const SpatialData * input, __global SpatialData * output, uint input_size
) {
    float r = 0;
    float divider = 0;

    size_t output_index = get_global_id(0);

    for (int k = 0; k < input_size; k++)
    {

        // Calculate the distance
        float longitude_dif = input[k].longitude - output[output_index].longitude;
        float latitude_dif = input[k].latitude - output[output_index].latitude;

        float d_k = hypot(longitude_dif, latitude_dif);

        r += input[k].data / d_k;

        divider += 1.0 / d_k;
    }

    output[output_index].data = r / divider;
}
