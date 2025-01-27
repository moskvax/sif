#ifndef VALHALLA_SIF_EDGELABEL_H_
#define VALHALLA_SIF_EDGELABEL_H_

#include <valhalla/baldr/graphid.h>
#include <valhalla/baldr/directededge.h>
#include <valhalla/sif/costconstants.h>
#include <limits>

namespace valhalla {
namespace sif {

constexpr uint32_t kInvalidLabel = std::numeric_limits<uint32_t>::max();

/**
 * Labeling information for shortest path algorithm. Contains cost,
 * predecessor, current time, and assorted information required during
 * construction of the shortest path (stored in AdjacencyList) and for
 * reconstructing the path upon completion (the EdgeLabel list forms
 * the list of permanently labeled elements - aka the Done set).
 */
class EdgeLabel {
 public:
  /**
   * Default constructor.
   */
  EdgeLabel();

  /**
   * Constructor with values.
   * @param predecessor Index into the edge label list for the predecessor
   *                       directed edge in the shortest path.
   * @param edgeid    Directed edge.
   * @param endnode   End node of the directed edge.
   * @param cost      True cost (cost and elapsed time in seconds) to the edge.
   * @param sortcost  Cost for sorting (includes A* heuristic)
   * @param dist      Distance meters to the destination
   * @param restrictions Restriction mask from prior edge - this is used
   *                  if edge is a transition edge. This allows restrictions
   *                  to be carried across different hierarchy levels.
   * @param mode      Mode of travel along this edge.
   */
  EdgeLabel(const uint32_t predecessor, const baldr::GraphId& edgeid,
            const baldr::DirectedEdge* edge, const Cost& cost,
            const float sortcost, const float dist,
            const uint32_t restrictions, const uint32_t opp_local_idx,
            const TravelMode mode);

  /**
   * Constructor with values - used in bidrectional A*.
   * @param predecessor Index into the edge label list for the predecessor
   *                       directed edge in the shortest path.
   * @param edgeid    Directed edge.
   * @param oppedgeid Opposing directed edge Id.
   * @param endnode   End node of the directed edge.
   * @param cost      True cost (cost and elapsed time in seconds) to the edge.
   * @param sortcost  Cost for sorting (includes A* heuristic)
   * @param dist      Distance meters to the destination
   * @param restrictions Restriction mask from prior edge - this is used
   *                  if edge is a transition edge. This allows restrictions
   *                  to be carried across different hierarchy levels.
   * @param mode      Mode of travel along this edge.
   * @param tc        Real transition cost (separated from Cost for the
   *                  reverse path logic.
   */
  EdgeLabel(const uint32_t predecessor, const baldr::GraphId& edgeid,
            const baldr::GraphId& oppedgeid,
            const baldr::DirectedEdge* edge, const Cost& cost,
            const float sortcost, const float dist,
            const uint32_t restrictions, const uint32_t opp_local_idx,
            const TravelMode mode, const uint32_t tc);

  /**
   * Constructor with values.  Used for multi-modal path.
   * @param predecessor Index into the edge label list for the predecessor
   *                       directed edge in the shortest path.
   * @param edgeid    Directed edge.
   * @param endnode   End node of the directed edge.
   * @param cost      True cost (cost and elapsed time in seconds) to the edge.
   * @param sortcost  Cost for sorting (includes A* heuristic)
   * @param dist      Distance meters to the destination
   * @param restrictions Restriction mask from prior edge - this is used
   *                  if edge is a transition edge. This allows restrictions
   *                  to be carried across different hierarchy levels.
   * @param mode      Mode of travel along this edge.
   * @param walking_distance  Accumulated walking distance.
   * @param tripid    Trip Id for a transit edge.
   * @param prior_stopid  Prior transit stop Id.
   * @param blockid   Transit trip block Id.
   * @param has_transit Does the path to this edge have any transit.
   */
  EdgeLabel(const uint32_t predecessor, const baldr::GraphId& edgeid,
            const baldr::DirectedEdge* edge, const Cost& cost,
            const float sortcost, const float dist,
            const uint32_t restrictions, const uint32_t opp_local_idx,
            const TravelMode mode, const uint32_t walking_distance,
            const uint32_t tripid, const uint32_t prior_stopid,
            const uint32_t blockid, const bool has_transit);

  /**
   * Update an existing edge label with new predecessor and cost information.
   * The mode, edge Id, and end node remain the same.
   * @param predecessor Predecessor directed edge in the shortest path.
   * @param cost        True cost (and elapsed time in seconds) to the edge.
   * @param sortcost    Cost for sorting (includes A* heuristic).
   * @param walking_distance  Accumulated walking distance.
   */
  void Update(const uint32_t predecessor, const Cost& cost,
              const float sortcost);

  /**
   * Update an existing edge label with new predecessor and cost information.
   * Update transit information: prior stop Id will stay the same but trip Id
   * and block Id may change (a new trip at an earlier departure time).
   * The mode, edge Id, and end node remain the same.
   * @param predecessor Predecessor directed edge in the shortest path.
   * @param cost        True cost (and elapsed time in seconds) to the edge.
   * @param sortcost    Cost for sorting (includes A* heuristic).
   * @param walking_distance  Accumulated walking distance.
   * @param tripid      Trip Id for a transit edge.
   * @param blockid     Transit trip block Id.
   */
  void Update(const uint32_t predecessor, const Cost& cost,
            const float sortcost, const uint32_t walking_distance,
            const uint32_t tripid, const uint32_t blockid);

  /**
   * Get the predecessor edge label.
   * @return Predecessor edge label.
   */
  uint32_t predecessor() const;

  /**
   * Get the GraphId of this directed edge.
   * @return  Returns the GraphId of this directed edge.
   */
  const baldr::GraphId& edgeid() const;

  /**
   * Get the GraphId of the opposing directed edge.
   * @return  Returns the GraphId of the opposing directed edge.
   */
  const baldr::GraphId& opp_edgeid() const;

  /**
   * Get the end node of this directed edge. Allows the A* algorithm
   * to expand the search from this end node, without having to read
   * the directed edge again.
   * @return  Returns the GraphId of the end node of this directed edge.
   */
  const baldr::GraphId& endnode() const;

  /**
   * Get the cost from the origin to this directed edge.
   * @return  Returns the cost (units are based on the costing method)
   *          and elapsed time (seconds) to the end of the directed edge.
   */
  const Cost& cost() const;

  /**
   * Get the sort cost from the origin to this directed edge. The sort
   * cost includes the A* heuristic.
   * @return  Returns the sort cost (units are based on the costing method).
   */
  float sortcost() const;

  /**
   * Set the sort cost from the origin to this directed edge. The sort
   * cost includes the A* heuristic.
   * @param sortcost Sort cost (units are based on the costing method).
   */
  void SetSortCost(float sortcost);

  /**
   * Get the distance to the destination.
   * @return  Returns the distance in meters.
   */
  float distance() const;

  /**
   * Get the use of the directed edge.
   * @return  Returns edge use.
   */
  baldr::Use use() const;

  /**
   * Get the opposing index - for bidirectional A*.
   * @return  Returns the opposing directed edge index to the incoming edge.
   */
  uint32_t opp_index() const;

  /**
   * Get the opposing local index. This is the index of the incoming edge
   * (on the local hierarchy) at the end node of the predecessor directed
   * edge. This is used for edge transition costs and Uturn detection.
   * @return  Returns the local index of the incoming edge.
   */
  uint32_t opp_local_idx() const;

  /**
   * Get the restriction mask at the end node. Each bit set to 1 indicates a
   * turn restriction onto the directed edge with matching local edge index.
   * @return  Returns the restriction mask.
   */
  uint32_t restrictions() const;

  /**
   * Get the transition up flag.
   * @return  Returns true if the prior edge was a transition up, false if not.
   */
  bool trans_up() const;

  /**
   * Get the transition down flag.
   * @return  Returns true if the prior edge was a transition down,
   *          false if not.
   */
  bool trans_down() const;

  /**
   * Get the shortcut flag.
   * @return  Returns true if the prior edge was a shortcut, false if not.
   */
  bool shortcut() const;

  /**
   * Get the travel mode along this edge.
   * @return  Returns the travel mode.
   */
  TravelMode mode() const;

  /**
   * Get the dest only flag.
   * @return  Returns true if the edge is part of a private or no through road that allows access
   *          only if required to get to a destination?
   */
  bool destonly() const;

  /**
   * Has any transit been taken up to this point on the path.
   * @return  Returns true if any transit has been taken, false if not.
   */
  bool has_transit() const;

  /**
   * Is this edge an origin edge?
   * @return  Returns true if this edge is an origin edge.
   */
  bool origin() const;

  /**
   * Does this edge have a toll?
   * @return  Returns true if this edge has a toll.
   */
  bool toll() const;

  /**
   * Sets this edge as an origin.
   */
  void set_origin();

  /**
   * Get the current walking distance in meters.
   * @return  Returns the current walking distance accumulated since last stop.
   */
  uint32_t walking_distance() const;

  /**
   * Get the transit trip Id.
   * @return   Returns the transit trip Id of the prior edge.
   */
  uint32_t tripid() const;

  /**
   * Get the prior transit stop Id.
   * @return  Returns the prior transit stop Id.
   */
  uint32_t prior_stopid() const;

  /**
   * Return the transit block Id of the prior trip.
   * @return  Returns the block Id.
   */
  uint32_t blockid() const;

  /**
   * Operator < used for sorting.
   */
  bool operator < (const EdgeLabel& other) const;

  /**
   * Get the transition cost in seconds. This is used in the bidirectional A*
   * reverse path search to allow the recovery of the true elapsed time along
   * the path. This is needed since the turn cost is applied at a different
   * node than the forward search.
   * @return  Returns the true transition cost (without penalties) in seconds.
   */
  uint32_t transition_cost() const;

  /**
   * Set the transition cost.
   * @param  tc  True transition cost in seconds.
   */
  void set_transition_cost(uint32_t tc);

 private:
  // Graph Id of the edge.
  baldr::GraphId edgeid_;

  // Graph Id of the opposing edge (for bidirectional A*).
  baldr::GraphId opp_edgeid_;

  // GraphId of the end node of the edge. This allows the expansion
  // to occur by reading the node and not having to re-read the directed
  // edge
  baldr::GraphId endnode_;

  // Cost and elapsed time along the path
  Cost cost_;

  // Sort cost - includes A* heuristic
  float sortcost_;

  // Distance to the destination
  float distance_;

  /**
   * Attributes to carry along with the edge label.
   * walking_distance: Current walking distance.
   * use:           Use of the prior edge.
   * opp_index:     Index at the end node of the opposing directed edge.
   * opp_local_idx: Index at the end node of the opposing local edge. This
   *                value can be compared to the directed edge local_edge_idx
   *                for edge transition costing and Uturn detection.
   * restrictions:  Bit mask of edges (by local edge index at the end node)
   *                that are restricted (simple turn restrictions)
   * trans_up:      Was the prior edge a transition up to a higher level?
   * trans_down:    Was the prior edge a transition down to a lower level?
   * shortcut:      Was the prior edge a shortcut edge?
   * dest_only      Was the prior edge destination only.
   * has_transit    True if any transit taken along the path to this edge.
   * origin         True if this is an origin edge.
   * toll           Edge is toll.
   */
  uint64_t walking_distance_ : 26;
  uint64_t use_              : 6;
  uint64_t opp_index_        : 7;
  uint64_t opp_local_idx_    : 7;
  uint64_t restrictions_     : 7;
  uint64_t trans_up_         : 1;
  uint64_t trans_down_       : 1;
  uint64_t shortcut_         : 1;
  uint64_t mode_             : 4;
  uint64_t dest_only_        : 1;
  uint64_t has_transit_      : 1;
  uint64_t origin_           : 1;
  uint64_t toll_             : 1;

  // Index to the predecessor edge label information.
  uint32_t predecessor_;

  // Transit trip Id
  uint32_t tripid_;

  // Prior stop Id
  uint32_t prior_stopid_;

  // Block Id
  uint32_t blockid_;

  // Real transition cost in seconds (used in bidirectional reverse
  // path search).
  uint32_t transition_cost_;
};

}
}

#endif  // VALHALLA_SIF_EDGELABEL_H_
