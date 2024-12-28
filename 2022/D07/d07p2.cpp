#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <limits>

// Structure to represent a file with name and size
struct File {
    std::string name;
    long size;
};

// Forward declaration of Directory for use in Directory structure
struct Directory;

// Structure to represent a directory with name, parent, subdirectories, and files
struct Directory {
    std::string name;
    Directory* parent;
    std::vector<std::unique_ptr<Directory>> subdirectories;
    std::vector<File> files;

    // Constructor to initialize directory with name and parent
    Directory(const std::string& name_, Directory* parent_) : name(name_), parent(parent_) {}

    // Function to find a subdirectory by name
    Directory* find_subdirectory(const std::string& dir_name) const {
        for (const auto& subdir : subdirectories) {
            if (subdir->name == dir_name) {
                return subdir.get();
            }
        }
        return nullptr;
    }

    // Function to add a subdirectory if it doesn't exist
    Directory* add_subdirectory(const std::string& dir_name) {
        Directory* existing = find_subdirectory(dir_name);
        if (existing) {
            return existing;
        }
        subdirectories.emplace_back(std::make_unique<Directory>(dir_name, this));
        return subdirectories.back().get();
    }

    // Function to add a file if it doesn't exist
    void add_file(const std::string& file_name, long size) {
        auto it = std::find_if(files.begin(), files.end(),
                               [&file_name](const File& f) { return f.name == file_name; });
        if (it == files.end()) {
            files.emplace_back(File{file_name, size});
        }
    }
};

// Function to compute total size of a directory and collect all directory sizes
long compute_directory_size(const Directory* dir, std::vector<long>& all_sizes) {
    long total_size = 0;

    // Sum sizes of all files in the current directory
    for (const auto& file : dir->files) {
        total_size += file.size;
    }

    // Recursively sum sizes of all subdirectories
    for (const auto& subdir : dir->subdirectories) {
        total_size += compute_directory_size(subdir.get(), all_sizes);
    }

    // Collect the total size of the current directory
    all_sizes.push_back(total_size);

    return total_size;
}

int main() {
    // Read all input lines first
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    // Initialize root directory
    auto root = std::make_unique<Directory>("/", nullptr);
    Directory* current_dir = root.get();

    // Process each line using an index
    for (size_t i = 0; i < lines.size(); ++i) {
        line = lines[i];
        if (line.empty()) continue;

        if (line[0] == '$') {
            // It's a command
            std::istringstream iss(line.substr(2)); // Skip "$ "
            std::string cmd;
            iss >> cmd;

            if (cmd == "cd") {
                std::string arg;
                iss >> arg;
                if (arg == "/") {
                    current_dir = root.get();
                }
                else if (arg == "..") {
                    if (current_dir->parent != nullptr) {
                        current_dir = current_dir->parent;
                    }
                }
                else {
                    // Change to the specified subdirectory, create if it doesn't exist
                    current_dir = current_dir->add_subdirectory(arg);
                }
            }
            else if (cmd == "ls") {
                // Process the output lines of ls until the next command or end of input
                while (i + 1 < lines.size() && lines[i + 1][0] != '$') {
                    ++i;
                    std::string ls_line = lines[i];
                    std::istringstream ls_iss(ls_line);
                    std::string first, second;
                    ls_iss >> first >> second;
                    if (first == "dir") {
                        // It's a directory
                        current_dir->add_subdirectory(second);
                    }
                    else {
                        // It's a file with size and name
                        try {
                            long size = std::stol(first);
                            std::string name = second;
                            current_dir->add_file(name, size);
                        }
                        catch (const std::invalid_argument& e) {
                            // Handle invalid size format if necessary
                            // For this problem, we can assume the input is well-formed
                        }
                    }
                }
            }
        }
    }

    // Compute total sizes and collect all directory sizes
    std::vector<long> all_directory_sizes;
    long total_used_space = compute_directory_size(root.get(), all_directory_sizes);

    // Part Two: Find the smallest directory to delete to free enough space
    const long TOTAL_DISK_SPACE = 70000000;
    const long REQUIRED_UNUSED_SPACE = 30000000;
    long current_unused_space = TOTAL_DISK_SPACE - total_used_space;
    long additional_space_needed = REQUIRED_UNUSED_SPACE - current_unused_space;

    // Initialize with maximum possible value
    long smallest_dir_size_part_two = std::numeric_limits<long>::max();

    for (const auto& size : all_directory_sizes) {
        if (size >= additional_space_needed) {
            if (size < smallest_dir_size_part_two) {
                smallest_dir_size_part_two = size;
            }
        }
    }

    // Output the results
    std::cout << smallest_dir_size_part_two << std::endl;

    return 0;
}

