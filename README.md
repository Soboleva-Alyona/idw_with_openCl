Есть функция для вычисления интерполяции по двумерным координатам IDW (inverse distance weight):
```
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
```

Надо её распараллелить на OpenCL. Написать код для хоста и для девайса.

Функцию сравнения можно написать, примерно, такую:
```
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

void idw_file_test()
{
  constexpr int SIZE = 200;
  const auto input = read_csv("Table1_Propane.csv");
  auto grid = makeGrid(input, SIZE, SIZE);
  auto gridGPU(grid);

  idw_vector_no_mem(input, grid);
 
  IdwGpu ig(input, gridGPU); // Выгрузку на GPU можно локализовать в отдельном классе.
  ig.Calculate();
 
  for (int i = 0; i < gridGPU.size(); ++i)
  {
    equals(grid[i].data, ig.outputData[i].data);
  }
}
```
