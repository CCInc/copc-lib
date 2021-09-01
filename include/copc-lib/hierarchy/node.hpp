#ifndef COPCLIB_HIERARCHY_NODE_H_
#define COPCLIB_HIERARCHY_NODE_H_

#include <sstream>
#include <vector>

#include <copc-lib/hierarchy/entry.hpp>
#include <copc-lib/hierarchy/key.hpp>
#include <copc-lib/io/reader.hpp>
#include <copc-lib/las/point.hpp>

namespace copc::hierarchy
{

class Page;
class Node : public Entry
{
  public:
    Node(VoxelKey key, uint64_t offset, int32_t size, int32_t point_count) : Entry(key, offset, size, point_count){};
    Node(Entry e, std::shared_ptr<io::Reader> reader) : Entry(e), reader_(reader){};
    Node(VoxelKey key, uint64_t offset, int32_t size, int32_t point_count, std::shared_ptr<io::Reader> reader)
        : Entry(key, offset, size, point_count), reader_(reader){};

    static void PackPoints(const std::vector<las::Point> &points, std::ostream &out_stream);
    static std::vector<char> PackPoints(const std::vector<las::Point> &points);

    // Reads the node's data into an uncompressed byte array
    std::vector<char> GetPointData();
    // Reads the node's data into a compressed byte array
    std::vector<char> GetPointDataCompressed();

    // Reads the node's data into Point objects
    std::vector<las::Point> GetPoints();

  private:
    std::shared_ptr<io::Reader> reader_;
};

} // namespace copc::hierarchy

#endif // COPCLIB_HIERARCHY_NODE_H_