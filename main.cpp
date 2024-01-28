#include <limits>
#include "IdwGpu.h"

#define SIZE 200

std::vector<SpatialData> makeGrid(
        int64_t lonMin,
        int64_t latMin,
        int64_t lonMax,
        int64_t latMax,
        uint64_t stepsLon,
        uint64_t stepsLat)
{
    std::vector<SpatialData> result;
    int64_t dLon = (lonMax - lonMin) / stepsLon;
    int64_t dLat = (latMax - latMin) / stepsLat;
    for (int64_t lon = lonMin; lon <= lonMax; lon += dLon)
    {
        for (int64_t lat = latMin; lat <= latMax; lat += dLat)
        {
            result.emplace_back(lon, lat, 0);
        }
    }
    return result;
}

int main() {
    int64_t lonMin = std::numeric_limits<int64_t>::max();
    int64_t latMin = std::numeric_limits<int64_t>::max();
    int64_t lonMax = std::numeric_limits<int64_t>::min();
    int64_t latMax = std::numeric_limits<int64_t>::min();
    const auto input = read_csv("Table1_Propane.csv", lonMin, latMin, lonMax, latMax);

    auto grid = makeGrid(lonMin, latMin, lonMax, latMax, SIZE, SIZE);
    auto gridGPU(grid);

    idw_vector_no_mem(input, grid);

    IdwGpu ig(input, gridGPU); // Выгрузку на GPU можно локализовать в отдельном классе.
    ig.Calculate();

    for (int i = 0; i < gridGPU.size(); ++i)
    {
        //equals(grid[i].data, ig.outputData[i].data);
    }
    return 0;
}
