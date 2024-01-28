#include <OpenCL/cl.h>

#include <vector>
#include <cmath>

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


void idw_vector_no_mem(const std::vector<SpatialData> &input, std::vector<SpatialData> &output)
{
    const int size = input.size();
    const int out_size = output.size();
    double r = 0;
    double divider = 0;
    for (int i = 0; i < out_size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            // Calculate the distance
            double d_k = sqrt(pow(input[k].longitude - output[i].longitude, 2) + pow(input[k].latitude - output[i].latitude, 2));
            r += input[k].data / d_k;
            divider += 1.0 / d_k;
        }
        output[i].data = r / divider;
        r = 0;
        divider = 0;
    }
}

