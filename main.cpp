#include <limits>
#include <vector>
#include "IdwGpu.h"

#define SIZE 200

#define EPS 0.00001

std::vector<SpatialData> makeGrid(
        int lonMin,
        int latMin,
        int lonMax,
        int latMax,
        uint stepsLon,
        uint stepsLat)
{
    std::vector<SpatialData> result;
    int dLon = (lonMax - lonMin) / stepsLon;
    int dLat = (latMax - latMin) / stepsLat;
    for (int lon = lonMin; lon <= lonMax; lon += dLon)
    {
        for (int lat = latMin; lat <= latMax; lat += dLat)
        {
            result.push_back(SpatialData(lon, lat, 0));
        }
    }
    return result;
}

int main() {
    int lonMin = std::numeric_limits<int>::max();
    int latMin = std::numeric_limits<int>::max();
    int lonMax = std::numeric_limits<int>::min();
    int latMax = std::numeric_limits<int>::min();
    const std::vector<SpatialData> input = read_csv("Table1_Propane.csv", lonMin, latMin, lonMax, latMax);

    std::vector<SpatialData> grid = makeGrid(lonMin, latMin, lonMax, latMax, SIZE, SIZE);
    std::vector<SpatialData> gridGPU(grid);

    auto seq_time_start = std::chrono::high_resolution_clock::now();

    idw_vector_no_mem(input, grid);
    auto seq_time_end = std::chrono::high_resolution_clock::now();
    float seq_time = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(seq_time_end - seq_time_start).count());
    printf("Seq time: %f ms\n", seq_time);


    IdwGpu ig(&input[0], &gridGPU[0], input.size(), gridGPU.size());
    ig.Calculate();

    for (int i = 0; i < gridGPU.size(); ++i)
    {
        if (fabs(grid[i].data - ig.outputData[i].data) > EPS) {
            printf("values for %i index are different\n", i);
        }
    }
    return 0;
}
