#ifndef COPCLIB_LAZ_DECOMPRESS_H_
#define COPCLIB_LAZ_DECOMPRESS_H_

#include <copc-lib/io/writer.hpp>

#include <lazperf/filestream.hpp>

#include <istream>
#include <vector>

using namespace lazperf;

namespace copc::laz
{

class Compressor
{
  public:
    static uint32_t CompressBytes(io::Writer *writer, std::vector<char> &in)
    {
        OutFileStream stream(writer->out_stream);

        las_compressor::ptr compressor = build_las_compressor(stream.cb(), writer->file->GetLasHeader().point_format_id,
                                                              writer->file->GetLasHeader().ebCount());

        int point_size = writer->file->GetLasHeader().point_record_length;
        if (in.size() % point_size != 0)
            throw std::runtime_error("Invalid input stream for compression!");

        uint32_t point_count = in.size() / point_size;

        std::vector<char> buff;
        for (int i = 0; i < point_count; i++)
        {
            buff = std::vector<char>(in.begin() + (i * point_size), in.begin() + ((i + 1) * point_size));
            compressor->compress(buff.data());
        }
        return point_count;
    }
};
} // namespace copc::laz

#endif // COPCLIB_LAZ_DECOMPRESS_H_