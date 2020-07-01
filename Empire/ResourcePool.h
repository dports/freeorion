#ifndef _ResourcePool_h_
#define _ResourcePool_h_

#include "../universe/EnumsFwd.h"
#include "../util/Export.h"

#include <boost/signals2/signal.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>

#include <vector>
#include <set>

/** The ResourcePool class keeps track of an empire's stockpile and production
  * of a particular resource (eg. research, industry). */
class FO_COMMON_API ResourcePool {
public:
    ResourcePool(ResourceType type);

    /** \name Accessors */ //@{
    const std::vector<int>&         ObjectIDs() const;                      ///< returns UniverseObject IDs in this ResourcePool
    float                           Stockpile() const;                      ///< returns current stockpiled amount of resource

    float                           TotalOutput() const;                    ///< returns amount of resource being generated by all ResourceCenters
    std::map<std::set<int>, float>  Output() const;                         ///< returns the sets of groups of objects that can share resources, and the amount of this pool's resource that each group is generating this turn
    float                           GroupOutput(int object_id) const;       ///< returns amount of resource being generated by resource sharing group that contains the object with id \a object_id

    float                           TargetOutput() const;
    float                           GroupTargetOutput(int object_id) const;

    float                           TotalAvailable() const;                 ///< returns amount of resource immediately available = output + stockpile from all ResourceCenters, ignoring limitations of connections between centers
    /** Returns the sets of groups of objects that can share resources, and the
      * amount of this pool's resource that each group has available  */
    std::map<std::set<int>, float>  Available() const;
    float                           GroupAvailable(int object_id) const;    ///< returns amount of resource available in resource sharing group that contains the object with id \a object_id

    std::string                     Dump() const;
    //@}

    /** emitted after updating production, or called externally to indicate
      * that stockpile and change need to be refreshed. */
    mutable boost::signals2::signal<void ()> ChangedSignal;

    void        SetObjects(const std::vector<int>& object_ids);
    /** specifies which sets systems can share resources.  any two sets should
      * have no common systems. */
    void        SetConnectedSupplyGroups(const std::set<std::set<int>>& connected_system_groups);

    void        SetStockpile(float d);      ///< sets current sockpiled amount of resource

    void        Update();                   ///< recalculates total resource production

private:
    ResourcePool(); ///< default ctor needed for serialization

    std::vector<int>                m_object_ids;                                       ///< IDs of objects to consider in this pool
    std::set<std::set<int>>         m_connected_system_groups;                          ///< sets of systems between and in which objects can share this pool's resource
    std::map<std::set<int>, float>  m_connected_object_groups_resource_output;          ///< cached map from connected group of objects that can share resources, to how much resource is output by ResourceCenters in the group.  regenerated during update from other state information.
    std::map<std::set<int>, float>  m_connected_object_groups_resource_target_output;   ///< cached map from connected group of objects that can share resources, to how much resource would, if all meters equaled their target meters, be output by ResourceCenters in the group.  regenerated during update from other state information.
    float                           m_stockpile = 0.0f;                                 ///< current stockpiled amount of resource
    ResourceType                    m_type;                                             ///< what kind of resource does this pool hold?

    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version);
};


BOOST_CLASS_VERSION(ResourcePool, 1)

// template implementations
template <typename Archive>
void ResourcePool::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_NVP(m_type)
        & BOOST_SERIALIZATION_NVP(m_object_ids)
        & BOOST_SERIALIZATION_NVP(m_stockpile);
    if (version < 1) {
        int dummy = -1;
        ar  & boost::serialization::make_nvp("m_stockpile_object_id", dummy);
    }
    ar  & BOOST_SERIALIZATION_NVP(m_connected_system_groups);
}

#endif // _ResourcePool_h_
