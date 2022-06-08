//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#ifdef F2C_DEF_TENSORFLOW

#include "fields2cover/objectives/pred_turn_path_obj.h"

namespace f2c {
namespace obj {

PredTurnPathObj::PredTurnPathObj(
    const std::string& _file_name,
    double _cost_function_perc,
    double _headland_width) : turn_model_(_file_name) {
  if (_cost_function_perc < 0 || _cost_function_perc > 1) {
    throw std::invalid_argument("cost_function_perc should be >=0 and <=1");
  }
  cost_function_perc_ = _cost_function_perc;
  if (_headland_width < 0) {
    throw std::invalid_argument("headland_width should be >=0");
  }
  headland_width_ = _headland_width;
}

double PredTurnPathObj::computeCost(
    const F2CPoint& p1, double ang1,
    const F2CPoint& p2, double ang2) {
  auto turn_input_values = pp::TurningBase::transformToNormalTurn(
      p1, ang1, p2, ang2);
  cppflow::tensor tensor_in = cppflow::tensor(std::vector<float>({
        static_cast<float>(cost_function_perc_),
        static_cast<float>(turn_input_values[0]),
        static_cast<float>(turn_input_values[1]),
        static_cast<float>(turn_input_values[3]),
        static_cast<float>(headland_width_)}), {1, 5});

  cppflow::tensor tensor_out = turn_model_(tensor_in);


  return (1-cost_function_perc_) * tensor_out.get_data<float>()[0] +
    cost_function_perc_ * tensor_out.get_data<float>()[1];
}



}  // namespace obj
}  // namespace f2c

#endif  // F2C_DEF_TENSORFLOW
