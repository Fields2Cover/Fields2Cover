
.. _program_listing_file_fields2cover_utils_visualizer.h:

Program Listing for File visualizer.h
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_fields2cover_utils_visualizer.h>` (``fields2cover/utils/visualizer.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //=============================================================================
   //    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
   //                     Author: Gonzalo Mier
   //                           BSD-3 License
   //=============================================================================
   
   #pragma once
   #ifndef FIELDS2COVER_UTILS_VISUALIZER_H_
   #define FIELDS2COVER_UTILS_VISUALIZER_H_
   
   #ifdef F2C_DEF_MATPLOTLIB
   
   #include <vector>
   #include <string>
   #include "fields2cover/types.h"
   
   namespace f2c {
   
   class Visualizer {
    public:
     static void plot(const std::vector<F2CPoint>& _points,
         const std::string& _opts = "");
     static void plot(const F2CPath& _path);
     static void plot(const F2CMultiLineString& _lines);
     static void plot(const F2CCell& _cell);
     static void plot(const F2CCells& _cells);
     static void plot(const F2CSwaths& _swaths);
     static void plot(const F2CSwath& _swath, std::string opt = "");
     static void plot(const F2CField& _field);
     static void plot(const F2CFields& _fields);
     static void plot(const F2CRobot& _robot);
     static void plot(const F2CRobots& _robots);
   
     static void plot(const std::vector<double>& _t,
         const std::vector<double>& _d, const std::string& _opts = "");
     static void plot(
         const std::vector<double>& _d, const std::string& _opts = "");
   
     template<class T>
     static void plot(const T& _t, const std::string& _opts);
   
     template<class T>
     static void plot(const std::vector<T>& _t);
   
     template<class T>
     static void plot(const T& _t);
   
     static void figure(void);
   
     static void figure(int _id);
   
     static void show(bool _block = true);
   
     static void save(const std::string& _file);
   
    private:
     template<class T>
     static std::vector<F2CPoint> data2vector(const T& _t);
   
     static std::vector<F2CPoint> data2vector(const F2CPoint& _t);
   
     static std::vector<std::vector<double>> getComponents(
         const std::vector<F2CPoint>& _points);
   
     static std::vector<double> linspace(double min, double max, size_t N);
     static std::string color2hex(uint32_t r, uint32_t g, uint32_t b);
     static std::vector<std::string> color_linspace(
         const std::vector<int>& min,
         const std::vector<int>& max,
         size_t N);
   };
   
   template<class T>
   std::vector<F2CPoint> Visualizer::data2vector(const T& _t) {
     std::vector<F2CPoint> res;
     for (auto&& i : _t) {
       auto v = data2vector(i);
       res.insert(res.end(), v.begin(), v.end());
     }
     return res;
   }
   
   template<class T>
   void Visualizer::plot(const std::vector<T>& _t) {
     for (auto&& t : _t) {
       plot(t);
     }
   }
   
   template<class T>
   void Visualizer::plot(const T& _t, const std::string& _opts) {
     plot(data2vector(_t), _opts);
   }
   
   template<class T>
   void Visualizer::plot(const T& _t) {
     plot(_t, "");
   }
   
   }  // namespace f2c
   
   
   #endif  // F2C_DEF_MATPLOTLIB
   #endif  // FIELDS2COVER_UTILS_VISUALIZER_H_
