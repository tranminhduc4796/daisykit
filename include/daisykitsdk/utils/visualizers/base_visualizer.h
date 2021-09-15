// Copyright 2021 The DaisyKit Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DAISYKIT_UTILS_VISUALIZERS_BASE_VISUALIZER_H_
#define DAISYKIT_UTILS_VISUALIZERS_BASE_VISUALIZER_H_

#include <opencv2/opencv.hpp>

namespace daisykit {
namespace utils {
namespace visualizers {

class BaseVisualizer {
 public:
  static void PutText(cv::Mat& im, const std::string label,
                      const cv::Point& origin,
                      int fontface = cv::FONT_HERSHEY_SIMPLEX,
                      double scale = 0.8, int thickness = 1, int baseline = 0,
                      cv::Scalar text_color = cv::Scalar(255, 255, 255),
                      cv::Scalar bg_color = cv::Scalar(255, 0, 0));

  template <typename T>
  static cv::Mat LineGraph(std::vector<T>& vals, int YRange[2],
                           std::vector<int> signals, int n_last_values = -1) {
    auto begin_elm = n_last_values == -1 ? vals.begin()
                                         : vals.size() > n_last_values
                                               ? vals.end() - n_last_values
                                               : vals.begin();
    auto it = minmax_element(begin_elm, vals.end());
    float scale = 1. / ceil(*it.second - *it.first);
    float bias = *it.first;
    int rows = YRange[1] - YRange[0] + 1;

    int graph_y_size = n_last_values != -1
                           ? std::min(n_last_values, (int)vals.size())
                           : vals.size();
    cv::Mat image = cv::Mat::zeros(rows, graph_y_size, CV_8UC3);
    image.setTo(0);

    int k = 0;
    if (n_last_values != -1) {
      if (vals.size() > n_last_values) {
        k = vals.size() - n_last_values;
      }
    }

    for (int i = k; i < (int)vals.size() - 1; i++) {
      cv::line(
          image,
          cv::Point(i - k, rows - 1 - (vals[i] - bias) * scale * YRange[1]),
          cv::Point(i + 1 - k,
                    rows - 1 - (vals[i + 1] - bias) * scale * YRange[1]),
          cv::Scalar(255, 0, 0), 1);
    }

    for (int i = k; i < (int)signals.size() - 1; i++) {
      if (signals[i] == 0) {
        continue;
      }
      int y = signals[i] == 1 ? (int)(rows / 2) - 20 : (int)(rows / 2) + 20;
      cv::circle(image, cv::Point(i - k, y), 2, cv::Scalar(0, 0, 255),
                 cv::FILLED, cv::LINE_8);
    }

    return image;
  }
};

}  // namespace visualizers
}  // namespace utils
}  // namespace daisykit

#endif