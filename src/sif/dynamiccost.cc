#include "sif/dynamiccost.h"

using namespace valhalla::baldr;

namespace valhalla{
namespace sif {

DynamicCost::DynamicCost(const boost::property_tree::ptree& pt,
                         const TravelMode mode)
    : allow_transit_connections_(false),
      travelmode_(mode),
      not_thru_distance_(5000.0f) {
  // Parse property tree to get hierarchy limits
  // TODO - get the number of levels
  uint32_t n_levels = sizeof(kDefaultMaxUpTransitions) /
      sizeof(kDefaultMaxUpTransitions[0]);
  for (uint32_t level = 0; level < n_levels; level++) {
    hierarchy_limits_.emplace_back(HierarchyLimits(pt, level));
  }
}

DynamicCost::~DynamicCost() {
}

// Does the costing allow hierarchy transitions? Defaults to false. Costing
// methods that wish to use hierarchy transitions must override this method.
bool DynamicCost::AllowTransitions() const {
  return false;
}

// Does the costing method allow multiple passes (with relaxed hierarchy
// limits). Defaults to false. Costing methods that wish to allow multiple
// passes with relaxed hierarchy transitions must override this method.
bool DynamicCost::AllowMultiPass() const {
  return false;
}

// Get the cost to traverse the specified directed edge using a transit
// departure (schedule based edge traversal). Cost includes
// the time (seconds) to traverse the edge. Only transit cost models override
// this method.
Cost DynamicCost::EdgeCost(const baldr::DirectedEdge* edge,
              const baldr::TransitDeparture* departure,
              const uint32_t curr_time) const {
  return { 0.0f, 0.0f };
}

// Returns the cost to make the transition from the predecessor edge.
// Defaults to 0. Costing models that wish to include edge transition
// costs (i.e., intersection/turn costs) must override this method.
Cost DynamicCost::TransitionCost(const DirectedEdge* edge,
                                 const NodeInfo* node,
                                 const EdgeLabel& pred) const {
  return { 0.0f, 0.0f };
}

// Returns the cost to make the transition from the predecessor edge
// when using a reverse search (from destination towards the origin).
// Defaults to 0. Costing models that wish to include edge transition
// costs (i.e., intersection/turn costs) must override this method.
Cost DynamicCost::TransitionCostReverse(const uint32_t idx,
                            const baldr::NodeInfo* node,
                            const baldr::DirectedEdge* opp_edge,
                            const baldr::DirectedEdge* opp_pred_edge) const {
  return { 0.0f, 0.0f };
}

// Returns the transfer cost between 2 transit stops.
Cost DynamicCost::TransferCost(const TransitTransfer* transfer) const {
  return { 0.0f, 0.0f };
}

// Returns the default transfer factor between 2 transit lines.
float DynamicCost::TransferCostFactor() const {
  return 0.0f;
}


// Returns the default transfer cost between 2 transit stops.
Cost DynamicCost::DefaultTransferCost() const {
  return { 0.0f, 0.0f };
}

// Get the general unit size that can be considered as equal for sorting
// purposes. Defaults to 1 (second).
uint32_t DynamicCost::UnitSize() const {
  return kDefaultUnitSize;
}

// Set to allow use of transit connections.
void DynamicCost::SetAllowTransitConnections(const bool allow) {
  allow_transit_connections_ = allow;
}

// This method overrides the max_distance with the multi-modal per segment
// distance. An example is a pure walking route may have a max distance of
// 10000 meters (10km) but for a multi-modal route a lower limit of 5000
// meters per segment (e.g. from origin to a transit stop or from the last
// transit stop to the destination).
void DynamicCost::UseMaxModeDistance() {
  ;
}

// Gets the hierarchy limits.
std::vector<HierarchyLimits>& DynamicCost::GetHierarchyLimits() {
  return hierarchy_limits_;
}

// Relax hierarchy limits.
void DynamicCost::RelaxHierarchyLimits(const float factor) {
  for (auto& hierarchy : hierarchy_limits_) {
    hierarchy.Relax(factor);
  }
}

// Do not transition up to highway level - remain on arterial. Used as last
// resort.
void DynamicCost::DisableHighwayTransitions() {
  hierarchy_limits_[1].DisableHighwayTransitions();
}

// Set the current travel mode.
void DynamicCost::set_travelmode(const TravelMode mode) {
  travelmode_ = mode;
}

// Get the current travel mode.
TravelMode DynamicCost::travelmode() const {
  return travelmode_;
}

// Set the distance from the destination where "not_thru" edges are allowed.
void DynamicCost::set_not_thru_distance(const float d) {
  not_thru_distance_ = d;
}

}
}
