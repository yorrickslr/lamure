// Copyright (c) 2014 Bauhaus-Universitaet Weimar
// This Software is distributed under the Modified BSD License, see license.txt.
//
// Virtual Reality and Visualization Research Group 
// Faculty of Media, Bauhaus-Universitaet Weimar
// http://www.uni-weimar.de/medien/vr

#ifndef REN_OOC_CACHE_H_
#define REN_OOC_CACHE_H_

#include <map>
#include <queue>
#include <lamure/utils.h>
#include <lamure/ren/config.h>
#include <lamure/ren/cache.h>
#include <lamure/ren/ooc_pool.h>

#include <lamure/ren/platform.h>
#include <lamure/ren/model_database.h>
#include <lamure/ren/policy.h>


namespace lamure {
namespace ren {

class RENDERING_DLL ooc_cache : public cache
{
public:

                        ooc_cache(const ooc_cache&) = delete;
                        ooc_cache& operator=(const ooc_cache&) = delete;
    virtual             ~ooc_cache();

    static ooc_cache*    get_instance();

    void                register_node(const model_t model_id, const node_t node_id, const int32_t priority);
    char*               node_data(const model_t model_id, const node_t node_id);

    const bool          is_node_resident_and_aquired(const model_t model_id, const node_t node_id);

    void                refresh();

    void                lock_pool();
    void                unlock_pool();

    void                begin_measure();
    void                end_measure();

protected:

                        ooc_cache(const size_t num_slots);
    static bool         is_instanced_;
    static ooc_cache*    single_;

private:
    static std::mutex   mutex_;

    char*               cache_data_;
    char*               cache_data__provenance;
    uint32_t            maintenance_counter_;
    ooc_pool*            pool_;
};


} } // namespace lamure


#endif // REN_OOC_CACHE_H_
