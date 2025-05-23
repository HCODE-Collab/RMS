#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits> // Added for std::numeric_limits

struct City
{
    int index;
    std::string name;
};

class CityRoadManager
{
private:
    std::vector<City> cities;
    std::vector<std::vector<int>> road_matrix;
    std::vector<std::vector<double>> budget_matrix;

    int get_city_index(const std::string &name)
    {
        for (const auto &city : cities)
        {
            if (city.name == name)
            {
                return city.index - 1; // 0-based index for matrices
            }
        }
        return -1;
    }

    bool city_name_exists(const std::string &name)
    {
        for (const auto &city : cities)
        {
            if (city.name == name)
            {
                return true;
            }
        }
        return false;
    }

    std::vector<std::string> split(const std::string &s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    void load_data()
    {
        std::ifstream city_file("cities.txt");
        if (city_file.is_open())
        {
            std::string line;
            std::getline(city_file, line); // Skip header
            while (std::getline(city_file, line))
            {
                auto parts = split(line, '\t');
                if (parts.size() == 2)
                {
                    int index = std::stoi(parts[0]);
                    std::string name = parts[1];
                    cities.push_back({index, name});
                }
            }
            city_file.close();
        }

        int n = cities.size();
        road_matrix.resize(n, std::vector<int>(n, 0));
        budget_matrix.resize(n, std::vector<double>(n, 0.0));

        std::ifstream road_file("roads.txt");
        if (road_file.is_open())
        {
            std::string line;
            std::getline(road_file, line); // Skip header
            while (std::getline(road_file, line))
            {
                auto parts = split(line, '\t');
                if (parts.size() == 3)
                {
                    std::string road = parts[1];
                    double budget = std::stod(parts[2]);
                    auto city_names = split(road, '-');
                    if (city_names.size() == 2)
                    {
                        int i = get_city_index(city_names[0]);
                        int j = get_city_index(city_names[1]);
                        if (i != -1 && j != -1)
                        {
                            road_matrix[i][j] = 1;
                            road_matrix[j][i] = 1;
                            budget_matrix[i][j] = budget;
                            budget_matrix[j][i] = budget;
                        }
                    }
                }
            }
            road_file.close();
        }
    }

    void save_data()
    {
        std::ofstream city_file("cities.txt");
        if (city_file.is_open())
        {
            city_file << "Index\tCity_Name\n";
            for (const auto &city : cities)
            {
                city_file << city.index << "\t" << city.name << "\n";
            }
            city_file.close();
        }

        std::ofstream road_file("roads.txt");
        if (road_file.is_open())
        {
            road_file << "#\tRoad\tBudget\n";
            int road_id = 1;
            for (size_t i = 0; i < cities.size(); ++i)
            {
                for (size_t j = i + 1; j < cities.size(); ++j)
                {
                    if (road_matrix[i][j] == 1)
                    {
                        std::string road_name = cities[i].name + "-" + cities[j].name;
                        road_file << road_id++ << "\t" << road_name << "\t"
                                  << std::fixed << std::setprecision(3) << budget_matrix[i][j] << "\n";
                    }
                }
            }
            road_file.close();
        }
    }

    void add_cities()
    {
        int num;
        std::cout << "Enter number of cities to add: ";
        if (!(std::cin >> num))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            return;
        }
        for (int k = 0; k < num; ++k)
        {
            std::string name;
            std::cout << "Enter city name: ";
            std::cin >> name;
            if (city_name_exists(name))
            {
                std::cout << "City name already exists. Each city must have a unique name.\n";
                continue;
            }
            int index = cities.size() + 1;
            cities.push_back({index, name});

            int new_size = cities.size();
            road_matrix.resize(new_size);
            budget_matrix.resize(new_size);
            for (auto &row : road_matrix)
            {
                row.resize(new_size, 0);
            }
            for (auto &row : budget_matrix)
            {
                row.resize(new_size, 0.0);
            }
        }
        save_data();
    }

    void add_road()
    {
        int city1, city2;
        std::cout << "Enter first city index: ";
        if (!(std::cin >> city1))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            return;
        }
        std::cout << "Enter second city index: ";
        if (!(std::cin >> city2))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            return;
        }

        if (city1 < 1 || city1 > cities.size() || city2 < 1 || city2 > cities.size() || city1 == city2)
        {
            std::cout << "Invalid city indices. Indices must exist and be different.\n";
            return;
        }

        int i = city1 - 1;
        int j = city2 - 1;

        if (road_matrix[i][j] == 1)
        {
            std::cout << "A road already exists between these cities.\n";
            return;
        }

        road_matrix[i][j] = 1;
        road_matrix[j][i] = 1;

        double budget;
        std::cout << "Enter budget for the road: ";
        if (!(std::cin >> budget))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid budget. Please enter a valid number.\n";
            return;
        }
        budget_matrix[i][j] = budget;
        budget_matrix[j][i] = budget;

        save_data();
    }

    void update_budget()
    {
        int city1, city2;
        std::cout << "Enter first city index: ";
        if (!(std::cin >> city1))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            return;
        }
        std::cout << "Enter second city index: ";
        if (!(std::cin >> city2))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            return;
        }

        if (city1 < 1 || city1 > cities.size() || city2 < 1 || city2 > cities.size() || city1 == city2)
        {
            std::cout << "Invalid city indices. Indices must exist and be different.\n";
            return;
        }

        int i = city1 - 1;
        int j = city2 - 1;

        if (road_matrix[i][j] == 0)
        {
            std::cout << "No road exists between these cities. Budget cannot be added.\n";
            return;
        }

        double budget;
        std::cout << "Enter new budget for the road: ";
        if (!(std::cin >> budget))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid budget. Please enter a valid number.\n";
            return;
        }
        budget_matrix[i][j] = budget;
        budget_matrix[j][i] = budget;

        save_data();
    }

    void edit_city()
    {
        int index;
        std::cout << "Enter city index to edit: ";
        if (!(std::cin >> index))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            return;
        }

        if (index < 1 || index > cities.size())
        {
            std::cout << "Invalid city index.\n";
            return;
        }

        std::string new_name;
        std::cout << "Enter new city name: ";
        std::cin >> new_name;

        if (city_name_exists(new_name))
        {
            std::cout << "City name already exists. Each city must have a unique name.\n";
            return;
        }

        cities[index - 1].name = new_name;
        save_data();
    }

    void search_city()
    {
        std::string choice;
        std::cout << "Search by index (i) or name (n)? ";
        std::cin >> choice;

        if (choice == "i")
        {
            int index;
            std::cout << "Enter city index: ";
            if (!(std::cin >> index))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                return;
            }
            if (index >= 1 && index <= cities.size())
            {
                std::cout << "City " << index << ": " << cities[index - 1].name << "\n";
            }
            else
            {
                std::cout << "City not found.\n";
            }
        }
        else if (choice == "n")
        {
            std::string substring;
            std::cout << "Enter part of the city name: ";
            std::cin >> substring;
            bool found = false;
            for (const auto &city : cities)
            {
                if (city.name.find(substring) != std::string::npos)
                {
                    std::cout << "City " << city.index << ": " << city.name << "\n";
                    found = true;
                }
            }
            if (!found)
            {
                std::cout << "No cities found matching '" << substring << "'.\n";
            }
        }
        else
        {
            std::cout << "Invalid choice. Use 'i' for index or 'n' for name.\n";
        }
    }

    void display_cities()
    {
        if (cities.empty())
        {
            std::cout << "No cities recorded.\n";
            return;
        }
        std::cout << "Cities:\n";
        for (const auto &city : cities)
        {
            std::cout << city.index << "\t" << city.name << "\n";
        }
    }

    void display_roads()
    {
        std::cout << "Roads (List):\n";
        int road_id = 1;
        bool has_roads = false;
        for (size_t i = 0; i < cities.size(); ++i)
        {
            for (size_t j = i + 1; j < cities.size(); ++j)
            {
                if (road_matrix[i][j] == 1)
                {
                    std::string road_name = cities[i].name + "-" + cities[j].name;
                    std::cout << road_id++ << "\t" << road_name << "\t"
                              << std::fixed << std::setprecision(3) << budget_matrix[i][j] << "\n";
                    has_roads = true;
                }
            }
        }
        if (!has_roads)
        {
            std::cout << "No roads recorded.\n";
        }

        display_road_matrix();
    }

    void display_road_matrix()
    {
        if (cities.empty())
        {
            std::cout << "No road matrix to display (no cities).\n";
            return;
        }
        std::cout << "\nRoad Adjacency Matrix:\n   ";
        for (const auto &city : cities)
        {
            std::cout << std::setw(3) << city.index;
        }
        std::cout << "\n";
        for (size_t i = 0; i < cities.size(); ++i)
        {
            std::cout << std::setw(3) << cities[i].index;
            for (size_t j = 0; j < cities.size(); ++j)
            {
                std::cout << std::setw(3) << road_matrix[i][j];
            }
            std::cout << "\n";
        }
    }

    void display_recorded_data()
    {
        display_cities();
        display_road_matrix();
        if (cities.empty())
        {
            std::cout << "No budget matrix to display (no cities).\n";
            return;
        }
        std::cout << "\nBudget Adjacency Matrix:\n   ";
        for (const auto &city : cities)
        {
            std::cout << std::setw(10) << city.index;
        }
        std::cout << "\n";
        for (size_t i = 0; i < cities.size(); ++i)
        {
            std::cout << std::setw(3) << cities[i].index;
            for (size_t j = 0; j < cities.size(); ++j)
            {
                std::cout << std::fixed << std::setprecision(3) << std::setw(10) << budget_matrix[i][j];
            }
            std::cout << "\n";
        }
    }
};

int main()
{
    CityRoadManager manager;
    manager.load_data();

    int choice;
    do
    {
        std::cout << "\nMenu:\n";
        std::cout << "1. Add new city(ies)\n";
        std::cout << "2. Add roads between cities\n";
        std::cout << "3. Add/Update budget to the roads\n";
        std::cout << "4. Edit city\n";
        std::cout << "5. Search for a city using its index or name\n";
        std::cout << "6. Display cities\n";
        std::cout << "7. Display roads\n";
        std::cout << "8. Display recorded data on the console\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 1 and 9.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            manager.add_cities();
            break;
        case 2:
            manager.add_road();
            break;
        case 3:
            manager.update_budget();
            break;
        case 4:
            manager.edit_city();
            break;
        case 5:
            manager.search_city();
            break;
        case 6:
            manager.display_cities();
            break;
        case 7:
            manager.display_roads();
            break;
        case 8:
            manager.display_recorded_data();
            break;
        case 9:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice. Please select 1-9.\n";
        }
    } while (choice != 9);

    return 0;
}