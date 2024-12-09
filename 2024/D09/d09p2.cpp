#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

typedef unsigned long long ull;

struct Segment {
    int start;
    int length; // For files: length = end - start + 1. For free space: same meaning.
};

static void parseInput(const std::string& data, std::vector<int>& files, std::vector<int>& freeSpace) {
    // Assumes input is alternating: fileLength, freeLength, fileLength, freeLength...
    for (size_t i = 0; i < data.size(); i++) {
        int value = data[i] - '0';
        if (i % 2 == 0) {
            files.push_back(value);
        } else {
            freeSpace.push_back(value);
        }
    }
}

static ull computeChecksum(const std::vector<int>& finalPositions) {
    ull checksum = 0;
    int start = -1;
    int currentId = -1;

    for (size_t i = 0; i < finalPositions.size(); i++) {
        if (finalPositions[i] >= 0) {
            if (currentId != finalPositions[i]) {
                // Process previous segment
                if (start != -1) {
                    size_t length = i - start;
                    ull sum = (length * (2ULL * start + length - 1)) / 2ULL;
                    checksum += sum * currentId;
                }
                // Start new segment
                start = (int)i;
                currentId = finalPositions[i];
            }
        } else if (start != -1) {
            // Process segment ending at free space
            size_t length = i - start;
            ull sum = (length * (2ULL * start + length - 1)) / 2ULL;
            checksum += sum * currentId;
            start = -1;
        }
    }

    // Handle last segment if exists
    if (start != -1) {
        size_t length = finalPositions.size() - start;
        ull sum = (length * (2ULL * start + length - 1)) / 2ULL;
        checksum += sum * currentId;
    }

    return checksum;
}

// Insert a free segment into the sorted freeSegments vector and merge if necessary
static void addFreeSegment(std::vector<Segment>& freeSegments, int start, int length) {
    if (length <= 0) return;
    Segment newSeg{ start, length };
    auto it = std::lower_bound(freeSegments.begin(), freeSegments.end(), newSeg,
                               [](const Segment& a, const Segment& b) {
                                   return a.start < b.start;
                               });
    it = freeSegments.insert(it, newSeg);

    // Merge with previous segment if adjacent or overlapping
    if (it != freeSegments.begin()) {
        auto prev = it - 1;
        int prevEnd = prev->start + prev->length - 1;
        int curEnd = it->start + it->length - 1;
        if (prevEnd >= it->start) {
            // Merge it into prev
            int overlapEnd = std::max(prevEnd, curEnd);
            prev->length = (overlapEnd - prev->start + 1);
            freeSegments.erase(it);
            it = prev;
        }
    }

    // Merge with next segment if adjacent or overlapping
    if ((it + 1) != freeSegments.end()) {
        auto next = it + 1;
        int curEnd = it->start + it->length - 1;
        int nextEnd = next->start + next->length - 1;
        if (curEnd >= next->start) {
            int overlapEnd = std::max(curEnd, nextEnd);
            it->length = (overlapEnd - it->start + 1);
            freeSegments.erase(next);
        }
    }
}

static std::vector<int> compactDiskWholeFiles(const std::vector<int>& files, const std::vector<int>& freeSpace) {
    // Build initial positions vector
    int totalSize = std::accumulate(files.begin(), files.end(), 0) +
                    std::accumulate(freeSpace.begin(), freeSpace.end(), 0);
    std::vector<int> positions;
    positions.reserve(totalSize);

    // Interleave files and free spaces
    for (size_t i = 0; i < files.size(); i++) {
        for (int f = 0; f < files[i]; f++) positions.push_back((int)i);
        if (i < freeSpace.size()) {
            for (int fs = 0; fs < freeSpace[i]; fs++) positions.push_back(-1);
        }
    }

    // Precompute file segments (start and length)
    std::vector<Segment> fileSegments(files.size(), { -1, 0 });
    {
        int index = 0;
        for (size_t f = 0; f < files.size(); f++) {
            if (files[f] > 0) {
                int start = index;
                int end = index + files[f] - 1;
                int length = end - start + 1;
                fileSegments[f] = { start, length };
                index += files[f];
            }
            if (f < freeSpace.size()) {
                index += freeSpace[f];
            }
        }
    }

    // Compute initial free segments
    std::vector<Segment> freeSegments;
    {
        int start = -1;
        for (int i = 0; i < (int)positions.size(); i++) {
            if (positions[i] == -1 && start == -1) {
                start = i;
            } else if (positions[i] != -1 && start != -1) {
                addFreeSegment(freeSegments, start, i - start);
                start = -1;
            }
        }
        if (start != -1) {
            addFreeSegment(freeSegments, start, (int)positions.size() - start);
        }
    }

    auto findFreeSegmentBefore = [&](int fileStart, int fileSize) -> int {
        for (size_t i = 0; i < freeSegments.size(); i++) {
            const auto& fs = freeSegments[i];
            int fsEnd = fs.start + fs.length - 1;
            // This segment must end before fileStart to consider placing the file here
            if (fsEnd < fileStart && fs.length >= fileSize) {
                return (int)i;
            }
        }
        return -1;
    };

    // Move files in reverse order (highest ID first)
    for (int fileId = (int)files.size() - 1; fileId >= 0; fileId--) {
        int fileSize = files[fileId];
        if (fileSize == 0) continue; // No file to move

        auto seg = fileSegments[fileId];
        int fileStart = seg.start;
        int fileEnd = seg.start + seg.length - 1;

        int fsIndex = findFreeSegmentBefore(fileStart, fileSize);
        if (fsIndex == -1) {
            // No suitable free segment found
            continue;
        }

        // We found a suitable segment
        int bestFreeStart = freeSegments[fsIndex].start;

        // Clear old file location
        for (int i = fileStart; i <= fileEnd; i++) {
            positions[i] = -1;
        }

        // Add old file space back to free segments
        addFreeSegment(freeSegments, fileStart, seg.length);

        // Place the file in the new location
        for (int i = 0; i < fileSize; i++) {
            positions[bestFreeStart + i] = fileId;
        }

        // Update file segment info
        fileSegments[fileId].start = bestFreeStart;
        fileSegments[fileId].length = fileSize;

        // Update the chosen free segment
        freeSegments[fsIndex].start += fileSize;
        freeSegments[fsIndex].length -= fileSize;
        if (freeSegments[fsIndex].length == 0) {
            freeSegments.erase(freeSegments.begin() + fsIndex);
        }
    }

    return positions;
}

int main() {
    std::string data;
    std::getline(std::cin, data);

    std::vector<int> files;
    std::vector<int> freeSpace;

    parseInput(data, files, freeSpace);
    auto finalPositions = compactDiskWholeFiles(files, freeSpace);
    ull checksum = computeChecksum(finalPositions);

    std::cout << checksum << std::endl;
    return 0;
}
