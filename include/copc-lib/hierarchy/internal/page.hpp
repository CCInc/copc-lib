#ifndef COPCLIB_HIERARCHY_PAGE_INTERNAL_H_
#define COPCLIB_HIERARCHY_PAGE_INTERNAL_H_

#include <copc-lib/hierarchy/page.hpp>

namespace copc
{
class PageInternal;
class PageInternal : public Page
{
  public:
    PageInternal(Entry e) : Page(e){};
    PageInternal(VoxelKey key, int64_t offset, int32_t size) : Page(key, offset, size){};
    PageInternal(VoxelKey key) : Page(key, -1, -1){};

    std::vector<std::shared_ptr<PageInternal>> sub_pages;
    std::vector<std::shared_ptr<Node>> nodes;
};

} // namespace copc

#endif // COPCLIB_HIERARCHY_PAGE_INTERNAL_H_