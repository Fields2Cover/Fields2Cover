/*
 * spline.h
 *
 *  The original version of this file can be found at with:
 *  https://github.com/joshhooker/CubicSplineClass
 *
 *  This file has MIT license.
 *
 *  Actual version is modified to adapt to Fields2Cover requirements.
 *
 * ---------------------------------------------------------------------
 *  Copyright (c) 2017 Joshua Hooker
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 * ---------------------------------------------------------------------
 *
 */


#ifndef FIELDS2COVER_UTILS_SPLINE_H_
#define FIELDS2COVER_UTILS_SPLINE_H_

#include <vector>
#include <string>

namespace f2c {

class CubicSpline {
 public:
  CubicSpline();
  CubicSpline(const std::vector<double> &x, const std::vector<double> &y,
      bool monotonic = false);

  ~CubicSpline();

  void SetPoints(const std::vector<double> &x, const std::vector<double> &y,
      bool monotonic = false);

  double operator()(double x) const;


 private:
  size_t size_{0};
  std::vector<double> x_vec_, y_vec_;
  std::vector<double> b_vec_, c_vec_, d_vec_;

  bool monotonic_{false};

  void SetSpline();

  void SetSplineCubic();

  void check_error(bool cond, const std::string& msg) const;
};

}  // namespace f2c

#endif  // FIELDS2COVER_UTILS_SPLINE_H_
