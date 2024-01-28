#include <string>
#include <fstream>
#include <sstream>

std::string stringFromFile(const std::string& filename) {
    const std::string& path = filename;

    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

std::vector<SpatialData> read_csv(const std::string& filename, int & lonMin, int & latMin, int & lonMax, int & latMax) {
    std::vector<SpatialData> result;

    std::ifstream myFile(filename);

    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    std::string line;

    int colIdx = 0;

    // column names
    std::getline(myFile, line);

    // entries
    while(std::getline(myFile, line)) {
        std::stringstream ss(line);

        int longitude;
        int latitude;
        double data;

        ss >> longitude;
        if(ss.peek() == ',') ss.ignore();
        ss >> latitude;
        if(ss.peek() == ',') ss.ignore();
        ss >> data;

        result.push_back(SpatialData(longitude, latitude, data));
        colIdx++;

        if (longitude < lonMin) {
            lonMin = longitude;
        }
        if (longitude > lonMax) {
            lonMax = longitude;
        }
        if (latitude < latMin) {
            latMin = latitude;
        }
        if (latitude > latMax) {
            latMax = latitude;
        }
    }

    // Close file
    myFile.close();

    return result;
}

