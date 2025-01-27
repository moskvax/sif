#ifndef VALHALLA_SIF_COSTFACTORY_H_
#define VALHALLA_SIF_COSTFACTORY_H_

#include <map>
#include <memory>

#include <valhalla/sif/autocost.h>
#include <valhalla/sif/bicyclecost.h>
#include <valhalla/sif/pedestriancost.h>
#include <valhalla/sif/truckcost.h>
#include <valhalla/sif/transitcost.h>

namespace valhalla {
namespace sif {

/**
* Generic factory class for creating objects based on type name.
*/
template <class cost_t>
class CostFactory {
 public:
  typedef std::shared_ptr<cost_t> cost_ptr_t;
  //TODO: might want to have some configurable params to each cost type
  typedef cost_ptr_t (*factory_function_t)(const boost::property_tree::ptree& config);

  /**
   * Constructor
   */
  CostFactory()
    : factory_funcs_() {
  }

  /**
   * Register the callback to create this type of cost
   *
   * @param name       the name of the cost that the function creates
   * @param function   the function pointer to call to actually create the cost object
   */
  void Register(const std::string& name, factory_function_t function) {
    factory_funcs_.emplace(name, function);
  }

  /**
   * Make a cost from its name
   * @param name    the name of the cost to create
   * @param config  Property tree with configuration / cost options
   */
  cost_ptr_t Create(const std::string& name,
                    const boost::property_tree::ptree& config) const {
    auto itr = factory_funcs_.find(name);
    if (itr == factory_funcs_.end()) {
      throw std::runtime_error("Please provide a valid costing: " + name);
    }
    //create the cost using the function pointer
    return itr->second(config);
  }

 private:
  std::map<std::string, factory_function_t> factory_funcs_;
};

}
}

#endif // VALHALLA_SIF_COSTFACTORY_H_
