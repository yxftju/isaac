#ifndef ISAAC_BACKEND_TEMPLATES_MDOT_H
#define ISAAC_BACKEND_TEMPLATES_MDOT_H

#include <vector>

#include "isaac/symbolic/expression.h"
#include "isaac/kernels/templates/base.h"

namespace isaac
{
namespace templates
{
struct reduce_2d_parameters : public base::parameters_type
{
  reduce_2d_parameters(unsigned int _simd_width,
                                unsigned int _local_size_0, unsigned int _local_size_1,
                                unsigned int _num_groups_0, unsigned int _num_groups_1, fetching_policy_type _fetch_policy);
  unsigned int num_groups_0;
  unsigned int num_groups_1;
  fetching_policy_type fetch_policy;
};


class reduce_2d : public base_impl<reduce_2d, reduce_2d_parameters>
{
protected:
  enum reduce_1d_type
  {
    REDUCE_ROWS,
    REDUCE_COLUMNS
  };
  reduce_2d(reduce_2d::parameters_type const & , reduce_1d_type, binding_policy_t);
private:
  int is_invalid_impl(driver::Device const &, expression_tree const &) const;
  unsigned int lmem_usage(expression_tree const &) const;
  unsigned int temporary_workspace(expression_tree const & expressions) const;
  std::string generate_impl(std::string const & suffix, expression_tree const &, driver::Device const & device, mapping_type const &) const;
public:
  virtual std::vector<int_t> input_sizes(expression_tree const & expressions) const;
  void enqueue(driver::CommandQueue & queue, driver::Program const & program, std::string const & suffix, base & fallback, execution_handler const &);
private:
  reduce_1d_type reduce_1d_type_;
};

class reduce_2d_rows : public reduce_2d
{
public:
  reduce_2d_rows(reduce_2d::parameters_type  const &, binding_policy_t binding_policy = BIND_INDEPENDENT);
  reduce_2d_rows(unsigned int simd, unsigned int ls1, unsigned int ls2, unsigned int ng1, unsigned int ng2, fetching_policy_type fetch, binding_policy_t bind = BIND_INDEPENDENT);
};

class reduce_2d_cols : public reduce_2d
{
public:
  reduce_2d_cols(reduce_2d::parameters_type  const &, binding_policy_t binding_policy = BIND_INDEPENDENT);
  reduce_2d_cols(unsigned int simd, unsigned int ls1, unsigned int ls2, unsigned int ng1, unsigned int ng2, fetching_policy_type fetch, binding_policy_t bind = BIND_INDEPENDENT);
};

}
}

#endif