#include <random>
#include <iterator>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

const std::string root = "C:\\Users\\carri\\Pictures\\slideshow_lockscreen\\";
const std::string ext = "png";

std::vector<std::string> files;
std::random_device rd;
std::mt19937 random_gen(rd());

void changeLockscreen() {
    std::string last_number_file_path = root + "last_file.txt";
    std::fstream last_number_file{last_number_file_path, std::ios::in};
    std::stringstream new_contents;
    int rolled_number = 0;
    if (!last_number_file) {
        printf("Failed to open last_file.txt for reading\n");
        return;
    }
    int last_index = -1;
    last_number_file >> last_index;
    new_contents << last_index + 1;
    new_contents << last_number_file.rdbuf();
    last_number_file.seekg(std::ios::beg);
    for (int i = 0; i < last_index + 2; i++)
    {
        last_number_file.ignore(100, '\n');
    }
    if (!(last_number_file >> rolled_number))
    {
        if (last_number_file.eof())
        {
            printf("eof");
            last_number_file.close();
            std::vector<int> numbers;
            numbers.push_back(0);
            for (int i = 0; i < files.size(); i++)
            {
                numbers.push_back(i);
            }
            std::shuffle(numbers.begin() + 1, numbers.end(), random_gen);
            last_number_file.open(last_number_file_path, std::ios::out);

            if (!last_number_file)
            {
                printf("Failed to open last_file.txt for wrlting\n");
                return;
            }
            std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(last_number_file, "\n"));
            std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(std::cout, "\n"));
            last_number_file.close();
            rolled_number = numbers[1];
        } 
        else 
        {
            printf("Failed fo read next index from last_file.txt. Lockscreen is not changed.\n");
            return;
        }
    } 
    else 
    {
        std::cout << last_index + 1 << ". " << rolled_number << "\n";
        last_number_file.close();
        last_number_file.open(last_number_file_path, std::ios::out);
        last_number_file << new_contents.rdbuf();
    }

    std::ifstream src{files[rolled_number], std::ios::binary};
    std::ofstream dst{root + "current.png", std::ios::binary};

    if (src && dst)
    {
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
}

int main()
{
    for (const auto& entry : std::filesystem::directory_iterator(root))
    {
        const std::string file_name = entry.path().string();
        if (std::equal(ext.rbegin(), ext.rend(), file_name.rbegin()) && file_name != root + "current.png")
        {
            files.push_back(file_name);
        }
    }

    changeLockscreen();
    return 0;
}